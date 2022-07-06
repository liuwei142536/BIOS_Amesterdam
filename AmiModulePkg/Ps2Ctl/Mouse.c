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

/** @file mouse.c
    This is the PS/2 mouse EFI driver source file. It has 
    PS/2 mouse I/O support functions.

**/

//----------------------------------------------------------------------

#include <Efi.h>
#include <AmiLib.h>
#include "Ps2Ctl.h"
#include "Kbc.h"
#include "Ps2Mouse.h"

//----------------------------------------------------------------------

BOOLEAN                     Ps2MouseDetected = FALSE;
UINT8                       MouseData[5] = {0,};
BOOLEAN                     InsideGetMouseData = FALSE; 
BOOLEAN                     MouseResetRequired = FALSE;
UINT8                       ByteCount = NON_WHEEL_REPORT_FORMAT;
extern  BOOLEAN             InsideKbdReadKey;
extern  BOOLEAN             InsideOnWaitingOnKey;
extern  KEYBOARD            gKbd;
MOUSE gMouse = {0,};
extern  EFI_CPU_ARCH_PROTOCOL     *gCpuArch;
extern  EFI_LEGACY_8259_PROTOCOL  *mLegacy8259;
UINT8                       gMouseIrqInstall = FALSE;
extern  UINT8               gKeyboardIrqInstall;
extern                      BOOLEAN KBDEnableState;  
UINT8                       gCurrentMouseIndex = 0;
UINT8                       gGetMouseStateStart = FALSE;
VOID                        GetMouseDataFromIrq(VOID);
UINT8                       gMouseCommandActive = FALSE;
BOOLEAN                     InsideMouseReset;
extern  BOOLEAN             MouseEnableState;
extern  BOOLEAN             DetectPs2MouseValue;
extern  BOOLEAN             InstallKeyboardMouseAlways;
extern  BOOLEAN             KbdMsIrqSupport;
extern  BOOLEAN             InsideKbdReset;
extern  UINT32              IbFreeMaxTimeoutValue;

BOOLEAN PS2MouseFound = FALSE;
BOOLEAN MousePresenceCheck = FALSE;

typedef struct {
    EFI_SIMPLE_POINTER_STATE Data;
    BOOLEAN                  DataValid;
}PENDING_MOUSE_DATA;

PENDING_MOUSE_DATA PendingMouseData = {0,};

/**
    Waits for Output Buffer Full and then reads the data port

    @param    None

    @retval   KBC Data port data

**/

UINT8
ObFullReadM ()
{
    for (;;) {
        if ((IoRead8(KBC_CMDSTS_PORT) & (KBC_OBF|KBC_AUX_OBF)) == (KBC_OBF|KBC_AUX_OBF)) {
            return IoRead8(KBC_DATA_PORT);
        }
    }
}


/**
    This routine checks for the data availability in output
    buffer for a short period of time, if data is available
    within this time, it reads and returns the data from 
    output buffer.

    @param    data - Pointer to the byte to be updated
              msec - Milliseconds timeout

    @retval    Returns FALSE if data is successfully updated  
               (no timeout), Returns TRUE if time-out

**/

BOOLEAN
ObFullReadMTimeout (
    UINT8*  data,
    UINT32  msec )
{

    UINT8       bData;
    UINT32      loopcount = msec << 1;

    for (; loopcount; loopcount--) {

        bData = IoRead8(KBC_CMDSTS_PORT);

        if ((bData & (KBC_OBF|KBC_AUX_OBF)) == (KBC_OBF|KBC_AUX_OBF)) {

            *data = IoRead8(KBC_DATA_PORT);
            if (bData & 0x40) {
                *data = IoRead8(KBC_DATA_PORT);
                return TRUE;
            }
            else return FALSE;  // No timeout

        }

        if ((bData & (KBC_OBF|KBC_AUX_OBF)) == (KBC_OBF)){
            return TRUE;
        }

        gSysTable->BootServices->Stall(500);                // 0.5 msec

    }

    return TRUE;    // Timeout
}


/**
    Sends the given command to KBD, reads and returns the
    Acknowledgment byte returned from KBD.

    @param    bCmd     - Command to send to KBC
              *Data    - Pointer to data buffer
              Response - Response expected

    @retval   EFI_SUCCESS Data == Response
    @retval   EFI_DEVICE_ERROR Data != Response

**/

