/** @file
  Header file for the PchPolicyInitDxe Driver.

@copyright
  Copyright (c) 2009 - 2014 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

**/
#ifndef _PCH_PLATFORM_POLICY_UPDATE_DXE_H_
#define _PCH_PLATFORM_POLICY_UPDATE_DXE_H_

// Aptio V Server Override Start: Program PCH physical slot number based on SDL porting.
#include <Token.h>
// Aptio V Server Override end: Program PCH physical slot number based on SDL porting.
#include <Platform.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/PchPlatformLib.h>
#include <Library/PchPlatformPolicyInitCommonLib.h>
#include <Library/PchPlatformPolicyInitDxeLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <IndustryStandard/Pci.h>

//AptioV Server override Start: Use SDL token to program defaul SVID and SID.
PCH_SVID_SID_INIT_ENTRY  SvidSidInitTable[PCH_MAX_DEVICES] = {
    {
      31,
      0,
      R_PCH_LPC_SS,
     {
      PCH_DEFAULT_SVID,
      PCH_DEFAULT_SID
     }
    },
    {
      31,
      2,
      R_PCH_SATA_AHCI_SVID,
      {
        PCH_DEFAULT_SVID,
        PCH_DEFAULT_SID
      }	  
    },
    {
      31,
      5,
      R_PCH_SATA_AHCI_SVID,
      {
       PCH_DEFAULT_SVID,
       PCH_DEFAULT_SID
      } 
    },
    {
      31,
      3,
      R_PCH_SMBUS_SVID,
      {
       PCH_DEFAULT_SVID,
       PCH_DEFAULT_SID
      }		  
    },
    {
      31,
      6,
      R_PCH_THERMAL_SVID,
      {
       PCH_DEFAULT_SVID,
       PCH_DEFAULT_SID
      }		  
    },
    {
      29,
      0,
      R_PCH_EHCI_SVID,
      {
        PCH_DEFAULT_SVID,
        PCH_DEFAULT_SID
      }		  
    },
    {
      26,
      0,
      R_PCH_EHCI_SVID,
      {
       PCH_DEFAULT_SVID,
       PCH_DEFAULT_SID
      }		  
    },
    {
      20,
      0,
      R_PCH_XHCI_SVID,
      {
       PCH_DEFAULT_SVID,
       PCH_DEFAULT_SID
      }		  
    },
    {
      27,
      0,
      R_PCH_HDA_SVID,
      {
        PCH_DEFAULT_SVID,
        PCH_DEFAULT_SID
      }		  
    },
    {
      28,
      0,
      R_PCH_PCIE_SVID,
      {
       PCH_DEFAULT_SVID,
       PCH_DEFAULT_SID
      }		  
    },
    {
      28,
      1,
      R_PCH_PCIE_SVID,
      {
        PCH_DEFAULT_SVID,
        PCH_DEFAULT_SID
      }		  
    },
    {
      28,
      2,
      R_PCH_PCIE_SVID,
      {
        PCH_DEFAULT_SVID,
        PCH_DEFAULT_SID
      }		  
    },
    {
      28,
      3,
      R_PCH_PCIE_SVID,
      {
        PCH_DEFAULT_SVID,
        PCH_DEFAULT_SID
      }		  
    },
    {
      28,
      4,
      R_PCH_PCIE_SVID,
      {
        PCH_DEFAULT_SVID,
        PCH_DEFAULT_SID
      }		  
    },
    {
      28,
      5,
      R_PCH_PCIE_SVID,
      {
       PCH_DEFAULT_SVID,
       PCH_DEFAULT_SID
      }		  
    },
    {
      28,
      6,
      R_PCH_PCIE_SVID,
      {
       PCH_DEFAULT_SVID,
       PCH_DEFAULT_SID
      }		  
    },
    {
      28,
      7,
      R_PCH_PCIE_SVID,
      {
       PCH_DEFAULT_SVID,
       PCH_DEFAULT_SID
      }		  
    },
    {
      25,
      0,
      R_PCH_LAN_SVID,
      {
        PCH_DEFAULT_SVID,
        PCH_DEFAULT_SID
      }		  
    },
    /* HECI */
    {
      22,
      0,
      PCI_SVID_OFFSET,
      {
        PCH_DEFAULT_SVID,
        PCH_DEFAULT_SID
      }		  
    },
    {
      22,
      1,
      PCI_SVID_OFFSET,
      {
        PCH_DEFAULT_SVID,
        PCH_DEFAULT_SID
      }		  
    },
    {
      22,
      2,
      PCI_SVID_OFFSET,
      {
        PCH_DEFAULT_SVID,
        PCH_DEFAULT_SID
      }		  
    },
    {
      22,
      3,
      PCI_SVID_OFFSET,
      {
        PCH_DEFAULT_SVID,
        PCH_DEFAULT_SID
      }		  
    },
    {
      17,
      0,
      PCI_SVID_OFFSET,
      {
        PCH_DEFAULT_SVID,
        PCH_DEFAULT_SID
      }		  
    },
    {
      17,
      1,
      PCI_SVID_OFFSET,
      {
        PCH_DEFAULT_SVID,
        PCH_DEFAULT_SID
      }		  
    },	
    {
      17,
      2,
      PCI_SVID_OFFSET,
      {
        PCH_DEFAULT_SVID,
        PCH_DEFAULT_SID
      }		  
    },	
    {
      17,
      3,
      PCI_SVID_OFFSET,
      {
        PCH_DEFAULT_SVID,
        PCH_DEFAULT_SID
      }		  
    },	
    {
      17,
      4,
      R_PCH_SATA_AHCI_SVID,
      {
        PCH_DEFAULT_SVID,
        PCH_DEFAULT_SID
      }		  
    }	
  };
//AptioV Server override End: Use SDL token to program defaul SVID and SID.


STATIC UINT8                    mSmbusRsvdAddresses[PLATFORM_NUM_SMBUS_RSVD_ADDRESSES] = {
  SMBUS_ADDR_CH_A_1,
  SMBUS_ADDR_CH_A_2,
  SMBUS_ADDR_CH_B_1,
  SMBUS_ADDR_CH_B_2
};

