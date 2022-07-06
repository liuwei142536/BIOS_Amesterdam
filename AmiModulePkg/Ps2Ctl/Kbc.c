//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file kbc.c
    PS/2 Controller I/O support functions

**/

//----------------------------------------------------------------------

#include "Ps2Ctl.h"
#include "Kbc.h"
#include "Library/UefiBootServicesTableLib.h"

//----------------------------------------------------------------------

extern  BOOLEAN MouseResetRequired;
extern  BOOLEAN DetectPs2KeyboardValue;
extern  BOOLEAN InstallKeyboardMouseAlways;
extern  UINT32  IbFreeTimeoutValue;
extern  UINT32  IbFreeMaxTimeoutValue;
BOOLEAN MouseEnableState = FALSE;
BOOLEAN KBDEnableState = TRUE;
BOOLEAN Ps2KbdDetected = FALSE;
BOOLEAN Ps2KbdMouseDetected=FALSE;
BOOLEAN ReSendLEDCommand = FALSE;
extern  BOOLEAN Ps2MouseDetected;
extern  BOOLEAN KbdMsIrqSupport;
extern  UINT8   gKeyboardIrqInstall;
extern  EFI_LEGACY_8259_PROTOCOL *mLegacy8259;
BOOLEAN InsidePS2DataDispatcher = FALSE;
extern BOOLEAN InsideKbdReadKey;

/**
    Reads keyboard data port to clear it

    @param    VOID *Context - Pointer to context

    @retval    UINT8 Keyboard data port data

**/

UINT8
KBCDummyGetData (
    VOID *Context )
{
    UINT8   bData;
    
    bData = IoRead8(KBC_DATA_PORT);
    return bData;

}


/**
    Reads keyboard data port to clear it

    @param    VOID *Context - Pointer to context

    @retval    UINT8 Keyboard data port data

**/

UINT8
MouseDummyGetData (
    VOID *Context )
{
    UINT8   bData;
    
    bData = IoRead8(KBC_DATA_PORT);
    MouseResetRequired = TRUE;          // Since a orphan mouse data is
                                        // received, mouse packet will be out
                                        // of sync.
    return bData;

}

//----------------------------------------------------------------------
// The following two function pointers are initialized with dummy
// routines; they will be updated with the real routine pointers
// in the corresponding device drivers' start functions.
//----------------------------------------------------------------------

STATEMACHINEPROC DrivePS2KbdMachine = (STATEMACHINEPROC)KBCDummyGetData;
STATEMACHINEPROC DrivePS2MouseMachine = (STATEMACHINEPROC)MouseDummyGetData;

extern KEYBOARD gKbd;


/**
    Runs Basic Assurance Test on KBC.

    @param    None

    @retval    EFI_SUCCESS or EFI_DEVICE_ERROR

**/

EFI_STATUS
KBCBatTest ()
{
    UINT16 wCounter = 0xFFFF;
    
    PROGRESS_CODE(DXE_KEYBOARD_SELF_TEST);
    // Empty KBC before BAT
    for (; wCounter; wCounter--) {
        IoRead8(KBC_DATA_PORT);
        IoDelay();
        if (!(IoRead8(KBC_CMDSTS_PORT) & (KBC_OBF | KBC_IBF))) {
            break;
        }
    }
    if (!wCounter) {
        return EFI_DEVICE_ERROR;
    }
    // Perform BAT
    if (Read8042(0xAA) != 0x55) {
        return EFI_DEVICE_ERROR;
    }

    return EFI_SUCCESS;
}

/**
    Auto detection of KB/MS using AMI KB-5.  This code will
    allow the connector swap of Keyboard and PS2 Mouse i.e.
    keyboard can be connected to PS2 Mouse connector and
    vice-versa.

    @param    None. AMI KB-5 present in the system, keyboard controller
              BAT is complete.

    @retval   None


    @note     This code should be used only if the motherboard has
              AMI KB-5 which is also available in IO chipsets having KBC
              e.g. SMC932, etc.

**/

