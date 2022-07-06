//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file Ps2Kbd.c
    PS/2 keyboard support routines

**/
//---------------------------------------------------------------------------

#include "Ps2Ctl.h"
#include "Kbc.h"
#include "Ps2CtlOem.h"

#include "Library/BaseMemoryLib.h"
#include <Protocol/LegacyBiosExt.h>
#include "Protocol/HiiDatabase.h"
#include "Library/UefiBootServicesTableLib.h"

/// Definition of the GUID used for describing the Japanese keyboard layout
#define EFI_KEYBOARD_JAPANESE_411_GUID \
    {0xecd17560, 0x7f6e, 0x4277, 0xa0, 0x1a, 0x1b, 0x4c, 0xff, 0xdb, 0xa0, 0x40}

static BOOLEAN   JapaneseKbdDetected = FALSE;
static BOOLEAN   KeyboardLayoutFlag = TRUE;

#define     E0_STATUS_IN_BDA        BIT1
#define     E1_STATUS_IN_BDA        BIT0
#define     LEGACY_8259_CONTROL_REGISTER_MASTER 0x20

#if CHECK_BDA_KEYBOARD_BUFFER
#if CSM_SUPPORT
#include <Protocol/LegacyBios.h>
UINT16                      BdaSeg      = 0x400;
UINT16                      *BdaKbdHead = (UINT16 *)0x41A;
UINT16                      *BdaKbdTail = (UINT16 *)0x41C;
EFI_LEGACY_BIOS_PROTOCOL    *gLegacy = NULL;
BOOLEAN                     gLegacyBiosProtocolFound = FALSE;
#endif
#endif

extern  BOOLEAN InsideGetMouseData;
extern  BOOLEAN KBDEnableState;
extern  UINT8   LedsAtStartup;
extern  BOOLEAN InsideMouseReset;
extern  BOOLEAN ReSendLEDCommand;
BOOLEAN InsideKbdReadKey = FALSE;
BOOLEAN InsideOnWaitingOnKey = FALSE;
BOOLEAN InsideKbdReset = FALSE;
extern BOOLEAN InsidePS2DataDispatcher;
AMI_MULTI_LANG_SUPPORT_PROTOCOL *gPs2MultiLangSupportProtocol = NULL;
BOOLEAN                         NumLock2APrecede = FALSE;


#if CLEAR_PENDING_KEYS_IN_PS2
VOID Ps2KbdReset ( VOID );
VOID ClearOBF ( VOID );
#endif

typedef struct _KEY_WAITING_RECORD{
  DLINK                     Link;
  EFI_KEY_DATA              Context;
  EFI_KEY_NOTIFY_FUNCTION   Callback;
  BOOLEAN                   CallbackStatus;
  EFI_HANDLE                NotifyEvent;
  EFI_KEY_DATA              KeyPressed;
} KEY_WAITING_RECORD;

DLIST                       mPs2KeyboardData;
EFI_EVENT                   Ps2KeyEvent;

extern STATEMACHINEPROC     DrivePS2KbdMachine;
extern STATEMACHINEPROC     DrivePS2MouseMachine;


KEYBOARD                    gKbd;
KEYBOARD_IRQ_STORE          gKeyboardIrqBuffer;
EFI_CPU_ARCH_PROTOCOL       *gCpuArch;
EFI_LEGACY_8259_PROTOCOL    *mLegacy8259;
BOOLEAN                     gKeyboardIrqInstall=FALSE;
BOOLEAN                     gKeyboardDriverStart=FALSE;

extern	BOOLEAN             Ps2MouseDetected;
extern  BOOLEAN             KbdMsIrqSupport;
extern  BOOLEAN             KbRdBeforeInstall; 
extern  BOOLEAN             InsideGetMouseData;
extern  BOOLEAN             KBDEnableState;

typedef EFI_STATUS (PS2CTL_HANDLEDATA_OEM_HOOK)( 
    KEYBOARD    *Kbd,
    UINT8       data
);

// ELink functions to execute before HandleKbdData function called 
extern PS2CTL_HANDLEDATA_OEM_HOOK Ps2CtlHandleDataOemHooksList EndOfPs2CtlHandleDataOemFunction;
PS2CTL_HANDLEDATA_OEM_HOOK *Ps2CtlHandleDataOemFunction[] = {
        Ps2CtlHandleDataOemHooksList NULL
};


/*
Table 2. PS/2 Scan Codes Supported in Windows 2000/Windows XP and Windows Me

HID usage/ key name  Set 1Make  Set 1Break  Set 2Make  Set 2Break
 Scan Next Track       E0 19       E0 99       E0 4D     E0 F0 4D
 Scan Previous Track   E0 10       E0 90       E0 15     E0 F0 15
 Stop                  E0 24       E0 A4       E0 3B     E0 F0 3B
 Play/ Pause           E0 22       E0 A2       E0 34     E0 F0 34
 Mute                  E0 20       E0 A0       E0 23     E0 F0 23
 Volume Increment      E0 30       E0 B0       E0 32     E0 F0 32
 Volume Decrement      E0 2E       E0 AE       E0 21     E0 F0 21
 AL Email Reader       E0 6C       E0 EC       E0 48     E0 F0 48
 AC Search             E0 65       E0 E5       E0 10     E0 F0 10
 AC Home               E0 32       E0 B2       E0 3A     E0 F0 3A
 AC Back               E0 6A       E0 EA       E0 38     E0 F0 38
 AC Forward            E0 69       E0 E9       E0 30     E0 F0 30
 AC Stop               E0 68       E0 E8       E0 28     E0 F0 28
 AC Refresh            E0 67       E0 E7       E0 20     E0 F0 20
 AC Bookmarks          E0 66       E0 E6       E0 18     E0 F0 18
 AL Calculator*        E0 21       E0 A1       E0 2B     E0 F0 2B
 AL Local Browser*     E0 6B       E0 EB       E0 40     E0 F0 40
 AL Consumer Control Configuration*
                       E0 6D       E0 ED       E0 50     E0 F0 50
*/
BOOLEAN IsEnhancedKey( UINT8 data)
{
    UINTN   i;

    data &= 0x7F;   // reset MSB
    for ( i = 0; i < sizeof(E0EnhancedKeys); i++) {
        if ( data == E0EnhancedKeys[i]) {
            return  TRUE;
        }
    }
    return  FALSE;
}


#if CLEAR_PS2KB_BUFFER_AT_READYTOBOOT
EFI_EVENT	gClearKbBufferEvent;

/** 
    This function clear PS2 KB buffer

           
    @param Event   - signaled event
    @param Context - pointer to event context

         
    @retval VOID
 
**/

EFI_STATUS 
EFIAPI 
ClearKbBuffer (
    IN EFI_EVENT    Event, 
    IN VOID         *Context)
{
    KEYBOARD    *Kbd = &gKbd;
    UINT8       *BdaKbHead = (UINT8*)((UINTN) 0x41a);
    UINT8       *BdaKbTail = (UINT8*)((UINTN) 0x41c);
    UINT8       *BdaKbBuffer = (UINT8*)((UINTN) 0x41e);

    Kbd->pBufHead = Kbd->pBufStart;
    Kbd->pBufTail = Kbd->pBufStart;

    pBS->SetMem(Kbd->pBufStart, BUFFER_SIZE * sizeof (AMI_EFI_KEY_DATA), 0);

    // Empty KB Buffer in BDA
    pBS->SetMem(BdaKbBuffer, 32, 0);
    *BdaKbTail = *BdaKbHead;

    pBS->CloseEvent(Event);

    return EFI_SUCCESS;
}
#endif


/**
    Clears any pending keys in Ps2 Output buffer. 

        
    @param    VOID

           
    @retval    VOID
  
    @note  
              This OBF clear is done since some PS2 KBC has pending keys 
			  before booting to Legacy OS
**/
VOID
EFIAPI
DisableKbdCallback (     
	IN EFI_EVENT    Event, 
	IN VOID         *Context)
{
    UINT8 Counter = 100;
      
    // Disable the Keyboard and Keyboard Interrupt.
    Write8042CommandByte(0x74);
     
    //Clears all the data pending in internal keyboard buffer
    while( (IoRead8(KBC_CMDSTS_PORT) & KBC_OBF) && (Counter))
    {
        IoRead8(KBC_DATA_PORT);
		Counter--;          
    }
    return; 
}


/**
    This function updates variable Legacy BIOS Protocol found,
    to check BDA for pending keys in EFI.
       
    @param    Event   -  signaled event
    @param    Context -  pointer to event context

    @retval   VOID
 
**/
#if CHECK_BDA_KEYBOARD_BUFFER
#if CSM_SUPPORT
VOID 
EFIAPI 
UpdateVariabletoCheckBda (
    IN EFI_EVENT    Event,
    IN VOID         *Context)
{
    EFI_STATUS Status;

    // Update Variable to check BDA if Legacy Bios Protocol Found.
    Status = pBS->LocateProtocol(&gEfiLegacyBiosProtocolGuid, NULL, (VOID**)&gLegacy);        
    if (!EFI_ERROR(Status)) {
        gLegacyBiosProtocolFound = TRUE;
    }
}
#endif
#endif

/** @internal
    This function checks whether the current keyboard layout is Japanese layout 
    @param    VOID
    
    @retval   VOID
    @endinternal
**/
VOID 
FindJapaneseKbd()
{
    EFI_STATUS                 Status;
    EFI_HII_DATABASE_PROTOCOL  *HiiDatabase = NULL;
    EFI_GUID                   HiiDatabaseGuid = EFI_HII_DATABASE_PROTOCOL_GUID;
    UINT16                     KeyboardLength = sizeof(EFI_HII_KEYBOARD_LAYOUT);
    EFI_HII_KEYBOARD_LAYOUT    *Keyboard = NULL;
    EFI_GUID                   KeyGuid;
    EFI_GUID                   JapaneseGuid = EFI_KEYBOARD_JAPANESE_411_GUID;
    
    Status = gBS->LocateProtocol(&HiiDatabaseGuid, NULL, (VOID**)&HiiDatabase);
    if(EFI_ERROR(Status)){
        return;
    }
    Status = gBS->AllocatePool(EfiBootServicesData, KeyboardLength, (VOID**)&Keyboard);

    if(!EFI_ERROR(Status)){
        Status = HiiDatabase->GetKeyboardLayout(HiiDatabase, NULL, &KeyboardLength, Keyboard);

        if (Status == EFI_NOT_FOUND || Status == EFI_INVALID_PARAMETER) {
            return;
        }
        if (Status == EFI_BUFFER_TOO_SMALL) {
            if(Keyboard != NULL)
                pBS->FreePool(Keyboard);

            Status = gBS->AllocatePool(EfiBootServicesData, KeyboardLength, (VOID**)&Keyboard);
            if (!EFI_ERROR(Status)){
                HiiDatabase->GetKeyboardLayout(HiiDatabase, NULL, &KeyboardLength, Keyboard);
            } else {
                return;
            }    
        }
        KeyGuid = Keyboard->Guid;
        KeyboardLayoutFlag = FALSE;
        
        if (CompareGuid (&KeyGuid, &JapaneseGuid)){
            JapaneseKbdDetected = TRUE;
        } else {
            JapaneseKbdDetected = FALSE;
        }
    }

    pBS->FreePool(Keyboard);
}

