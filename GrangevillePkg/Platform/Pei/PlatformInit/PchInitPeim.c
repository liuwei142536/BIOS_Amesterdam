/**
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
**/
/**

Copyright (c)  2012 - 2016 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


  @file 
  
    PchInitPeim.c
   
  @brief 
    Do Early PCH initialization.

--*/
#include "PlatformEarlyInit.h"
#include <Library/PciExpressLib.h>
#include <Library/PchPlatformPolicyInitPeiLib.h>
#include <Iio/IioRegs.h>
#include <Ppi/PchReset.h>


#define TIMER1_CONTROL_PORT 0x43
#define TIMER1_COUNT_PORT   0x41
#define LOAD_COUNTER1_LSB   0x54
#define COUNTER1_COUNT      0x12
//
// Reset Generator I/O Port
//
#define RESET_GENERATOR_PORT  0xCF9

//-----------------------------------------------------------------------;
// PCH: Chipset Configuration Register Equates
//-----------------------------------------------------------------------;
#define ICH_RCRB_IRQ0                       0
#define ICH_RCRB_IRQA                       1
#define ICH_RCRB_IRQB                       2
#define ICH_RCRB_IRQC                       3
#define ICH_RCRB_IRQD                       4
#define ICH_RCRB_PIRQA                      0
#define ICH_RCRB_PIRQB                      1
#define ICH_RCRB_PIRQC                      2
#define ICH_RCRB_PIRQD                      3
#define ICH_RCRB_PIRQE                      4
#define ICH_RCRB_PIRQF                      5
#define ICH_RCRB_PIRQG                      6
#define ICH_RCRB_PIRQH                      7

#define PchPciDeviceMmBase(Bus, Device, Function) \
    ( \
      (UINTN) PcdGet64 (PcdPciExpressBaseAddress) + (UINTN) (Bus << 20) + (UINTN) (Device << 15) + (UINTN) \
        (Function << 12) \
    )

typedef struct _SOFTSTRAP_FIXUP_ENTRY
{
  UINT8  BoardId;
  UINT8  SoftStrapNumber;
  UINT8  BitfieldOffset;
  UINT8  BitfieldLength;
  UINT32 Value; 
} SOFTSTRAP_FIXUP_ENTRY;



STATIC PCH_GPIO_INDEX_DATA  mPchGpioInitData_BeverlyCove[] = {
  { BEVERLYCOVE_GPIO_USE_SEL_VAL,    R_PCH_GPIO_USE_SEL  },
  { BEVERLYCOVE_GPIO_LVL_VAL,        R_PCH_GPIO_LVL      },
  { BEVERLYCOVE_GPIO_IO_SEL_VAL,     R_PCH_GPIO_IO_SEL   },
  { BEVERLYCOVE_GPIO_USE_SEL2_VAL,   R_PCH_GPIO_USE_SEL2 },
  { BEVERLYCOVE_GPIO_LVL2_VAL,       R_PCH_GPIO_LVL2     },
  { BEVERLYCOVE_GPIO_IO_SEL2_VAL,    R_PCH_GPIO_IO_SEL2  },
  { BEVERLYCOVE_GPIO_USE_SEL3_VAL,   R_PCH_GPIO_USE_SEL3 },
  { BEVERLYCOVE_GPIO_LVL3_VAL,       R_PCH_GPIO_LVL3     },
  { BEVERLYCOVE_GPIO_IO_SEL3_VAL,    R_PCH_GPIO_IO_SEL3  },
  { BEVERLYCOVE_GPIO_INV_VAL,        R_PCH_GPIO_GPI_INV  },
  { BEVERLYCOVE_GPIO_BLINK_VAL,      R_PCH_GPIO_BLINK    },
  { 0,                          		(UINT8) 0xFF     }
};

STATIC PCH_GPIO_INDEX_DATA  mPchGpioInitData_TrabucoCanyon[] = {
  { TRABUCOCANYON_GPIO_USE_SEL_VAL,    R_PCH_GPIO_USE_SEL  },
  { TRABUCOCANYON_GPIO_LVL_VAL,        R_PCH_GPIO_LVL      },
  { TRABUCOCANYON_GPIO_IO_SEL_VAL,     R_PCH_GPIO_IO_SEL   },
  { TRABUCOCANYON_GPIO_USE_SEL2_VAL,   R_PCH_GPIO_USE_SEL2 },
  { TRABUCOCANYON_GPIO_LVL2_VAL,       R_PCH_GPIO_LVL2     },
  { TRABUCOCANYON_GPIO_IO_SEL2_VAL,    R_PCH_GPIO_IO_SEL2  },
  { TRABUCOCANYON_GPIO_USE_SEL3_VAL,   R_PCH_GPIO_USE_SEL3 },
  { TRABUCOCANYON_GPIO_LVL3_VAL,       R_PCH_GPIO_LVL3     },
  { TRABUCOCANYON_GPIO_IO_SEL3_VAL,    R_PCH_GPIO_IO_SEL3  },
  { TRABUCOCANYON_GPIO_INV_VAL,        R_PCH_GPIO_GPI_INV  },
  { TRABUCOCANYON_GPIO_BLINK_VAL,      R_PCH_GPIO_BLINK    },
  { 0,                          		(UINT8) 0xFF     }
};

STATIC PCH_GPIO_INDEX_DATA  mPchGpioInitData_CamelbackMountain[] = {
  { CAMELBACKMOUNTAIN_GPIO_USE_SEL_VAL,    R_PCH_GPIO_USE_SEL  },
  { CAMELBACKMOUNTAIN_GPIO_LVL_VAL,        R_PCH_GPIO_LVL      },
  { CAMELBACKMOUNTAIN_GPIO_IO_SEL_VAL,     R_PCH_GPIO_IO_SEL   },
  { CAMELBACKMOUNTAIN_GPIO_USE_SEL2_VAL,   R_PCH_GPIO_USE_SEL2 },
  { CAMELBACKMOUNTAIN_GPIO_LVL2_VAL,       R_PCH_GPIO_LVL2     },
  { CAMELBACKMOUNTAIN_GPIO_IO_SEL2_VAL,    R_PCH_GPIO_IO_SEL2  },
  { CAMELBACKMOUNTAIN_GPIO_USE_SEL3_VAL,   R_PCH_GPIO_USE_SEL3 },
  { CAMELBACKMOUNTAIN_GPIO_LVL3_VAL,       R_PCH_GPIO_LVL3     },
  { CAMELBACKMOUNTAIN_GPIO_IO_SEL3_VAL,    R_PCH_GPIO_IO_SEL3  },
  { CAMELBACKMOUNTAIN_GPIO_INV_VAL,        R_PCH_GPIO_GPI_INV  },
  { CAMELBACKMOUNTAIN_GPIO_BLINK_VAL,      R_PCH_GPIO_BLINK    },
  { 0,                          		(UINT8) 0xFF     }
};