UINT32 mAzaliaVerbTableData1[] = {
  //
  // Slim River(FFDS3) Audio Verb Table - 0x10EC0272/0000/00
  //
  // Pin Complex (NID 0x20 )
  //
  0x02050002,
  0x0204A2A0,
  0x02050003,
  0x02044843,
  //
  // Pin Complex (NID 0x11 )
  //
  0x01171CF0,
  0x01171D11,
  0x01171E11,
  0x01171F41,
  //
  // Pin Complex (NID 0x12 )
  //
  0x01271C20,
  0x01271D09,
  0x01271EA3,
  0x01271F99,
  //
  // Pin Complex (NID 0x13 )
  //
  0x01371CF0,
  0x01371D11,
  0x01371E11,
  0x01371F41,
  //
  // Pin Complex (NID 0x14 )
  //
  0x01471C10,
  0x01471D01,
  0x01471E13,
  0x01471F99,
  //
  // Pin Complex (NID 0x15 )
  //
  0x01571CF0,
  0x01571D11,
  0x01571E11,
  0x01571F41,
  //
  // Pin Complex (NID 0x16 )
  //
  0x01671CF0,
  0x01671D11,
  0x01671E11,
  0x01671F41,
  //
  // Pin Complex (NID 0x17 )
  //
  0x01771CF0,
  0x01771D11,
  0x01771E11,
  0x01771F41,
  //
  // Pin Complex (NID 0x18 )
  //
  0x01871C30,
  0x01871D98,
  0x01871EA1,
  0x01871F04,
  //
  // Pin Complex (NID 0x19 )
  //
  0x01971CF0,
  0x01971D11,
  0x01971E11,
  0x01971F41,
  //
  // Pin Complex (NID 0x1A )
  //
  0x01A71CF0,
  0x01A71D11,
  0x01A71E11,
  0x01A71F41,
  //
  // Pin Complex (NID 0x1B )
  //
  0x01B71CF0,
  0x01B71D11,
  0x01B71E11,
  0x01B71F41,
  //
  // Pin Complex (NID 0x1D )
  //
  0x01D71C2D,
  0x01D71D9A,
  0x01D71E17,
  0x01D71F40,
  //
  // Pin Complex (NID 0x1E )
  //
  0x01E71CF0,
  0x01E71D11,
  0x01E71E11,
  0x01E71F41,
  //
  // Pin Complex (NID 0x21 )
  //
  0x02171C1F,
  0x02171D40,
  0x02171E21,
  0x02171F04,
  //
  // Pin Complex (NID 0x20 ) - 1
  //
  0x02050003,
  0x02044003,
  0x02050003,
  0x02044003
};

UINT32 mAzaliaVerbTableData2[] = {
  //
  // Buffalo Trail Audio Verb Table - 0x10EC0269/0000/00
  //
  // Pin Complex (NID 0x12 )
  //
  0x01271C30,
  0x01271D09,
  0x01271EA3,
  0x01271F99,
  //
  // Pin Complex (NID 0x14 )
  //
  0x01471C10,
  0x01471D01,
  0x01471E13,
  0x01471F99,
  //
  // Pin Complex (NID 0x15 )
  //
  0x01571C1F,
  0x01571D40,
  0x01571E21,
  0x01571F04,
  //
  // Pin Complex (NID 0x16 )
  //
  0x01671CF0,
  0x01671D11,
  0x01671E11,
  0x01671F41,
  //
  // Pin Complex (NID 0x18 )
  //
  0x01871C40,
  0x01871D98,
  0x01871EA1,
  0x01871F04,
  //
  // Pin Complex (NID 0x19 )
  //
  0x01971CF0,
  0x01971D11,
  0x01971E11,
  0x01971F41,
  //
  // Pin Complex (NID 0x1A )
  //
  0x01A71CF0,
  0x01A71D11,
  0x01A71E11,
  0x01A71F41,
  //
  // Pin Complex (NID 0x1B )
  //
  0x01B71CF0,
  0x01B71D11,
  0x01B71E11,
  0x01B71F41,
  //
  // Pin Complex (NID 0x1D )
  //
  0x01D71C05,
  0x01D71D82,
  0x01D71E13,
  0x01D71F40,
  //
  // Pin Complex (NID 0x1E )
  //
  0x01E71C20,
  0x01E71D11,
  0x01E71E45,
  0x01E71F04,
  //
  // Pin Complex (NID 0x11 )
  //
  0x01171CF0,
  0x01171D11,
  0x01171E11,
  0x01171F41,
  //
  // Pin Complex (NID 0x20 )
  //
  0x02071CFF,
  0x02071DFF,
  0x02071E00,
  0x02071F00,
  0x0205000F,
  0x02040000,
  0x02050010,
  0x02041FEA,
  0x02050011,
  0x02040D73,
  0x0205001D,
  0x02040011,
  0x0205000F,
  0x0204A100,
  0x0205000F,
  0x0204A100
};

UINT32 mAzaliaVerbTableData3[] = {
  //
  // Rear Audio Verb Table 0x10EC0888
  //
  // (NID 01h)
  // ===== HDA Codec Subsystem ID Verb-table =====
  // HDA Codec Subsystem ID  : 0x10EC0000
  //
  0x00172000,
  0x00172100,
  0x001722EC,
  0x00172310,
  //
  // ===== Pin Widget Verb-table =====
  // Pin Complex 1       (NID 14h)
  //
  0x01471C10,
  0x01471D44,
  0x01471E01,
  0x01471F01,
  //
  // Pin Complex 2       (NID 15h)
  //
  0x01571CF0,
  0x01571D10,
  0x01571E11,
  0x01571F41,
  //
  // Pin Complex 3       (NID 16h)
  //
  0x01671CF0,
  0x01671D11,
  0x01671E11,
  0x01671F41,
  //
  // Pin Complex 4       (NID 17h)
  //
  0x01771CF0,
  0x01771D11,
  0x01771E11,
  0x01771F41,
  //
  // Pin Complex 5       (NID 18h)
  //
  0x01871C40,
  0x01871D9C,
  0x01871EA1,
  0x01871F01,
  //
  // Pin Complex 6       (NID 19h)
  //
  0x01971C50,
  0x01971D9C,
  0x01971EA1,
  0x01971F02,
  //
  // Pin Complex 7       (NID 1Ah)
  //
  0x01A71C4F,
  0x01A71D3C,
  0x01A71E81,
  0x01A71F01,
  //
  // Pin Complex 8       (NID 1Bh)
  //
  0x01B71C20,
  0x01B71D4C,
  0x01B71E21,
  0x01B71F02,
  //
  // Pin Complex 9       (NID 1Ch)
  //
  0x01C71CF0,
  0x01C71D11,
  0x01C71E11,
  0x01C71F41,
  //
  // Pin Complex 10       (NID 1Dh)
  //
  0x01D71C01,
  0x01D71DC6,
  0x01D71E04,
  0x01D71F40,
  //
  // Pin Complex 11       (NID 1Eh)
  //
  0x01E71C30,
  0x01E71D71,
  0x01E71E44,
  0x01E71F01,
  //
  // Pin Complex 12       (NID 1Fh)
  //
  0x01F71C60,
  0x01F71D61,
  0x01F71EC4,
  0x01F71F01,
  //
  // Pin Complex 13      (NID 11h)
  //
  0x01171CF0,
  0x01171D11,
  0x01171E11,
  0x01171F41,
  //
  // Pin Complex 14      (NID 12h)
  //
  0x01271CF0,
  0x01271D11,
  0x01271E11,
  0x01271F41,
  //
  // Pin Complex 15      (NID 20h)
  //
  0x02050008,
  0x02040030,
  0x02050008,
  0x02040030
};

