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

/** @file SysKbc.c
    AMI USB keyboard driver data conversion and presentation
    routines, KBC is present

**/

#include "AmiUsb.h"
#include "UsbKbd.h"
#include <Library/AmiUsbHcdLib.h>
extern USB_GLOBAL_DATA *gUsbData;
extern UINT8 aTypematicRateDelayTable[];
extern EFI_EMUL6064KBDINPUT_PROTOCOL* gKbdInput;
extern EFI_EMUL6064MSINPUT_PROTOCOL*       gMsInput;
extern EFI_EMUL6064TRAP_PROTOCOL* gEmulationTrap;
extern VOID USBKB_LEDOn();
extern USB_DATA_LIST   *gUsbDataList;

UINT8   aStaticSet2ToSet1ScanCode[133]  =
        {   0x000,0x0E5,0x0F5,0x006,0x016,0x026,0x01E,0x03D,    //; 00h - 07h
            0x0DC,0x0DD,0x0ED,0x0FD,0x00E,0x087,0x0B6,0x035,    //; 08h - 0Fh
            0x0D4,0x03E,0x0AE,0x07C,0x017,0x07F,0x0EF,0x02D,    //; 10h - 17h
            0x0CC,0x074,0x09E,0x007,0x00F,0x077,0x0E7,0x025,    //; 18h - 1Fh
            0x0C4,0x08E,0x096,0x0FE,0x06F,0x0D7,0x0DF,0x01D,    //; 20h - 27h
            0x0BC,0x036,0x086,0x0F6,0x05F,0x067,0x0CF,0x015,    //; 28h - 2Fh
            0x0B4,0x076,0x07E,0x0E6,0x0EE,0x057,0x0C7,0x00D,    //; 30h - 37h
            0x0AC,0x06C,0x06E,0x0DE,0x04F,0x0BF,0x0B7,0x005,    //; 38h - 3Fh
            0x0A4,0x066,0x0D6,0x047,0x03F,0x0A7,0x0AF,0x0FC,    //; 40h - 47h
            0x09C,0x05E,0x056,0x0CE,0x0C6,0x037,0x09F,0x0F4,    //; 48h - 4Fh
            0x094,0x064,0x0BE,0x05C,0x02F,0x097,0x0EC,0x08C,    //; 50h - 57h
            0x02E,0x04E,0x01F,0x027,0x054,0x0A6,0x0E4,0x04C,    //; 58h - 5Fh
            0x055,0x04D,0x044,0x03C,0x034,0x02C,0x08F,0x024,    //; 60h - 67h
            0x01C,0x085,0x014,0x0A5,0x0C5,0x00C,0x004,0x084,    //; 68h - 6Fh
            0x06D,0x065,0x07D,0x09D,0x095,0x0BD,0x0F7,0x0D5,    //; 70h - 77h
            0x045,0x08D,0x075,0x0AD,0x046,0x0B5,0x0CD,0x05D,    //; 78h - 7Fh
            0x0FB,0x0F3,0x0EB,0x0F5,0x05D                   //; 80h - 84h
        };

//----------------------------------------------------------------------------
//------ USA ENGLISH keyboard -------
// USB Key Code to Scan Code Set 2

UINT8   aUSBKeyCodeToScanCodeSet2Table[256]     =
        {
            0x00, 0x00, 0x00, 0x00, 0x1C, 0x32, 0x21, 0x23,  //00 - 07h
            0x24, 0x2B, 0x34, 0x33, 0x43, 0x3B, 0x42, 0x4B,  //08 - 0Fh
            0x3A, 0x31, 0x44, 0x4D, 0x15, 0x2D, 0x1B, 0x2C,  //10 - 17h
            0x3C, 0x2A, 0x1D, 0x22, 0x35, 0x1A, 0x16, 0x1E,  //18 - 1Fh
            0x26, 0x25, 0x2E, 0x36, 0x3D, 0x3E, 0x46, 0x45,  //20 - 27h
            0x5A, 0x76, 0x66, 0x0D, 0x29, 0x4E, 0x55, 0x54,  //28 - 2Fh
            0x5B, 0x5D, 0x5D, 0x4C, 0x52, 0x0E, 0x41, 0x49,  //30 - 37h
            0x4A, 0x58, 0x05, 0x06, 0x04, 0x0C, 0x03, 0x0B,  //38 - 3Fh
            0x83, 0x0A, 0x01, 0x09,                         // 40 - 43h
            0x78, 0x07, PRINT_SCREEN,0x7E,                  // 44 - 47h
            PAUSE_KEY, INSERT_KEY, HOME_KEY,                    // 48 - 4Ah
            PAGE_UP_KEY, DEL_KEY, END_KEY,                  // 4B - 4Dh
            PAGE_DOWN_KEY, RIGHT_KEY,                       // 4E - 4Fh
            LEFT_KEY, DOWN_KEY, UP_KEY, 0x77,                   // 50 - 53h
            SLASH_KEY, 0x7C, 0x7B, 0x79,                        // 54 - 57h
            RIGHT_ENTER, 0x69, 0x72, 0x7A,                  // 58 - 5Ch
            0x6B, 0x73, 0x74, 0x6C,                         // 5D - 5Fh
            0x75, 0x7D, 0x70, 0x71, 0x61, APP_MS_KEY,00, 0x00,  // 60 - 67h
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // 68 - 6Fh
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // 70 - 77h
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      //78 - 7Fh

            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x51,     // 80 - 87h
            0x13, 0x6A, 0x64, 0x67, 0x00, 0x00, 0x00, 0x00,     // 88 - 8Fh
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // 90 - 97h
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // 98 - 9Fh
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // A0 - A7h
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // A8 - AFh
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // B0 - B7h
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // B8 - BFh
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // C0 - C7h
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // C8 - CFh
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // D0 - D7h
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // D8 - DFh
            0x14, 0x12, 0x11, LEFT_MS_KEY,                      // E0 - E3h
            RIGHT_CTRL, 0x59, RIGHT_ALT, RIGHT_MS_KEY,          // E4 - E7h
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // E8 - EFh
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // F0 - F7h
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00      // F8 - FFh
        };
// Keyboard Scan Code Set 3 Table
#define SCAN_CODE_SET_TABLE_SIZE_DW     8   // In double word
UINT8   aStaticScanCodeSet3Table[32]    =
        {
            0x00, 0x15, 0x00, 0x28, 0x94, 0x29, 0xA4, 0x2A,
            0xA8, 0x2A, 0xA8, 0x2A, 0xA8, 0x2A, 0xA0, 0x2A,
            0xA8, 0x2A, 0xA8, 0x2A, 0xA8, 0x0A, 0xA4, 0x0A,
            0x8A, 0x22, 0x20, 0x00, 0x00, 0x00, 0x00, 0x02
        };


UINT8   aStaticExtendedKeyScan2Table[6] =
        {   RIGHT_ALT, RIGHT_CTRL, RIGHT_ENTER, LEFT_MS_KEY,
            RIGHT_MS_KEY, APP_MS_KEY };

UINT8   aStaticKeyPadScan2Table[10]     =
        {   END_KEY, LEFT_KEY, HOME_KEY, INSERT_KEY,
            DEL_KEY, DOWN_KEY, RIGHT_KEY, UP_KEY,
            PAGE_DOWN_KEY, PAGE_UP_KEY };


UINT8   aStaticExtendedKeyScan3Table[6] =
        {   0x39, 0x58, 0x79, 0x8B, 0x8C, 0x8D};
UINT8   aStaticKeyPadScan3Table[10]     =
        {   0x65, 0x61, 0x6E, 0x67, 0x64, 0x60, 0x6A, 0x63, 0x6D, 0x6F };

//----------------------------------------------------------------------------
// Table used to find length of scan code needed. Please refer to comments
// in the code below (somewhere) for the usage.
//
UINT8   aScanCodeLengthTable_1112[4]    = {0x01, 0x01, 0x01, 0x02};
UINT8   aScanCodeLengthTable_2223[4]    = {0x02, 0x02, 0x02, 0x003};
UINT8   aScanCodeLengthTable_4446[4]    = {0x04, 0x04, 0x04, 0x06};
UINT8   aScanCodeLengthTable_4500[4]    = {0x04, 0x05, 0x00, 0x00};
UINT8   aScanCodeLengthTable_4545[4]    = {0x04, 0x05, 0x04, 0x05};
UINT8   aScanCodeLengthTable_6800[4]    = {0x06, 0x08, 0x00, 0x00};


/**
    This routine checks the input buffer free bit and waits till
    it is set by the keyboard controller

    @param None

    @retval USB_SUCCESS    If Input buffer is Empty
    @retval USB_ERROR      If Input buffer is Full 

**/
UINT8
KBC_WaitForInputBufferToBeFree ()
{
    UINT8   Count = 16,
            Status;
    UINT16  Count2  = 0xFFFF;
    do {
        Status     = (UINT8)(ByteReadIO(KBC_STATUS_REG) & BIT1);
        while(Status) {
            if(Count2 == 0)
                break;
            Status = (UINT8)(ByteReadIO(KBC_STATUS_REG) & BIT1);
            --Count2;
        }
        --Count;
    } while (Count && Status);

    if (!Status) {
        return USB_SUCCESS;
    } else {
        return USB_ERROR;
    }
}


/**
    This routine checks the output buffer full bit and waits till
    it is set by the keyboard controller

    @param VOID

    @retval USB_SUCCESS      If OutPut buffer is Full
    @retval USB_ERROR        If Output buffer Empty

**/

UINT8
KBC_WaitForOutputBufferToBeFilled ()
{
    UINT8   Count = 16,
            Status;
    UINT16  Count2 = 0xffff;
    do {
        Status = (UINT8)(ByteReadIO(KBC_STATUS_REG) & BIT0);
        while (!Status) {
            if(Count2 == 0) break;
            Status = (UINT8)(ByteReadIO(KBC_STATUS_REG) & BIT0);
            Count2--;
        }
        Count--;
    } while (Count && (!Status));

    if (Status) {
        return USB_SUCCESS;
    } else {
        return USB_ERROR;
    }
}


/**
    This routine till the keyboard controller sends a data byte

    @param Data           Data buffer

    @retval USB_SUCCESS   If OutPut buffer is Full
    @retval USB_ERROR     If Output buffer Empty

**/

UINT8
KBC_ReadDataByte (
    IN UINT8    *Data )
{
    UINT8    Status;

    Status = KBC_WaitForOutputBufferToBeFilled();

    if (Status == USB_SUCCESS) {
        *Data = ByteReadIO(KBC_DATA_REG);
    }

    return Status;
}


/**
    This routine sends the command byte to the keyboard
    controller

    @param  Cmd           Command byte

    @retval USB_SUCCESS   On success
    @retval USB_ERROR     On error

**/

UINT8
KBC_WriteCommandByte (
    IN UINT8    Cmd )
{
    UINT8  Status;

    Status = KBC_WaitForInputBufferToBeFree();

    if (Status == USB_SUCCESS) {
        ByteWriteIO(KBC_COMMAND_REG, Cmd);
    } 

    Status = KBC_WaitForInputBufferToBeFree();
    return Status;
}


/**
    This routine sends the sub-command byte to the keyboard
    controller

    @param  Cmd           Command byte

    @retval None

**/

VOID
KBC_WriteSubCommandByte (UINT8  Cmd)
{
    ByteWriteIO(KBC_SUBCOMMAND_REG, Cmd);
    KBC_WaitForInputBufferToBeFree();
}


