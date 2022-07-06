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

//*************************************************************************
// $Header: $
//
// $Revision: $
//
// $Date: $
//**********************************************************************
// Revision History
// ----------------
// $Log: $
// 
//
//**********************************************************************

//<AMI_FHDR_START>
//----------------------------------------------------------------------------
//
// Name:        SB.h
//
// Description: South Bridge header file, define all the South Bridge
//              specific equates and structures in this file.
//
//---------------------------------------------------------------------------
//<AMI_FHDR_END>
#ifndef  _SB_H_                         //To Avoid this header get compiled twice
#define  _SB_H_

#include <Hob.h>
#include <Library/DebugLib.h>
#include <Protocol/DebugPort.h>


#define EFI_SB_PCI_CFG_ADDRESS(bus, dev, func, reg) \
  (((UINT64)((((UINTN)bus) << 24) + (((UINTN)dev) << 16) + (((UINTN)func) << 8) + ((UINTN)reg))) & 0x00000000ffffffff)

#define SB_PCI_CFG_ADDRESS(bus, dev, func, reg) \
  ((VOID*) (UINTN)(((bus) << 16) + ((dev) << 11) + ((func) << 8) + reg))

#define SB_PCIE_CFG_ADDRESS(bus, dev, func, reg) \
   ((VOID*) (UINTN) (PcdGet64 (PcdPciExpressBaseAddress) + ((bus) << 20) + ((dev) << 15) + ((func) << 12) + reg))

#define PCH_DESKTOP_SKU    0x0000       // (P021309C)
#define PCH_MOBILE_SKU     0x0001       // (P021309C)
#define PCH_A0   0x00                   // (P020309E)
#define PCH_A1   0x02                   // (P020309E)
#define PCH_B0   0x03                   // (P020309E)
#define PCH_B1   0x04                   // (P042809G)
#define PCH_B2   0x05
#define PCH_B3   0x06                   // (P081709C)


                                        // (P040109A)>
/*** PORTING **** Create a new GUID for every new SMBUS Controller **/
/** Define new GUID for every new SMBUS controller **/
// {42ACDDDC-6573-47f3-8AD5-A63AB1166243}
#define AMI_SMBUS_CONTROLLER_1_GUID \
    {0x42acdddc, 0x6573, 0x47f3, 0x8a, 0xd5, 0xa6, 0x3a, 0xb1, 0x16, 0x62, 0x43}

// {4DB9B306-80F6-4a63-8092-F57C735968F2}
#define AMI_SMBUS_CONTROLLER_2_GUID \
    {0x4db9b306, 0x80f6, 0x4a63, 0x80, 0x92, 0xf5, 0x7c, 0x73, 0x59, 0x68, 0xf2}

                                        // <(P040109A)
//-----------------------------------------------------------------------
// SB Device Bus Device Function Definitions
//-----------------------------------------------------------------------
// PCI to PCI Bridge Bus Device Function Definitions
#define P2P_BUS     0
#define P2P_DEV     30
#define P2P_FUNC    0
#define P2P_BUS_DEV_FUNC  EFI_SB_PCI_CFG_ADDRESS(P2P_BUS, P2P_DEV, P2P_FUNC, 0)
#define P2P_PCIE_CFG      SB_PCIE_CFG_ADDRESS(P2P_BUS, P2P_DEV, P2P_FUNC, 0)

// LPC Bridge Bus Device Function Definitions
#define LPC_BUS     0
#define SB_BUS      LPC_BUS
#define LPC_DEVICE  31
#define SB_DEV      LPC_DEVICE
#define LPC_FUNC    0
#define SB_FUN      LPC_FUNC
#define LPC_BUS_DEV_FUNC  EFI_SB_PCI_CFG_ADDRESS(LPC_BUS, LPC_DEVICE, LPC_FUNC, 0)
#define LPC_PCIE_CFG      SB_PCIE_CFG_ADDRESS(LPC_BUS_NUM, LPC_DEV_NUM, LPC_FUNC, 0)

// SATA0 Controller Device Function Definitions
#define SATA0_BUS   0
#define SATA0_DEV   31
#define SATA0_FUNC  2
#define SATA0_BUS_DEV_FUNC  EFI_SB_PCI_CFG_ADDRESS(SATA0_BUS, SATA0_DEV, SATA0_FUNC, 0)
#define SATA0_PCIE_CFG      SB_PCIE_CFG_ADDRESS(SATA0_BUS, SATA0_DEV, SATA0_FUNC, 0)

// SmBus Bus Device Function Definitions
#define SMBUS_BUS   0
#define SMBUS_BUS_NUM SMBUS_BUS
#define SMBUS_DEV   31
#define SMBUS_DEV_NUM SMBUS_DEV
#define SMBUS_FUNC  3
#define SMBUS_FUNC_NUM SMBUS_FUNC
#define SMBUS_DEV_FUN  (SMBUS_DEV << 3) + SMBUS_FUNC
#define SMBUS_BUS_DEV_FUNC  EFI_SB_PCI_CFG_ADDRESS(SMBUS_BUS, SMBUS_DEV, SMBUS_FUNC, 0)
#define SMBUS_PCIE_CFG      SB_PCIE_CFG_ADDRESS(SMBUS_BUS, SMBUS_DEV, SMBUS_FUNC, 0)

// SATA1 Controller Device Function Definitions
#define SATA1_BUS   0
#define SATA1_DEV   31
#define SATA1_FUNC  5
#define SATA1_BUS_DEV_FUNC  EFI_SB_PCI_CFG_ADDRESS(SATA1_BUS, SATA1_DEV, SATA1_FUNC, 0)
#define SATA1_PCIE_CFG      SB_PCIE_CFG_ADDRESS(SATA1_BUS, SATA1_DEV, SATA1_FUNC, 0)

                                        // (P040709B)>
// Thermal Bus Device Function Definitions
#define THERMAL_BUS  0
#define THERMAL_DEV  31
#define THERMAL_FUNC 6
#define THERMAL_DEV_FUN  (THERMAL_DEV << 3) + THERMAL_FUNC
#define THERMAL_BUS_DEV_FUNC  EFI_SB_PCI_CFG_ADDRESS(THERMAL_BUS, THERMAL_DEV, THERMAL_FUNC, 0)
#define THERMAL_PCIE_CFG      SB_PCIE_CFG_ADDRESS(THERMAL_BUS, THERMAL_DEV, THERMAL_FUNC, 0)
                                        // <(P040709B)

// ME
#define ME_BUS   0
#define ME_DEV   22
#define ME_FUNC0 0
#define ME_FUNC1 1
#define ME_FUNC2 2
#define ME_FUNC3 3
#define ME_BUS_DEV_FUNC0  EFI_SB_PCI_CFG_ADDRESS(ME_BUS, ME_DEV, ME_FUNC0, 0)
#define ME_BUS_DEV_FUNC1  EFI_SB_PCI_CFG_ADDRESS(ME_BUS, ME_DEV, ME_FUNC1, 0)
#define ME_BUS_DEV_FUNC2  EFI_SB_PCI_CFG_ADDRESS(ME_BUS, ME_DEV, ME_FUNC2, 0)
#define ME_BUS_DEV_FUNC3  EFI_SB_PCI_CFG_ADDRESS(ME_BUS, ME_DEV, ME_FUNC3, 0)
#define ME_PCIE_CFG0      SB_PCIE_CFG_ADDRESS(ME_BUS, ME_DEV, ME_FUNC0, 0)
#define ME_PCIE_CFG1      SB_PCIE_CFG_ADDRESS(ME_BUS, ME_DEV, ME_FUNC1, 0)
#define ME_PCIE_CFG2      SB_PCIE_CFG_ADDRESS(ME_BUS, ME_DEV, ME_FUNC2, 0)
#define ME_PCIE_CFG3      SB_PCIE_CFG_ADDRESS(ME_BUS, ME_DEV, ME_FUNC3, 0)

// VE
#define VE_BUS   0
#define VE_DEV   23
#define VE_FUNC  0
#define VE_BUS_DEV_FUNC   EFI_SB_PCI_CFG_ADDRESS(VE_BUS, VE_DEV, VE_FUNC, 0)
#define VE_PCIE_CFG       SB_PCIE_CFG_ADDRESS(VE_BUS, VE_DEV, VE_FUNC, 0)

// LAN
#define LAN_BUS  0
#define LAN_DEV  25
#define LAN_FUNC 0
#define LAN_BUS_DEV_FUNC  EFI_SB_PCI_CFG_ADDRESS(LAN_BUS, LAN_DEV, LAN_FUNC, 0)
#define LAN_PCIE_CFG      SB_PCIE_CFG_ADDRESS(LAN_BUS, LAN_DEV, LAN_FUNC, 0)

// USB Bus Device Function Definitions
#define USB_BUS     0
#define PCI_EHCI_BUS_NUMBER USB_BUS
#define USB_DEV_26  26
//#define PCI_EHCI_DEVICE_NUMBER USB_DEV_26  // [EIP22933]
#define USB_DEV_29  29
#define USB_FUNC_0  0
#define USB_FUNC_1  1
#define USB_FUNC_2  2
#define USB_FUNC_3  3
#define USB_FUNC_4  4
#define USB_FUNC_7  7
#define USB_BUS_DEV_26_FUNC_0 EFI_SB_PCI_CFG_ADDRESS(USB_BUS, USB_DEV_26, USB_FUNC_0, 0)
#define USB_BUS_DEV_26_FUNC_1 EFI_SB_PCI_CFG_ADDRESS(USB_BUS, USB_DEV_26, USB_FUNC_1, 0)
#define USB_BUS_DEV_26_FUNC_2 EFI_SB_PCI_CFG_ADDRESS(USB_BUS, USB_DEV_26, USB_FUNC_2, 0)
#define USB_BUS_DEV_26_FUNC_7 EFI_SB_PCI_CFG_ADDRESS(USB_BUS, USB_DEV_26, USB_FUNC_7, 0)
#define USB_BUS_DEV_29_FUNC_0 EFI_SB_PCI_CFG_ADDRESS(USB_BUS, USB_DEV_29, USB_FUNC_0, 0)
#define USB_BUS_DEV_29_FUNC_1 EFI_SB_PCI_CFG_ADDRESS(USB_BUS, USB_DEV_29, USB_FUNC_1, 0)
#define USB_BUS_DEV_29_FUNC_2 EFI_SB_PCI_CFG_ADDRESS(USB_BUS, USB_DEV_29, USB_FUNC_2, 0)
#define USB_BUS_DEV_29_FUNC_3 EFI_SB_PCI_CFG_ADDRESS(USB_BUS, USB_DEV_29, USB_FUNC_3, 0)
#define USB_BUS_DEV_29_FUNC_4 EFI_SB_PCI_CFG_ADDRESS(USB_BUS, USB_DEV_29, USB_FUNC_4, 0)
#define USB_BUS_DEV_29_FUNC_7 EFI_SB_PCI_CFG_ADDRESS(USB_BUS, USB_DEV_29, USB_FUNC_7, 0)

// HDA
#define HDA_BUS  0
#define HDA_DEV  27
#define HDA_FUNC 0
#define HDA_BUS_DEV_FUNC  EFI_SB_PCI_CFG_ADDRESS(HDA_BUS, HDA_DEV, HDA_FUNC, 0)
#define HDA_PCIE_CFG      SB_PCIE_CFG_ADDRESS(HDA_BUS, HDA_DEV, HDA_FUNC, 0)

// PCIE Bus Device Function Definitions
#define SBPCIE_BUS     0
#define SBPCIE_DEV     28
//#define PCI_EHCI_DEVICE_NUMBER USB_DEV_26  // [EIP22933]
#define SBPCIE_FUNC_0  0
#define SBPCIE_FUNC_1  1
#define SBPCIE_FUNC_2  2
#define SBPCIE_FUNC_3  3
#define SBPCIE_FUNC_4  4
#define SBPCIE_FUNC_5  5
#define SBPCIE_FUNC_6  6
#define SBPCIE_FUNC_7  7