UINT32 mAzaliaVerbTableData4[] = {
  //
  // Rear Audio Verb Table 0x10EC0885
  //
  // (NID 01h)
  // ;===== HDA Codec Subsystem ID Verb-table =====
  // ;HDA Codec Subsystem ID  : 0x80860021
  //
  0x00172021,
  0x00172100,
  0x00172286,
  0x00172380,
  //
  // ===== Pin Widget Verb-table =====
  // Pin Complex 1       (NID 14h)
  //
  0x01471C10,
  0x01471D44,
  0x01471E01,
  0x01471F01,
  //
  // Pin Complex 2       (NID 15h)
  //
  0x01571C20,
  0x01571D44,
  0x01571E21,
  0x01571F02,
  //
  // Pin Complex 3       (NID 16h)
  //
  0x01671C11,
  0x01671D60,
  0x01671E01,
  0x01671F01,
  //
  // Pin Complex 4       (NID 17h)
  //
  0x01771CF0,
  0x01771D11,
  0x01771E11,
  0x01771F41,
  //
  // Pin Complex 5       (NID 18h)
  //
  0x01871C50,
  0x01871D98,
  0x01871EA1,
  0x01871F02,
  //
  // Pin Complex 6       (NID 19h)
  //
  0x01971C12,
  0x01971D10,
  0x01971E01,
  0x01971F01,
  //
  // Pin Complex 7       (NID 1Ah)
  //
  0x01A71C40,
  0x01A71D34,
  0x01A71E81,
  0x01A71F01,
  //
  // Pin Complex 8       (NID 1Bh)
  //
  0x01B71C4F,
  0x01B71D98,
  0x01B71EA1,
  0x01B71F01,
  //
  // Pin Complex 9       (NID 1Ch)
  //
  0x01C71CF0,
  0x01C71D11,
  0x01C71E11,
  0x01C71E41,
  //
  // Pin Complex 10       (NID 1Dh)
  //
  0x01D71CF0,
  0x01D71D01,
  0x01D71E83,
  0x01D71F59,
  //
  // Pin Complex 11       (NID 1Eh)
  //
  0x01E71C30,
  0x01E71D11,
  0x01E71E45,
  0x01E71F01,
  //
  // Pin Complex 12       (NID 1Fh)
  //
  0x01F71CF0,
  0x01F71D11,
  0x01F71E11,
  0x01F71F41
};

UINT32 mAzaliaVerbTableData5[] = {
  //
  // IDT 92HD81 Audio Verb Table - 0x111D7605/0000/00
  //
  // port A configuration,front mic	0x02A19020
  //
  0x00A71C20,
  0x00A71D90,
  0x00A71EA1,
  0x00A71F02,
  //
  // port B configuration,front headphone	0x0221401F
  //
  0x00B71C1F,
  0x00B71D40,
  0x00B71E21,
  0x00B71F02,
  //
  // port C configuration,no connection	0x40F00000
  //
  0x00C71C00,
  0x00C71D00,
  0x00C71EF0,
  0x00C71F40,
  //
  // port D configuration,front speaker	0x90110110
  //
  0x00D71C10,
  0x00D71D01,
  0x00D71E11,
  0x00D71F90,
  //
  // port E configuration,no connection
  //
  0x00E71C00,
  0x00E71D00,
  0x00E71EF0,
  0x00E71F40,
  //
  // port F configuration,no connection
  //
  0x00F71C00,
  0x00F71D00,
  0x00F71EF0,
  0x00F71F40,
  //
  // port mono out configuration,no connection
  //
  0x01071C00,
  0x01071D00,
  0x01071EF0,
  0x01071F40,
  //
  // DMIC0 configuration, no connection (digital mic 0x99A30120)
  //
  0x01171C00,
  0x01171D00,
  0x01171EF0,
  0x01171F40,
  //
  // SPDIF0 configuration, no connection
  //
  0x01F71C00,
  0x01F71D00,
  0x01F71EF0,
  0x01F71F40,
  //
  // SPDIF1 configuration, no connection
  //
  0x02071C00,
  0x02071D00,
  0x02071EF0,
  0x02071F40,
  //
  // Analog PCBeep enable
  // PCBeepTable dw (PCBeepTableEnd - ($ + 2)) / 4
  //
  0x00B707C0, // set port B as output
  0x00D70740, // set port D as output
  0x0017EE31, // enable pcbeep, 2017EE30h->disable
  0x0017F412, // BTL gain
};