/**
    This routine sends a command and receives the response byte
    for that command

    @param Cmd       Command to be sent
    @param Data      Data to be Read

    @retval USB_SUCCESS   On success
    @retval USB_ERROR     On error

**/

UINT8
KBC_ReadKeyboardControllerData (
    IN UINT8    Cmd,
    IN UINT8    *Data )
{
    UINT8  Status;

    Status = KBC_WaitForInputBufferToBeFree();   // Wait for input buffer to be free

    if (Status == USB_SUCCESS) {
        Status = KBC_WriteCommandByte(Cmd);      // Send the command
        if (Status == USB_SUCCESS) {
            Status = KBC_ReadDataByte(Data);      // Read the data
        }
    }
    return Status;
}

/**
    This routine writes a data byte to the keyboard controller
    by first sending a command byte first

    @param Cmd            Command to be sent
    @param Data           Data to be sent

    @retval USB_SUCCESS   On success
    @retval USB_ERROR     On error

**/

UINT8
KBC_WriteKeyboardControllerData (
    IN UINT8    Cmd,
    IN UINT8    Data )
{
    UINT8    Status;

    Status = KBC_WaitForInputBufferToBeFree();   // Wait for input buffer to be free

    if (Status == USB_SUCCESS) {
        Status = KBC_WriteCommandByte(Cmd);         // Send the command
        if (Status == USB_SUCCESS) {    
            ByteWriteIO(KBC_DATA_REG, Data);        // Write the data
        }
    }

    Status = KBC_WaitForInputBufferToBeFree();   // Wait for input buffer to be free
    return Status;
}


/**
    This routine updates LEDs on Legacy keyboard

    @param This     EFI_EMUL6064KBDINPUT_PROTOCOL pointer
    @param Data     LED state data
    
    @retval EFI_SUCCESS  

**/
EFI_STATUS
EFIAPI
SYSKBC_UpdateLEDState (
    EFI_EMUL6064KBDINPUT_PROTOCOL* This,
    UINT8 Data
)
{

    return EFI_SUCCESS;
}

/**
    This routine will read the CCB and store it .

    @param None
    
    @retval None

**/
VOID
SYSKBC_GetAndStoreCCB()
{
    UINT8       Status;
    UINT8       Cmd;
    UINT8       Data = 0;
    UINT8       Temp;
    UINT8       ReturnStatus;


    //
    //If Emulation Active disable the SMI Generation
    //
    if(gEmulationTrap) 
        gEmulationTrap->TrapDisable(gEmulationTrap);

    Status = ByteReadIO(KBC_STATUS_REG);
    if(Status & BIT0)
    {
        Data = ByteReadIO(KBC_DATA_REG);
    }

    ReturnStatus = KBC_ReadKeyboardControllerData(0x20, &Temp);
    if (ReturnStatus == USB_SUCCESS) {
        gUsbData->bCCB = Temp;
    }

    if(Status & BIT0)
    {
        Cmd = 0xD2;
        if(Status & BIT5)
        {
            Cmd = 0xD3;
        }
        KBC_WriteKeyboardControllerData(Cmd, Data);
    }
    //
    //If Emulation Active Enable the SMI Generation
    //
    if(gEmulationTrap) 
        gEmulationTrap->TrapEnable(gEmulationTrap);
}


/**
    This routine is called to send the mouse data to the KB
    controller

    @param None

    @retval 0xFF       data processed
    @retval 0          data not processed

**/

UINT8 USBMSSendMouseData()
{
    UINT16  EbdaSeg;
    UINT8   Data;
    UINT8   *Ebda = NULL;
    UINT8   PackageSize;
    UINT8   Index;

  

    EFI_STATUS  Status;
    VOID        *AddrCheck = ((VOID*)0x40E); 
    Status = AmiUsbValidateMemoryBuffer(AddrCheck, sizeof(UINT32));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)) return 0;

    
    EbdaSeg = *(UINT16*)0x40E;
    Ebda = (UINT8*)((UINTN)EbdaSeg<<4);


    Status = AmiUsbValidateMemoryBuffer((VOID*)Ebda, 0x100);
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)) return 0;


    PackageSize = Ebda[0x27] & 7;
    Index = 0;

    if (gUsbDataList->MouseInputBufferHeadPtr == gUsbDataList->MouseInputBufferTailPtr) {
        return 0;
    }
   
    if ((gUsbData->bCCB & CCB_MOUSE_DISABLED) || 
        !(gUsbData->bCCB & CCB_MOUSE_INTRPT)) {
        gUsbDataList->MouseInputBufferHeadPtr = &gUsbData->UsbMsInputBuffer[0];
        gUsbDataList->MouseInputBufferTailPtr = &gUsbData->UsbMsInputBuffer[0];
        return 0;
    }

    Ebda[0x26] &= ~7;
    

    if ((gUsbData->UsbFeature & USB_DEV_MOUSE_UPDATE_EBDA_DATA) == USB_DEV_MOUSE_UPDATE_EBDA_DATA) {
        for (; Index<PackageSize; Index++) {
            Ebda[0x28+Index] = USBMouse_GetFromMouseBuffer();
            Ebda[0x26]++;
        }
    }

    Data = USBMouse_GetFromMouseBuffer();
    KBC_WriteKeyboardControllerData(0xD3, Data);   // Send the last byte of data
    KBC_WaitForOutputBufferToBeFilled();

    //
    // Flag indicates data from USB mouse
    //
    //gUsbData->bMouseStatusFlag |= MOUSE_DATA_FROM_USB_BIT_MASK;

    //
    // Indicate that first byte already sent
    //
    //gUsbData->bMouseStatusFlag &= ~(MOUSE_DATA_READY_BIT_MASK);

    //USBMSUpdateMouseInterface();    // Enable/disable mouse interface

    return  0xFF;
}


/**
    This routine puts a byte into the mouse input buffer.
    Mouse input buffer pointers are also updated

    @param Data   Byte to be put in the mouse input buffer

    @retval None

**/
void
USBMouse_SendToMouseBuffer(UINT8 Data)
{
    *gUsbDataList->MouseInputBufferHeadPtr = Data;  // Put the byte in the buffer
     gUsbDataList->MouseInputBufferHeadPtr++;         // Advance the buffer pointer

    //
    // Check whether the buffer end is reached, if buffer end reached
    // then position the buffer pointer to the start
    //
    if (gUsbDataList->MouseInputBufferHeadPtr ==
        (gUsbData->UsbMsInputBuffer + sizeof(gUsbData->UsbMsInputBuffer)))
    {
        gUsbDataList->MouseInputBufferHeadPtr = gUsbData->UsbMsInputBuffer;
    }
}

/**
    This routine retrieves a byte from the mouse input buffer.
    Mouse input buffer pointers are also updated

    @param None

    @retval Data       Byte to be taken from the mouse input buffer

**/

UINT8 USBMouse_GetFromMouseBuffer   ()
{
     UINT8   Data = *gUsbDataList->MouseInputBufferTailPtr;
    *gUsbDataList->MouseInputBufferTailPtr = 0;
     gUsbDataList->MouseInputBufferTailPtr++;

    //
    // Check whether the buffer end is reached, if buffer end reached
    // then wrap around to the start
    //
    if (gUsbDataList->MouseInputBufferTailPtr ==
        (&gUsbData->UsbMsInputBuffer[0] + sizeof(gUsbData->UsbMsInputBuffer)))
    {
        gUsbDataList->MouseInputBufferTailPtr = &gUsbData->UsbMsInputBuffer[0];
    }
    return  Data;
}


/**
    This routine is invoked periodically (every 8msec) to send
    USB keyboard and mouse data to the keyboard controller
    or to the keyboard controller emulation code

    @param VOID

    @retval VOID

**/

