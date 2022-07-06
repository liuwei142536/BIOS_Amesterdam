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
//
// $Header: $
//
// $Revision: $
//
// $Date: $
//
//*****************************************************************************
//<AMI_FHDR_START>
//----------------------------------------------------------------------------
//
// Name:        SBCspLib.h
//
// Description: 
//
// Notes:       
//
//----------------------------------------------------------------------------
//<AMI_FHDR_END>
#ifndef __SBLIB_H__
#define __SBLIB_H__

#include <efi.h>
#include <token.h>
#include <Protocol\PciRootBridgeIo.h>
#include <Protocol\BootScriptSave.h>
                                        // [EIP21061]>
#include <Protocol\PciIo.h>
#include <Protocol\AmiSio.h>
                                        // <[EIP21061]

                                        // (P042709A)>
//#include <Include\CmosAccess.h>
#if CMOS_MANAGER_SUPPORT
#include <CmosAccess.h>
#endif
                                        // <(P042709A)

#if CSM_SUPPORT
#include <Protocol\LegacyInterrupt.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if CSM_SUPPORT
EFI_STATUS
SBGen_InitializeRouterRegisters (
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRBIo);

UINT8
SBGen_GetPIRQIndex (
  UINT8 PIRQRegister);

EFI_STATUS 
SBGen_ReadPirq (
 IN EFI_LEGACY_INTERRUPT_PROTOCOL *This,
 IN UINT8       PirqNumber,
 OUT UINT8       *PirqData );

EFI_STATUS 
SBGen_WritePirq(
 IN EFI_LEGACY_INTERRUPT_PROTOCOL *This,
 IN UINT8       PirqNumber,
 IN UINT8       PirqData);


EFI_STATUS SBGen_WriteBootScript(
 IN EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave
);

#endif

EFI_STATUS CheckLFDPlatform(VOID);
EFI_STATUS CheckHVDPlatform(VOID);
EFI_STATUS CheckJFPlatform(VOID);       // (P112309C)

EFI_STATUS CheckNotJFPlatform(VOID);    // (P112309C)

EFI_STATUS CheckAuburndalePlatform(VOID); // (P042009C)
EFI_STATUS PchAxPatch(VOID);            // (P020309C)
EFI_STATUS PchB0Patch(VOID);            // (P020309C)
EFI_STATUS CheckPchMobileSku(VOID);     // (P021309C)
EFI_STATUS CheckPchDesktopSku(VOID);    // (P021309C)
EFI_STATUS EnableUsbr(VOID);            // (P052109A)(P042709B)
EFI_STATUS DisableUsbr(VOID);           // (P052109A)(P042709B)
BOOLEAN IsUsbRmhMode(VOID);             // [EIP21454]
EFI_STATUS DMIInterfacePatch(VOID);     // (P042809D)
EFI_STATUS SATAPatch(VOID);             // (P042809F)

VOID SBLib_Shutdown();
VOID SB_Shutdown();
BOOLEAN SbLib_GetSmiState(VOID);
VOID SbLib_SmiDisable(VOID);
VOID SbLib_SmiEnable(VOID);
VOID SbDelayUs(IN UINT16 wCount);       // (P052209B)

                                        // (P121409A)>
UINT8 SBLib_CmosRead(
  IN UINT8 Index
);

VOID SBLib_CmosWrite(
  IN UINT8 Index,
  IN UINT8 Value
);
                                        // <(P121409A)

#if SMM_SUPPORT                         // (P062409B)>
EFI_STATUS SBSmmSaveRestoreStates(IN BOOLEAN Save);

EFI_STATUS SbInstallSmmConfigurationTable(
    IN EFI_SMM_SYSTEM_TABLE *SmmSystemTable,
    IN EFI_GUID *Guid,
    IN VOID* Table,
    IN UINTN TableSize);
#endif                                  // <(P062409B)

#if SB_RESET_PPI_SUPPORT
void SBLib_ResetSystem (
 IN EFI_RESET_TYPE  ResetType
);