/**
    This routine is called from Driver Binding Start function,
    it starts the keyboard

    @param    *This      - A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance
              Controller - Handle for this controller

    @retval    EFI_STATUS status of the operation

**/

EFI_STATUS 
EFIAPI 
StartKeyboard (
    EFI_DRIVER_BINDING_PROTOCOL     *This,
    EFI_HANDLE                      Controller)
{

    EFI_STATUS                  Status;
    EFI_DEVICE_PATH_PROTOCOL    *pDummyDevPath;
    KEYBOARD                    *Kbd = &gKbd;
    KEYBOARD_IRQ_STORE          *KbIrqBuffer = &gKeyboardIrqBuffer;
    UINT8                       Index = 0;
    EFI_EVENT                   DisableKbdEvent;
    VOID                        *Registration;
    
#if CHECK_BDA_KEYBOARD_BUFFER
#if CSM_SUPPORT
    EFI_EVENT                   Event;
    VOID                        *pRegistration;
#endif
#endif

    if (EFI_ERROR(gSysTable->BootServices->OpenProtocol(
            Controller,
            &gEfiDevicePathProtocolGuid,
            (VOID**)&pDummyDevPath,
            This->DriverBindingHandle,
            Controller,
            EFI_OPEN_PROTOCOL_BY_DRIVER))) {
        return EFI_INVALID_PARAMETER;
    }

#if CLEAR_PENDING_KEYS_IN_PS2
    // Reset the keyboard and Clear Pending Keys as some
    // NoteBook Kbc needs reset and Clearing the OBF.
    Ps2KbdReset();
    ClearOBF ();
#endif
	PROGRESS_CODE(DXE_KEYBOARD_INIT);

    DetectPS2KeyboardAndMouse();

    if (EFI_ERROR(DetectPS2Keyboard())) {
        gSysTable->BootServices->CloseProtocol(
            Controller,
            &gEfiDevicePathProtocolGuid,
            This->DriverBindingHandle,
            Controller);
        // Don't report EFI_ERROR_MAJOR as if keyboard not present also error is reported
        return EFI_DEVICE_ERROR;
    }

    // Initialize keyboard interface functions
    (Kbd->iSimpleIn).Reset = KbdReset;
    (Kbd->iSimpleIn).ReadKeyStroke = KbdReadKey;

    (Kbd->iSimpleInEx).Reset = KbdResetEx;
    (Kbd->iSimpleInEx).ReadKeyStrokeEx = KbdReadKeyEx;
    (Kbd->iSimpleInEx).SetState = SetState;
    (Kbd->iSimpleInEx).RegisterKeyNotify = RegisterKeyNotify;
    (Kbd->iSimpleInEx).UnregisterKeyNotify = UnRegisterKeyNotify;

    (Kbd->iKeycodeInEx).Reset = KbdResetEx;
    (Kbd->iKeycodeInEx).ReadEfikey = KbdReadEfiKeyEx;
    (Kbd->iKeycodeInEx).SetState = SetState;
    (Kbd->iKeycodeInEx).RegisterKeyNotify = RegisterKeyNotify;
    (Kbd->iKeycodeInEx).UnregisterKeyNotify = UnRegisterKeyNotify;

    Status = gSysTable->BootServices->CreateEvent(
        EVT_NOTIFY_WAIT,
        TPL_NOTIFY,
        OnWaitingOnKey,
        Kbd,
        &Kbd->iSimpleIn.WaitForKey);
    ASSERT_EFI_ERROR(Status);

    Status = gSysTable->BootServices->CreateEvent(
        EVT_NOTIFY_WAIT,
        TPL_NOTIFY,
        OnWaitingOnKey,
        Kbd,
        &Kbd->iSimpleInEx.WaitForKeyEx);
    ASSERT_EFI_ERROR(Status);

    Status = gSysTable->BootServices->CreateEvent(
        EVT_NOTIFY_WAIT,
        TPL_NOTIFY,
        OnWaitingOnKey,
        Kbd,
        &Kbd->iKeycodeInEx.WaitForKeyEx);
    ASSERT_EFI_ERROR(Status);

    // Install protocol interfaces for the keyboard device.
    Status = gSysTable->BootServices->InstallMultipleProtocolInterfaces (
        &Controller,
        &gEfiSimpleTextInProtocolGuid, &Kbd->iSimpleIn,
        &gEfiSimpleTextInputExProtocolGuid, &Kbd->iSimpleInEx,
        &gAmiEfiKeycodeProtocolGuid, &Kbd->iKeycodeInEx,
        NULL
    );

    if (EFI_ERROR(Status)) {
        gSysTable->BootServices->CloseProtocol(
            Controller,
            &gEfiDevicePathProtocolGuid,
            This->DriverBindingHandle,
            Controller);

        gSysTable->BootServices->CloseEvent(Kbd->iSimpleIn.WaitForKey);
        gSysTable->BootServices->CloseEvent(Kbd->iSimpleInEx.WaitForKeyEx);
        gSysTable->BootServices->CloseEvent(Kbd->iKeycodeInEx.WaitForKeyEx);
        
        ERROR_CODE (DXE_KEYBOARD_CONTROLLER_ERROR, EFI_ERROR_MAJOR);
    }

    if (!(EFI_ERROR(Status))) {
        // Initialize keyboard device
        Kbd->KeyIsReady = FALSE;
        Kbd->ScannerState = KBST_READY;
        Kbd->KeyData.KeyState.KeyToggleState = LedsAtStartup;
        Kbd->KeyData.KeyState.KeyToggleState |= TOGGLE_STATE_VALID;
        Kbd->KeyData.KeyState.KeyShiftState = SHIFT_STATE_VALID;

        Kbd->pBufStart = MallocZ(BUFFER_SIZE * sizeof (AMI_EFI_KEY_DATA));
        if(!Kbd->pBufStart) return EFI_OUT_OF_RESOURCES;
        Kbd->pBufHead = Kbd->pBufStart;
        Kbd->pBufTail = Kbd->pBufStart;
        Kbd->pBufEnd = Kbd->pBufStart + BUFFER_SIZE;

        DrivePS2KbdMachine = ReadAndProcessKey;

        // Set LED's
        LEDsOnOff(Kbd);

//      for (Count = 3; Count; Count--) {
//          if(IssueCommand(0xF4) == 0xFA) break;       // Clear the KBD buffer
//      }

        Kbd->LEDCommandState = 0;

#if CLEAR_PS2KB_BUFFER_AT_READYTOBOOT
        CreateReadyToBootEvent(TPL_NOTIFY, ClearKbBuffer, NULL, &gClearKbBufferEvent);
#endif       
        RegisterProtocolCallback( &gAmiLegacyBootProtocolGuid,
                                  DisableKbdCallback,
                                  NULL, 
                                  &DisableKbdEvent, 
                                  &Registration
                                  );
    
#if CHECK_BDA_KEYBOARD_BUFFER
#if CSM_SUPPORT
        RegisterProtocolCallback(
        &gEfiLegacyBiosProtocolGuid, UpdateVariabletoCheckBda,
        NULL, &Event,&pRegistration
        );

        // Check whether Legacy BIOS Protocol Installed.
        UpdateVariabletoCheckBda(NULL,NULL);

#if CLEAR_PENDING_KEYS_IN_PS2        
        if( gLegacyBiosProtocolFound ) {
            // Clear the Key's proceed by Legacy Int9 handler
            if( *BdaKbdHead != *BdaKbdTail ) {
                *BdaKbdHead = *BdaKbdTail;
            }
        }        
#endif
        
#endif
#endif
    }

    DListInit(&mPs2KeyboardData);

    Status = pBS->CreateEvent(
                    EVT_TIMER | EFI_EVENT_NOTIFY_SIGNAL,
                    TPL_NOTIFY,
                    StartPollingKey,
                    Kbd,
                    &Ps2KeyEvent
    );

    // if before keyboard driver starts data is available in buffer
    // it will be collected in local buffer in KeyboardInterrupt Handler
    // only if KBD_READ_BEFORE_INSTALL = 1, the available data in local 
    // buffer is processed.
    if(KbRdBeforeInstall){
        for (Index=0;Index < KbIrqBuffer->KbdIndex; Index++){
            ProcessKBDData(Kbd, KbIrqBuffer->KbdBuffer[Index]);
        }
    }
    gKeyboardDriverStart = TRUE;
    // Re enable keyboard irq as it was previously disabled.
    // if KBD_READ_BEFORE_INSTALL = 0 then initialize keyboard irq here
    if(KbdMsIrqSupport){
        if(KbRdBeforeInstall){   
            gKeyboardIrqInstall = TRUE;
            // Now enable the interrupt
            mLegacy8259->EnableIrq(mLegacy8259, SYSTEM_KEYBOARD_IRQ, FALSE);
        }
        else {
            InitKeyboardIrq();
        }
    }
    // Enable the Keyboard and Keyboard Interrupt. We must initialize this
    // one for the Keyboard to work on Legacy mode. And also Legacy mode AMIUSB driver
    // expects Keyboard and Interrupt should be enabled once Port 60/64 is present.
    Write8042CommandByte(0x65);
    return Status;
}

/**
    This routine is called from Driver Binding Stop function.

    @param    *This      - A pointer to the
              Controller - Handle for this controller

    @retval    EFI_STATUS status of the operation

**/

EFI_STATUS 
EFIAPI 
StopKeyboard (
    EFI_DRIVER_BINDING_PROTOCOL     *This,
    EFI_HANDLE                      Controller)
{
    KEYBOARD    *Kbd = &gKbd;
    EFI_STATUS  Status;

    // Uninstall protocol interfaces from the keyboard device.
    Status = gSysTable->BootServices->UninstallMultipleProtocolInterfaces (
        Controller,
        &gEfiSimpleTextInProtocolGuid, &Kbd->iSimpleIn,
        &gEfiSimpleTextInputExProtocolGuid, &Kbd->iSimpleInEx,
        &gAmiEfiKeycodeProtocolGuid, &Kbd->iKeycodeInEx,
        NULL
    );

    if (EFI_ERROR(Status)) {
        return Status;
    }

    // Close protocols that is open during Start
    Status = gSysTable->BootServices->CloseProtocol(
        Controller,
        &gEfiDevicePathProtocolGuid,
        This->DriverBindingHandle,
        Controller);

    if (EFI_ERROR(Status)) {
        return Status;
    }

    // Kill wait event
    gSysTable->BootServices->CloseEvent(Kbd->iSimpleIn.WaitForKey);
    gSysTable->BootServices->CloseEvent(Kbd->iSimpleInEx.WaitForKeyEx);
    gSysTable->BootServices->CloseEvent(Kbd->iKeycodeInEx.WaitForKeyEx);
    gSysTable->BootServices->CloseEvent(Ps2KeyEvent);

    pBS->FreePool(Kbd->pBufStart);


    return Status;
}


