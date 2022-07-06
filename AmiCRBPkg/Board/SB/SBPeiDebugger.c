
//<AMI_FHDR_START>
//----------------------------------------------------------------------------
//
// Name: SBPeiDebugger.C
//
// Description: This file contains PEI stage debugger code for SB template
//
//----------------------------------------------------------------------------
//<AMI_FHDR_END>

#include <Efi.h>
#include <AmiLib.h>
#include <AmiCspLib.h>
#include <AmiDebugPort.h>
#include <Token.h>

//Sudhir TEMP Declarations
//#define EHCI_MMIO_BASE_ADDRESS 0xDFFFFC00
//#define EHCI_MMIO_SIZE 0x400
//#define DEBUG_EHCI_BUS_NUMBER 0x0
//#define DEBUG_EHCI_DEVICE_NUMBER 0x1A
//#define DEBUG_EHCI_FUNCTION_NUMBER 0x0
//
#define ICH5_PMCSR         0x8000
#define PWR_MGT_CAP_ID     1
#define DBG_PRT_CAP_ID     0xA
#define TIMER_CONTROL_PORT 0x43
#define TIMER0_COUNT_PORT  0x40

// Function Prototypes
UINT8  ReadPCIRegByte( IN UINT8 RegNum, IN UINT8 BusNum, IN UINT8 DevFunc);
UINT16 ReadPCIRegWord( IN UINT8 RegNum, IN UINT8 BusNum, IN UINT8 DevFunc);
UINT32 ReadPCIRegDWord( IN UINT8 RegNum, IN UINT8 BusNum, IN UINT8 DevFunc);
VOID   WritePCIRegByte( IN UINT8 RegNum, IN UINT8 BusNum, IN UINT8 DevFunc, IN UINT8 val);
VOID   OrPCIRegByte( IN UINT8 RegNum, IN UINT8 BusNum, IN UINT8 DevFunc, IN UINT8 val);
VOID   WritePCIRegWord( IN UINT8 RegNum, IN UINT8 BusNum, IN UINT8 DevFunc, IN UINT16 val);
VOID   WritePCIRegDWord( IN UINT8 RegNum, IN UINT8 BusNum, IN UINT8 DevFunc, IN UINT32 val);
VOID   OrPCIRegDWord( IN UINT8 RegNum, IN UINT8 BusNum, IN UINT8 DevFunc, IN UINT32 val);
VOID   ANDPCIRegDWord( IN UINT8 RegNum, IN UINT8 BusNum, IN UINT8 DevFunc, IN UINT32 val);
VOID   ANDPCIRegByte( IN UINT8 RegNum, IN UINT8 BusNum, IN UINT8 DevFunc, IN UINT8 val);
UINT32 GetPCIAddrFormat( IN UINT8 RegNum, IN UINT8 BusNum, IN UINT8 DevFunc);
UINT8  FindCapPtrDbg( IN UINT8 Bus, IN UINT8 DevFunc, IN UINT8 CapId);
VOID   PEI8259WriteMask( IN UINT16 Mask, IN UINT16 EdgeLevel);
VOID   PEI8259SetVectorBase ( IN UINT8 MasterBase, IN UINT8 SlaveBase);
VOID   Program8254Timer0( IN UINT16 Count);

                                        // (P051810B)>
//Macros for Mmio Read/Write

#define MmioOr32(Address, Value)    (*(volatile UINT32*)(UINTN)(Address) |= (Value))
#define MmioAnd32(Address, Value)    (*(volatile UINT32*)(UINTN)(Address) &= (Value))