STATIC PCH_GPIO_INDEX_DATA  mPchGpioInitData_Tlaquepaque[] = {
  { TLAQUEPAQUE_GPIO_USE_SEL_VAL,    R_PCH_GPIO_USE_SEL  },
  { TLAQUEPAQUE_GPIO_LVL_VAL,        R_PCH_GPIO_LVL      },
  { TLAQUEPAQUE_GPIO_IO_SEL_VAL,     R_PCH_GPIO_IO_SEL   },
  { TLAQUEPAQUE_GPIO_USE_SEL2_VAL,   R_PCH_GPIO_USE_SEL2 },
  { TLAQUEPAQUE_GPIO_LVL2_VAL,       R_PCH_GPIO_LVL2     },
  { TLAQUEPAQUE_GPIO_IO_SEL2_VAL,    R_PCH_GPIO_IO_SEL2  },
  { TLAQUEPAQUE_GPIO_USE_SEL3_VAL,   R_PCH_GPIO_USE_SEL3 },
  { TLAQUEPAQUE_GPIO_LVL3_VAL,       R_PCH_GPIO_LVL3     },
  { TLAQUEPAQUE_GPIO_IO_SEL3_VAL,    R_PCH_GPIO_IO_SEL3  },
  { TLAQUEPAQUE_GPIO_INV_VAL,        R_PCH_GPIO_GPI_INV  },
  { TLAQUEPAQUE_GPIO_BLINK_VAL,      R_PCH_GPIO_BLINK    },
  { 0,                          		(UINT8) 0xFF     }
};

STATIC PCH_GPIO_INDEX_DATA  mPchGpioInitData_PiestewaPeak[] = {
  { PiestewaPeak_GPIO_USE_SEL_VAL,    R_PCH_GPIO_USE_SEL  },
  { PiestewaPeak_GPIO_LVL_VAL,        R_PCH_GPIO_LVL      },
  { PiestewaPeak_GPIO_IO_SEL_VAL,     R_PCH_GPIO_IO_SEL   },
  { PiestewaPeak_GPIO_USE_SEL2_VAL,   R_PCH_GPIO_USE_SEL2 },
  { PiestewaPeak_GPIO_LVL2_VAL,       R_PCH_GPIO_LVL2     },
  { PiestewaPeak_GPIO_IO_SEL2_VAL,    R_PCH_GPIO_IO_SEL2  },
  { PiestewaPeak_GPIO_USE_SEL3_VAL,   R_PCH_GPIO_USE_SEL3 },
  { PiestewaPeak_GPIO_LVL3_VAL,       R_PCH_GPIO_LVL3     },
  { PiestewaPeak_GPIO_IO_SEL3_VAL,    R_PCH_GPIO_IO_SEL3  },
  { PiestewaPeak_GPIO_INV_VAL,        R_PCH_GPIO_GPI_INV  },
  { PiestewaPeak_GPIO_BLINK_VAL,      R_PCH_GPIO_BLINK    },
  { 0,                          		(UINT8) 0xFF     }
};

static const SOFTSTRAP_FIXUP_ENTRY SoftstrapFixupTable[] = {


  {TypeBeverlyCoveCRB,            9,  0,  2, 0x02}, //PCIEPCS1
  {TypeBeverlyCoveCRB,            9,  2,  2, 0x03}, //PCIEPCS2
  {TypeBeverlyCoveCRB,            9,  4,  2, 0x00}, //PCIELR2/PCIELR1
  {TypeBeverlyCoveCRB,            9,  6,  1, 0x00}, //DMILR
  {TypeBeverlyCoveCRB,            9,  8,  3, 0x01}, //DMIPCIEPORTSEL
  {TypeBeverlyCoveCRB,            9,  11, 1, 0x01}, //GB_PCIE_EN
  {TypeBeverlyCoveCRB,            9,  18, 2, 0x00}, //PCIEP1MODE
  {TypeBeverlyCoveCRB,            9,  20, 2, 0x00}, //PCIEP2MODE

  {TypeBeverlyCoveSODimmCRB,      9,  0,  2, 0x02}, //PCIEPCS1
  {TypeBeverlyCoveSODimmCRB,      9,  2,  2, 0x03}, //PCIEPCS2
  {TypeBeverlyCoveSODimmCRB,      9,  4,  2, 0x00}, //PCIELR2/PCIELR1
  {TypeBeverlyCoveSODimmCRB,      9,  6,  1, 0x00}, //DMILR
  {TypeBeverlyCoveSODimmCRB,      9,  8,  3, 0x01}, //DMIPCIEPORTSEL
  {TypeBeverlyCoveSODimmCRB,      9,  11, 1, 0x01}, //GB_PCIE_EN
  {TypeBeverlyCoveSODimmCRB,      9,  18, 2, 0x00}, //PCIEP1MODE
  {TypeBeverlyCoveSODimmCRB,      9,  20, 2, 0x00}, //PCIEP2MODE

  {TypeTrabucoCanyon,             9,  0,  2, 0x03}, //PCIEPCS1
  {TypeTrabucoCanyon,             9,  2,  2, 0x00}, //PCIEPCS2
  {TypeTrabucoCanyon,             9,  4,  2, 0x00}, //PCIELR2/PCIELR1
  {TypeTrabucoCanyon,             9,  6,  1, 0x00}, //DMILR
  {TypeTrabucoCanyon,             9,  8,  3, 0x01}, //DMIPCIEPORTSEL
  {TypeTrabucoCanyon,             9,  11, 1, 0x01}, //GB_PCIE_EN
  {TypeTrabucoCanyon,             9,  18, 2, 0x00}, //PCIEP1MODE
  {TypeTrabucoCanyon,             9,  20, 2, 0x00}, //PCIEP2MODE

  {TypeCamelbackMountain,         9,  0,  2, 0x03}, //PCIEPCS1
  {TypeCamelbackMountain,         9,  2,  2, 0x00}, //PCIEPCS2
  {TypeCamelbackMountain,         9,  4,  2, 0x00}, //PCIELR2/PCIELR1
  {TypeCamelbackMountain,         9,  6,  1, 0x00}, //DMILR
  {TypeCamelbackMountain,         9,  8,  3, 0x01}, //DMIPCIEPORTSEL
  {TypeCamelbackMountain,         9,  11, 1, 0x01}, //GB_PCIE_EN
  {TypeCamelbackMountain,         9,  18, 2, 0x00}, //PCIEP1MODE
  {TypeCamelbackMountain,         9,  20, 2, 0x00}, //PCIEP2MODE

  {TypeEchoCanyon,				  9,  0,  2, 0x03}, //PCIEPCS1
  {TypeEchoCanyon,				  9,  2,  2, 0x00}, //PCIEPCS2
  {TypeEchoCanyon,				  9,  4,  2, 0x00}, //PCIELR2/PCIELR1
  {TypeEchoCanyon,				  9,  6,  1, 0x00}, //DMILR
  {TypeEchoCanyon,				  9,  8,  3, 0x01}, //DMIPCIEPORTSEL
  {TypeEchoCanyon,				  9,  11, 1, 0x01}, //GB_PCIE_EN
  {TypeEchoCanyon,				  9,  18, 2, 0x00}, //PCIEP1MODE
  {TypeEchoCanyon,				  9,  20, 2, 0x00}, //PCIEP2MODE

  {TypeTlaquepaque,               9,  0,  2, 0x03}, //PCIEPCS1
  {TypeTlaquepaque,               9,  2,  2, 0x00}, //PCIEPCS2
  {TypeTlaquepaque,               9,  4,  2, 0x00}, //PCIELR2/PCIELR1
  {TypeTlaquepaque,               9,  6,  1, 0x00}, //DMILR
  {TypeTlaquepaque,               9,  8,  3, 0x01}, //DMIPCIEPORTSEL
  {TypeTlaquepaque,               9,  11, 1, 0x01}, //GB_PCIE_EN
  {TypeTlaquepaque,               9,  18, 2, 0x00}, //PCIEP1MODE
  {TypeTlaquepaque,               9,  20, 2, 0x00}, //PCIEP2MODE

  {TypePiestewaPeak,             9,  0,  2, 0x03}, //PCIEPCS1
  {TypePiestewaPeak,             9,  2,  2, 0x00}, //PCIEPCS2
  {TypePiestewaPeak,             9,  4,  2, 0x00}, //PCIELR2/PCIELR1
  {TypePiestewaPeak,             9,  6,  1, 0x00}, //DMILR
  {TypePiestewaPeak,             9,  8,  3, 0x01}, //DMIPCIEPORTSEL
  {TypePiestewaPeak,             9,  11, 1, 0x01}, //GB_PCIE_EN
  {TypePiestewaPeak,             9,  18, 2, 0x00}, //PCIEP1MODE
  {TypePiestewaPeak,             9,  20, 2, 0x00}, //PCIEP2MODE

  {TypePlatformUnknown,           0,   0, 0, 0x00}  // END OF LIST

};

