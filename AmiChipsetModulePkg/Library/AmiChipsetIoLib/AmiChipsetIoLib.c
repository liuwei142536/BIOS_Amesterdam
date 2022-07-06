//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2012, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

//**********************************************************************
// $Header: $
//
// $Revision: $
//
// $Date: $
//**********************************************************************
//<AMI_FHDR_START>
//
// Name:	AmiChipsetIoLib.c
//
// Description:	
//  This file contains implementation of Ami Chipset Io Library
//  Mark all avoiding redefine and build error
//<AMI_FHDR_END>
//**********************************************************************
/*
//======================================================================
// Module specific Includes
#include <AmiChipsetIoLib.h>
#include <Uefi/UefiMultiPhase.h>
#include <Pi/PiMultiPhase.h>
#include <AmiDxeLib.h>
#include <IndustryStandard/Pci22.h>


//======================================================================
// Produced Protocols


//======================================================================
// Variable Declaration


//======================================================================
// GUID Definitions


//======================================================================
// Function Declarations


//======================================================================
// Function Definitions

//----------------------------------------------------------------------

//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure: FindCapPtr
//
// Description: This function searches the PCI address space for the PCI
//              device specified for a particular capability ID and
//              returns the Capability ID Offset if one found
//
// Input:       Bus       Pci Bus Number
//              Device    Pci Device Number
//              Function  Pci Function Number
//              CapId     CAPID to search for
//                        CAPID list:
//                        0x01 = PCI Power Management Interface
//                        0x10 = PCI Express Capability
//
// Output:      Capability ID Offset if one found
//              Otherwise returns 0
//----------------------------------------------------------------------
//<AMI_PHDR_END>
UINT8
FindCapPtr (
    IN UINTN        PciAddress,
    IN UINT8        CapId
  )
{
  UINT8     StatusReg;
  UINT8     HeaderTypeReg;
  UINT8     NextCapReg;
  UINT8     GetCapId;

  //
  // Return 0 if the device does not exist or does not have
  // the capabilities list
  //
  StatusReg = MmioRead8 (PciAddress + PCI_PRIMARY_STATUS_OFFSET);

  if ((StatusReg == 0xFF) || ((StatusReg & BIT4) == 0)) {
    return 0;
  }

  //
  // Assign Capabilities List Pointer
  // For CardBus bridge, it should be 0x14. Otherwise, it should be 0x34
  //
  HeaderTypeReg = MmioRead8 (PciAddress + PCI_HEADER_TYPE_OFFSET) & HEADER_LAYOUT_CODE;

  if (HeaderTypeReg == HEADER_TYPE_CARDBUS_BRIDGE) {
    NextCapReg = 0x14;
  } else {
    NextCapReg = 0x34;
  }

  //
  // Search for the matched Cap ID
  //
  for(;;)
  {
    NextCapReg = MmioRead8 (PciAddress + NextCapReg);

    if (NextCapReg == 0) {
      return 0;
    }

    GetCapId = MmioRead8 (PciAddress + NextCapReg);
    if (GetCapId == CapId) {
      return NextCapReg;
    }

    NextCapReg ++;
  }
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure: FindExtendedCapPtr
//
// Description: This function Search and return the offset of desired
//              Pci Express Capability ID
//
// Input:       Bus       Pci Bus Number
//              Device    Pci Device Number
//              Function  Pci Function Number
//              CapId     CAPID to search for
//                        CAPID list:
//                        0x0001 = Advanced Error Rreporting Capability
//                        0x0002 = Virtual Channel Capability
//                        0x0003 = Device Serial Number Capability
//                        0x0004 = Power Budgeting Capability
//
// Output:      Extended Capability ID Offset if one found
//              Otherwise returns 0
//----------------------------------------------------------------------
//<AMI_PHDR_END>
UINT16
FindExtendedCapPtr (
    IN UINTN        PciAddress,
    IN UINT16       CapId
  )
{
  UINT16    NextCapReg;
  UINT16    GetCapId;

  NextCapReg = 0x100;

  //
  // Return 0 if the device does not exist
  //
  if (MmioRead8 (PciAddress) == 0xFF) {
    return 0;
  }

  for(;;)
  {
    GetCapId = MmioRead16 (PciAddress + NextCapReg);

    if (GetCapId == CapId) {
      return NextCapReg;
    }

    NextCapReg = MmioRead16 (PciAddress + NextCapReg + 2);

    NextCapReg >>= 4;

    if (NextCapReg == 0) {
      return 0;
    }
  }
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   WriteIo8S3
//
// Description: This function writes an 8bit value to a specific I/O port
//              and writes a copy to Boot Script Table.
//
// Input:       *mBootScriptSave - Pointer to Boot Script Save Protocal
//              IoBaseReg16      - A 16 Bit I/O Port Address
//              Value8           - An 8 Bit Value to write.
//
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID WriteIo8S3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT16                           IoBaseReg16,
    IN UINT8                            Value8 )
{
    IoWrite8 ( IoBaseReg16, Value8 );

    BOOT_SCRIPT_S3_IO_WRITE_MACRO( mBootScriptSave, \
                                   EfiBootScriptWidthUint8, \
                                   IoBaseReg16, \
                                   1, \
                                   &Value8 );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   WriteIo16S3
//
// Description: This function writes a 16bit value to a specific I/O port
//              and writes a copy to Boot Script Table.
//
// Input:       *mBootScriptSave - Pointer to Boot Script Save Protocal
//              IoBaseReg16      - A 16 Bit I/O Port Address
//              Value16          - A 16 Bit Value to write.
//
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID WriteIo16S3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT16                           IoBaseReg16,
    IN UINT16                           Value16 )
{
    IoWrite16 ( IoBaseReg16, Value16 );

    BOOT_SCRIPT_S3_IO_WRITE_MACRO( mBootScriptSave, \
                                   EfiBootScriptWidthUint16,\
                                   IoBaseReg16, \
                                   1, \
                                   &Value16 );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   WriteIo32S3
//
// Description: This function writes a 32bit value to a specific I/O port
//              and writes a copy to Boot Script Table.
//
// Input:       *mBootScriptSave - Pointer to Boot Script Save Protocal
//              IoBaseReg16      - A 16 Bit I/O Port Address
//              Value32          - a 32 Bit Value to write.
//
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID WriteIo32S3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT16                           IoBaseReg16,
    IN UINT32                           Value32 )
{
    IoWrite32 ( IoBaseReg16, Value32 );

    BOOT_SCRIPT_S3_IO_WRITE_MACRO( mBootScriptSave, \
                                   EfiBootScriptWidthUint32,\
                                   IoBaseReg16, \
                                   1, \
                                   &Value32 );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   RwIo8S3
//
// Description: This function reads an 8bit value from a specific I/O port, 
//              then applies Set/Reset masks, and writes it back, then
//              writes a copy to Boot Script Table.
//
// Input:       *mBootScriptSave - Pointer to Boot Script Save Protocal
//              IoBaseReg16      - A 16 Bit I/O Port Address
//              SetBit8          - Mask of 8bits to set (1 = Set)
//              ResetBit8        - Mask of 8bits to reset (1 = Reset)
//
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID RwIo8S3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT16                           IoBaseReg16,
    IN UINT8                            SetBit8,
    IN UINT8                            ResetBit8 )
{   

    RW_IO8 ( IoBaseReg16, SetBit8, ResetBit8 );

    ResetBit8 = ~ResetBit8;

    BOOT_SCRIPT_S3_IO_READ_WRITE_MACRO( mBootScriptSave, \
                                        EfiBootScriptWidthUint8, \
                                        IoBaseReg16, \
                                        &SetBit8, \
                                        &ResetBit8 );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   RwIo16S3
//
// Description: This function reads a 16bit value from a specific I/O port, 
//              then applies Set/Reset masks, and writes it back, then
//              writes a copy to Boot Script Table.
//
// Input:       *mBootScriptSave - Pointer to Boot Script Save Protocal
//              IoBaseReg16      - A 16 Bit I/O Port Address
//              SetBit16         - Mask of 16bits to set (1 = Set)
//              ResetBit16       - Mask of 16bits to reset (1 = Reset)
//
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID RwIo16S3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT16                           IoBaseReg16,
    IN UINT16                           SetBit16,
    IN UINT16                           ResetBit16 )
{   
    RW_IO16 ( IoBaseReg16, SetBit16, ResetBit16 );

    ResetBit16 = ~ResetBit16;

    BOOT_SCRIPT_S3_IO_READ_WRITE_MACRO( mBootScriptSave, \
                                        EfiBootScriptWidthUint16, \
                                        IoBaseReg16, \
                                        &SetBit16, \
                                        &ResetBit16 );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   RwIo32S3
//
// Description: This function reads a 32bit value from a specific I/O port, 
//              then applies Set/Reset masks, and writes it back, then
//              writes a copy to Boot Script Table.
//
// Input:       *mBootScriptSave - Pointer to Boot Script Save Protocal
//              IoBaseReg16      - A 16 Bit I/O Port Address
//              SetBit32         - Mask of 32bits to set (1 = Set)
//              ResetBit32       - Mask of 32bits to reset (1 = Reset)
//
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID RwIo32S3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT16                           IoBaseReg16,
    IN UINT32                           SetBit32,
    IN UINT32                           ResetBit32 )
{   
    RW_IO32 ( IoBaseReg16, SetBit32, ResetBit32 );

    ResetBit32 = ~ResetBit32;

    BOOT_SCRIPT_S3_IO_READ_WRITE_MACRO( mBootScriptSave, \
                                        EfiBootScriptWidthUint32, \
                                        IoBaseReg16, \
                                        &SetBit32, \
                                        &ResetBit32 );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   WriteIo8IdxDataS3
//
// Description: This function writes an 8bit value to a specific I/O
//              Index/Data ports and writes a copy to Boot Script Table.
//
// Input:       *mBootScriptSave - Pointer to Boot Script Save Protocal
//              IoBase16         - A 16 Bit I/O Address for Index I/O Port 
//              RegIdx8          - An 8 Bit Register Index
//              Value8           - An 8 Bit Value to write.
//
// Output:      None
//
// Notes:       The default Data I/O Port is the Index I/O Port plus 1, if
//              your Data I/O Port is not that, please modify below 
//              "IoBase16+1".
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID WriteIo8IdxDataS3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT16                           IoBase16,
    IN UINT8                            IoReg8,
    IN UINT8                            Value8 )
{
    WriteIo8S3 (mBootScriptSave, IoBase16, IoReg8);
    WriteIo8S3 (mBootScriptSave, IoBase16 + 1, Value8);
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   ReadIo8IdxData
//
// Description: This function reads an 8bit value from a specific I/O
//              Index/Data port.
//
// Input:       IoBase16 - A 16 Bit I/O Address for Index I/O Port
//              RegIdx8  - An 8 Bit Register offset
//
// Output:      An 8Bit data read from the specific Index/Data I/O port.
//
// Notes:       The default Data I/O Port is the Index I/O Port plus 1, if
//              your Data I/O Port is not that, please modify below
//              "++IoBase16".
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

UINT8 ReadIo8IdxData (
    IN UINT16           IoBase16,
    IN UINT8            RegIdx8 )
{
    IoWrite8 ( IoBase16, RegIdx8 );
    return IoRead8 ( ++IoBase16 );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   WriteIo8IdxData
//
// Description: This function writes an 8bit value to a specific I/O
//              Index/Data port.
//
// Input:       IoBase16 - A 16 Bit I/O Address for Index I/O Port
//              RegIdx8  - An 8 Bit Register Index
//              Value8   - An 8 Bit Value to write.
//
// Output:      None
//
// Notes:       The default Data I/O Port is the Index I/O Port plus 1, if
//              your Data I/O Port is not that, please modify below
//              "++IoBase16".
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID WriteIo8IdxData (
    IN UINT16       IoBase16,
    IN UINT8        RegIdx8,
    IN UINT8        Value8 )
{
    IoWrite8 ( IoBase16, RegIdx8 );
    IoWrite8 ( ++IoBase16, Value8 );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   RwIo8IdxData
//
// Description: This function reads an 8bit value from a specific I/O
//              Index/Data port, then applies Set/Reset masks and writes
//              it back.
//
// Input:       IoBase16  - A 16 Bit I/O Address for Index I/O Port
//              RegIdx8   - An 8 Bit Register Index
//              SetBit8   - Mask of 8bits to set (1 = Set)
//              ResetBit8 - Mask of 8bits to reset (1 = Reset)
//
// Output:      None
//
// Notes:       The default Data I/O Port is the Index I/O Port plus 1, if
//              your Data I/O Port is not that, please modify IoRead8IdxData
//              and IoWrite8IdxData's "++IoBase16".
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID RwIo8IdxData (
    IN UINT16       IoBase16,
    IN UINT8        RegIdx8,
    IN UINT8        SetBit8,
    IN UINT8        ResetBit8 )
{
    UINT8           Buffer8 ;

    Buffer8 = ReadIo8IdxData ( IoBase16, RegIdx8 ) & ~ResetBit8 | SetBit8;
    WriteIo8IdxData ( IoBase16, RegIdx8, Buffer8 );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   RwIo8IdxDataS3
//
// Description: This function reads an 8bit value from a specific I/O
//              Index/Data ports, then applies Set/Reset masks, and writes
//              it back. Also writes a copy to Boot Script Table.
//
// Input:       *mBootScriptSave - Pointer to Boot Script Save Protocal
//              IoBase16         - A 16 Bit I/O Address for Index I/O Port 
//              RegIdx8          - An 8 Bit Register Index
//              SetBit8          - Mask of 8bits to set (1 = Set)
//              ResetBit8        - Mask of 8bits to reset (1 = Reset)
//
// Output:      An 8Bit data read from the specific Index/Data I/O port
//              after appling Set/Reset masks. 
//
// Notes:       The default Data I/O Port is the Index I/O Port plus 1, if
//              your Data I/O Port is not that, please modify below 
//              "IoBase16+1" and IoWrite8IdxData's "++IoBase16".
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID RwIo8IdxDataS3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT16                           IoBase16,
    IN UINT8                            IoReg8,
    IN UINT8                            SetBit8,
    IN UINT8                            ResetBit8 )
{
    RwIo8IdxData (IoBase16, IoReg8, SetBit8, ResetBit8);

    BOOT_SCRIPT_S3_IO_WRITE_MACRO( mBootScriptSave, \
                                   EfiBootScriptWidthUint8,\
                                   IoBase16, \
                                   1, \
                                   &IoReg8 );
    ResetBit8 = ~ResetBit8;
    BOOT_SCRIPT_S3_IO_READ_WRITE_MACRO( mBootScriptSave, \
                                        EfiBootScriptWidthUint8, \
                                        IoBase16 + 1, \
                                        &SetBit8, \
                                        &ResetBit8 );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   WritePci8S3
//
// Description: This function writes an 8bits data to the specific PCI
//              register and Boot Script.
//
// Input:       *mBootScriptSave - Pointer to Boot Script Save Protocal
//              Bus              - PCI Bus number.
//              Dev              - PCI Device number.
//              Fun              - PCI Function number.
//              Reg              - PCI Register number.
//              Value8           - An 8 Bits data will be written to the
//                                 specific PCI register and Boot Script.
//
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID WritePci8S3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT8                            Bus,
    IN UINT8                            Dev,
    IN UINT8                            Fun,
    IN UINT16                           Reg,
    IN UINT8                            Value8 )
{
	WRITE_PCI8(Bus, Dev, Fun, Reg, Value8);

  BOOT_SCRIPT_S3_MEM_WRITE_MACRO( \
    mBootScriptSave, \
    EfiBootScriptWidthUint8, \
    CSP_PCIE_CFG_ADDRESS(Bus, Dev, Fun, Reg), \
    1, \
    &Value8
    );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   WritePci16S3
//
// Description: This function writes a 16bits data to the specific PCI
//              register and Boot Script.
//
// Input:       *mBootScriptSave - Pointer to Boot Script Save Protocal
//              Bus              - PCI Bus number.
//              Dev              - PCI Device number.
//              Fun              - PCI Function number.
//              Reg              - PCI Register number.
//              Value16          - A 16 Bits data will be written to the
//                                 specific PCI register and Boot Script.
//
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID WritePci16S3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT8                            Bus,
    IN UINT8                            Dev,
    IN UINT8                            Fun,
    IN UINT16                           Reg,
    IN UINT16                           Value16 )
{

	WRITE_PCI16(Bus, Dev, Fun, Reg, Value16);

  BOOT_SCRIPT_S3_MEM_WRITE_MACRO( \
    mBootScriptSave, \
    EfiBootScriptWidthUint16, \
    CSP_PCIE_CFG_ADDRESS(Bus, Dev, Fun, Reg), \
    1, \
    &Value16
    );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   WritePci32S3
//
// Description: This function writes a 32bits data to the specific PCI
//              register and Boot Script.
//
// Input:       *mBootScriptSave - Pointer to Boot Script Save Protocal
//              Bus              - PCI Bus number.
//              Dev              - PCI Device number.
//              Fun              - PCI Function number.
//              Reg              - PCI Register number.
//              Value32          - A 32 Bits data will be written to the
//                                 specific PCI register and Boot Script.
//
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID WritePci32S3 (
    IN EFI_BOOT_SCRIPT_SAVE_PROTOCOL    *mBootScriptSave,
    IN UINT8                            Bus,
    IN UINT8                            Dev,
    IN UINT8                            Fun,
    IN UINT16                           Reg,
    IN UINT32                           Value32 )
{

	WRITE_PCI32(Bus, Dev, Fun, Reg, Value32);

  BOOT_SCRIPT_S3_MEM_WRITE_MACRO( \
    mBootScriptSave, \
    EfiBootScriptWidthUint32, \
    CSP_PCIE_CFG_ADDRESS(Bus, Dev, Fun, Reg), \
    1, \
    &Value32
    );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   RwPci8S3
//
// Description: This function reads an 8bits data from the specific PCI
//              register, applies masks, and writes it back, also writes it
//              to Boot Script.
//
// Input:       *mBootScriptSave - Pointer to Boot Script Save Protocal
//              Bus              - PCI Bus number.
//              Dev              - PCI Device number.
//              Fun              - PCI Function number.
//              Reg              - PCI Register number.
//              SetBit8          - Mask of bits to set (1 = Set)
//              ResetBit8        - Mask of bits to clear  (1 = clear)
//
// Output:      None.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID RwPci8S3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT8                            Bus,
    IN UINT8                            Dev,
    IN UINT8                            Fun,
    IN UINT16                           Reg,
    IN UINT8                            SetBit8,
    IN UINT8                            ResetBit8 )
{
    RW_PCI8(Bus, Dev, Fun, Reg, SetBit8, ResetBit8);

    ResetBit8 = ~ResetBit8;

    BOOT_SCRIPT_S3_MEM_READ_WRITE_MACRO( \
      mBootScriptSave, \
      EfiBootScriptWidthUint8, \
      CSP_PCIE_CFG_ADDRESS(Bus, Dev, Fun, Reg), \
      &SetBit8, \
      &ResetBit8
      );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   RwPci16S3
//
// Description: This function reads a 16bits data from the specific PCI
//              register, applies masks, and writes it back, also writes it
//              to Boot Script.
//
// Input:       *mBootScriptSave - Pointer to Boot Script Save Protocal
//              Bus              - PCI Bus number.
//              Dev              - PCI Device number.
//              Fun              - PCI Function number.
//              Reg              - PCI Register number.
//              SetBit16         - Mask of bits to set (1 = Set)
//              ResetBit16       - Mask of bits to clear  (1 = clear)
//
// Output:      None.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID RwPci16S3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT8                            Bus,
    IN UINT8                            Dev,
    IN UINT8                            Fun,
    IN UINT16                           Reg,
    IN UINT16                           SetBit16,
    IN UINT16                           ResetBit16 )
{
    RW_PCI16(Bus, Dev, Fun, Reg, SetBit16, ResetBit16);

    ResetBit16 = ~ResetBit16;

    BOOT_SCRIPT_S3_MEM_READ_WRITE_MACRO( \
      mBootScriptSave, \
      EfiBootScriptWidthUint16, \
      CSP_PCIE_CFG_ADDRESS(Bus, Dev, Fun, Reg), \
      &SetBit16, \
      &ResetBit16
      );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   RwPci32S3
//
// Description: This function reads a 32bits data from the specific PCI
//              register, applies masks, and writes it back, also writes it
//              to Boot Script.
//
// Input:       *mBootScriptSave - Pointer to Boot Script Save Protocal
//              Bus              - PCI Bus number.
//              Dev              - PCI Device number.
//              Fun              - PCI Function number.
//              Reg              - PCI Register number.
//              SetBit32         - Mask of bits to set (1 = Set)
//              ResetBit32       - Mask of bits to clear  (1 = clear)
//
// Output:      None.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID RwPci32S3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINT8                            Bus,
    IN UINT8                            Dev,
    IN UINT8                            Fun,
    IN UINT16                           Reg,
    IN UINT32                           SetBit32,
    IN UINT32                           ResetBit32 )
{
    RW_PCI32(Bus, Dev, Fun, Reg, SetBit32, ResetBit32);

    ResetBit32 = ~ResetBit32;

    BOOT_SCRIPT_S3_MEM_READ_WRITE_MACRO( \
      mBootScriptSave, \
      EfiBootScriptWidthUint32, \
      CSP_PCIE_CFG_ADDRESS(Bus, Dev, Fun, Reg), \
      &SetBit32, \
      &ResetBit32
      );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   WriteMem8S3
//
// Description: This function writes an 8bits data to a specific memory
//              (or MMIO) address and Boot Script.
//
// Input:       *mBootScriptSave - Pointer to Boot Script Save Protocal
//              Address          - A 64Bits Memory (or MMIO) address
//              Value8           - An 8Bits data writes to the address.
//
// Output:      None.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID WriteMem8S3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINTN                            Address,
    IN UINT8                            Value8 )
{
    MmioWrite8 (Address, Value8);

    BOOT_SCRIPT_S3_MEM_WRITE_MACRO( mBootScriptSave, \
                                    EfiBootScriptWidthUint8, \
                                    Address, \
                                    1, \
                                    &Value8 );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   WriteMem16S3
//
// Description: This function writes a 16bits data to a specific memory
//              (or MMIO) address and Boot Script.
//
// Input:       *mBootScriptSave - Pointer to Boot Script Save Protocal
//              Address          - A 64Bits Memory (or MMIO) address
//              Value16          - A 16Bits data writes to the address.
//
// Output:      None.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID WriteMem16S3(
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINTN                            Address,
    IN UINT16                           Value16 )
{
    MmioWrite16 (Address, Value16);

    BOOT_SCRIPT_S3_MEM_WRITE_MACRO( mBootScriptSave, \
                                    EfiBootScriptWidthUint16, \
                                    Address, \
                                    1, \
                                    &Value16 );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   WriteMem32S3
//
// Description: This function writes a 32bits data to a specific memory
//              (or MMIO) address and Boot Script.
//
// Input:       *mBootScriptSave - Pointer to Boot Script Save Protocal
//              Address          - A 64Bits Memory (or MMIO) address
//              Value32          - A 32Bits data writes to the address.
//
// Output:      None.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID WriteMem32S3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINTN                            Address,
    IN UINT32                           Value32 )
{
    MmioWrite32 (Address, Value32);

    BOOT_SCRIPT_S3_MEM_WRITE_MACRO( mBootScriptSave, \
                                    EfiBootScriptWidthUint32, \
                                    Address, \
                                    1, \
                                    &Value32 );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   RwMem8S3
//
// Description: This function reads an 8bits data from a specific memory
//              (or MMIO) address, applies masks, and writes it back, also
//              writes it to Boot Script.
//
// Input:       *mBootScriptSave - Pointer to Boot Script Save Protocal
//              Address - A 64Bits Memory (or MMIO) address
//              SetBit8 - Mask of bits to set (1 = Set)
//              ResetBit8 - Mask of bits to clear  (1 = clear)
//
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID RwMem8S3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINTN                            Address,
    IN UINT8                            SetBit8,
    IN UINT8                            ResetBit8 )
{
    ResetBit8 = ~ResetBit8;
    MmioAndThenOr8 (Address, ResetBit8, SetBit8);

    BOOT_SCRIPT_S3_MEM_READ_WRITE_MACRO( mBootScriptSave, \
                                         EfiBootScriptWidthUint8, \
                                         Address, \
                                         &SetBit8, \
                                         &ResetBit8 );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   RwMem16S3
//
// Description: This function reads a 16bits data from a specific memory
//              (or MMIO) address, applies masks, and writes it back, also
//              writes it to Boot Script.
//
// Input:       *mBootScriptSave - Pointer to Boot Script Save Protocal
//              Address - A 64Bits Memory (or MMIO) address
//              SetBit16 - Mask of bits to set (1 = Set)
//              ResetBit16 - Mask of bits to clear  (1 = clear)
//
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID RwMem16S3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINTN                            Address,
    IN UINT16                           SetBit16,
    IN UINT16                           ResetBit16 )
{
    ResetBit16 = ~ResetBit16;
    MmioAndThenOr16 (Address, ResetBit16, SetBit16);

    BOOT_SCRIPT_S3_MEM_READ_WRITE_MACRO( mBootScriptSave, \
                                         EfiBootScriptWidthUint16, \
                                         Address, \
                                         &SetBit16, \
                                         &ResetBit16 );
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   RwMem32S3
//
// Description: This function reads a 32bits data from a specific memory
//              (or MMIO) address, applies masks, and writes it back, also
//              writes it to Boot Script.
//
// Input:       *mBootScriptSave - Pointer to Boot Script Save Protocal
//              Address - A 64Bits Memory (or MMIO) address
//              SetBit32 - Mask of bits to set (1 = Set)
//              ResetBit32 - Mask of bits to clear  (1 = clear)
//
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID RwMem32S3 (
    IN AMI_S3_SAVE_PROTOCOL             *mBootScriptSave,
    IN UINTN                            Address,
    IN UINT32                           SetBit32,
    IN UINT32                           ResetBit32 )
{
    ResetBit32 = ~ResetBit32;
    MmioAndThenOr32 (Address, ResetBit32, SetBit32);

    BOOT_SCRIPT_S3_MEM_READ_WRITE_MACRO( mBootScriptSave, \
                                         EfiBootScriptWidthUint32, \
                                         Address, \
                                         &SetBit32, \
                                         &ResetBit32 );
}
//*/
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2012, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