#define STALL_PERIOD   10               // 10 microseconds
#define STALL_TIME     1000000          // 1,000,000 microseconds = 1 second


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SmbusGetIoBase
//
// Description:
//
// Input:
//
// Output: UINT32
//
// Notes:
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
UINT32 SmbusGetIoBase (VOID)
{
    return SMBUS_BASE_ADDRESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SmbusIoRead
//
// Description:
//
// Input: IN UINT8 Offset
//
// Output: UINT8
//
// Notes:
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
UINT8 SmbusIoRead (IN UINT8 Offset)
{
    return IoRead8 (SmbusGetIoBase () + Offset);
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SmbusIoWrite
//
// Description:
//
// Input: 
//
// Output: 
//
// Notes:
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID SmbusIoWrite (IN UINT8 Offset, IN UINT8 Data)
{
  //
  // Write New Value
  //
  IoWrite8 (SmbusGetIoBase () + Offset, Data);

  return ;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: PchPmTimerStall
//
// Description:
//
// Input: 
//
// Output: 
//
// Notes:
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID PchPmTimerStall (IN UINTN Usec)
{
  UINTN   Counter = Usec * 3;
  UINTN   i;
  UINT32  Data32;
  UINT32  PrevData;

  PrevData = IoRead32(PM_BASE_ADDRESS + ICH_IOREG_PM1_TMR); // 08h
  for (i = 0; i < Counter; )
  {
    Data32 = IoRead32(PM_BASE_ADDRESS + ICH_IOREG_PM1_TMR); // 08h
    if (Data32 < PrevData)              // Reset if there is a overlap
    {
      PrevData=Data32;
      continue;
    }
    i += (Data32 - PrevData); 
    PrevData=Data32;
  }

  return;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: IoDone
//
// Description:
//
// Input: 
//
// Output: 
//
// Notes:
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
BOOLEAN IoDone (IN UINT8 *StsReg)
{
  //
  // Wait for IO to complete
  //
  UINTN StallIndex;
  UINTN StallTries;

  StallTries  = STALL_TIME / STALL_PERIOD;
  
  for (StallIndex  = 0; StallIndex < StallTries; StallIndex++)
  {
    *StsReg = SmbusIoRead (SMB_HST_STS);
    if (*StsReg & (HST_STS_INTR | HST_STS_DS | HST_STS_DEV_ERR | HST_STS_BUS_ERR)) {
      return TRUE;
        } else {
        PchPmTimerStall (STALL_PERIOD);
        }
    }
    return FALSE;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: AcquireBus
//
// Description:
//
// Input: 
//
// Output: 
//
// Notes:
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS AcquireBus (VOID)
{
    UINT8 StsReg;

    StsReg  = 0;
    StsReg  = SmbusIoRead (SMB_HST_STS);
    if (StsReg & HST_STS_INUSE) {
        return EFI_TIMEOUT;
    } else if (StsReg & HST_STS_HOST_BUSY) {
        //
        // Clear Status Register and exit
        //
        SmbusIoWrite (SMB_HST_STS, HST_STS_ALL);
        return EFI_TIMEOUT;
    } else {
        //
        // Clear out any odd status information (Will Not Clear In Use)
        //
        SmbusIoWrite (SMB_HST_STS, StsReg);
        return EFI_SUCCESS;
    }
    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SmbusExec
//
// Description:
//
// Input: 
//
// Output: 
//
// Notes:
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS SmbusExec (
  IN OUT  UINTN                     *Length,
  IN OUT  VOID                      *Buffer
  )
{
  EFI_STATUS  Status;
  UINT8       AuxcReg;
  UINT8       AuxStsReg;
  UINT8       SmbusOperation;
  UINT8       StsReg;
  UINT8       SlvAddrReg;
  UINT8       HostCmdReg;
  UINT8       BlockCount;
  UINTN       Index;
  UINTN       BusIndex;
  UINT8       *CallBuffer;
  UINT8       ClockAddress = 0xD2;

    CallBuffer  = Buffer;
    BlockCount  = 0;
    
    //
    // See if its ok to use the bus based upon INUSE_STS bit.
    //
    Status = AcquireBus ();
    if (EFI_ERROR (Status)) {
        return Status;
    }
    //
    // This is the main operation loop.  If the operation results in a Smbus
    // collision with another master on the bus, it attempts the requested
    // transaction again at least BUS_TRIES attempts.
    //
    for (BusIndex = 0; BusIndex < BUS_TRIES; BusIndex++) {
        //
        // Operation Specifics (pre-execution)
        //
        Status          = EFI_SUCCESS;
        SmbusOperation  = SMB_CMD_QUICK;
        SlvAddrReg      = (UINT8) (ClockAddress | 1);
        HostCmdReg      = 0;
        AuxcReg         = 0;

        SmbusIoWrite (SMB_HST_DAT_0, *(UINT8 *) Length);
        SlvAddrReg--;
        BlockCount = (UINT8) (*Length);

        //
        //#### The "break;" command is not present here to allow code execution
        // do drop into the next case, which contains common code to this case.
        //
        SmbusOperation = SMB_CMD_BLOCK;
        if ((*Length < 1) || (*Length > 32)) {
            Status = EFI_INVALID_PARAMETER;
        }

        AuxcReg |= SMB_AUX_E32B;

        //
        // Set Auxiliary Control register
        //
        SmbusIoWrite (SMB_AUX_CTL, AuxcReg);

        //
        // Reset the pointer of the internal buffer
        //
        SmbusIoRead (SMB_HST_CNT);

        //
        // Now that the 32 byte buffer is turned on, we can write th block data
        // into it
        //
        for (Index = 0; Index < BlockCount; Index++) {
            //
            // Write next byte
            //
            SmbusIoWrite (SMB_HST_BLK_DAT, CallBuffer[Index]);
        }
        //
        // Set SMBus slave address for the device to send/receive from
        //
        SmbusIoWrite (SMB_HST_ADD, SlvAddrReg);

        //
        // Set Command register
        //
        SmbusIoWrite (SMB_HST_CMD, HostCmdReg);

        //
        // Set Control Register (Initiate Operation, Interrupt disabled)
        //
        SmbusIoWrite (SMB_HST_CNT, (UINT8) (SmbusOperation + HST_CNT_START));

        //
        // Wait for IO to complete
        //
        if (!IoDone (&StsReg)) {
            Status = EFI_TIMEOUT;
            break;
        } else if (StsReg & HST_STS_DEV_ERR) {
            AuxStsReg = SmbusIoRead (SMB_AUX_STS);
            if (AuxStsReg & SMB_AUXS_CRCE) {
                Status = EFI_CRC_ERROR;
            } else {
                Status = EFI_DEVICE_ERROR;
            }
            break;
        } else if (StsReg & HST_STS_BUS_ERR) {
            //
            // Clear the Bus Error for another try
            //
            Status = EFI_DEVICE_ERROR;
            SmbusIoWrite (SMB_HST_STS, HST_STS_BUS_ERR);
            //
            // Clear Status Registers
            //
            SmbusIoWrite (SMB_HST_STS, HST_STS_ALL);
            SmbusIoWrite (SMB_AUX_STS, SMB_AUXS_CRCE);
            //
            // If bus collision happens, stall some time, then try again
            // Here we choose 10 milliseconds to avoid MTCP transfer.
            //
            PchPmTimerStall (STALL_PERIOD);
            continue;
        }

        //
        // successfull completion
        // Operation Specifics (post-execution)
        //
        SmbusIoWrite (SMB_HST_STS, HST_STS_DS);

        if ((StsReg & HST_STS_BUS_ERR) && (Status != EFI_BUFFER_TOO_SMALL)) {
            //
            // Clear the Bus Error for another try
            //
            Status = EFI_DEVICE_ERROR;
            SmbusIoWrite (SMB_HST_STS, HST_STS_BUS_ERR);
            //
            // If bus collision happens, stall some time, then try again
            // Here we choose 10 milliseconds to avoid MTCP transfer.
            //
            PchPmTimerStall (STALL_PERIOD);
            continue;
        } else {
            break;
        }
    }

    //
    // Clear Status Registers and exit
    //
    SmbusIoWrite (SMB_HST_STS, HST_STS_ALL);
    SmbusIoWrite (SMB_AUX_STS, SMB_AUXS_CRCE);
    SmbusIoWrite (SMB_AUX_CTL, 0);
    return Status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SmbusInit
//
// Description:
//
// Input: 
//
// Output: 
//
// Notes:
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
/*
VOID SmbusInit(VOID)
{
  UINT8   ConfigurationTableDefault[] = CLOCK_GENERATOR_SETTINGS;
  UINTN   Length = sizeof (ConfigurationTableDefault);

  //
  // set the BAR & I/O space enable ourselves
  //
  WritePCIRegDWord(ICH_SMB_BASE, SMBUS_BUS, SMBUS_DEV_FUN,(UINT32)SMBUS_BASE_ADDRESS);
  OrPCIRegByte(ICH_SMB_CMD, SMBUS_BUS, SMBUS_DEV_FUN,(UINT8) B_IOSE);

  //
  // Reset the SMBus host controller
  //
  OrPCIRegByte(B_SSRESET, SMBUS_BUS, SMBUS_DEV_FUN,(UINT8) B_IOSE);

  //
  // Enable the SMBus host controller
  //  
  ANDPCIRegByte(ICH_SMB_HOSTC, SMBUS_BUS, SMBUS_DEV_FUN,(UINT8)((B_SMB_SMI_EN | B_I2C_EN)));
  OrPCIRegByte(ICH_SMB_HOSTC, SMBUS_BUS, SMBUS_DEV_FUN,(UINT8) B_HST_EN);

  //
  // Clear Status Register before anyone uses the interfaces
  //
  SmbusIoWrite (SMB_HST_STS, HST_STS_ALL);

  //
  //Do the Clock Programming
  //
  SmbusExec(&Length, ConfigurationTableDefault);

  return;
}
                                        // <(P051810B)
*/

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SBPEIDBG_InitUsbEhci
//
// Description: This eLink function is used to initialize the EHCI controller
//    debug port for USB PEI Debugging. It also fills the transport
//    interface structure with appropriate information
//
// Input:  DebugPort  Debug transport interface structure
//
// Output:  EFI_STATUS
//
// Notes:  PORTING REQUIRED
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

//#ifdef USB_DEBUGGER
//Grangeville AptioV Server Override: Changes done for SerialDebugger support
#if defined(SERIAL_DEBUGGER_SUPPORT) && (SERIAL_DEBUGGER_SUPPORT == 0)
EFI_STATUS SBPEIDBG_InitUsbEhci (
 IN OUT PEI_DBG_PORT_INFO *DebugPort
)
{
 UINT8    PwrMgtCapReg;
 UINT8    DbgPrtCapReg;
 UINT16   CmdReg;
 UINT16   DbgPrtBaseOffset;
 UINT32   EhciBaseAddress = 0;
 UINT8    PCI_EHCI_FUNCTION_NUMBER = 0;

 //Aptio V Server override, Support for AMIDebugRx. fixed In RoseCity only 0xFED00000 to 0xFEE00000 range is decoded before QPI Init.
 DebugPort->UsbDebugPort.USBBaseAddr = USBDebugPort_BaseAddresDecodedBeforeQPI;
 //Aptio V Server override, Support for AMIDebugRx.

                                        // (P051810B)>
 //Clear Thermal Base
 WritePCIRegDWord(R_PCH_THERMAL_TBARB, THERMAL_BUS, THERMAL_DEV_FUN, 0);
 WritePCIRegDWord(R_PCH_THERMAL_TBARBH, THERMAL_BUS, THERMAL_DEV_FUN, 0);

 if(DebugPort->UsbDebugPort.USBBaseAddr) {
   // DXE
   EhciBaseAddress = DebugPort->UsbDebugPort.USBBaseAddr;
 } else {
   // PEI
   EhciBaseAddress = EHCI_MMIO_BASE_ADDRESS;
   // Init Clockgen
#if INIT_CLKGEN_IN_PEI_DBG
   SmbusInit();
#endif
 }
                                        // <(P051810B)


 //First disable the EHCI device
 CmdReg = ReadPCIRegWord(PCI_CMD,       //command register
               PCI_EHCI_BUS_NUMBER,
               (UINT8)((UINT8)(PCI_EHCI_DEVICE_NUMBER << 3) | PCI_EHCI_FUNCTION_NUMBER));
 
 WritePCIRegWord(PCI_CMD,               //command register
               PCI_EHCI_BUS_NUMBER,
               (UINT8)((UINT8)(PCI_EHCI_DEVICE_NUMBER << 3) | PCI_EHCI_FUNCTION_NUMBER),
               0);

 // Assign base address register
 WritePCIRegDWord(PCI_EHCI_BAR_OFFSET,
               PCI_EHCI_BUS_NUMBER,
               (UINT8)((UINT8)(PCI_EHCI_DEVICE_NUMBER << 3) | PCI_EHCI_FUNCTION_NUMBER),
               EhciBaseAddress);

 //Set the Power state of the device to D0
 PwrMgtCapReg = FindCapPtrDbg(
               PCI_EHCI_BUS_NUMBER,
               (UINT8)((UINT8)(PCI_EHCI_DEVICE_NUMBER << 3) | PCI_EHCI_FUNCTION_NUMBER),
               PWR_MGT_CAP_ID
               );

 WritePCIRegWord(PwrMgtCapReg + 4,
               PCI_EHCI_BUS_NUMBER,
               (UINT8)((UINT8)(PCI_EHCI_DEVICE_NUMBER << 3) | PCI_EHCI_FUNCTION_NUMBER),
               ICH5_PMCSR);

 // Enable PCH EHCI register & make it Bus master
 CmdReg = CmdReg | 0x06;
 WritePCIRegWord(PCI_CMD,               //command register
               PCI_EHCI_BUS_NUMBER,
               (UINT8)((UINT8)(PCI_EHCI_DEVICE_NUMBER << 3) | PCI_EHCI_FUNCTION_NUMBER),
               CmdReg);

 // Locate the Debug port register Interface location
 DbgPrtCapReg = FindCapPtrDbg(
               PCI_EHCI_BUS_NUMBER,
               (UINT8)((UINT8)(PCI_EHCI_DEVICE_NUMBER << 3) | PCI_EHCI_FUNCTION_NUMBER),
               DBG_PRT_CAP_ID
               );

 DbgPrtBaseOffset = ReadPCIRegWord(DbgPrtCapReg + 2,
               PCI_EHCI_BUS_NUMBER,
               (UINT8)((UINT8)(PCI_EHCI_DEVICE_NUMBER << 3) | PCI_EHCI_FUNCTION_NUMBER));

 DbgPrtBaseOffset &= 0x1fff;
 if (DebugPort->UsbDebugPort.USBBaseAddr == 0)
 {
   DebugPort->UsbDebugPort.USBBaseAddr = EHCI_MMIO_BASE_ADDRESS;
// Aptio V Server Override : Fix for the USB ports (other than port 1A) not working when AMIDEBUGRX and Debugger is enabled.
//   DebugPort->UsbDebugPort.MemoryMappedIoSpaceSize = EHCI_MMIO_SIZE;
 }

 // Aptio V Server Override Start : Fix for the USB ports (other than port 1A) not working when AMIDEBUGRX and Debugger is enabled.
 DebugPort->UsbDebugPort.MemoryMappedIoSpaceSize = EHCI_MMIO_SIZE;
 // Aptio V Server Override End : Fix for the USB ports (other than port 1A) not working when AMIDEBUGRX and Debugger is enabled.
 DebugPort->UsbDebugPort.USBDebugPortStartAddr = EhciBaseAddress + DbgPrtBaseOffset;
 DebugPort->UsbDebugPort.PciBusNumber      = PCI_EHCI_BUS_NUMBER;
 DebugPort->UsbDebugPort.PciDeviceNumber   = PCI_EHCI_DEVICE_NUMBER;
 DebugPort->UsbDebugPort.PciFunctionNumber = PCI_EHCI_FUNCTION_NUMBER;
 DebugPort->UsbDebugPort.PciBAROffset      = PCI_EHCI_BAR_OFFSET;
 DebugPort->UsbDebugPort.InitUSBEHCI       = SBPEIDBG_InitUsbEhci;

 return EFI_SUCCESS;
}
//Grangeville AptioV Server Override: Changes done for SerialDebugger support
#endif

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: EnableSerialIRQ
//
// Description: This function programs the SB register to enable the serial
//    IRQ
//
// Input:  None
//
// Output:  None
//
// Notes:  PORTING REQUIRED
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID EnableSerialIRQ (VOID)
{
 // Device 31 , Function 0 , Bus 0 , Reg 64h, Value d0
 WritePCIRegByte (ICH_REG_LPC_SIRQ_CNTL, \
                  LPC_BUS, \
                  (UINT8)(LPC_DEVICE << 3 | LPC_FUNC), \
                  0xD0);
}

#if SB_IO_DECODE_IN_DBG
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: Enable_LPC_IO_Decode
//
// Description: This function programs the SB register to enable the LPC IO
//    Decoding ranges to enable the programming of SIO and Serial Port.
//
// Input:  None
//
// Output:  None
//
// Notes:  PORTING REQUIRED
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID Enable_LPC_IO_Decode (VOID)
{
  UINT16  LpcEn;

  // Enable COMA/COMB decode
  WritePCIRegWord (ICH_REG_LPC_IODEC, \
                   LPC_BUS, \
                   (UINT8)(LPC_DEVICE << 3 | LPC_FUNC), \
                   0x0010); // 0x80

  LpcEn = ReadPCIRegWord(ICH_REG_LPC_EN, 
                         LPC_BUS, \
                         (UINT8)(LPC_DEVICE << 3 | LPC_FUNC)) | 3;  // 0x82

  WritePCIRegWord (ICH_REG_LPC_EN, \
                   LPC_BUS, \
                   (UINT8)(LPC_DEVICE << 3 | LPC_FUNC), \
                   LpcEn);
}
#endif

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: ProgramACPIBaseToDisableTCO
//
// Description: This function programs the SB register to disable
//    the TCO timer. If this timer is not disabled the system
//    will shutdown or reset as soon as the timer is expired
//
// Input:  None
//
// Output:  None
//
// Notes:  PORTING REQUIRED
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID ProgramACPIBaseToDisableTCO (VOID)
{
 UINT32    dData;
 UINT16    TCOBASE;

 dData = PM_BASE_ADDRESS;                     //Port base addr Value of SDL token
 
 WritePCIRegDWord(ICH_REG_LPC_PMBASE,         //ACPI Base Addr (LPC I/F D31:F0)
      LPC_BUS, //bus #
      (UINT8)((LPC_DEVICE << 3) | LPC_FUNC),  //dev & func #
      dData);

 TCOBASE = (UINT16)(dData + 0x60);    //TCO base addr = 0xFF80 + 0x60

 //
 //Now! Enable the decode of IO range pointed by ACPI base address.
 // (Note: ACPI power management function is also enabled)
 dData = 0x00000080;         //bit 4 = 1 to enable ACPI

 WritePCIRegDWord(ICH_REG_LPC_ACPI_CNTL,        //ACPI_CNTL - ACPI control (LPC I/F D31:F0)
      LPC_BUS,         //bus #
      (UINT8)((LPC_DEVICE << 3) | LPC_FUNC),   //dev & func #
      dData);

 // Halt the TCO timer
 IoWrite16(TCOBASE + ICH_IOREG_TCO1_CNT, IoRead16(TCOBASE + ICH_IOREG_TCO1_CNT) | 0x0800); //TCOBASE+8 = TCO1 control register
 IoWrite16(TCOBASE + ICH_IOREG_TCO2_STS, IoRead16(TCOBASE + ICH_IOREG_TCO2_STS) | 0x0002);
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: SBPEIDBG_Initialize
//
// Description: This eLink function is used to initialize the South Bridge
//    for PEI Debugger support
//
// Input:  DebugPort  Debug transport interface structure
//
// Output:  EFI_STATUS
//
// Notes:  Normally no PORTING REQUIRED
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS SBPEIDBG_Initialize (
 IN OUT PEI_DBG_PORT_INFO *DebugPort
)
{
 //Program the TCO IO to avoid rebooting of the hardware
 ProgramACPIBaseToDisableTCO();

 // Init 8259 Controller 
 PEI8259SetVectorBase (LEGACY_MODE_BASE_VECTOR_MASTER, LEGACY_MODE_BASE_VECTOR_SLAVE ); //testing

 // Set all 8259 Interrupts to edge triggered and disabled
 PEI8259WriteMask (0xffff , 0x0000);

#if SB_IO_DECODE_IN_DBG
 Enable_LPC_IO_Decode();
#endif

 EnableSerialIRQ();
 Program8254Timer0(0);

 return EFI_SUCCESS;
}

/** 
//----------------------------------------------------------------------------
 NO PORTING IS GENERALLY REQUIRED FOR THE ROUTINES BELOW.
//----------------------------------------------------------------------------
 **/ 


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: GetPCIAddrFormat
//
// Description: Converts the provided bus, device, function and register
//    number of a PCI register into 32bit PCI access format
//
// Input:  RegNum  PCI Register number
//         BusNum  PCI Bus number
//         DevFunc PCI Device and function number
//
// Output:  Converted 32 bit PCI register access value
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

UINT32 
GetPCIAddrFormat(
 IN UINT8 RegNum,
 IN UINT8 BusNum,
 IN UINT8 DevFunc
)
{
 UINT32 addr;

 addr = ((UINT32) (( (UINT16) BusNum ) << 8) +  DevFunc) << 8;
 addr += (RegNum & 0xfc);
 addr |= 0x80000000;
 return addr;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: ReadPCIRegByte
//
// Description: Reads a byte value from the PCI address space
//
// Input:  RegNum  PCI Register number
//         BusNum  PCI Bus number
//         DevFunc PCI Device and function number
//
// Output:  Byte read
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

UINT8
ReadPCIRegByte(
 IN UINT8 RegNum,
 IN UINT8 BusNum,
 IN UINT8 DevFunc
)
{ 
 UINT32 PciAddr = GetPCIAddrFormat(RegNum, BusNum, DevFunc);

 IoWrite32(0xcf8, PciAddr);
 return IoRead8(0xcfc +(RegNum & 0x3));
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: ReadPCIRegWord
//
// Description: Reads a word value from the PCI address space
//
// Input:  RegNum  PCI Register number
//         BusNum  PCI Bus number
//         DevFunc PCI Device and function number
//
// Output:  Word read
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

UINT16
ReadPCIRegWord(
 IN UINT8 RegNum,
 IN UINT8 BusNum,
 IN UINT8 DevFunc
)
{ 
 UINT32 PciAddr = GetPCIAddrFormat(RegNum,BusNum,DevFunc);

 IoWrite32(0xcf8, PciAddr);
 return IoRead16(0xcfc +(RegNum & 0x3));
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: ReadPCIRegDWord
//
// Description: Reads a double word value from the PCI address space
//
// Input:  RegNum  PCI Register number
//         BusNum  PCI Bus number
//         DevFunc PCI Device and function number
//
// Output:  Double word read
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

UINT32
ReadPCIRegDWord(
 IN UINT8 RegNum,
 IN UINT8 BusNum,
 IN UINT8 DevFunc
)
{ 
 UINT32 PciAddr = GetPCIAddrFormat(RegNum,BusNum,DevFunc);

 IoWrite32(0xcf8, PciAddr);
 return IoRead32(0xcfc +(RegNum & 0x3));
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: WritePCIRegByte
//
// Description: Writes a byte value to the PCI address space
//
// Input:  RegNum  PCI Register number
//         BusNum  PCI Bus number
//         DevFunc PCI Device and function number
//         Val     Value to write to the PCI address space
//
// Output:  None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID 
WritePCIRegByte(
 IN UINT8 RegNum,
 IN UINT8 BusNum,
 IN UINT8 DevFunc,
 IN UINT8 val
)
{ 
 UINT32 PciAddr = GetPCIAddrFormat(RegNum,BusNum,DevFunc);

 IoWrite32(0xcf8, PciAddr);
 IoWrite8(0xcfc + (RegNum & 0x3), val);
}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: OrPCIRegByte
//
// Description: Or a byte value to the PCI address space
//
// Input:  RegNum  PCI Register number
//         BusNum  PCI Bus number
//         DevFunc PCI Device and function number
//         Val     Value to write to the PCI address space
//
// Output:  None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID 
OrPCIRegByte(
 IN UINT8 RegNum,
 IN UINT8 BusNum,
 IN UINT8 DevFunc,
 IN UINT8 val
)
{ 
 UINT32 PciAddr = GetPCIAddrFormat(RegNum,BusNum,DevFunc);
 UINT8 Value;

 IoWrite32(0xcf8, PciAddr);
 Value = IoRead8(0xcfc + (RegNum & 0x3));
 Value |= val;
 IoWrite32(0xcf8, PciAddr);
 IoWrite8(0xcfc + (RegNum & 0x3), Value);
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: ANDPCIRegByte
//
// Description: And a byte value to the PCI address space
//
// Input:  RegNum  PCI Register number
//         BusNum  PCI Bus number
//         DevFunc PCI Device and function number
//         Val     Value to AND to the PCI address space
//
// Output:  None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID 
ANDPCIRegByte(
 IN UINT8 RegNum,
 IN UINT8 BusNum,
 IN UINT8 DevFunc,
 IN UINT8 val
)
{ 
 UINT32 PciAddr = GetPCIAddrFormat(RegNum, BusNum, DevFunc);
 UINT8 Value;

 IoWrite32(0xcf8, PciAddr);
 Value=IoRead8(0xcfc +(RegNum & 0x3));
 Value &= ~val;
 IoWrite32(0xcf8, PciAddr);
 IoWrite8(0xcfc + (RegNum & 0x3), Value);
}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: WritePCIRegWord
//
// Description: Writes a word value to the PCI address space
//
// Input:  RegNum  PCI Register number
//         BusNum  PCI Bus number
//         DevFunc PCI Device and function number
//         Val     Value to write to the PCI address space
//
// Output:  None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID 
WritePCIRegWord(
 IN UINT8 RegNum,
 IN UINT8 BusNum,
 IN UINT8 DevFunc,
 IN UINT16 val
)
{ 
 UINT32 PciAddr = GetPCIAddrFormat(RegNum,BusNum,DevFunc);

 IoWrite32(0xcf8, PciAddr);
 IoWrite16(0xcfc + (RegNum & 0x3), val);
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: WritePCIRegDword
//
// Description: Writes a double word value to the PCI address space
//
// Input:  RegNum  PCI Register number
//         BusNum  PCI Bus number
//         DevFunc PCI Device and function number
//         Val     Value to write to the PCI address space
//
// Output:  None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID 
WritePCIRegDWord(
 IN UINT8 RegNum,
 IN UINT8 BusNum,
 IN UINT8 DevFunc,
 IN UINT32 val
)
{ 
 UINT32 PciAddr = GetPCIAddrFormat(RegNum,BusNum,DevFunc);

 IoWrite32(0xcf8, PciAddr);
 IoWrite32(0xcfc +(RegNum & 0x3), val);
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: OrPCIRegDWord
//
// Description: Or a double word value to the PCI address space
//
// Input:  RegNum  PCI Register number
//         BusNum  PCI Bus number
//         DevFunc PCI Device and function number
//         Val     Value to write to the PCI address space
//
// Output:  None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID 
OrPCIRegDWord(
 IN UINT8 RegNum,
 IN UINT8 BusNum,
 IN UINT8 DevFunc,
 IN UINT32 val
)
{ 
 UINT32  Value;
 UINT32 PciAddr = GetPCIAddrFormat(RegNum, BusNum, DevFunc);

 IoWrite32(0xcf8, PciAddr);
 Value = IoRead32(0xcfc + (RegNum & 0x3));
 Value|= val;
 IoWrite32(0xcf8, PciAddr);
 IoWrite32(0xcfc + (RegNum & 0x3), Value);

 return;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: ANDPCIRegDWord
//
// Description: AND a double word value to the PCI address space
//
// Input:  RegNum  PCI Register number
//         BusNum  PCI Bus number
//         DevFunc PCI Device and function number
//         Val     Value to AND to the PCI address space
//
// Output:  None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID 
ANDPCIRegDWord(
 IN UINT8 RegNum,
 IN UINT8 BusNum,
 IN UINT8 DevFunc,
 IN UINT32 val
)
{ 
 UINT32 Value;
 UINT32 PciAddr = GetPCIAddrFormat(RegNum, BusNum, DevFunc);

 IoWrite32(0xcf8, PciAddr);
 Value = IoRead32(0xcfc + (RegNum & 0x3));
 Value &= ~val;
 IoWrite32(0xcf8, PciAddr);
 IoWrite32(0xcfc + (RegNum & 0x3), Value);

 return;
}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: FindCapPtrDbg
//
// Description: This function searches the PCI address space for the PCI
//              device specified for a particular capability ID and returns
//              the offset in the PCI address space if one found
//
// Input:  Bus      PCI Bus number
//         DevFunc  PCI Device and function number
//         CapId    Capability ID to look for
//
// Output:  Capability ID location if one found
//          Otherwise returns 0
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
UINT8 
FindCapPtrDbg(
 IN UINT8 Bus,
 IN UINT8 DevFunc,
 IN UINT8 CapId
)
{
 UINT8  Value;
 UINT8  Reg;

 Value = ReadPCIRegByte(ICH_REG_EHCI_PCISTS, Bus, DevFunc);

 if (Value == 0xff) return 0;          //No device.
 if (!(Value & (1 << 4))) return 0;    // Check if capabilities list.
  
 Reg = ICH_REG_EHCI_CAPTR;             // Register to First capabilities pointer
                                       //if 0, then capabilities
 for(;;)
 {
  Value = ReadPCIRegByte(Reg, Bus, DevFunc);
  if (!Value) return 0;
    
  Reg = Value;  // ptr to CapID
  Value = ReadPCIRegByte(Reg, Bus, DevFunc);  //If capablity ID, return register that points to it.
  if (Value == CapId) return (UINT8)Reg; 

  ++Reg;   //equals to next capability pointer.
   }
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: PEI8259WriteMask
//
// Description: Writes PC 8259 interrupt Controller mask register
//
// Input:  Mask  Mask to write
//    EdgeLevel Edge/level trigger to be programmed
//
// Output:  None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID 
PEI8259WriteMask (
 IN UINT16  Mask, 
 IN UINT16  EdgeLevel)
{
 IoWrite8 (LEGACY_8259_MASK_REGISTER_MASTER, (UINT8)Mask);
 IoWrite8 (LEGACY_8259_MASK_REGISTER_SLAVE,  (UINT8)(Mask >> 8));
 IoWrite8 (LEGACY_8259_EDGE_LEVEL_TRIGGERED_REGISTER_MASTER, (UINT8)EdgeLevel);
 IoWrite8 (LEGACY_8259_EDGE_LEVEL_TRIGGERED_REGISTER_SLAVE,  (UINT8)(EdgeLevel >> 8));
}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: PEI8259SetVectorBase
//
// Description: Set up the 8259 interrupt controller master and slave
//    base values
//
// Input:  MasterBase  Master base to be programmed
//    SlaveBase  Slave base to be programmed
//
// Output:  None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID 
PEI8259SetVectorBase (
 IN UINT8  MasterBase, 
 IN UINT8  SlaveBase
)
{
 UINT8   Mask;
 
 // Initialize Slave interrupt controller.
 Mask = IoRead8 (LEGACY_8259_MASK_REGISTER_SLAVE);
 IoWrite8 (LEGACY_8259_CONTROL_REGISTER_SLAVE, 0x11);
 IoWrite8 (LEGACY_8259_MASK_REGISTER_SLAVE,    SlaveBase);
 IoWrite8 (LEGACY_8259_MASK_REGISTER_SLAVE,    0x02); 
 IoWrite8 (LEGACY_8259_MASK_REGISTER_SLAVE,    0x01);
 IoWrite8 (LEGACY_8259_MASK_REGISTER_SLAVE,    Mask);

 // Initialize Master interrupt controller.
 Mask = IoRead8 (LEGACY_8259_MASK_REGISTER_MASTER);
 IoWrite8 (LEGACY_8259_CONTROL_REGISTER_MASTER, 0x11);
 IoWrite8 (LEGACY_8259_MASK_REGISTER_MASTER,    MasterBase);
 IoWrite8 (LEGACY_8259_MASK_REGISTER_MASTER,    0x04);
 IoWrite8 (LEGACY_8259_MASK_REGISTER_MASTER,    0x01); 
 IoWrite8 (LEGACY_8259_MASK_REGISTER_MASTER,    Mask); 

 IoWrite8 (LEGACY_8259_CONTROL_REGISTER_SLAVE,  LEGACY_8259_EOI);
 IoWrite8 (LEGACY_8259_CONTROL_REGISTER_MASTER, LEGACY_8259_EOI);
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure: Program8254Timer0
//
// Description: Programs the 8254 Timer channel 0
//
// Input:  Count Timer tick count
//
// Output:  None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID 
Program8254Timer0(
 IN UINT16  Count
)
{
 UINT8 LCntbyte,HCntbyte;

 LCntbyte = (UINT8)Count;
 HCntbyte = (UINT8)(Count >> 8);

 //Write the timer control port to select timer 0 and set to mode 3
 IoWrite8(TIMER_CONTROL_PORT, 0x36);

 //Write the Counter 0 with initial count value
 IoWrite8(TIMER0_COUNT_PORT, LCntbyte);
 IoWrite8(TIMER0_COUNT_PORT, HCntbyte);

 //Enable the IRQ0 interrupt for this timer 0 in USB Xport module
 //......
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