EFI_STATUS
SYSKBC_SendKBCData()
{
    UINT8       bCmd = 0,
                bTemp1 = 0,
                bTemp = 0,
                bFlag = 1;
    UINT16      wTemp;
//  UINT16    ebdaSeg = *(UINT16*)0x40E;
//  UINT8    *ebda = (UINT8*)((UINTN)ebdaSeg<<4);
//  UINT8    mouse_flag3;
    UINT8       Status;
    UINT8       Temp;

    //Is KBC access allowed?
    if (gUsbData->IsKbcAccessBlocked) {
        return EFI_NOT_READY;  
    }
//  USBKBC_CheckAutoRepeat();

    //
    //If Emulation Active disable the SMI Generation
    //
    if(gEmulationTrap){
        if (!gEmulationTrap->TrapDisable(gEmulationTrap)) {
            // Data is still pending. So don't push data.
            goto enableRepeat_Exit;
        }
    }

    //
    // Check whether BIOS is processing IRQ1 or IRQ12
    //
    ByteWriteIO(0x20, 0xB);
    if (ByteReadIO(0x20) & 2) {
        goto enableRepeat_Exit;
    }

    ByteWriteIO(0xA0, 0xB);
    if (ByteReadIO(0xA0) & 0x10) {
        goto enableRepeat_Exit;
    }

    if (ByteReadIO(KBC_STATUS_REG) & BIT0){
        goto enableRepeat_Exit;
    }

    for(;;) {
        //
        // Check the data transmit order
        //
        wTemp   = (UINT16)(gUsbData->UsbKbcStatusFlag &
                                        KBC_DATA_TX_ORDER_BIT_MASK);
        if(wTemp == KBC_DATA_TX_ORDER_KB_FIRST) {
            bFlag   = 0;
            bCmd    = 0xD2;     // Send keyboard data
            //
            // Check for keyboard data and then for mouse data
            //
            if(gUsbDataList->KbcCharacterBufferHead !=
                                        gUsbDataList->KbcCharacterBufferTail)
                break;
            //
            // Check for mouse data
            //

            if ((gUsbData->UsbDevSupport & USB_MS_DEV_SUPPORT) == USB_MS_DEV_SUPPORT){
                bCmd    = 0xD3;
                if(gUsbDataList->MouseInputBufferHeadPtr !=
                    gUsbDataList->MouseInputBufferTailPtr) {
                    break;
                }
            }

            bFlag   = 1;
        } else {
            bFlag   = 0;

            if ((gUsbData->UsbDevSupport & USB_MS_DEV_SUPPORT) == USB_MS_DEV_SUPPORT){
                bCmd    = 0xD3;
                if(gUsbDataList->MouseInputBufferHeadPtr !=
                    gUsbDataList->MouseInputBufferTailPtr) {
                    //
                    // Check for keyboard data and then for mouse data
                    //
                    break;
                }
            }

            //
            // Check for keyboard data
            //
            bCmd    = 0xD2;
            if(gUsbDataList->KbcCharacterBufferHead !=
                                    gUsbDataList->KbcCharacterBufferTail)
                break;
            bFlag   = 1;
        }
        break;
    }

    if(bFlag) {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_8, "USBKBC_SendKBCData: Scan Code Buffer Empty \n");
        //
        // Check whether the scan code buffer is empty
        //
        if(gUsbDataList->KbcScanCodeBufferPtr != gUsbData->KbcScanCodeBufferStart)
        {
            goto enableRepeat_Exit;
        }
        //
        // Get pointer to the HC driver
        // Scan code buffer is empty. Stop the key repeat by stopping repeat TD
        //
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_8, "HCDDisableKeyRepeat \n");
        USBKeyRepeat(NULL, 1);  // Disable Key repeat

        if(gEmulationTrap) {
           gEmulationTrap->TrapEnable(gEmulationTrap);
        }
        return EFI_SUCCESS;
    }

    for (;;) {
        Status = KBC_WaitForInputBufferToBeFree();
        if (Status == USB_SUCCESS) {
            ByteWriteIO(KBC_COMMAND_REG, gUsbData->UsbSendCmdToKbc);
        } else {
            goto enableRepeat_Exit;
        }
        //
        // Check the return status
        //
        do {
            bTemp1  = ByteReadIO(KBC_STATUS_REG);
            if(bTemp1 & BIT0) {
                bFlag   = 3 ;
                break;
            }
        } while (bTemp1 & BIT1);
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_8, "USBKBC_SendKBCData: bFlag %x\n",bFlag);
        if (bFlag == 3) break;
        //
        // Update the data transmit order
        //
        gUsbData->UsbKbcStatusFlag =
            (UINT16)((gUsbData->UsbKbcStatusFlag) +
                            (KBC_DATA_TX_ORDER_INC_VALUE));
        wTemp   = (UINT16)(gUsbData->UsbKbcStatusFlag &
                                    KBC_DATA_TX_ORDER_BIT_MASK);
        if((wTemp & KBC_DATA_TX_ORDER_BIT_MASK) == 0) {
            gUsbData->UsbKbcStatusFlag &=
                    ~((UINT16)KBC_DATA_TX_ORDER_BIT_MASK);
        }

        //
        // Get the CCB
        //
        if ((gEmulationTrap == NULL) || (gUsbData->bCCB & CCB_MOUSE_DISABLED) || 
            !(gUsbData->bCCB & CCB_MOUSE_INTRPT) || ((bCmd & 1) == 0)) {
            Status = KBC_WriteCommandByte(0x20);      // Send command to KBC
            if (Status == USB_SUCCESS) {
                Status = KBC_ReadDataByte(&Temp);    // Get data from KBC
                if (EFI_ERROR(Status)) {
                    goto enableRepeat_Exit;
                }
                gUsbData->bCCB = Temp;
            } else {
                goto enableRepeat_Exit ;
            }
        }
        
        //
        // Check for mouse data
        //
        if((bCmd & 1) == 0) {
            //
            // Check whether keyboard is disabled
            //
            if((gUsbData->bCCB & CCB_KEYBOARD_DISABLED) == 0) {
                //
                // Check whether the current mode is IRQ
                // Check whether password is enabled
                //
                if((gUsbData->UsbKbcStatusFlag &
                            KBC_PASSWORD_FLAG_BIT_MASK) == 0)
                    bTemp   = USBKBC_GetFromCharacterBuffer();

                    if (bTemp == 0xff) { 
                        goto enableRepeat_Exit; 
                    }
                KBC_WriteKeyboardControllerData(bCmd , bTemp);
                KBC_WaitForOutputBufferToBeFilled();
                USBKeyRepeat(NULL, 2);  // Enable Key repeat
                if (gEmulationTrap) { 
                    gEmulationTrap->TrapEnable(gEmulationTrap);
                }
                return EFI_SUCCESS;
            }
            bTemp   = 0xFF;
        } else {

            if ((gUsbData->UsbDevSupport & USB_MS_DEV_SUPPORT) == USB_MS_DEV_SUPPORT){
                if (USBMSSendMouseData()) {
                    USBKeyRepeat(NULL, 2);  // Enable Key repeat
                }


                if(gEmulationTrap) 
        gEmulationTrap->TrapEnable(gEmulationTrap);
                return EFI_SUCCESS;

            } else {
                bTemp   = 0xFF;
            }
        }
        break;
    }

    if(bFlag == 3) {
        bTemp   = 0xAE;
        if(bTemp1 & 0x20) {     // 0/1 = KB/Mouse data
            goto enableRepeat_Exit;
        }
    }
    KBC_WriteCommandByte(bTemp);

enableRepeat_Exit:
    USBKeyRepeat(NULL, 2);  // Enable Key repeat
    if (gEmulationTrap) { 
        gEmulationTrap->TrapEnable(gEmulationTrap);
    }    
    return EFI_NOT_READY;  
}


/**
    This routine sends the Keyboard Data to Controller.

    @param This    pointer to the Emulation Protocol
    @param Data    Data buffer
    @param Count   Data buffer Length
    
    @retval EFI_SUCCESS       Success
    @retval Others            Error

**/

EFI_STATUS
EFIAPI
SYSKBC_KbdInput_Send(
    IN EFI_EMUL6064KBDINPUT_PROTOCOL* This,
    IN UINT8* Data,
    IN UINT32 Count
 )
{
    EFI_STATUS  Status;

    if (Count == 0) {
        return EFI_SUCCESS;
    }

    do {
        Status=SYSKBC_SendKBCData();
        Count--;
    } while (Count != 0 && Status == EFI_SUCCESS);

    return Status;

}

/**
    This routine sends the Mouse Data to Controller.

    @param This  Pointer to the Emulation Protocol
    @param Data  Ps2 mouse Data

    @retval EFI_SUCCESS       Success
    @retval Others            Error

**/

EFI_STATUS
EFIAPI
SYSKBC_MouseInput_Send(
    IN EFI_EMUL6064MSINPUT_PROTOCOL*   This,
    IN PS2MouseData*                   Data
)
{
    return SYSKBC_SendKBCData();
}


/**
    This routine sends the Mouse Data to Local Mouse Buffer 

    @param Data   Ps2 mouse Data

    @retval EFI_SUCCESS       Success
    @retval Others            Error

**/

EFI_STATUS
SendMouseData(
    IN PS2MouseData* Data
)
{
    UINT8   *TempHeadPtr = gUsbDataList->MouseInputBufferHeadPtr;
    UINT8   PacketSize = (gUsbData->bMouseStatusFlag & MOUSE_4BYTE_DATA_BIT_MASK) ? 4 : 3;


    //Before adding data check if there is atleast three/four byte difference between head and tail
    if (gUsbDataList->MouseInputBufferTailPtr != gUsbDataList->MouseInputBufferHeadPtr) {
        
        // Check if 3/4 bytes are added to the Headptr, will it overwrite the TailPtr or not
        if ((TempHeadPtr + PacketSize) > (gUsbData->UsbMsInputBuffer + sizeof(gUsbData->UsbMsInputBuffer))) {
            TempHeadPtr = gUsbData->UsbMsInputBuffer;
        }
        
        if ((TempHeadPtr <= gUsbDataList->MouseInputBufferTailPtr) && ((TempHeadPtr + PacketSize) >= gUsbDataList->MouseInputBufferTailPtr)){
            return EFI_SUCCESS;
        }
        
    }

    USBMouse_SendToMouseBuffer(Data->flags);    // Send status byte
    USBMouse_SendToMouseBuffer(Data->x);        // Send X-axis information
    USBMouse_SendToMouseBuffer(Data->y);        // Send Y-axis information

    //
    // Check for 4-byte mouse data and send the 4th dummy data if needed
    //
    if (gUsbData->bMouseStatusFlag & MOUSE_4BYTE_DATA_BIT_MASK) {
        USBMouse_SendToMouseBuffer(0);
    }
    //
    // Set flag to indicate mouse data ready
    //
    //gUsbData->bMouseStatusFlag |= MOUSE_DATA_READY_BIT_MASK;

    return EFI_SUCCESS;
}

/**
    This routine Get the KBC SCAN code transalation value

    @param This         Pointer to the Emulation Protocol
    @param OutTrans     KBC_KBDTRANS_AT = 0
                        KBC_KBDTRANS_PCXT = 1
    
    @retval EFI_SUCCESS       Success
    @retval Others            Error

**/

EFI_STATUS
EFIAPI
SYSKBC_GetTranslation(
    IN EFI_EMUL6064KBDINPUT_PROTOCOL* This,
    OUT KBC_KBDTRANSLATION* OutTrans 
)
{
    static BOOLEAN IsCcbLoaded = FALSE;
    
    if (!IsCcbLoaded) {
        IsCcbLoaded = TRUE;
        SYSKBC_GetAndStoreCCB();
    }
    
    *OutTrans = (gUsbData->bCCB & CCB_TRANSLATE_SCAN_CODE_BIT_MASK) != 0?
                 KBC_KBDTRANS_PCXT : KBC_KBDTRANS_AT;

    return EFI_SUCCESS;
}

/**
    This routine converts the USB keyboard input data into
    IBM PC format scan code

    @param FpDevInfo   Pointer to the device info. structure
    @param FpBuffer    Pointer to the 8byte USB keyboard data

    @retval VOID

    @note  The format of the 8byte USB keyboard data is as follows:
  Byte                     Description
  Byte 0      Contains modifier key bits like SHIFT, CTRL, ALT etc
  Byte 1      Reserved
  Byte 2 - 7      Keycode (1-6) of the keys pressed
**/