#define SBPCIE_BUS_DEV_FUNC_0 EFI_SB_PCI_CFG_ADDRESS(SBPCIE_BUS, SBPCIE_DEV, SBPCIE_FUNC_0, 0)
#define SBPCIE_BUS_DEV_FUNC_1 EFI_SB_PCI_CFG_ADDRESS(SBPCIE_BUS, SBPCIE_DEV, SBPCIE_FUNC_1, 0)
#define SBPCIE_BUS_DEV_FUNC_2 EFI_SB_PCI_CFG_ADDRESS(SBPCIE_BUS, SBPCIE_DEV, SBPCIE_FUNC_2, 0)
#define SBPCIE_BUS_DEV_FUNC_3 EFI_SB_PCI_CFG_ADDRESS(SBPCIE_BUS, SBPCIE_DEV, SBPCIE_FUNC_3, 0)
#define SBPCIE_BUS_DEV_FUNC_4 EFI_SB_PCI_CFG_ADDRESS(SBPCIE_BUS, SBPCIE_DEV, SBPCIE_FUNC_4, 0)
#define SBPCIE_BUS_DEV_FUNC_5 EFI_SB_PCI_CFG_ADDRESS(SBPCIE_BUS, SBPCIE_DEV, SBPCIE_FUNC_5, 0)
#define SBPCIE_BUS_DEV_FUNC_6 EFI_SB_PCI_CFG_ADDRESS(SBPCIE_BUS, SBPCIE_DEV, SBPCIE_FUNC_6, 0)
#define SBPCIE_BUS_DEV_FUNC_7 EFI_SB_PCI_CFG_ADDRESS(SBPCIE_BUS, SBPCIE_DEV, SBPCIE_FUNC_7, 0)

//----------------------------------------------------------------------------
// PCH : ME Equates (D22:F0/F1)
//----------------------------------------------------------------------------
#define ICH_REG_ME_VID           0x00
#define ICH_REG_ME_DID           0x02
#define ICH_REG_ME_PCICMD        0x04
#define ICH_REG_ME_PCISTS        0x06
#define ICH_REG_ME_RID           0x08
#define ICH_REG_ME_CC            0x09
#define ICH_REG_ME_CLS           0x0C
#define ICH_REG_ME_PLT           0x0D
#define ICH_REG_ME_HT            0x0E
#define ICH_REG_ME_HECI0_BAR     0x10
#define ICH_REG_ME_SVID          0x2C
#define ICH_REG_ME_SID           0x2E
#define ICH_REG_ME_CAPP          0x34
#define ICH_REG_ME_INTR          0x3C
#define ICH_REG_ME_MLMG          0x3E
#define ICH_REG_ME_HFS           0x40
#define ICH_REG_ME_UMA           0x44
#define ICH_REG_ME_GMES          0x48
#define ICH_REG_ME_H_GS          0x4C
#define ICH_REG_ME_PID           0x50
#define ICH_REG_ME_PC            0x52
#define ICH_REG_ME_PMCS          0x54
#define ICH_REG_ME_MID           0x8C
#define ICH_REG_ME_MC            0x8E
#define ICH_REG_ME_MA            0x90
#define ICH_REG_ME_MUA           0x94
#define ICH_REG_ME_MD            0x98
#define ICH_REG_ME_MIDM          0xA0
#define ICH_REG_ME_HERES         0xBC
#define ICH_REG_ME_HER1          0xC0
#define ICH_REG_ME_HER2          0xC4
#define ICH_REG_ME_HER3          0xC8
#define ICH_REG_ME_HER4          0xCC
#define ICH_REG_ME_HER5          0xD0
#define ICH_REG_ME_HER6          0xD4
#define ICH_REG_ME_HER7          0xD8
#define ICH_REG_ME_HER8          0xDC

//----------------------------------------------------------------------------
// PCH : LAN Equates (D25:F0)
//----------------------------------------------------------------------------
#define ICH_REG_LAN_VID         0x000
#define ICH_REG_LAN_DID         0x002
#define ICH_REG_LAN_PCICMD      0x004
#define ICH_REG_LAN_PCISTS      0x006
#define ICH_REG_LAN_RID         0x008
#define ICH_REG_LAN_CC          0x009
#define ICH_REG_LAN_CLS         0x00C
#define ICH_REG_LAN_PLT         0x00D
#define ICH_REG_LAN_HEADTYP     0x00E
#define ICH_REG_LAN_MBARA       0x010
#define ICH_REG_LAN_MBARB       0x014
#define ICH_REG_LAN_MBARC       0x018
#define ICH_REG_LAN_SVID        0x02C
#define ICH_REG_LAN_SID         0x02E
#define ICH_REG_LAN_ERBA        0x030
#define ICH_REG_LAN_CAPP        0x034
#define ICH_REG_LAN_INTR        0x03C
#define ICH_REG_LAN_MLMG        0x03E
#define ICH_REG_LAN_CLIST1      0x0C8
#define ICH_REG_LAN_PMC         0x0CA
#define ICH_REG_LAN_PMCS        0x0CC
#define ICH_REG_LAN_DR          0x0CF
#define ICH_REG_LAN_CLIST2      0x0D0
#define ICH_REG_LAN_MCTL        0x0D2
#define ICH_REG_LAN_MADDL       0x0D4
#define ICH_REG_LAN_MADDH       0x0D8
#define ICH_REG_LAN_MDAT        0x0DC
#define ICH_REG_LAN_FLRCAP      0x0E0
#define ICH_REG_LAN_FLRCLV      0x0E2
#define ICH_REG_LAN_DEVCTRL     0x0E4

//----------------------------------------------------------------------------
// PCH : PCIE Equates (D28:F0-F7)
//----------------------------------------------------------------------------
#define ICH_REG_PCIE_VID         0x000
#define ICH_REG_PCIE_DID         0x002
#define ICH_REG_PCIE_PCICMD      0x004
#define ICH_REG_PCIE_PCISTS      0x006
#define ICH_REG_PCIE_RID         0x008
#define ICH_REG_PCIE_PI          0x009
#define ICH_REG_PCIE_SCC         0x00A
#define ICH_REG_PCIE_BCC         0x00B
#define ICH_REG_PCIE_CLS         0x00C
#define ICH_REG_PCIE_PLT         0x00D
#define ICH_REG_PCIE_HEADTYP     0x00E
#define ICH_REG_PCIE_BNUM        0x018
#define ICH_REG_PCIE_SLT         0x01B
#define ICH_REG_PCIE_IOBL        0x01C
#define ICH_REG_PCIE_SSTS        0x01E
#define ICH_REG_PCIE_MBL         0x020
#define ICH_REG_PCIE_PMBL        0x024
#define ICH_REG_PCIE_PMBU32      0x028
#define ICH_REG_PCIE_PMLU32      0x02C
#define ICH_REG_PCIE_CAPP        0x034
#define ICH_REG_PCIE_INTR        0x03C
#define ICH_REG_PCIE_BCTRL       0x03E
#define ICH_REG_PCIE_CLIST       0x040
#define ICH_REG_PCIE_XCAP        0x042
#define ICH_REG_PCIE_DCAP        0x044
#define ICH_REG_PCIE_DCTL        0x048
#define ICH_REG_PCIE_DSTS        0x04A
#define ICH_REG_PCIE_LCAP        0x04C
#define ICH_REG_PCIE_LCTL        0x050
#define ICH_REG_PCIE_LSTS        0x052
#define ICH_REG_PCIE_SLCAP       0x054
#define ICH_REG_PCIE_SLCTL       0x058
#define ICH_REG_PCIE_SLSTS       0x05A
#define ICH_REG_PCIE_RCTL        0x05C
#define ICH_REG_PCIE_RSTS        0x060
#define ICH_REG_PCIE_DCAP2       0x064
#define ICH_REG_PCIE_DCTL2       0x068
#define ICH_REG_PCIE_LCTL2       0x070
#define ICH_REG_PCIE_MID         0x080
#define ICH_REG_PCIE_MC          0x082
#define ICH_REG_PCIE_MA          0x084
#define ICH_REG_PCIE_MS          0x088
#define ICH_REG_PCIE_SVCAP       0x090
#define ICH_REG_PCIE_SVID        0x094
#define ICH_REG_PCIE_PMCAP       0x0A0
#define ICH_REG_PCIE_PMC         0x0A2
#define ICH_REG_PCIE_PMCS        0x0A4
#define ICH_REG_PCIE_MPC2        0x0D4
#define ICH_REG_PCIE_MPC         0x0D8
#define ICH_REG_PCIE_SMSCS       0x0DC
#define ICH_REG_PCIE_RPDCHEN     0x0E1
#define ICH_REG_PCIE_PECR1       0x0E8
#define ICH_REG_PCIE_UES         0x104
#define ICH_REG_PCIE_UEM         0x108
#define ICH_REG_PCIE_UEV         0x10C
#define ICH_REG_PCIE_CES         0x110
#define ICH_REG_PCIE_CMD         0x114
#define ICH_REG_PCIE_AECC        0x118
#define ICH_REG_PCIE_RES         0x130
#define ICH_REG_PCIE_RCTCL       0x180
#define ICH_REG_PCIE_ESD         0x184
#define ICH_REG_PCIE_ULD         0x190
#define ICH_REG_PCIE_ULBA        0x198
#define ICH_REG_PCIE_PECR2       0x300
#define ICH_REG_PCIE_PEETM       0x318
#define ICH_REG_PCIE_PEC1        0x330 // [ EIP59633 ]
//----------------------------------------------------------------------------
// PCH : P2P Equates (D30:F0)
//----------------------------------------------------------------------------
#define ICH_REG_P2P_VID          0x00
#define ICH_REG_P2P_DID          0x02
#define ICH_REG_P2P_PCICMD       0x04
#define ICH_REG_P2P_PSTS         0x06
#define ICH_REG_P2P_RID          0x08
#define ICH_REG_P2P_CC           0x09
#define ICH_REG_P2P_PMLT         0x0D
#define ICH_REG_P2P_HEADTYP      0x0E
#define ICH_REG_P2P_BNUM         0x18
#define ICH_REG_P2P_SMLT         0x1B
#define ICH_REG_P2P_IOBASE       0x1C
#define ICH_REG_P2P_SECSTS       0x1E
#define ICH_REG_P2P_MEMBASE      0x20
#define ICH_REG_P2P_PMBL         0x24
#define ICH_REG_P2P_PMBU32       0x28
#define ICH_REG_P2P_PMLU32       0x2C
#define ICH_REG_P2P_CAPP         0x34
#define ICH_REG_P2P_INTR         0x3C
#define ICH_REG_P2P_BCTRL        0x3E
#define ICH_REG_P2P_SPDH         0x40
#define ICH_REG_P2P_DTC          0x44
#define ICH_REG_P2P_BPS          0x48
#define ICH_REG_P2P_BPC          0x4C
#define ICH_REG_P2P_SVCAP        0x50
#define ICH_REG_P2P_SVID         0x54

//----------------------------------------------------------------------------
// PCH : LPC Equates (D31:F0)
//----------------------------------------------------------------------------
#define ICH_REG_LPC_VID          0x00
#define ICH_REG_LPC_DID          0x02
#define ICH_REG_LPC_PCICMD       0x04
#define ICH_REG_LPC_PCISTS       0x06
#define ICH_REG_LPC_RID          0x08
#define ICH_REG_LPC_PI           0x09
#define ICH_REG_LPC_SCC          0x0A
#define ICH_REG_LPC_BSS          0x0B
#define ICH_REG_LPC_PLT          0x0D
#define ICH_REG_LPC_HDTP         0x0E
#define ICH_REG_LPC_SSID         0x2C
#define ICH_REG_LPC_PMBASE       0x40
#define ICH_REG_LPC_ACPI_CNTL    0x44
#define ICH_REG_LPC_GPIOBASE     0x48
#define ICH_REG_LPC_GC           0x4C

#define ICH_REG_LPC_PIRQA_ROUT   0x60
#define ICH_REG_LPC_PIRQB_ROUT   0x61
#define ICH_REG_LPC_PIRQC_ROUT   0x62
#define ICH_REG_LPC_PIRQD_ROUT   0x63