/**
    Resets the input device hardware. This routine is a part
    of SimpleTextIn protocol implementation.

    @param    *This                - A pointer to the EFI_SIMPLE_TEXT_INPUT_PROTOCOL 
                                     instance.
              ExtendedVerification - Indicates that the driver may perform a more 
                                     exhaustive verification operation of the
                                     device during reset.

    @retval    EFI_SUCCESS The device was reset.
               EFI_DEVICE_ERROR The device is not functioning correctly
               and could not be reset.

**/

EFI_STATUS 
EFIAPI 
KbdReset (
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *This,
    BOOLEAN                         ExtendedVerification )
{
    UINT8       bCount, bData, bBCount;
    KEYBOARD    *Kbd = &gKbd;
    EFI_STATUS  Status;

    InsideKbdReset = TRUE;
    Kbd->KeyIsReady = FALSE;
    Kbd->ScannerState = KBST_READY;

    Kbd->pBufHead = Kbd->pBufStart;
    Kbd->pBufTail = Kbd->pBufStart;

    pBS->SetMem(Kbd->pBufStart, BUFFER_SIZE * sizeof (AMI_EFI_KEY_DATA), 0);

    // If the ExtendedVerification is TRUE then proceed to reset the 
    // KBC Controller and devices. Otherwise return from here
    if(!ExtendedVerification) {
        InsideKbdReset = FALSE;
        return EFI_SUCCESS;
    }
    
    // Check for keyboard IRQ support
    if(KbdMsIrqSupport && gKeyboardIrqInstall) {
        
        // Now Disable the interrupt
        mLegacy8259->DisableIrq(mLegacy8259, SYSTEM_KEYBOARD_IRQ);
    }

    DisableKeyboard();
    PROGRESS_CODE(DXE_KEYBOARD_RESET);
  
    for (bBCount = 0; bBCount < 4; bBCount++) {
        for (bCount = 0; bCount < 3; bCount++) {
            Status = ReadDevice(KBD_DISABLE_SCANNING, &bData, KB_ACK_COM);
            if (!EFI_ERROR(Status)) {
                break;
            }
        }
        if (EFI_ERROR(Status)) {
            continue;
        }
        for (bCount = 0; bCount < 3; bCount++) {
            Status = ReadDevice(KBD_ENABLE_SCANNING, &bData, KB_ACK_COM);
            if (!EFI_ERROR(Status)) {
                break;
            }
        }
        if (!EFI_ERROR(Status)) {
            break;
        }
    }
    
    // Report the Buffer FULL error code if there is failure.
    if (EFI_ERROR(Status)) {
        ERROR_CODE (DXE_KEYBOARD_BUFFER_FULL_ERROR, EFI_ERROR_MAJOR);
    }
    
    KBDEnableState = FALSE;
    Status = EnableKeyboard();
    
    if (EFI_ERROR(Status)) {
        
        // Report the KBC Controller error code.
        ERROR_CODE (DXE_KEYBOARD_CONTROLLER_ERROR, EFI_ERROR_MAJOR);
    }

#if CLEAR_PENDING_KEYS_IN_PS2
    // Clear OBF as some Notebook KBC has some keys even after
    // disabling and enabling scanning.
    ClearOBF ();
#endif

    if(!gKeyboardIrqInstall){
        LEDsOnOff(Kbd);
    }

    Kbd->LEDCommandState = 0;
    InsideKbdReset = FALSE;

    if(KbdMsIrqSupport && gKeyboardIrqInstall) {
        // Now enable the interrupt
        mLegacy8259->EnableIrq(mLegacy8259, SYSTEM_KEYBOARD_IRQ, FALSE);
    }

    return EFI_SUCCESS;

}

/**
    Resets the input device hardware. This routine is a part
    of SimpleTextInEx protocol implementation.

    @param    *This                - A pointer to the EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL
                                     instance.
              ExtendedVerification - Indicates that the driver may perform 
                                     a more exhaustive verification operation of the
                                     device during reset.

    @retval    EFI_SUCCESS The device was reset.
               EFI_DEVICE_ERROR The device is not functioning correctly
               and could not be reset.

**/
EFI_STATUS 
EFIAPI 
KbdResetEx (
    EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL  *This,
    BOOLEAN                            ExtendedVerification )
{
    return KbdReset(0, ExtendedVerification);
}


/**
    Reads the next keystroke from the input device. This
    routine is a part of SimpleTextIn protocol
    implementation.

    @param    This    - A pointer to the EFI_SIMPLE_TEXT_INPUT_PROTOCOL
                        instance.
              InputKey -  A pointer to a buffer that is to be filled in
                          with the keystroke information for the key 
                          that was pressed.

    @retval    EFI_SUCCESS The keystroke information was returned.
    @retval    EFI_NOT_READY There was no keystroke data available.
    @retval    EFI_DEVICE_ERROR The keystroke information was not
               returned due to hardware errors.

**/

EFI_STATUS 
EFIAPI 
KbdReadKey (
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *This,
    EFI_INPUT_KEY                   *InputKey )
{
    KEYBOARD    *Kbd = &gKbd;

    if (InsideGetMouseData || InsideKbdReadKey || InsideOnWaitingOnKey || InsideKbdReset || InsideMouseReset) {
        return GetKeyFromBuffer (Kbd, (VOID*)InputKey, sizeof(EFI_INPUT_KEY));
    }

    PS2DataDispatcher(This);

#if CHECK_BDA_KEYBOARD_BUFFER
#if CSM_SUPPORT
    if( gLegacyBiosProtocolFound ) {
        if (!CheckKeyinBuffer(Kbd)) {
            if( *BdaKbdHead != *BdaKbdTail ) {
                ProcessKBDData (Kbd, *((UINT8 *)(*BdaKbdHead + BdaSeg + 1)));
                *BdaKbdHead += 2;
                if(*BdaKbdHead == 0x3E) {
                    *BdaKbdHead = 0x1E;
                }
            }
        }
    }
#endif
#endif

    return GetKeyFromBuffer (Kbd, (VOID*)InputKey, sizeof(EFI_INPUT_KEY));

}


/**
    Reads the next keystroke from the input device. This
    routine is a part of SimpleTextInEx protocol
    implementation.

    @param    *This - A pointer to the EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL
                      instance.

    @retval    *KeyData -  A pointer to a buffer that is filled in with the
               keystroke state data for the key that was pressed.

**/