EFI_STATUS SetResetType(
    IN EFI_RUNTIME_SERVICES *pRS,
    IN EFI_RESET_TYPE       ResetType
);
#endif

#if SB_STALL_PPI_SUPPORT
EFI_STATUS
CountTime (
 IN UINTN   DelayTime,
 IN UINT16   BaseAddr // only needs to be 16 bit for I/O address
);
#endif

                                        // (P010609A)>
//typedef EFI_STATUS (*EFI_PEI_SB_TASK_FUNCTION)(
// IN EFI_PEI_SERVICES **PeiServices,
// IN OUT SB_SETUP_CONFIGURATION *SbSetup
//);
                                        // <(P010609A)

                                        // [EIP21061]>
EFI_STATUS SbLib_SetLpcDeviceDecoding (
    IN EFI_PCI_IO_PROTOCOL      *LpcPciIo,
    IN UINT16                   Base,
    IN UINT8                    DevUid,
    IN SIO_DEV_TYPE             Type
);

EFI_STATUS SbLib_SetLpcGenericDecoding (
    IN EFI_PCI_IO_PROTOCOL      *LpcPciIo,
    IN UINT16                   Base,
    IN UINT16                   Length,
    IN BOOLEAN                  Enable
);
                                        // <[EIP21061]

                                        // (P042709A)>
#if CMOS_MANAGER_SUPPORT
EFI_STATUS ReadWriteCmosBank2 (
    IN EFI_PEI_SERVICES         **PeiServices,  // NULL in DXE phase
    IN CMOS_ACCESS_TYPE         AccessType,
    IN UINT16                   CmosRegister,
    IN OUT UINT8                *CmosParameterValue
);
#endif
                                        // <(P042709A)

                              // (P071709A)>
UINT32 SbFindCapPtr(
    IN UINT64  PciAddress,
    IN UINT8   CapId
);
                              // <(P071709A)

//----------------------------------------------------------------------------
#if SB_SMBUS_PPI_SUPPORT

VOID  TCOWorkAround();
VOID  ClearHostStatus(IN UINTN SmbIoAddr);
VOID  ClearHostByteDoneStatus(IN UINTN SmbIoAddr);
UINT8 SmbusReadCommand(UINTN Register);
EFI_STATUS WaitForOperationComplete(UINTN  SmBusIoAddr );
EFI_STATUS SmbusWriteCommand(UINT16 Register,UINT16 Data);
EFI_STATUS ReadSMB(UINTN SmbIoAddr, UINT8 *bData, UINT8 CtrlFlag);
EFI_STATUS WriteSMB(UINTN SmbIoAddr, UINTN DataByte, UINT8 CtrlFlag);
EFI_STATUS CheckReadOperationComplete(UINTN SmBusAddr);

#endif
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
EFI_STATUS SBInitializeHDA(VOID);
EFI_STATUS IntelEhciWorkaround(VOID);   // (P051509B)
EFI_STATUS EnableSpiLock(VOID);         // (P102909A)

//----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif


//-----------------------------------------------------------------------
//
//-----------------------------------------------------------------------
#ifndef _EFI_MMIO_ACCESS_H_
#define _EFI_MMIO_ACCESS_H_

#define MmioAddress(BaseAddr, Register) \
 ( (UINTN)BaseAddr + (UINTN)(Register) )

// 32-bit
#define Mmio32Ptr(BaseAddr, Register) \
 ( (volatile UINT32 *)MmioAddress(BaseAddr, Register) )

#define Mmio32(BaseAddr, Register) \
 *Mmio32Ptr(BaseAddr, Register)

// 16-bit
#define Mmio16Ptr(BaseAddr, Register) \
 ( (volatile UINT16 *)MmioAddress(BaseAddr, Register) )

#define Mmio16(BaseAddr, Register) \
 *Mmio16Ptr(BaseAddr, Register)

// 8-bit
#define Mmio8Ptr(BaseAddr, Register) \
 ( (volatile UINT8 *)MmioAddress(BaseAddr, Register) )

