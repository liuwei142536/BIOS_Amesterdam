//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2012, American Megatrends, Inc.            **
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
//*************************************************************************
// $Header:$
//
// $Revision:  $
//
// $Date: $
//*************************************************************************
//<AMI_FHDR_START>
//
// Name:    AmiChipsetIoLib.h
//
// Description: 
//
// Notes:
//
//<AMI_FHDR_END>
//*************************************************************************
#ifndef _AMI_CHIPSET_IO_LIB_H_
#define _AMI_CHIPSET_IO_LIB_H_
#ifdef __cplusplus
extern "C" {
#endif
/*
//-------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Protocol/S3SaveState.h>
#include <token.h>


//-------------------------------------------------------------------------
// Constants, Macros and Type Definitions
//-------------------------------------------------------------------------
#ifdef PCIEX_BASE_ADDRESS
#define CSP_PCIE_CFG_ADDRESS(bus, dev, func, reg) \
            ((UINTN)(PCIEX_BASE_ADDRESS + ((UINT8)(bus) << 20) + \
            ((UINT8)(dev) << 15) + ((UINT8)(func) << 12) + (reg)))
#else
#define CSP_PCIE_CFG_ADDRESS(Bus, Device, Function, Register) \
    ( \
      (UINTN) PcdGet64 (PcdPciExpressBaseAddress) + (UINTN) (Bus << 20) + (UINTN) (Device << 15) + (UINTN) \
        (Function << 12) + (Register) \
    )
#endif

#define CSP_PCI_CFG_ADDRESS(bus,dev,func,reg)   \
 ((UINT64) ( (((UINTN)bus) <<   24) +   (((UINTN)dev)   << 16) + (((UINTN)func) << 8)   +   ((UINTN)reg) ))& 0x00000000ffffffff

#ifndef AMI_S3_SAVE_PROTOCOL
#define AMI_S3_SAVE_PROTOCOL      EFI_S3_SAVE_STATE_PROTOCOL
#define AMI_S3_SAVE_PROTOCOL_GUID &gEfiS3SaveStateProtocolGuid
#endif

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

#define WRITE_IO8_S3(mBtScSv, IoAddr16, bValue) \
                                    WriteIo8S3(mBtScSv, IoAddr16, bValue)
#define SET_IO8_S3(mBtScSv, IoAddr16, bSet) \
                                    RwIo8S3(mBtScSv, IoAddr16, bSet, 0)
#define RESET_IO8_S3(mBtScSv, IoAddr16, bReset) \
                                    RwIo8S3(mBtScSv, IoAddr16, 0, bReset)
#define RW_IO8_S3(mBtScSv, IoAddr16, bSet, bReset) \
                                    RwIo8S3(mBtScSv, IoAddr16, bSet, bReset)
#define WRITE_IO16_S3(mBtScSv, IoAddr16, wValue) \
                                    WriteIo16S3(mBtScSv, IoAddr16, wValue)
#define SET_IO16_S3(mBtScSv, IoAddr16, wSet) \
                                    RwIo16S3(mBtScSv, IoAddr16, wSet, 0)
#define RESET_IO16_S3(mBtScSv, IoAddr16, wReset) \
                                    RwIo16S3(mBtScSv, IoAddr16, 0, wReset)
#define RW_IO16_S3(mBtScSv, IoAddr16, wSet, wReset) \
                                    RwIo16S3(mBtScSv, IoAddr16, wSet, wReset)
#define WRITE_IO32_S3(mBtScSv, IoAddr16, dValue) \
                                    WriteIo32S3(mBtScSv, IoAddr16, dValue)
#define SET_IO32_S3(mBtScSv, IoAddr16, dSet) \
                                    RwIo32S3(mBtScSv, IoAddr16, dSet, 0)
#define RESET_IO32_S3(mBtScSv, IoAddr16, dReset) \
                                    RwIo32S3(mBtScSv, IoAddr16, 0, dReset)
#define RW_IO32_S3(mBtScSv, IoAddr16, dSet, dReset) \
                                    RwIo32S3(mBtScSv, IoAddr16, dSet, dReset)

//----------------------------------------------------------------------------
// Standard Memory Macros, No Porting Required.
//----------------------------------------------------------------------------

#define READ_MEM8(Addr64)               MmioRead8(Addr64)
#define MEM_READ8(Addr64)               MmioRead8(Addr64)
#define READ_MMIO8(Addr64)              MmioRead8(Addr64)
#define WRITE_MEM8(Addr64, bValue)      MmioWrite8(Addr64, bValue)
#define MEM_WRITE8(Addr64, bValue)      MmioWrite8(Addr64, bValue)
#define WRITE_MMIO8(Addr64, bValue)     MmioWrite8(Addr64, bValue)
#define SET_MEM8(Addr64, bSet)          MmioOr8(Addr64, bSet)
#define MEM_SET8(Addr64, bSet)          MmioOr8(Addr64, bSet)
#define SET_MMIO8(Addr64, bSet)         MmioOr8(Addr64, bSet)
#define MMIO_SET8(Addr64, bSet)         MmioOr8(Addr64, bSet)
#define RESET_MEM8(Addr64, bReset)      MmioAnd8(Addr64, ~bReset)
#define MEM_RESET8(Addr64, bReset)      MmioAnd8(Addr64, ~bReset)
#define RESET_MMIO8(Addr64, bReset)     MmioAnd8(Addr64, ~bReset)
#define MMIO_RESET8(Addr64, bReset)     MmioAnd8(Addr64, ~bReset)
#define RW_MEM8(Addr64, bSet, bReset)   MmioAndThenOr8(Addr64, ~bReset, bSet)
#define MEM_RW8(Addr64, bSet, bReset)   MmioAndThenOr8(Addr64, ~bReset, bSet)
#define RW_MMIO8(Addr64, bSet, bReset)  MmioAndThenOr8(Addr64, ~bReset, bSet)
#define MMIO_RW8(Addr64, bSet, bReset)  MmioAndThenOr8(Addr64, ~bReset, bSet)

#define READ_MEM16(Addr64)              MmioRead16(Addr64)
#define MEM_READ16(Addr64)              MmioRead16(Addr64)
#define READ_MMIO16(Addr64)             MmioRead16(Addr64)
#define WRITE_MEM16(Addr64, wValue)     MmioWrite16(Addr64, wValue)
#define MEM_WRITE16(Addr64, wValue)     MmioWrite16(Addr64, wValue)
#define WRITE_MMIO16(Addr64, wValue)    MmioWrite16(Addr64, wValue)
#define SET_MEM16(Addr64, wSet)         MmioOr16(Addr64, wSet)
#define MEM_SET16(Addr64, wSet)         MmioOr16(Addr64, wSet)
#define SET_MMIO16(Addr64, wSet)        MmioOr16(Addr64, wSet)
#define MMIO_SET16(Addr64, wSet)        MmioOr16(Addr64, wSet)
#define RESET_MEM16(Addr64, wReset)     MmioAnd16(Addr64, ~wReset)
#define MEM_RESET16(Addr64, wReset)     MmioAnd16(Addr64, ~wReset)
#define RESET_MMIO16(Addr64, wReset)    MmioAnd16(Addr64, ~wReset)
#define MMIO_RESET16(Addr64, wReset)    MmioAnd16(Addr64, ~wReset)
#define RW_MEM16(Addr64, wSet, wReset)  MmioAndThenOr16(Addr64, ~wReset, wSet)
#define MEM_RW16(Addr64, wSet, wReset)  MmioAndThenOr16(Addr64, ~wReset, wSet)
#define RW_MMIO16(Addr64, wSet, wReset) MmioAndThenOr16(Addr64, ~wReset, wSet)
#define MMIO_RW16(Addr64, wSet, wReset) MmioAndThenOr16(Addr64, ~wReset, wSet)

#define READ_MEM32(Addr64)              MmioRead32(Addr64)
#define MEM_READ32(Addr64)              MmioRead32(Addr64)
#define READ_MMIO32(Addr64)             MmioRead32(Addr64)
#define WRITE_MEM32(Addr64, dValue)     MmioWrite32(Addr64, dValue)
#define MEM_WRITE32(Addr64, dValue)     MmioWrite32(Addr64, dValue)
#define WRITE_MMIO32(Addr64, dValue)    MmioWrite32(Addr64, dValue)
#define SET_MEM32(Addr64, dSet)         MmioOr32(Addr64, dSet)
#define MEM_SET32(Addr64, dSet)         MmioOr32(Addr64, dSet)
#define SET_MMIO32(Addr64, dSet)        MmioOr32(Addr64, dSet)
#define MMIO_SET32(Addr64, dSet)        MmioOr32(Addr64, dSet)
#define RESET_MEM32(Addr64, dReset)     MmioAnd32(Addr64, ~dReset)
#define MEM_RESET32(Addr64, dReset)     MmioAnd32(Addr64, ~dReset)
#define RESET_MMIO32(Addr64, dReset)    MmioAnd32(Addr64, ~dReset)
#define MMIO_RESET32(Addr64, dReset)    MmioAnd32(Addr64, ~dReset)
#define RW_MEM32(Addr64, dSet, dReset)  MmioAndThenOr32(Addr64, ~dReset, dSet)
#define MEM_RW32(Addr64, dSet, dReset)  MmioAndThenOr32(Addr64, ~dReset, dSet)
#define RW_MMIO32(Addr64, dSet, dReset) MmioAndThenOr32(Addr64, ~dReset, dSet)
#define MMIO_RW32(Addr64, dSet, dReset) MmioAndThenOr32(Addr64, ~dReset, dSet)

//----------------------------------------------------------------------------

#define WRITE_MEM8_S3(mBtScSv, Addr64, bValue) \
                                    WriteMem8S3(mBtScSv, Addr64, bValue)
#define MEM_WRITE8_S3(mBtScSv, Addr64, bValue) \
                                    WriteMem8S3(mBtScSv, Addr64, bValue)
#define WRITE_MMIO8_S3(mBtScSv, Addr64, bValue) \
                                    WriteMem8S3(mBtScSv, Addr64, bValue)
#define MMIO_WRITE8_S3(mBtScSv, Addr64, bValue) \
                                    WriteMem8S3(mBtScSv, Addr64, bValue)
#define SET_MEM8_S3(mBtScSv, Addr64, bSet) \
                                    RwMem8S3(mBtScSv, Addr64, bSet, 0)
#define MEM_SET8_S3(mBtScSv, Addr64, bSet) \
                                    RwMem8S3(mBtScSv, Addr64, bSet, 0)
#define SET_MMIO8_S3(mBtScSv, Addr64, bSet) \
                                    RwMem8S3(mBtScSv, Addr64, bSet, 0)
#define MMIO_SET8_S3(mBtScSv, Addr64, bSet) \
                                    RwMem8S3(mBtScSv, Addr64, bSet, 0)
#define RESET_MEM8_S3(mBtScSv, Addr64, bReset) \
                                    RwMem8S3(mBtScSv, Addr64, 0, bReset)
#define MEM_RESET8_S3(mBtScSv, Addr64, bReset) \
                                    RwMem8S3(mBtScSv, Addr64, 0, bReset)
#define RESET_MMIO8_S3(mBtScSv, Addr64, bReset) \
                                    RwMem8S3(mBtScSv, Addr64, 0, bReset)
#define MMIO_RESET8_S3(mBtScSv, Addr64, bReset) \
                                    RwMem8S3(mBtScSv, Addr64, 0, bReset)
#define RW_MEM8_S3(mBtScSv, Addr64, bSet, bReset) \
                                    RwMem8S3(mBtScSv, Addr64, bSet, bReset)
#define MEM_RW8_S3(mBtScSv, Addr64, bSet, bReset) \
                                    RwMem8S3(mBtScSv, Addr64, bSet, bReset)
#define RW_MMIO8_S3(mBtScSv, Addr64, bSet, bReset) \
                                    RwMem8S3(mBtScSv, Addr64, bSet, bReset)
#define MMIO_RW8_S3(mBtScSv, Addr64, bSet, bReset) \
                                    RwMem8S3(mBtScSv, Addr64, bSet, bReset)
#define WRITE_MEM16_S3(mBtScSv, Addr64, wValue) \
                                    WriteMem16S3(mBtScSv, Addr64, wValue)
#define MEM_WRITE16_S3(mBtScSv, Addr64, wValue) \
                                    WriteMem16S3(mBtScSv, Addr64, wValue)
#define WRITE_MMIO16_S3(mBtScSv, Addr64, wValue) \
                                    WriteMem16S3(mBtScSv, Addr64, wValue)
#define MMIO_WRITE16_S3(mBtScSv, Addr64, wValue) \
                                    WriteMem16S3(mBtScSv, Addr64, wValue)
#define SET_MEM16_S3(mBtScSv, Addr64, wSet) \
                                    RwMem16S3(mBtScSv, Addr64, wSet, 0)
#define MEM_SET16_S3(mBtScSv, Addr64, wSet) \
                                    RwMem16S3(mBtScSv, Addr64, wSet, 0)
#define SET_MMIO16_S3(mBtScSv, Addr64, wSet) \
                                    RwMem16S3(mBtScSv, Addr64, wSet, 0)
#define MMIO_SET16_S3(mBtScSv, Addr64, wSet) \
                                    RwMem16S3(mBtScSv, Addr64, wSet, 0)
#define RESET_MEM16_S3(mBtScSv, Addr64, wReset) \
                                    RwMem16S3(mBtScSv, Addr64, 0, wReset)
#define MEM_RESET16_S3(mBtScSv, Addr64, wReset) \
                                    RwMem16S3(mBtScSv, Addr64, 0, wReset)
#define RESET_MMIO16_S3(mBtScSv, Addr64, wReset) \
                                    RwMem16S3(mBtScSv, Addr64, 0, wReset)
#define MMIO_RESET16_S3(mBtScSv, Addr64, wReset) \
                                    RwMem16S3(mBtScSv, Addr64, 0, wReset)
#define RW_MEM16_S3(mBtScSv, Addr64, wSet, wReset) \
                                    RwMem16S3(mBtScSv, Addr64, wSet, wReset)
#define MEM_RW16_S3(mBtScSv, Addr64, wSet, wReset) \
                                    RwMem16S3(mBtScSv, Addr64, wSet, wReset)
#define RW_MMIO16_S3(mBtScSv, Addr64, wSet, wReset) \
                                    RwMem16S3(mBtScSv, Addr64, wSet, wReset)
#define MMIO_RW16_S3(mBtScSv, Addr64, wSet, wReset) \
                                    RwMem16S3(mBtScSv, Addr64, wSet, wReset)
#define WRITE_MEM32_S3(mBtScSv, Addr64, dValue) \
                                    WriteMem32S3(mBtScSv, Addr64, dValue)
#define MEM_WRITE32_S3(mBtScSv, Addr64, dValue) \
                                    WriteMem32S3(mBtScSv, Addr64, dValue)
#define WRITE_MMIO32_S3(mBtScSv, Addr64, dValue) \
                                    WriteMem32S3(mBtScSv, Addr64, dValue)
#define MMIO_WRITE32_S3(mBtScSv, Addr64, dValue) \
                                    WriteMem32S3(mBtScSv, Addr64, dValue)
#define SET_MEM32_S3(mBtScSv, Addr64, dSet) \
                                    RwMem32S3(mBtScSv, Addr64, dSet, 0)
#define MEM_SET32_S3(mBtScSv, Addr64, dSet) \
                                    RwMem32S3(mBtScSv, Addr64, dSet, 0)
#define SET_MMIO32_S3(mBtScSv, Addr64, dSet) \
                                    RwMem32S3(mBtScSv, Addr64, dSet, 0)
#define MMIO_SET32_S3(mBtScSv, Addr64, dSet) \
                                    RwMem32S3(mBtScSv, Addr64, dSet, 0)
#define RESET_MEM32_S3(mBtScSv, Addr64, dReset) \
                                    RwMem32S3(mBtScSv, Addr64, 0, dReset)
#define MEM_RESET32_S3(mBtScSv, Addr64, dReset) \
                                    RwMem32S3(mBtScSv, Addr64, 0, dReset)
#define RESET_MMIO32_S3(mBtScSv, Addr64, dReset) \
                                    RwMem32S3(mBtScSv, Addr64, 0, dReset)
#define MMIO_RESET32_S3(mBtScSv, Addr64, dReset) \
                                    RwMem32S3(mBtScSv, Addr64, 0, dReset)
#define RW_MEM32_S3(mBtScSv, Addr64, dSet, dReset) \
                                    RwMem32S3(mBtScSv, Addr64, dSet, dReset)
#define MEM_RW32_S3(mBtScSv, Addr64, dSet, dReset) \
                                    RwMem32S3(mBtScSv, Addr64, dSet, dReset)
#define RW_MMIO32_S3(mBtScSv, Addr64, dSet, dReset) \
                                    RwMem32S3(mBtScSv, Addr64, dSet, dReset)
#define MMIO_RW32_S3(mBtScSv, Addr64, dSet, dReset) \
                                    RwMem32S3(mBtScSv, Addr64, dSet, dReset)

//----------------------------------------------------------------------------
// Standard PCI Macros, No Porting Required.
//----------------------------------------------------------------------------

#define READ_PCI8(Bx, Dx, Fx, Rx)           PciRead8 (PCI_LIB_ADDRESS(Bx, Dx, Fx, Rx))
#define READ_PCI16(Bx, Dx, Fx, Rx)          PciRead16 (PCI_LIB_ADDRESS(Bx, Dx, Fx, Rx))
#define READ_PCI32(Bx, Dx, Fx, Rx)          PciRead32 (PCI_LIB_ADDRESS(Bx, Dx, Fx, Rx))
#define WRITE_PCI8(Bx, Dx, Fx, Rx, bVal)    PciWrite8 (PCI_LIB_ADDRESS(Bx, Dx, Fx, Rx), bVal)
#define WRITE_PCI16(Bx, Dx, Fx, Rx, wVal)   PciWrite16 (PCI_LIB_ADDRESS(Bx, Dx, Fx, Rx), wVal)
#define WRITE_PCI32(Bx, Dx, Fx, Rx, dVal)   PciWrite32 (PCI_LIB_ADDRESS(Bx, Dx, Fx, Rx), dVal)
#define RW_PCI8(Bx, Dx, Fx, Rx, Set, Rst) \
          PciAndThenOr8 (PCI_LIB_ADDRESS(Bx, Dx, Fx, Rx), ~Rst, Set)
#define RW_PCI16(Bx, Dx, Fx, Rx, Set, Rst) \
          PciAndThenOr16 (PCI_LIB_ADDRESS(Bx, Dx, Fx, Rx), ~Rst, Set)
#define RW_PCI32(Bx, Dx, Fx, Rx, Set, Rst) \
          PciAndThenOr32 (PCI_LIB_ADDRESS(Bx, Dx, Fx, Rx), ~Rst, Set)
#define SET_PCI8(Bx, Dx, Fx, Rx, bSet)      PciOr8 (PCI_LIB_ADDRESS(Bx, Dx, Fx, Rx), bSet)
#define SET_PCI16(Bx, Dx, Fx, Rx, wSet)     PciOr16 (PCI_LIB_ADDRESS(Bx, Dx, Fx, Rx), wSet)
#define SET_PCI32(Bx, Dx, Fx, Rx, dSet)     PciOr32 (PCI_LIB_ADDRESS(Bx, Dx, Fx, Rx), dSet)
#define RESET_PCI8(Bx, Dx, Fx, Rx, bRst)    PciAnd8 (PCI_LIB_ADDRESS(Bx, Dx, Fx, Rx), ~(bRst))
#define RESET_PCI16(Bx, Dx, Fx, Rx, wRst)   PciAnd16 (PCI_LIB_ADDRESS(Bx, Dx, Fx, Rx), ~(wRst))
#define RESET_PCI32(Bx, Dx, Fx, Rx, dRst)   PciAnd32 (PCI_LIB_ADDRESS(Bx, Dx, Fx, Rx), ~(dRst))

#define WRITE_PCI8_S3(mBtScSv, Bx, Dx, Fx, Rx, bValue) \
                                WritePci8S3(mBtScSv, Bx, Dx, Fx, Rx, bValue)
#define SET_PCI8_S3(mBtScSv, Bx, Dx, Fx, Rx, bSet) \
                                RwPci8S3(mBtScSv, Bx, Dx, Fx, Rx, bSet, 0)
#define RESET_PCI8_S3(mBtScSv, Bx, Dx, Fx, Rx, bReset) \
                                RwPci8S3(mBtScSv, Bx, Dx, Fx, Rx, 0, bReset)
#define RW_PCI8_S3(mBtScSv, Bx, Dx, Fx, Rx, bSet, bRst) \
                                RwPci8S3(mBtScSv, Bx, Dx, Fx, Rx, bSet, bRst)
#define WRITE_PCI16_S3(mBtScSv, Bx, Dx, Fx, Rx, wValue) \
                                WritePci16S3(mBtScSv, Bx, Dx, Fx, Rx, wValue)
#define SET_PCI16_S3(mBtScSv, Bx, Dx, Fx, Rx, wSet) \
                                RwPci16S3(mBtScSv, Bx, Dx, Fx, Rx, wSet, 0)
#define RESET_PCI16_S3(mBtScSv, Bx, Dx, Fx, Rx, wReset) \
                                RwPci16S3(mBtScSv, Bx, Dx, Fx, Rx, 0, wReset)
#define RW_PCI16_S3(mBtScSv, Bx, Dx, Fx, Rx, wSet, wRst) \
                                RwPci16S3(mBtScSv, Bx, Dx, Fx, Rx, wSet, wRst)
#define WRITE_PCI32_S3(mBtScSv, Bx, Dx, Fx, Rx, dValue) \
                                WritePci32S3(mBtScSv, Bx, Dx, Fx, Rx, dValue)
#define SET_PCI32_S3(mBtScSv, Bx, Dx, Fx, Rx, dSet) \
                                RwPci32S3(mBtScSv, Bx, Dx, Fx, Rx, dSet, 0)
#define RESET_PCI32_S3(mBtScSv, Bx, Dx, Fx, Rx, dReset) \
                                RwPci32S3(mBtScSv, Bx, Dx, Fx, Rx, 0, dReset)
#define RW_PCI32_S3(mBtScSv, Bx, Dx, Fx, Rx, dSet, dRst) \
                                RwPci32S3(mBtScSv, Bx, Dx, Fx, Rx, dSet, dRst)

//-------------------------------------------------------------------------
// Variable, Prototype, and External Declarations
//-------------------------------------------------------------------------
UINT8
FindCapPtr (
    IN UINTN        PciAddress,
    IN UINT8        CapId
);

UINT16
FindExtendedCapPtr (
    IN UINTN        PciAddress,
    IN UINT16       CapId
);

UINT8 ReadIo8IdxData (
    IN UINT16       IoBase16,
    IN UINT8        RegIdx8
);

VOID WriteIo8IdxData (
    IN UINT16       IoBase16,
    IN UINT8        RegIdx8,
    IN UINT8        WriteValue8
);

VOID RwIo8IdxData (
    IN UINT16       IoBase16,
    IN UINT8        RegIdx8,
    IN UINT8        SetBit8,
    IN UINT8        ResetBit8
);

VOID WriteIo8S3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT16                           IoBaseReg16,
    IN UINT8                            WriteValue8
);

VOID WriteIo16S3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT16                           IoBaseReg16,
    IN UINT16                           WriteValue16
);

VOID WriteIo32S3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT16                           IoBaseReg16,
    IN UINT32                           WriteValue32
);

VOID RwIo8S3(
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT16                           IoBaseReg16,
    IN UINT8                            SetBit8,
    IN UINT8                            ResetBit8
);

VOID RwIo16S3(
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT16                           IoBaseReg16,
    IN UINT16                           SetBit16,
    IN UINT16                           ResetBit16
);

VOID RwIo32S3(
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT16                           IoBaseReg16,
    IN UINT32                           SetBit32,
    IN UINT32                           ResetBit32
);

VOID WriteIo8IdxDataS3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT16                           IoBase16,
    IN UINT8                            IoReg8,
    IN UINT8                            Value8 );

VOID RwIo8IdxDataS3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT16                           IoBase16,
    IN UINT8                            IoReg8,
    IN UINT8                            SetBit8,
    IN UINT8                            ResetBit8 );

VOID
WritePci8S3(
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT8                            Bus,
    IN UINT8                            Dev,
    IN UINT8                            Fun,
    IN UINT16                           Reg,
    IN UINT8                            WriteValue8
);

VOID
WritePci16S3(
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT8                            Bus,
    IN UINT8                            Dev,
    IN UINT8                            Fun,
    IN UINT16                           Reg,
    IN UINT16                           WriteValue16
);

VOID
WritePci32S3(
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT8                            Bus,
    IN UINT8                            Dev,
    IN UINT8                            Fun,
    IN UINT16                           Reg,
    IN UINT32                           WriteValue32
);

VOID
RwPci8S3(
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT8                            Bus,
    IN UINT8                            Dev,
    IN UINT8                            Fun,
    IN UINT16                           Reg,
    IN UINT8                            SetBit8,
    IN UINT8                            ResetBit8
);

VOID
RwPci16S3(
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT8                            Bus,
    IN UINT8                            Dev,
    IN UINT8                            Fun,
    IN UINT16                           Reg,
    IN UINT16                           SetBit16,
    IN UINT16                           ResetBit16
);

VOID
RwPci32S3(
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT8                            Bus,
    IN UINT8                            Dev,
    IN UINT8                            Fun,
    IN UINT16                           Reg,
    IN UINT32                           SetBit32,
    IN UINT32                           ResetBit32
);

VOID
WriteMem8S3(
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT64                           Address,
    IN UINT8                            Value
);

VOID
WriteMem16S3(
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT64                           Address,
    IN UINT16                           Value
);

VOID
WriteMem32S3(
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT64                           Address,
    IN UINT32                           Value
);

VOID
RwMem8S3(
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT64                           Address,
    IN UINT8                            SetBit8,
    IN UINT8                            ResetBit8
);

VOID
RwMem16S3(
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT64                           Address,
    IN UINT16                           SetBit16,
    IN UINT16                           ResetBit16
);

VOID
RwMem32S3(
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT64                           Address,
    IN UINT32                           SetBit32,
    IN UINT32                           ResetBit32
);
//*/
/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif //_AMI_CHIPSET_IO_LIB_H_
//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2012, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