VOID
PchGpioInit (
  IN EFI_PEI_SERVICES            **PeiServices, 
  IN UINT16                      PlatformType,
  IN SYSTEM_CONFIGURATION        *SystemConfiguration
  )
{
  PCH_GPIO_INDEX_DATA         *mPchGpioInitData = NULL;
  UINT32                      GpiRoute;
  UINT32                      DataSel32;
  UINT32                      DataLvl32;
  UINT16                      Data16;
    
  switch (PlatformType) {

    case TypeBeverlyCoveCRB:
    case TypeBeverlyCoveSODimmCRB:
      mPchGpioInitData = mPchGpioInitData_BeverlyCove;	
      break;

    case TypeTrabucoCanyon:
      mPchGpioInitData = mPchGpioInitData_TrabucoCanyon;
      break;

    case TypeCamelbackMountain:
      mPchGpioInitData = mPchGpioInitData_CamelbackMountain;
      break;
	
	case TypeEchoCanyon:
      mPchGpioInitData = mPchGpioInitData_CamelbackMountain;
      break;

    case TypeTlaquepaque:
      mPchGpioInitData = mPchGpioInitData_Tlaquepaque;
      break;

    case TypePiestewaPeak:
      mPchGpioInitData = mPchGpioInitData_PiestewaPeak;
      break;

    default:
      mPchGpioInitData = mPchGpioInitData_BeverlyCove;
      break;
  }  
  
  //
  // Set up the PCH GPIO configuration
  //
  if (mPchGpioInitData) {  
    while (mPchGpioInitData->Register != 0xFF) {
      IoWrite32 (
        mPchGpioInitData->Register + PCH_GPIO_BASE_ADDRESS,
        mPchGpioInitData->Value
        );
      mPchGpioInitData++;
    }
  }

  //
  // Program GPI_ROUT (0:1F:0:B8) according to board schematics.
  //

  // GPIO 0  IRQ_SSB_SCI_WHEA_N    generates SCI
  // GPIO 2  FM_ERR0_N             generates SCI
  // GPIO 3  FM_ERR1_N             generates SCI
  // GPIO 4  FM_IBMC_SSB_SMI_LPC_N generates SMI -- used as PROCHOT for GNV
  // GPIO 5  IRQ_IBMC_SSB_NMI      generates NMI -- used as CATERR for GNV
  // GPIO 13 FM_IBMC_SSB_SCI_LPC_N generates SCI -- Not used for GNV

  GpiRoute = PciRead32(PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0, R_PCH_LPC_GPI_ROUT));

  GpiRoute &= ~(B_PCH_LPC_GPI_ROUT_0 | B_PCH_LPC_GPI_ROUT_2 | B_PCH_LPC_GPI_ROUT_3);
  //GPIO Route
  // 00 No efect
  // 01 SMI# if the corresponding ALT_GPI_SMI_EN
  // 10 SCI if GPE0_EN
  // 11 NMI 
  GpiRoute |=  ((2 << 0*2) | (2 << 2*2) | (2 << 3*2));
//  PchLpcPciCfg32(R_PCH_LPC_GPI_ROUT) = GpiRoute;     //Write value of GPIO ROUT
  PciWrite32(PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0, R_PCH_LPC_GPI_ROUT), GpiRoute);
  
  // Clear NMI
  Data16  = IoRead16(PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_NMI_STS);      
  IoWrite16 (PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_NMI_STS, Data16);

  // Clear SMI
  Data16 = IoRead16(PM_BASE_ADDRESS + R_ACPI_SMI_STS);         
  IoWrite16 (PM_BASE_ADDRESS + R_ACPI_SMI_STS, Data16);

  //
  // Set for USB Wakeup
  //    
  DataSel32 = IoRead32(PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_IO_SEL3); //GPIO72
  DataLvl32 = IoRead32(PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_LVL3); //GPIO72