#define Mmio8(BaseAddr, Register) \
 *Mmio8Ptr(BaseAddr, Register)

#define MmioRW8(Addr, set, reset) \
    (Mmio8(Addr, 0) = ((Mmio8(Addr, 0) & (UINT8)~(reset)) | (UINT8)set))

#endif
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
//
//-----------------------------------------------------------------------
#ifndef _EFI_PCI_ACCESS_H_
#define _EFI_PCI_ACCESS_H_

#define MmPciAddress(Base, Bus, Device, Function, Register) \
 ( (UINTN)(Base) + \
 (UINTN)(Bus << 20) + \
 (UINTN)(Device << 15) + \
 (UINTN)(Function << 12) + \
 (UINTN)(Register) )

// 32-bit
#define MmPci32Ptr(Bus, Device, Function, Register) \
 ( (volatile UINT32 *)MmPciAddress( PcdGet64 (PcdPciExpressBaseAddress), Bus, Device, Function, Register) )

#define MmPci32(Bus, Device, Function, Register) \
 *MmPci32Ptr(Bus, Device, Function, Register)

#define MmPciRead32(Bus, Device, Function, Register) \
    MmPci32(Bus, Device, Function, Register)

// 16-bit
#define MmPci16Ptr(Bus, Device, Function, Register) \
 ( (volatile UINT16 *)MmPciAddress(PcdGet64 (PcdPciExpressBaseAddress), Bus, Device, Function, Register) )

#define MmPci16(Bus, Device, Function, Register) \
 *MmPci16Ptr(Bus, Device, Function, Register)

// 8-bit
#define MmPci8Ptr(Bus, Device, Function, Register) \
 ( (volatile UINT8 *)MmPciAddress(PcdGet64 (PcdPciExpressBaseAddress), Bus, Device, Function, Register) )

#define MmPci8(Bus, Device, Function, Register) \
 *MmPci8Ptr(Bus, Device, Function, Register)

#endif
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
//
//-----------------------------------------------------------------------
#ifndef _ICSPLIB_H_
#define _ICSPLIB_H_

#define _CSP_LIB_ 2

typedef void (*TASK_FUNCTION) ();
typedef EFI_STATUS (*EFI_TASK_FUNCTION) (VOID);

//-----------------------------------------------------------------------
// Initialize MMIO Struct declaration
//-----------------------------------------------------------------------
typedef struct {
 UINT32 Register;
 UINT32 ANDMask;
 UINT32 ORMask;
 EFI_TASK_FUNCTION TaskFunctionPtr;
} AMI_INIT_MMIO_TABLE_STRUCT;
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Initialize MMIO Function declaration
//-----------------------------------------------------------------------
/*
EFI_STATUS ProgramMMIOTable
(
 UINTN BaseAddress, 
 AMI_INIT_MMIO_TABLE_STRUCT *pTable, 
 UINT16 Size
);

EFI_STATUS ProgramMMIOTable_S3
(
 EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave,
 UINTN BaseAddress, 
 AMI_INIT_MMIO_TABLE_STRUCT *pTable, 
 UINT16 Size
);
*/
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Read & Write MMIO Function declaration
//-----------------------------------------------------------------------

VOID MemRW8_S3(EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave, UINTN Address, UINT8 Value, UINT8 Mask);
VOID MemRW16_S3(EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave, UINTN Address, UINT16 Value, UINT16 Mask);
VOID MemRW32_S3(EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave, UINTN Address, UINT32 Value, UINT32 Mask);

VOID MemWrite8_S3(EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave, UINTN Address, UINT8 Value);
VOID MemWrite16_S3(EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave, UINTN Address, UINT16 Value);
VOID MemWrite32_S3(EFI_S3_SAVE_STATE_PROTOCOL *BootScriptSave, UINTN Address, UINT32 Value);

//-----------------------------------------------------------------------
#endif