EFI_STATUS 
EFIAPI 
KbdReadKeyEx (
    IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
    OUT EFI_KEY_DATA                     *KeyData)
{
    EFI_STATUS  Status;
    KEYBOARD    *Kbd = &gKbd;

    if(KeyData == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    if (!InsideGetMouseData && !InsideKbdReadKey && !InsideOnWaitingOnKey && !InsideKbdReset && !InsideMouseReset) {
        PS2DataDispatcher(This);
    }

#if CHECK_BDA_KEYBOARD_BUFFER
#if CSM_SUPPORT
    if( gLegacyBiosProtocolFound ) {
        if (!CheckKeyinBuffer(Kbd)) {
            if( *BdaKbdHead != *BdaKbdTail ) {
                ProcessKBDData (Kbd, *((UINT8 *)(*BdaKbdHead + BdaSeg + 1)));            
                *BdaKbdHead += 2;
                if(*BdaKbdHead == 0x3E) {
                    *BdaKbdHead = 0x1E;
                }
            }
        }
    }
#endif
#endif

    Status = GetKeyFromBuffer (Kbd, (VOID*)KeyData, sizeof(EFI_KEY_DATA));
    if (EFI_ERROR(Status)) {
        return Status;
    } else {
        // Update the most recent Toggle State.
        KeyData->KeyState.KeyToggleState = 
                 Kbd->KeyData.KeyState.KeyToggleState | TOGGLE_STATE_VALID;
    }

    return EFI_SUCCESS;
}

/**
    Reads the next keystroke from the input device. This
    routine is a part of AmiEfiKeyCode protocol implementation.

    @param    *This - A pointer to the AMI_EFIKEYCODE_PROTOCOL
                      instance.

    @retval    *KeyData -  A pointer to a buffer that is filled in with the
               keystroke state data for the key that was pressed.

**/

EFI_STATUS 
EFIAPI 
KbdReadEfiKeyEx (
    IN AMI_EFIKEYCODE_PROTOCOL *This,
    OUT AMI_EFI_KEY_DATA       *KeyData)
{
    EFI_STATUS Status;
    KEYBOARD *Kbd = &gKbd;

    if (!InsideGetMouseData && !InsideKbdReadKey && !InsideOnWaitingOnKey && !InsideKbdReset && !InsideMouseReset) {
        PS2DataDispatcher(This);
    }


#if CHECK_BDA_KEYBOARD_BUFFER
#if CSM_SUPPORT
    if( gLegacyBiosProtocolFound ) {
        // If there is no key, check whether any key is present BDA Keyboard buffer as
        // we may miss any key in legacy mode.
        if (!CheckKeyinBuffer(Kbd)) {
            if( *BdaKbdHead != *BdaKbdTail ) {
                // Get the scan code from BDA keyboard buffer and process it.
                ProcessKBDData (Kbd, *((UINT8 *)(*BdaKbdHead + BdaSeg + 1)));
                *BdaKbdHead += 2;
                // Check whether we have reached end of circular buffer, if reached initialize
                // header to beginning of the buffer.
                if(*BdaKbdHead == 0x3E) {
                    *BdaKbdHead = 0x1E;
                }
            }
        }
    }
#endif
#endif

    Status = GetKeyFromBuffer (Kbd, (VOID*)KeyData, sizeof(AMI_EFI_KEY_DATA));
    if (EFI_ERROR(Status)) {        
        return Status;
    } else {
        // Update the most recent Toggle State.
        KeyData->KeyState.KeyToggleState = 
                       Kbd->KeyData.KeyState.KeyToggleState | TOGGLE_STATE_VALID;
    }

    return Status;
}

/**
    Set certain state for the input device.

    @param    This           - A pointer to the EFI_SIMPLE_TEXT_INPUT_PROTOCOL_EX
                               instance.
              KeyToggleState - Pointer to the EFI_KEY_TOGGLE_STATE to
                               set the state for the input device.

    @retval    EFI_STATUS

**/

EFI_STATUS 
EFIAPI 
SetState (
    IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
    IN EFI_KEY_TOGGLE_STATE              *KeyToggleState)
{
    BOOLEAN ChgSL = FALSE;
    BOOLEAN ChgCL = FALSE;
    BOOLEAN ChgNL = FALSE;

    KEYBOARD  *Kbd = &gKbd;

    if(KeyToggleState == NULL ) {
        return EFI_INVALID_PARAMETER;
    }

    if (!(*KeyToggleState & TOGGLE_STATE_VALID) ||
        ((*KeyToggleState & (~(TOGGLE_STATE_VALID | KEY_STATE_EXPOSED |
                            SCROLL_LOCK_ACTIVE | NUM_LOCK_ACTIVE | CAPS_LOCK_ACTIVE)))) ) {
        return EFI_UNSUPPORTED;
    }

	// Check for KEY_STATE_EXPOSED bit enabled.
    if(*KeyToggleState & KEY_STATE_EXPOSED){
        Kbd->KeyData.KeyState.KeyToggleState |= KEY_STATE_EXPOSED;
    } else {
        Kbd->KeyData.KeyState.KeyToggleState &= ~KEY_STATE_EXPOSED;
    }

    ChgSL = ((*KeyToggleState & SCROLL_LOCK_ACTIVE)!=0) ^ ((Kbd->KeyData.KeyState.KeyToggleState & SCROLL_LOCK_ACTIVE)!=0);
    ChgNL = ((*KeyToggleState & NUM_LOCK_ACTIVE)!=0) ^ ((Kbd->KeyData.KeyState.KeyToggleState & NUM_LOCK_ACTIVE)!=0);
    ChgCL = ((*KeyToggleState & CAPS_LOCK_ACTIVE)!=0) ^ ((Kbd->KeyData.KeyState.KeyToggleState & CAPS_LOCK_ACTIVE)!=0);

    if (ChgSL || ChgCL || ChgNL) {
        Kbd->KeyData.KeyState.KeyToggleState &= ~(SCROLL_LOCK_ACTIVE | NUM_LOCK_ACTIVE | CAPS_LOCK_ACTIVE);

        if (*KeyToggleState & SCROLL_LOCK_ACTIVE) Kbd->KeyData.KeyState.KeyToggleState |= SCROLL_LOCK_ACTIVE;
        if (*KeyToggleState & NUM_LOCK_ACTIVE) Kbd->KeyData.KeyState.KeyToggleState |= NUM_LOCK_ACTIVE;
        if (*KeyToggleState & CAPS_LOCK_ACTIVE) Kbd->KeyData.KeyState.KeyToggleState |= CAPS_LOCK_ACTIVE;

        LEDsOnOff(Kbd);
    }

    return EFI_SUCCESS;
}

/**
    Get the keys from the keyboard controller

    @param    Event   -   event that has been signaled
              VOID    -   *Context

    @retval    VOID


**/

VOID 
EFIAPI 
StartPollingKey (
    EFI_EVENT    Event, 
    VOID         *Context)
{
    if (!InsideGetMouseData && !InsideKbdReadKey && !InsideOnWaitingOnKey && !InsideKbdReset && !InsideMouseReset) {
        PS2DataDispatcher(Context);
    }
    return;
}

/** @internal
    Check for Record is present or Not

    @param     *Ps2KeyIn

    @retval    FALSE If Record is deleted from list
    @retval    TRUE If Record is present in List
    @endinternal
**/
BOOLEAN
ValidateKeyRecord (
    IN KEY_WAITING_RECORD    *Ps2KeyIn)
{
    KEY_WAITING_RECORD *TempRecord = OUTTER(mPs2KeyboardData.pHead, Link, KEY_WAITING_RECORD);

    while ( TempRecord != NULL) {
        if(TempRecord == Ps2KeyIn) {
            return TRUE;
        }  
        // go to the next element in the list
        TempRecord = OUTTER(TempRecord->Link.pNext, Link, KEY_WAITING_RECORD);
    }

    return FALSE;
}

/** @internal
    Call the notification function based on the key pressed

    @param     *Key   - Key pressed

    @retval    EFI_STATUS
    @endinternal
**/
EFI_STATUS
CheckKeyNotify(
  AMI_EFI_KEY_DATA *Key)
{

    KEY_WAITING_RECORD  *Ps2KeyIn = OUTTER(mPs2KeyboardData.pHead, Link, KEY_WAITING_RECORD);
    BOOLEAN             KeyScanCodeMatch  = FALSE;
    BOOLEAN             KeyUniCodeMatch   = FALSE;
    BOOLEAN             KeyShiftCodeMatch = FALSE;
    BOOLEAN             KeyToggleKeyMatch = FALSE;
    BOOLEAN             DefaultNotify = FALSE;

    // if the list is empty return the status that was passed in
    if (Ps2KeyIn == NULL)
        return EFI_SUCCESS;

    // check for a handle that was already identified
    while ( Ps2KeyIn != NULL)
    {
        KeyScanCodeMatch  = FALSE;
        KeyUniCodeMatch   = FALSE;
        KeyShiftCodeMatch = FALSE;
        KeyToggleKeyMatch = FALSE;
        DefaultNotify     = FALSE;
         
        // If KeyData.Key, KeyData.KeyState.KeyToggleState and KeyData.KeyState.KeyShiftState are 0, 
        // then any incomplete keystroke will trigger a notification of the KeyNotificationFunction. 
        if(Ps2KeyIn->Context.Key.ScanCode == 0 && Ps2KeyIn->Context.Key.UnicodeChar == 0 
                       && Ps2KeyIn->Context.KeyState.KeyShiftState == 0 && Ps2KeyIn->Context.KeyState.KeyToggleState == 0 ) {
            DefaultNotify = TRUE;
        }

        // Check the Scan Code if the Scan code is not 0
        if (Ps2KeyIn->Context.Key.ScanCode != 0) {
            if( Ps2KeyIn->Context.Key.ScanCode == Key->Key.ScanCode) {
                KeyScanCodeMatch=TRUE;
            }
        } else {
            KeyScanCodeMatch=TRUE;
        }

        // Check the Unicode Code Matching
        if(Ps2KeyIn->Context.Key.UnicodeChar == Key->Key.UnicodeChar) {
            KeyUniCodeMatch=TRUE;
        }

        if(Ps2KeyIn->Context.KeyState.KeyShiftState & SHIFT_STATE_VALID){
           if(Ps2KeyIn->Context.KeyState.KeyShiftState == Key->KeyState.KeyShiftState){
             KeyShiftCodeMatch = TRUE;
           }
        }else {
            KeyShiftCodeMatch=TRUE;
        }

        // Check the Key Toggle State
        if(Ps2KeyIn->Context.KeyState.KeyToggleState & TOGGLE_STATE_VALID){
            if(Ps2KeyIn->Context.KeyState.KeyToggleState == Key->KeyState.KeyToggleState) {
                KeyToggleKeyMatch=TRUE;
            }
        } else {
            KeyToggleKeyMatch=TRUE;
        }

        // If everything matched, call the callback function.
        if((DefaultNotify || (KeyScanCodeMatch && KeyUniCodeMatch && KeyShiftCodeMatch && KeyToggleKeyMatch)) && (!Ps2KeyIn->CallbackStatus)) {

            //Call the notification function
            Ps2KeyIn->CallbackStatus=TRUE;
            pBS->CopyMem(&Ps2KeyIn->KeyPressed, Key, sizeof(Ps2KeyIn->KeyPressed));
            pBS->SignalEvent(Ps2KeyIn->NotifyEvent);
            if(ValidateKeyRecord(Ps2KeyIn)) {
                // go to the next element in the list
                Ps2KeyIn = OUTTER(Ps2KeyIn->Link.pNext, Link, KEY_WAITING_RECORD);
            } else {
                // Record deleted. So go to the first element in the list
                Ps2KeyIn = OUTTER(mPs2KeyboardData.pHead, Link, KEY_WAITING_RECORD);
            }
            continue;
        }
        // go to the next element in the list
        Ps2KeyIn = OUTTER(Ps2KeyIn->Link.pNext, Link, KEY_WAITING_RECORD);
    }

    return EFI_SUCCESS;
}

/**
    Ps2KeyNotifyCallback function is called when registered key is pressed

    @param    Event   - Event that caused this function to be called
              Context - Context of the event

    @retval   VOID
**/

VOID 
EFIAPI 
Ps2KeyNotifyCallback (
    EFI_EVENT   Event,
    VOID        *Context
)
{
    KEY_WAITING_RECORD *mPs2KeyboardRecord = (KEY_WAITING_RECORD*)Context;

    mPs2KeyboardRecord->CallbackStatus = FALSE;

    mPs2KeyboardRecord->Callback(&mPs2KeyboardRecord->KeyPressed);

}

/**
    Register the callback function for the specific Key

    @param    This                    - A pointer to the EFI_SIMPLE_TEXT_INPUT_PROTOCOL_EX
                                        instance.
              KeyData                 - A pointer to a buffer that is filled in with the keystroke 
                                        information for the key that was pressed.
              KeyNotificationFunction - Pointer to the Notification Function
              NotificationHandle      - Handle to be unregistered

    @retval    EFI_SUCCESS                  Key notify was registered successfully.
               EFI_INVALID_PARAMETER        The data for registering is invalid.
**/
EFI_STATUS 
EFIAPI 
RegisterKeyNotify (
    IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
    IN EFI_KEY_DATA                      *KeyData,
    IN EFI_KEY_NOTIFY_FUNCTION           KeyNotificationFunction,
    OUT EFI_HANDLE                       *NotifyHandle)
{
    EFI_STATUS                  Status;
    DLINK                       *Link = NULL;
    KEY_WAITING_RECORD          *mPs2KeyboardRecord=NULL;

    if(KeyData == NULL || KeyNotificationFunction == NULL || NotifyHandle == NULL ) {
        return EFI_INVALID_PARAMETER;
    }
    
    // Check if the key has been registered
    for (Link = mPs2KeyboardData.pHead; Link != NULL; Link = Link->pNext) {
        mPs2KeyboardRecord = OUTTER(Link, Link, KEY_WAITING_RECORD);
        if (MemCmp(&mPs2KeyboardRecord->Context, KeyData, sizeof(EFI_KEY_DATA)) == 0 &&
             mPs2KeyboardRecord->Callback == KeyNotificationFunction) {
            *NotifyHandle = (EFI_HANDLE)(&mPs2KeyboardRecord->Link);
            return EFI_SUCCESS;
        }
    }

    // Create database record and add to database
    Status = pBS->AllocatePool ( EfiBootServicesData,
                                 sizeof (KEY_WAITING_RECORD),
                                 (VOID**)&mPs2KeyboardRecord);

    if(EFI_ERROR(Status)) {
        return Status;
    }
    
    
    pBS->SetMem(mPs2KeyboardRecord, sizeof (KEY_WAITING_RECORD), 0);

    //
    // Gather information about the registration request
    //
    pBS->CopyMem(&mPs2KeyboardRecord->Context, KeyData, sizeof(mPs2KeyboardRecord->Context));
    // Gather information about the registration request
    mPs2KeyboardRecord->Callback  = KeyNotificationFunction;
    mPs2KeyboardRecord->CallbackStatus = FALSE;
    
    Status =pBS->CreateEvent(EFI_EVENT_NOTIFY_SIGNAL, TPL_CALLBACK,
                  Ps2KeyNotifyCallback, mPs2KeyboardRecord, &mPs2KeyboardRecord->NotifyEvent);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
        pBS->FreePool(mPs2KeyboardRecord);
        return Status;
    }

    DListAdd (&mPs2KeyboardData, &(mPs2KeyboardRecord->Link));

    // Child's handle will be the address linked list link in the record
    *NotifyHandle = (EFI_HANDLE) (&mPs2KeyboardRecord->Link);

    if(mPs2KeyboardData.Size == 1) {
        pBS->SetTimer(Ps2KeyEvent,
                        TimerPeriodic,
                        KEY_POLLING_INTERVAL);
    }

    return EFI_SUCCESS;
}
/**
    Unregister the callback function

    @param    This               - A pointer to the EFI_SIMPLE_TEXT_INPUT_PROTOCOL_EX
                                   instance.
              NotificationHandle - Handle to be unregistered

    @retval    EFI_SUCCESS                  Key notify was Unregistered successfully.
               EFI_INVALID_PARAMETER        The data for unregistering is invalid.
            
**/
EFI_STATUS 
EFIAPI 
UnRegisterKeyNotify (
    IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This,
    IN EFI_HANDLE                        NotificationHandle )
{

    DLINK               *ListPtr;
    KEY_WAITING_RECORD  *Ps2KeyIn;


    if(NotificationHandle == NULL ) {
        return EFI_INVALID_PARAMETER;
    }

    ListPtr = mPs2KeyboardData.pHead;
    while ( ListPtr != NULL)
    {
        Ps2KeyIn = OUTTER(ListPtr, Link, KEY_WAITING_RECORD);
        if ( (&Ps2KeyIn->Link) == NotificationHandle)
        {
            DListDelete(&mPs2KeyboardData, ListPtr);
            pBS->CloseEvent(Ps2KeyIn->NotifyEvent);
            pBS->FreePool(Ps2KeyIn);
            break;
        }

        ListPtr = ListPtr->pNext;
    }

    if(ListPtr == NULL) {
        return EFI_INVALID_PARAMETER;
    }

    if(mPs2KeyboardData.Size == 0) {
        pBS->SetTimer(Ps2KeyEvent,
                        TimerCancel,
                        0);
    }

    return EFI_SUCCESS;
}