//  if(SystemConfiguration->PchUsbWake == 1) {
    DataSel32 &= (~BIT8);
    DataLvl32 &=(~BIT8);
//  } else {
//    DataSel32 |= BIT8;
//    DataLvl32 |=BIT8;   
//  }
  IoWrite32(PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_IO_SEL3, DataSel32);
  IoWrite32(PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_LVL3, DataLvl32);

  
  Data16 = IoRead16(PM_BASE_ADDRESS + R_PCH_LPTH_ALT_GP_SMI_EN); 
  if(Data16 !=0 )
  {
    DEBUG((DEBUG_ERROR,"R_PCH_LPTH_ALT_GP_SMI_EN (0x438) is %x, clear it.\n", Data16));
    Data16 = 0;
    IoWrite16 (PM_BASE_ADDRESS + R_PCH_LPTH_ALT_GP_SMI_EN, Data16);
  }
}

VOID
CheckPowerOffNow (
  VOID
  )
/*++

Routine Description:

  Turn off system if needed.

Arguments:

  PeiServices Pointer to PEI Services
  CpuIo       Pointer to CPU I/O Protocol
  
Returns:

  None.
  
--*/
{
  UINT16  Pm1Sts;

  //
  // Read and check the ACPI registers
  //
  Pm1Sts = IoRead16 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_STS);
  if ((Pm1Sts & B_PCH_ACPI_PM1_STS_PWRBTN) == B_PCH_ACPI_PM1_STS_PWRBTN) {
    IoWrite16 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_STS, B_PCH_ACPI_PM1_STS_PWRBTN);
    IoWrite16 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_CNT, V_PCH_ACPI_PM1_CNT_S5);
    IoWrite16 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_CNT, V_PCH_ACPI_PM1_CNT_S5 + B_PCH_ACPI_PM1_CNT_SLP_EN);
  }
}

VOID
ClearPowerState (
  IN SYSTEM_CONFIGURATION        *SystemConfiguration
  )
{
  //
  // Check for PowerState option for AC power loss and program the chipset
  //
  if (SystemConfiguration->PowerState == 1) {
    //
    // Program Chipset to be off after AC power loss
    //
    PciOr8 (
        PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0,R_PCH_LPC_GEN_PMCON_3),
        B_PCH_LPC_GEN_PMCON_AFTERG3_EN
        );
  } else {
    //
    // Program Chipset to power On after AC power loss
    //
    PciAnd8 (
        PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0,R_PCH_LPC_GEN_PMCON_3),
      (UINT8)~B_PCH_LPC_GEN_PMCON_AFTERG3_EN
        );
  }
  //
  // Clear PWROK (Set to Clear)
  //
  PciOr8 (
      PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0,R_PCH_LPC_GEN_PMCON_2),
      B_PCH_LPC_GEN_PMCON_PWROK_FLR
      );

  //
  // Clear Power Failure Bit (Set to Clear)
  //
  PciOr8 (
      PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0,R_PCH_LPC_GEN_PMCON_3),
      B_PCH_LPC_GEN_PMCON_PWR_FLR
      );

  //
  // Clear the GPE and PM enable
  //
  IoWrite16 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_EN, 0);
  IoWrite32 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_GPE0a_EN, 0);
  IoWrite32 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_GPE0a_EN + 4, 0);

  //
  // Halt the TCO timer
  //
  IoOr16 (PCH_ACPI_BASE_ADDRESS + PCH_TCO_BASE + R_PCH_TCO1_CNT, B_PCH_TCO_CNT_TMR_HLT);

  //
  // Clear the Second TO status bit
  //
  IoOr8 (PCH_ACPI_BASE_ADDRESS + PCH_TCO_BASE + R_PCH_TCO2_STS, B_PCH_TCO2_STS_SECOND_TO);

  //
  // Clear the global platform reset bit
  //
  PciAnd32 (
    PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0, R_PCH_LPC_PMIR),
    (UINT32)~B_PCH_LPC_PMIR_CF9GR
    );

} // ClearPowerState()

VOID
ClearSmiAndWake (
  VOID
  )
/*++

Routine Description:

  Clear any SMI status or wake status left over from boot.
  
Arguments:
  
Returns:

  None.

--*/
{
  UINT16  Pm1Sts;
  UINT32  Pm1Cnt;
  UINT32  Gpe0Sts;
  UINT32  Gpe1Sts;
  UINT32  SmiSts;

  //
  // Read the ACPI registers
  //
  Pm1Cnt  = IoRead32 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_CNT);
  Gpe0Sts = IoRead32 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_GPE0a_STS);
  Gpe1Sts = IoRead32 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_GPE0a_STS + 4);
  SmiSts  = IoRead32 (PCH_ACPI_BASE_ADDRESS + R_PCH_SMI_STS);

  //
  // Clear any SMI or wake state from the boot
  //
  Pm1Sts = (B_PCH_ACPI_PM1_STS_PRBTNOR | B_PCH_ACPI_PM1_STS_PWRBTN);
  Gpe0Sts |=
    (
      B_PCH_ACPI_GPE0a_STS_GPInn |
      B_PCH_ACPI_GPE0a_STS_PME_B0 |
      B_PCH_ACPI_GPE0a_STS_PME |
      B_PCH_ACPI_GPE0a_STS_BATLOW |
      B_PCH_ACPI_GPE0a_STS_PCI_EXP |
      B_PCH_ACPI_GPE0a_STS_RI |
      B_PCH_ACPI_GPE0a_STS_SMB_WAK |
      B_PCH_ACPI_GPE0a_STS_TC0SCI |
      B_PCH_ACPI_GPE0a_STS_HOT_PLUG 
    );

  Gpe1Sts |=
    (
      BIT0  //B_USB6_STS
    );

  SmiSts |=
    (
      B_PCH_SMI_STS_SMBUS |
      B_PCH_SMI_STS_PERIODIC |
      B_PCH_SMI_STS_TCO |
      B_PCH_SMI_STS_MCSMI |
      B_PCH_SMI_STS_SWSMI_TMR |
      B_PCH_SMI_STS_APM |
      B_PCH_SMI_STS_ON_SLP_EN |
      B_PCH_SMI_STS_BIOS
    );

  //
  // Write them back
  //
  if (IsSoftSdvPlatform() == FALSE) {
    IoWrite16 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_STS, Pm1Sts);
  }
  IoWrite32 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_CNT, Pm1Cnt);
  IoWrite32 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_GPE0a_STS, Gpe0Sts);
  IoWrite32 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_GPE0a_STS + 4, Gpe1Sts);
  IoWrite32 (PCH_ACPI_BASE_ADDRESS + R_PCH_SMI_STS, SmiSts);
}