#define ICH_REG_LPC_SIRQ_CNTL    0x64

#define ICH_REG_LPC_PIRQE_ROUT   0x68
#define ICH_REG_LPC_PIRQF_ROUT   0x69
#define ICH_REG_LPC_PIRQG_ROUT   0x6A
#define ICH_REG_LPC_PIRQH_ROUT   0x6B

#define ICH_REG_LPC_IBDF         0x6C
#define ICH_REG_LPC_IODEC        0x80
#define ICH_REG_LPC_EN           0x82
#define ICH_REG_LPC_GEN1_DEC     0x84
#define ICH_REG_LPC_GEN2_DEC     0x88
#define ICH_REG_LPC_GEN3_DEC     0x8C
#define ICH_REG_LPC_GEN4_DEC     0x90

#define ICH_REG_LPC_ULKMC        0x94
#define ICH_REG_LPC_LGMR         0x98

#define ICH_REG_LPC_FWH_SEL1     0xD0
#define ICH_REG_LPC_FWH_SEL2     0xD2
#define ICH_REG_LPC_FWH_DEC_EN1  0xD8

#define ICH_REG_LPC_BIOS_CNTL    0xDC
#define ICH_REG_LPC_RCBA         0xF0

#define ICH_REG_NMI_SC           0x61
//-----------------------------------------------------------------------
// PCH: Power Management PCI Configuration Registers Equates
//-----------------------------------------------------------------------
#define ICH_REG_GEN_PMCON_1      0xA0
#define ICH_REG_LPC_PMCON_1      ICH_REG_GEN_PMCON_1
#define ICH_REG_GEN_PMCON_2      0xA2
#define ICH_REG_LPC_PMCON_2      ICH_REG_GEN_PMCON_2
#define ICH_REG_GEN_PMCON_3      0xA4
#define ICH_REG_LPC_PMCON_3      ICH_REG_GEN_PMCON_3
#define ICH_REG_GEN_PMCON_LOCK   0xA6
#define ICH_REG_LPC_PMCON_LOCK   ICH_REG_GEN_PMCON_LOCK
#define ICH_REG_CxSTATE_CNF      0xA9
#define ICH_REG_LPC_CxSTATE_CNF  ICH_REG_CxSTATE_CNF
#define ICH_REG_C4TIMING_CNT     0xAA
#define ICH_REG_LPC_C4TIMING_CNT ICH_REG_C4TIMING_CNT
#define ICH_REG_BM_BREAK_EN      0xAB
#define ICH_REG_LPC_BM_BREAK_EN  ICH_REG_BM_BREAK_EN
#define ICH_REG_PMIR             0xAC
#define ICH_REG_LPC_PMIR         ICH_REG_PMIR
#define ICH_REG_GPI_ROUT         0xB8
#define ICH_REG_LPC_GPI_ROUT     ICH_REG_GPI_ROUT
//-----------------------------------------------------------------------
// PCH: ACPI Power Management I/O Space Equates
//-----------------------------------------------------------------------
#define ICH_IOREG_PM1_STS        0x00 // PM1 Status
#define ICH_IOREG_PM1_EN         0x02 // PM1 Enable
#define ICH_IOREG_PM1_CNT        0x04 // PM1 Control
#define ICH_IOREG_PM1_TMR        0x08 // PM1 Timer
#define ICH_IOREG_PROC_CNT       0x10 // Processor Control
#define ICH_IOREG_LV2            0x14 // Level 2
#define ICH_IOREG_LV3            0x15 // Level 3 (Mobile Only)
#define ICH_IOREG_LV4            0x16 // Level 4 (Mobile Only)
#define ICH_IOREG_PM2_CNT        0x50 // PM2 Control (Mobile Only)
#define ICH_IOREG_GPE0_STS       0x20 // General Purpose Event 0 Status
#define ICH_IOREG_GPE0_EN        0x28 // General Purpose Event 0 Enables
#define ICH_IOREG_SMI_EN         0x30 // SMI# Control and Enable
#define ICH_IOREG_SMI_STS        0x34 // SMI Status
#define ICH_IOREG_ALT_GP_SMI_EN  0x38 // Alternate GPI SMI Enable
#define ICH_IOREG_ALT_GP_SMI_STS 0x3A // Alternate GPI SMI Status
#define ICH_IOREG_UPRWC          0x3C // USB Per Port Register Write Control
#define ICH_IOREG_GPE_CNTL       0x42 // General Purpose Event Control
#define ICH_IOREG_DEVACT_STS     0x44 // Device Activity Status
#define ICH_IOREG_C3_RES         0x54 // C3-Residency

// Sleep MACRO and Constant Definitions
#define BIT_WAK_STS       0x8000
#define BIT_SLP_TYP_MASK  0x1C00
#define S3_SLP_TYP        0x05
#define S4_SLP_TYP        0x06          // (P030309F)

//-----------------------------------------------------------------------
// PCH: System Management TCO Registers Equates
//-----------------------------------------------------------------------
// These are the TCO Register equates which are at the end of PM space
#define TCO_BASE_ADDRESS (PM_BASE_ADDRESS + 0x60)

#define ICH_IOREG_TCO_RLD        0x00 // TCO Timer Reload and Current Value
#define ICH_IOREG_TCO_DAT_IN     0x02 // TCO Data In
#define ICH_IOREG_TCO_DAT_OUT    0x03 // TCO Data Out
#define ICH_IOREG_TCO1_STS       0x04 // TCO1 Status
#define ICH_IOREG_TCO2_STS       0x06 // TCO2 Status
#define ICH_IOREG_TCO1_CNT       0x08 // TCO1 Count
#define ICH_IOREG_TCO2_CNT       0x0A // TCO2 Count
#define ICH_IOREG_TCO_MESSAGE1   0x0C // TCO Message 1
#define ICH_IOREG_TCO_MESSAGE2   0x0D // TCO Message 2
#define ICH_IOREG_TCO_WDCNT      0x0E // TCO Watchdog Control
#define ICH_IOREG_TCO_SWIRQGEN   0x10 // Software IRQ Generation
#define ICH_IOREG_TCO_TMR        0x12 // TCO Timer Initial Value

#define BIT_SECOND_TO_STS        0x0002
#define BIT_TCO_TMR_HLT          0x0800


//-----------------------------------------------------------------------
// PCH: General Purpose I/O Registers Equates
//-----------------------------------------------------------------------
#define ICH_GPIO_USE_SEL         0x00 // GPIO Use Select
#define ICH_GPIO_IO_SEL          0x04 // GPIO Input/Output Select
#define ICH_GPIO_GP_LVL          0x0C // GPIO Level for Input or Ouput
#define ICH_GPIO_GPO_BLINK       0x18 // GPIO Blink Enable
#define ICH_GPIO_GPI_INV         0x2C // GPI Signal Invert
#define ICH_GPIO_USE_SEL2        0x30 // GPIO Use Select 2
#define ICH_GPIO_IO_SEL2         0x34 // GPIO Input/Output Select 2
#define ICH_GPIO_GP_LVL2         0x38 // GPIO Level for Input or Output 2
#define ICH_GPIO_USE_SEL3        0x40 // GPIO Use Select 3
#define ICH_GPIO_IO_SEL3         0x44 // GPIO Input/Output Select 3
#define ICH_GPIO_GP_LVL3         0x48 // GPIO Level for Input or Output 3
                                        // (P091109D)>
#define ICH_GPIO_GP_RST_SEL1     0x60 // GPIO Reset Select
#define ICH_GPIO_GP_RST_SEL2     0x64 // GPIO Reset Select 2
#define ICH_GPIO_GP_RST_SEL3     0x68 // GPIO Reset Select 3
                                        // <(P091109D)

//----------------------------------------------------------------------------
// PCH : EHCI Equates (D26/D29:F0)
//----------------------------------------------------------------------------
#define ICH_REG_EHCI_VID         0x00
#define ICH_REG_EHCI_DID         0x02
#define ICH_REG_EHCI_PCICMD      0x04
#define ICH_REG_EHCI_PCISTS      0x06
#define ICH_REG_EHCI_RID         0x08
#define ICH_REG_EHCI_PI          0x09
#define ICH_REG_EHCI_SCC         0x0A
#define ICH_REG_EHCI_BCC         0x0B
#define ICH_REG_EHCI_PMLT        0x0D
#define ICH_REG_EHCI_HEADTYP     0x0E
#define ICH_REG_EHCI_MEMBASE     0x10
#define PCI_EHCI_BAR_OFFSET      ICH_REG_EHCI_MEMBASE
#define ICH_REG_EHCI_SVID        0x2C
#define ICH_REG_EHCI_SID         0x2E
#define ICH_REG_EHCI_CAPTR       0x34
#define ICH_REG_EHCI_INTLN       0x3C
#define ICH_REG_EHCI_INTPN       0x3D
#define ICH_REG_EHCI_PWRCID      0x50
#define ICH_REG_EHCI_NXTPTR      0x51
#define ICH_REG_EHCI_PWRCAP      0x52
#define ICH_REG_EHCI_PWRCTKSTS   0x54
#define ICH_REG_EHCI_RELNUM      0x60
#define ICH_REG_EHCI_LEG_EXT_CAP 0x68
#define ICH_REG_EHCI_LEG_EXT_CS  0x6C
#define ICH_REG_EHCI_SPECIAL_SMI 0x70
#define ICH_REG_EHCI_ACCTL       0x80
#define ICH_REG_EHCI_IR1         0x84
#define ICH_REG_EHCI_FLRCTL      0x9C
#define ICH_REG_EHCI_FLRSTS      0x9D
#define ICH_REG_EHCI_IR2         0xFC

#define ICH_MMIO_EHCI_CMD        0x20

#define ICH_MMIO_EHCI_PORTSC0    0x64
#define ICH_MMIO_EHCI_PORTSC1    0x68
#define ICH_MMIO_EHCI_PORTSC2    0x6C
#define ICH_MMIO_EHCI_PORTSC3    0x70
#define ICH_MMIO_EHCI_PORTSC4    0x74
#define ICH_MMIO_EHCI_PORTSC5    0x78
#define ICH_MMIO_EHCI_PORTSC6    0x7C
#define ICH_MMIO_EHCI_PORTSC7    0x80

#define ICH_MMIO_EHCI_PORTSC8    0x64
#define ICH_MMIO_EHCI_PORTSC9    0x68
#define ICH_MMIO_EHCI_PORTSC10   0x6C
#define ICH_MMIO_EHCI_PORTSC11   0x70
#define ICH_MMIO_EHCI_PORTSC12   0x74
#define ICH_MMIO_EHCI_PORTSC13   0x78
//----------------------------------------------------------------------------
// PCH : SATA Equates (D31:F2 & D31:F5)
//----------------------------------------------------------------------------
#define ICH_REG_SATA_VID                0x00
#define ICH_REG_SATA_DID                0x02
#define ICH_REG_SATA_PCICMD             0x04
#define ICH_REG_SATA_PCISTS             0x06
#define ICH_REG_SATA_RID                0x08
#define ICH_REG_SATA_PI                 0x09
#define ICH_REG_SATA_SCC                0x0A
#define ICH_REG_SATA_BCC                0x0B
#define ICH_REG_SATA_PMLT               0x0D
#define ICH_REG_SATA_HTYP               0x0E
#define ICH_REG_SATA_PCMD_BAR           0x10
#define ICH_REG_SATA_PCNL_BAR           0x14
#define ICH_REG_SATA_SCMD_BAR           0x18
#define ICH_REG_SATA_SCNL_BAR           0x1C
#define ICH_REG_SATA_BAR                0x20
#define ICH_REG_SATA_ABAR               0x24
#define ICH_REG_SATA_SVID               0x2C
#define ICH_REG_SATA_SID                0x2E
#define ICH_REG_SATA_CAP                0x34
#define ICH_REG_SATA_INTL               0x3C
#define ICH_REG_SATA_INTP               0x3D
#define ICH_REG_SATA_PRI_IDE_TIM        0x40      // Primary IDE Timing
#define ICH_REG_SATA_SEC_IDE_TIM        0x42      // Secondary IDE Timing