/**
    Callback for the EFI_SIMPLE_TEXT_INPUT_PROTOCOL.WaitForKey
    event;
    checks whether the new key is available and if so -
    signals the event.

    @param    Event   - Event to signal
              Context - Event specific context (pointer to Kbd
              device)

    @retval    VOID

**/

VOID 
EFIAPI 
OnWaitingOnKey (
    EFI_EVENT   Event,
    VOID        *Context )
{

    AMI_EFI_KEY_DATA    EfiKey = {0};
    KEYBOARD            *Kbd = (KEYBOARD*)Context;
    
    // Check if a key is present in Local buffer.
    // If Local Key buffer is empty, call PS2DataDispatcher().
    if(!CheckKeyinBuffer(Kbd)) {
        if((InsideGetMouseData || InsideKbdReadKey || InsideOnWaitingOnKey || InsideKbdReset || InsideMouseReset)) {
            return;
        } else {
            InsideOnWaitingOnKey = TRUE;

            PS2DataDispatcher(Context); // Process new keys if available

            InsideOnWaitingOnKey = FALSE;
        }
    }
    if(CheckKeyinBuffer(Kbd)) {
        // An Event will only be triggered if EfiKey.Key has information contained 
        // within it for EFI_SIMPLE_TEXT_INPUT_PROTOCOL WaitforKey.
        if((Event == Kbd->iSimpleIn.WaitForKey)) {
            pBS->CopyMem(&EfiKey,Kbd->pBufHead , sizeof(AMI_EFI_KEY_DATA));
            //There maybe a partial keystroke in the queue, the event shouldn't be signaled 
            // when partial key stroke is detected and dequeue this partial keystroke.
            if ((EfiKey.Key.UnicodeChar == 0) && (EfiKey.Key.ScanCode == SCAN_NULL)
                  && (EfiKey.KeyState.KeyToggleState & KEY_STATE_EXPOSED)) {
                Kbd->pBufHead++;
                if (Kbd->pBufHead >= Kbd->pBufEnd) Kbd->pBufHead = Kbd->pBufStart;  // Point to the beginning
                return;
            }
        }
        gSysTable->BootServices->SignalEvent(Event);
        return;
    }
}
/** @internal
    PS2 keyboard keys processor, called from main PS2
    dispatcher.
    It gets the key from KBC output buffer and calls
    HandleKBDData.

    @param    *Context - keyboard device pointer

    @retval    VOID

    @note
     PS2 output buffer has data, incoming key is processed,
                  Context->KeyIsReady is updated.
    @endinternal
**/

VOID
ReadAndProcessKey (
    VOID    *Context )
{

    KEYBOARD    *Kbd = &gKbd;
    UINT8       data;

    data = KBCGetData();
    ProcessKBDData (Kbd, data);
    return;

}


/** @internal
    If key is ready, processes any hotkey, and inserts key
    in buffer

    @param    *Kbd - Pointer to key buffer
              data - Key data

    @retval    VOID

    @note
              Incoming key is processed, Context->KeyIsReady is updated
    @endinternal
**/

VOID
ProcessKBDData (
    KEYBOARD    *Kbd,
    UINT8       data )
{
    EFI_STATUS      OemHandleDataStatus=EFI_NOT_FOUND;
    UINT8           Index;
    
    // Call the Oem HandleKbdData function before call the Core HandleKbdData
    for(Index = 0; Ps2CtlHandleDataOemFunction[Index] != NULL; Index++) {
        OemHandleDataStatus=Ps2CtlHandleDataOemFunction[Index](Kbd, data);
        // check the if the OEM function handled the data 
        if(!EFI_ERROR(OemHandleDataStatus)) {
            // The data has been handled successfully. 
            break;
        }
    }

    // Check if the OEM function handled the data
    if(EFI_ERROR(OemHandleDataStatus)) {
        // OEM function doesn't handle the data. So Core Function called to handle the data
        HandleKBDData(Kbd, data);
    }

    if (Kbd->KeyIsReady) {

        if (Kbd->KeyData.Key.ScanCode == 0 && Kbd->KeyData.Key.UnicodeChar == 0 && Kbd->ScannerState == KBST_READY ) {
            // If the Key ShiftState has valid key but KEY_STATE_EXPOSED bit not enabled,  do not insert key into buffer
            if ((Kbd->KeyData.KeyState.KeyShiftState & ~SHIFT_STATE_VALID)!= 0){
                if(!(Kbd->KeyData.KeyState.KeyToggleState & KEY_STATE_EXPOSED)){
                    Kbd->KeyIsReady = FALSE;
                    return;
            	}
            }
        }
        InsertKeyToBuffer(Kbd, &Kbd->KeyData);
        Kbd->KeyIsReady = FALSE;
    }

    return;
}


/** @internal
    Processes the input data. If a callback function is
    associated with the key, then it is executed.

    @param    *Kbd - Pointer to key buffer
              data - Key data

    @retval    VOID


    @note
              This routine can be called re-entrantly from LEDsOnOff(OutToKb)
              e.g. when Numlock and Del is pressed continuously.
              Incoming key is processed, Context->KeyIsReady is updated.
    @endinternal
**/

VOID
HandleKBDData (
    KEYBOARD    *Kbd,
    UINT8       data )
{
    // PS2 keyboard could send one, two, four or six bytes in a row.
    // Kbd->ScannerState is ST_READY if it is the first byte
    if (Kbd->ScannerState == KBST_READY)
    {
        switch (data) {
            case 0xE0:
                Kbd->ScannerState = KBST_E0;
                break;
            case 0xE1:
                Kbd->ScannerState = KBST_E1;
                break;
            default:
                ProcessByte(Kbd, data, FALSE);
        }
        return;
    }
    else    // Multi-byte sequence is being processed
    {
        if(data == 0xE0) {
            Kbd->ScannerState = KBST_E0;
            return;
        }
        if (Kbd->ScannerState == KBST_E1) {  // Processing E1 state
            if (data != E1Seq[Kbd->Count]) {    // Wrong byte in a sequence
                ResetStateMachine(Kbd);
                return;
            }
            // E1 sequence data is correct, proceed
            if  (Kbd->Count == 2) { // The ONLY 2-key sequence starting with E1 is Pause
                Kbd->KeyData.EfiKey = EfiKeyPause;
                Kbd->KeyData.Key.ScanCode = SCAN_PAUSE;
                Kbd->KeyData.Key.UnicodeChar = 0;
                Kbd->KeyData.PS2ScanCode = 0;
                Kbd->KeyIsReady = TRUE;
                ResetStateMachine(Kbd);
                return;
            }
            if  (Kbd->Count == 4) { // E1 sequence has finished
                ProcessByte(Kbd, data, FALSE);
                ResetStateMachine(Kbd);
                return;
            }
            // E1 sequence is not complete, update the counter and return
            Kbd->Count++;
            return;
        } else { // Kbd->ScannerState == ST_E0 - processing E0 state
            // For E0 state the Count values will be:
            //   0 for 1st and 2nd byte
            //   2 for 3rd byte (if available)
            //   1 for 4th byte (if available)
            // No validity checking will be done for 2nd,3rd and 4th bytes
        if (Kbd->ScannerState == KBST_E0) {
            if ( IsEnhancedKey( data)) {
                ResetStateMachine(Kbd);
                return;
            }

            if(Kbd->KeyData.KeyState.KeyToggleState & (TOGGLE_STATE_VALID | NUM_LOCK_ACTIVE)) {
                if(data == 0x2A) {
                    NumLock2APrecede=TRUE;
                }

                if(data == 0xAA) {
                    if(!NumLock2APrecede) {
                        Kbd->KeyData.KeyState.KeyShiftState &= ~LEFT_SHIFT_PRESSED;
                    }
                    NumLock2APrecede=FALSE;
                }
            }
            // Processing E0 state, if data is 2A or AA or MSB bit is
            // set (break key) ignore it.
            if  (data == 0x2A || data == 0xAA || data & 0x80 && \
                // except in special case of alt, ctrl, Print key, Left Logo, Right Logo and Menu Key
                 data != 0xb8 && data != 0x9d && data != 0xB7 && \
                 data != 0xDB && data != 0xDC && data != 0xDD) {
                ResetStateMachine(Kbd);
                return;
            }
            else {
                ProcessByte(Kbd, data, TRUE);
                ResetStateMachine(Kbd);
                return;
            }
        }
        ProcessByte(Kbd, data, FALSE);
        ResetStateMachine(Kbd);
        return;

        }
    }
}
/** @internal
    Reset State machine

    @param    *Kbd - Pointer to key buffer

    @retval    VOID

    @endinternal
**/