UINT32 mAzaliaVerbTableData6[] = {
  //
  // Rear Audio Verb Table 0x10EC0889
  //
  // (NID 01h)
  // ;===== HDA Codec Subsystem ID Verb-table =====
  // ;HDA Codec Subsystem ID  : 0x00000000
  //
  0x20172000,
  0x20172100,
  0x20172200,
  0x20172300,
  //
  // ===== Pin Widget Verb-table =====
  // Pin Complex 1       (NID 14h)
  //
  0x21471C10,
  0x21471D44,
  0x21471E01,
  0x21471F01,
  //
  // Pin Complex 2       (NID 15h)
  //
  0x21571C20,
  0x21571D40,
  0x21571E21,
  0x21571F02,
  //
  // Pin Complex 3       (NID 16h)
  //
  0x21671C11,
  0x21671D64,
  0x21671E01,
  0x21671F01,
  //
  // Pin Complex 4       (NID 17h)
  //
  0x21771CF0,
  0x21771D11,
  0x21771E11,
  0x21771F41,
  //
  // Pin Complex 5       (NID 18h)
  //
  0x21871C50,
  0x21871D98,
  0x21871EA1,
  0x21871F02,
  //
  // Pin Complex 6       (NID 19h)
  //
  0x21971C12,
  0x21971D14,
  0x21971E01,
  0x21971F01,
  //
  // Pin Complex 7       (NID 1Ah)
  //
  0x21A71C40,
  0x21A71D34,
  0x21A71E81,
  0x21A71F01,
  //
  // Pin Complex 8       (NID 1Bh)
  //
  0x21B71C4F,
  0x21B71D9C,
  0x21B71EA1,
  0x21B71F01,
  //
  // Pin Complex 9       (NID 1Ch)
  //
  0x21C71CF0,
  0x21C71D11,
  0x21C71E11,
  0x21C71F41,
  //
  // Pin Complex 10       (NID 1Dh)
  //
  0x21D71CF0,
  0x21D71D01,
  0x21D71E83,
  0x21D71F59,
  //
  // Pin Complex 11       (NID 1Eh)
  //
  0x21E71C30,
  0x21E71D11,
  0x21E71E45,
  0x21E71F01,
  //
  // Pin Complex 12       (NID 1Fh)
  //
  0x21F71CF0,
  0x21F71D11,
  0x21F71E11,
  0x21F71F41,
  //
  // Pin Complex 13       (NID 11h)
  //
  0x21171CF0,
  0x21171D11,
  0x21171E11,
  0x21171F41,
  //
  // Pin Complex 14       (NID 12h)
  //
  0x21271CF0,
  0x21271D11,
  0x21271E11,
  0x21271F41
};

UINT32 mAzaliaVerbTableData7[] = {
  //
  // Rear Audio Verb Table 0x10EC0262
  //
  // ;HDACfg Version : 4.10
  // ;HD Audio Codec : ALC262
  // ;PCI PnP ID : PCI\VEN_8086&DEV_2668&SUBSYS_00000000
  // ;HDA Codec PnP ID : HDAUDIO\FUNC_01&VEN_10EC&DEV_0262&SUBSYS_00000000
  // ;The number of verb command block : 14
  // ;    NID 0x11 : 0x411111F0
  // ;    NID 0x12 : 0x99A30940
  // ;    NID 0x14 : 0x411111F0
  // ;    NID 0x15 : 0x04214010
  // ;    NID 0x16 : 0x411111F0
  // ;    NID 0x18 : 0x411111F0
  // ;    NID 0x19 : 0x04A19830
  // ;    NID 0x1A : 0x0481303F
  // ;    NID 0x1B : 0x411111F0
  // ;    NID 0x1C : 0x411111F0
  // ;    NID 0x1D : 0x598301F0
  // ;    NID 0x1E : 0x99430120
  // ;    NID 0x1F : 0x99C30150
  // ;===== HDA Codec Subsystem ID Verb-table =====
  // ;HDA Codec Subsystem ID  : 0x00000000
  //
  0x00172000,
  0x00172100,
  0x00172200,
  0x00172300,
  //
  // ;===== Pin Widget Verb-table =====
  // ;Function widget (NID 0x01 )
  //
  0x0017FF00,
  0x0017FF00,
  0x0017FF00,
  0x0017FF00,
  //
  // ;Pin Complex (NID 0x11 )
  //
  0x01171CF0,
  0x01171D11,
  0x01171E11,
  0x01171F41,
  //
  // ;Pin Complex (NID 0x12 )
  //
  0x01271C40,
  0x01271D09,
  0x01271EA3,
  0x01271F99,
  //
  // ;Pin Complex (NID 0x14 )
  //
  0x01471CF0,
  0x01471D11,
  0x01471E11,
  0x01471F41,
  //
  // ;Pin Complex (NID 0x15 )
  //
  0x01571C10,
  0x01571D40,
  0x01571E21,
  0x01571F04,
  //
  // ;Pin Complex (NID 0x16 )
  //
  0x01671CF0,
  0x01671D11,
  0x01671E11,
  0x01671F41,
  //
  // ;Pin Complex (NID 0x18 )
  //
  0x01871CF0,
  0x01871D11,
  0x01871E11,
  0x01871F41,
  //
  // ;Pin Complex (NID 0x19 )
  //
  0x01971C30,
  0x01971D98,
  0x01971EA1,
  0x01971F04,
  //
  // ;Pin Complex (NID 0x1A )
  //
  0x01A71C3F,
  0x01A71D30,
  0x01A71E81,
  0x01A71F04,
  //
  // ;Pin Complex (NID 0x1B )
  //
  0x01B71CF0,
  0x01B71D11,
  0x01B71E11,
  0x01B71F41,
  //
  // ;Pin Complex (NID 0x1C )
  //
  0x01C71CF0,
  0x01C71D11,
  0x01C71E11,
  0x01C71F41,
  //
  // ;Pin Complex (NID 0x1D )
  //
  0x01D71CF0,
  0x01D71D01,
  0x01D71E83,
  0x01D71F59,
  //
  // ;Pin Complex (NID 0x1E )
  //
  0x01E71C20,
  0x01E71D01,
  0x01E71E43,
  0x01E71F99,
  //
  // ;Pin Complex (NID 0x1F )
  //
  0x01F71C50,
  0x01F71D01,
  0x01F71EC3,
  0x01F71F99
};