#define ICH_REG_SATA_SLV_IDETIM         0x44
#define ICH_REG_SATA_SDMA_CNT           0x48
#define ICH_REG_SATA_SDMA_TIM           0x4A
#define ICH_REG_SATA_IDE_CONFIG         0x54

#define ICH_REG_SATA_PID                0x70      // PCI Power Management Capability Identification
#define ICH_REG_SATA_PC                 0x72      // PCI Power Management Capabilities
#define ICH_REG_SATA_PMCS               0x74      // PCI Power Management Control and Status

#define ICH_REG_SATA_MSICI              0x80      // Message Signaled Interrupt Capability
#define ICH_REG_SATA_MSIMC              0x82      // Message Signaled Interrupt Message Control
#define ICH_REG_SATA_MSIMA              0x84      // Message Signaled Interrupt Message Address
#define ICH_REG_SATA_MSIMD              0x88      // Message Signaled Interrupt Message Data
#define R_SATA_MAP                      0x90      // SATA Address Map Register 8-bit
#define ICH_REG_SATA_MAP                R_SATA_MAP
#define   V_SATA_MAP_NON_COMBINED         0x00    // Non-Combined Mode
#define   V_SATA_MAP_COMBINED_PATA        0x01    // Combined Mode - PATA as Primary
#define   V_SATA_MAP_COMBINED_SATA        0x02    // Combined Mode - SATA as Primary
#define   V_SATA_MAP_IDE                  0x00    // IDE Mode
#define   V_SATA_MAP_AHCI                 0x40    // AHCI Mode
#define   V_SATA_MAP_RAID                 0x80    // RAID Mode

#define R_SATA_PCS                      0x92      // SATA Port Control & Status Register 16-bit
#define ICH_REG_SATA_PCS                R_SATA_PCS
#define   B_SATA_PORT_0_ENABLED           BIT00   // SATA Port 0 is Enabled
#define   B_SATA_PORT_1_ENABLED           BIT01   // SATA Port 1 is Enabled
#define   B_SATA_PORT_2_ENABLED           BIT02   // SATA Port 2 is Enabled
#define   B_SATA_PORT_3_ENABLED           BIT03   // SATA Port 3 is Enabled
#define   B_SATA_PORT_4_ENABLED           BIT04   // SATA Port 4 is Enabled
#define   B_SATA_PORT_5_ENABLED           BIT05   // SATA Port 5 is Enabled
#define   B_SATA_PORT_0_PRESENT           BIT08   // SATA Port 0 is Present
#define   B_SATA_PORT_1_PRESENT           BIT09   // SATA Port 1 is Present
#define   B_SATA_PORT_2_PRESENT           BIT10   // SATA Port 2 is Present
#define   B_SATA_PORT_3_PRESENT           BIT11   // SATA Port 3 is Present
#define   B_SATA_PORT_4_PRESENT           BIT12   // SATA Port 4 is Present
#define   B_SATA_PORT_5_PRESENT           BIT13   // SATA Port 5 is Present
#define ICH_REG_SATA_SCLKCG             0x94      // SATA Clock Gating Control
#define ICH_REG_SATA_SCLKGC             0x9C      // SATA Clock General Configuration
#define ICH_REG_SATA_SIRI               0xA0      // SATA Indexed Register Index
#define ICH_REG_SATA_SIRD               0xA4      // SATA Indexed Register Data
#define ICH_REG_SATA_CR0                0xA8      // Sata Capability Register 0
#define ICH_REG_SATA_CR1                0xAC      // Sata Capability Register 1
#define ICH_REG_SATA_FLRCID             0xB0      // FLR Capability ID
#define ICH_REG_SATA_FLRCLV             0xB2      // FLR Capability Length and Version
#define ICH_REG_SATA_FLRC               0xB4      // FLR Control
#define ICH_REG_SATA_ATC                0xC0      // APM Trapping Control
#define ICH_REG_SATA_ATS                0xC4      // APM Trapping Status
#define ICH_REG_SATA_SP                 0xD0      // Scratch Pad
#define ICH_REG_SATA_BFCS               0xE0      // BIST FIS Control / Status
#define ICH_REG_SATA_BFTD1              0xE4      // BIST FIS Transmit Data1
#define ICH_REG_SATA_BFTD2              0xE8      // BIST FIS Transmit Data2
//----------------------------------------------------------------------------
// PCH : AHCI Equates
//----------------------------------------------------------------------------
#define ICH_REG_AHCI_CAP                0x000
#define ICH_REG_AHCI_GHC                0x004
#define ICH_REG_AHCI_IS                 0x008
#define ICH_REG_AHCI_PI                 0x00C
#define ICH_REG_AHCI_VS                 0x010
#define ICH_REG_AHCI_CCC_CTL            0x014
#define ICH_REG_AHCI_CCC_PORTS          0x018
#define ICH_REG_AHCI_EM_LOC             0x01C
#define ICH_REG_AHCI_EM_CTRL            0x020
#define ICH_REG_AHCI_ECAP               0x024
#define ICH_REG_AHCI_NVMHCI_CAP         0x060
#define ICH_REG_AHCI_NVMHCI_PI          0x06C
#define ICH_REG_AHCI_VSP                0x0A0
#define ICH_REG_AHCI_RSTF               0x0C8
#define ICH_REG_AHCI_P0CMD              0x118
#define ICH_REG_AHCI_P1CMD              0x198
#define ICH_REG_AHCI_P2CMD              0x218
#define ICH_REG_AHCI_P3CMD              0x298
#define ICH_REG_AHCI_P4CMD              0x318
#define ICH_REG_AHCI_P5CMD              0x398
#define ICH_REG_AHCI_P6CMD              0x418
#define ICH_REG_AHCI_P7CMD              0x498


#define AHCI_CONTROLLER_DRIVER_VERSION 0x01
#define AHCI_INTEL_Vendor_ID  0x8086 // Intel

#define SCC_RAID_DEVICE                 4       // Sub Class Code
#define SCC_AHCI_DEVICE                 6       // Sub Class Code
#define BCC_MASS_STORAGE_DEVICE         1       // Base class code
#define IO_ENABLE                       1
#define MEMORY_ENABLE                   2
#define BUS_MASTER_ENABLE               4



                                        // (P061109C.1)>
/*
#ifndef _CR
#define _CR(Record, TYPE, Field)           ((TYPE *) ((CHAR8 *) (Record) - (CHAR8 *) &(((TYPE *) 0)->Field)))
#endif
#ifndef CR
#define CR(record, TYPE, field, signature) _CR(record, TYPE, field)
#endif
*/
#ifndef EFI_SIGNATURE_16
#define EFI_SIGNATURE_16(A, B)             ((A) | (B << 8))
#endif
#ifndef EFI_SIGNATURE_32
#define EFI_SIGNATURE_32(A, B, C, D)       (EFI_SIGNATURE_16 (A, B) | (EFI_SIGNATURE_16 (C, D) << 16))
#endif
                                        // <(P061109C.1)

#define SATA_CONTROLLER_SIGNATURE          EFI_SIGNATURE_32('S','A','T','A')
#define SATA_CONTROLLER_PRIVATE_DATA_FROM_THIS(a) CR(a, EFI_SATA_CONTROLLER_PRIVATE_DATA, IdeInit, SATA_CONTROLLER_SIGNATURE)

#define   B_ICH_SIDE_TIM_SITRE                   BIT14 // Drive 1 Timing Register Enable
#define   B_ICH_SIDE_TIM_ISP_MASK                (BIT13 | BIT12) //IORDY Sample Point
#define   B_ICH_SIDE_TIM_RCT_MASK                (BIT9 | BIT8) //Recovery time
#define     V_ICH_SIDE_TIM_ISP_5_CLOCK                0x00   //IORDY Sample Point = 5 clocks
#define     V_ICH_SIDE_TIM_ISP_4_CLOCK                0x01   //IORDY Sample Point = 4 clocks
#define     V_ICH_SIDE_TIM_ISP_3_CLOCK                0x02   //IORDY Sample Point = 3 clocks
#define     V_ICH_SIDE_TIM_RCT_4_CLOCK                0x00   //Recovery time = 4 clocks
#define     V_ICH_SIDE_TIM_RCT_3_CLOCK                0x01   //Recovery time = 3 clocks
#define     V_ICH_SIDE_TIM_RCT_2_CLOCK                0x02   //Recovery time = 2 clocks
#define     V_ICH_SIDE_TIM_RCT_1_CLOCK                0x03   //Recovery time = 1 clock
#define   B_ICH_SIDE_TIM_DTE1                    BIT7 //Drive 1 DMA Timing Enable
#define   B_ICH_SIDE_TIM_PPE1                    BIT6 //Drive 1 Prefetch/Posting Enable
#define   B_ICH_SIDE_TIM_IE1                     BIT5 //Drive 1 IORDY Sample Point Enable
#define   B_ICH_SIDE_TIM_TIME1                   BIT4 //Drive 1 Fast Timing Bank
#define   B_ICH_SIDE_TIM_DTE0                    BIT3 //Drive 0 DMA Timing Enable
#define   B_ICH_SIDE_TIM_PPE0                    BIT2 //Drive 0 Prefetch/Posting Enable
#define   B_ICH_SIDE_TIM_IE0                     BIT1 //Drive 0 IORDY Sample Point Enable
#define   B_ICH_SIDE_TIM_TIME0                   BIT0 //Drive 0 Fast Timing Bank
#define   B_ICH_SIDE_SIDETIM_SISP1_MASK               (BIT7 | BIT6) //IORDY Sample Point
#define   B_ICH_SIDE_SIDETIM_SRCT1_MASK               (BIT5 | BIT4) //Recovery time
#define   B_ICH_SIDE_SIDETIM_PISP1_MASK               (BIT3 | BIT2) //IORDY Sample Point
#define   B_ICH_SIDE_SIDETIM_PRCT1_MASK               (BIT1 | BIT0) //Recovery time
#define   B_ICH_IDE_SDMA_CNT_PSDE0               BIT0
#define   B_ICH_SDMA_TIM_PCT0_MASK                    0x0003
#define   B_ICH_SIDE_IDE_CONFIG_FAST_SCB1             BIT15
#define   B_ICH_SIDE_IDE_CONFIG_FAST_SCB0             BIT14
#define   B_ICH_SIDE_IDE_CONFIG_FAST_PCB1             BIT13
#define   B_ICH_SIDE_IDE_CONFIG_FAST_PCB0             BIT12
#define   B_ICH_SIDE_IDE_CONFIG_SCB1                  BIT3
#define   B_ICH_SIDE_IDE_CONFIG_SCB0                  BIT2
#define   B_ICH_SIDE_IDE_CONFIG_PCB1                  BIT1
#define   B_ICH_SIDE_IDE_CONFIG_PCB0                  BIT0
#define     V_ICH_SIDE_SDMA_TIM_CT4_RP6               0x00
#define     V_ICH_SIDE_SDMA_TIM_CT3_RP5               0x01
#define     V_ICH_SIDE_SDMA_TIM_CT2_RP4               0x02
#define     V_ICH_SIDE_SDMA_TIM_CT3_RP8               0x01
#define     V_ICH_SIDE_SDMA_TIM_CT2_RP8               0x02
#define     V_ICH_SIDE_SDMA_TIM_CT3_RP16              0x01

#define ICH_SATA_MAX_CHANNELS 6
#define ICH_SATA_MAX_DEVICES  1         // If Port Multiplier is supported then 32 else 1
#define ICH_SATA_MAX_PORTS    6

#define ICH_SATA_MASTER_DRIVE                0x00
#define ICH_SATA_SLAVE_DRIVE                 0x01


                                        // (P090109B)>