EFI_STATUS
ReadDeviceM (
    UINT8   bCmd, 
    UINT8   *Data, 
    UINT8   Response )
{
    EFI_STATUS  Status;
    UINT8       loop;
    Status = IbFreeTimeout(IbFreeMaxTimeoutValue);
    if (EFI_ERROR(Status)) {
        return Status;
    }
    IoWrite8(KBC_CMDSTS_PORT, 0xD4);
    IbFree();
    WriteKeyboardData(bCmd);
	
    // Data from ObFullReadMTimeout() may or may not be equal to Response.
	// So check for Match, if not match Retry for 3 times.  
    for (loop = 3; loop; loop--) {
        if (ObFullReadMTimeout(Data, 1000)) { 
            return EFI_DEVICE_ERROR;
        }
        
        if (*Data == Response) { 
            return EFI_SUCCESS;
        }
    }
    return EFI_DEVICE_ERROR; 
}



/**
    An interrupt handler for PS2 mouse


    @param    InterruptType  Interrupt type
              SystemContext  System context

    @retval    EFI_SUCCESS

**/
VOID 
EFIAPI 
MouseInterruptHandler (
    IN EFI_EXCEPTION_TYPE   InterruptType,
    IN EFI_SYSTEM_CONTEXT   SystemContext)
{

    // Disable interrupt to prevent interrupt chaining
    gCpuArch->DisableInterrupt(gCpuArch);

    // Post Code verifies the Mouse IRQ generation by sending command 0xD4(Write mouse device)
    // and data. For the data if the control comes to Mouse IRQ handler, and IRQ method is enabled
    // and active. Otherwise it will disable the Mouse IRQ 
    if (gMouseCommandActive){
        gMouseIrqInstall = TRUE;
        // Throw out the Dummy data that we sent to test the Mouse IRQ generation
        IoRead8(KBC_DATA_PORT);
        
    } else {
        // Handle the Mouse Data
        GetMouseDataFromIrq();
    }

    EnableAuxDevice();
    // Send End Of Interrupt command twice as PIC is connected 
    // in cascade mode, once for master and once for corresponding
    // slave 
    mLegacy8259->EndOfInterrupt(mLegacy8259, SYSTEM_MOUSE_IRQ);
    gCpuArch->EnableInterrupt(gCpuArch);
}

/**
    Initializes mouse driver

    @param     ppMouse - Pointer to mouse structure

    @retval    EFI_SUCCESS Mouse initialized
    @retval    EFI_DEVICE_ERROR Mouse not detected
    @retval    EFI_OUT_OF_RESOURCES Not enough memory for buffers

**/