UINT32 mAzaliaVerbTableData8[] = {
// ;===================================================================================================
// ;
// ;                               Realtek Semiconuctor Corp.
// ;
// ;===================================================================================================

// ;Realtek High Definition Auio Configuration - Version : 5.0.1.5
// ;Realtek HD Auio Coec : ALC282
// ;PCI PnP ID : PCI\VEN_0000&DEV_0000&SUBSYS_00000000
// ;HDA Coec PnP ID : HDAUDIO\FUNC_01&VEN_10EC&DEV_0282&SUBSYS_00000000
// ;The number of verb comman block : 18

// ;    NID 0x12 : 0x90A60930
// ;    NID 0x14 : 0x90170110
// ;    NID 0x17 : 0x411111F0
// ;    NID 0x18 : 0x03A11820
// ;    NID 0x19 : 0x411111F0
// ;    NID 0x1A : 0x411111F0
// ;    NID 0x1B : 0x411111F0
// ;    NID 0x1D : 0x4015842D
// ;    NID 0x1E : 0x411111F0
// ;    NID 0x21 : 0x0321101F
// ;    NID 0x20 : 0x4070FFFF


// ;===== HDA Coec Subsystem ID Verb-table =====
// ;HDA Coec Subsystem ID  : 0x00000000
0x00172000,
0x00172100,
0x00172200,
0x00172300,


// ;===== Pin Wiget Verb-table =====
// ;Wiget noe 0x01 :
0x0017FF00,
0x0017FF00,
0x0017FF00,
0x0017FF00,
// ;Pin wiget 0x12 - DMIC
0x01271C30,
0x01271D09,
0x01271EA6,
0x01271F90,
// ;Pin wiget 0x14 - FRONT (Port-D)
0x01471C10,
0x01471D01,
0x01471E17,
0x01471F90,
// ;Pin wiget 0x17 - MONO-OUT (Port-H)
0x01771CF0,
0x01771D11,
0x01771E11,
0x01771F41,
// ;Pin wiget 0x18 - MIC1 (Port-B)
0x01871C20,
0x01871D18,
0x01871EA1,
0x01871F03,
// ;Pin wiget 0x19 - MIC2 (Port-F)
0x01971CF0,
0x01971D11,
0x01971E11,
0x01971F41,
// ;Pin wiget 0x1A - LINE1 (Port-C)
0x01A71CF0,
0x01A71D11,
0x01A71E11,
0x01A71F41,
// ;Pin wiget 0x1B - LINE2 (Port-E)
0x01B71CF0,
0x01B71D11,
0x01B71E11,
0x01B71F41,
// ;Pin wiget 0x1D - BEEP-IN
0x01D71C2D,
0x01D71D84,
0x01D71E15,
0x01D71F40,
// ;Pin wiget 0x1E - S/PDIF-OUT1
0x01E71CF0,
0x01E71D11,
0x01E71E11,
0x01E71F41,
// ;Pin wiget 0x21 - HP-OUT (Port-I)
0x02171C1F,
0x02171D10,
0x02171E21,
0x02171F03,
// ;Wiget noe 0x20 :
0x0205006D,
0x02048900,
0x02050078,
0x02049615,
// ;Wiget noe 0x20 - 1 :
0x02050006,
0x02042101,
0x02050019,
0x02041A57,
// ;Wiget noe 0x20 - 2 :
0x0205001A,
0x02044000,
0x02050037,
0x0204802A,
// ;Wiget noe 0x20 - 3 :
0x0205000D,
0x02042800,
0x0205000E,
0x02046E00,
// ;Wiget noe 0x20 - 4 :
0x0205001B,
0x0204100B,
0x02050064,
0x0204E737,
// ;Wiget noe 0x20 - 5 :
0x02050067,
0x020446A3,
0x0205006A,
0x02042F3E
};

UINT32 mAzaliaVerbTableData9[] = {
  //
  // Realtek Audio Verb Table - 0x10EC0268/0000/00
  //
  // Pin Complex (NID 0x12 ) D-Mic In 1/2
  //
  0x01271C30,
  0x01271D09,
  0x01271EA3,
  0x01271F99,
  //
  // Pin Complex (NID 0x13 ) D-Mic In 3/4
  //
  0x01371C30,
  0x01371D09,
  0x01371EA3,
  0x01371F99,
  //
  // Pin Complex (NID 0x14 ) SPK
  //
  0x01471C20,
  0x01471D00,
  0x01471E13,
  0x01471F59,
  //
  // Pin Complex (NID 0x15 ) HP Out
  //
  0x01571C10,
  0x01571D40,
  0x01571E21,
  0x01571F01,
  //
  // Pin Complex (NID 0x16 ) Line out (N/P)
  //
  0x01671CF0,
  0x01671D11,
  0x01671E11,
  0x01671F41,
  //
  // Pin Complex (NID 0x18 ) Mic In
  //
  0x01871C50,
  0x01871D90,
  0x01871EA1,
  0x01871F01,
  //
  // Pin Complex (NID 0x19 ) Mic In 2
  //
  0x01971C60,
  0x01971D90,
  0x01971EA1,
  0x01971F02,
  //
  // Pin Complex (NID 0x1A ) Line in
  //
  0x01A71C40,
  0x01A71D30,
  0x01A71E81,
  0x01A71F01,
  //
  // Pin Complex (NID 0x1C ) SPK (N/P)
  //
  0x01C71CF0,
  0x01C71D11,
  0x01C71E11,
  0x01C71F41,
  //
  // Pin Complex (NID 0x1D ) Line In (N/P)
  //
  0x01D71CF0,
  0x01D71D01,
  0x01D71E83,
  0x01D71F59,
  //
  // Pin Complex (NID 0x1E ) SPDIF Out
  //
  0x01E71C30,
  0x01E71D10,
  0x01E71E45,
  0x01E71F01
};