//----------------------------------------------------------------------------
// PCH : Thermal Controller Registers (D31:F6)
//----------------------------------------------------------------------------
#ifndef _PCH_REGS_THERMAL_H_
#define _PCH_REGS_THERMAL_H_
#define PCI_DEVICE_NUMBER_PCH_THERMAL   31
#define PCI_FUNCTION_NUMBER_PCH_THERMAL 6
#define R_PCH_THERMAL_VENDOR_ID         0x00
#define V_PCH_THERMAL_VENDOR_ID         V_PCH_INTEL_VENDOR_ID
#define R_PCH_THERMAL_DEVICE_ID         0x02
#define V_PCH_IBXPK_THERMAL_DEVICE_ID_0 0x3B32
#define R_PCH_THERMAL_COMMAND           0x04
#define  B_PCH_THERMAL_COMMAND_MSE       BIT01
#define  B_PCH_THERMAL_COMMAND_BME       BIT02
#define R_PCH_THERMAL_TBAR              0x10
#define V_PCH_THERMAL_TBAR_SIZE         (4 * 1024)
#define N_PCH_THREMAL_TBAR_ALIGNMENT    12
#define R_PCH_THERMAL_TBARH             0x14
#define R_PCH_THERMAL_SVID              0x2C
#define R_PCH_THERMAL_TBARB             0x40
#define V_PCH_THERMAL_TBARB_SIZE        (4 * 1024)
#define N_PCH_THREMAL_TBARB_ALIGNMENT   12
#define  B_PCH_THERMAL_SPTYPEN           BIT00
#define R_PCH_THERMAL_TBARBH            0x44
#define  B_PCH_THERMAL_TBARB_MASK        0xFFFFF000

#define R_PCH_TBARB_TSE                 0x01
#define R_PCH_TBARB_TSTTP               0x04
#define R_PCH_TBARB_TSCO                0x08
#define  B_PCH_TBARB_TSCO_LBC            BIT07
#define R_PCH_TBARB_TSES                0x0C
#define R_PCH_TBARB_TSGPEN              0x0D
#define R_PCH_TBARB_TSPC                0x0E
#define  B_PCH_TBARB_TSPC_PLD            BIT07
#define  B_PCH_TBARB_TSPC_CPDE           BIT06
#define R_PCH_TBARB_PPEC                0x10
#define R_PCH_TBARB_CTA                 0x12
#define R_PCH_TBARB_PTA                 0x14
#define R_PCH_TBARB_MGTA                0x16
#define R_PCH_TBARB_TRC                 0x1A
#define  B_PCH_TBARB_TRC_CPU2_RDEN       BIT15
#define  B_PCH_TBARB_TRC_THRM_PRT_EN     BIT12
#define  B_PCH_TBARB_TRC_C6WA_EN         BIT08
#define  B_PCH_TBARB_TRC_CPU_RDEN        BIT07
#define  B_PCH_TBARB_TRC_CPU_ENGY_RDEN   BIT06
#define  B_PCH_TBARB_TRC_PCH_RDEN        BIT05
#define  B_PCH_TBARB_TRC_MCH_RDEN        BIT04
#define  B_PCH_TBARB_TRC_DIMM3_RDEN      BIT03
#define  B_PCH_TBARB_TRC_DIMM2_RDEN      BIT02
#define  B_PCH_TBARB_TRC_DIMM1_RDEN      BIT01
#define  B_PCH_TBARB_TRC_DIMM0_RDEN      BIT00
#define R_PCH_TBARB_AE                  0x3F
#define  B_PCH_TBARB_AE_LockEnable       BIT07
#define  B_PCH_TBARB_AE_CPU_AltEn        BIT06
#define  B_PCH_TBARB_AE_MCH_AltEn        BIT05
#define  B_PCH_TBARB_AE_PCH_AltEn        BIT04
#define  B_PCH_TBARB_AE_DIMM_AltEn       BIT03
#define R_PCH_TBARB_PTL                 0x56
#define R_PCH_TBARB_MMGPC               0x64
#define R_PCH_TBARB_MMPC                0x66
#define R_PCH_TBARB_MPCPC               0x68
#define R_PCH_TBARB_TSPIEN              0x82
#define R_PCH_TBARB_TSLOCK              0x83
#define  B_PCH_TBARB_TSLOCK_LC           BIT02
#endif
                                        // <(P090109B)

//----------------------------------------------------------------------------
// PCH : Azalia Controller Registers (D27:F0)
//----------------------------------------------------------------------------
#define PCI_DEVICE_NUMBER_ICH_AZALIA           27
#define PCI_FUNCTION_NUMBER_ICH_AZALIA         0

#define R_ICH_HDA_COMMAND                       0x04
#define   B_ICH_HDA_COMMAND_BME                   BIT02
#define   B_ICH_HDA_COMMAND_MSE                   BIT01
#define   B_ICH_HDA_COMMAND_IOSE                  BIT00
#define R_ICH_HDA_HDBARL                        0x10
#define   V_ICH_HDA_HDBAR_SIZE                    (1 << 14)
#define R_ICH_HDA_HDBARU                        0x14
#define R_ICH_HDA_SVID                          0x2C
#define R_ICH_HDA_SID                           0x2E
#define R_ICH_HDA_HDCTL                         0x40
#define   B_ICH_HDA_HDCTL_CLKDETCLR               BIT03
#define   B_ICH_HDA_HDCTL_CLKDETEN                BIT02
#define   B_ICH_HDA_HDCTL_CLKDET                  BIT01
#define   B_ICH_HDA_HDCTL_MODE                    BIT00
#define R_ICH_HDA_TCSEL                         0x44
#define R_ICH_HDA_PCS                           0x54
#define   B_ICH_HDA_PCS_PMES                      BIT15
#define   B_ICH_HDA_PCS_PMEE                      BIT08
#define R_ICH_HDA_DEVC                          0x78    // (P081109B)
#define   B_ICH_HDA_NSNPEN                        BIT11 // (P081109B)

#define R_ICH_HDA_VCCAP                         0x100
#define R_ICH_HDA_PVCCAP1                       0x104
#define R_ICH_HDA_PVCCAP2                       0x108
#define R_ICH_HDA_PVCCTL                        0x10C
#define R_ICH_HDA_PVCSTS                        0x10E
#define R_ICH_HDA_VC0CAP                        0x110

#define R_ICH_HDA_VC0CTL                        0x114
#define R_ICH_HDA_VCICTL                        0x120
#define   B_ICH_HDA_VCICTL_EN                     BIT31
#define   V_ICH_HDA_VCICTL_ID_MASK               (3 << 24)
#define   V_ICH_HDA_VCICTL_TVM_MASK              0xFF

#define R_ICH_HDA_VCISTS                        0x124
#define R_ICH_HDA_RCCAP                         0x130
#define R_ICH_HDA_ESD                           0x134
#define R_ICH_HDA_L1DESC                        0x140
#define R_ICH_HDA_L1ADDL                        0x148
#define R_ICH_HDA_L1ADDU                        0x14C


//----------------------------------------------------------------------------
// PCH: MMIO Register Offset Address
//----------------------------------------------------------------------------
#define ICH_RCRB_V0CTL      0x0014  // Virtual Channel 0 Resource Control Register
#define ICH_RCRB_V0STS      0x001a  // Virtual Channel 0 Resource Status Register
#define ICH_RCRB_V1CAP      0x001c  // Virtual Channel 1 Resource Capability Register
#define ICH_RCRB_V1CTL      0x0020  // Virtual Channel 1 Resource Control Register
#define ICH_RCRB_V1STS      0x0026  // Virtual Channel 1 Resource Status Register
#define ICH_RCRB_CIR0       0x0050  // Chipset Initialization Register 0
#define ICH_RCRB_CIR1       0x0088  // Chipset Initialization Register 1
#define ICH_RCRB_REC        0x00AC  // Root Error Command Register
#define ICH_RCRB_ESD        0x0104  // Element Self Description
#define ICH_RCRB_ULD        0x0110  // Upstream Link Description
#define ICH_RCRB_ULBA       0x0118  // Upstream Link Base Address
#define ICH_RCRB_RP1D       0x0120  // Root Port 1 Descriptor Register
#define ICH_RCRB_RP1BA      0x0128  // Root Port 1 Base Address Register
#define ICH_RCRB_RP2D       0x0130  // Root Port 2 Descriptor Register
#define ICH_RCRB_RP2BA      0x0138  // Root Port 2 Base Address Register
#define ICH_RCRB_RP3D       0x0140  // Root Port 3 Descriptor Register
#define ICH_RCRB_RP3BA      0x0148  // Root Port 3 Base Address Register
#define ICH_RCRB_RP4D       0x0150  // Root Port 4 Descriptor Register
#define ICH_RCRB_RP4BA      0x0158  // Root Port 4 Base Address Register
#define ICH_RCRB_HDD        0x0160  // High Definition Audio Descriptor Register
#define ICH_RCRB_HDBA       0x0168  // High Definition Audio Base Address Register
#define ICH_RCRB_RP5D       0x0170  // Root Port 5 Descriptor Register
#define ICH_RCRB_RP5BA      0x0178  // Root Port 5 Base Address Register
#define ICH_RCRB_RP6D       0x0180  // Root Port 6 Descriptor Register
#define ICH_RCRB_RP6BA      0x0188  // Root Port 6 Base Address Register
#define ICH_RCRB_ILCL       0x01A0  // Internal Link Capabilities Register
#define ICH_RCRB_LCAP       0x01A4  // Link Capabilities Register
#define ICH_RCRB_LCTL       0x01A8  // Link Control Register
#define ICH_RCRB_LSTS       0x01AA  // Link Status Register
#define ICH_RCRB_CIR2       0x01f4  // Chipset Initialization Register 2
#define ICH_RCRB_CIR3       0x01fc  // Chipset Initialization Register 3
#define ICH_RCRB_BCR        0x0220  // Backbone Configuration Register
#define ICH_RCRB_RPC        0x0224  // Root Port Configuration Register
#define ICH_RCRB_DMIC       0x0234  // DMI COntrol Register
#define ICH_RCRB_RPFN       0x0238  // Root Port Function Number and Hide for PCI Express* Root Ports Register
#define ICH_RCRB_FLRSTAT    0x0290  // FLR Pending Status Register

#define ICH_RCRB_CIR5       0x1D40  // Chipset Initialization Register 5
#define ICH_RCRB_TRSR       0x1E00  // Trap Status Register
#define ICH_RCRB_TRCR       0x1E10  // Trap Cycle Register
#define ICH_RCRB_TWDR       0x1E18  // Trap Write Data Register
#define ICH_RCRB_IOTR0      0x1E80  // I/O Trap Register 0
#define ICH_RCRB_IOTR1      0x1E88  // I/O Trap Register 1
#define ICH_RCRB_IOTR2      0x1E90  // I/O Trap Register 2
#define ICH_RCRB_IOTR3      0x1E98  // I/O Trap Register 3

#define ICH_RCRB_DMC        0x2010  // DMI Misc Control Register
#define ICH_RCRB_CIR6       0x2024  // Chipset Initialization Register 6 (Mobile Only)
#define ICH_RCRB_DMC2       0x2324  // DMI Misc Control Register 2

#define ICH_RCRB_TCTL       0x3000  // TCO Configuration Register
#define ICH_RCRB_D31IP      0x3100 // Device 31 Interrupt Pin
#define ICH_RCRB_D30IP      0x3104 // Device 30 Interrupt Pin
#define ICH_RCRB_D29IP      0x3108 // Device 29 Interrupt Pin
#define ICH_RCRB_D28IP      0x310C // Device 28 Interrupt Pin
#define ICH_RCRB_D27IP      0x3110 // Device 27 Interrupt Pin
#define ICH_RCRB_D26IP      0x3114 // Device 26 Interrupt Pin
#define ICH_RCRB_D25IP      0x3118 // Device 26 Interrupt Pin
#define ICH_RCRB_D22IP      0x3124 // Device 22 Interrupt Pin, (P101508A)
#define ICH_RCRB_IP_FUN0 (BIT03+BIT02+BIT01+BIT00)
#define ICH_RCRB_IP_FUN1 (BIT07+BIT06+BIT05+BIT04)
#define ICH_RCRB_IP_FUN2 (BIT11+BIT10+BIT09+BIT08)
#define ICH_RCRB_IP_FUN3 (BIT15+BIT14+BIT13+BIT12)
#define ICH_RCRB_IP_FUN4 (BIT19+BIT18+BIT17+BIT16)
#define ICH_RCRB_IP_FUN5 (BIT23+BIT22+BIT21+BIT20)
#define ICH_RCRB_IP_FUN6 (BIT27+BIT26+BIT25+BIT24)
#define ICH_RCRB_IP_FUN7 (BIT31+BIT30+BIT29+BIT28)
#define ICH_RCRB_IRQA 1
#define ICH_RCRB_IRQB 2
#define ICH_RCRB_IRQC 3
#define ICH_RCRB_IRQD 4