VOID
AutodetectKbdMousePorts ()
{
    UINT8           bData, Index;
    EFI_STATUS      Status;
    
    Status = IbFreeTimeout(IbFreeMaxTimeoutValue);
    if (EFI_ERROR(Status)) {
        return;
    }
    WriteKeyboardCommand(0x60);         // Lock KBD
    IoRead8(KBC_DATA_PORT);             // Discard any data

    Write8042CommandByte(0x74);         // KBD and Aux device disabled

    // Check for KBC version
    IoRead8(KBC_DATA_PORT);             // Discard any data
    WriteKeyboardCommand(0xa1);         //
    if (!ObFullReadTimeout(&bData, 20, TRUE) && bData == 0x35) {

        WriteKeyboardCommand(0x60);
        WriteKeyboardData(4);

        for (Index = 6; Index; Index--){  // Read max. 6 data
            if (ObFullReadTimeout(&bData, 10, TRUE)) break;
        }

        WriteKeyboardCommand(0xa7);         // Disable Mouse
        WriteKeyboardCommand(0xc8);         //  Select Primary

        WriteKeyboardData(rKeyboardID);     // Read KBD ID

        ObFullReadTimeout(&bData, 1000, TRUE);      // Read ACK

        if (bData == rKeyboardID) goto PortSwap;

        if (bData == KB_ACK_COM) {
            ObFullReadTimeout(&bData, 100, TRUE);
            // When Mouse is connected to KBD port, control goes to PortSwap here
            if (!bData) goto PortSwap;
            ObFullReadTimeout(&bData, 100, TRUE);
        }
        bData = IoRead8(KBC_CMDSTS_PORT);
        // When KBD is connected to the KBD port, control returns here
        if (!(bData & KBC_TIMEOUT_ERR)) return;

        WriteKeyboardCommand(0xD4);         // Secondary Port
        WriteKeyboardData(rKeyboardID);     // Read KBD ID
        ObFullReadTimeout(&bData, 1000, TRUE);
        if (bData == rKeyboardID) return;
        if (bData == KB_ACK_COM) {
            // When Mouse alone is connected to Mouse support, control returns here
            if (!ObFullRead()) return;
            bData = ObFullRead();
        }
        bData = IoRead8(KBC_CMDSTS_PORT);
        // When KBD alone is connected to Mouse port, no time out error and control
        //  goes to portswap.
        if (bData & KBC_TIMEOUT_ERR) return;

PortSwap:
        WriteKeyboardCommand(0xC9);
        return;
    }
}


/**
    This routine issues AuxDevice command, and returns the
    from the AUX device the connector swap of Keyboard and 
    PS2 Mouse i.e. keyboard can be connected to PS2 Mouse 
    connector and vice-versa.

    @param    bCmd - AUX device command

    @retval   Data from AUX device

    @note  
              Only AUX commands that expect the response from AUX device
              can be executed using this function; otherwise the code will
              be stuck waiting for OBF

**/

UINT8
AuxDeviceCommand (
    UINT8   bCmd )
{
    EFI_STATUS              Status;
    
    Status = IbFreeTimeout(IbFreeMaxTimeoutValue);
    if (EFI_ERROR(Status)) {
        return (UINT8)Status;
    }
    IoWrite8(KBC_CMDSTS_PORT, 0xD4);

    return IssueCommand(bCmd);
}


/**
    Helper function to read the data after executing AUX
    device command.

    @param    bCmd - AUX device command

    @retval   Data - from AUX device

    @note     Only AUX commands that expect the response from AUX device
              can be executed using this function; otherwise the code will
              be stuck waiting for OBF

**/

UINT8
IssueCommand (
    UINT8   bCmd )
{
    EFI_STATUS              Status;
    
    Status = IbFreeTimeout(IbFreeMaxTimeoutValue);
    if (EFI_ERROR(Status)) {
        return (UINT8)Status;
    }
    IoWrite8(KBC_DATA_PORT, bCmd);
    IbFree();
    for (;;)
    {
        if (IoRead8(KBC_CMDSTS_PORT) & KBC_OBF) {
            return IoRead8(KBC_DATA_PORT);
        }
    }
}