#ifndef _SB_ICSPLIB_H_
#define _SB_ICSPLIB_H_
//-----------------------------------------------------------------------
// SB : GPIO Function and Struct Details
//-----------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
 UINT8 OFFSET;
 UINT8 USE:1;
 UINT8 IO:1;
 UINT8 LVL:1;
 UINT8 INV:1;
 UINT8 RST:1;                          // (P091109D)
} AMI_GPIO_STRUCT;

typedef union {
 UINT16 GPIO;
 AMI_GPIO_STRUCT CONFIG;
} AMI_INIT_GPIO_TABLE_STRUCT;

EFI_STATUS ProgramGPIOTable
(
 UINTN BaseAddress, 
 AMI_INIT_GPIO_TABLE_STRUCT *pTable, 
 UINT16 Size,
 UINT32 *USE1_SEL,
 UINT32 *USE2_SEL,
 UINT32 *USE3_SEL,
 UINT32 *IO1_SEL,
 UINT32 *IO2_SEL,
 UINT32 *IO3_SEL,
 UINT32 *LVL1_SEL,
 UINT32 *LVL2_SEL,
 UINT32 *LVL3_SEL,
 UINT32 *INV1_SEL
);
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//
//-----------------------------------------------------------------------
EFI_STATUS ReadSPIDescriptor (
  IN UINT8 FDSS,
  IN UINT8 FDSI,
  OUT UINT32 *FDOD
);
//-----------------------------------------------------------------------

#ifndef __CSP_LIB__
#define __CSP_LIB__

#define _SBCSP_LIB_ 1

UINT8
ReadPci8(
    IN UINT8    Bus, 
    IN UINT8    Dev,
    IN UINT8    Fun, 
    IN UINT16   Reg
);

UINT16
ReadPci16(
    IN UINT8    Bus, 
    IN UINT8    Dev,
    IN UINT8    Fun, 
    IN UINT16   Reg
);

UINT32
ReadPci32(
    IN UINT8    Bus, 
    IN UINT8    Dev,
    IN UINT8    Fun, 
    IN UINT16   Reg
);

VOID
WritePci8(
    IN UINT8    Bus, 
    IN UINT8    Dev,
    IN UINT8    Fun, 
    IN UINT16   Reg,
    IN UINT8    Value8
);

VOID
WritePci16(
    IN UINT8    Bus, 
    IN UINT8    Dev,
    IN UINT8    Fun, 
    IN UINT16   Reg,
    IN UINT16   Value16
);

VOID
WritePci32(
    IN UINT8    Bus, 
    IN UINT8    Dev,
    IN UINT8    Fun, 
    IN UINT16   Reg,
    IN UINT32   Value32
);

VOID
WriteMem8 (
    IN UINT64       Address,
    IN UINT8        Value8
);

VOID
WriteMem16 (
    IN UINT64       Address,
    IN UINT16       Value16
);

VOID
WriteMem32 (
    IN UINT64       Address,
    IN UINT32       Value32
);

VOID
RwMem8(
    IN UINT64   Address,
    IN UINT8    SetBit8,
    IN UINT8    ResetBit8
);

VOID
RwMem32(
    IN UINT64   Address,
    IN UINT32   SetBit32,
    IN UINT32   ResetBit32
);

VOID    CPULib_DisableInterrupt();
VOID    CPULib_EnableInterrupt();

BOOLEAN IsTopSwapOn(
    VOID
);

VOID  SetTopSwap(
    IN BOOLEAN                      On
);

//---------------------------------------------------------------------------
// Standard I/O Macros, No Porting Required.
//---------------------------------------------------------------------------

#define SET_IO8(IoAddr, bSet)     IoWrite8(IoAddr, IoRead8(IoAddr) | (bSet))
#define RESET_IO8(IoAddr, bRst)   IoWrite8(IoAddr, IoRead8(IoAddr) & ~(bRst))
#define SET_IO32(IoAddr, dSet)    IoWrite32(IoAddr, IoRead32(IoAddr) | (dSet))
#define RESET_IO32(IoAddr, Rst)   IoWrite32(IoAddr, IoRead32(IoAddr) & ~(Rst))