VOID
USBKB_Scanner (
    DEV_INFO    *FpDevInfo,
    UINT8       *FpBuffer
)
{
    UINT8   KeyCode;
    UINT8   Value;
    UINT8   Temp1;

    UINT16  WorkOffset;
    UINT16  Count;
    UINT16  Temp;

    UINT8   *FptKbInputBuffer;
    UINT8   *FptBuffer;
    UINT8   *Pointer;

    FptKbInputBuffer = gUsbData->UsbKbInputBuffer ;
    FptBuffer = (UINT8*)FpBuffer;

    //
    // Convert the device address (1,2,3 .. 8) into
    // device ID bit pattern (001h, 002h, 004h .. 080h)
    //
    gUsbData->CurrentDeviceId = (UINT8)(1 << ((FpDevInfo->DeviceAddress) -1));

    Temp1 = 0xE0;
    Count = 0x0;
    Value = (UINT8)(*FptBuffer);

    do {
        if(Value & 0x01) {
            *FptKbInputBuffer   = Temp1;  // Modifier key pressed. Send appropriate byte to the input buffer
            ++FptKbInputBuffer;
        }
        Value  >>= 1;
        ++Temp1;
    } while (Temp1 <= 0xE7);

    //
    // Discard the reserved byte from the USB KB input data stream
    //
    ++FptBuffer;
    ++FptBuffer;

    //
    // Process the remaining 6 input bytes
    //
    Count = 6;

    do {
        if(*FptBuffer) {
            //
            // Check whether the input is overrun. Overrun condition occurs if more than
            // 6 keys are pressed at a same time. In that case, USB KB sends 01h in all
            // its 6 key code bytes
            //
            if(*FptBuffer == 1) {   // Keyboard error roll over (overrun) ?
//              USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_5, "USBKB_Scanner:  Overrun condition occur\n");
                //
                // Check the space availability for the overrun code in the
                // character buffer
                // Overrun code needs 1 byte (0+1)
                //
                Temp1      = USBKBC_CheckCharacterBufferFull(0);
                if(Temp1 == 0) {
                   return;
                }
                Temp1 = 0xFF;  // Scan code set 2 & 3 overrun code
#if USB_6064_TRAP
//
// The following routine returns overrun code depending upon the scan code
// set chosen. This facility is available only when 60/64h port trapping
// is enabled. Otherwise the scan code set is locked to set 2.
//
                Temp1 = USBTrap_GetOverrunCode();
#endif
                USBKBC_SendToCharacterBuffer(Temp1);
                //
                // Invalidate the last key code processed data
                //
//                  gUsbData->bLastUSBKeyCode      = 0x00;  
                return;
            }
            else {
                *FptKbInputBuffer   = *FptBuffer;
                ++FptKbInputBuffer;
                ++FptBuffer;
            }
        }
        else {
            ++FptBuffer;
        }
        --Count;
    } while (Count);

    *FptKbInputBuffer = 0;  // Null terminate the input buffer
    ++FptKbInputBuffer;

    //
    // Check for break code generation condition
    //
    Count = (UINT16)(FptKbInputBuffer - gUsbData->UsbKbInputBuffer);
    FptKbInputBuffer = gUsbData->UsbKbInputBuffer;
    Value = 0;
    //
    // wCount = Length of the buffer
    //
    while(Count) {
        if (*FptKbInputBuffer)
            break;
        ++FptKbInputBuffer;
        --Count;
    }

    if(!Count) {
        //
        // No key pressed. Set flags to generate break code
        //
        gUsbData->BreakCodeDeviceId |= gUsbData->CurrentDeviceId;
    }

    FptKbInputBuffer = gUsbData->UsbKbInputBuffer;
//
// Save the keycode in the local variable
//
UKS_KeyCodeGenerateLoop:

    KeyCode                = *FptKbInputBuffer;
    ++FptKbInputBuffer;

    if(KeyCode) {  // Check end of the list
        //
        // Get scan code buffer size
        //
        Count = (UINT16)(gUsbDataList->KbcScanCodeBufferPtr -
                            (UINT8*)gUsbData->KbcScanCodeBufferStart);
        Temp1 = 0;

        while(Count) {
            if(KeyCode == gUsbData->KbcScanCodeBufferStart[Temp1]) {
                gUsbData->KbcDeviceIdBufferStart[Temp1]  |=
                                            gUsbData->CurrentDeviceId;
                    goto    UKS_KeyCodeGenerateLoop;
            }
            ++Temp1;
            --Count;
        };
        if(Temp1 >= sizeof (gUsbData->KbcScanCodeBufferStart))
            goto    UKS_CheckBreakCodeGen;
        //
        // The key code is not in the buffer and also the buffer is not full. So
        // generate the make code for the newly pressed key.
        //
        USBKB_GenerateScanCode(USB_GEN_MAKE_CODE, KeyCode, 0);
//          gUsbData->bLastUSBKeyCode                  = KeyCode;    
        gUsbData->KbcScanCodeBufferStart[Temp1]  = KeyCode;
        //
        // Store the device ID
        //
        gUsbData->KbcDeviceIdBufferStart[Temp1] = gUsbData->CurrentDeviceId;
        //
        // Store the shift key status
        //
        gUsbData->KbcShiftKeyStatusBufferStart[Temp1] = gUsbData->UsbKbShiftKeyStatus;
        ++(gUsbDataList->KbcScanCodeBufferPtr);  // Update pointer index
        //
        // Check and load typematic rate setting
        //
        if (gUsbData->RepeatIntervalMode){
            Temp = (UINT16)(gUsbData->UsbKbcStatusFlag & KBC_TYPE_DELAY_BIT_MASK_16MS);
            Temp >>= KBC_TYPE_DELAY_BIT_SHIFT_16MS;
        } else {
            Temp = (UINT16)(gUsbData->UsbKbcStatusFlag & KBC_TYPE_DELAY_BIT_MASK_8MS);
            Temp >>= KBC_TYPE_DELAY_BIT_SHIFT_8MS;          
        }
        gUsbData->RepeatRate = aTypematicRateDelayTable[Temp];
        gUsbData->RepeatCounter = 0;

        goto    UKS_KeyCodeGenerateLoop;
    }
UKS_CheckBreakCodeGen:
    --FptKbInputBuffer;

    Pointer = (UINT8*)(gUsbData->KbcScanCodeBufferStart);
    Temp = (UINT16)(FptKbInputBuffer - gUsbData->UsbKbInputBuffer);

UKS_ProcessNextCharacter:
    if(Pointer >= gUsbDataList->KbcScanCodeBufferPtr)
        goto    UKS_BreakCodeGenCompleted;

    KeyCode =  *Pointer;
    ++Pointer;

    FptKbInputBuffer =  gUsbData->UsbKbInputBuffer;
    Count = Temp;
    while(Count) {
        if(KeyCode == *FptKbInputBuffer)
            goto    UKS_ProcessNextCharacter;

        ++FptKbInputBuffer;
        --Count;
    }

    --Pointer;
    WorkOffset = (UINT16)(Pointer - gUsbData->KbcScanCodeBufferStart);

    gUsbData->KbcDeviceIdBufferStart[WorkOffset] &= ~((UINT8)(gUsbData->CurrentDeviceId));
    if(gUsbData->KbcDeviceIdBufferStart[WorkOffset] ) {
        ++Pointer;
        goto    UKS_ProcessNextCharacter;
    }
    Value = gUsbData->KbcShiftKeyStatusBufferStart[WorkOffset];
    gUsbData->KbcShiftKeyStatusBufferStart[WorkOffset]   =
                                        gUsbData->UsbKbShiftKeyStatus;
    gUsbData->UsbKbShiftKeyStatus = Value;
    USBKB_GenerateScanCode(USB_GEN_BREAK_CODE,(UINT8)*Pointer, WorkOffset);
    //
    // Restore shift key status
    //
    Value = gUsbData->KbcShiftKeyStatusBufferStart[WorkOffset];
    gUsbData->KbcShiftKeyStatusBufferStart[WorkOffset] = gUsbData->UsbKbShiftKeyStatus;
    gUsbData->UsbKbShiftKeyStatus         = Value;
    Value = *Pointer;
    USBKB_DiscardCharacter(&gUsbData->KbcShiftKeyStatusBufferStart[WorkOffset]);

    --(gUsbDataList->KbcScanCodeBufferPtr);
    //
    // Stop auto repeat
    //
//      gUsbData->bLastUSBKeyCode = 0;  
    goto    UKS_ProcessNextCharacter;
UKS_BreakCodeGenCompleted:
    gUsbData->BreakCodeDeviceId &= ~((UINT8)(gUsbData->CurrentDeviceId));
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_7, "USBKB_Scanner:  End\n");
}


/**
    This routine generates the make/break code for the key
    pressed depending on the make/break flag

    @param bCodeGenFlag    Flag indicating whether it is a make or break code sequence
    @param bKeyCode        USB key code for the key pressed
    @param wWorkOffset     Offset into the buffers which has the code we are processing

    @retval None


**/

VOID
USBKB_GenerateScanCode (
    UINT8 bCodeGenFlag,
    UINT8   bKeyCode,
    UINT16  wWorkOffset)
{
    UINT8       bMakeCode,
                bTemp = 0,
                bValue;
    UINT16      wScanCode,
                wTemp;
    UINT8       *fPointer;
    UINT8       ScanCodeCount;
    UINT16      i=0;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_7, "USBKB_GenerateScanCode:  bKeyCode %x:\n",bKeyCode);

    //
    // Set the flag to indicate that make code is generated
    //
    gUsbData->UsbKbcStatusFlag &= ~((UINT16) KBC_MK_BRK_CODE_BIT_MASK);

    //
    // Set the approprate flag indicating make or break code sequence
    //
    if (bCodeGenFlag != USB_GEN_MAKE_CODE)
    {
        //
        // Set the flag to indicate that break code is generated
        //
        gUsbData->UsbKbcStatusFlag   |= KBC_MK_BRK_CODE_BIT_MASK;
    }

    //
    // Save the current USB key code
    //
    gUsbData->CurrentUsbKeyCode       = bKeyCode;

    //
    // Convert the USB key code into scan code (Set 2)
    //
    bMakeCode   = USBKB_ConvertUSBKeyCodeToScanCodeSet2(bKeyCode);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_8, "USBKB_GenerateScanCode:  bMakeCode %x:\n",bMakeCode);

    //
    // Save the make code in the temporary variable
    //
    gUsbData->Set2ScanCode = bMakeCode;
    //
    // Check whether any modifier keys (ALT, CTRL & SHIFT) are pressed
    //
    wScanCode = USBKB_CheckModifierKeyPress(bMakeCode);
    wTemp = (UINT16)(wScanCode >> 8);

    if(wTemp)
    {
        //
        // Check whether it is a make or break code generation
        //
        if(gUsbData->UsbKbcStatusFlag & KBC_MK_BRK_CODE_BIT_MASK)
        {
            //
            // Generating the break code, so clear the modifier status.
            // Check for left/right shift key press
            //
            if(!(wTemp & (KB_LSHIFT_KEY_BIT_MASK + KB_RSHIFT_KEY_BIT_MASK)))
            {
                fPointer    = gUsbDataList->KbcScanCodeBufferPtr ;

                while(fPointer != (gUsbData->KbcScanCodeBufferStart))
                {
                    --fPointer;     // Decrement scan code buffer pointer
                    bTemp   = USBKB_ConvertUSBKeyCodeToScanCodeSet2(*fPointer);
                    if(bTemp == (wScanCode & 0xFF))
                    {
                        break;
                    }
                }
            }

            if(bTemp != (wScanCode & 0xFF) ||
                (wTemp & (KB_LSHIFT_KEY_BIT_MASK + KB_RSHIFT_KEY_BIT_MASK)))
            {
                wTemp = (UINT16)(~wTemp);
                gUsbData->UsbKbShiftKeyStatus &= wTemp;
                gUsbData->KbcShiftKeyStatusBufferStart[wWorkOffset] &= wTemp;
            }
        }
        else
        {
            gUsbData->UsbKbShiftKeyStatus     |= wTemp;   // Set proper bits
        }
    }

#if USB_KBC_EMULATION
    bScanNum    = USBTrap_GetCurrentScanCodeSetNumber();
    if(bScanNum == 3)
    {
        USBTrap_ProcessScanCodeSet3();
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_7, "USBKB_GenerateScanCode:  Return 1.\n");
        return;
    }
#endif
    //
    // Check and process pause key
    //
    if(gUsbData->Set2ScanCode == PAUSE_KEY )
    {
//
// It is scan code for pause key. No need to auto repeat pause key and
// also pause key doesn't have break key. Check the above two conditions
// and take care of them properly
//
        //
        // Check whether it is to generate make or break code
        //
        if(gUsbData->UsbKbcStatusFlag & KBC_MK_BRK_CODE_BIT_MASK)
        {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_5, "USBKB_GenerateScanCode:  Return 2.\n");
            return;
        }
//
// It is not a break code check whether it is for auto repeat
// Compare old key code and the current key code
//
                                        //(EIP102150+)>
        ScanCodeCount = (UINT8)(gUsbDataList->KbcScanCodeBufferPtr - 
                         (UINT8*)gUsbData->KbcScanCodeBufferStart); 
        for (i = 0; i < ScanCodeCount; i++) {
            if ((gUsbData->KbcDeviceIdBufferStart[i] & gUsbData->CurrentDeviceId) &&
                (gUsbData->CurrentUsbKeyCode == gUsbData->KbcScanCodeBufferStart[i])) {
                return;
            }
        }
                                        //<(EIP102150+)