UINT32 mAzaliaVerbTableData10[] = {

//===================================================================================================
//
//                               Realtek Semiconductor Corp.
//
//===================================================================================================

//Realtek High Definition Audio Configuration - Version : 5.0.2.1
//Realtek HD Audio Codec : ALC898
//PCI PnP ID : PCI\VEN_0000&DEV_0000&SUBSYS_00000000
//HDA Codec PnP ID : HDAUDIO\FUNC_01&VEN_10EC&DEV_0899&SUBSYS_00000000
//The number of verb command block : 18

//    NID 0x11 : 0x40178000
//    NID 0x12 : 0x411111F0
//    NID 0x14 : 0x01014010
//    NID 0x15 : 0x01011012
//    NID 0x16 : 0x01016011
//    NID 0x17 : 0x411111F0
//    NID 0x18 : 0x01A19040
//    NID 0x19 : 0x02A19041
//    NID 0x1A : 0x0181304F
//    NID 0x1B : 0x02214020
//    NID 0x1C : 0x411111F0
//    NID 0x1D : 0x4036F601
//    NID 0x1E : 0x01451130
//    NID 0x1F : 0x411111F0


//===== HDA Codec Subsystem ID Verb-table =====
//HDA Codec Subsystem ID  : 0x00000000
0x00172099, //AptioV server override start
0x00172108,
0x001722EC,
0x00172310, //AptioV server override end


//===== Pin Widget Verb-table =====
//Widget node 0x01 :
0x0017FF00,
0x0017FF00,
0x0017FF00,
0x0017FF00,
//Pin widget 0x11 - S/PDIF-OUT2
0x01171C00,
0x01171D80,
0x01171E17,
0x01171F40,
//Pin widget 0x12 - DMIC
0x01271CF0,
0x01271D11,
0x01271E11,
0x01271F41,
//Pin widget 0x14 - FRONT (Port-D)
0x01471C10,
0x01471D40,
0x01471E01,
0x01471F01,
//Pin widget 0x15 - SURR (Port-A)
0x01571C12,
0x01571D10,
0x01571E01,
0x01571F01,
//Pin widget 0x16 - CEN/LFE (Port-G)
0x01671C11,
0x01671D60,
0x01671E01,
0x01671F01,
//Pin widget 0x17 - SIDESURR (Port-H)
0x01771CF0,
0x01771D11,
0x01771E11,
0x01771F41,
//Pin widget 0x18 - MIC1 (Port-B)
0x01871C40,
0x01871D90,
0x01871EA1,
0x01871F01,
//Pin widget 0x19 - MIC2 (Port-F)
0x01971C41,
0x01971D90,
0x01971EA1,
0x01971F02,
//Pin widget 0x1A - LINE1 (Port-C)
0x01A71C4F,
0x01A71D30,
0x01A71E81,
0x01A71F01,
//Pin widget 0x1B - LINE2 (Port-E)
0x01B71C20,
0x01B71D40,
0x01B71E21,
0x01B71F02,
//Pin widget 0x1C - CD-IN
0x01C71CF0,
0x01C71D11,
0x01C71E11,
0x01C71F41,
//Pin widget 0x1D - BEEP-IN
0x01D71C01,
0x01D71DF6,
0x01D71E36,
0x01D71F40,
//Pin widget 0x1E - S/PDIF-OUT1
0x01E71C30,
0x01E71D11,
0x01E71E45,
0x01E71F01,
//Pin widget 0x1F - S/PDIF-IN
0x01F71CF0,
0x01F71D11,
0x01F71E11,
0x01F71F41,
//Widget node 0x20 :
0x02050007,
0x02040180,
0x02050001,
0x0204C7AA,
//Widget node 0x20 - 1 :
0x02050015,
0x0204026A,
0x02050008,
0x02040071
};

UINT32 mAzaliaVerbTableData11[] = {

//===================================================================================================
//
//                               Realtek Semiconductor Corp.
//
//===================================================================================================

//Realtek High Definition Audio Configuration - Version : 5.0.2.2
//Realtek HD Audio Codec : ALC892
//PCI PnP ID : PCI\VEN_8086&DEV_2668&SUBSYS_00000000
//HDA Codec PnP ID : HDAUDIO\FUNC_01&VEN_10EC&DEV_0892&SUBSYS_00000000
//The number of verb command block : 17

//    NID 0x11 : 0x40130000
//    NID 0x12 : 0x411111F0
//    NID 0x14 : 0x01014010
//    NID 0x15 : 0x01011012
//    NID 0x16 : 0x01016011
//    NID 0x17 : 0x01012014
//    NID 0x18 : 0x01A19030
//    NID 0x19 : 0x02A19040
//    NID 0x1A : 0x0181303F
//    NID 0x1B : 0x02214020
//    NID 0x1C : 0x411111F0
//    NID 0x1D : 0x4036F601
//    NID 0x1E : 0x411111F0
//    NID 0x1F : 0x411111F0


//===== HDA Codec Subsystem ID Verb-table =====
//HDA Codec Subsystem ID  : 0x00000000
0x00172092, //AptioV server override start
0x00172108,
0x001722EC,
0x00172310, //AptioV server override end


//===== Pin Widget Verb-table =====
//Widget node 0x01 :
0x0017FF00,
0x0017FF00,
0x0017FF00,
0x0017FF00,
//Pin widget 0x11 - S/PDIF-OUT2
0x01171C00,
0x01171D00,
0x01171E13,
0x01171F40,
//Pin widget 0x12 - DMIC
0x01271CF0,
0x01271D11,
0x01271E11,
0x01271F41,
//Pin widget 0x14 - FRONT (Port-D)
0x01471C10,
0x01471D40,
0x01471E01,
0x01471F01,
//Pin widget 0x15 - SURR (Port-A)
0x01571C12,
0x01571D10,
0x01571E01,
0x01571F01,
//Pin widget 0x16 - CEN/LFE (Port-G)
0x01671C11,
0x01671D60,
0x01671E01,
0x01671F01,
//Pin widget 0x17 - SIDESURR (Port-H)
0x01771C14,
0x01771D20,
0x01771E01,
0x01771F01,
//Pin widget 0x18 - MIC1 (Port-B)
0x01871C30,
0x01871D90,
0x01871EA1,
0x01871F01,
//Pin widget 0x19 - MIC2 (Port-F)
0x01971C40,
0x01971D90,
0x01971EA1,
0x01971F02,
//Pin widget 0x1A - LINE1 (Port-C)
0x01A71C3F,
0x01A71D30,
0x01A71E81,
0x01A71F01,
//Pin widget 0x1B - LINE2 (Port-E)
0x01B71C20,
0x01B71D40,
0x01B71E21,
0x01B71F02,
//Pin widget 0x1C - CD-IN
0x01C71CF0,
0x01C71D11,
0x01C71E11,
0x01C71F41,
//Pin widget 0x1D - BEEP-IN
0x01D71C01,
0x01D71DF6,
0x01D71E36,
0x01D71F40,
//Pin widget 0x1E - S/PDIF-OUT
0x01E71CF0,
0x01E71D11,
0x01E71E11,
0x01E71F41,
//Pin widget 0x1F - S/PDIF-IN
0x01F71CF0,
0x01F71D11,
0x01F71E11,
0x01F71F41,
//Widget node 0x20 :
0x02050007,
0x020409C8,
0x02050007,
0x020409C8
};