EFI_STATUS
EFIAPI
PchResetPlatform (
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
/*++

Routine Description:

  Provide hard reset PPI service.
  To generate full hard reset.

Arguments:

  PeiServices       General purpose services available to every PEIM.
  
Returns:
  
  Not return        System reset occured.
  EFI_DEVICE_ERROR  Device error, could not reset the system.

--*/
{
  PCH_RESET_PPI   *PchResetPpi;
  EFI_STATUS      Status;


  Status = (*PeiServices)->LocatePpi (
                            PeiServices,
                            &gPchResetPpiGuid,
                            0,
                            NULL,
                            (VOID **)&PchResetPpi
                            );

  ASSERT_EFI_ERROR (Status);

  PchResetPpi->Reset (PchResetPpi, PowerCycleReset);

  // Aptio V Server override start: The reset takes place in a while after the assert is seen. It is only a time lag for the reset to happen
  // on the platform. Hence, commenting out the assert.
  //
  // System reset occured, should never reach at this line.
  //
  //ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
  // Aptio V Server override end

  return EFI_DEVICE_ERROR;
}


  VOID
PchBaseInit (
  )
{
  //
  // Program SIO Base Address, 16 byte
  //
  PciWrite32(PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0, R_PCH_LPC_GEN1_DEC), \
      (((~SIO_BASE_MASK) & 0xFC) << 16) | (SIO_BASE_ADDRESS & 0xFFFC) | BIT0);

  //
  // Program LPC BMC KCS base address, 16 byte
  //
  PciWrite32(PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0, R_PCH_LPC_GEN2_DEC), \
      (((~BMC_KCS_BASE_MASK) & 0xFC) << 16) | (BMC_KCS_BASE_ADDRESS & 0xFFFC) | BIT0);
}

VOID
PchLpcInit (
  IN SYSTEM_CONFIGURATION        *SystemConfiguration
  )
{
  EFI_BOOT_MODE BootMode;

  PeiServicesGetBootMode (&BootMode);

  if ((BootMode != BOOT_ON_S3_RESUME)) {
    //
    // Clear all pending SMI. On S3 clear power button enable so it wll not generate an SMI
    //
    ClearSmiAndWake ();
  }

  ClearPowerState (SystemConfiguration);

  //
  // Flash Size 4MB / 8MB / 16MB access from CPU/BIOS
  //
  PciWrite16(
    PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0,R_PCH_LPC_FWH_BIOS_DEC),
    ( B_PCH_LPC_FWH_BIOS_DEC_F8 |
    B_PCH_LPC_FWH_BIOS_DEC_F0 |
    B_PCH_LPC_FWH_BIOS_DEC_E8 |
    B_PCH_LPC_FWH_BIOS_DEC_E0 |
    B_PCH_LPC_FWH_BIOS_DEC_D8 |
    B_PCH_LPC_FWH_BIOS_DEC_D0 |
    B_PCH_LPC_FWH_BIOS_DEC_C8 |
    B_PCH_LPC_FWH_BIOS_DEC_C0 |
    B_PCH_LPC_FWH_BIOS_DEC_70 |
    B_PCH_LPC_FWH_BIOS_DEC_60 |
    B_PCH_LPC_FWH_BIOS_DEC_50 |
    B_PCH_LPC_FWH_BIOS_DEC_40 |
    B_PCH_LPC_FWH_BIOS_LEG_E  |
    B_PCH_LPC_FWH_BIOS_LEG_F )
    );

  //
  // Disable SERR NMI and IOCHK# NMI in port 61
  //
  IoOr8 (R_PCH_NMI_SC, (B_PCH_NMI_SC_PCI_SERR_EN | B_PCH_NMI_SC_IOCHK_NMI_EN));

  //
  // LPC I/O Configuration
  //
  PciAndThenOr16(
    PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0,R_PCH_LPC_IO_DEC),
    (UINT16)~(B_PCH_LPC_LPT_DEC | B_PCH_LPC_COMB_DEC | B_PCH_LPC_COMA_DEC),
    (1 << 4)
    );

  PciWrite16(
    PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0,R_PCH_LPC_ENABLES),
    ( B_PCH_LPC_ENABLES_CNF2_EN |
    B_PCH_LPC_ENABLES_CNF1_EN |
    B_PCH_LPC_ENABLES_MC_EN |
    B_PCH_LPC_ENABLES_COMB_EN |
    B_PCH_LPC_ENABLES_COMA_EN)
    );

  //
    // Enable LPC decode at 0xCA5 for Pilot BMC
    //
  
     PciWrite32(
     PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0,R_PCH_LPC_GEN3_DEC),
    (((~BMC_KCS_BASE_MASK) & 0xFC)<<16)| ((IPMI_DEFAULT_SMM_IO_BASE & 0xFFFF) | 0x01)  ); 
    
    //
    // Enable LPC decode at 0x600 for Pilot Mailbox
    //
    PciWrite32(
     PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0,R_PCH_LPC_GEN4_DEC),
    (((~PILOTIII_MAILBOX_MASK) & 0xFC)<<16 )|((PILOTIII_MAILBOX_BASE_ADDRESS & 0xFFFF) | 0x01));
   

  //
  // SV work around
  //
//  PchLpcPciCfg8And (R_PCH_LPC_BIOS_CNTL, 0xDF);

}





VOID
PchRcrbInit (
  IN SYSTEM_CONFIGURATION        *SystemConfiguration
  )
{
  //
  // Disable the Watchdog timer expiration from causing a system reset
  // Port80 to LPC 
  //
  MmioOr32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_GCS, B_PCH_RCRB_GCS_NR);

  //
  // Enable IOxAPIC and FERR#
  //
  MmioOr16 (PCH_RCBA_ADDRESS + R_PCH_RCRB_OIC, B_PCH_RCRB_OIC_AEN + B_PCH_RCRB_OIC_CEN);

  //
  // Enable the upper 128-byte bank of RTC RAM
  //
  MmioOr32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_RTC_CONF, B_PCH_RCRB_RTC_CONF_UCMOS_EN);

  //
  // Program Interrupt routing registers
  //
  // Program Dev31 and Dev26 interrupt pin (all other devices use default values)
  // based on BIOS Platform Design Guidelines 
  // Update PlatformPirq.ini entry if any changes are made to DxxIP and DxxIR registers.