//
// Pause key can have
//    4 bytes of make code for scan code set 1, if CTRL key is also pressed
//    5 bytes of make code for scan code set 2, if CTRL key is also pressed
//    6 bytes of make code for scan code set 1, if CTRL key is not pressed
//    8 bytes of make code for scan code set 2, if CTRL key is not pressed
//    0 bytes if it is a break code for pause key.
// The above conditions are handled generically using two tables
//   pScanCodeLengthTable_4500 & pScanCodeLengthTable_6800.  If CTRL key is
// pressed then _4500 table will be used or else _6800 table will be used.
// The logic in the later stage of the code can then choose one entry from
// this table depending on whether it is make or break code and whether
// it is for scan code set 1 or 2.
//
        fPointer = aScanCodeLengthTable_4500;   // CTRL + PAUSE
        if((gUsbData->UsbKbShiftKeyStatus & KB_CTRL_KEY_BIT_MASK) == 0)
            fPointer = aScanCodeLengthTable_6800;       // PAUSE
    }
    else
    if(gUsbData->Set2ScanCode == PRINT_SCREEN )
    {
/*---
 Print screen key can have
    1 bytes of make code for scan code set 1, if pressed with ALT key
    1 bytes of make code for scan code set 2, if pressed with ALT key
    1 bytes of break code for scan code set 1, if pressed with ALT key
    2 bytes of break code for scan code set 2, if pressed with ALT key
    2 bytes of make code for scan code set 1, if pressed with CTRL+SFT key
    2 bytes of make code for scan code set 2, if pressed with CTRL+SFT key
    2 bytes of break code for scan code set 1, if pressed with CTRL+SFT key
    4 bytes of make code for scan code set 1, if no other key is pressed
    4 bytes of make code for scan code set 2, if no other key is pressed
    4 bytes of break code for scan code set 1, if no other key is pressed
    6 bytes of break code for scan code set 2, if no other key is pressed
 The above conditions are handled generically using three tables
 pScanCodeLengthTable_1112, pScanCodeLengthTable_2223
 & pScanCodeLengthTable_4446.  If ALT key is pressed then _1112 table
 will be used. If CTRL key is pressed then _2223 table will be used or
 else _4446 table will be used.
 The logic in the later stage of the code can then choose one entry from
 this table depending on whether it is make or break code and whether
 it is for scan code set 1 or 2.
---*/
        if(gUsbData->UsbKbShiftKeyStatus & KB_ALT_KEY_BIT_MASK)
            fPointer    = aScanCodeLengthTable_1112;
        else if(gUsbData->UsbKbShiftKeyStatus &
                                ( KB_CTRL_KEY_BIT_MASK
                                + KB_LSHIFT_KEY_BIT_MASK
                                + KB_RSHIFT_KEY_BIT_MASK))
            fPointer    = aScanCodeLengthTable_2223;
        else
            fPointer    = aScanCodeLengthTable_4446;

 /*     fPointer    = aScanCodeLengthTable_1112;
        if((gUsbData->UsbKbShiftKeyStatus & KB_ALT_KEY_BIT_MASK) == 0)
        {
            fPointer    = aScanCodeLengthTable_2223;
            if((gUsbData->UsbKbShiftKeyStatus & (KB_NUM_LOCK_BIT_MASK + KB_LSHIFT_KEY_BIT_MASK +  KB_RSHIFT_KEY_BIT_MASK)) == 0)
            {
                fPointer    = aScanCodeLengthTable_4446;
                bTemp   = (UINT8)USBKB_CheckForNumericKeyPadKey(gUsbData->Set2ScanCode);
                if(bTemp)
                {
//               goto   UKGSC_NotANumericKeyPadKey;
                }
                fPointer    = aScanCodeLengthTable_2223;
                if(gUsbData->UsbKbShiftKeyStatus & (KB_NUM_LOCK_BIT_MASK
                                    + KB_LSHIFT_KEY_BIT_MASK
                                    + KB_RSHIFT_KEY_BIT_MASK))
                {
                    fPointer    = aScanCodeLengthTable_4446;
                    if((gUsbData->UsbKbShiftKeyStatus & KB_NUM_LOCK_BIT_MASK) == 0)
                    {
                        if((gUsbData->UsbKbShiftKeyStatus & KB_NUM_LOCK_BIT_MASK) == 0)
                        {
                            fPointer    = aScanCodeLengthTable_4545;
                        }
                    }
                }
            }
        }  */
    }
    else
    {
        wScanCode   = USBKB_CheckForNumericKeyPadKey(gUsbData->Set2ScanCode);
        wTemp       = (UINT16)(wScanCode & 0xFF);
        if(!wTemp)
        {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_7, "USBKB_GenerateScanCode:  Extended Key Pressed\n");

/*---
 It is a numeric key pad key. It can have
  if no key is pressed or (NUMLOCK is ENABLED and SHIFT key is pressed)
    2 bytes of make code for scan code set 1
    2 bytes of make code for scan code set 2
    2 bytes of break code for scan code set 1
    3 bytes of break code for scan code set 2
  if NUMLOCK is ENABLED
    4 bytes of make code for scan code set 1
    4 bytes of make code for scan code set 2
    4 bytes of break code for scan code set 1
    6 bytes of break code for scan code set 2
  if pressed with SHIFT key  
    4 bytes of make code for scan code set 1
    5 bytes of make code for scan code set 2
    4 bytes of break code for scan code set 1
    5 bytes of break code for scan code set 2
 The above conditions are handled generically using three tables
 pScanCodeLengthTable_2223, pScanCodeLengthTable_4446
 & pScanCodeLengthTable_4545.  If no other key is pressed then _2223 table
 will be used. If NUMLOCK is enabled then _4446 table will be used or
 else _4545 table will be used.
 The logic in the later stage of the code can then choose one entry from
 this table depending on whether it is make or break code and whether
 it is for scan code set 1 or 2.
---*/

                if( (!(gUsbData->UsbKbShiftKeyStatus &
                               ( KB_NUM_LOCK_BIT_MASK
                              + KB_LSHIFT_KEY_BIT_MASK
                              + KB_RSHIFT_KEY_BIT_MASK))) || 
                           ((gUsbData->UsbKbShiftKeyStatus &
                              ( KB_NUM_LOCK_BIT_MASK
                              + KB_LSHIFT_KEY_BIT_MASK)) ==
                              ( KB_NUM_LOCK_BIT_MASK
                              + KB_LSHIFT_KEY_BIT_MASK)) ||
                           ((gUsbData->UsbKbShiftKeyStatus &
                              ( KB_NUM_LOCK_BIT_MASK
                              + KB_RSHIFT_KEY_BIT_MASK)) ==
                              ( KB_NUM_LOCK_BIT_MASK
                              + KB_RSHIFT_KEY_BIT_MASK))     )
                    fPointer    = aScanCodeLengthTable_2223;

                else if(gUsbData->UsbKbShiftKeyStatus &
                                                KB_NUM_LOCK_BIT_MASK)
                {

                  fPointer    = aScanCodeLengthTable_4446;
                  if (!(gUsbData->UsbKbcStatusFlag & KBC_MK_BRK_CODE_BIT_MASK)) {
                        ScanCodeCount = (UINT8)(gUsbDataList->KbcScanCodeBufferPtr - 
                                        (UINT8*)gUsbData->KbcScanCodeBufferStart); 
                        for (i = 0; i < ScanCodeCount; i++) {
                            if ((gUsbData->KbcDeviceIdBufferStart[i] & gUsbData->CurrentDeviceId) &&
                               (gUsbData->CurrentUsbKeyCode == gUsbData->KbcScanCodeBufferStart[i])) {
                               fPointer    = aScanCodeLengthTable_2223;
                               break;
                            }
                        }
                  }

                }
                else
                 fPointer    = aScanCodeLengthTable_4545;
        }
        else
        {
        // UKGSC_NotANumericKeyPadKey
/*---
 The extended keys will have
    2 bytes of make code for scan code set 1
    2 bytes of make code for scan code set 2
    2 bytes of break code for scan code set 1
    3 bytes of break code for scan code set 2
---*/
            wScanCode   = USBKB_CheckForExtendedKey(gUsbData->Set2ScanCode);
            if(!(wScanCode & 0xff))
                fPointer    = aScanCodeLengthTable_2223;
            else if(gUsbData->Set2ScanCode == SLASH_KEY)
            {
/*---
 The slash key will normally have
    2 bytes of make code for scan code set 1
    2 bytes of make code for scan code set 2
    2 bytes of break code for scan code set 1
    3 bytes of break code for scan code set 2
 But if pressed with slash key it has
    4 bytes of make code for scan code set 1
    5 bytes of make code for scan code set 2
    4 bytes of break code for scan code set 1
    5 bytes of break code for scan code set 2
---*/
                if(!(gUsbData->UsbKbShiftKeyStatus &
                            (KB_LSHIFT_KEY_BIT_MASK +
                            KB_RSHIFT_KEY_BIT_MASK)))
                    fPointer    = aScanCodeLengthTable_2223;
                else
                    fPointer    = aScanCodeLengthTable_4545;
            }
            else        // UKGSC_NotSlashKey:
            /*---
                 Regular keys will have
                    1 bytes of make code for scan code set 1
                    1 bytes of make code for scan code set 2
                    1 bytes of break code for scan code set 1
                    2 bytes of break code for scan code set 2
            ---*/
                fPointer    = aScanCodeLengthTable_1112;
        }
    }
    //
    // Find the appropriate scan code length from the table
    // bOffset = Offset in the table
    //
    bValue = 0;     // Assume make code for set 1

    //
    // Check whether to generate make or break code
    //
    if(gUsbData->UsbKbcStatusFlag & KBC_MK_BRK_CODE_BIT_MASK)
    {
        bValue = 2;
    }
    //
    // Check whether to generate set 1 scan code or set 2
    //
    if((gUsbData->bCCB & CCB_TRANSLATE_SCAN_CODE_BIT_MASK) == 0 )
    {
        ++bValue;   // Update offset (for scan code set 2)
    }

    bValue      = fPointer[bValue];

    bValue      = USBKBC_CheckCharacterBufferFull(bValue);

    if (!bValue) return;

    //
    // We had found out correct scan code length to generate and also we
    // verified we have enough space. Now generate the scan code
    //
    bValue = gUsbData->Set2ScanCode;

    if(bValue == PAUSE_KEY )
    {
        //
        // Check whether CTRL key is pressed
        //
        if(!(gUsbData->UsbKbShiftKeyStatus & KB_CTRL_KEY_BIT_MASK))
        {
            //
            // Pause key scan code generation
            //
            USBKBC_SendToCharacterBuffer(0x0E1);
            gUsbData->Set2ScanCode    = 0x14;
            USBKB_GenerateType1MakeCode();
            gUsbData->Set2ScanCode    = 0x77;
            USBKB_GenerateType1MakeCode();
            USBKBC_SendToCharacterBuffer(0xE1);
            gUsbData->Set2ScanCode    = 0x14;
            USBKB_GenerateType1BreakCode();
            gUsbData->Set2ScanCode    = 0x77;
            USBKB_GenerateType1BreakCode();
            return;
        }
        else
        {
            gUsbData->Set2ScanCode    = 0x7E;
            USBKB_GenerateType2MakeCode();
            USBKB_GenerateType2BreakCode();
        }

    }
    else if(bValue == PRINT_SCREEN )
    {
        gUsbData->Set2ScanCode    = 0x7C;     // PRNSCRN for scan code set 2
        //
        // Check for ALT+PRNSCRN key combination
        //
        if(!(gUsbData->UsbKbShiftKeyStatus & KB_ALT_KEY_BIT_MASK ))
        {
            //
            // Check for CTRL or SHIFT+PRNSCRN key combination
            //
            if(!(gUsbData->UsbKbShiftKeyStatus &
                                    (KB_CTRL_KEY_BIT_MASK
                                    + KB_LSHIFT_KEY_BIT_MASK
                                    + KB_RSHIFT_KEY_BIT_MASK)))
            {
 UKGSC_KeyCombination2:
//
// Process PRNSCRN only key processing, NUMLOCK + any numeric key press
// processing at this point. Generate either (E0,12,E0,xx) or
// (E0,F0,xx,E0,F0,12). Where xx Set2ScanCode
//
                //
                // Check whether to generate make or break code
                //
                if(!(gUsbData->UsbKbcStatusFlag & KBC_MK_BRK_CODE_BIT_MASK))
                {
                    bTemp   = gUsbData->Set2ScanCode;
                    gUsbData->Set2ScanCode    = 0x12;
                    USBKB_GenerateType2MakeCode();
                    gUsbData->Set2ScanCode    = bTemp;
                    USBKB_GenerateType2MakeCode();
                    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_5, "USBKB_GenerateScanCode:  Return 5.\n");
                    return;

                }
                else
                {
                    USBKB_GenerateType2BreakCode();
                    gUsbData->Set2ScanCode    = 0x12;
                    USBKB_GenerateType2BreakCode();
                    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_5, "USBKB_GenerateScanCode:  Return 6.\n");
                    return;
                }
            }
            else
                goto UKGSC_KeyCombination1;

        }
        else
        {
            gUsbData->Set2ScanCode    = 0x84;
UKGSC_ProcessRegularKey:
//
// Regular key press make & break code generation (xx or 0F0h, xx)
// Check whether to generate make or break code
//
            if((gUsbData->UsbKbcStatusFlag & KBC_MK_BRK_CODE_BIT_MASK ) == 0)
            {
                USBKB_GenerateType1MakeCode();
                USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_7, "USBKB_GenerateScanCode:  Return 7.\n");
                return;
            }
            USBKB_GenerateType1BreakCode();
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_7, "USBKB_GenerateScanCode:  Return 8.\n");
            return;
        }

    }
    else
    {
        //
        // Check for numeric key pad keys
        //
        wScanCode = USBKB_CheckForNumericKeyPadKey(gUsbData->Set2ScanCode);
        wTemp = (UINT16)(wScanCode & 0xFF);
        if(wTemp == 0)
        {
            gUsbData->Set2ScanCode    &= 0x7F;

            if( (!(gUsbData->UsbKbShiftKeyStatus &
                                    (KB_NUM_LOCK_BIT_MASK
                                    + KB_LSHIFT_KEY_BIT_MASK
                                    + KB_RSHIFT_KEY_BIT_MASK))) ||
                           ((gUsbData->UsbKbShiftKeyStatus &
                              ( KB_NUM_LOCK_BIT_MASK
                              + KB_LSHIFT_KEY_BIT_MASK)) ==
                              ( KB_NUM_LOCK_BIT_MASK
                              + KB_LSHIFT_KEY_BIT_MASK)) ||
                           ((gUsbData->UsbKbShiftKeyStatus &
                              ( KB_NUM_LOCK_BIT_MASK
                              + KB_RSHIFT_KEY_BIT_MASK)) ==
                              ( KB_NUM_LOCK_BIT_MASK
                              + KB_RSHIFT_KEY_BIT_MASK))     )

                 goto   UKGSC_KeyCombination1;

            if(gUsbData->UsbKbShiftKeyStatus & KB_NUM_LOCK_BIT_MASK)

            {
        if (!(gUsbData->UsbKbcStatusFlag & KBC_MK_BRK_CODE_BIT_MASK)) {
                  ScanCodeCount = (UINT8)(gUsbDataList->KbcScanCodeBufferPtr - 
                                   (UINT8*)gUsbData->KbcScanCodeBufferStart); 
                  for (i = 0; i < ScanCodeCount; i++) {
            if ((gUsbData->KbcDeviceIdBufferStart[i] & gUsbData->CurrentDeviceId) &&
                         (gUsbData->CurrentUsbKeyCode == gUsbData->KbcScanCodeBufferStart[i])) { 
                goto UKGSC_KeyCombination1;
                      } 
                  }
        }
                goto UKGSC_KeyCombination2; 
            }

UKGSC_KeyCombination3:
//
// The following code will generate scan code for SHIFT+Numeric key pad
// key combination. It will generate one of the following sequence
// (0E0h, 0F0h, 012H or 059h, 0E0h, xx)  or
// (0E0h, 0F0h, xx, 0E0h, 012H or 059h)
//
            if((gUsbData->UsbKbcStatusFlag & KBC_MK_BRK_CODE_BIT_MASK) == 0)
                {
                    //
                    // Check whether to generate make or break code
                    //
                    bTemp = gUsbData->Set2ScanCode;
                     if(gUsbData->UsbKbShiftKeyStatus & KB_LSHIFT_KEY_BIT_MASK)
                     {
                        gUsbData->Set2ScanCode = 0x12; // Save current scan code (xx)
                        USBKB_GenerateType2BreakCode();     // 0E0h, 0F0h, 012h
                     }
                     if(gUsbData->UsbKbShiftKeyStatus & KB_RSHIFT_KEY_BIT_MASK)
                     {
                        gUsbData->Set2ScanCode = 0x59;
                        USBKB_GenerateType2BreakCode();     //  0E0h, 0F0h, 059h
                     }
                     gUsbData->Set2ScanCode   = bTemp;    // Restore current code (xx)
                     USBKB_GenerateType2MakeCode();         // 0E0h, xx
                     USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_5, "USBKB_GenerateScanCode:  Return 9.\n");
                     return;
                }
                else
                {
                    USBKB_GenerateType2BreakCode();
                    if(gUsbData->UsbKbShiftKeyStatus & KB_RSHIFT_KEY_BIT_MASK)
                    {
                        gUsbData->Set2ScanCode = 0x59; // Save current scan code (xx)
                        USBKB_GenerateType2MakeCode();      // 0E0h, 59h
                    }
                    if(gUsbData->UsbKbShiftKeyStatus & KB_LSHIFT_KEY_BIT_MASK)
                    {
                        gUsbData->Set2ScanCode = 0x12; // Save current scan code (xx)
                        USBKB_GenerateType2MakeCode();      // 0E0h, 12h
                    }
                    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_5, "USBKB_GenerateScanCode:  Return 10.\n");
                    return;
            }
        }
        else
        {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_7, "USBKB_GenerateScanCode:  Notnumeric key\n");
            //
            // Check and process extended key press
            //
            bValue = (UINT8)USBKB_CheckForExtendedKey(gUsbData->Set2ScanCode);
            if(bValue == 0)
            {
                gUsbData->Set2ScanCode &= 0x7F;
UKGSC_KeyCombination1:
//
// Following code processes the following key combinations:
//   Extended key make & break code generation
//   CTRL or SHIFT + PRNSCRN key combination
//   Numeric key pad key code generation
// One of the following sequence will be generated (0E0h, xx) or
// (0E0h, 0F0h, xx)
//
                if(gUsbData->UsbKbcStatusFlag & KBC_MK_BRK_CODE_BIT_MASK)
                {
                    USBKB_GenerateType2BreakCode();
                    return;
                }
                USBKB_GenerateType2MakeCode();
                return;
            }
            else
            {
                //
                // Check and process '/' key
                //
                if (gUsbData->Set2ScanCode != SLASH_KEY)
                {
                    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_7, "USBKB_GenerateScanCode:  Regular Key.\n");
                    goto UKGSC_ProcessRegularKey;
                }
                //
                // Generate make or break code for '/' key
                //
                gUsbData->Set2ScanCode    = 0x4A;
                if(!(gUsbData->UsbKbcStatusFlag &
                                        ( KB_LSHIFT_KEY_BIT_MASK
                                        + KB_RSHIFT_KEY_BIT_MASK)))
                    goto UKGSC_KeyCombination1;
                goto UKGSC_KeyCombination3;
            }
        }
    }
}


