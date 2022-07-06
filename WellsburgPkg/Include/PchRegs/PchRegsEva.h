/** @file 
  Register names for PCH Eva devices.
  
  Conventions:

  - Prefixes:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values of bits within the registers
    Definitions beginning with "S_" are register sizes
    Definitions beginning with "N_" are the bit position
  - In general, PCH registers are denoted by "_PCH_" in register names
  - Registers / bits that are different between PCH generations are denoted by
    "_PCH_<generation_name>_" in register/bit names. e.g., "_PCH_LPT_"
  - Registers / bits that are different between SKUs are denoted by "_<SKU_name>"
    at the end of the register/bit names
  - Registers / bits of new devices introduced in a PCH generation will be just named
    as "_PCH_" without <generation_name> inserted.

@copyright
  Copyright (c) 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
#ifndef _PCH_REGS_EVA_H_
#define _PCH_REGS_EVA_H_

#define PCI_DEVICE_NUMBER_EVA 17
#define PCI_FUNCTION_NUMBER_EVA_MROM 0
#define PCI_FUNCTION_NUMBER_EVA_SMBUS0 1
#define PCI_FUNCTION_NUMBER_EVA_SMBUS1 2
#define PCI_FUNCTION_NUMBER_EVA_SMBUS2 3
#define PCI_FUNCTION_NUMBER_EVA_SSATA  4
#define PCI_FUNCTION_NUMBER_EVA_SSATA2 5

///
/// Wellsburg SKUs
///
#define WBG_SKU_G    1
#define WBG_SKU_X    2
#define WBG_SKU_A    3

#define PCI_DEVICE_NUMBER_PCH_SSATA     17
#define PCI_FUNCTION_NUMBER_PCH_SSATA   4

#define PCH_SSATA_MAX_CONTROLLERS 1
#define PCH_SSATA_MAX_PORTS       6   // But only 4 ports are enable, BIOS needs to disable Port 4 and 5 

#define R_PCH_WBG_SSATA_DEVICE_ID      0x02

///
///  SATA Controllers DID definition
///
#define V_PCH_WBG_SSATA_DEVICE_ID_D_IDE                      0x8D60      // Server IDE Mode (Ports 0,1, 2 and 3)
#define V_PCH_WBG_SSATA_DEVICE_ID_D_AHCI                     0x8D62      // Server AHCI Mode (Ports 0-4)
#define V_PCH_WBG_SSATA_DEVICE_ID_D_RAID                     0x8D64      // Server RAID 0/1/5/10 
#define V_PCH_WBG_SSATA_DEVICE_ID_D_RAID_PREMIUM             0x8D66      // Server RAID 0/1/5/10 Mode, based on D31:F2:9Ch[7]
#define V_PCH_WBG_SSATA_DEVICE_ID_D_RAID_PREMIUM_DSEL0       0x2823      // Server RAID 0/1/5/10 Mode, based on D31:F2:9Ch[7]
#define V_PCH_WBG_SSATA_DEVICE_ID_D_RAID_PREMIUM_DSEL1       0x2827      // Server RAID 0/1/5/10 Mode, based on D31:F2:9Ch[7]
#define V_PCH_WBG_SSATA2_DEVICE_ID_D_IDE                     0x8D68      // Server IDE Mode (Ports 0,1, 2 and 3)
#define V_PCH_WBG_SSATA_DEVICE_ID_D_RAID1                    0x8D6E      // Server RAID 1/RRT

///
/// MSUint SMBUS DID definition
///
#define V_PCH_WBG_MS_SMBUS_DEVICE_ID_0                       0x8D7D       // MS Unit SMB 0
#define V_PCH_WBG_MS_SMBUS_DEVICE_ID_1                       0x8D7E       // MS Unit SMB 1
#define V_PCH_WBG_MS_SMBUS_DEVICE_ID_2                       0x8D7F       // MS Unit SMB 2

#define V_PCH_WBG_MROM_DEVICE_ID_0                           0x8D7C       // MS Unit MROM

#define R_PCH_WBG_MROM_DEVCLKGCTL     0xE4

#define R_PCH_WBG_SMBUS0_DEVCLKGCTL   0xE4
#define R_PCH_WBG_SMBUS1_DEVCLKGCTL   0xE4
#define R_PCH_WBG_SMBUS2_DEVCLKGCTL   0xE4
#define R_PCH_WBG_MROM_PLKCTL         0xE8
#define R_PCH_WBG_SMBUS0_PLKCTL       0xE8
#define R_PCH_WBG_SMBUS1_PLKCTL       0xE8
#define R_PCH_WBG_SMBUS2_PLKCTL       0xE8

#define R_PCH_WBG_MROM_ADRTIMERCTRL   0x180


///
///  MS Unit Hide Control Register
///
#define PCH_WBG_MSUINT_FUNCS              4
#define R_PCH_WBG_MSUINT_MSDEVFUNCHIDE          0xD4
#define B_PCH_EVA_MSUNIT_MDEVFUNCHIDE_LOCK      BIT31
#define B_PCH_WBG_MSUINT_MSDEVFUNCHIDE_RSVD    (BIT30|BIT29|BIT28|BIT27|BIT26|BIT25|BIT24|\
                                            BIT23|BIT22|BIT21|BIT20|BIT19|BIT18|BIT17|\
                                            BIT16|BIT15|BIT14|BIT13|BIT12|BIT11|BIT10|\
                                            BIT9|BIT8|BIT7|BIT6)

#define B_PCH_EVA_MSUNIT_MSDEVFUNCHIDE_SSATA2 (BIT5)
#define B_PCH_EVA_MSUNIT_MSDEVFUNCHIDE_SSATA  (BIT4)


#define B_PCH_EVA_MSUNIT_MSDEVFUNCHIDE_SMBUS2  BIT3
#define B_PCH_EVA_MSUNIT_MSDEVFUNCHIDE_SMBUS1  BIT2
#define B_PCH_EVA_MSUNIT_MSDEVFUNCHIDE_SMBUS0  BIT1
#define B_PCH_EVA_MSUNIT_MSDEVFUNCHIDE_MROM    BIT0

#endif