/**
    Sends the given command to KBC, reads and returns the
    Acknowledgment byte returned from KBC.

    @param   bCmd - Command to send to KBC

    @retval  Acknowledgment byte

**/

UINT8 
Read8042 (
    UINT8   bCmd )
{

    UINT8 bData = 0xFE;
    
    WriteKeyboardCommand(bCmd);
    ObFullReadTimeout(&bData, 40, FALSE);
    return bData;

}



/**
    Sends the given command to KBD, reads and returns the
    Acknowledgment byte returned from KBD.

    @param   bCmd      - Command to send to KBC
             *Data     - Pointer to data buffer
             Response  - Response expected

    @retval    EFI_SUCCESS Data == Response
    @retval    EFI_DEVICE_ERROR Data != Response

**/

EFI_STATUS
ReadDevice (
    UINT8   bCmd,
    UINT8   *Data,
    UINT8   Response )
{

    WriteKeyboardData(bCmd);
    if (ObFullReadTimeout(Data, 40, FALSE)) return EFI_DEVICE_ERROR;
    if (*Data == Response) return EFI_SUCCESS;
    return EFI_DEVICE_ERROR;

}

/**
    Writes command to KBC.

    @param   bCmd - Command to send to KBC

    @retval  VOID

**/

VOID
WriteKeyboardCommand (
    UINT8   bCmd )
{
    EFI_STATUS              Status;
    
    Status = IbFreeTimeout(IbFreeMaxTimeoutValue);
    if (EFI_ERROR(Status)) {
        return;
    }
    IoWrite8(KBC_CMDSTS_PORT, bCmd);
    IbFree();
}

/**
    Writes data to KBC.

    @param     bCmd - Data to send to KBC

    @retval    VOID

**/

VOID
WriteKeyboardData (
    UINT8   bCmd )
{
    EFI_STATUS              Status;
    
    Status = IbFreeTimeout(IbFreeMaxTimeoutValue);
    if (EFI_ERROR(Status)) {
        return;
    }
    IoWrite8(KBC_DATA_PORT, bCmd);
    IbFree();
}

/**
    Writes CCB to KBC

    @param    bCCB - Command byte to send to KBC

    @retval   VOID

**/

VOID 
Write8042CommandByte (
    UINT8   bCCB )
{
    EFI_STATUS              Status;
    
    Status = IbFreeTimeout(IbFreeMaxTimeoutValue);
    if (EFI_ERROR(Status)) {
        return;
    }
    WriteKeyboardCommand(0x60);     // CMD to send command byte
    IoWrite8(KBC_DATA_PORT, bCCB);  // Write command byte into KBC
    IbFree();                       // Wait until input buffer is free
}


/**
    Waits for Output Buffer Full and then reads the data port

    @param    VOID

    @retval    KBC Data port data

**/

UINT8
ObFullRead ()
{
    for (;;) {
        if (IoRead8(KBC_CMDSTS_PORT) & KBC_OBF) {
            return IoRead8(KBC_DATA_PORT);
        }
    }
}

/**
    Waits for Input Buffer to be empty

    @param    None

    @retval   VOID

**/

VOID
IbFree ()
{
    for (;;) {
        if (!(IoRead8(KBC_CMDSTS_PORT) & KBC_IBF)) {
            break;
        }
    }
}

/**
    Waits a specified timeout for Input Buffer to be empty

    @param    TimeoutValue

    @retval   EFI_STATUS (EFI_SUCCESS or EFI_TIMEOUT)

**/

EFI_STATUS
IbFreeTimeout ( UINT32 TimeoutValue)
{
    UINTN   i;

    for (i = 0; i < TimeoutValue; i++) {
        if (!(IoRead8(KBC_CMDSTS_PORT) & KBC_IBF)) {
            return EFI_SUCCESS;
        }
        gSysTable->BootServices->Stall(1000);   // 1 ms
    }
    return EFI_TIMEOUT;
}

