/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:
  
  TridentData.h
   
Abstract:

  Define softstraps which are fixed up according to Trident board installed
  Trident board type is read from PLD and is installed only on Kahuna

--*/

typedef struct _SOFTSTRAP_ONLY_ENTRY {
  UINT8  SoftStrapNumber;
  UINT8  BitfieldOffset;
  UINT8  BitfieldLength;
  UINT32 Value; 
} SOFTSTRAP_ONLY_ENTRY;


typedef struct _TRIDENT_SOFTSTRAP_FIXUP_ENTRY {
  UINT16  TridentId;
  SOFTSTRAP_ONLY_ENTRY SoftstrapFixupEntry[7];
} TRIDENT_SOFTSTRAP_FIXUP_ENTRY;


static const TRIDENT_SOFTSTRAP_FIXUP_ENTRY TridentSoftstrapFixupTable[16] = {

  {0x70,                  // Oregon Cherry (default)
    {
      {4,  2,  2, 0x00},      // SATA5_PCIEP2_MODE	
      {9,  30, 2, 0x00},      // SATA4_PCIEP1_MODE
      {9,  20, 2, 0x01},      // USB3P3_PCIEP2_MODE
      {9,  18, 2, 0x01},      // USB3P2_PCIEP1_MODE
      {9,  2,  2, 0x03},      // PCIEPCS2 "1x4 Ports5(x4),  Ports6-8(Disable)"
      {9,  0,  2, 0x01},      // PCIEPCS1 "1x2, 2x1 Port1(x2), Port2(Disable), Ports 3,4(x1)"
    },
  },

  {0x71,                  // Chokecherry
    {
      {4,  2,  2, 0x00},      // SATA5_PCIEP2_MODE		
      {9,  30, 2, 0x00},      // SATA4_PCIEP1_MODE
      {9,  20, 2, 0x01},      // USB3P3_PCIEP2_MODE
      {9,  18, 2, 0x01},      // USB3P2_PCIEP1_MODE
      {9,  2,  2, 0x00},      // PCIEPCS2 "4x1 Ports5-8(x1)"
      {9,  0,  2, 0x01},      // PCIEPCS1 "1x2, 2x1 Port1(x2), Port2(Disable), Ports 3,4(x1)"
    },
  },

  {0x72,                  // Chinese Plum-Cherry
    {
      {4,  2,  2, 0x00},      // SATA5_PCIEP2_MODE
      {9,  30, 2, 0x00},      // SATA4_PCIEP1_MODE
      {9,  20, 2, 0x00},      // USB3P3_PCIEP2_MODE
      {9,  18, 2, 0x00},      // USB3P2_PCIEP1_MODE
      {9,  2,  2, 0x00},      // PCIEPCS2 "4x1 Ports5-8(x1)"
      {9,  0,  2, 0x03},      // PCIEPCS1 "1x4 Port1 (x4), Ports 2-4(Disable)"
    },
  },

  {0x73,                  // Taiwan Cherry
    {
      {4,  2,  2, 0x00},      // SATA5_PCIEP2_MODE	
      {9,  30, 2, 0x00},      // SATA4_PCIEP1_MODE
      {9,  20, 2, 0x00},      // USB3P3_PCIEP2_MODE
      {9,  18, 2, 0x00},      // USB3P2_PCIEP1_MODE
      {9,  2,  2, 0x00},      // PCIEPCS2 "4x1 Ports5-8(x1)"
      {9,  0,  2, 0x02},      // PCIEPCS1 "2x2 Port1(x2) Port3(x2), Ports2,4 (disable)"
    },
  },

  {0x74,                  // Korean Cherry
    {
      {4,  2,  2, 0x00},      // SATA5_PCIEP2_MODE	
      {9,  30, 2, 0x00},      // SATA4_PCIEP1_MODE
      {9,  20, 2, 0x00},      // USB3P3_PCIEP2_MODE
      {9,  18, 2, 0x00},      // USB3P2_PCIEP1_MODE
      {9,  2,  2, 0x03},      // PCIEPCS2 "1x4 Port5 (x4), Ports 6-8(Disable)"
      {9,  0,  2, 0x00},      // PCIEPCS1 "4x1 Ports1-4(x1)"
    },
  },

  {0x75,                  // Japanese Cherry
    {
      {4,  2,  2, 0x00},      // SATA5_PCIEP2_MODE
      {9,  30, 2, 0x00},      // SATA4_PCIEP1_MODE
      {9,  20, 2, 0x00},      // USB3P3_PCIEP2_MODE
      {9,  18, 2, 0x00},      // USB3P2_PCIEP1_MODE
      {9,  2,  2, 0x02},      // PCIEPCS2  "2x2 Port5(x2) Port7(x2), Ports 6,8 (disable)"
      {9,  0,  2, 0x00},      // PCIEPCS1  "4x1 Ports1-4(x1)"
    },
  },

  {0x76,                  // Wild Cherry
    {
      {4,  2,  2, 0x00},      // SATA5_PCIEP2_MODE	
      {9,  30, 2, 0x00},      // SATA4_PCIEP1_MODE
      {9,  20, 2, 0x00},      // USB3P3_PCIEP2_MODE
      {9,  18, 2, 0x00},      // USB3P2_PCIEP1_MODE
      {9,  2,  2, 0x00},      // PCIEPCS2 "4x1 Ports5-8(x1)"
      {9,  0,  2, 0x00},      // PCIEPCS1 "4x1 Ports1-4(x1)"
    },
  },

  {0x77,                  // Sour Cherry
    {
      {4,  2,  2, 0x00},      // SATA5_PCIEP2_MODE	
      {9,  30, 2, 0x00},      // SATA4_PCIEP1_MODE
      {9,  20, 2, 0x00},      // USB3P3_PCIEP2_MODE
      {9,  18, 2, 0x00},      // USB3P2_PCIEP1_MODE
      {9,  2,  2, 0x01},      // PCIEPCS2 "1x2, 2x1 Port5(x2), Port6(disable), Port7,8(x1)"
      {9,  0,  2, 0x01},      // PCIEPCS1 "1x2, 2x1 Port1(x2), Port2(disable), Port3,4(x1)"
    },
  },

  {0x78,                  // Sand Cherry
    {
      {4,  2,  2, 0x01},      // SATA5_PCIEP2_MODE		
      {9,  30, 2, 0x01},      // SATA4_PCIEP1_MODE
      {9,  20, 2, 0x01},      // USB3P3_PCIEP2_MODE
      {9,  18, 2, 0x01},      // USB3P2_PCIEP1_MODE
      {9,  2,  2, 0x00},      // PCIEPCS2 "4x1 Ports5-8(x1)"
      {9,  0,  2, 0x03},      // PCIEPCS1 "1x4 Port1 (x4), Ports 2-4(Disable)"
    },
  },

  {0x79,                  // Black Cherry
    {
      {4,  2,  2, 0x01},      // SATA5_PCIEP2_MODE	
      {9,  30, 2, 0x01},      // SATA4_PCIEP1_MODE
      {9,  20, 2, 0x01},      // USB3P3_PCIEP2_MODE
      {9,  18, 2, 0x01},      // USB3P2_PCIEP1_MODE
      {9,  2,  2, 0x00},      // PCIEPCS2 "4x1 Ports5-8(x1)"
      {9,  0,  2, 0x02},      // PCIEPCS1 "2x2 Port1(x2) Port3(x2), Ports2,4 (disable)"
    },
  },

  {0x7A,                  // Higan Cherry
    {
      {4,  2,  2, 0x00},      // SATA5_PCIEP2_MODE		
      {9,  30, 2, 0x00},      // SATA4_PCIEP1_MODE
      {9,  20, 2, 0x00},      // USB3P3_PCIEP2_MODE
      {9,  18, 2, 0x00},      // USB3P2_PCIEP1_MODE
      {9,  2,  2, 0x03},      // PCIEPCS2 "1x4 Port5(x4), Ports 6-8(disable)"
      {9,  0,  2, 0x03},      // PCIEPCS1 "1x4 Port1(x4), Ports 2-4(disable)"
    },
  },

  {0x7B,                  // Clove Cherry
    {
      {4,  2,  2, 0x01},      // SATA5_PCIEP2_MODE	
      {9,  30, 2, 0x01},      // SATA4_PCIEP1_MODE
      {9,  20, 2, 0x01},      // USB3P3_PCIEP2_MODE
      {9,  18, 2, 0x01},      // USB3P2_PCIEP1_MODE
      {9,  2,  2, 0x03},      // PCIEPCS2 "1x4 Port5(x4), Ports 6-8(disable)"
      {9,  0,  2, 0x03},      // PCIEPCS1 "1x4 Port1(x4), Ports 2-4(disable)"
    },
  },

  {0x7C,                  // Fuji Cherry
    {
      {4,  2,  2, 0x01},      // SATA5_PCIEP2_MODE
      {9,  30, 2, 0x01},      // SATA4_PCIEP1_MODE
      {9,  20, 2, 0x01},      // USB3P3_PCIEP2_MODE
      {9,  18, 2, 0x01},      // USB3P2_PCIEP1_MODE
      {9,  2,  2, 0x00},      // PCIEPCS2 "4x1 Ports5-8(x1)"
      {9,  0,  2, 0x00},      // PCIEPCS1 "4x1 Ports1-4(x1)"
    },
  },

  {0x7D,                  // Mountain Cherry
    {
      {4,  2,  2, 0x01},      // SATA5_PCIEP2_MODE	
      {9,  30, 2, 0x01},      // SATA4_PCIEP1_MODE
      {9,  20, 2, 0x01},      // USB3P3_PCIEP2_MODE
      {9,  18, 2, 0x01},      // USB3P2_PCIEP1_MODE
      {9,  2,  2, 0x01},      // PCIEPCS2 "1x2, 2x1 Port5(x2), Port6(disable), Port7,8(x1)" 
      {9,  0,  2, 0x01},      // PCIEPCS1 "1x2, 2x1 Port1(x2), Port2(disable), Port3,4(x1)"
    },
  },

  {0x7E,                  // Dwarf Cherry
    {
      {4,  2,  2, 0x01},      // SATA5_PCIEP2_MODE		
      {9,  30, 2, 0x01},      // SATA4_PCIEP1_MODE
      {9,  20, 2, 0x00},      // USB3P3_PCIEP2_MODE
      {9,  18, 2, 0x01},      // USB3P2_PCIEP1_MODE
      {9,  2,  2, 0x01},      // PCIEPCS2 "1x2, 2x1 Port5(x2), Port6(disable), Port7,8(x1)" 
      {9,  0,  2, 0x00},      // PCIEPCS1 "4x1 Ports1-4(x1)"
    },
  },

  {0x7F,                  // Rock Cherry
    {
      {4,  2,  2, 0x01},      // SATA5_PCIEP2_MODE	
      {9,  30, 2, 0x01},      // SATA4_PCIEP1_MODE
      {9,  20, 2, 0x01},      // USB3P3_PCIEP2_MODE
      {9,  18, 2, 0x00},      // USB3P2_PCIEP1_MODE
      {9,  2,  2, 0x01},      // PCIEPCS2  "1x2, 2x1 Port5(x2), Port6(disable), Port7,8(x1)" 
      {9,  0,  2, 0x00},      // PCIEPCS1  "4x1 Ports1-4(x1)"
    },
  },
};