/**
    This routine is used to update the LED state between the
    USB & PS/2 keyboard

    @param Flag   Indicates how the LED local variables are adjusted

    @retval None

    @note  This routine is not executed during EFI phase
**/

VOID
USBKB_UpdateLEDState (UINT8 Flag)
{
    UINT8       LStatus,
                HStatus;

    UINT8       *Ptr;


    EFI_STATUS          Status;
    VOID                *AddrCheck = ((VOID*)0x417); 
    Status = AmiUsbValidateMemoryBuffer (AddrCheck, sizeof(UINT8));
    if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)) return;


    Ptr = (UINT8*)(UINTN)0x417;
    //
    // Make sure the password LED status is not changed. If KBC password is
    // enabled then the LED state for NUMLOCK should be off, SCROLL-LOCK LED
    // state should be on and CAPSLOCK LED state is in dont-care state.
    //
    LStatus =  gUsbData->NonUsbKbShiftKeyStatus;
    LStatus &= 0x07 ;  // Masking BIT0, BIT1 and BIT2
    if (LStatus & 1)
    {
        //
        // None of the LED is ON. Make Scroll LOCK LED to be ON
        //
        LStatus = 0x04;
    }
    else
    {
        LStatus >>= 1;
    }

    gUsbData->NonUsbKbShiftKeyStatus  &= 0xF8;
    gUsbData->NonUsbKbShiftKeyStatus  |= LStatus;

    //
    // Check whether password is disabled
    //
    if (gUsbData->UsbKbcStatusFlag & KBC_PASSWORD_FLAG_BIT_MASK)
    {
        HStatus = (UINT8)(gUsbData->UsbKbShiftKeyStatus & 0x70);
        LStatus = (UINT8)((gUsbData->NonUsbKbShiftKeyStatus & 0x07) << 0x04);

        if(HStatus == LStatus)
        {
            return;
        }
        gUsbData->UsbKbShiftKeyStatus &=
                        ~ (KB_CAPS_LOCK_BIT_MASK +
                        KB_NUM_LOCK_BIT_MASK +
                        KB_SCROLL_LOCK_BIT_MASK);

        gUsbData->UsbKbShiftKeyStatus |= LStatus;
        //
        // Check whether SCROLL-LOCK is ON
        //
        LStatus = (UINT8)(LStatus >> 3);
        if(LStatus & 0x08)
        {
            LStatus ^= 0x09;
        }
        //
        // ScrollOff
        //
        USBKB_LEDOn();
        return;
    }

    if (gUsbData->UsbStateFlag & USB_FLAG_RUNNING_UNDER_OS) {
        HStatus = (UINT8)(gUsbData->UsbKbShiftKeyStatus & 0x70);
        LStatus = (UINT8)(gUsbData->KbShiftKeyStatusUnderOs & 0x70);
        if (HStatus == LStatus) {
            return;
        }
         gUsbData->UsbKbShiftKeyStatus &=
                    ~ (KB_CAPS_LOCK_BIT_MASK +
                    KB_NUM_LOCK_BIT_MASK +
                    KB_SCROLL_LOCK_BIT_MASK);

        gUsbData->UsbKbShiftKeyStatus |= LStatus;
        USBKB_LEDOn();
        return;
    }

    //
    // Get the current PS/2 keyboard LED state
    //
    //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_5, "USBKB_UpdateLEDState :  [0:417] = %x\n", *fPtr);
    LStatus = (UINT8)(((*Ptr) & 0x70) >> 1);
    HStatus = (UINT8)((gUsbData->UsbKbShiftKeyStatus) & 0x70);
    if(LStatus & 0x08)
        LStatus ^= 0x48;

    if(LStatus == HStatus)
        return;

    gUsbData->UsbKbShiftKeyStatus &=
                ~ (KB_CAPS_LOCK_BIT_MASK +
                KB_NUM_LOCK_BIT_MASK +
                KB_SCROLL_LOCK_BIT_MASK);

    gUsbData->UsbKbShiftKeyStatus |= LStatus;
    USBKB_LEDOn();

    return;
}