//----------------------------------------------------------------------------
// Standard PCI Macros, No Porting Required.
//----------------------------------------------------------------------------

#define READ_PCI8(Bx, Dx, Fx, Rx)           ReadPci8(Bx, Dx, Fx, Rx)
#define READ_PCI16(Bx, Dx, Fx, Rx)          ReadPci16(Bx, Dx, Fx, Rx)
#define READ_PCI32(Bx, Dx, Fx, Rx)          ReadPci32(Bx, Dx, Fx, Rx)
#define WRITE_PCI8(Bx, Dx, Fx, Rx, bVal)    WritePci8(Bx, Dx, Fx, Rx, bVal)
#define WRITE_PCI16(Bx, Dx, Fx, Rx, wVal)   WritePci16(Bx, Dx, Fx, Rx, wVal)
#define WRITE_PCI32(Bx, Dx, Fx, Rx, dVal)   WritePci32(Bx, Dx, Fx, Rx, dVal)
#define RW_PCI8(Bx, Dx, Fx, Rx, Set, Rst) \
          WritePci8(Bx, Dx, Fx, Rx, ReadPci8(Bx, Dx, Fx, Rx) & ~(Rst) | (Set))
#define RW_PCI16(Bx, Dx, Fx, Rx, Set, Rst) \
          WritePci16(Bx, Dx, Fx, Rx, ReadPci16(Bx, Dx, Fx, Rx) & ~(Rst)|(Set))
#define RW_PCI32(Bx, Dx, Fx, Rx, Set, Rst) \
          WritePci32(Bx, Dx, Fx, Rx, ReadPci32(Bx, Dx, Fx, Rx) & ~(Rst)|(Set))
#define SET_PCI8(Bx, Dx, Fx, Rx, bSet) \
                WritePci8(Bx, Dx, Fx, Rx, ReadPci8(Bx, Dx, Fx, Rx) | (bSet))
#define SET_PCI16(Bx, Dx, Fx, Rx, wSet) \
                WritePci16(Bx, Dx, Fx, Rx, ReadPci16(Bx, Dx, Fx, Rx) | (wSet))
#define SET_PCI32(Bx, Dx, Fx, Rx, dSet) \
                WritePci32(Bx, Dx, Fx, Rx, ReadPci32(Bx, Dx, Fx, Rx) | (dSet))
#define RESET_PCI8(Bx, Dx, Fx, Rx, bReset) \
               WritePci8(Bx, Dx, Fx, Rx, ReadPci8(Bx, Dx, Fx, Rx) & ~(bReset))
#define RESET_PCI16(Bx, Dx, Fx, Rx, wRst) \
               WritePci16(Bx, Dx, Fx, Rx, ReadPci16(Bx, Dx, Fx, Rx) & ~(wRst))
#define RESET_PCI32(Bx, Dx, Fx, Rx, dRst) \
               WritePci32(Bx, Dx, Fx, Rx, ReadPci32(Bx, Dx, Fx, Rx) & ~(dRst))

//----------------------------------------------------------------------------
// Standard Memory Macros, No Porting Required.
//----------------------------------------------------------------------------

#define READ_MEM8(Addr64)               MMIO_READ8(Addr64)
#define RW_MEM8(Addr64, bSet, bReset)   RwMem8(Addr64, bSet, bReset)

#define READ_MEM32(Addr64)              MMIO_READ32(Addr64)
#define WRITE_MEM32(Addr64, dValue)     WriteMem32(Addr64, dValue)
#define RW_MEM32(Addr64, dSet, dReset)  RwMem32(Addr64, dSet, dReset)

#endif //#ifndef __CSP_LIB__