EFI_STATUS
InitMOUSE ( 
    MOUSE**     ppMouse )
{
    MOUSE*      pmouse;
    EFI_STATUS  Status;
    UINT32      MouseVector = 0;
    UINT32      loopcount = 1000;

    pmouse = *ppMouse = &gMouse;

    MemSet( &pmouse->state, sizeof(pmouse->state), 0 );
    pmouse->saved_flags=0;

    if (EFI_ERROR(IsPs2MousePresent())) {
        //
        // Ps2 Mouse not Present.
        return EFI_DEVICE_ERROR;
    }
    // Ps2 Mouse Present. Install Simple Pointer protocol.
    (pmouse->iSmplPtr).Mode =   MallocZ(sizeof (EFI_SIMPLE_POINTER_MODE));

    if(!((pmouse->iSmplPtr).Mode)) { 
        return EFI_OUT_OF_RESOURCES;
    }
    // Check for IRQ support. 
    if(KbdMsIrqSupport){
        if (!gMouseIrqInstall){
            if(mLegacy8259 == NULL){
                Status = pBS->LocateProtocol(&gEfiLegacy8259ProtocolGuid, NULL, (VOID**)&mLegacy8259);
                if (EFI_ERROR(Status)) {
                    return Status;
                }
            }
            // Find the CPU Arch Protocol  
            if(gCpuArch == NULL){
                Status = pBS->LocateProtocol(&gEfiCpuArchProtocolGuid, NULL, (VOID**)&gCpuArch);
                if (EFI_ERROR(Status)) {
                    return Status;
                }
            }
            // Get Mouse vector
            Status = mLegacy8259->GetVector(mLegacy8259, SYSTEM_MOUSE_IRQ, (UINT8 *) & MouseVector);
            if (EFI_ERROR(Status))	{
                return Status;
            }
            // Register interrupt handler for mouse
            Status = gCpuArch->RegisterInterruptHandler(gCpuArch, MouseVector, MouseInterruptHandler);
            if (EFI_ERROR(Status)) {
                return Status;
            }

            // Enable Mouse interrupt
            Status = mLegacy8259->EnableIrq(mLegacy8259, SYSTEM_MOUSE_IRQ, FALSE);
            if (EFI_ERROR(Status)) {
                return Status;
            }

            // Enable Slave Interrupt
            Status = mLegacy8259->EnableIrq(mLegacy8259, SLAVE_IRQ, FALSE);
            if (EFI_ERROR(Status)) {
                return Status;
            }

            // Enable INT2 bit in command register. 
            Write8042CommandByte(0x67);

            // Is Mouse IRQ active?
            gMouseCommandActive = TRUE;
            Status = IbFreeTimeout(IbFreeMaxTimeoutValue);
            if (EFI_ERROR(Status)) {
                return Status;
            }
            IoWrite8(KBC_CMDSTS_PORT, 0xD4);
            IbFree();

            // Write mouse data
            WriteKeyboardData(0xf4);

            for (; loopcount; loopcount--) {
                if (gMouseIrqInstall) break;
                gSysTable->BootServices->Stall(500);        // 0.5 msec
            }
            gMouseCommandActive = FALSE;

            // If Mouse irq is not installed then disable interrupt
            if (!gMouseIrqInstall){
                // a dummy read after writing to mouse
                IoRead8(0x60);
                Write8042CommandByte(0x65);
                mLegacy8259->DisableIrq(mLegacy8259, SYSTEM_MOUSE_IRQ);
            }
        }
    }

    (pmouse->iSmplPtr).Mode->ResolutionX = 4;
    (pmouse->iSmplPtr).Mode->ResolutionY = 4;
    (pmouse->iSmplPtr).Mode->ResolutionZ = 1;
    (pmouse->iSmplPtr).Mode->RightButton = TRUE;
    (pmouse->iSmplPtr).Mode->LeftButton = TRUE;

    // Initialize pointer interface functions
    (pmouse->iSmplPtr).Reset = (EFI_SIMPLE_POINTER_RESET)MouseReset;
    (pmouse->iSmplPtr).GetState= (EFI_SIMPLE_POINTER_GET_STATE)MouseGetState;
    return EFI_SUCCESS;
}

/**
    If the Mouse data detected by Ps2 Driver dispatcher 
    after the mouse protocol installed , the data is proceeded and 
    returned via GetState function 

    @param    *Context 

    @retval    VOID


**/

VOID
ReadAndProcessMouseData (
    VOID    *Context )
{

    GetMouseData();

    // Already Packet is ready and nobody consumed. Throw out the new data and reset the device
    if (MouseData[ByteCount] == 1) { 
        MouseDummyGetData(Context);
    }

    return;
}



/**
    Implements Reset operation of SimplePointer protocol.
    As part of initialization process, the firmware/device will make a quick 
    but reasonable attempt to verify that the device is functioning. If the 
    ExtendedVerification flag is TRUE the firmware may take an extended amount 
    of time to verify the device is operating on reset. Otherwise the reset 
    operation is to occur as quickly as possible.

    @param    None

    @retval   Returns EFI_SUCCESS if the device was reset, otherwise returns 
              EFI_DEVICE_ERROR device is not functioning correctly and could 
              not be reset.

**/