PCH_AZALIA_VERB_TABLE mAzaliaVerbTable[] = {
  {
    //
    // VerbTable: (for Slim River, FFDS3)
    //  Revision ID = 0x00
    //  Codec Verb Table For AZALIA
    //  Codec Address: CAd value (0/1/2)
    //  Codec Vendor: 0x10EC0272
    //
    {
      0x10EC0272, // Vendor ID/Device ID
      0x0000,     // SubSystem ID
      0x00,       // Revision ID
      0x01,       // Front panel support (1=yes, 2=no)
      0x000E,     // Number of Rear Jacks
      0x0002      // Number of Front Jacks
    },
    0             // Pointer to verb table data, need to be inited in the code.
  },
  {
    //
    //  VerbTable: (for Buffalo Trail)
    //  Revision ID = 0x00
    //  Codec Verb Table For AZALIA
    //  Codec Address: CAd value (0/1/2)
    //  Codec Vendor: 0x10EC0269
    //
    {
      0x10EC0269, // Vendor ID/Device ID
      0x0000,     // SubSystem ID
      0x00,       // Revision ID
      0x01,       // Front panel support (1=yes, 2=no)
      0x000A,     // Number of Rear Jacks
      0x0002      // Number of Front Jacks
    },
    0             // Pointer to verb table data, need to be inited in the code.
  },
  {
    //
    //  VerbTable: (RealTek ALC888)
    //  Revision ID = 0xFF
    //  Codec Verb Table For Redfort
    //  Codec Address: CAd value (0/1/2)
    //  Codec Vendor: 0x10EC0888
    //
    {
      0x10EC0888, // Vendor ID/Device ID
      0x0000,     // SubSystem ID
      0xFF,       // Revision ID
      0x01,       // Front panel support (1=yes, 2=no)
      0x000B,     // Number of Rear Jacks
      0x0002      // Number of Front Jacks
    },
    0             // Pointer to verb table data, need to be inited in the code.
  },
  {
    //
    //  VerbTable: (RealTek ALC885)
    //  Revision ID = 0xFF
    //  Codec Verb Table For Redfort
    //  Codec Address: CAd value (0/1/2)
    //  Codec Vendor: 0x10EC0885
    //
    {
      0x10EC0885, // Vendor ID/Device ID
      0x0000,     // SubSystem ID
      0xFF,       // Revision ID
      0x01,       // Front panel support (1=yes, 2=no)
      0x000B,     // Number of Rear Jacks
      0x0002      // Number of Front Jacks
    },
    0             // Pointer to verb table data, need to be inited in the code.
  },
  {
    //
    //  VerbTable: (IDT 92HD81)
    //  Revision ID = 0xFF
    //  Codec Vendor: 0x111D7605
    //
    {
      0x111D76d5, // Vendor ID/Device ID
      0x0000,     // SubSystem ID
      0xFF,       // Revision ID
      0x01,       // Front panel support (1=yes, 2=no)
      0x0008,     // Number of Rear Jacks
      0x0002      // Number of Front Jacks
    },
    0             // Pointer to verb table data, need to be inited in the code.
  },
  {
    //
    //  VerbTable: (RealTek ALC889)
    //  Revision ID = 0xFF
    //  Codec Verb Table For LosLunas2
    //  Codec Address: CAd value (0/1/2)
    //  Codec Vendor: 0x10EC0889
    //
    {
      0x10EC0889, // Vendor ID/Device ID
      0x0000,     // SubSystem ID
      0xFF,       // Revision ID
      0x01,       // Front panel support (1=yes, 2=no)
      0x000B,     // Number of Rear Jacks
      0x0002      // Number of Front Jacks
    },
    0             // Pointer to verb table data, need to be inited in the code.
  },
  {
    //
    //  VerbTable: (RealTek ALC262)
    //  Revision ID = 0xFF
    //  Codec Verb Table For SharkBay
    //  Codec Address: CAd value (0/1/2)
    //  Codec Vendor: 0x10EC0262
    //
    {
      0x10EC0262, // Vendor ID/Device ID
      0x0000,     // SubSystem ID
      0xFF,       // Revision ID
      0x01,       // Front panel support (1=yes, 2=no)
      0x000B,     // Number of Rear Jacks
      0x0003      // Number of Front Jacks
    },
    0             // Pointer to verb table data, need to be inited in the code.
  },
  {
    //
    //  VerbTable: (RealTek ALC282)
    //  Revision ID = 0xff
    //  Codec Verb Table For Azalia on SharkBay-WhiteBluff refresh and Haswell ULT FFRD Harris Beach, WTM1
    //  Codec Address: CAd value (0/1/2)
    //  Codec Vendor: 0x10EC0282
    //
    {
      0x10EC0282, // Vendor ID/Device ID
      0x0000,     // SubSystem ID
      0xFF,       // Revision ID
      0x01,       // Front panel support (1=yes, 2=no)
      0x000C,     // Number of Rear Jacks, 0x0010 for Harris Beach, 0x000B for WTM1
      0x0002      // Number of Front Jacks
    },
    0             // Pointer to verb table data, need to be inited in the code.
  },
  {
    //
    //  VerbTable: (RealTek ALC268)
    //  Revision ID = 0x00
    //  Codec Verb Table For Azalia on SharkBay
    //  Codec Address: CAd value (0/1/2)
    //  Codec Vendor: 0x10EC0268
    //
    {
      0x10EC0268, // Vendor ID/Device ID
      0x0000,     // SubSystem ID
      0x00,       // Revision ID
      0x01,       // Front panel support (1=yes, 2=no)
      0x0009,     // Number of Rear Jacks
      0x0002      // Number of Front Jacks
    },
    0             // Pointer to verb table data, need to be inited in the code.
  },
  {
    //
    //  VerbTable: (RealTek ALC898)
    //  Revision ID = 0xff
    //  Codec Verb Table For Azalia on Lituya Bay
    //  Codec Address: CAd value (0/1/2)
    //  Codec Vendor: 0x10EC0899
    //
    {
      0x10EC0899, // Vendor ID/Device ID
      0x0000,     // SubSystem ID
      0xFF,       // Revision ID
      0x01,       // Front panel support (1=yes, 2=no)
      0x0010,     // Number of Rear Jacks
      0x0002      // Number of Front Jacks
    },
    0             // Pointer to verb table data, need to be inited in the code.
  },
  {
    //
    //  VerbTable: (RealTek ALC892)
    //  Revision ID = 0xFF
    //  Codec Verb Table For Azalia on Aztec City
    //  Codec Address: CAd value (0/1/2)
    //  Codec Vendor: 0x10EC0892
    //
    {
      0x10EC0892, // Vendor ID/Device ID
      0x0000,     // SubSystem ID
      0xFF,       // Revision ID
      0x01,       // Front panel support (1=yes, 2=no)
      0x000F,     // Number of Rear Jacks
      0x0002      // Number of Front Jacks
    },
    0             // Pointer to verb table data, need to be inited in the code.
  }
};