//---------------------------------------------------------------------------
// Standard I/O Macros, No Porting Required.
//---------------------------------------------------------------------------
#define ReadIo8(IoAddr)           IoRead8(IoAddr)
#define READ_IO8(IoAddr)          IoRead8(IoAddr)
#define WriteIo8(IoAddr, bVal)    IoWrite8(IoAddr, bVal)
#define WRITE_IO8(IoAddr, bVal)   IoWrite8(IoAddr, bVal)
#define SET_IO8(IoAddr, bSet)     IoWrite8(IoAddr, IoRead8(IoAddr) | (bSet))
#define RESET_IO8(IoAddr, bRst)   IoWrite8(IoAddr, IoRead8(IoAddr) & ~(bRst))
#define RW_IO8(Bx, Set, Rst)      IoWrite8(Bx, IoRead8(Bx) & ~(Rst) | (Set))
#define ReadIo16(IoAddr)          IoRead16(IoAddr)
#define READ_IO16(IoAddr)         IoRead16(IoAddr)
#define WriteIo16(IoAddr, wVal)   IoWrite16(IoAddr, wVal)
#define WRITE_IO16(IoAddr, wVal)  IoWrite16(IoAddr, wVal)
#define SET_IO16(IoAddr, wSet)    IoWrite16(IoAddr, IoRead16(IoAddr) | (wSet))
#define RESET_IO16(IoAddr, Rst)   IoWrite16(IoAddr, IoRead16(IoAddr) & ~(Rst))
#define RW_IO16(Bx, Set, Rst)     IoWrite16(Bx, IoRead16(Bx) & ~(Rst) | (Set))
#define ReadIo32(IoAddr)          IoRead32(IoAddr)
#define READ_IO32(IoAddr)         IoRead32(IoAddr)
#define WriteIo32(IoAddr, dVal)   IoWrite32(IoAddr, dVal)
#define WRITE_IO32(IoAddr, dVal)  IoWrite32(IoAddr, dVal)
#define SET_IO32(IoAddr, dSet)    IoWrite32(IoAddr, IoRead32(IoAddr) | (dSet))
#define RESET_IO32(IoAddr, Rst)   IoWrite32(IoAddr, IoRead32(IoAddr) & ~(Rst))
#define RW_IO32(Bx, Set, Rst)     IoWrite32(Bx, IoRead32(Bx) & ~(Rst) | (Set))

//---------------------------------------------------------------------------
// Chipset PCI Macros, Porting Required.
//---------------------------------------------------------------------------

#define READ_PCI8_SB(Rx)          READ_PCI8(SB_BUS, SB_DEV, SB_FUN, Rx)
#define WRITE_PCI8_SB(Rx, Val)    WRITE_PCI8(SB_BUS, SB_DEV, SB_FUN, Rx, Val)
#define SET_PCI8_SB(Rx, Set)      SET_PCI8(SB_BUS, SB_DEV, SB_FUN, Rx, Set)
#define RESET_PCI8_SB(Rx, Rst)    RESET_PCI8(SB_BUS, SB_DEV, SB_FUN, Rx, Rst)
#define RW_PCI8_SB(Rx, St, Rt)    RW_PCI8(SB_BUS, SB_DEV, SB_FUN, Rx, St, Rt)
#define READ_PCI16_SB(Rx)         READ_PCI16(SB_BUS, SB_DEV, SB_FUN, Rx)
#define WRITE_PCI16_SB(Rx, Val)   WRITE_PCI16(SB_BUS, SB_DEV, SB_FUN, Rx, Val)
#define SET_PCI16_SB(Rx, Set)     SET_PCI16(SB_BUS, SB_DEV, SB_FUN, Rx, Set)
#define RESET_PCI16_SB(Rx, Rst)   RESET_PCI16(SB_BUS, SB_DEV, SB_FUN, Rx, Rst)
#define RW_PCI16_SB(Rx, St, Rt)   RW_PCI16(SB_BUS, SB_DEV, SB_FUN, Rx, St, Rt)
#define READ_PCI32_SB(Rx)         READ_PCI32(SB_BUS, SB_DEV, SB_FUN, Rx)
#define WRITE_PCI32_SB(Rx, Val)   WRITE_PCI32(SB_BUS, SB_DEV, SB_FUN, Rx, Val)
#define SET_PCI32_SB(Rx, Set)     SET_PCI32(SB_BUS, SB_DEV, SB_FUN, Rx, Set)
#define RESET_PCI32_SB(Rx, Rst)   RESET_PCI32(SB_BUS, SB_DEV, SB_FUN, Rx, Rst)
#define RW_PCI32_SB(Rx, St, Rt)   RW_PCI32(SB_BUS, SB_DEV, SB_FUN, Rx, St, Rt)