#define ICH_RCRB_D31IR      0x3140 // Device 31 Interrupt Route
#define ICH_RCRB_D30IR      0x3142 // Device 30 Interrupt Route
#define ICH_RCRB_D29IR      0x3144 // Device 29 Interrupt Route
#define ICH_RCRB_D28IR      0x3146 // Device 28 Interrupt Route
#define ICH_RCRB_D27IR      0x3148 // Device 27 Interrupt Route
#define ICH_RCRB_D26IR      0x314C // Device 26 Interrupt Route
#define ICH_RCRB_D25IR      0x3150 // Device 25 Interrupt Route
#define ICH_RCRB_D24IR      0x3154 // Device 24 Interrupt Route
#define ICH_RCRB_D22IR      0x315C // Device 22 Interrupt Route
#define ICH_RCRB_IR_INTA (BIT02+BIT01+BIT00)
#define ICH_RCRB_IR_INTB (BIT06+BIT05+BIT04)
#define ICH_RCRB_IR_INTC (BIT10+BIT09+BIT08)
#define ICH_RCRB_IR_INTD (BIT14+BIT13+BIT12)
#define ICH_RCRB_PIRQA 0
#define ICH_RCRB_PIRQB 1
#define ICH_RCRB_PIRQC 2
#define ICH_RCRB_PIRQD 3
#define ICH_RCRB_PIRQE 4
#define ICH_RCRB_PIRQF 5
#define ICH_RCRB_PIRQG 6
#define ICH_RCRB_PIRQH 7

#define ICH_RCRB_OIC        0x31FE // Other Interrupt Control Register
#define ICH_RCRB_PRSTS      0x3310 // Power and Reset Srtatus Register
#define ICH_RCRB_CIR7       0x3314 // Chipset Initialization Register 7
#define ICH_RCRB_CIR8       0x3324 // Chipset Initialization Register 8
#define ICH_RCRB_CIR9       0x3330 // Chipset Initialization Register 9
#define ICH_RCRB_CIR10      0x3340 // Chipset Initialization Register 10
#define ICH_RCRB_CIR13      0x3350 // Chipset Initialization Register 13
#define ICH_RCRB_CIR14      0x3368 // Chipset Initialization Register 14
#define ICH_RCRB_CIR15      0x3378 // Chipset Initialization Register 15
#define ICH_RCRB_CIR16      0x3388 // Chipset Initialization Register 16
#define ICH_RCRB_CIR17      0x33A0 // Chipset Initialization Register 17
#define ICH_RCRB_CIR18      0x33A8 // Chipset Initialization Register 18
#define ICH_RCRB_CIR19      0x33C0 // Chipset Initialization Register 19
#define ICH_RCRB_CIR20      0x33CC // Chipset Initialization Register 20
#define ICH_RCRB_CIR21      0x33D0 // Chipset Initialization Register 21
#define ICH_RCRB_CIR22      0x33D4 // Chipset Initialization Register 22

#define ICH_RCRB_RTC        0x3400 // RTC Control Register
#define ICH_RCRB_HPTC       0x3404 // High Precision Timer Control Register
#define ICH_RCRB_GCS        0x3410 // General Control and Status Register
#define R_RCBA_GCS          ICH_RCRB_GCS
#define   B_RCRB_GCS_BBS      0x0C00
#define   B_RCRB_GCS_BBS_SPI  0x0400

#define ICH_RCRB_BUC        0x3414 // Backed Up Control Register
#define R_RCBA_BUC          ICH_RCRB_BUC
#define   B_RCRB_BUC_TS       0x0001

#define R_FUNCTION_DISABLE              0x3418    // Function Disable Register 32-bit
#define   B_BIT0_RESERVED                 BIT00   // Reserved Bit
#define   B_BIT1_RESERVED                 BIT01   // Reserved Bit
#define   B_SATA_0_DISABLE                BIT02   // Disable Serial ATA (SATA#0)
#define   B_SMBUS_DISABLE                 BIT03   // Disable SMBUS
#define   B_AZALIA_HD_AUDIO_DISABLE       BIT04   // Disable High Definition(HD) Audio
#define   B_UHCI_6_DISABLE                BIT07   // Disable UHCI 6
#define   B_UHCI_1_DISABLE                BIT08   // Disable UHCI 1
#define   B_UHCI_2_DISABLE                BIT09   // Disable UHCI 2
#define   B_UHCI_3_DISABLE                BIT10   // Disable UHCI 3
#define   B_UHCI_4_DISABLE                BIT11   // Disable UHCI 4
#define   B_UHCI_5_DISABLE                BIT12   // Disable UHCI 5
#define   B_EHCI_2_DISABLE                BIT13   // Disable EHCI 2
#define   B_LPC_DISABLE                   BIT14   // Disable LPC Bridge
#define   B_EHCI_1_DISABLE                BIT15   // Disable EHCI 1
#define   B_PCIE_1_DISABLE                BIT16   // Disable PCI Express 1
#define   B_PCIE_2_DISABLE                BIT17   // Disable PCI Express 2
#define   B_PCIE_3_DISABLE                BIT18   // Disable PCI Express 3
#define   B_PCIE_4_DISABLE                BIT19   // Disable PCI Express 4
#define   B_PCIE_5_DISABLE                BIT20   // Disable PCI Express 5
#define   B_PCIE_6_DISABLE                BIT21   // Disable PCI Express 6
#define   B_THERMAL_DISABLE               BIT24   // Disable Thermal subsystem
#define   B_SATA_1_DISABLE                BIT25   // Disable Serial ATA (SATA#1)
#define   B_NVM_DISABLE                   BIT26   // Disable NVM Host, (P042809A)
#define   B_UHCI_9_DISABLE                BIT28   // Disable UHCI 9 // (P120208B)

#define ICH_RCRB_GC         0x341C // Clock Gating Register
#define ICH_RCRB_FDSW       0x3420 // Function Disable SUS Register
#define ICH_RCRB_FD2        0x3428 // Function Disable Register 2

#define ICH_RCRB_USBIR0     0x3500 // USB Initialization Register 0
#define ICH_RCRB_USBIR1     0x3504 // USB Initialization Register 1
#define ICH_RCRB_USBIR2     0x3508 // USB Initialization Register 2
#define ICH_RCRB_USBIR3     0x350C // USB Initialization Register 3
#define ICH_RCRB_USBIR4     0x3510 // USB Initialization Register 4
#define ICH_RCRB_USBIR5     0x3514 // USB Initialization Register 5
#define ICH_RCRB_USBIR6     0x3518 // USB Initialization Register 6
#define ICH_RCRB_USBIR7     0x351C // USB Initialization Register 7
#define ICH_RCRB_USBIR8     0x3520 // USB Initialization Register 8
#define ICH_RCRB_USBIR9     0x3524 // USB Initialization Register 9
#define ICH_RCRB_USBIR10    0x3528 // USB Initialization Register 10
#define ICH_RCRB_USBIR11    0x352C // USB Initialization Register 11
#define ICH_RCRB_USBIR12    0x3530 // USB Initialization Register 12
#define ICH_RCRB_USBIR13    0x3534 // USB Initialization Register 13

#define ICH_RCRB_USBIRC     0x3564 // USB Initialization Register C
#define ICH_RCRB_USBIRA     0x3570 // USB Initialization Register A
#define ICH_RCRB_USBIRB     0x357C // USB Initialization Register B

#define ICH_RCRB_MISCCTL    0x3590 // Misc Control Register
#define ICH_RCRB_PDO        0x359C // USB Port Disable Override Register
#define ICH_RCRB_USBOCM1    0x35A0 // USB Overcurrent MAP Register 1
#define ICH_RCRB_USBOCM2    0x35A4 // USB Overcurrent MAP Register 2

#define ICH_RCRB_RMHWKCTL   0x35B0 // Rate Matching Hub Wake Control Register


//-----------------------------------------------------------------------
// PCH: SPI MMIO Configuration  Registers Equates
//-----------------------------------------------------------------------
#define ICH_REG_SPI_FDOC    0xB0 // Flash Descriptor Observability Control Register
#define ICH_REG_SPI_FDOD    0xB4 // Flash Descriptor Observability Data Register

#define ICH_REG_SPI_HSFS    0x04 // Hardware Sequencing Flash Status Register
#define ICH_REG_SPI_SSFSTS  0x90 // Software Squencing Flash Status
#define ICH_REG_SPI_SSFCTL  0x91 // Software Squencing Flash Control
#define ICH_REG_SPI_PREOP   0x94 // Prefix Opcode Configuration
#define ICH_REG_SPI_OPTYPE  0x96 // Opcode Type Configuration
#define ICH_REG_SPI_OPMENU  0x98 // Opcode Menu Configuration

#define R_RCRB_SPI_PREOP    ICH_REG_SPI_PREOP
#define R_RCRB_SPI_OPTYPE   ICH_REG_SPI_OPTYPE
#define R_RCRB_SPI_OPMENU   ICH_REG_SPI_OPMENU

#define ICH_REG_SPI_LVSCC   0xC4 // Host Lower Vendor Specific Component Capabilities Register
#define ICH_REG_SPI_UVSCC   0xC8 // Host Upper Vendor Specific Component Capabilities Register

#define   B_RCRB_SPI_SPIS_SCIP      0x0001
#define   B_RCRB_SPI_SPIS_SAG       0x0002
#define   B_RCRB_SPI_SPIS_SCDS      0x0004
#define   B_RCRB_SPI_SPIS_SBAS      0x0008

#define   B_RCRB_SPI_SPIC_AR        0x0001
#define   B_RCRB_SPI_SPIC_SCGO      0x0002
#define   B_RCRB_SPI_SPIC_ACS       0x0004
#define   B_RCRB_SPI_SPIC_SPOP      0x0008
#define   B_RCRB_SPI_SPIC_COP       0x0070
#define   B_RCRB_SPI_SPIC_DBC       0x3F00
#define   B_RCRB_SPI_SPIC_DATA      0x4000

#define R_RCRB_SPI_SPIS           0x0090      // SPI Status
#define   B_RCRB_SPI_SPIS_FCERR     0x0008    // Flash Cycle Error
#define R_RCRB_SPI_SPIC           0x0091      // SPI Control
#define   B_RCRB_SPI_SPIC_DATA      0x4000    // SPI Transaction Data Enable
#define   B_RCRB_SPI_SPIC_DBC       0x3F00    // SPI Data Byte Count (1..8,16,24,32,40,48,56,64)
#define   B_RCRB_SPI_SPIC_COP       0x0070    // SPI Cycle Opcode Pointer
#define   B_RCRB_SPI_SPIC_SPOP      0x0008    // Sequence Prefix Opcode Pointer
#define   B_RCRB_SPI_SPIC_ACS       0x0004    // SPI Atomic Cycle Sequence
#define   B_RCRB_SPI_SPIC_SCGO      0x0002    // SPI Cycle Go
#define R_RCRB_SPI_SPIA           0x0008      // SPI Address
#define R_RCRB_SPI_SPID0          0x0010      // SPI Data 0


//----------------------------------------------------------------------------
// HPET Configuration Register (SB_RCBA + 0x3404)
//----------------------------------------------------------------------------
#define R_ICH_HPET_CONFIG                       ICH_RCRB_HPTC
#define   B_ICH_HPET_ENABLE   BIT07