/*
  //
  // ttip=b, sip2=d, cip=c, smip=b, sip1=a
  //
  MmioWrite32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_D31IP, 0x02432110);
  //
  // e2ip=c, u2p=d, u1p=b, u0p=a
  //
  MmioWrite32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_D26IP, 0x30000421);
  //
  // Program Dev31  to Dev 25  interrupt routing
  // based on BIOS Platform Design Guidelines
  //
  MmioWrite16 (
    PCH_RCBA_ADDRESS + R_PCH_RCRB_D31IR,  // Program Dev31 routing
    (V_PCH_RCRB_DXXIR_IDR_PIRQF | V_PCH_RCRB_DXXIR_ICR_PIRQA | V_PCH_RCRB_DXXIR_IBR_PIRQC | V_PCH_RCRB_DXXIR_IAR_PIRQD)
    );
  //
  // dev 30 generates no interrupts, so no routing needed
  //
  MmioWrite16 (
    PCH_RCBA_ADDRESS + R_PCH_RCRB_D29IR,  // Program Dev29 routing
    (V_PCH_RCRB_DXXIR_IDR_PIRQA | V_PCH_RCRB_DXXIR_ICR_PIRQC | V_PCH_RCRB_DXXIR_IBR_PIRQD | V_PCH_RCRB_DXXIR_IAR_PIRQH)
    );
  MmioWrite16 (
    PCH_RCBA_ADDRESS + R_PCH_RCRB_D28IR,  // Program Dev28 routing
    (V_PCH_RCRB_DXXIR_IDR_PIRQD | V_PCH_RCRB_DXXIR_ICR_PIRQC | V_PCH_RCRB_DXXIR_IBR_PIRQA | V_PCH_RCRB_DXXIR_IAR_PIRQB)
    );
  MmioWrite16 (
    PCH_RCBA_ADDRESS + R_PCH_RCRB_D27IR,  // Program Dev27 routing
    (V_PCH_RCRB_DXXIR_IDR_PIRQD | V_PCH_RCRB_DXXIR_ICR_PIRQC | V_PCH_RCRB_DXXIR_IBR_PIRQB | V_PCH_RCRB_DXXIR_IAR_PIRQG)
    );
  MmioWrite16 (
    PCH_RCBA_ADDRESS + R_PCH_RCRB_D26IR,  // Program Dev26 routing
    (V_PCH_RCRB_DXXIR_IDR_PIRQD | V_PCH_RCRB_DXXIR_ICR_PIRQC | V_PCH_RCRB_DXXIR_IBR_PIRQF | V_PCH_RCRB_DXXIR_IAR_PIRQA)
    );
  MmioWrite16 (
    PCH_RCBA_ADDRESS + R_PCH_RCRB_D25IR,  // Program Dev25 routing
    (V_PCH_RCRB_DXXIR_IDR_PIRQD | V_PCH_RCRB_DXXIR_ICR_PIRQC | V_PCH_RCRB_DXXIR_IBR_PIRQB | V_PCH_RCRB_DXXIR_IAR_PIRQE)
    );
*/

  // PIRQ routing Info
  // Device 31 Interrupt Pin, reg#3100h
  MmioWrite32 ((PCH_RCBA_ADDRESS + R_PCH_RCRB_D31IP), (ICH_RCRB_IRQA << 8)  +
                                        (ICH_RCRB_IRQC << 12) +
                                        (ICH_RCRB_IRQD << 16) +
                                        (ICH_RCRB_IRQA << 20) +
                                        (ICH_RCRB_IRQC << 24));

  // Device 30 Interrupt Pin, reg#3104h - Read Only

  // Device 29 Interrupt Pin, reg#3108h
  MmioWrite32 ((PCH_RCBA_ADDRESS + R_PCH_RCRB_D29IP), (ICH_RCRB_IRQC << 0));

  // Device 28 Interrupt Pin, reg#310Ch
  MmioWrite32 ((PCH_RCBA_ADDRESS + R_PCH_RCRB_D28IP), (ICH_RCRB_IRQA << 0) +
                                        (ICH_RCRB_IRQB << 4) +
                                        (ICH_RCRB_IRQC << 8) +
                                        (ICH_RCRB_IRQD << 12) +
                                        (ICH_RCRB_IRQA << 16) +
                                        (ICH_RCRB_IRQB << 20) +
                                        (ICH_RCRB_IRQC << 24) +
                                        (ICH_RCRB_IRQD << 28));

  // Device 27 Interrupt Pin, reg#3110h
  MmioWrite32 ((PCH_RCBA_ADDRESS + R_PCH_RCRB_D27IP),(ICH_RCRB_IRQA << 0));

  // Device 26 Interrupt Pin, reg#3114h
  MmioWrite32 ((PCH_RCBA_ADDRESS + R_PCH_RCRB_D26IP), (ICH_RCRB_IRQC << 0));

  // Device 25 Interrupt Pin, reg#3118h
  MmioWrite32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_D25IP, (ICH_RCRB_IRQA << 0));


  // Device 22 Interrupt Pin, reg#3124h
  MmioWrite32 ((PCH_RCBA_ADDRESS + R_PCH_RCRB_D22IP), (ICH_RCRB_IRQA << 0) +
                                      (ICH_RCRB_IRQB << 4) +
                                      (ICH_RCRB_IRQA << 8) +
                                      (ICH_RCRB_IRQB << 12));

  // Device 20 Interrupt Pin, reg#3128h
  MmioWrite32 ((PCH_RCBA_ADDRESS + R_PCH_RCRB_D20IP), (ICH_RCRB_IRQD << 0));

  // Device 31 Interrupt Route, reg#3140h
  MmioWrite16 ((PCH_RCBA_ADDRESS + R_PCH_RCRB_D31IR), (ICH_RCRB_PIRQA << 0) +
                                        (ICH_RCRB_PIRQD << 4) +
                                        (ICH_RCRB_PIRQC << 8) +
                                        (ICH_RCRB_PIRQA << 12));

  // Device 29 Interrupt Route, reg#3144h
  MmioWrite16 ((PCH_RCBA_ADDRESS + R_PCH_RCRB_D29IR), (ICH_RCRB_PIRQA << 0) +
                                        (ICH_RCRB_PIRQB << 4) +
                                        (ICH_RCRB_PIRQC << 8) +
                                        (ICH_RCRB_PIRQD << 12));

  // Device 28 Interrupt Route, reg#3146h
  MmioWrite16 ((PCH_RCBA_ADDRESS + R_PCH_RCRB_D28IR), (ICH_RCRB_PIRQA << 0) +
                                        (ICH_RCRB_PIRQB << 4) +
                                        (ICH_RCRB_PIRQC << 8) +
                                        (ICH_RCRB_PIRQD << 12));

  // Device 27 Interrupt Route, reg#3148h
  MmioWrite16 ((PCH_RCBA_ADDRESS + R_PCH_RCRB_D27IR), (ICH_RCRB_PIRQG << 0) +
                                        (ICH_RCRB_PIRQB << 4) +
                                        (ICH_RCRB_PIRQC << 8) +
                                        (ICH_RCRB_PIRQD << 12));

  // Device 26 Interrupt Route, reg#314Ch
  MmioWrite16 ((PCH_RCBA_ADDRESS + R_PCH_RCRB_D26IR), (ICH_RCRB_PIRQA << 0) +
                                        (ICH_RCRB_PIRQB << 4) +
                                        (ICH_RCRB_PIRQC << 8) +
                                        (ICH_RCRB_PIRQD << 12));

  // Device 25 Interrupt Route, reg#3150h
  MmioWrite16 ((PCH_RCBA_ADDRESS + R_PCH_RCRB_D25IR), (ICH_RCRB_PIRQE << 0) +
                                        (ICH_RCRB_PIRQF << 4) +
                                        (ICH_RCRB_PIRQG << 8) +
                                        (ICH_RCRB_PIRQH << 12));

  // Device 22 Interrupt Route, reg#315Ch
  MmioWrite16 ((PCH_RCBA_ADDRESS + R_PCH_RCRB_D22IR), (ICH_RCRB_PIRQA << 0) +
                                        (ICH_RCRB_PIRQB << 4) +
                                        (ICH_RCRB_PIRQA << 8) +
                                        (ICH_RCRB_PIRQB << 12));

  // Device 20 Interrupt Route, reg#3160h
  MmioWrite16 ((PCH_RCBA_ADDRESS + R_PCH_RCRB_D20IR), (ICH_RCRB_PIRQA << 0) +
                                        (ICH_RCRB_PIRQB << 4) +
                                        (ICH_RCRB_PIRQC << 8) +
                                        (ICH_RCRB_PIRQD << 12));
}