//---------------------------------------------------------------------------
// Chipset I/O Macros, Porting Required.
//---------------------------------------------------------------------------

#define READ_IO8_PM(bReg)           READ_IO8(PM_BASE_ADDRESS+bReg)
#define WRITE_IO8_PM(bReg, bVal)    WRITE_IO8(PM_BASE_ADDRESS+bReg, bVal)
#define SET_IO8_PM(bReg, Set)       SET_IO8(PM_BASE_ADDRESS+bReg, Set)
#define RESET_IO8_PM(bReg, Reset)   RESET_IO8(PM_BASE_ADDRESS+bReg, Reset)
#define RW_IO8_PM(bReg, Set, Rst)   RW_IO8(PM_BASE_ADDRESS+bReg, Set, Rst)
#define READ_IO16_PM(bReg)          READ_IO16(PM_BASE_ADDRESS+bReg)
#define WRITE_IO16_PM(bReg, wVal)   WRITE_IO16(PM_BASE_ADDRESS+bReg, wVal)
#define SET_IO16_PM(bReg, Set)      SET_IO16(PM_BASE_ADDRESS+bReg, Set)
#define RESET_IO16_PM(bReg, Reset)  RESET_IO16(PM_BASE_ADDRESS+bReg, Reset)
#define RW_IO16_PM(bReg, Set, Rst)  RW_IO16(PM_BASE_ADDRESS+bReg, Set, Rst)
#define READ_IO32_PM(bReg)          READ_IO32(PM_BASE_ADDRESS+bReg)
#define WRITE_IO32_PM(bReg, dVal)   WRITE_IO32(PM_BASE_ADDRESS+bReg, dVal)
#define SET_IO32_PM(bReg, Set)      SET_IO32(PM_BASE_ADDRESS+bReg, Set)
#define RESET_IO32_PM(bReg, Reset)  RESET_IO32(PM_BASE_ADDRESS+bReg, Reset)
#define RW_IO32_PM(bReg, Set, Rst)  RW_IO32(PM_BASE_ADDRESS+bReg, Set, Rst)

//---------------------------------------------------------------------------

#define READ_IO8_TCO(bReg)          READ_IO8(TCO_BASE_ADDRESS+bReg)
#define WRITE_IO8_TCO(bReg, bVal)   WRITE_IO8(TCO_BASE_ADDRESS+bReg, bVal)
#define SET_IO8_TCO(bReg, Set)      SET_IO8(TCO_BASE_ADDRESS+bReg, Set)
#define RESET_IO8_TCO(bReg, Reset)  RESET_IO8(TCO_BASE_ADDRESS+bReg, Reset)
#define RW_IO8_TCO(bReg, Set, Rst)  RW_IO8(TCO_BASE_ADDRESS+bReg, Set, Rst)
#define READ_IO16_TCO(bReg)         READ_IO16(TCO_BASE_ADDRESS+bReg)
#define WRITE_IO16_TCO(bReg, wVal)  WRITE_IO16(TCO_BASE_ADDRESS+bReg, wVal)
#define SET_IO16_TCO(bReg, Set)     SET_IO16(TCO_BASE_ADDRESS+bReg, Set)
#define RESET_IO16_TCO(bReg, Reset) RESET_IO16(TCO_BASE_ADDRESS+bReg, Reset)
#define RW_IO16_TCO(bReg, Set, Rst) RW_IO16(TCO_BASE_ADDRESS+bReg, Set, Rst)
#define READ_IO32_TCO(bReg)         READ_IO32(TCO_BASE_ADDRESS+bReg)
#define WRITE_IO32_TCO(bReg, dVal)  WRITE_IO32(TCO_BASE_ADDRESS+bReg, dVal)
#define SET_IO32_TCO(bReg, Set)     SET_IO32(TCO_BASE_ADDRESS+bReg, Set)
#define RESET_IO32_TCO(bReg, Reset) RESET_IO32(TCO_BASE_ADDRESS+bReg, Reset)
#define RW_IO32_TCO(bReg, Set, Rst) RW_IO32(TCO_BASE_ADDRESS+bReg, Set, Rst)