EFI_STATUS
DetectPs2Mouse (
    BOOLEAN Flag )
{

    UINT8       bData = 0, b1, b2, loop;
    EFI_STATUS  Status;
    UINT8       Counter=200;

    // Flag is used to determine whether DETECT_PS2_MOUSE is to be considered or not
    if (!Flag) {
        Flag = DetectPs2MouseValue;
    }

    if ( Flag && !MousePresenceCheck ) {
        DisableKeyboard();
        PROGRESS_CODE(DXE_MOUSE_RESET);

        for (loop = 3; loop; loop--) {
            // Clear OBF
            while( IoRead8(KBC_CMDSTS_PORT) & KBC_OBF ) {
                IoRead8(KBC_DATA_PORT);
                // wait 10 ms for KBC OBF output.
                pBS->Stall(10*1000);
                Counter--;
                // If still keys are present after 2 sec. exit from the loop.
                if(Counter == 0) {
                    break;
                }
            }
            
            PROGRESS_CODE(EFI_PERIPHERAL_MOUSE|EFI_P_MOUSE_PC_SELF_TEST);
            // Resetting Mouse
            Status = ReadDeviceM(0xFF, &bData, 0xFA);

            // ACK received
            if (Status == EFI_SUCCESS) {    
                if (ObFullReadMTimeout(&b1, 1000)) { 
                    continue;
                }
                if (ObFullReadMTimeout(&b2, 1000)) {
                    continue;
                }
                if ( b1 == 0xAA && b2 == 0x00) {
                    // Enable wheel report format
                    if (EFI_ERROR(ReadDeviceM(0xF3, &bData, 0xFA))) {
                        continue;       // Set Sample Rate
                    }
                    if (EFI_ERROR(ReadDeviceM(0xC8, &bData, 0xFA))) {
                        continue;       // 200 samples/sec
                    }
                    if (EFI_ERROR(ReadDeviceM(0xF3, &bData, 0xFA))) {
                        continue;       // Set Sample Rate
                    }
                    if (EFI_ERROR(ReadDeviceM(0x64, &bData, 0xFA))) {
                        continue;       // 100 samples/sec
                    }
                    if (EFI_ERROR(ReadDeviceM(0xF3, &bData, 0xFA))) {
                        continue;       // Set Sample Rate
                    }
                    if (EFI_ERROR(ReadDeviceM(0x50, &bData, 0xFA))) {
                        continue;       // 80 samples/sec
                    }
                    ReadDeviceM(0xF2, &bData, 0xFA); // Read Mouse ID
                    ObFullReadMTimeout(&bData, 1000);
                    // If scroll wheel is supported Mouse Response = 3 
                    // else Mouse Response = 0
                    if (bData == 3) {
                        ByteCount = WHEEL_REPORT_FORMAT;
                    }
                    if (EFI_ERROR(ReadDeviceM(0xF3, &bData, 0xFA))) {
                        continue;       // Set Sample Rate
                    }    
                    if (EFI_ERROR(ReadDeviceM(0x64, &bData, 0xFA))) { 
                        continue;       // 20 samples/sec
                    }
                    if (EFI_ERROR(ReadDeviceM(0xE8, &bData, 0xFA))) {
                        continue;       // Set resolution
                    }
                    if (EFI_ERROR(ReadDeviceM(0x2, &bData, 0xFA))) { 
                        continue;        // 4 Count/mm
                    }
                    // Enable streaming
                    ReadDeviceM(0xF4, &bData, 0xFA);
                    MouseEnableState = TRUE;
                    DisableAuxDevice();
                    Ps2MouseDetected = TRUE;

                    PS2MouseFound = TRUE;
                    MousePresenceCheck = TRUE;
                    return EFI_SUCCESS;
                }
            }
        }
        MousePresenceCheck = TRUE;
        EnableKeyboard();
        // Ps2 Mouse Not Present. Return with Error.
        return EFI_DEVICE_ERROR;
    } else {
        // By default Ps2 mouse is enabled and Present
        MouseEnableState = TRUE;
        Ps2MouseDetected = TRUE;
        return EFI_SUCCESS;
    }
}

/**
    Implements Reset operation of SimplePointer protocol.
    As part of initialization process, the firmware/device will make a quick 
    but reasonable attempt to verify that the device is functioning. If the 
    ExtendedVerification flag is TRUE the firmware may take an extended amount 
    of time to verify the device is operating on reset. Otherwise the reset 
    operation is to occur as quickly as possible.

    @param    *self     - pointer to the mouse structure
              extended  - Indicates that the driver may perform a more exhaustive
                          verification operation of the device during reset.

                 
    @retval   Returns EFI_SUCCESS if the device was reset, otherwise returns 
              EFI_DEVICE_ERROR device is not functioning correctly and could 
              not be reset.

**/