/**
    Performs IO delay by executing IO read.

    @param    none

    @retval    VOID

**/

VOID
IoDelay ()
{
    IoRead8(IO_DELAY_PORT);
}


/**
    This routine checks for the data availability in output
    buffer for a short period of time, if data is available
    within this time, it reads and returns the data from
    output buffer.

    @param    data    - Pointer to the byte to be updated
              msec    - Milliseconds timeout
              ONLYOBF - Only waits for OBF if true

    @retval    Returns FALSE if data is successfully updated
               (no timeout), Returns TRUE if time-out

**/

BOOLEAN
ObFullReadTimeout (
    UINT8*      data,
    UINT32      msec,
    BOOLEAN     ONLYOBF )
{

    UINT8       bData;
    UINT32      loopcount = msec << 1;


    for (; loopcount; loopcount--) {

        bData = IoRead8(KBC_CMDSTS_PORT);

        if (ONLYOBF && (bData & KBC_OBF)) {
            *data = IoRead8(KBC_DATA_PORT);
            return FALSE;
        }

        if ((bData & (KBC_OBF|KBC_AUX_OBF)) == KBC_OBF) {
            *data = IoRead8(KBC_DATA_PORT);
            if (bData & 0x40) {
                return TRUE;
            }
            else return FALSE;  // No timeout
        }

        if ((bData & (KBC_OBF|KBC_AUX_OBF)) == (KBC_OBF | KBC_AUX_OBF)){
            return TRUE;
        }

        gSysTable->BootServices->Stall(500);        // 0.5msec

    }
    return TRUE;    // Timeout
}


/**
    Send the given command to KDB during runtime.

    @param    kbd  - Pointer to keyboard buffer
              bCmd - Command to send to keyboard

    @retval    EFI_SUCCESS or EFI_DEVICE_ERROR

**/

EFI_STATUS
OutToKb (
    KEYBOARD*   kbd,
    UINT8       bCmd )
{
    UINT8       bCounter1, bData;
    UINT32      Counter;
    EFI_STATUS  Status;
    // If Keyboard irq is supported, device acknowledgment is pressed by IRQ
    // Handler. The acknowledgment data is stored in Kbd->CommandResponded
    if(KbdMsIrqSupport && gKeyboardIrqInstall){
        if(KBDEnableState) {
            Status = IbFreeTimeout(IbFreeMaxTimeoutValue);
            if (EFI_ERROR(Status)) {
                return Status;
            }
            IoWrite8(KBC_DATA_PORT, bCmd);
            IbFree();
            for (Counter = 1000; Counter > 0; Counter--) {
                if (kbd->CommandResponded == KB_ACK_COM){
                    kbd->CommandResponded =0;
                    return EFI_SUCCESS;
                }
                if (kbd->CommandResponded == KB_RSND_COM){
                    kbd->CommandResponded =0;
                    break;
                }
                gSysTable->BootServices->Stall(1000);
            }
            return EFI_DEVICE_ERROR;
        } 
    }

    for (bCounter1 = 3; bCounter1 > 0; bCounter1--) {
        IbFree();
        IoWrite8(KBC_DATA_PORT, bCmd);
        IbFree();

        for (Counter = 1000; Counter > 0; Counter--) {
            if (IoRead8(KBC_CMDSTS_PORT) & KBC_OBF) {
                bData = IoRead8(KBC_DATA_PORT);
                if (bData == 0xFA) {
                    return EFI_SUCCESS;
                } else if (bData == 0xFE) {
                    break;
                } else {
                    if (IoRead8(KBC_CMDSTS_PORT) & KBC_TIMEOUT_ERR) break;
                    // Insert the key into the buffer
                    if (kbd) {
                        HandleKBDData(kbd, bData);
                        if (kbd->KeyIsReady) {
                            InsertKeyToBuffer(kbd, &kbd->KeyData);
                            kbd->KeyIsReady = FALSE;
                        }
                    }
                }
            }
            gSysTable->BootServices->Stall(1000);           // 1msec
        }
    }

    return EFI_DEVICE_ERROR;
}