void ResetStateMachine(KEYBOARD *Kbd)
{
    Kbd->ScannerState = KBST_READY;
    Kbd->Count = 0;
}


/** @internal
    Returns TRUE if the given make code belongs to the
    alphabetical symbol, otherwise returns FALSE.

    @param      data - The character to test

    @retval     True if character is letter
    @endinternal
**/

BOOLEAN
IsLetter (
    UINT8   data )
{
    if ((data >= 0x10 && data <= 0x19) ||   // Q...P
        (data >= 0x1E && data <= 0x26) ||   // A...L
        (data >= 0x2C && data <= 0x32)) {   // Z...M
        return TRUE;
    }
    return FALSE;
}

/** @internal
    Checks for valid key, updates key modifiers, keyboard
    queue head and tail as well as Kbd->KeyIsReady field.

    @param    *Kbd          - A pointer to the KEYBOARD device.
              data          - byte to process
              long_sequence - the indication of whether it is a
                              4 byte sequence or not; used to differentiate Shift keys

    @retval    VOID
    @endinternal
**/
VOID
ProcessByte (
    KEYBOARD    *Kbd,
    UINT8       Data,
    BOOLEAN     long_sequence )
{
    BOOLEAN     bUpperCase, bShifted;
    EFI_EXTKEY  *extkey;
    static BOOLEAN Make_Capslock = FALSE, Make_SCRLOCK = FALSE, Make_NUMLOCK = FALSE;

    Kbd->KeyData.Key.ScanCode = SCAN_NULL;
    Kbd->KeyData.Key.UnicodeChar = 0;
    Kbd->KeyData.PS2ScanCode = Data;
    Kbd->KeyData.EfiKey = 0;

    // Process ESC key
    if (Data == 1) {
        Kbd->KeyData.Key.ScanCode = EFI_SCAN_ESC;
        Kbd->KeyData.EfiKey = EfiKeyEsc;
        Kbd->KeyIsReady = TRUE;
        return;
    }

    // Get the E0, E1 Status from BDA (40:96). If those bit get set means Int9 processed the E0
    // E1 data and before processing other Scan code it came out of Legacy mode.
    // Now EFI driver sees other scan code that's part of the E0 and E1 scan. So we don't want to Process
    // those data.
#if CHECK_BDA_KEYBOARD_BUFFER
#if CSM_SUPPORT
    {
    UINT8   *KeyboardTypeBda = (UINT8*)((UINTN) 0x496);
    if( gLegacyBiosProtocolFound ) {
        if(*KeyboardTypeBda & (E0_STATUS_IN_BDA + E1_STATUS_IN_BDA)) {
            *KeyboardTypeBda &=~(E0_STATUS_IN_BDA + E1_STATUS_IN_BDA);
            if(Data != 0xFA && Data != 0xFE && Data != 0xFF ) {
                return;
            }
        }
    }
    }
#endif
#endif

    // Process key modifiers: xyzLock (update LEDs) and Ctrl/Alt/Shift
    switch (Data) {
        case 0x38:
            if (Kbd->ScannerState == KBST_E0) {
                Kbd->KeyData.KeyState.KeyShiftState |= RIGHT_ALT_PRESSED;
                Kbd->KeyIsReady = TRUE;
            } else {
                Kbd->KeyData.KeyState.KeyShiftState |= LEFT_ALT_PRESSED;
                Kbd->KeyIsReady = TRUE;
            }
            return; //break;
        case 0xB8:
            if (Kbd->ScannerState == KBST_E0) {
                Kbd->KeyData.KeyState.KeyShiftState &= ~RIGHT_ALT_PRESSED;
            } else {
                Kbd->KeyData.KeyState.KeyShiftState &= ~LEFT_ALT_PRESSED;
            }
            return; //break;
        case 0x1D:
            if (Kbd->ScannerState == KBST_E0) {
                Kbd->KeyData.KeyState.KeyShiftState |= RIGHT_CONTROL_PRESSED;
                Kbd->KeyIsReady = TRUE;
            } else {
                Kbd->KeyData.KeyState.KeyShiftState |= LEFT_CONTROL_PRESSED;
                Kbd->KeyIsReady = TRUE;
            }
            return; //break;
        case 0x9D:
            if (Kbd->ScannerState == KBST_E0) {
                Kbd->KeyData.KeyState.KeyShiftState &= ~RIGHT_CONTROL_PRESSED;
            } else {
                Kbd->KeyData.KeyState.KeyShiftState &= ~LEFT_CONTROL_PRESSED;
            }
            return; //break;
        case 0x2A:
            Kbd->KeyData.KeyState.KeyShiftState |= LEFT_SHIFT_PRESSED;
            Kbd->KeyIsReady = TRUE;
            return; //break;
        case 0xAA: // could be a part of a long break code
            if (!long_sequence) {
                Kbd->KeyData.KeyState.KeyShiftState &= ~LEFT_SHIFT_PRESSED;
            }
            return; //break;
        case 0x36:
            Kbd->KeyData.KeyState.KeyShiftState |= RIGHT_SHIFT_PRESSED;
            Kbd->KeyIsReady = TRUE;
            return; //break;
        case 0xB6:
            Kbd->KeyData.KeyState.KeyShiftState &= ~RIGHT_SHIFT_PRESSED;
            return; //break;
        case 0x3A:
            if (!Make_Capslock) {
                Kbd->KeyData.KeyState.KeyToggleState ^= CAPS_LOCK_ACTIVE;
                Make_Capslock = TRUE;
            }
            Kbd->KeyData.EfiKey = EfiKeyCapsLock;
            Kbd->KeyIsReady = TRUE;
            break;

        case 0xBA:
            Make_Capslock = FALSE;
            break;
        case 0x46:
            if (Kbd->ScannerState != KBST_E0){
                    if (!Make_SCRLOCK) {
                        Kbd->KeyData.KeyState.KeyToggleState ^= SCROLL_LOCK_ACTIVE;
                        Make_SCRLOCK = TRUE;
                    }
                    Kbd->KeyData.EfiKey = EfiKeySLck;
                    Kbd->KeyIsReady = TRUE;
            }
            break;
        case 0xC6:
            if (Kbd->ScannerState != KBST_E0){
                Make_SCRLOCK = FALSE;
            }
            break;
        case 0x45:
            if (!Make_NUMLOCK) {
                Kbd->KeyData.KeyState.KeyToggleState ^= NUM_LOCK_ACTIVE;
                Make_NUMLOCK = TRUE;
            }
            Kbd->KeyData.EfiKey = EfiKeyNLck;
            Kbd->KeyIsReady = TRUE;
            break;
        case 0xC5:
            Make_NUMLOCK = FALSE;
            break;
        // Handle the PrintScreen/SysRq make Code
        case 0x37:
            if (Kbd->ScannerState == KBST_E0) {
                Kbd->KeyData.KeyState.KeyShiftState |= SYS_REQ_PRESSED;
                return;
            }
            break;
        // Handle the PrintScreen/SysRq break code
        case 0xB7:
            if (Kbd->ScannerState == KBST_E0) {
                Kbd->KeyData.KeyState.KeyShiftState &= ~ SYS_REQ_PRESSED;
                return;
            }
            break;
        // Handle the Left Logo make Code
        case 0x5B:
            if (Kbd->ScannerState == KBST_E0) {
                Kbd->KeyData.KeyState.KeyShiftState |= LEFT_LOGO_PRESSED;
                Kbd->KeyIsReady = TRUE;
                return;
            }
            break;
        // Handle the Left Logo break code
        case 0xDB:
            if (Kbd->ScannerState == KBST_E0) {
                Kbd->KeyData.KeyState.KeyShiftState &= ~ LEFT_LOGO_PRESSED;
                return;
            }
            break;
        // Handle the Right Logo make Code
        case 0x5C:
            if (Kbd->ScannerState == KBST_E0) {
                Kbd->KeyData.KeyState.KeyShiftState |= RIGHT_LOGO_PRESSED;
                Kbd->KeyIsReady = TRUE;
                return;
            }
            break;
        // Handle the Right Logo break code
        case 0xDC:
            if (Kbd->ScannerState == KBST_E0) {
                Kbd->KeyData.KeyState.KeyShiftState &= ~ RIGHT_LOGO_PRESSED;
                return;
            }
            break;
        // Handle the Menu Key make Code
        case 0x5D:
            if (Kbd->ScannerState == KBST_E0) {
                Kbd->KeyData.KeyState.KeyShiftState |= MENU_KEY_PRESSED;
                Kbd->KeyIsReady = TRUE;
                return;
            }
            break;
        // Handle the Meny Key break code
        case 0xDD:
            if (Kbd->ScannerState == KBST_E0) {
                Kbd->KeyData.KeyState.KeyShiftState &= ~ MENU_KEY_PRESSED;
                return;
            }
            break;
        case 0xFA:
            ProcessKBDResponse(Kbd, Data);
            break;
        case 0xFE:
            ProcessKBDResponse(Kbd, Data);
            break;
        case 0xFF:
            ProcessKBDResponse(Kbd, Data);
            break;
    }

    // If Scan Code is 2B, check whether the Keyboard Layout selected is English or Japanese
    if(Data == 0x2B){
        if(KeyboardLayoutFlag) {
            FindJapaneseKbd();
        }
        if(JapaneseKbdDetected) {
            bShifted = Kbd->KeyData.KeyState.KeyShiftState & (RIGHT_SHIFT_PRESSED | LEFT_SHIFT_PRESSED);
            
            if (Kbd->ScannerState == KBST_E0){
                bShifted = 0;
            }
            if (bShifted) {
                Kbd->KeyData.Key.UnicodeChar = '}';
            }else {
                Kbd->KeyData.Key.UnicodeChar = ']';
            }

            Kbd->KeyData.EfiKey = EfiKeyC12;
            Kbd->KeyIsReady = TRUE;
            return;
        }
    }
    
    //if Scan Code is (E0,35), it belongs to KP Divide symbol ('/') 
    if ( Data == 0x35 && Kbd->ScannerState == KBST_E0){
        Kbd->KeyData.Key.UnicodeChar = '/';
        Kbd->KeyData.EfiKey = EfiKeySlash;
        Kbd->KeyIsReady = TRUE;
        return;
    }
    
    // Process main block of keys
    if (Data < 0x3A && !(Kbd->ScannerState == KBST_E0 && Data == 0x37)) {// Exceptional case is PrintScreen/SysReq

        bShifted = Kbd->KeyData.KeyState.KeyShiftState & (RIGHT_SHIFT_PRESSED | LEFT_SHIFT_PRESSED);


        if (IsLetter(Data)) {   // for not-a-letter Caps-Lock must not work
            bUpperCase = (Kbd->KeyData.KeyState.KeyToggleState & CAPS_LOCK_ACTIVE)? !bShifted: bShifted;
        }
        else {
            bUpperCase = bShifted;
        }
        if (bUpperCase) {
            Kbd->KeyData.Key.UnicodeChar = Code_Table[Data];    // UPPER CASE TABLE
        }
        else {
            Kbd->KeyData.Key.UnicodeChar = code_table[Data];    // lower case table
        }
        Kbd->KeyData.EfiKey = ScancodeToEfi_table[Data];
        if (Kbd->KeyData.Key.UnicodeChar != 0) {
            Kbd->KeyIsReady = TRUE;
        }
        return;
    }

    // UK Keyboard "|\" (EfiKeyB0)
    // data = 0x56(ScanCode Set1), EfiKeyB0, "|\"
    if ( Data == 0x56) {
        bShifted = Kbd->KeyData.KeyState.KeyShiftState & (RIGHT_SHIFT_PRESSED | LEFT_SHIFT_PRESSED);
        if ( Kbd->ScannerState == KBST_E0) bShifted = 0;  // Check for '/' in Keypad. Otherwise '?' will be sent
        bUpperCase = bShifted;      // for not-a-letter Caps-Lock must not work
        if (bUpperCase) {
            Kbd->KeyData.Key.UnicodeChar = '|';     // UPPER CASE TABLE
        }
        else {
            Kbd->KeyData.Key.UnicodeChar = '\\';    // lower case
        }
        Kbd->KeyData.EfiKey = EfiKeyB0;
        Kbd->KeyIsReady = TRUE;
        return;
    }

    // Scan code included for Japanese Keyboard

    if (Data == 0x7D) {
        bShifted = Kbd->KeyData.KeyState.KeyShiftState & (RIGHT_SHIFT_PRESSED | LEFT_SHIFT_PRESSED);
        if (bShifted)
            Kbd->KeyData.Key.UnicodeChar = '|';
        else
            Kbd->KeyData.Key.UnicodeChar = 0xA5;
       
        Kbd->KeyData.EfiKey = EfiKeyD13;
        Kbd->KeyIsReady = TRUE;
        return;
    }

    if (Data == 0x73) {
        bShifted = Kbd->KeyData.KeyState.KeyShiftState & (RIGHT_SHIFT_PRESSED | LEFT_SHIFT_PRESSED);
        if (bShifted)
            Kbd->KeyData.Key.UnicodeChar = '_';
        else
            Kbd->KeyData.Key.UnicodeChar = '\\';

        Kbd->KeyData.EfiKey = EfiKeyB0;
        Kbd->KeyIsReady = TRUE;
        return;
    } 

    // Process keypad keys.  Exceptional cases:  -, + on keypad
    // Keypad numbers when NUMLOCK only is ON OR Shift only is pressed.
    if (Kbd->ScannerState != KBST_E0) {
        if ((Data > 0x46) && (Data < 0x54) && !long_sequence) {
            if (((Kbd->KeyData.KeyState.KeyToggleState & NUM_LOCK_ACTIVE)   // Only NUMLOCK in ON
                    && ((Kbd->KeyData.KeyState.KeyShiftState & (RIGHT_SHIFT_PRESSED | LEFT_SHIFT_PRESSED)) == 0))
                || (((Kbd->KeyData.KeyState.KeyToggleState & NUM_LOCK_ACTIVE) == 0)   // Only shift key is pressed
                    && (Kbd->KeyData.KeyState.KeyShiftState & (RIGHT_SHIFT_PRESSED | LEFT_SHIFT_PRESSED)))
                || Data == 0x4a || Data ==0x4e) // check for -,  + keys in keypad
            {
                Kbd->KeyData.Key.UnicodeChar = KeyPad_Table[Data-0x47];
                Kbd->KeyData.EfiKey = KeyPadEfiCode_Table[Data-0x47];
                Kbd->KeyIsReady = TRUE;
                
                // Checking Whether the DEL key is pressed in Keypad along with Ctrl and Alt combination.
                // If Pressed, Updating the Scan code of DELETE key.
                if(Kbd->KeyData.KeyState.KeyShiftState & (RIGHT_CONTROL_PRESSED | LEFT_CONTROL_PRESSED) &&
                   Kbd->KeyData.KeyState.KeyShiftState & (RIGHT_ALT_PRESSED | LEFT_ALT_PRESSED) &&
                   Data == 0x53) {
                    Kbd->KeyData.Key.UnicodeChar = 0;
                    Kbd->KeyData.Key.ScanCode = EFI_SCAN_DEL;
                    Kbd->KeyData.EfiKey = EfiKeyDel;
                    Kbd->KeyIsReady = TRUE;
                }
                return; 
            }
        }
     }

    
    // Process F-keys
    for (extkey = ScanCode_Table; extkey->makecode != 0xFF; extkey++) {
        if (Data == extkey->makecode) {
            Kbd->KeyData.Key.ScanCode = extkey->efi_scancode;
            Kbd->KeyData.EfiKey = extkey->efi_key;
            Kbd->KeyIsReady = TRUE;
        }
    } 
    
    return ;
}