EFI_STATUS 
EFIAPI 
MouseReset (
    MOUSE   *self,
    BOOLEAN extended)
{
    UINT8       bData = 0;
    UINT8       b1;
    UINT8       b2;
    UINT8       loop;
    EFI_STATUS  Status;
    UINT8       Counter=200;
    
    // If ExtendedVerification Flag is true then reset the mouse, set 
    // sampleRate and Resolution and Enable streaming.
    if(extended) {
        DisableKeyboard();
        PROGRESS_CODE(DXE_MOUSE_RESET);
        InsideMouseReset = TRUE;
        Write8042CommandByte(0x74);

        // Check for IRQ support
        if(KbdMsIrqSupport ) {
            // Disable the keyboard interrupt
            mLegacy8259->DisableIrq(mLegacy8259, SYSTEM_KEYBOARD_IRQ);

            // Disable the mouse interrupt
            mLegacy8259->DisableIrq(mLegacy8259, SLAVE_IRQ);
            mLegacy8259->DisableIrq(mLegacy8259, SYSTEM_MOUSE_IRQ);
        }

        for (loop = 3; loop; loop--) {
           // Clear OBF
            while( IoRead8(KBC_CMDSTS_PORT) & KBC_OBF ) {
                IoRead8(KBC_DATA_PORT);
                // wait 10 ms for KBC OBF output.
                pBS->Stall(10*1000);
                Counter--;
                // If still keys are present after 2 sec. exit from the loop.
                if(Counter == 0) {
                    break;
                }
            }
            
            PROGRESS_CODE(EFI_PERIPHERAL_MOUSE|EFI_P_MOUSE_PC_SELF_TEST);
            // Resetting Mouse
            Status = ReadDeviceM(0xFF, &bData, 0xFA);
            // ACK received
            if (Status == EFI_SUCCESS) {    

                ByteCount = NON_WHEEL_REPORT_FORMAT;

                if (ObFullReadMTimeout(&b1, 1000)) { 
                    continue;
                }
                if (ObFullReadMTimeout(&b2, 1000)) {
                    continue;
                }

                if ( b1 == 0xAA && b2 == 0x00) {
                    // Enable wheel report format
                    if (EFI_ERROR(ReadDeviceM(0xF3, &bData, 0xFA))) {
                        continue;       // Set Sample Rate
                    }
                    if (EFI_ERROR(ReadDeviceM(0xC8, &bData, 0xFA))) {
                        continue;       // 200 samples/sec
                    }
                    if (EFI_ERROR(ReadDeviceM(0xF3, &bData, 0xFA))) {
                        continue;       // Set Sample Rate
                    }
                    if (EFI_ERROR(ReadDeviceM(0x64, &bData, 0xFA))) {
                        continue;       // 100 samples/sec
                    }
                    if (EFI_ERROR(ReadDeviceM(0xF3, &bData, 0xFA))) {
                        continue;       // Set Sample Rate
                    }
                    if (EFI_ERROR(ReadDeviceM(0x50, &bData, 0xFA))) {
                        continue;       // 80 samples/sec
                    }
                    ReadDeviceM(0xF2, &bData, 0xFA); // Read Mouse ID
                    ObFullReadMTimeout(&bData, 1000);

                    // If scroll wheel is supported Mouse Response = 3 
                    // else Mouse Response = 0
                    if (bData == 3) {
                        ByteCount = WHEEL_REPORT_FORMAT;
                    }
                    if (EFI_ERROR(ReadDeviceM(0xF3, &bData, 0xFA))) {
                        continue;       // Set Sample Rate
                    }    
                    if (EFI_ERROR(ReadDeviceM(0x64, &bData, 0xFA))) { 
                        continue;       // 20 samples/sec
                    }
                    if (EFI_ERROR(ReadDeviceM(0xE8, &bData, 0xFA))) {
                        continue;       // Set resolution
                    }
                    if (EFI_ERROR(ReadDeviceM(0x2, &bData, 0xFA))) { 
                        continue;        // 4 Count/mm
                    }
                    // Enable streaming
                    ReadDeviceM(0xF4, &bData, 0xFA);
                    DisableAuxDevice();

                    PS2MouseFound = TRUE;
                    MousePresenceCheck = TRUE;
                    break;
               }
            }
        }
        // Check for IRQ support
        if(KbdMsIrqSupport ) {
            gCurrentMouseIndex = 0;
            pBS->SetMem(MouseData,ByteCount,0);
            // Enable the keyboard interrupt
            mLegacy8259->EnableIrq(mLegacy8259, SYSTEM_KEYBOARD_IRQ, FALSE);

            // Enable the mouse interrupt
            mLegacy8259->EnableIrq(mLegacy8259, SYSTEM_MOUSE_IRQ, FALSE);
            mLegacy8259->EnableIrq(mLegacy8259, SLAVE_IRQ, FALSE);
        }
        
        EnableKeyboard();
        Write8042CommandByte(0x47);
        InsideMouseReset = FALSE;
    }

    return EFI_SUCCESS;
}