//---------------------------------------------------------------------------

#define READ_IO8_GPIO(bReg)           READ_IO8(GPIO_BASE_ADDRESS+bReg)
#define WRITE_IO8_GPIO(bReg, bVal)    WRITE_IO8(GPIO_BASE_ADDRESS+bReg, bVal)
#define SET_IO8_GPIO(bReg, Set)       SET_IO8(GPIO_BASE_ADDRESS+bReg, Set)
#define RESET_IO8_GPIO(bReg, Reset)   RESET_IO8(GPIO_BASE_ADDRESS+bReg, Reset)
#define RW_IO8_GPIO(bReg, Set, Rst)   RW_IO8(GPIO_BASE_ADDRESS+bReg, Set, Rst)
#define READ_IO16_GPIO(bReg)          READ_IO16(GPIO_BASE_ADDRESS+bReg)
#define WRITE_IO16_GPIO(bReg, wVal)   WRITE_IO16(GPIO_BASE_ADDRESS+bReg, wVal)
#define SET_IO16_GPIO(bReg, Set)      SET_IO16(GPIO_BASE_ADDRESS+bReg, Set)
#define RESET_IO16_GPIO(bReg, Reset)  RESET_IO16(GPIO_BASE_ADDRESS+bReg, Reset)
#define RW_IO16_GPIO(bReg, Set, Rst)  RW_IO16(GPIO_BASE_ADDRESS+bReg, Set, Rst)
#define READ_IO32_GPIO(bReg)          READ_IO32(GPIO_BASE_ADDRESS+bReg)
#define WRITE_IO32_GPIO(bReg, dVal)   WRITE_IO32(GPIO_BASE_ADDRESS+bReg, dVal)
#define SET_IO32_GPIO(bReg, Set)      SET_IO32(GPIO_BASE_ADDRESS+bReg, Set)
#define RESET_IO32_GPIO(bReg, Reset)  RESET_IO32(GPIO_BASE_ADDRESS+bReg, Reset)
#define RW_IO32_GPIO(bReg, Set, Rst)  RW_IO32(GPIO_BASE_ADDRESS+bReg, Set, Rst)


//---------------------------------------------------------------------------
// Chipset MMIO Macros, Porting Required.
//---------------------------------------------------------------------------

#define READ_MEM8_RCRB(wReg)        READ_MEM8(SB_RCBA | wReg)
#define SET_MEM8_RCRB(wReg, Set)    RW_MEM8(SB_RCBA | wReg, Set, 0)
#define RESET_MEM8_RCRB(wReg, Rst)  RW_MEM8(SB_RCBA | wReg,0,Rst)
#define READ_MEM32_RCRB(wReg)       READ_MEM32(SB_RCBA | wReg)
#define WRITE_MEM32_RCRB(wReg,Val)  WRITE_MEM32(SB_RCBA|wReg,Val)
#define SET_MEM32_RCRB(wReg,Set)    RW_MEM32(SB_RCBA|wReg, Set,0)
#define RESET_MEM32_RCRB(wReg,Rst)  RW_MEM32(SB_RCBA|wReg,0,Rst)
#define RW_MEM32_RCRB(wReg,Set,Rst)  RW_MEM32(SB_RCBA|wReg,Set,Rst)

//---------------------------------------------------------------------------

#define RTC_NMI_MASK 0x80
#define RTC_INDEX_REG 0x70
#define RTC_DATA_REG        0x71

#ifdef __cplusplus
}
#endif
#endif
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