/** @internal
    Turns keyboard LEDs on and off.

    @param    *Kbd - A pointer to the KEYBOARD device.

    @retval   VOID
    @endinternal
**/

void LEDsOnOff (
    KEYBOARD*   Kbd )
{

    UINT8 bIndicators = Kbd->KeyData.KeyState.KeyToggleState & 7;    // SCRL/NUM/CPSL
    
    if (InsidePS2DataDispatcher) return;
    InsidePS2DataDispatcher = TRUE;

    // Check for IRQ support
    if(KbdMsIrqSupport){
        if(gKeyboardIrqInstall && KBDEnableState){
            if (bIndicators != Kbd->Indicators){
                // Turn on/off the lights
                Kbd->CommandResponded = 0;
                // Issue Keyboard LED Command 0xED
                CheckIssueLEDCmd(Kbd);
                if(Kbd->LEDCommandState == ED_COMMAND_ISSUED) {
                    // LED command "ED" has been issues. Process the Data here itself.
                    UINT32	Counter; 
                    for (Counter = 1000; Counter > 0; Counter--) {
                        // The Interrupt handler will handle the response
                        // and set the Kbd->LEDCommandState to zero in both
                        // case with or without error. Exit once the response is
                        // handled.
                        if(Kbd->LEDCommandState == 0) {
                            break;
                        }
                        gSysTable->BootServices->Stall(1000);
                    }
                }
            }
            Kbd->LEDCommandState = 0;
            InsidePS2DataDispatcher = FALSE;
            return;
        }
    }
    if (bIndicators != Kbd->Indicators && Kbd->ScannerState == KBST_READY &&  Kbd->LEDCommandState == 0 ) {
        // Disable keyboard before issuing the command
        DisableKeyboard();

        // Issue Keyboard LED Command 0xED
        CheckIssueLEDCmd(Kbd);

        //Process the led command and data
        ProcessLEDCommandData(Kbd);

        // Enable keyboard once command has processed
        EnableKeyboard();
    }
    InsidePS2DataDispatcher = FALSE;
    return;
}

/** @internal
    Gets the next key from the circular buffer

    @param    *Kbd - A pointer to the KEYBOARD device.
              *key - Pointer to input key data

    @retval    EFI_SUCCESS   - Key.ScanCode and key.UnicodeChar updated
    @retval    EFI_NOT_READY - There was no keystroke data available. 
                               Current KeyData.KeyState values are updated.
    @endinternal
**/

EFI_STATUS
GetKeyFromBuffer (
    KEYBOARD    *Kbd,
    VOID        *Key,
    UINT8       KeySize)
{
    EFI_KEY_DATA *KeyData = (EFI_KEY_DATA*)Key;
    
    // No keystroke data available, updating the Current KeyData.KeyState values.
    if (Kbd->pBufHead == Kbd->pBufTail) {
        pBS->SetMem(KeyData, sizeof (EFI_KEY_DATA), 0);
        KeyData->KeyState.KeyShiftState = Kbd->KeyData.KeyState.KeyShiftState | SHIFT_STATE_VALID;
        KeyData->KeyState.KeyToggleState = Kbd->KeyData.KeyState.KeyToggleState | TOGGLE_STATE_VALID;
        return EFI_NOT_READY;
    }

    pBS->CopyMem(Key, Kbd->pBufHead, sizeof(AMI_EFI_KEY_DATA));
    
    Kbd->pBufHead++;
    if (Kbd->pBufHead >= Kbd->pBufEnd) Kbd->pBufHead = Kbd->pBufStart;  // Point to the beginning

    return EFI_SUCCESS;

}

/** @internal
    Insert the key into the circular buffer.

    @param    *Kbd - A pointer to the KEYBOARD device.
              key  - input key data

    @retval    EFI_SUCCESS Key placed in buffer
    @retval    EFI_WARN_BUFFER_TOO_SMALL Buffer to small

    @note  
               If buffer is full, the EFI_WARN_BUFFER_TOO_SMALL.
               Tail points to the new Data.
    @endinternal
**/