/**
    Retrieves the current state of a pointer device.

    @param    *self  - Pointer to the mouse structure
              *state - Pointer to state machine buffer

    @retval    EFI_SUCCESS Mouse data ready
    @retval    EFI_NOT_READY Mouse data not ready

    @note  
     The function retrieves the current state of a pointer device. This includes
     information on the buttons associated with the pointer device and the 
     distance that each of the axes associated with the pointer device has been
     moved. If the state of the pointer device has not changed since the last 
     call to MouseGetState(), then EFI_NOT_READY is returned. If the state of 
     the pointer device has changed since the last call to MouseGetState(), then
     the state information is placed in State, and EFI_SUCCESS is returned. If a
     device error occurs while attempting to retrieve the state information,
     then EFI_DEVICE_ERROR is returned.
**/


EFI_STATUS 
EFIAPI 
MouseGetState (
    MOUSE                    *self,
    EFI_SIMPLE_POINTER_STATE *state)
{
    StdMouseDataPacket   *packet = NULL;
    EFI_STATUS           Status;
  
    // Checking whether PS2 mouse detection completed or not
    if(!MousePresenceCheck) {
        // TRUE is passing as argument to intimate, don't consider DETECT_PS2_MOUSE token
        // Also indicates to perform Mouse Detection
        DetectPs2Mouse(TRUE);
    }

    // If PS2 mouse detected, then read the data from mouse otherwise return
    if(PS2MouseFound) {
        if(KbdMsIrqSupport && gMouseIrqInstall && PendingMouseData.DataValid){
            // Disabling Interrupt
            gCpuArch->DisableInterrupt(gCpuArch);
    
            // If data is present in Temporary Buffer, form the packet with the 
            // those data and send
            state->RelativeMovementX = PendingMouseData.Data.RelativeMovementX;
            state->RelativeMovementY = PendingMouseData.Data.RelativeMovementY;
            state->RelativeMovementZ = PendingMouseData.Data.RelativeMovementZ;
            state->LeftButton = PendingMouseData.Data.LeftButton;
            state->RightButton = PendingMouseData.Data.RightButton;
    
            // Stating that the Data is read
            PendingMouseData.DataValid = FALSE;
    
            // Enabling Interrupt
            gCpuArch->EnableInterrupt(gCpuArch);
            
            return EFI_SUCCESS;
        }
    
        // Consume the MouseData to form the data Packet
        packet = (StdMouseDataPacket*)MouseData;
    
        //  Check if Mouse Data ready
        if (!MouseData[ByteCount]) {
            Status = GetMouseData();
            if (EFI_ERROR(Status)) {
                return Status;
            }
        }
        // Checking if Mouse Data is Ready
        if (MouseData[ByteCount]) {
            // Stating that the Mouse data is read
            MouseData[ByteCount] = 0;
    
            if( packet->flags & (MOUSE_X_OVF|MOUSE_Y_OVF) ){
                return EFI_DEVICE_ERROR;
            } else {
                // Forming the data packet with the Present Mouse Data
                state->RelativeMovementX = (packet->flags & MOUSE_X_NEG)? (signed char)packet->x:packet->x;
                state->RelativeMovementY = -(int)((packet->flags & MOUSE_Y_NEG)? (signed char)packet->y:packet->y);
                state->LeftButton = (packet->flags & BUTTON_LEFT)?TRUE:FALSE;
                state->RightButton = (packet->flags & BUTTON_RIGHT)?TRUE:FALSE;
    
                if (ByteCount == WHEEL_REPORT_FORMAT) {
                    state->RelativeMovementZ = ((signed char)packet->z);
                } else {
                    state->RelativeMovementZ = 0;
                } 
                return EFI_SUCCESS;
            }
        }

        return EFI_NOT_READY;
    } else {
        return EFI_NOT_READY;
    }
}

/**
    Detects the presence of mouse in KBC port.

    @param     None

    @retval    EFI_SUCCESS if mouse is detected
               EFI_NOT_FOUND if mouse is not detected

**/