EFI_STATUS
PchSpecificSoftStrap(
  IN EFI_PEI_SERVICES            **PeiServices,
  IN SYSTEM_CONFIGURATION        *SystemConfiguration,
IN UINT8                          PlatformType
)
{
  BOOLEAN                        ResetRequired;
  UINT32                         CurrentStrapValue;
  UINT32                         RequiredStrapValue;  
  UINT8                          Data8;
  EFI_PEI_SPI_SOFTSTRAPS_PPI     *mSpiAccPpi;
  EFI_STATUS                     Status;
  
  Data8=0; 
  
  //
  // Get SoftstrapPpiGuid 
  //
  Status = (**PeiServices).LocatePpi(
                            PeiServices,
                            &gPeiSpiSoftStrapsPpiGuid, 
                            0, 
                            NULL, 
                            &mSpiAccPpi
                            ) ;
  ASSERT_EFI_ERROR (Status);

  Data8=0; 
    
  ResetRequired = FALSE;
  
  if((PlatformType == TypeCamelbackMountain) ||
  (PlatformType== TypeTlaquepaque) || 
  (PlatformType == TypeTrabucoCanyon) ||
  (PlatformType == TypePiestewaPeak) ||
  (PlatformType == TypeEchoCanyon)){
  mSpiAccPpi->SoftStrapRead (PeiServices, 
						   mSpiAccPpi, 
						   0, 
						   PCHSTRAP_9 * sizeof(UINT32), 
						   &CurrentStrapValue);

//  PCIePortConf1 value="11: 2x2 Port 1 (x2)   BIT 3-BIT2
// PCIePortConf2_Pch value="00: 1x4 Port 5 (x4)    BIT 1- BIT0
//  USBP2PCIeP1Mode value="00 : PCIe Lane 1 is statically assigned to USB3 Port 2" - BIT 21-BIT 20
//  USBP3PCIeP2Mode value="00 : PCIe Lane 2 is statically assigned to USB3 Port 3"-- BIT 19-BIT 18


     if(((CurrentStrapValue & 0x3C0000) != 0 ) ||((CurrentStrapValue & 0x3 )!= 0x3))
       {
       
       // Setting the	USBP2PCIeP1Mode value="00 : PCIe Lane 1 is statically assigned to USB3 Port 2" - BIT 21-BIT 20 as 00
       // Settign the	USBP3PCIeP2Mode value="00 : PCIe Lane 2 is statically assigned to USB3 Port 3"-- BIT 19-BIT 18	as 00
       CurrentStrapValue = (CurrentStrapValue & ~(0x3C0000)) ;
       CurrentStrapValue = (CurrentStrapValue & ~(0xF)) ;
       
       // Setting the PCIePortConf1 value="00: 2x2 Port 1 (x2)	BIT 3-BIT2	  as  00
       // Setting the PCIePortConf2_Pch value="11: 1x4 Port 5 (x4)	 BIT 1- BIT0 as 11 
       CurrentStrapValue = (CurrentStrapValue | (0x3)) ;
       
       
       mSpiAccPpi->SoftStrapWrite (PeiServices, mSpiAccPpi, 0, PCHSTRAP_9 * sizeof(UINT32), &CurrentStrapValue);  
       DEBUG((DEBUG_ERROR,"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n"));
       DEBUG((DEBUG_ERROR,"Performing global reset to latch new strap values.\n"));
       ResetRequired= TRUE;
       }
     }
  
  mSpiAccPpi->SoftStrapRead (PeiServices, 
                           mSpiAccPpi, 
                           0, 
                           PCHSTRAP_1 * sizeof(UINT32), 
                           &CurrentStrapValue);
  
  RequiredStrapValue = (CurrentStrapValue & ~(BIT25)) ;
  // PCH Softstrap 1 hook: TPM on SPI is Enabled and TPM on LPC is disabled if BIT 25 sets.
  if (SystemConfiguration->TpmOnSpi == OPTION_ENABLED){
      RequiredStrapValue |= WBG_SS01_TOS_ENABLE;
      DEBUG((DEBUG_INFO,"PCH Softstrap 1 hook: TPM on SPI is Enabled or TPM on LPC is Disabled.\n"));
  } else {
      RequiredStrapValue &= ~((UINT32) WBG_SS01_TOS_ENABLE);
      DEBUG((DEBUG_INFO,"PCH Softstrap 1 hook: TPM on SPI is Disabled or TPM on LPC is Enabled.\n"));
  }
  if (SystemConfiguration->TpmOnSpi != OPTION_AUTO) {
  /// Write softstrap values. 
  if ((CurrentStrapValue) != (RequiredStrapValue)) {
     ResetRequired = TRUE;
     mSpiAccPpi->SoftStrapWrite (PeiServices, mSpiAccPpi, 0, PCHSTRAP_1 * sizeof(UINT32), &RequiredStrapValue);     
   }    
  }
  //
  // Avoid reset if running under SoftSDV 
  //
#ifndef SOFTSDV_FLAG
  if (ResetRequired == TRUE) {
    DEBUG((DEBUG_ERROR,"Performing global reset to latch new strap values.\n"));
    PciOr32 (
      PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0, R_PCH_LPC_PMIR),
      (UINT32)B_PCH_LPC_PMIR_CF9GR
      );
    Data8 = COLD_RESET | FULL_RESET;
    IoWrite8 (RESET_PORT, Data8);
    Data8 |= RST_CPU;
    IoWrite8 (RESET_PORT, Data8);
    CpuDeadLoop();
  }