#define SW_SMI_BIOS_LOCK              0xA9
#define PCI_CLASS_NETWORK             0x02
#define PCI_CLASS_NETWORK_ETHERNET    0x00
#define PCI_CLASS_NETWORK_OTHER       0x80
//
// HSD 4169006 Unlock option for Overcurrent pins for NOA usage, WA-to enable policy variable earlier in code 
//
#define SV_USB_FOR_NOA_LOCK           1
#define SV_USB_FOR_NOA_UNLOCK         2

PCH_PCIE_DEVICE_ASPM_OVERRIDE mDevAspmOverride[] = {
  ///
  /// Intel PRO/Wireless
  ///
  {0x8086, 0x422b, 0xff, 0xff, 0xff, PchPcieAspmL1},
  {0x8086, 0x422c, 0xff, 0xff, 0xff, PchPcieAspmL1},
  {0x8086, 0x4238, 0xff, 0xff, 0xff, PchPcieAspmL1},
  {0x8086, 0x4239, 0xff, 0xff, 0xff, PchPcieAspmL1},
  ///
  /// Intel WiMAX/WiFi Link
  ///
  {0x8086, 0x0082, 0xff, 0xff, 0xff, PchPcieAspmL1},
  {0x8086, 0x0085, 0xff, 0xff, 0xff, PchPcieAspmL1},
  {0x8086, 0x0083, 0xff, 0xff, 0xff, PchPcieAspmL1},
  {0x8086, 0x0084, 0xff, 0xff, 0xff, PchPcieAspmL1},
  {0x8086, 0x0086, 0xff, 0xff, 0xff, PchPcieAspmL1},
  {0x8086, 0x0087, 0xff, 0xff, 0xff, PchPcieAspmL1},
  {0x8086, 0x0088, 0xff, 0xff, 0xff, PchPcieAspmL1},
  {0x8086, 0x0089, 0xff, 0xff, 0xff, PchPcieAspmL1},
  {0x8086, 0x008F, 0xff, 0xff, 0xff, PchPcieAspmL1},
  {0x8086, 0x0090, 0xff, 0xff, 0xff, PchPcieAspmL1},
  ///
  /// Intel Crane Peak WLAN NIC
  ///
  {0x8086, 0x08AE, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  {0x8086, 0x08AF, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  ///
  /// Intel Crane Peak w/BT WLAN NIC
  ///
  {0x8086, 0x0896, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  {0x8086, 0x0897, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  ///
  /// Intel Kelsey Peak WiFi, WiMax
  ///
  {0x8086, 0x0885, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  {0x8086, 0x0886, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  ///
  /// Intel Centrino Wireless-N 105
  ///
  {0x8086, 0x0894, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  {0x8086, 0x0895, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  ///
  /// Intel Centrino Wireless-N 135
  ///
  {0x8086, 0x0892, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  {0x8086, 0x0893, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  ///
  /// Intel Centrino Wireless-N 2200
  ///
  {0x8086, 0x0890, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  {0x8086, 0x0891, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  ///
  /// Intel Centrino Wireless-N 2230
  ///
  {0x8086, 0x0887, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  {0x8086, 0x0888, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  ///
  /// Intel Centrino Wireless-N 6235
  ///
  {0x8086, 0x088E, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  {0x8086, 0x088F, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  ///
  /// Intel CampPeak 2 Wifi
  ///
  {0x8086, 0x08B5, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  {0x8086, 0x08B6, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  ///
  /// Intel WilkinsPeak 1 Wifi
  ///
  {0x8086, 0x08B3, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  {0x8086, 0x08B4, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  ///
  /// Intel Wilkins Peak 2 Wifi
  ///
  {0x8086, 0x08B1, 0x3a, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmDisabled},
  {0x8086, 0x08B1, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  {0x8086, 0x08B2, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1},
  ///
  /// Intel Wilkins Peak PF Wifi
  ///
  {0x8086, 0x08B0, 0xff, PCI_CLASS_NETWORK, PCI_CLASS_NETWORK_OTHER, PchPcieAspmL1}

};


EFI_STATUS
SetupAzalia (
  IN     DXE_PCH_PLATFORM_POLICY_PROTOCOL   *DxePlatformPchPolicy
  );

/**
  Update PCH platform policy from test menu options

  @param[in] Event                   The event that triggered this notification function  
  @param[in] ParentImageHandle       Pointer to the notification functions context
  
  @retval    None
**/
EFI_STATUS
EFIAPI
UpdateSvDxePchPlatformPolicy (
  IN SYSTEM_CONFIGURATION                       *SetupVariables,  
  IN OUT DXE_PCH_PLATFORM_POLICY_PROTOCOL       *DxePlatformPchPolicy    
  );

/**
  RegisterPchInitSyncCallback

  @param[in] ParentImageHandle       Pointer to the notification functions context
  @param[in] Whatever                      Whatever you like  
  @retval    None
**/
EFI_STATUS
EFIAPI
RegisterPchInitSyncCallback (
  IN     DXE_PCH_PLATFORM_POLICY_PROTOCOL   *This
  );
#endif