EFI_STATUS
InsertKeyToBuffer (
    KEYBOARD            *Kbd,
    AMI_EFI_KEY_DATA    *Key)
{
    AMI_EFI_KEY_DATA    *Temp = Kbd->pBufTail;
    AMI_EFI_KEY_DATA    EfiKey = {0};
    EFI_STATUS          Status;
    
    gBS->CopyMem(&EfiKey, Key, sizeof(AMI_EFI_KEY_DATA));
    
    EfiKey.EfiKeyIsValid = 1;
    EfiKey.PS2ScanCodeIsValid = 1;

    Status = ProcessMultiLanguage(&EfiKey);

    // As per UEFI spec., if a class of printable characters that are normally 
    // adjusted by shift modifiers (e.g. Shift Key + "f" key) 
    // would be presented solely as a KeyData.Key.UnicodeChar 
    // without the associated shift state.
    // Tab, BackSpace and Enter Key are considered that it's non Printable
    // Char and returned the ShiftKey status as it is. 
    if( EfiKey.Key.UnicodeChar != 9 && EfiKey.Key.UnicodeChar != 8 
        && EfiKey.Key.UnicodeChar != 13 && EfiKey.Key.UnicodeChar != 0 && EfiKey.Key.UnicodeChar != 0x20) {
        EfiKey.KeyState.KeyShiftState &= ~(RIGHT_SHIFT_PRESSED | LEFT_SHIFT_PRESSED);
    }

    CheckKeyNotify(&EfiKey);
    if (Kbd->pBufTail != Kbd->pBufHead) {   // Check if space available
        Temp++;
        if (Temp > Kbd->pBufEnd) Temp = Kbd->pBufStart;
        if (Kbd->pBufHead == Temp) return EFI_WARN_BUFFER_TOO_SMALL; // No more space

    }
    pBS->CopyMem(Kbd->pBufTail, &EfiKey, sizeof(AMI_EFI_KEY_DATA));
    Kbd->pBufTail++;

    if (Kbd->pBufTail >= Kbd->pBufEnd) Kbd->pBufTail = Kbd->pBufStart;
    
    // If a deadKey is not followed by a valid dependent key, KeyboardLayoutMap returns 
	// EFI_WARN_STALE_DATA and returns the deadkey as a character to print.
    // So handle this input key again.
    if (Status == EFI_WARN_STALE_DATA) {
        InsertKeyToBuffer(Kbd, Key);
    }
    return EFI_SUCCESS;

}

/** @internal
    Checks if any key is present in the buffer

    @param    *Kbd - A pointer to the KEYBOARD device.

    @retval   TRUE if a key is in the buffer, FALSE otherwise.
    @endinternal
**/

BOOLEAN
CheckKeyinBuffer (
    KEYBOARD* Kbd)
{
    if (Kbd->pBufHead == Kbd->pBufTail) return FALSE;
    return TRUE;
}

/**
    An interrupt handler for keyboard IRQ.


    @param    InterruptType  Interrupt type
              SystemContext  System context

    @retval    VOID

**/
VOID 
EFIAPI 
KeyboardInterruptHandler (
    IN EFI_EXCEPTION_TYPE   InterruptType,
    IN EFI_SYSTEM_CONTEXT   SystemContext)
{
    KEYBOARD_IRQ_STORE  *KbIrqBuffer = &gKeyboardIrqBuffer;
    KEYBOARD            *Kbd = &gKbd;
    UINT8               bIndicators = Kbd->KeyData.KeyState.KeyToggleState & 7; // SCRL/NUM/CPSL
    UINT8               Data = IoRead8(KBC_DATA_PORT);
 
    gCpuArch->DisableInterrupt(gCpuArch);
    // Stored received acknowledgement
    switch (Data) {
        case 0xFA:
             Kbd->CommandResponded = KB_ACK_COM;
             if (Kbd->LEDCommandState == ED_COMMAND_ISSUED) {
                Kbd->LEDCommandState = ED_DATA_ISSUED;
                Kbd->Indicators = bIndicators;
                WriteKeyboardData(bIndicators);
                if(ReSendLEDCommand) {
                    ReSendLEDCommand = FALSE;           
                }
                break;
             }
             if (Kbd->LEDCommandState == ED_DATA_ISSUED) {
                Kbd->LEDCommandState = 0;
                break;
             }
             break;
        case 0xFE:
             Kbd->CommandResponded = KB_RSND_COM;
             if(ReSendLEDCommand) {
			    // Resending the LED command also failed,
			    // So set the local flag that LED state is synced. 
                Kbd->Indicators = bIndicators;
			    Kbd->LEDCommandState = 0;
                ReSendLEDCommand = FALSE;
                break;               
             }
             if (Kbd->LEDCommandState == ED_COMMAND_ISSUED || Kbd->LEDCommandState == ED_DATA_ISSUED) {

                WriteKeyboardData(0xF4);
                Kbd->LEDCommandState = 0;
                ReSendLEDCommand = TRUE;
				// Resend the LED command for one more time
                LEDsOnOff(Kbd);
                break;
             }
             break;
        case 0xFF:
             Kbd->CommandResponded = KB_RSND_COM;
             Kbd->LEDCommandState = 0;
             break;
        default:
            // If key has been pressed before keyboard start then data will be saved in local buffer
            // else data will be processed
            if(!gKeyboardDriverStart){
                if (KbIrqBuffer->KbdIndex < BUFFER_SIZE){
                    KbIrqBuffer->KbdBuffer[KbIrqBuffer->KbdIndex]=Data;
                    KbIrqBuffer->KbdIndex++;
                }
            }
             else {
                ProcessKBDData (Kbd, Data);
            }
            break;
    }

    // End of interrupt command sent
    mLegacy8259->EndOfInterrupt(mLegacy8259, SYSTEM_KEYBOARD_IRQ);
    gCpuArch->EnableInterrupt(gCpuArch);
}

/** @internal
    To initialize keyboard interrupt, register keyboard
    handler, and enable the keyboard interrupt
 
    @param     None

    @retval    VOID
    @endinternal
**/
VOID
InitKeyboardIrq (VOID)
{

    EFI_STATUS          Status;
    KEYBOARD_IRQ_STORE  *KbIrqBuffer                = &gKeyboardIrqBuffer;
    UINT32              KeyboardVector              = 0;
   
    if (!gKeyboardIrqInstall){
        Status = pBS->LocateProtocol(&gEfiLegacy8259ProtocolGuid, NULL, (VOID**)&mLegacy8259);
        if (EFI_ERROR(Status)) {
            return;
        }
        // Find the CPU Arch Protocol
        Status = pBS->LocateProtocol(&gEfiCpuArchProtocolGuid, NULL, (VOID**)&gCpuArch);
        if (EFI_ERROR(Status)) {
            return;
        }

        // Get keyboard Interrupt vector
        Status = mLegacy8259->GetVector(mLegacy8259, SYSTEM_KEYBOARD_IRQ, (UINT8 *) & KeyboardVector);
        if (EFI_ERROR(Status)) {
            return;
        }

        // Register interrupt handler for keyboard
        Status = gCpuArch->RegisterInterruptHandler(gCpuArch, KeyboardVector, KeyboardInterruptHandler);
        if (EFI_ERROR(Status)) {
            return;
        }

        KbIrqBuffer->KbdIndex=0;
        gKeyboardIrqInstall = TRUE;
        // Now enable the interrupt
        mLegacy8259->EnableIrq(mLegacy8259, SYSTEM_KEYBOARD_IRQ, FALSE);
    }
    return;
}

#if CLEAR_PENDING_KEYS_IN_PS2
/** @internal
    Clears any pending keys in Ps2 Output Buffer. 

        
    @param    VOID

           
    @retval    VOID
  
    @note  
              This OBF clear is done since some PS2 KBC has pending keys even
              after resetting the Keyboard.
    @endinternal
**/
VOID
ClearOBF ( VOID )
{
    UINT8 bData;
    UINT8 Counter=200;

    pBS->Stall(10*1000);

    while( IoRead8(KBC_CMDSTS_PORT) & KBC_OBF )
    {
        bData = IoRead8(KBC_DATA_PORT);
        // wait 10 ms for KBC OBF output.
        pBS->Stall(10*1000);
        Counter--;
        // If still keys are present after 2 sec. exit from the loop.
        if(Counter == 0) {
            break;
        }
    }
    return; 
}

/** @internal
    Reset the Ps2 Keyboard by sending disabling and Enabling scanning. 

        
    @param    VOID

           
    @retval    VOID
    @endinternal
**/
VOID
Ps2KbdReset( VOID )
{
    EFI_STATUS  Status;
    UINT8       bCount, bData, bBCount;

    // Disable and enable keyboard to reset.
    DisableKeyboard();

    for (bBCount = 0; bBCount < 4; bBCount++) {
        for (bCount = 0; bCount < 3; bCount++) {
            Status = ReadDevice(KBD_DISABLE_SCANNING, &bData, KB_ACK_COM);
            if (!EFI_ERROR(Status)) {
                break;
            }    
        }

        if (EFI_ERROR(Status)) {
            continue; 
        }

        for (bCount = 0; bCount < 3; bCount++) {
            Status = ReadDevice(KBD_ENABLE_SCANNING, &bData, KB_ACK_COM);
            if (!EFI_ERROR(Status)) { 
                break;
            }
        }

        if (!EFI_ERROR(Status)) {
            break;
        }
    }

    EnableKeyboard();
    return;
}

#endif

/** @internal
    Process the LED command and data.

    @param    *Kbd - A pointer to the KEYBOARD device.

    @retval    VOID
    @endinternal
**/

VOID
ProcessLEDCommandData (
    KEYBOARD*   Kbd )
{

    UINT32	Counter;
    UINT8   data;

    if(Kbd->LEDCommandState == ED_COMMAND_ISSUED) {
        // LED command "ED" has been issued. Disable the keyboard and 
        // Process the Data here itself.
        DisableKeyboard();

        for (Counter = 1000; Counter > 0; Counter--) {
            for(data = IoRead8(KBC_CMDSTS_PORT); data & KBC_OBF; data = IoRead8(KBC_CMDSTS_PORT)) {
                if (!(data & KBC_AUX_OBF)) {
                    // Handle the Command or Data Status
                    DrivePS2KbdMachine(NULL);
                } else {
                    DrivePS2MouseMachine(NULL);
                }    
            }
            // If the command and data is processed with or without error, exit here.
            if(Kbd->LEDCommandState == 0) {
                return;
            }
            gSysTable->BootServices->Stall(1000);			
        }

        if (Counter == 0) {
            Kbd->LEDCommandState = 0;
        }

        EnableKeyboard();
    }
    return;
}

/** @internal
    It maps the current key to a Unicode character from
    the keyboard layout

    @param    KeyData - Pointer to the AMI_EFI_KEY_DATA .

    @retval    EFI_STATUS
    @endinternal
**/
EFI_STATUS 
ProcessMultiLanguage (
    IN OUT  AMI_EFI_KEY_DATA                *KeyData)
{
    EFI_STATUS Status;

    if(gPs2MultiLangSupportProtocol == NULL) {
        Status= pBS->LocateProtocol (
                  &gAmiMultiLangSupportProtocolGuid,
                  NULL,
                  (VOID**)&gPs2MultiLangSupportProtocol
                  );
        if(EFI_ERROR(Status)) {
            return EFI_NOT_FOUND;
        }
    }    
    Status = gPs2MultiLangSupportProtocol->KeyboardLayoutMap(gPs2MultiLangSupportProtocol,KeyData);

    return Status;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