/**
    Disables KBD interface and reads the data from KBC
    data port.
    
    @param    None

    @retval   VOID

**/

VOID
DisableKeyboard ()
{
    EFI_STATUS  Status;

    if (!KBDEnableState) return;
    Status = IbFreeTimeout(IbFreeMaxTimeoutValue);
    if (EFI_ERROR(Status)) {
        return;
    }
    IoWrite8(KBC_CMDSTS_PORT, 0xAD);
    IbFree();
    KBDEnableState = FALSE;
}


/**
    Enables KBD interface.

    @param    None

    @retval    Status

**/

EFI_STATUS
EnableKeyboard ()
{
    EFI_STATUS Status;

    if (KBDEnableState) {
        return EFI_SUCCESS;
    }
    Status = IbFreeTimeout(IbFreeMaxTimeoutValue);
    if (EFI_ERROR(Status)) {
        return Status;
    }
    IoWrite8(KBC_CMDSTS_PORT, 0xAE);
    Status = IbFreeTimeout(IbFreeTimeoutValue);
    KBDEnableState = TRUE;
    if (EFI_ERROR(Status)) {
        KBDEnableState = FALSE;
    }

    return Status;
}

/**
    Disables Aux interface.

    @param    None

    @retval    VOID

**/

VOID
DisableAuxDevice ()
{
    EFI_STATUS  Status;
    
    if (!MouseEnableState) return;
    Status = IbFreeTimeout(IbFreeMaxTimeoutValue);
    if (EFI_ERROR(Status)) {
        return;
    }
    IoWrite8(KBC_CMDSTS_PORT, 0xA7);
    IbFree();
    MouseEnableState = FALSE;
}

/**
    Enables Aux interface.

    @param    None

    @retval    VOID

**/

VOID
EnableAuxDevice ()
{
    EFI_STATUS  Status;
    
    if (MouseEnableState) return;
    Status = IbFreeTimeout(IbFreeMaxTimeoutValue);
    if (EFI_ERROR(Status)) {
        return;
    }
    IoWrite8(KBC_CMDSTS_PORT, 0xA8);
    IbFree();
    MouseEnableState = TRUE;
}


/**
    Detects the presence of Keyboard and Mouse in KBC port.

    @param    None. Keyboard interface is disabled.

    @retval    Ps2KbdDetected and Ps2MouseDetected variable set according 
               the device presence 

**/