#endif
 
 return EFI_SUCCESS;
}



EFI_STATUS
PchFixupSofStraps(
  IN EFI_PEI_SERVICES            **PeiServices,
  IN SYSTEM_CONFIGURATION        *SystemConfiguration,
  IN UINT16                      PlatformType
)
{
  const SOFTSTRAP_FIXUP_ENTRY    *SoftstrapFixupEntry;
  BOOLEAN                        ResetRequired;
  UINT8                          RevId;
  UINT32                         PciD31F0RegBase;
  UINT8                          StrapNumber; 
  UINT32                         CurrentStrapValue[21];
  UINT32                         RequiredStrapValue[21];
  UINT64                         Mask;
  UINTN                          Count;  
  UINTN                          Index; 
  UINT8                          Data8;
  EFI_PEI_SPI_SOFTSTRAPS_PPI     *mSpiAccPpi;
  EFI_STATUS                     Status;
  
  Data8=0; 
  
      //
    // Get SoftstrapPpiGuid 
    //
   Status = (**PeiServices).LocatePpi(
                              PeiServices,
                              &gPeiSpiSoftStrapsPpiGuid, 
                              0, 
                              NULL, 
                              &mSpiAccPpi
                              ) ;
    ASSERT_EFI_ERROR (Status);

  Data8=0; 
    
  if (PlatformType > TypePlatformMax) {
     DEBUG((DEBUG_ERROR,"PchFixupSofStraps: Platform not identified.\n"));
     return EFI_UNSUPPORTED;
  }
  
  ResetRequired = FALSE;
  PciD31F0RegBase =  PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, PCI_FUNCTION_NUMBER_PCH_LPC , 0);
  
  RevId = MmioRead8(PciD31F0RegBase + R_PCH_LPC_RID);  

  for (Index = 0; Index <21; Index ++) {
  mSpiAccPpi->SoftStrapRead (PeiServices, 
                           mSpiAccPpi, 
                           0, 
                              Index * sizeof(UINT32), 
                              &CurrentStrapValue[Index]);
  }

   CopyMem(RequiredStrapValue, CurrentStrapValue, sizeof(RequiredStrapValue));   
  
  for (SoftstrapFixupEntry = SoftstrapFixupTable; SoftstrapFixupEntry->BoardId != TypePlatformUnknown; SoftstrapFixupEntry++) 
  {

    if (PlatformType != SoftstrapFixupEntry->BoardId) continue;
    StrapNumber=SoftstrapFixupEntry->SoftStrapNumber;

    Mask = 1;
    Count = 0;

    while (Count++ < SoftstrapFixupEntry->BitfieldLength) {
      Mask = MultU64x32(Mask, 2);
    }

    Mask -= 1;
    Mask = LShiftU64(Mask, SoftstrapFixupEntry->BitfieldOffset);
    RequiredStrapValue[StrapNumber] = ((CurrentStrapValue[StrapNumber] & ~Mask) | 
                          ((SoftstrapFixupEntry->Value << SoftstrapFixupEntry->BitfieldOffset) & Mask));
    }


  /// Write softstrap values. 
  for (Index=0; Index<21; Index ++)
  {
  

    if ((CurrentStrapValue[Index]) != (RequiredStrapValue[Index])) {
      ResetRequired = TRUE;
      mSpiAccPpi->SoftStrapWrite (PeiServices, mSpiAccPpi, 0, Index * sizeof(UINT32), &RequiredStrapValue[Index]);
    }    
  }

  if (ResetRequired == TRUE) {
    DEBUG((DEBUG_ERROR,"Performing global reset to latch new strap values.\n"));
    PciOr32 (
      PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0, R_PCH_LPC_PMIR),
      (UINT32)B_PCH_LPC_PMIR_CF9GR
      );
    Data8 = COLD_RESET | FULL_RESET;
    IoWrite8 (RESET_PORT, Data8);
    Data8 |= RST_CPU;
    IoWrite8 (RESET_PORT, Data8);
    CpuDeadLoop();
  }

 return EFI_SUCCESS;
}


EFI_STATUS
PlatformPchInit (
  IN SYSTEM_CONFIGURATION        *SystemConfiguration,
  IN EFI_PEI_SERVICES            **PeiServices,
  IN UINT16                      PlatformType

  )
{

  InitPchPolicyPei (PeiServices, SystemConfiguration, (UINT8)PlatformType);
  
#if USE_AMI_GPIO_PROGRAMMING == 0
  PchGpioInit (PeiServices, PlatformType, SystemConfiguration);
#endif

  PchRcrbInit (SystemConfiguration);

  PchLpcInit (SystemConfiguration);
  
  

 // InitPchUsbConfig(UsbConfig);  

//
//  TPM ON SPI BIT25 and correct the Softstraps accordingly for platform
//
  if (IsSoftSdvPlatform() == FALSE) {
    PchSpecificSoftStrap(PeiServices,SystemConfiguration,(UINT8)PlatformType);  
    }
  

#ifdef SDP_FLAG
  //
  // Avoid emergency fan speed by enabling PCH temperature reporting.
  // TODO: If it's decided to fix this in all build flavors, remove this
  // call and correct the straps in the FITC XML instead.
  //
//  PchFixupSofStraps(PeiServices,SystemConfiguration,PlatformType);
#endif
  //
  // --cr in case user wrongly set the variable to PCIE but no PCIE plug in video card present
  // so should not disable onboard video here, otherwize there's no VGA handle
  //
  //PchInitOnBoardVideo(SystemConfiguration, PeiServices, PlatformType);
 
  //
  // Program port 61 timer 1 as refresh timer. 
  //
  IoWrite8 (TIMER1_CONTROL_PORT, LOAD_COUNTER1_LSB);
  IoWrite8 (TIMER1_COUNT_PORT, COUNTER1_COUNT);

  return EFI_SUCCESS;
}