EFI_STATUS
IsPs2MousePresent ()
{
    if ( InstallKeyboardMouseAlways ) {
        return EFI_SUCCESS;
    } else {
        return Ps2MouseDetected ? EFI_SUCCESS : EFI_NOT_FOUND;
    }
}


/**
    Call back for the WaitForInput event;
    checks whether the mouse state has changed and if so - 
    signals the event.

    @param    event - Event to signal
              *self - Pointer to the mouse structure    

    @retval    VOID

**/

VOID 
EFIAPI 
OnWaitingOnMouse (
    EFI_EVENT   event, 
    MOUSE       *self )
{
    //  Check if Mouse Data ready
    if (!MouseData[ByteCount]) {
        GetMouseData();
    }

    if (MouseData[ByteCount]) gSysTable->BootServices->SignalEvent(event);

}

/**
    Enables Mouse and returns data if available 

    @param    VOID

    @retval    EFI_SUCCESS Mouse data returned
    @retval    EFI_NO_RESPONSE No response from mouse

**/
VOID
GetMouseDataFromIrq (VOID)
{
    UINT8    Port64flag = IoRead8(KBC_CMDSTS_PORT); 

    // If previous is data is not consumed and there is an interrupt call, copy the 
    // Previous data to a Temporary Buffer.
    if(MouseData[ByteCount] == 1){
       PendingMouseData.Data.RelativeMovementX = (MouseData[0] & MOUSE_X_NEG)? (signed char)MouseData[1]:MouseData[1];
       PendingMouseData.Data.RelativeMovementY = -(int)((MouseData[0] & MOUSE_Y_NEG)? (signed char)MouseData[2]:MouseData[2]);
       PendingMouseData.Data.LeftButton = (MouseData[0] & BUTTON_LEFT)?TRUE:FALSE;
       PendingMouseData.Data.RightButton = (MouseData[0] & BUTTON_RIGHT)?TRUE:FALSE;

       if (ByteCount == WHEEL_REPORT_FORMAT) {
           PendingMouseData.Data.RelativeMovementZ = ((signed char)MouseData[3]);
       } else {
           PendingMouseData.Data.RelativeMovementZ = 0;           
       }
       // Stating that the data in Temporary Buffer is available
       PendingMouseData.DataValid = TRUE;
       MouseData[ByteCount] = 0;
    }

    if ((Port64flag & (KBC_OBF|KBC_AUX_OBF)) == (KBC_OBF|KBC_AUX_OBF)) {
        MouseData[gCurrentMouseIndex] = KBCGetData();
        if (gCurrentMouseIndex ==(ByteCount-1)){ 
            DisableAuxDevice();
            gCurrentMouseIndex = 0;
            MouseData[ByteCount] = 1;
            return;
        }
        gCurrentMouseIndex++;
    }

    return;
}