/**
    This routine updates the keyboard LED status for all the
    USB keyboards present in the system

    @param VOID

    @retval VOID

**/

VOID
USBKB_LEDOn(
)
{
    UINT8           LByte;
    UINT8           Count;
    DEV_INFO*       DevInfo;

    //
    // Set the LED update progress status
    //
    gUsbData->UsbKbcExtStatusFlag    |= KBCEXT_LED_UPDATE_IN_PROGRESS_BIT;

    //
    // Get the current USB LED status
    //
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_7, "USBKB_LEDOn: LedStatus %x\n",gUsbData->UsbKbShiftKeyStatus);
    //
    // Form the byte to be transmitted to the USB keyboard
    //
    LByte = (UINT8)(((gUsbData->UsbKbShiftKeyStatus) >> 4) & 0x07);
    USB_DEBUG(DEBUG_INFO,DEBUG_LEVEL_4, "USBKB_LEDOn: LedStatus %x, bLByte %x\n",
              gUsbData->UsbKbShiftKeyStatus, LByte);

    //
    // Update LED status in every USB keyboard on the system
    //
    for (Count = 0; Count < gUsbData->MaxHidCount; Count++) {
        DevInfo = gUsbDataList->UsbKbDeviceTable[Count];
        if (DevInfo) {
            UsbKbdSetLed(DevInfo, LByte);
        }
    }

    //
    // Reset the LED update progress status
    //
    gUsbData->UsbKbcExtStatusFlag &= ~(KBCEXT_LED_UPDATE_IN_PROGRESS_BIT);
}

/**
    Converts the set 2 scan code to set 1 scan code

    @param  ScanCode   Set 2 scan code

    @retval ScanCode  Set 1 scan code

**/

UINT8
USBKB_ConvertSet2CodeToSet1Code(UINT8   ScanCode)
{
    return USBKB_ConvertScanCodeBetweenCodeSet(
                    ScanCode,
                    aStaticSet2ToSet1ScanCode);
}


/**
    Converts the set 2 scan code to set 3/1 scan code

    @param ScanCode   Set 2 scan code
    @param ConvTable  Pointer to the code set conversion table

    @retval ScanCode  Set 1/3 scan code

**/

UINT8
USBKB_ConvertScanCodeBetweenCodeSet(
    UINT8   ScanCode,
    UINT8   *ConvTable
)
{
    UINT8       Value;
    //
    // Load the parameter from the stack to register
    //
    ScanCode =  ConvTable[ScanCode];
    ScanCode = (UINT8)((ScanCode << 4) | (ScanCode >> 4));
    Value = (UINT8)((ScanCode & 0x80) >> 7);
    ScanCode = (UINT8)(~((ScanCode << 1) | Value));
    return ScanCode;
}


/**
    This routine generates the type 1 make code for the byte
    in the variable Set2ScanCode

    @param None

    @retval None

**/

VOID
USBKB_GenerateType1MakeCode()
{
    UINT8       Value =0;

//  USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_5, "USBKB_GenerateType1MakeCode");
#if     USB_KBC_EMULATION
    Value      = USBTrap_GetCurrentScanCodeSetNumber();
#endif
// Check whether conversion (from set 2 to set 1) is opted
    if(Value != 1)
    {
        if(((UINT32)(gUsbData->bCCB) & (UINT32)CCB_TRANSLATE_SCAN_CODE_BIT_MASK ) == 0)
        {
            USBKBC_SendToCharacterBuffer(gUsbData->Set2ScanCode);
            return;
        }
    }
        Value      = USBKB_ConvertSet2CodeToSet1Code(gUsbData->Set2ScanCode);

        USBKBC_SendToCharacterBuffer(Value);
}


/**
    This routine generates the type 1 break code for the byte
    in the variable Set2ScanCode

    @param None

    @retval None

**/

VOID
USBKB_GenerateType1BreakCode()
{
    UINT8       Value =0;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_8, "USBKB_GenerateType1BreakCode \n");

#if     USB_KBC_EMULATION
    Value      = USBTrap_GetCurrentScanCodeSetNumber();
#endif
    //
    // Check whether conversion (from set 2 to set 1) is opted
    //
    if(Value != 1)
    {
        if(((UINT32)(gUsbData->bCCB) & (UINT32)CCB_TRANSLATE_SCAN_CODE_BIT_MASK ) == 0)
        {
            USBKBC_SendToCharacterBuffer(0xF0);
            USBKBC_SendToCharacterBuffer(gUsbData->Set2ScanCode);
            return;
        }
    }
    Value = USBKB_ConvertSet2CodeToSet1Code(gUsbData->Set2ScanCode);
    Value |= 0x80;
    USBKBC_SendToCharacterBuffer(Value);
}


/**
    This routine generates the type 2 make code for the byte
    in the variable Set2ScanCode

    @param None

    @retval None

**/

VOID
USBKB_GenerateType2MakeCode()
{
    USBKBC_SendToCharacterBuffer(0xE0);
    USBKB_GenerateType1MakeCode();
}


/**
    This routine generates the type 2 break code for the byte
    in the variable Set2ScanCode

    @param None

    @retval None

**/

VOID
USBKB_GenerateType2BreakCode()
{
    USBKBC_SendToCharacterBuffer(0xE0);
    USBKB_GenerateType1BreakCode();
}


/**
    Returns the current scan code set number

    @param  None

    @retval Status     Scan code set number (1, 2 or 3)

**/

UINT8
USBTrap_GetCurrentScanCodeSetNumber()
{
    UINT16 Status;
    Status = (UINT16)(gUsbData->UsbKbcStatusFlag & KBC_SCAN_CODE_SET_BIT_MASK);
    Status >>=  KBC_SCAN_CODE_SET_BIT_SHIFT;

    return (UINT8)Status;

}


/**
    Returns the overrun code depending on the current
    scan code set

    @param None

    @retval 0xFF Overrun code

**/

UINT8
USBTrap_GetOverrunCode()
{
    return  0xFF;
}


/**
    This routine discards the top character in the scan
    code buffer, keyboard status flag buffer and device ID
    buffer

    @param FlagBuffer    Points to the keyboard status flag buffer

    @retval None

**/

VOID
USBKB_DiscardCharacter (UINT8* FlagBuffer)
{
    UINT8           Count;
    UINT32          Count2 = 0;
    UINT8*          PtrEnd;

    PtrEnd     = gUsbData->KbcShiftKeyStatusBufferStart +
                    sizeof (gUsbData->KbcShiftKeyStatusBufferStart);
    Count      = (UINT8)(PtrEnd - (FlagBuffer + 1));

    do
    {
        FlagBuffer[Count2]    = FlagBuffer[Count2+1];
        ++Count2;
        --Count;
    } while(Count);

    //
    // Calculate offset
    //
    Count2 = (UINT32)(FlagBuffer -
                (UINT8*)gUsbData->KbcShiftKeyStatusBufferStart);
    Count = (UINT8)(PtrEnd - (FlagBuffer + 1));
    do
    {
        gUsbData->KbcDeviceIdBufferStart[Count2]  =
        gUsbData->KbcDeviceIdBufferStart[Count2+1];
        ++Count2;
        --Count;
    } while(Count);

    Count2 = (UINT32)(FlagBuffer -
             (UINT8*)gUsbData->KbcShiftKeyStatusBufferStart);
    //
    // Update device ID buf
    //
    Count = (UINT8)(PtrEnd - (FlagBuffer + 1));
    do
    {
        gUsbData->KbcScanCodeBufferStart[Count2]  =
        gUsbData->KbcScanCodeBufferStart[Count2+1];
        ++Count2;
        --Count;
    } while(Count);
}


/**
    Checks whether the key pressed is a extended key

    @param ScanCode   Set 2 scan code for the key pressed

    @retval 0      Key pressed matches the numeric key pad key
           <> 0    It is not a numeric key pad key
                   Set 3 scan code for the key pressed
                   (if set 3 is chosen)

**/

UINT16
USBKB_CheckForExtendedKey (UINT8 ScanCode)
{
    UINT8       Count,
                Found = FALSE;
    UINT16      RetValue = 0xff;

    Count      = 0;
    while(Count < sizeof (aStaticExtendedKeyScan2Table))
    {
        if(aStaticExtendedKeyScan2Table[Count] == ScanCode)
        {
            Found  = TRUE;
            break;
        }
        ++Count;
    }
    if(Found == TRUE)
    {
#if USB_KBC_EMULATION
        RetValue = (UINT16)aStaticExtendedKeyScan3Table[bCount];
        RetValue = wRetValue << 8;
#endif
        RetValue &= 0xff00;
    }

    return(RetValue);
}


/**
    Checks whether the key pressed is from numeric key pad

    @param ScanCode   Set 2 scan code for the key pressed

    @retval 0   Key pressed matches the numeric key pad key
           <> 0 It is not a numeric key pad key
                Set 3 scan code for the key pressed
                (if set 3 is chosen)

**/

UINT16
USBKB_CheckForNumericKeyPadKey (UINT8 ScanCode)
{
    UINT8       Count;
    BOOLEAN     Found = FALSE;
    UINT16      RetValue = 0xff;

    Count = 0;
    while(Count < sizeof (aStaticKeyPadScan2Table))
    {
        if(aStaticKeyPadScan2Table[Count] == ScanCode)
        {
            Found = TRUE;
            break;
        }
        ++Count;
    }
    if(Found == TRUE)
    {
#if USB_KBC_EMULATION
        RetValue = aStaticKeyPadScan3Table[Count];
        RetValue = RetValue << 8;
#endif
        RetValue &= 0xff00;
    }
    return(RetValue);
}


/**
    This routine checks whether the character buffer can hold
    'N'+1 character

    @param Count  Space needed in the buffer (in characters)

    @retval 0     If buffer is full
            <> 0  If buffer is not full

**/

UINT8
USBKBC_CheckCharacterBufferFull (UINT8 Count)
{
    UINT8   *Head, *Tail, *Start, *End;

    Head = gUsbDataList->KbcCharacterBufferHead;
    Tail = gUsbDataList->KbcCharacterBufferTail;
    Start = gUsbData->KbcCharacterBufferStart;
    End = Head + sizeof (gUsbData->KbcCharacterBufferStart);
    ++Count;
    do {
        ++Head;
        if(Head == End) Head = Start;
        if(Head == Tail) return 0;
        --Count;
    } while(Count);

    return  0xFF;
}


/**
    This routine gets a character from the character buffer.
    Character buffer pointers are also updated

    @param VOID

    @retval Character taken from the character buffer

**/