EFI_STATUS
DetectPS2KeyboardAndMouse ()
{
    UINT16      wCount;
    UINT8       bData;
    BOOLEAN     bAck = FALSE;

    if(Ps2KbdMouseDetected) {
        // Ps2Keyboard and Mouse Detected already
        return EFI_SUCCESS;
    }
            
    Ps2KbdMouseDetected=TRUE;

    // FALSE is passing as argument to intimate, according to DETECT_PS2_MOUSE value, perform Mouse Detection
    DetectPs2Mouse(FALSE);


    if ( DetectPs2KeyboardValue ) {

        PROGRESS_CODE(DXE_KEYBOARD_DETECT);
        Write8042CommandByte (0x6d);
        KBCGetData();                                       // Dummy read

        for (wCount = 0; wCount < 3; wCount++) {
            // Disable Scanning
            if (!ReadDevice(KBD_DISABLE_SCANNING, &bData, KB_ACK_COM)) break;
            if (IoRead8(KBC_CMDSTS_PORT) & 0x40) {              // Time out error
                gSysTable->BootServices->Stall(6000);           // 6 msec
                // After power-up some junk data comes from KBD. If not eaten
                // other command will fail.
                KBCGetData();
            }
        }

        DisableKeyboard();
        KBCGetData();   
        // 3 times retry on keyboard reset
        for (wCount = 0; wCount < 3; wCount++) {
            if (!ReadDevice(KBD_RESET, &bData, KB_ACK_COM)) {   // ACK received
                bAck = TRUE;
                break;
            } else {        
                KBCGetData();                       // Dummy read
            }
        }

        if (bAck) {                             //If not not Keyboard
            if (ObFullRead() == 0xAA) {     // Reset successful
                Ps2KbdDetected=TRUE;
            } else if (Read8042(0xAB)) {       // On Success returns 0
                // 0x01 if Clock line stuck low, 0x02 if clock line stuck high, 
                // 0x03 if data line stuck low, and 0x04 if data line stuck high       
                Ps2KbdDetected=FALSE;
            }
        }
    } else { 
        Ps2KbdDetected = TRUE;
        KBDEnableState = TRUE;
    }

    return EFI_SUCCESS;
}
#if defined(GenericSio_SUPPORT) && (GenericSio_SUPPORT != 0)
/**
    Update the SIO variable in the ACPI name space depend on the 
    Ps2keyboard and Mouse Present state.

    @param    Event    - periodic check event
              *Context - event calling context

    @retval    VOID


**/
VOID 
EFIAPI 
UpdateSioVariableForKeyboardMouse (
    EFI_EVENT   Event,
    VOID        *Context
)
{
    static EFI_GUID     SioDevStatusVarGuid = SIO_DEV_STATUS_VAR_GUID;
    UINTN               SioDevStatusVarSize = sizeof(SIO_DEV_STATUS);
    SIO_DEV_STATUS      SioDevStatus;
    UINT32              SioDevStatusVarAttributes = 0;
    EFI_STATUS          Status;

    // Get the SIO variable.
    Status = pRS->GetVariable( SIO_DEV_STATUS_VAR_NAME, 
                                &SioDevStatusVarGuid, 
                                &SioDevStatusVarAttributes,
                                &SioDevStatusVarSize, 
                                &SioDevStatus.DEV_STATUS);

    // If variable not found return without updating it.
    if(EFI_ERROR(Status)) {
        SioDevStatus.DEV_STATUS = 0;
        SioDevStatusVarAttributes = EFI_VARIABLE_BOOTSERVICE_ACCESS;
        SioDevStatus.Res3 = 1;   // To indicate that PS2 state vas updated
    }

    // Set the flag based on the Ps2 keyboard presence state
    if(DetectPS2Keyboard() == EFI_SUCCESS) {
        SioDevStatus.Key60_64 = 1;
    } else {
        SioDevStatus.Key60_64 = 0;
    }


    // Set the Mouse flag based on the Mouse Presence state.
    if(IsPs2MousePresent() == EFI_SUCCESS) {
        SioDevStatus.Ps2Mouse = 1;
    } else {
        SioDevStatus.Ps2Mouse = 0;
    }

    // Set the SIO variable.
    Status = pRS->SetVariable(  SIO_DEV_STATUS_VAR_NAME, 
	                   &SioDevStatusVarGuid, 
                       SioDevStatusVarAttributes,
                       SioDevStatusVarSize, 
                       &SioDevStatus);
    ASSERT_EFI_ERROR(Status);
	return;
}
#endif

/**
    Detects the presence of Keyboard in KBC port.

    @param     None

    @retval    EFI_SUCCESS if mouse is detected
               EFI_NOT_FOUND if mouse is not detected

**/

EFI_STATUS
DetectPS2Keyboard ( )
{
    if ( InstallKeyboardMouseAlways ) {
        return EFI_SUCCESS;
    } else {
        return Ps2KbdDetected ? EFI_SUCCESS : EFI_NOT_FOUND;
    }
}

/**
    This function checks whether data is available in the PS2
    controller output buffer. If so, it gives control to the
    corresponding state machine executor.

    @param    *Context - Pointer to the context for this function

    @retval    VOID

**/