/**
    Enables Mouse and returns data if available 

    @param     None

    @retval    EFI_SUCCESS Mouse data returned
    @retval    EFI_NO_RESPONSE No response from mouse

**/
EFI_STATUS
GetMouseData () 
{
    UINT8       Index;
    BOOLEAN     MouseDataRead;
    KEYBOARD    *kbd = &gKbd;
    UINT8       KbdData[20], KbdDataIndex=0, Data8;
    UINTN       Delay, DelayPeriod = TIMEOUT_FOR_1BYTE_MOUSEPACKET_READ;
    BOOLEAN     MouseDataValid = FALSE;
    UINT8       ByteReadCount = 0;
    BOOLEAN     MouseDataReadFail = FALSE;
    // If mouse irq is supported and installed then return with success
    if(KbdMsIrqSupport){                          
        if (gMouseIrqInstall){
            return EFI_SUCCESS;
        }
    }

    //  Return if we are in the middle of KBD get key
    if (InsideGetMouseData || InsideKbdReadKey || InsideKbdReset || InsideMouseReset) {
        return EFI_DEVICE_ERROR;
    }

    if (MouseData[ByteCount] == 1) return EFI_SUCCESS;

    //  Try to complete the LED sequence
    for (Index = 0; (Index < 40) && kbd->LEDCommandState; Index++ ) {
        // if irq is supported but not installed yet then call
        // Ps2DataDispatcher to check whether data is available on port or not
        if(KbdMsIrqSupport)
            if (!gKeyboardIrqInstall)
                PS2DataDispatcher(NULL);
        pBS->Stall(500);        
    }

    //  LED sequence is still on, restart it
    if (kbd->LEDCommandState) {
        if(KbdMsIrqSupport)          
            if (!gKeyboardIrqInstall){
                ProcessKBDResponse(kbd, 0xfe);
            }
    }

    InsideGetMouseData = TRUE;

    if ((IoRead8(KBC_CMDSTS_PORT) & (KBC_OBF|KBC_AUX_OBF)) == KBC_OBF) {
        KbdData[KbdDataIndex] = KBCGetData();
        KbdDataIndex++;
    }

    EnableAuxDevice();
    if (MouseResetRequired) {
        DisableKeyboard();
        if ((IoRead8(KBC_CMDSTS_PORT) & (KBC_OBF|KBC_AUX_OBF)) == KBC_OBF) {
            KbdData[KbdDataIndex] = KBCGetData();
            KbdDataIndex++;
        }
        ReadDeviceM(0xF4, &Index, 0xFA);
        MouseResetRequired = FALSE;
    }

    MouseData[ByteCount] = 0;

    DelayPeriod = TIMEOUT_FOR_1BYTE_MOUSEPACKET_READ;                  // Delay for the first byte
    for (Index = 0; Index < ByteCount; Index++) {

        MouseDataRead = FALSE;

        for (Delay = 0; Delay < DelayPeriod; Delay++) {
            Data8 = IoRead8(KBC_CMDSTS_PORT);
            if ((Data8 & (KBC_OBF|KBC_AUX_OBF)) == KBC_OBF) {           
                KbdData[KbdDataIndex] = KBCGetData();
                KbdDataIndex++;
            }
            if ((Data8 & (KBC_OBF|KBC_AUX_OBF)) == (KBC_OBF|KBC_AUX_OBF)) {         
                MouseData[Index] = KBCGetData();
                MouseDataValid = TRUE;
                MouseDataRead = TRUE;
                if (Index == (ByteCount-1)) DisableAuxDevice();
                break;              
            }
            pBS->Stall(100);
        }
        DelayPeriod = TIMEOUT_FOR_REM_MOUSEPACKET_READ;          // Delay for the Second & Third byte
        if (Index == (ByteCount-1) && MouseDataRead) {
                MouseData[ByteCount] = 1;
        } else if (Index == (ByteCount-1) && MouseDataValid) {
            MouseResetRequired=TRUE;
        }
        if (MouseDataRead) {
            continue;
        } else {
            if (Index >=1) //Failed at 2nd or 3rd Byte
                MouseDataReadFail = TRUE;
            break;
        }

    }
	
    //If Mouse data is not read properly then adding extra delay to flush out remaining mouse data.
    //So that it won't affect next sequecne of Mouse data
    if (MouseDataReadFail) {
        ByteReadCount = Index;
        DisableAuxDevice();
        DelayPeriod = 50; 
        for ( ; ByteReadCount < ByteCount; ByteReadCount++) {
            for (Delay = 0; Delay < DelayPeriod; Delay++) {
                Data8 = IoRead8(KBC_CMDSTS_PORT);
                if ((Data8 & (KBC_OBF|KBC_AUX_OBF)) == (KBC_OBF|KBC_AUX_OBF)) {
                    MouseData[ByteReadCount] = KBCGetData();
                    break; 
                }
                pBS->Stall(1000);
            }
        }
        ReadDeviceM(0xF4, &Index, 0xFA);
    }

    //  Reset mouse counters if the mouse sequence is not correct 
    if ( (MouseData[ByteCount] == 1 && !(MouseData[0] & 8)) ) {
        for (; Index; Index--) {
        }
        MouseData[ByteCount] = 0;
        DisableKeyboard();
        if ((IoRead8(KBC_CMDSTS_PORT) & (KBC_OBF|KBC_AUX_OBF)) == KBC_OBF) {
            KbdData[KbdDataIndex] = KBCGetData();
            KbdDataIndex++;
        }
        ReadDeviceM(0xF4, &Index, 0xFA);
    }

    EnableKeyboard();                   
    DisableAuxDevice();

    for (Index =0 ; KbdDataIndex; KbdDataIndex--, Index++) {
        ProcessKBDData(kbd, KbdData[Index]);        
    }

    InsideGetMouseData = FALSE;

    return EFI_SUCCESS;
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