UINT8
USBKBC_GetFromCharacterBuffer()
{
    UINT8       bValue;
    UINT8       *fPtr, *fPtrEnd;

    fPtrEnd = gUsbData->KbcCharacterBufferStart +
                sizeof (gUsbData->KbcCharacterBufferStart);
    fPtr = gUsbDataList->KbcCharacterBufferTail;
    bValue = *fPtr;
    ++fPtr;
    //
    // Check for buffer end condition
    //
    if(fPtr == fPtrEnd)
        fPtr = gUsbData->KbcCharacterBufferStart;
    gUsbDataList->KbcCharacterBufferTail = fPtr;

    return  bValue;
}


/**
    This routine puts a character into the character buffer.
    Character buffer pointers are also updated

    @param Char            A character

    @retval VOID

**/

VOID
USBKBC_SendToCharacterBuffer (UINT8 Char)
{
    UINT8 *Pointer, *PtrEnd;

    PtrEnd = gUsbData->KbcCharacterBufferStart +
                sizeof (gUsbData->KbcCharacterBufferStart);

    Pointer = gUsbDataList->KbcCharacterBufferHead;
    *Pointer = Char;
    ++Pointer;

    if(Pointer == PtrEnd) {
        Pointer = gUsbData->KbcCharacterBufferStart;
    }

    gUsbDataList->KbcCharacterBufferHead     = Pointer;

    USBKeyRepeat(NULL, 2);  // Enable Key repeat
}


/**
    This routine converts the USB keycode into scan code set
    2 scan code. Conversion is accomplished using a static table.

    @param KeyCode     USB Key code

    @retval ScanCode   Set 2 scan code for the key

**/

UINT8
USBKB_ConvertUSBKeyCodeToScanCodeSet2 (UINT8 KeyCode)
{
    return aUSBKeyCodeToScanCodeSet2Table[KeyCode];
}


/**
    This routine checks whether any of the modifier keys, like
    shift, control or alternate keys, are pressed

    @param ScanCode   Scan code set 2 scan code

    @retval 0          None of the modifier keys are pressed
    @retval Value      Modifier key is pressed
                       High Byte, Low Byte = Modifier key identifier

**/

UINT16
USBKB_CheckModifierKeyPress (UINT8 ScanCode)
{
    UINT8       LByte = 0,
                HByte = 0;
    UINT16      Value;
    //
    // Check for left shift key status
    //
    if(ScanCode == LEFT_SHIFT)
        HByte  = KB_LSHIFT_KEY_BIT_MASK;
    //
    // Check for right shift key status
    //
    if (ScanCode == RIGHT_SHIFT)
        HByte  = KB_RSHIFT_KEY_BIT_MASK;
    //
    // Check for left control key status
    //
    if (ScanCode == LEFT_CTRL)
    {
        HByte  = KB_CTRL_KEY_BIT_MASK;
        LByte  = RIGHT_CTRL;
    }
    //
    // Check for left control key status
    //
    if (ScanCode == RIGHT_CTRL)
    {
        HByte  = KB_CTRL_KEY_BIT_MASK;
        LByte  = LEFT_CTRL;
    }
    //
    // Check for left alternate key status
    //
    if (ScanCode == LEFT_ALT)
    {
        HByte  = KB_ALT_KEY_BIT_MASK;
        LByte  = RIGHT_ALT;
    }
    //
    // Check for left alternate key status
    //
    if (ScanCode == RIGHT_ALT)
    {
        HByte  = KB_ALT_KEY_BIT_MASK;
        LByte  = LEFT_ALT;
    }
    Value      = (UINT16)(LByte + (HByte << 8));
    return(Value);
}


/**
    This routine will read the CCB from the keyboard controller
    and store it in a local variable

    @param None

    @retval None

**/

VOID
USBKBC_GetAndStoreCCB()
{
    KBC_KBDTRANSLATION tr;

    //ASSERT(gKbdInput);

    if (gKbdInput == NULL) {
        return;
    }
    //
    // CCB is internal buisness of KBC relted code
    // USB code only need to know about translation type
    //
    gKbdInput->GetTranslation(gKbdInput,&tr);
    gUsbData->bCCB = tr == KBC_KBDTRANS_PCXT? 0x40 : 0;
    return ;
}

/**
    This routine will check the repeat counter and repeat rate
    and perform the auto repeat appropriately

    @param None

    @retval None

**/
VOID
USBKBC_CheckAutoRepeat()
{

    UINT8   ScanCodeCount = (UINT8)(gUsbDataList->KbcScanCodeBufferPtr - 
                     (UINT8*)gUsbData->KbcScanCodeBufferStart);    
    UINT8   Index;

    if(ScanCodeCount){ 
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_7, "USBKBC_CheckAutoRepeat: \n");

        //
        // Update repeat counter
        //
        gUsbData->RepeatCounter++;

        if (gUsbData->RepeatCounter >= gUsbData->RepeatRate) {

            //
            // Repeat rate reached. Reload repeat delay counter
            //
            if (gUsbData->RepeatIntervalMode){
                gUsbData->RepeatRate = aTypematicRateDelayTable[
                    (gUsbData->UsbKbcStatusFlag & KBC_TYPE_RATE_BIT_MASK_16MS) >>
                     KBC_TYPE_RATE_BIT_SHIFT_16MS];
            } else {
                gUsbData->RepeatRate = aTypematicRateDelayTable[
                    (gUsbData->UsbKbcStatusFlag & KBC_TYPE_RATE_BIT_MASK_8MS) >>
                     KBC_TYPE_RATE_BIT_SHIFT_8MS];            
            }
            gUsbData->RepeatCounter   = 0;

            //
            // Check for scan code in the buffer
            //
            if(gUsbDataList->KbcCharacterBufferHead ==
                            gUsbDataList->KbcCharacterBufferTail)
            {
                //
                // Buffer has character to process.
                //
                for(Index = 0; Index < ScanCodeCount; Index++)
                { 
                    USBKB_GenerateScanCode(USB_GEN_MAKE_CODE,
                                        gUsbData->KbcScanCodeBufferStart[Index], 0);
                }    
            }
        }
    }

}

/**
    This routine perform the auto repeat
    @param HcStruc   Pointer to the Host controller structure

    @retval None

**/
VOID
SysKbcAutoRepeat(
    HC_STRUC* HcStruc
)
{
    UINT8           BreakCodeDeviceId;
    UINT8           DevAddr;
    UINT8           *KbInputBuffer;
    DEV_INFO        *DevInfo;
    PS2MouseData    MouseData;
    EFI_STATUS      EfiStatus;
    static UINT8    Buffer[64];
    UINT32          Count = 1;

    USBKB_UpdateLEDState(0x0);
    
    BreakCodeDeviceId = gUsbData->BreakCodeDeviceId;
    
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_8, "USBKBDpih: BreakCodeID %x \n", BreakCodeDeviceId);

    if (BreakCodeDeviceId) {
        DevAddr = 1;

        while (!(BreakCodeDeviceId & 1)) {
            BreakCodeDeviceId >>= 1;
            ++DevAddr;
        }
        DevInfo = USB_GetDeviceInfoStruc(
                        USB_SRCH_DEV_ADDR,
                        (DEV_INFO*)0,
                        DevAddr,
                        HcStruc);
        ASSERT(DevInfo != NULL);
        if (DevInfo == NULL) {
            return;
        }

        KbInputBuffer = gUsbData->UsbKbInputBuffer;
        *KbInputBuffer = 0;
        ++KbInputBuffer;
        *KbInputBuffer = 0;
        --KbInputBuffer;

        USBKB_Scanner(DevInfo, KbInputBuffer);
    }

    USBKBC_CheckAutoRepeat();
    if (gUsbDataList->KbcCharacterBufferTail != gUsbDataList->KbcCharacterBufferHead) {
        // Get the Data from UsbKbInputBuffer Local Buffer
    *Buffer = *gUsbDataList->KbcCharacterBufferTail;
        //Optimization: Enable the code below; it must be more efficient
        //      UINT8  *p;
        //      for( p = Buffer; gUsbDataList->KbcCharacterBufferTail != gUsbDataList->KbcCharacterBufferHead;
        //              ++Count)
        //      *p++ = USBKBC_GetFromCharacterBuffer();

        if (gKbdInput->Send == SYSKBC_KbdInput_Send) {

            // USB driver Emulation function already working on the UsbKbInputBuffer buffer
            // So we don't need to remove the data from UsbKbInputBuffer incase if data send
            // Successfully 
            EfiStatus = gKbdInput->Send(gKbdInput, Buffer, Count);
        } else {
            // Other Emulation drivers doesn't use the UsbKbInputBuffer.
            // So remove the data from UsbKbInputBuffer if the data sent successfully.
            EfiStatus = gKbdInput->Send(gKbdInput, Buffer, Count);

          if (!EFI_ERROR(EfiStatus)) {
            *Buffer = USBKBC_GetFromCharacterBuffer();
          }
        }

        USBKBC_GetAndStoreCCB();
        USBKeyRepeat(NULL, 2);  
    } else if (gUsbDataList->MouseInputBufferHeadPtr !=
                        gUsbDataList->MouseInputBufferTailPtr) {

        // Get the Mouse data Packet from UsbMsInputBuffer buffer
        MouseData.flags = *gUsbDataList->MouseInputBufferTailPtr;
        MouseData.x = *(gUsbDataList->MouseInputBufferTailPtr + 1);
        MouseData.y = *(gUsbDataList->MouseInputBufferTailPtr + 2);

        if (gMsInput->Send == SYSKBC_MouseInput_Send) {

            // USB driver Emulation function already working on the UsbMsInputBuffer buffer
            // So we don't need to remove the data from UsbMsInputBuffer incase if data send
            // Successfully 
            EfiStatus = gMsInput->Send(gMsInput, &MouseData);
        } else {
            // Other Emulation drivers doesn't use the UsbMsInputBuffer.
            // So remove the data from UsbMsInputBuffer if the data sent successfully.
            EfiStatus = gMsInput->Send(gMsInput, &MouseData);
            if (!EFI_ERROR(EfiStatus)) {
                USBMouse_GetFromMouseBuffer();
                USBMouse_GetFromMouseBuffer();
                USBMouse_GetFromMouseBuffer();
        }
        }

        USBKeyRepeat(NULL, 2);  // Enable Key repeat

    } else if(gUsbDataList->KbcScanCodeBufferPtr != gUsbData->KbcScanCodeBufferStart ) {
        USBKeyRepeat(NULL, 2);  // Enable Key repeat
    } else {
        USBKeyRepeat(NULL, 1);  // Disable Key repeat
    }
}

static EFI_EMUL6064KBDINPUT_PROTOCOL TheKbdInput =
{
SYSKBC_KbdInput_Send,
SYSKBC_GetTranslation,
SYSKBC_UpdateLEDState
};

EFI_EMUL6064MSINPUT_PROTOCOL TheMsInput =
{
    SYSKBC_MouseInput_Send
};

/**
    Initial Keyboard data protocol and mouse protocol
    @param ppKbd   
    @param ppMouse   
    @retval None

**/
VOID 
EFIAPI
InitSysKbc(
    EFI_EMUL6064KBDINPUT_PROTOCOL** PpKbd,
    EFI_EMUL6064MSINPUT_PROTOCOL** PpMouse )
{
    *PpKbd   = &TheKbdInput;
    *PpMouse = &TheMsInput;
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