typedef struct
{
    UINT32        *VerbTable;
    UINT32        VendorDeviceId;
    UINT16        SubSystemId;
    UINT8         FrontPanelSupport;
    UINT16        NumberOfRearJacks;
    UINT16        NumberOfFrontJacks;
    UINT16        OemFlag;              // (P031009A)
} AMI_AZALIA_CODEC_VERB_TABLE;

//-----------------------------------------------------------------------
// PCH IDE and SATA registers
//-----------------------------------------------------------------------
#define IDE_CONTROLLER_DRIVER_VERSION 0x01

//  Number of Channels in PATA
#define     MAXIMUM_CHANNELS                0x02
#define     MAXIMUM_DEVICES                 0x02
#define     SCC_IDE_DEVICE                  1       // Sub Class Code
#define     BCC_MASS_STORAGE_DEVICE         1       // Base class code
#define     IO_ENABLE                       1
#define     MEMORY_ENABLE                   2
#define     BUS_MASTER_ENABLE               4


// PCI Config Space Equates
#define IDE_INTEL_Vendor_ID  0x8086  // Intel

#define ICH_IDE_BASE_ADDRESS         0xFFA0
#define ICH_IDE_REG_IDE_CONFIG       0x54 // IDE I/O Configuration
#define PEI_ICH_IDE_NONE             0x00
#define PEI_ICH_IDE_PRIMARY          0x01
#define PEI_ICH_IDE_SECONDARY        0x02
#define PEI_ICH_SATA_NONE            0x04
#define PEI_ICH_SATA_PRIMARY         0x08
#define PEI_ICH_SATA_SECONDARY       0x010

#define R_ICH_IDE_VID                          0x00
#define R_ICH_IDE_COMMAND                      0x04
#define   B_ICH_IDE_COMMAND_INT_DIS              0x0400
#define   B_ICH_IDE_COMMAND_FBE                  0x0200
#define   B_ICH_IDE_COMMAND_SERR_EN              0x0100
#define   B_ICH_IDE_COMMAND_WCC                  0x0080
#define   B_ICH_IDE_COMMAND_PER                  0x0040
#define   B_ICH_IDE_COMMAND_VPS                  0x0020
#define   B_ICH_IDE_COMMAND_PMWE                 0x0010
#define   B_ICH_IDE_COMMAND_SCE                  0x0008
#define   B_ICH_IDE_COMMAND_BME                  0x0004
#define   B_ICH_IDE_COMMAND_MSE                  0x0002
#define   B_ICH_IDE_COMMAND_IOSE                 0x0001
#define R_ICH_IDE_BASE_ADDRESS                 0x20
#define   B_ICH_IDE_BASE_ADDRESS_BAR             0xFFE0
#define R_ICH_IDE_TIMP                         0x40
#define R_ICH_IDE_TIMS                         0x42
#define   B_ICH_IDE_DEC_EN                       0x8000
#define R_ICH_SLV_IDETIM                       0x44
#define R_ICH_SDMA_CNT                         0x48
#define R_ICH_SDMA_TIM                         0x4A
#define R_ICH_IDE_CONFIG                       0x54
#define B_ICH_IDE_SIDE_PCS      0x000f
#define B_ICH_IDE_SATA_AMR      0x01
#define R_ICH_SATA_AMR                          0x90
#define V_ICH_IDE_SVID                         V_INTEL_VENDOR_ID
#define V_ICH_IDE_SID                          V_ICH_DEFAULT_SID
#define R_ICH_SIDE_PCS                          0x92

#define Programming_Interface 0x09

#define PI_PNE_NATIVE BIT00 //SATA, Reg Programming Interface (PI), Bit Primary Mode Native Enable (PNE), Native Mode
#define PI_SNE_NATIVE BIT02 //SATA, Reg Programming Interface (PI), Bit Secondary Mode Native Enable (SNE), Native Mode
#define PI_PNE_LEGACY 0x00 //SATA, Reg Programming Interface (PI), Bit Primary Mode Native Enable (PNE), Legacy Mode
#define PI_SNE_LEGACY 0x00 //SATA, Reg Programming Interface (PI), Bit Secondary Mode Native Enable (SNE), Legacy Mode

#define R_ICH_IDE_PCMD_BAR 0x10
#define R_ICH_IDE_PCNL_BAR 0x14
#define R_ICH_IDE_SCMD_BAR 0x18
#define R_ICH_IDE_SCNL_BAR 0x1C


//-----------------------------------------------------------------------;
// PCH : SMBus PCI Equates
//-----------------------------------------------------------------------;
#define ICH_SMB_CMD       0x04
#define R_COMMAND         ICH_SMB_CMD
#define  B_IOSE             BIT00
#define  B_MSE              BIT01
#define ICH_SMB_MBAR0     0x10
#define ICH_SMB_MBAR1     0x14
#define ICH_SMB_BASE      0x20
#define R_BASE_ADDRESS    ICH_SMB_BASE
#define ICH_SMB_INT_LN    0x3C
#define ICH_SMB_HOSTC     0x40
#define R_HOST_CONFIGURATION ICH_SMB_HOSTC
#define  B_HST_EN           BIT00
#define  B_SMB_SMI_EN       BIT01
#define  B_I2C_EN           BIT02
#define  B_SSRESET          BIT03

//-----------------------------------------------------------------------;
// PCH : SMBus I/O Space Equates
//-----------------------------------------------------------------------;
/*
#define BIT_SLAVE_ADDR      BIT00
#define BIT_COMMAND         BIT01
#define BIT_DATA            BIT02
#define BIT_COUNT           BIT03
#define BIT_WORD            BIT04
#define BIT_CONTROL         BIT05
#define BIT_PEC             BIT06
#define BIT_READ            BIT07
#define SMBUS_IO_READ_BIT   BIT00*/

#define HST_STS_BYTE_DONE    0x80

#define SMB_HST_STS          0x000
#ifndef HOST_STATUS_REGISTER
#define HOST_STATUS_REGISTER SMB_HST_STS
#endif
#ifndef HST_STS_HOST_BUSY
#define   HST_STS_HOST_BUSY     BIT00
#endif
#ifndef HST_STS_INTR
#define   HST_STS_INTR          BIT01
#endif
#ifndef HST_STS_DEV_ERR
#define   HST_STS_DEV_ERR       BIT02
#endif
#ifndef HST_STS_BUS_ERR
#define   HST_STS_BUS_ERR       BIT03
#endif
#ifndef HST_STS_FAILED
#define   HST_STS_FAILED        BIT04
#endif
#define   HST_STS_SMBALERT      BIT05
#ifndef HST_STS_INUSE
#define   HST_STS_INUSE         BIT06
#endif
#define   HST_STS_DS            BIT07
#define   HST_STS_ALL           0xFF


#define SMB_HST_CNT          0x002
#ifndef HOST_CONTROL_REGISTER
#define HOST_CONTROL_REGISTER SMB_HST_CNT
#endif
#ifndef HST_CNT_INTREN
#define   HST_CNT_INTREN        BIT00
#endif
#ifndef HST_CNT_KILL
#define   HST_CNT_KILL          BIT01
#endif
#ifndef SMBUS_B_SMB_CMD
#define   SMBUS_B_SMB_CMD       0x1C
#endif
#define    SMB_CMD_QUICK         0x00
#define    SMB_CMD_BYTE          0x04
#define    SMB_CMD_BYTE_DATA     0x08
#define    SMB_CMD_WORD_DATA     0x0C
#define    SMB_CMD_PROCESS_CALL  0x10
#define    SMB_CMD_BLOCK         0x14
#define    SMB_CMD_I2C_READ      0x18
#define    SMB_CMD_RESERVED      0x1c
#ifndef HST_CNT_START
#define   HST_CNT_START         BIT06
#endif
#ifndef HST_CNT_PEC_EN
#define   HST_CNT_PEC_EN        BIT07
#endif

#define SMB_HST_CMD          0x003
#ifndef HOST_COMMAND_REGISTER
#define HOST_COMMAND_REGISTER SMB_HST_CMD
#endif
#define SMB_HST_ADD          0x004
#ifndef XMIT_SLAVE_ADDRESS_REGISTER
#define XMIT_SLAVE_ADDRESS_REGISTER SMB_HST_ADD
#endif
#define SMB_HST_DAT_0        0x005
#ifndef HOST_DATA_0_REGISTER
#define HOST_DATA_0_REGISTER SMB_HST_DAT_0
#endif
#define SMB_HST_DAT_1        0x006
#ifndef HOST_DATA_1_REGISTER
#define HOST_DATA_1_REGISTER SMB_HST_DAT_1
#endif
#define SMB_HST_BLK_DAT      0x007
#ifndef HOST_BLOCK_DATA_BYTE_REGISTER
#define HOST_BLOCK_DATA_BYTE_REGISTER SMB_HST_BLK_DAT
#endif
#define SMB_PEC              0x008
#define SMB_RCV_SLVA         0x009
#define SMB_SLV_DAT          0x00A
#define SMB_AUX_STS          0x00C
#define    SMB_AUXS_CRCE        BIT00
#define    SMB_AUXS_STVO        BIT01
#define SMB_AUX_CTL          0x00D
#ifndef AUXILIARY_CONTROL_REGISTER
#define AUXILIARY_CONTROL_REGISTER SMB_AUX_CTL
#endif
#define   SMB_AUX_AAC           BIT00
#ifndef SMBUS_B_AAC
#define   SMBUS_B_AAC           SMB_AUX_AAC
#endif
#define   SMB_AUX_E32B          BIT01
#ifndef SMBUS_B_E32B
#define   SMBUS_B_E32B          SMB_AUX_E32B
#endif

#define SMB_SMLINK_PIN_CTL   0x00E
#define SMB_SMBUS_PIN_CTL    0x00F
#define SMB_SLV_STS          0x010
#define SMB_SLV_CMD          0x011
#define SMB_NTFY_DADDR       0x014
#define SMB_NTFY_DLOW        0x016
#define SMB_NTFY_DHIGH       0x017

#define BUS_TRIES            3          // How many times to retry on Bus Errors
#define SMBUS_NUM_RESERVED   21         // Number of device addresses that are


//
// SMBus Related equates
//
//#define MAX_SMBUS_DEVICES   50        // (P040109A)


#define SMBUS_DEVICE_DEFAULT_ADDRESS 0xC2 >> 1
#define PREPARE_TO_ARP               0x01
#define GET_UDID_DIRECTED            0x01
#define RESET_DEVICE_GENERAL         0x02
#define GET_UDID_GENERAL             0x03
#define ASSIGN_ADDRESS               0x04
#define GET_UDID_DATA_LENGTH         0x11 // 16 byte UDID + 1 byte address


#define SMBUSDXE_IOREAD8(Port) IoRead8((UINT16)(Port))
#define SMBUSDXE_IOWRITE8(Port,Data) IoWrite8((UINT16)(Port),(Data))

#if  SB_INSTALL_SMBUS_1_PPI
#define SB_NUM_PPI_DESC             (SB_MAX_SMBUS_CONTROLLER * 3)   //Install both SMBUS, SMBUS2 and Info PPI
#define SB_NUM_SMBUS_CONTROLLER     (SB_MAX_SMBUS_CONTROLLER * 2)   //Install both SMBUS & SMBUS2 PPI
#else
#define SB_NUM_PPI_DESC             (SB_MAX_SMBUS_CONTROLLER * 2)   //Install only SMBUS2 PPI
#define SB_NUM_SMBUS_CONTROLLER     SB_MAX_SMBUS_CONTROLLER         //Install both SMBUS2 PPI
#endif

//-----------------------------------------------------------------------
// 8259 Hardware definitions
//-----------------------------------------------------------------------
#define LEGACY_MODE_BASE_VECTOR_MASTER                    0x08
#define LEGACY_MODE_BASE_VECTOR_SLAVE                     0x10
#define LEGACY_8259_CONTROL_REGISTER_MASTER               0x20
#define LEGACY_8259_MASK_REGISTER_MASTER                  0x21
#define LEGACY_8259_CONTROL_REGISTER_SLAVE                0xA0
#define LEGACY_8259_MASK_REGISTER_SLAVE                   0xA1
#define LEGACY_8259_EDGE_LEVEL_TRIGGERED_REGISTER_MASTER  0x4D0
#define LEGACY_8259_EDGE_LEVEL_TRIGGERED_REGISTER_SLAVE   0x4D1
#define LEGACY_8259_EOI           0x20