VOID
PS2DataDispatcher (
    VOID    *Context )
{
    UINT8       data;
    KEYBOARD    *kbd = &gKbd;
    UINT8       bIndicators;
    
    EFI_TPL     OldTpl;

    //Raise TPL to avoid re-entrancy
    // This should be the first line. DO NOT ADD ANY CODE ABOVE THIS LINE 
    // Restore to OldTpl before exit of this function

    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);
    
    bIndicators = kbd->KeyData.KeyState.KeyToggleState & 7;    // SCRL/NUM/CPSL 
    if(KbdMsIrqSupport){
        // if irq supported check status of SCRL/NUM/CPSL keys
        // and send the command to Keyboard to update the LED status
        UINT8 bIndicators = kbd->KeyData.KeyState.KeyToggleState & 7; 
        if (bIndicators != kbd->Indicators && kbd->LEDCommandState == 0){
            LEDsOnOff(kbd);
        }
        gBS->RestoreTPL(OldTpl);
        return;
    }

    if (InsidePS2DataDispatcher) {
        gBS->RestoreTPL(OldTpl);
        return;
        }
    InsidePS2DataDispatcher = TRUE;

    data = IoRead8(KBC_CMDSTS_PORT);
    if (data & KBC_OBF) {
        do {
            if (data & KBC_AUX_OBF) {
                DrivePS2MouseMachine(Context);
                InsidePS2DataDispatcher = FALSE;
                gBS->RestoreTPL(OldTpl);
                return;
            }
            if (data & KBC_OBF) {
                // Removed the DisableKeyboard() as to read multiple data from port60h,
                // If any valid key is received we break out of the loop. 
                InsideKbdReadKey = TRUE;
                DrivePS2KbdMachine(Context);
                InsideKbdReadKey = FALSE;
                if (CheckKeyinBuffer(kbd) ) {
                    // The next pass through this function will come long
                    // before 10 ms is up without this delay, and KBC_OBF
                    // may not be set yet, even though a key is available.
                    gSysTable->BootServices->Stall(10000); // wait 10 ms
                    break;
                }
            }
            gSysTable->BootServices->Stall(10000); // wait 10 ms
            data = IoRead8(KBC_CMDSTS_PORT); // read status port again
        } while (data & (KBC_OBF | KBC_AUX_OBF));
    }

    // Check LED state before issuing ED command
    if (bIndicators != kbd->Indicators && kbd->LEDCommandState == 0 && kbd->ScannerState == KBST_READY) {
        // Disable the keyboard before issuing ED command 
        DisableKeyboard();
        CheckIssueLEDCmd(kbd);
    }

    //Process the led command and data
    ProcessLEDCommandData(kbd);
    EnableKeyboard();
    InsidePS2DataDispatcher = FALSE;
    gBS->RestoreTPL(OldTpl);

}

/**
    This function check if KBD LED command ED needs to be
    issued.
    If 'yes', sends ED command. No data is read.

    @param    *kbd - Pointer to keyboard buffer

    @retval   VOID

**/

VOID
CheckIssueLEDCmd (
    KEYBOARD    *Kbd )
{

    UINT8 bIndicators = Kbd->KeyData.KeyState.KeyToggleState & 7;    // SCRL/NUM/CPSL

    if (bIndicators != Kbd->Indicators && Kbd->LEDCommandState == 0) {
        // Don't issue LED command when data is pending
        if (IoRead8(KBC_CMDSTS_PORT) & KBC_OBF) return;
        Kbd->LEDCommandState = ED_COMMAND_ISSUED;
        WriteKeyboardData(0xED);
    }

}

/**
    If 0xFA is received as data, check for any pending ACK
    and take necessary action.

    @param    kbd   - Pointer to keyboard buffer
              bData - Data received from keyboard

    @retval   VOID

**/

VOID
ProcessKBDResponse (
    KEYBOARD    *Kbd,
    UINT8       Data )
{

    UINT8 bIndicators = Kbd->KeyData.KeyState.KeyToggleState & 7;    // SCRL/NUM/CPSL

    switch (Data) {
        case 0xFA:
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


        case 0xFE:
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
        case 0xFF:
                Kbd->LEDCommandState = 0;
                break;
        default:  break;
    }

}

/**
    Reads and returns byte of data from KBC data port. Also
    used as dummy KBC data process routine.

    @param    None

    @retval   Data read from KBC Data port.

**/

UINT8
KBCGetData ()
{
    UINT8   Data;
    Data = IoRead8(KBC_DATA_PORT);

    return Data;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