//-----------------------------------------------------------------------
//
//-----------------------------------------------------------------------
#define NUM_BITS_IN_ACPI_TIMER  24  // Programmed to 24 not 32
// This is the maximum possible bits in the timer.  Currently this is 32 according to the spec
#define MAX_ACPI_TIMER_BITS     32

// Define all the Southbridge specific equates and structures in this file
#define REG_ACPI_BASE_ADDRESS   0x40
#define REG_ACPI_BASE_ADDRESS1  0x41
#define REG_ACPI_CNTL           0x44
#define ACPI_ENABLE             0x80
#define REG_GPIO_BASE_ADDRESS   0x48
#define REG_GPIO_BASE_ADDRESS1  0x49
#define REG_GPIO_CNTL           0x4C
#define GPIO_ENABLE             0x10




//----------------------------------------------------------------------------
//   Misc. Constants
//----------------------------------------------------------------------------
                                        // [EIP17262]>
#ifdef  USB_UHCI
#define MAX_UHCI    USB_UHCI
#else
#define MAX_UHCI    0
#endif

#ifdef  USB_EHCI
#define MAX_EHCI    USB_EHCI
#else
#define MAX_EHCI    0
#endif
                                        // <[EIP17262]

                                        // [EIP21454]>
#ifndef EHCI_RMH_PCI_DEVICES
#define EHCI_RMH_PCI_DEVICES {0xD0, 0x20}, {0xE8, 0x20}
#endif
                                        // <[EIP21454]

                                        // (P031109A)>
#define TIME_64ms   640000    // 64ms
#define TIME_32ms   320000    // 32ms
#define TIME_16ms   160000    // 16ms
#define TIME_1_5ms  15000     // 1.5ms
                                        // <(P031109A)

                                        // (P091009A)>
#define TIME_64s    64        // 64s
#define TIME_32s    32        // 32s
#define TIME_16s    16        // 16s
#define TIME_8s     8         // 8s
                                        // <(P091009A)

//----------------------------------------------------------------------------
//   Timer Constants
//----------------------------------------------------------------------------
#define SYSTEM_TIMER_IRQ 0
//TODO: Now using Legacy 8259 protocol to initialize. Remove this line later.
//#define SYSTEM_TIMER_INTR_VECTOR (MASTER_INTERRUPT_BASE + SYSTEM_TIMER_IRQ)

// Cpu I/O space defines
#define TIMER_CTRL     0x43
#define TIMER_0_COUNT  0x40

// Timer Period
#define TIMER_TICK     838 //ns

// default duration is 0xffff ticks
//#define DEFAULT_TICK_DURATION ((65536 * 838 + 50) / 100)          // [ EIP38894 ]
#define DEFAULT_TICK_DURATION ((65535 * 838 + 50) / 100)
#define MAX_TICK_DURATION DEFAULT_TICK_DURATION

//8259 PIC defines
#define ICW1            0x11    //Slave exists and ICW4 required.
#define ICW3_M          1 << 2  //IRQ 2 connects to slave
#define ICW3_S          2       //IRQ 2 connects to master
#define ICW4            1       //Bit 4 Normal Nested Mode
//Bit 3 Non-buffered Mode
//Bit 2 Unused with non-buffered mode
//Bit 1 Set manual EOI instead of automatic
//Bit 0 8086/8088 mode

#define OCW1_M          0xff    //Master Mask
#define OCW1_S          0xff    //Slave Mask

#define EOI_COMMAND     0x20    //EOI Command

#define INTERRUPTS_TRIGGER_REG    0x4d0   //Trigger for Interrupts (Edge or Level).
#define INTERRUPTS_EDGE_TRIGGER   0       //Set all interrupts at edge level.

#define VOLUME_LEVEL              0x8000

                                        // (P090109B)>
#if defined ME_SUPPORT && ME_SUPPORT == 1
#if THERMAL_REPORTING_SUPPORT
#ifndef NB_NUMBER_OF_MEM_MODULE
#define NB_NUMBER_OF_MEM_MODULE 4
#endif
#endif
#endif

//----------------------------------------------------------------------------
// SB Chipset Reset Type
//----------------------------------------------------------------------------

#define AMI_CSP_RESET_GUID \
 {0x308DD02C, 0x092B, 0x4123, 0xA2, 0xAF, 0x3E, 0xF4, 0x44, 0x0A, 0x6B, 0x4A}

#define RESET_PORT        0x0CF9
#define CLEAR_RESET_BITS  0x0F9
#define COLD_RESET        0x02 // Set bit 1 for cold reset
#define RST_CPU           0x04 // Setting this bit triggers a reset of the CPU

typedef enum _SB_RESET_TYPE
{
    HardReset = 0,
    SoftReset,
    ShutDown,                           // (P112509A)
    FullReset,
    GlobalReset                         // (P112509A)
} SB_RESET_TYPE;

                                        // (P040809A)>
typedef struct {
  UINT32 BDFReg;
  UINT32 DevSSID;
}SB_PCI_DEVICE_SSID;
                                        // <(P040809A)

                                        // (P010609A)>
//-----------------------------------------------------------------------
// SB : Setup Details
//-----------------------------------------------------------------------

// {A09A3266-0D9D-476a-B8EE-0C226BE1DAAE}
#define AMI_CSP_SETUP_GUID \
 {0xa09a3266, 0xd9d, 0x476a, 0xb8, 0xee, 0xc, 0x22, 0x6b, 0xe1, 0xda, 0xae}

#define SB_SETUP_VARIABLE L"SBSetup"

// {D4CA32B3-B1FE-4ff7-B073-60EDBB1619AF}
#define AMI_SB_CONFIGURATION_GUID \
 {0xd4ca32b3, 0xb1fe, 0x4ff7, 0xb0, 0x73, 0x60, 0xed, 0xbb, 0x16, 0x19, 0xaf}

typedef struct _SB_SETUP_CONFIGURATION
{
    BOOLEAN SetupDataValidHob; // 1=Valid; 0=Invalid
    UINT8 Smbus;
    UINT8 SBGbELan;
    UINT8 SBGbELanLock;
    UINT8 GbEWakeOnLanS5;
    UINT8 AfterG3;
    UINT8 SlpS4AsseEn;                  // (P121409B)
    UINT8 SlpS4AsseW;                   // (P121409B)
    UINT8 Azalia;
    UINT8 HdmiCodec;                    // (P042009A)
#if HPET_SUPPORT
    UINT8 Hpet;
#endif
#if RC_PORT_0
    UINT8 PciEPort1;
    UINT8 PcieHp1;
#if RC_PORT_1
    UINT8 PciEPort2;
    UINT8 PcieHp2;
#endif
#if RC_PORT_2
    UINT8 PciEPort3;
    UINT8 PcieHp3;
#endif
#if RC_PORT_3
    UINT8 PciEPort4;
    UINT8 PcieHp4;
#endif
#if RC_PORT_4
    UINT8 PciEPort5;
    UINT8 PcieHp5;
#endif
#if RC_PORT_5
    UINT8 PciEPort6;
    UINT8 PcieHp6;
#endif
#if RC_PORT_6
    UINT8 PciEPort7;
    UINT8 PcieHp7;
#endif
#if RC_PORT_7
    UINT8 PciEPort8;
    UINT8 PcieHp8;
#endif
    UINT8 GbePciePortNum;
#endif
    UINT8 Usb;
    UINT8 Ehci1;
    UINT8 Ehci2;
    UINT8 Rmh;                          // (P020309E)
    UINT8 Uhci1;
    UINT8 Uhci2;
    UINT8 Uhci3;
    UINT8 Uhci4;
    UINT8 Uhci5;
    UINT8 Uhci6;
    UINT8 Uhci7;
    UINT8 UsbPort0;                     // (P021709A)>
    UINT8 UsbPort1;
    UINT8 UsbPort2;
    UINT8 UsbPort3;
    UINT8 UsbPort4;
    UINT8 UsbPort5;
    UINT8 UsbPort6;
    UINT8 UsbPort7;
    UINT8 UsbPort8;
    UINT8 UsbPort9;
    UINT8 UsbPort10;
    UINT8 UsbPort11;
    UINT8 UsbPort12;
    UINT8 UsbPort13;                    // <(P021709A)
    UINT8 SataMode;
    UINT8 SataController0;
    UINT8 SataController1;
    UINT8 SataSSS;
    UINT8 ESataPort0;
    UINT8 ESataPort1;
    UINT8 ESataPort2;
    UINT8 ESataPort3;
    UINT8 ESataPort4;
    UINT8 ESataPort5;
    UINT8 SataHpP0;
    UINT8 SataHpP1;
    UINT8 SataHpP2;
    UINT8 SataHpP3;
    UINT8 SataHpP4;
    UINT8 SataHpP5;
                                        // (P090109B)>
#if THERMAL_REPORTING_SUPPORT
    UINT8 MCHTempReadEnable;
    UINT8 PCHTempReadEnable;
    UINT8 CPUEnergyReadEnable;
    UINT8 ThermalDataReportingEnable;
#if IBEX_PEAK_SKU == 1
    UINT16 PPEC;
    UINT16 PTL;
    UINT16 MMGPC;
    UINT16 MPPC;
    UINT16 MPCPC;
#endif
    UINT8 PTA;
    UINT8 PTA_OFFSET;
    UINT8 MGTA;
    UINT8 MGTA_OFFSET;
    UINT8 CPUTempReadEnable;
    UINT8 AlertEnableLock;
    UINT8 CPUAlert;
    UINT8 MCHAlert;
    UINT8 PCHAlert;
    UINT8 DIMMAlert;
    UINT8 TrEnabled;
    UINT8 SMBusECMsgLen;
    UINT8 SMBusECMsgPEC;
    UINT8 TrSmbusConfig;
    UINT8 TsOnDimm0;
    UINT8 TsOnDimm1;
    UINT8 TsOnDimm2;
    UINT8 TsOnDimm3;
#endif
                                        // <(P090109B)
} SB_SETUP_CONFIGURATION;

typedef struct
{
    EFI_HOB_GUID_TYPE  EfiHobGuidType;
    SB_SETUP_CONFIGURATION  Configuration;
} SB_CONFIGURATION_HOB;
                                        // <(P010609A)

                                        // (P090109B)>
#if THERMAL_REPORTING_SUPPORT
typedef struct _TR_BIOS_PARAM
{
  UINT32  HeciHeader;
  UINT8   Command;
  UINT8   PollingTimeout;
  UINT8   SMBusECMsgLen;
  UINT8   SMBusECMsgPEC;
  UINT8   DimmNumber;
  UINT8   SmbusAddress0;
  UINT8   SmbusAddress1;
  UINT8   SmbusAddress2;
  UINT8   SmbusAddress3;
} TR_BIOS_PARAM;
#endif
                                        // <(P090109B)

void GetSbSetupData (                   // [ EIP53941 ]
    IN VOID                 *Service,
    IN OUT SB_SETUP_CONFIGURATION   *SbSetupData,
    IN BOOLEAN              Pei
);

// {685113C7-526D-4b30-9907-45EEFF6C5FA8}
#define SMM_SB_INFO_TABLE_GUID \
    {0x685113c7, 0x526d, 0x4b30, 0x99, 0x7, 0x45, 0xee, 0xff, 0x6c, 0x5f, 0xa8}

                                        // (EIP52570A)>
                                        // (I020911B)>
#pragma pack(push, 1)
typedef struct 
{
    UINT32 TcoClearMask;
    BOOLEAN ClearBiosWriteEn;
#if defined DISABLED_RESTORE_NMI && DISABLED_RESTORE_NMI == 1
    BOOLEAN DisableRestoreNmi;
#endif //defined DISABLED_RESTORE_NMI && DISABLED_RESTORE_NMI == 1
} SMM_SB_INFO_TABLE;
#pragma pack(pop)

#endif  // #ifndef  _SB_H_

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

