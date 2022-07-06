/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  IioDfxInit.c

Abstract:

  Houses all code related to Initialize DFX 

Revision History:

--*/

#include "IioEarlyInitialize.h"



#define IS_RXRECIPE_FOR_HSX_DMI(RegOffset, AndMask) \
  ( \
    (RegOffset == CLSPHYCTL8_IIO_DFX_REG)        || /*rxsqexitw4alllanes, spare0, lnknumtightcheck*/ \
    ((RegOffset == CLSPHYCTL1_IIO_DFX_REG) &&       /*speedchangetime_constin1us, speedchangetime_constin4us*/ \
       (AndMask != 0xFFF81FFF))                  || /*Not apply for speedchangetime_gen3in1us*/ \
    (RegOffset == PXPSQCNT_IIO_DFX_REG)          || /*sqcnt*/ \
    (RegOffset == DEBUGCLUSTER1_IIO_DFX_REG)     || /*recexiteito, l1exiteito, spare_signals*/ \
    (RegOffset == DFXCHICKENBIT0_IIO_DFX_REG)    || /*cfg_lanewait_timer_setting*/ \
    (RegOffset == UNIPHYPMCTL_IIO_DFX_REG)       || /*rxsqex_cntrval*/ \
    (RegOffset == PRTPHYCTL_IIO_DFX_REG)         || /*kalign_mode*/ \
    (RegOffset == RX_CTLE_RDEG_GEN2_IIO_DFX_REG) || /*rx_ctle_rdegen2*/ \
    (RegOffset == RX_CTLE_PEAK_GEN2_0_IIO_DFX_REG)  /*rx_ctle_peak_gen2*/ \
  ) //s4985620, s4985617, s4168811

static CSR_ACCESS_OP_STRUCT HSX_ALL_RxRecipe[] = {
     /*
     * START 4166237: HSX Uniphy Recipe v0.6
     */
    // Register: port_ctl.rx_dfesum_mfc_10gen ([21:20]) = 2
    { 0, 4, PORT_CTL_IIO_DFX_REG, 0xFFCFFFFF, 0x00200000  },
    { 1, 4, PORT_CTL_IIO_DFX_REG, 0xFFCFFFFF, 0x00200000  },
    { 2, 4, PORT_CTL_IIO_DFX_REG, 0xFFCFFFFF, 0x00200000  },
    { 3, 4, PORT_CTL_IIO_DFX_REG, 0xFFCFFFFF, 0x00200000  },
    { 4, 4, PORT_CTL_IIO_DFX_REG, 0xFFCFFFFF, 0x00200000  },
    { 5, 4, PORT_CTL_IIO_DFX_REG, 0xFFCFFFFF, 0x00200000  },
    { 6, 4, PORT_CTL_IIO_DFX_REG, 0xFFCFFFFF, 0x00200000  },
    { 7, 4, PORT_CTL_IIO_DFX_REG, 0xFFCFFFFF, 0x00200000  },
    { 8, 4, PORT_CTL_IIO_DFX_REG, 0xFFCFFFFF, 0x00200000  },
    { 9, 4, PORT_CTL_IIO_DFX_REG, 0xFFCFFFFF, 0x00200000  },
    { 10, 4, PORT_CTL_IIO_DFX_REG, 0xFFCFFFFF, 0x00200000  },

    // Register: rx_afe_offsetcor_gain.bndl =    3
    { 0, 4, RX_AFE_OFFSETCOR_GAIN_IIO_DFX_REG, 0xFFFFFFF0, 0x0000000F },
    { 1, 4, RX_AFE_OFFSETCOR_GAIN_IIO_DFX_REG, 0xFFFFFF00, 0x000000FF },
    { 3, 4, RX_AFE_OFFSETCOR_GAIN_IIO_DFX_REG, 0xFFFF0000, 0x0000FFFF },
    { 7, 4, RX_AFE_OFFSETCOR_GAIN_IIO_DFX_REG, 0xFFFF0000, 0x0000FFFF },

    // Register: RX_VCM_DS_SEL.bndl0 ([1:0]) = 1
    { 0, 4, RX_VCM_DS_SEL_IIO_DFX_REG, 0xFFFFFFF0, 0x00000005 },
    { 1, 4, RX_VCM_DS_SEL_IIO_DFX_REG, 0xFFFFFF00, 0x00000055 },
    { 3, 4, RX_VCM_DS_SEL_IIO_DFX_REG, 0xFFFF0000, 0x00005555 },
    { 7, 4, RX_VCM_DS_SEL_IIO_DFX_REG, 0xFFFF0000, 0x00005555 },

    // Register: RX_VCM_SUM_SEL.bndl0 = 0
    { 0, 4, RX_VCM_SUM_SEL_IIO_DFX_REG, 0xFFFFFFFC, 0x00000000 },  //port 0
    { 1, 4, RX_VCM_SUM_SEL_IIO_DFX_REG, 0xFFFFFFF0, 0x00000000 },  //port 1A
    { 3, 4, RX_VCM_SUM_SEL_IIO_DFX_REG, 0xFFFFFF00, 0x00000000 },  //port 2A
    { 7, 4, RX_VCM_SUM_SEL_IIO_DFX_REG, 0xFFFFFF00, 0x00000000 },  //port 3A

    //Register: RX_VREF_SEL_GEN1.lane0/1 = 14
    { 0, 4, RX_VREF_SEL_GEN1_0_IIO_DFX_REG, 0xFFF00000, 0x000739CE },
    { 1, 4, RX_VREF_SEL_GEN1_0_IIO_DFX_REG, 0xC0000000, 0x1CE739CE },
    { 1, 4, RX_VREF_SEL_GEN1_1_IIO_DFX_REG, 0xC0000000, 0x000001CE },
    { 3, 4, RX_VREF_SEL_GEN1_0_IIO_DFX_REG, 0xC0000000, 0x1CE739CE },
    { 3, 4, RX_VREF_SEL_GEN1_1_IIO_DFX_REG, 0xC0000000, 0x1CE739CE },
    { 3, 4, RX_VREF_SEL_GEN1_2_IIO_DFX_REG, 0xFFF00000, 0x000739CE },
    { 7, 4, RX_VREF_SEL_GEN1_0_IIO_DFX_REG, 0xC0000000, 0x1CE739CE },
    { 7, 4, RX_VREF_SEL_GEN1_1_IIO_DFX_REG, 0xC0000000, 0x1CE739CE },
    { 7, 4, RX_VREF_SEL_GEN1_2_IIO_DFX_REG, 0xFFF00000, 0x000739CE },

    //Register: RX_VREF_SEL_NOTGEN1.lane0/1 = 7
    { 0, 4, RX_VREF_SEL_NOTGEN1_0_IIO_DFX_REG, 0xFFF00000, 0x00039CE7 },
    { 1, 4, RX_VREF_SEL_NOTGEN1_0_IIO_DFX_REG, 0xC0000000, 0x0E739CE7 },
    { 1, 4, RX_VREF_SEL_NOTGEN1_1_IIO_DFX_REG, 0xC0000000, 0x000000E7 },
    { 3, 4, RX_VREF_SEL_NOTGEN1_0_IIO_DFX_REG, 0xC0000000, 0x0E739CE7 },
    { 3, 4, RX_VREF_SEL_NOTGEN1_1_IIO_DFX_REG, 0xC0000000, 0x0E739CE7 },
    { 3, 4, RX_VREF_SEL_NOTGEN1_2_IIO_DFX_REG, 0xFFF00000, 0x00039CE7 },
    { 7, 4, RX_VREF_SEL_NOTGEN1_0_IIO_DFX_REG, 0xC0000000, 0x0E739CE7 },
    { 7, 4, RX_VREF_SEL_NOTGEN1_1_IIO_DFX_REG, 0xC0000000, 0x0E739CE7 },
    { 7, 4, RX_VREF_SEL_NOTGEN1_2_IIO_DFX_REG, 0xFFF00000, 0x00039CE7 },

    // Register: ig_acq                   4
    { 0, 4, IG_ACQ_IIO_DFX_REG, 0xFFFFFFC0, 0x00000024  },
    { 1, 4, IG_ACQ_IIO_DFX_REG, 0xFFFFF000, 0x00000924  },
    { 3, 4, IG_ACQ_IIO_DFX_REG, 0xFF000000, 0x00924924  },
    { 7, 4, IG_ACQ_IIO_DFX_REG, 0xFF000000, 0x00924924  },

    // Register: pg_track_0               8
    { 0, 4, PG_TRACK_0_IIO_DFX_REG, 0xFFFFF000, 0x00000208  },
    { 1, 4, PG_TRACK_0_IIO_DFX_REG, 0xFF000000, 0x00208208  },
    { 3, 4, PG_TRACK_0_IIO_DFX_REG, 0xF0000000, 0x08208208  },
    { 3, 4, PG_TRACK_1_IIO_DFX_REG, 0xFFFF0000, 0x00008208  },
    { 7, 4, PG_TRACK_0_IIO_DFX_REG, 0xF0000000, 0x08208208  },
    { 7, 4, PG_TRACK_1_IIO_DFX_REG, 0xFFFF0000, 0x00008208  },

    // Register: cdr_acq_length           2
    { 0, 4, CDR_ACQ_LENGTH_IIO_DFX_REG, 0xFFFFFF00, 0x000000AA },
    { 1, 4, CDR_ACQ_LENGTH_IIO_DFX_REG, 0xFFFF0000, 0x0000AAAA },
    { 3, 4, CDR_ACQ_LENGTH_IIO_DFX_REG, 0x00000000, 0xAAAAAAAA },
    { 7, 4, CDR_ACQ_LENGTH_IIO_DFX_REG, 0x00000000, 0xAAAAAAAA },

    // Register: BNDL_SPARE.bndl0 = 14h (20d)
    { 0, 4, R_BNDL_SPARE_0,0x00000000, 0x00140014 },
    { 1, 4, R_BNDL_SPARE_0,0x00000000, 0x00140014 },
    { 1, 4, R_BNDL_SPARE_1,0x00000000, 0x00140014 },
    { 3, 4, R_BNDL_SPARE_0,0x00000000, 0x00140014 },
    { 3, 4, R_BNDL_SPARE_1,0x00000000, 0x00140014 },
    { 3, 4, R_BNDL_SPARE_2, 0x00000000, 0x00140014 },
    { 3, 4, R_BNDL_SPARE_3, 0x00000000, 0x00140014 },
    { 7, 4, R_BNDL_SPARE_0, 0x00000000, 0x00140014 },
    { 7, 4, R_BNDL_SPARE_1, 0x00000000, 0x00140014 },
    { 7, 4, R_BNDL_SPARE_2, 0x00000000, 0x00140014 },
    { 7, 4, R_BNDL_SPARE_3, 0x00000000, 0x00140014 },

    // Register: RX_CTLE_PEAK_GEN1.bndl0 = "Peaking" v2 for 14dB
    { 0, 4, RX_CTLE_PEAK_GEN1_0_IIO_DFX_REG, 0xFFFFFC00, 0x000000E7 },
    { 1, 4, RX_CTLE_PEAK_GEN1_0_IIO_DFX_REG, 0xFFFC0000, 0x00039CE7 },
    { 3, 4, RX_CTLE_PEAK_GEN1_0_IIO_DFX_REG, 0xC0000000, 0x0E739CE7 },
    { 3, 4, RX_CTLE_PEAK_GEN1_1_IIO_DFX_REG, 0xFFFFFC00, 0x000000E7 },
    { 7, 4, RX_CTLE_PEAK_GEN1_0_IIO_DFX_REG, 0xC0000000, 0x0E739CE7 },
    { 7, 4, RX_CTLE_PEAK_GEN1_1_IIO_DFX_REG, 0xFFFFFC00, 0x000000E7 },

    // Register: RX_CTLE_PEAK_GEN2.bndl0 = "Peaking" v2 for 14dB
    { 0, 4, RX_CTLE_PEAK_GEN2_0_IIO_DFX_REG, 0xFFFFFC00, 0x000000E7 },
    { 1, 4, RX_CTLE_PEAK_GEN2_0_IIO_DFX_REG, 0xFFFC0000, 0x00039CE7 },
    { 3, 4, RX_CTLE_PEAK_GEN2_0_IIO_DFX_REG, 0xC0000000, 0x0E739CE7 },
    { 3, 4, RX_CTLE_PEAK_GEN2_1_IIO_DFX_REG, 0xFFFFFC00, 0x000000E7 },
    { 7, 4, RX_CTLE_PEAK_GEN2_0_IIO_DFX_REG, 0xC0000000, 0x0E739CE7 },
    { 7, 4, RX_CTLE_PEAK_GEN2_1_IIO_DFX_REG, 0xFFFFFC00, 0x000000E7 },

    // Register: RX_CTLE_PEAK_GEN3.bndl0 = "Peaking" v2 for 14dB
    { 0, 4, RX_CTLE_PEAK_GEN3_0_IIO_DFX_REG, 0xFFFFFC00, 0x000000E7 },
    { 1, 4, RX_CTLE_PEAK_GEN3_0_IIO_DFX_REG, 0xFFFC0000, 0x00039CE7 },
    { 3, 4, RX_CTLE_PEAK_GEN3_0_IIO_DFX_REG, 0xC0000000, 0x0E739CE7 },
    { 3, 4, RX_CTLE_PEAK_GEN3_1_IIO_DFX_REG, 0xFFFFFC00, 0x000000E7 },
    { 7, 4, RX_CTLE_PEAK_GEN3_0_IIO_DFX_REG, 0xC0000000, 0x0E739CE7 },
    { 7, 4, RX_CTLE_PEAK_GEN3_1_IIO_DFX_REG, 0xFFFFFC00, 0x000000E7 },

    // Register: RX_CTLE_RDEG_GEN1.bndl0 = 4
    { 0, 4, RX_CTLE_RDEG_GEN1_IIO_DFX_REG, 0xFFFFFF00, 0x00000044 },
    { 1, 4, RX_CTLE_RDEG_GEN1_IIO_DFX_REG, 0xFFFF0000, 0x00004444 },
    { 3, 4, RX_CTLE_RDEG_GEN1_IIO_DFX_REG, 0x00000000, 0x44444444 },
    { 7, 4, RX_CTLE_RDEG_GEN1_IIO_DFX_REG, 0x00000000, 0x44444444 },

    // Register:RX_CTLE_RDEG_GEN2_IIO_DFX_REG.bndl = '0100'B           // bndl - 4 bits wide
    { 0, 4, RX_CTLE_RDEG_GEN2_IIO_DFX_REG, 0xFFFFFF00, 0x00000044 },
    { 1, 4, RX_CTLE_RDEG_GEN2_IIO_DFX_REG, 0xFFFF0000, 0x00004444 },
    { 3, 4, RX_CTLE_RDEG_GEN2_IIO_DFX_REG, 0x00000000, 0x44444444 },
    { 7, 4, RX_CTLE_RDEG_GEN2_IIO_DFX_REG, 0x00000000, 0x44444444 },

    // Register: RX_CTLE_RDEG_GEN3.bndl0 = 4
    { 0, 4, RX_CTLE_RDEG_GEN3_IIO_DFX_REG, 0xFFFFFF00, 0x00000044 },
    { 1, 4, RX_CTLE_RDEG_GEN3_IIO_DFX_REG, 0xFFFF0000, 0x00004444 },
    { 3, 4, RX_CTLE_RDEG_GEN3_IIO_DFX_REG, 0x00000000, 0x44444444 },
    { 7, 4, RX_CTLE_RDEG_GEN3_IIO_DFX_REG, 0x00000000, 0x44444444 },

    // Register: DFXCHICKENBIT1_IIO_DFX_REG.remote_tx_preset_dfx = 8
    { 0, 4, DFXCHICKENBIT1_IIO_DFX_REG, 0xFFFFFFE1, 0x00000010 },
    { 1, 4, DFXCHICKENBIT1_IIO_DFX_REG, 0xFFFFFFE1, 0x00000010 },
    { 3, 4, DFXCHICKENBIT1_IIO_DFX_REG, 0xFFFFFFE1, 0x00000010 },
    { 7, 4, DFXCHICKENBIT1_IIO_DFX_REG, 0xFFFFFFE1, 0x00000010 },

    // Register: CC_SPARE.cc_spare = 0xC
    { 0xF, 4, CC_SPARE_IIO_DFX_GLOBAL_REG, 0xFFFF0000, 0x0000000C },

    // Register: CC_RX_CTL.rx_ctle_ctl = 0x63
    { 0xF, 4, CC_RX_CTL_IIO_DFX_GLOBAL_REG, 0xFF00FFFF, 0x00630000 },

    // Register: CC_TX_CTL.tx_rxdet_ctl = 2
    { 0xF, 4, CC_TX_CTL_IIO_DFX_GLOBAL_REG, 0xFFFFFFFC, 0x00000002 },

    // Register: CC_BIAS_TX.tx_vref_rxdetect = 0
    { 0xF, 1, CC_BIAS_TX_IIO_DFX_GLOBAL_REG, 0xFFFFF83F, 0x00000000 },

    // Register: UNIPHY_FUSE_CTRL.csr_override = 1
    { 0xF, 1, UNIPHY_FUSE_CTRL_IIO_DFX_GLOBAL_REG, 0xFFFFFFFE, 0x00000001 },

    /*
     * END 4166237: HSX Uniphy Recipe v0.6
     */

    /*
     * START 4166638: HSX Uniphy Recipe v2.0
     */
    // Register:PSA_CFG_GEN2_IIO_DFX_REG.bndl = '00'B                   // bndl - 2 bits wide
    { 0, 4, PSA_CFG_GEN2_IIO_DFX_REG, 0xFFFFFFF0, 0x00000000 },
    { 1, 4, PSA_CFG_GEN2_IIO_DFX_REG, 0xFFFFFF00, 0x00000000 },
    { 3, 4, PSA_CFG_GEN2_IIO_DFX_REG, 0xFFFF0000, 0x00000000 },
    { 7, 4, PSA_CFG_GEN2_IIO_DFX_REG, 0xFFFF0000, 0x00000000 },

    // Register:AGC_GAIN_ACQ_IIO_DFX_REG.bndl = '10'B                  // bndl - 2 bits wide
    { 0, 4, AGC_GAIN_ACQ_IIO_DFX_REG, 0xFFFFFFF0, 0x0000000a },
    { 1, 4, AGC_GAIN_ACQ_IIO_DFX_REG, 0xFFFFFF00, 0x000000aa },
    { 3, 4, AGC_GAIN_ACQ_IIO_DFX_REG, 0xFFFF0000, 0x0000aaaa },
    { 7, 4, AGC_GAIN_ACQ_IIO_DFX_REG, 0xFFFF0000, 0x0000aaaa },

    // Register:CLSPHYCTL8_IIO_DFX_REG.rxsqexitw4alllanes = '0'B       // Bit 9
    { 0, 4, CLSPHYCTL8_IIO_DFX_REG, 0xFFFFFDFF, 0x00000000 },
    { 1, 4, CLSPHYCTL8_IIO_DFX_REG, 0xFFFFFDFF, 0x00000000 },
    { 3, 4, CLSPHYCTL8_IIO_DFX_REG, 0xFFFFFDFF, 0x00000000 },
    { 7, 4, CLSPHYCTL8_IIO_DFX_REG, 0xFFFFFDFF, 0x00000000 },

    // Register:CLSPHYCTL8_IIO_DFX_REG.spare0 = '10001'B              // Bits 18:14
    { 0, 4, CLSPHYCTL8_IIO_DFX_REG, 0xFFF83FFF, 0x00044000 },
    { 1, 4, CLSPHYCTL8_IIO_DFX_REG, 0xFFF83FFF, 0x00044000 },
    { 3, 4, CLSPHYCTL8_IIO_DFX_REG, 0xFFF83FFF, 0x00044000 },
    { 7, 4, CLSPHYCTL8_IIO_DFX_REG, 0xFFF83FFF, 0x00044000 },

    // Register:CLSPHYCTL8_IIO_DFX_REG.lnktightnumcheck = '1'B        // Bit 27 
    { 0, 4, CLSPHYCTL8_IIO_DFX_REG, 0xF7FFFFFF, 0x08000000 },
    { 1, 4, CLSPHYCTL8_IIO_DFX_REG, 0xF7FFFFFF, 0x08000000 },
    { 3, 4, CLSPHYCTL8_IIO_DFX_REG, 0xF7FFFFFF, 0x08000000 },
    { 7, 4, CLSPHYCTL8_IIO_DFX_REG, 0xF7FFFFFF, 0x08000000 },

    // Register:CLSPHYCTL1_IIO_DFX_REG.speedchangetime_gen3in1us = '001010'B     // Bits 18:13 
    { 0, 4, CLSPHYCTL1_IIO_DFX_REG, 0xFFF81FFF, 0x00014000 },
    { 1, 4, CLSPHYCTL1_IIO_DFX_REG, 0xFFF81FFF, 0x00014000 },
    { 3, 4, CLSPHYCTL1_IIO_DFX_REG, 0xFFF81FFF, 0x00014000 },
    { 7, 4, CLSPHYCTL1_IIO_DFX_REG, 0xFFF81FFF, 0x00014000 },

    // Register:PXPSQCNT_IIO_DFX_REG.sqcnt = '0000000000100'B                   // Bits 12:0 
    { 0, 2, PXPSQCNT_IIO_DFX_REG, 0xFFFFE000, 0x00000004 },
    { 1, 2, PXPSQCNT_IIO_DFX_REG, 0xFFFFE000, 0x00000004 },
    { 2, 2, PXPSQCNT_IIO_DFX_REG, 0xFFFFE000, 0x00000004 },
    { 3, 2, PXPSQCNT_IIO_DFX_REG, 0xFFFFE000, 0x00000004 },
    { 4, 2, PXPSQCNT_IIO_DFX_REG, 0xFFFFE000, 0x00000004 },
    { 5, 2, PXPSQCNT_IIO_DFX_REG, 0xFFFFE000, 0x00000004 },
    { 6, 2, PXPSQCNT_IIO_DFX_REG, 0xFFFFE000, 0x00000004 },
    { 7, 2, PXPSQCNT_IIO_DFX_REG, 0xFFFFE000, 0x00000004 },
    { 8, 2, PXPSQCNT_IIO_DFX_REG, 0xFFFFE000, 0x00000004 },
    { 9, 2, PXPSQCNT_IIO_DFX_REG, 0xFFFFE000, 0x00000004 },
    { 10, 2, PXPSQCNT_IIO_DFX_REG, 0xFFFFE000, 0x00000004 },

    // Register:DEBUGCLUSTER1_IIO_DFX_REG.recexiteito = '00010000'B            // Bits 11:4 
    { 0, 4, DEBUGCLUSTER1_IIO_DFX_REG, 0xFFFFF00F, 0x00000100 },
    { 1, 4, DEBUGCLUSTER1_IIO_DFX_REG, 0xFFFFF00F, 0x00000100 },
    { 3, 4, DEBUGCLUSTER1_IIO_DFX_REG, 0xFFFFF00F, 0x00000100 },
    { 7, 4, DEBUGCLUSTER1_IIO_DFX_REG, 0xFFFFF00F, 0x00000100 },

    // Register:DEBUGCLUSTER1_IIO_DFX_REG.l1exiteito = '00010000'B              // Bits 26:19 
    { 0, 4, DEBUGCLUSTER1_IIO_DFX_REG, 0xF807FFFF, 0x00800000 },
    { 1, 4, DEBUGCLUSTER1_IIO_DFX_REG, 0xF807FFFF, 0x00800000 },
    { 3, 4, DEBUGCLUSTER1_IIO_DFX_REG, 0xF807FFFF, 0x00800000 },
    { 7, 4, DEBUGCLUSTER1_IIO_DFX_REG, 0xF807FFFF, 0x00800000 },

     // Register:DEBUGCLUSTER1_IIO_DFX_REG.spare_signals = '00001'B             // Bits 31:27 
    { 0, 4, DEBUGCLUSTER1_IIO_DFX_REG, 0x07FFFFFF, 0x08000000 },
    { 1, 4, DEBUGCLUSTER1_IIO_DFX_REG, 0x07FFFFFF, 0x08000000 },
    { 3, 4, DEBUGCLUSTER1_IIO_DFX_REG, 0x07FFFFFF, 0x08000000 },
    { 7, 4, DEBUGCLUSTER1_IIO_DFX_REG, 0x07FFFFFF, 0x08000000 },

     // Register:PXPPRTGEN3EQPRIV_IIO_DFX_REG.bypass_txeqcoeff_rule4b = '0'B    // Bit 2  
    { 0, 4, PXPPRTGEN3EQPRIV_IIO_DFX_REG, 0xFFFFFFFB, 0x00000000 },
    { 1, 4, PXPPRTGEN3EQPRIV_IIO_DFX_REG, 0xFFFFFFFB, 0x00000000 },
    { 3, 4, PXPPRTGEN3EQPRIV_IIO_DFX_REG, 0xFFFFFFFB, 0x00000000 },
    { 7, 4, PXPPRTGEN3EQPRIV_IIO_DFX_REG, 0xFFFFFFFB, 0x00000000 },

     // Register:PXPPRTGEN3EQPRIV_IIO_DFX_REG.dis_txdcbal = '0'B                // Bit 3  
    { 0, 4, PXPPRTGEN3EQPRIV_IIO_DFX_REG, 0xFFFFFFF7, 0x00000000 },
    { 1, 4, PXPPRTGEN3EQPRIV_IIO_DFX_REG, 0xFFFFFFF7, 0x00000000 },
    { 3, 4, PXPPRTGEN3EQPRIV_IIO_DFX_REG, 0xFFFFFFF7, 0x00000000 },
    { 7, 4, PXPPRTGEN3EQPRIV_IIO_DFX_REG, 0xFFFFFFF7, 0x00000000 },

     // Register:PXPPRTGEN3EQPRIV_IIO_DFX_REG.dis_rxdcbal = '0'B                // Bit 4  
    { 0, 4, PXPPRTGEN3EQPRIV_IIO_DFX_REG, 0xFFFFFFEF, 0x00000000 },
    { 1, 4, PXPPRTGEN3EQPRIV_IIO_DFX_REG, 0xFFFFFFEF, 0x00000000 },
    { 3, 4, PXPPRTGEN3EQPRIV_IIO_DFX_REG, 0xFFFFFFEF, 0x00000000 },
    { 7, 4, PXPPRTGEN3EQPRIV_IIO_DFX_REG, 0xFFFFFFEF, 0x00000000 },

     // Register:PXPPRTGEN3EQPRIV_IIO_DFX_REG.en_july0_7spec = '0'B             // Bit 1 
    { 0, 4, PXPPRTGEN3EQPRIV_IIO_DFX_REG, 0xFFFFFFFD, 0x00000000 },
    { 1, 4, PXPPRTGEN3EQPRIV_IIO_DFX_REG, 0xFFFFFFFD, 0x00000000 },
    { 3, 4, PXPPRTGEN3EQPRIV_IIO_DFX_REG, 0xFFFFFFFD, 0x00000000 },
    { 7, 4, PXPPRTGEN3EQPRIV_IIO_DFX_REG, 0xFFFFFFFD, 0x00000000 },

     // Register:PRTPHYCTL_IIO_DFX_REG.kalign_mode = '10'B                      // Bit 25:24 
    { 0, 4, PRTPHYCTL_IIO_DFX_REG, 0xFCFFFFFF, 0x02000000 },
    { 1, 4, PRTPHYCTL_IIO_DFX_REG, 0xFCFFFFFF, 0x02000000 },
    { 3, 4, PRTPHYCTL_IIO_DFX_REG, 0xFCFFFFFF, 0x02000000 },
    { 7, 4, PRTPHYCTL_IIO_DFX_REG, 0xFCFFFFFF, 0x02000000 },

     // Register:UNIPHYPMCTL_IIO_DFX_REG.rxsqex_cntrval = '1100'B               // Bit 11:8 
    { 0, 4, UNIPHYPMCTL_IIO_DFX_REG, 0xFFFFF0FF, 0x00000C00 },
    { 1, 4, UNIPHYPMCTL_IIO_DFX_REG, 0xFFFFF0FF, 0x00000C00 },
    { 3, 4, UNIPHYPMCTL_IIO_DFX_REG, 0xFFFFF0FF, 0x00000C00 },
    { 7, 4, UNIPHYPMCTL_IIO_DFX_REG, 0xFFFFF0FF, 0x00000C00 },

    // Register: CC_ICOMP_IIO_DFX_GLOBAL_REG.tx_icomp_gain_sel = '0001'b        // bits 7:4
    { 0xF, 4, CC_ICOMP_IIO_DFX_GLOBAL_REG, 0xFFFFFFFF, 0x00000010 },
    /*
     * END 4166237: HSX Uniphy Recipe v2.0
     */
     
     
     /*
     * START 4166638: HSX Uniphy Recipe v4.0
     */ 
    // Register: RX_VCM_DS_SEL.bndl0 ([1:0]) = '11'B
    { 0, 4, RX_VCM_DS_SEL_IIO_DFX_REG, 0xFFFFFFF0, 0x0000000F },
    { 1, 4, RX_VCM_DS_SEL_IIO_DFX_REG, 0xFFFFFF00, 0x000000FF },
    { 3, 4, RX_VCM_DS_SEL_IIO_DFX_REG, 0xFFFF0000, 0x0000FFFF },
    { 7, 4, RX_VCM_DS_SEL_IIO_DFX_REG, 0xFFFF0000, 0x0000FFFF },
	
    // Register: BNDL_SPARE.bndl0 = 20h (32d)
    { 0, 4, R_BNDL_SPARE_0,0x00000000, 0x00200020 },
    { 1, 4, R_BNDL_SPARE_0,0x00000000, 0x00200020 },
    { 1, 4, R_BNDL_SPARE_1,0x00000000, 0x00200020 },
    { 3, 4, R_BNDL_SPARE_0,0x00000000, 0x00200020 },
    { 3, 4, R_BNDL_SPARE_1,0x00000000, 0x00200020 },
    { 3, 4, R_BNDL_SPARE_2,0x00000000, 0x00200020 },
    { 3, 4, R_BNDL_SPARE_3,0x00000000, 0x00200020 },
    { 7, 4, R_BNDL_SPARE_0,0x00000000, 0x00200020 },
    { 7, 4, R_BNDL_SPARE_1,0x00000000, 0x00200020 },
    { 7, 4, R_BNDL_SPARE_2,0x00000000, 0x00200020 },
    { 7, 4, R_BNDL_SPARE_3,0x00000000, 0x00200020 },
    /*
     * END 4166237: HSX Uniphy Recipe v4.0
     */
         
    /*
     * START 4167964: HSX Uniphy Recipe v5.0
     */
    // DFXCHICKENBIT0_IIO_DFX_REG.cfg_lanewait_timer_settin =1;
    { 0, 4, DFXCHICKENBIT0_IIO_DFX_REG, 0xE7FFFFFF, 0x08000000 },
    { 1, 4, DFXCHICKENBIT0_IIO_DFX_REG, 0xE7FFFFFF, 0x08000000 },
    { 3, 4, DFXCHICKENBIT0_IIO_DFX_REG, 0xE7FFFFFF, 0x08000000 },
    { 7, 4, DFXCHICKENBIT0_IIO_DFX_REG, 0xE7FFFFFF, 0x08000000 },
   
    /*
     * END 4167964: HSX Uniphy Recipe v5.0
     */

    /*
     * START 4168811: PCIe Gen 2 Equalization Changes (CTLE_PEAK_GEN2 = 0x0)
     */ 
    // Register: RX_CTLE_PEAK_GEN2.bndl0 = "Peaking"
    { 0, 4, RX_CTLE_PEAK_GEN2_0_IIO_DFX_REG, 0xFFFFFC00, 0x00000000 },
    { 1, 4, RX_CTLE_PEAK_GEN2_0_IIO_DFX_REG, 0xFFFC0000, 0x00000000 },
    { 3, 4, RX_CTLE_PEAK_GEN2_0_IIO_DFX_REG, 0xC0000000, 0x00000000 },
    { 3, 4, RX_CTLE_PEAK_GEN2_1_IIO_DFX_REG, 0xFFFFFC00, 0x00000000 },
    { 7, 4, RX_CTLE_PEAK_GEN2_0_IIO_DFX_REG, 0xC0000000, 0x00000000 },
    { 7, 4, RX_CTLE_PEAK_GEN2_1_IIO_DFX_REG, 0xFFFFFC00, 0x00000000 },
    /*
     * END 4168811: PCIe Gen 2 Equalization Changes
     */ 

    /*
     * START 4985620: Change Gen 3 PCIe (8.0 GT/s) ctle_peak to 0x18 (24)
     */
    // Register: RX_CTLE_PEAK_GEN3.bndl0 = "Peaking"
        { 0, 4, RX_CTLE_PEAK_GEN3_0_IIO_DFX_REG, 0xFFFFFC00, 0x00000318 },
        { 1, 4, RX_CTLE_PEAK_GEN3_0_IIO_DFX_REG, 0xFFF00000, 0x000C6318 },
        { 3, 4, RX_CTLE_PEAK_GEN3_0_IIO_DFX_REG, 0xC0000000, 0x318C6318 },
        { 3, 4, RX_CTLE_PEAK_GEN3_1_IIO_DFX_REG, 0xFFFFFC00, 0x00000318 },
        { 7, 4, RX_CTLE_PEAK_GEN3_0_IIO_DFX_REG, 0xC0000000, 0x318C6318 },
        { 7, 4, RX_CTLE_PEAK_GEN3_1_IIO_DFX_REG, 0xFFFFFC00, 0x00000318 },
    /*
     * END 4985620: Change Gen 3 PCIe (8.0 GT/s) ctle_peak to 0x18 (24)
     */      

    /*
     * START 4985972: HSX Uniphy Recipe v7.0
     */
    // Register:TXEQREUTPRITXEQREUTPRIV1_IIO_DFX_REGV_IIO_DFX_REG.ph3_500nstimerincreasein_1us = '00 0000 0010'B (2d)             // Bits 9:0
        { 0, 4, TXEQREUTPRIV1_IIO_DFX_REG, 0xFFFFFC00, 0x00000002 },
        { 1, 4, TXEQREUTPRIV1_IIO_DFX_REG, 0xFFFFFC00, 0x00000002 },
        { 3, 4, TXEQREUTPRIV1_IIO_DFX_REG, 0xFFFFFC00, 0x00000002 },
        { 7, 4, TXEQREUTPRIV1_IIO_DFX_REG, 0xFFFFFC00, 0x00000002 },

    // Register:DEBUGCLUSTER1_IIO_DFX_REG.l1exiteito = '0010 1000'B = (40d)             // Bits 26:19
        { 0, 4, DEBUGCLUSTER1_IIO_DFX_REG, 0xF807FFFF, 0x01400000 },
        { 1, 4, DEBUGCLUSTER1_IIO_DFX_REG, 0xF807FFFF, 0x01400000 },
        { 3, 4, DEBUGCLUSTER1_IIO_DFX_REG, 0xF807FFFF, 0x01400000 },
        { 7, 4, DEBUGCLUSTER1_IIO_DFX_REG, 0xF807FFFF, 0x01400000 },
    /*
     * END 4985972: HSX Uniphy Recipe v7.0
     */
};

static CSR_ACCESS_OP_STRUCT HSX_Cx_RxRecipe[] = {
	//
    //HSX_C0_SPECIFIC_UNIPHY SETTTINGS_BEGIN 
	//
    /*
     * START 4167964 :HSX UniPhy Recipe V5.00 
     * Recipe is the same as B0 but without WSEL WA. 
     */
         
    // Register:CLSPHYCTL1_IIO_DFX_REG.speedchangetime_constin4us = 31      // Bits 12:7 
    { 0, 4, CLSPHYCTL1_IIO_DFX_REG, 0xFFFFE07F, 0x00000F80 },
    { 1, 4, CLSPHYCTL1_IIO_DFX_REG, 0xFFFFE07F, 0x00000F80 },
    { 3, 4, CLSPHYCTL1_IIO_DFX_REG, 0xFFFFE07F, 0x00000F80 },
    { 7, 4, CLSPHYCTL1_IIO_DFX_REG, 0xFFFFE07F, 0x00000F80 },
    
    // Register:RX_VREF_SEL_NOTGEN1_0_IIO_DFX_REG.laneX = 11      
    { 0, 4, RX_VREF_SEL_NOTGEN1_0_IIO_DFX_REG, 0xFFF00000, 0x0005AD6B },
    { 1, 4, RX_VREF_SEL_NOTGEN1_0_IIO_DFX_REG, 0xC0000000, 0X16B5AD6B },
    { 3, 4, RX_VREF_SEL_NOTGEN1_0_IIO_DFX_REG, 0xC0000000, 0X16B5AD6B },
    { 7, 4, RX_VREF_SEL_NOTGEN1_0_IIO_DFX_REG, 0xC0000000, 0X16B5AD6B },
    
    // Register:RX_VREF_SEL_NOTGEN1_1_IIO_DFX_REG.laneX = 11 
    { 1, 4, RX_VREF_SEL_NOTGEN1_1_IIO_DFX_REG, 0xFFFFFC00, 0x0000016B },
    { 3, 4, RX_VREF_SEL_NOTGEN1_1_IIO_DFX_REG, 0xC0000000, 0X16B5AD6B },
    { 7, 4, RX_VREF_SEL_NOTGEN1_1_IIO_DFX_REG, 0xC0000000, 0X16B5AD6B },
    
    // Register:RX_VREF_SEL_NOTGEN1_2_IIO_DFX_REG.laneX = 11     
    { 3, 4, RX_VREF_SEL_NOTGEN1_2_IIO_DFX_REG, 0xFFF00000, 0x0005AD6B },
    { 7, 4, RX_VREF_SEL_NOTGEN1_2_IIO_DFX_REG, 0xFFF00000, 0x0005AD6B },

    // 4986196: PCIE: Enable DLW Fix
    // Register:DEBUGCLUSTER1_IIO_DFX_REG.spare_signals = '00101'B             // Bits 31:27
    { 1, 4, DEBUGCLUSTER1_IIO_DFX_REG, 0x07FFFFFF, 0x28000000 },    
    { 3, 4, DEBUGCLUSTER1_IIO_DFX_REG, 0x07FFFFFF, 0x28000000 },
    { 7, 4, DEBUGCLUSTER1_IIO_DFX_REG, 0x07FFFFFF, 0x28000000 }        
    
    /*
     * END 4167964: HSX Uniphy Recipe v5.0
     */

    //
    // HSX_C0_SPECIFIC_UNIPHY SETTTINGS_END
    //
};

static CSR_ACCESS_OP_STRUCT HSX_Bx_RxRecipe[] = {
	//
    //HSX_B0_SPECIFIC_UNIPHY SETTTINGS_BEGIN 
	//
    /*
     * START 4167964 :HSX UniPhy Recipe V5.00 
     */
         
    // WSEL_IIO_DFX_REG.bndX = 6 
    { 0, 4, WSEL_IIO_DFX_REG, 0xFFFFFFC0, 0x00000036 },
    { 1, 4, WSEL_IIO_DFX_REG, 0xFFFFF000, 0x00000DB6 },
    { 3, 4, WSEL_IIO_DFX_REG, 0xFFFFE07F, 0x00DB6DB6 },
    { 7, 4, WSEL_IIO_DFX_REG, 0xFFFFE07F, 0x00DB6DB6 },
    
    // Register:CLSPHYCTL1_IIO_DFX_REG.speedchangetime_constin4us = 31      // Bits 12:7 
    { 0, 4, CLSPHYCTL1_IIO_DFX_REG, 0xFFFFE07F, 0x00000F80 },
    { 1, 4, CLSPHYCTL1_IIO_DFX_REG, 0xFFFFE07F, 0x00000F80 },
    { 3, 4, CLSPHYCTL1_IIO_DFX_REG, 0xFFFFE07F, 0x00000F80 },
    { 7, 4, CLSPHYCTL1_IIO_DFX_REG, 0xFFFFE07F, 0x00000F80 },
    
    // Register:RX_VREF_SEL_NOTGEN1_0_IIO_DFX_REG.laneX = 11      
    { 0, 4, RX_VREF_SEL_NOTGEN1_0_IIO_DFX_REG, 0xFFF00000, 0x0005AD6B },
    { 1, 4, RX_VREF_SEL_NOTGEN1_0_IIO_DFX_REG, 0xC0000000, 0X16B5AD6B },
    { 3, 4, RX_VREF_SEL_NOTGEN1_0_IIO_DFX_REG, 0xC0000000, 0X16B5AD6B },
    { 7, 4, RX_VREF_SEL_NOTGEN1_0_IIO_DFX_REG, 0xC0000000, 0X16B5AD6B },
    
    // Register:RX_VREF_SEL_NOTGEN1_1_IIO_DFX_REG.laneX = 11 
    { 1, 4, RX_VREF_SEL_NOTGEN1_1_IIO_DFX_REG, 0xFFFFFC00, 0x0000016B },
    { 3, 4, RX_VREF_SEL_NOTGEN1_1_IIO_DFX_REG, 0xC0000000, 0X16B5AD6B },
    { 7, 4, RX_VREF_SEL_NOTGEN1_1_IIO_DFX_REG, 0xC0000000, 0X16B5AD6B },
    
    // Register:RX_VREF_SEL_NOTGEN1_2_IIO_DFX_REG.laneX = 11     
    { 3, 4, RX_VREF_SEL_NOTGEN1_2_IIO_DFX_REG, 0xFFF00000, 0x0005AD6B },
    { 7, 4, RX_VREF_SEL_NOTGEN1_2_IIO_DFX_REG, 0xFFF00000, 0x0005AD6B }
    
    /*
     * END 4167964: HSX Uniphy Recipe v5.0
     */

    //
    // HSX_B0_SPECIFIC_UNIPHY SETTTINGS_END
    //
};

static CSR_ACCESS_OP_STRUCT HSX_Ax_RxRecipe[] = {
    //
    //HSX_A0_SPECIFIC SETTINGS_BEGIN
	//
    // Register:CLSPHYCTL1_IIO_DFX_REG.speedchangetime_constin1us = '011111'B    // Bits 12:7 
    { 0, 4, CLSPHYCTL1_IIO_DFX_REG, 0xFFFFFFFF, 0x00000000 },
    { 1, 4, CLSPHYCTL1_IIO_DFX_REG, 0xFFFFE07F, 0x00001F80 },
    { 3, 4, CLSPHYCTL1_IIO_DFX_REG, 0xFFFFE07F, 0x00001F80 },
    { 7, 4, CLSPHYCTL1_IIO_DFX_REG, 0xFFFFE07F, 0x00001F80 }
    //
    // HSX_A0_SPECIFIC SETTINGS_END
    //
};

static CSR_ACCESS_OP_STRUCT HSX_RxRecipe_6[] = {
    /*
     * START 4168553: PCIe HSX UniPhy Recipe Updated to v6.00 (PCIe changes only)
     */
    // Register:RX_CTLE_RDEG_GEN1_IIO_DFX_REG.bndl = '0101'B           // bndl - 4 bits wide
    { 0, 4, RX_CTLE_RDEG_GEN1_IIO_DFX_REG, 0xFFFFFF00, 0x00000055 },
    { 1, 4, RX_CTLE_RDEG_GEN1_IIO_DFX_REG, 0xFFFF0000, 0x00005555 },
    { 3, 4, RX_CTLE_RDEG_GEN1_IIO_DFX_REG, 0x00000000, 0x55555555 },
    { 7, 4, RX_CTLE_RDEG_GEN1_IIO_DFX_REG, 0x00000000, 0x55555555 },

    // Register:RX_CTLE_RDEG_GEN2_IIO_DFX_REG.bndl = '0101'B           // bndl - 4 bits wide
    { 0, 4, RX_CTLE_RDEG_GEN2_IIO_DFX_REG, 0xFFFFFF00, 0x00000055 },
    { 1, 4, RX_CTLE_RDEG_GEN2_IIO_DFX_REG, 0xFFFF0000, 0x00005555 },
    { 3, 4, RX_CTLE_RDEG_GEN2_IIO_DFX_REG, 0x00000000, 0x55555555 },
    { 7, 4, RX_CTLE_RDEG_GEN2_IIO_DFX_REG, 0x00000000, 0x55555555 },

    // Register:RX_CTLE_RDEG_GEN3_IIO_DFX_REG.bndl = '0101'B           // bndl - 4 bits wide
    { 1, 4, RX_CTLE_RDEG_GEN3_IIO_DFX_REG, 0xFFFF0000, 0x00005555 },
    { 3, 4, RX_CTLE_RDEG_GEN3_IIO_DFX_REG, 0x00000000, 0x55555555 },
    { 7, 4, RX_CTLE_RDEG_GEN3_IIO_DFX_REG, 0x00000000, 0x55555555 }
    /*
     * END 4168553: HSX Uniphy Recipe v6.0 
     */
};

// For Si RX Recipe settings
VOID IioRxRecipeSettings (
    IIO_GLOBALS             *IioGlobalData,
    UINT8                   Iio,
    UINT8                   BusNumber
    )
{
  CSR_ACCESS_OP_STRUCT	    *RxRecipe;
  UINT32                    RxRecipeSize;
  UINT32                    DcuMode;

  if ((IioGlobalData->IioVData.CpuType == CPU_HSX)) {
    //
    // Write common CPU specific settings
    //
    RxRecipe = HSX_ALL_RxRecipe;
    RxRecipeSize = sizeof(HSX_ALL_RxRecipe);
    WriteCsrTable(IioGlobalData, Iio, BusNumber, RxRecipe, RxRecipeSize);
  
    //
    // Now overwrite with CPU stepping specific settings
    //
    switch (IioGlobalData->IioVData.CpuStepping)
    {
      case A0_REV_HSX:
        RxRecipe     = HSX_Ax_RxRecipe; 
        RxRecipeSize = sizeof(HSX_Ax_RxRecipe);
        break;
      case B0_REV_HSX:
        RxRecipe     = HSX_Bx_RxRecipe; 
        RxRecipeSize = sizeof(HSX_Bx_RxRecipe); 
        break;
      case C0_REV_HSX:     
        RxRecipe     = HSX_Cx_RxRecipe; 
        RxRecipeSize = sizeof(HSX_Cx_RxRecipe); 
        break;
      default:
        IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_ERROR, "HSX Stepping not found = %x\n",IioGlobalData->IioVData.CpuStepping);
        break;
    }

    WriteCsrTable(IioGlobalData, Iio, BusNumber, RxRecipe, RxRecipeSize);

    //
    // Now apply the RX_CTLE_RDGEN recipe 6.0 base on CPU stepping and DCU_Mode
    //
    DcuMode = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketUncoreBusNumber[0], PCU22_DEV_NUM, PCU22_FUNC_NUM, ONLY_REGISTER_OFFSET(CAPID0_PCU_FUN3_REG)));
    DcuMode = (DcuMode & 0x40000000) >> 30;
       
    if((IioGlobalData->IioVData.CpuStepping >= B0_REV_HSX) && (DcuMode == 0)) {
      RxRecipe = HSX_RxRecipe_6;
      RxRecipeSize = sizeof(HSX_RxRecipe_6);
      WriteCsrTable(IioGlobalData, Iio, BusNumber, RxRecipe, RxRecipeSize);
    }

    IioStall (IioGlobalData, UNIPHY_RECIPE_DELAY);   //Intended fix for s4030382: give a delay of 180us after RX recipe
#ifndef IA32
    S3BootScriptSaveStall(UNIPHY_RECIPE_DELAY);
#endif
  }
}

VOID
WriteCsrTable(
    IIO_GLOBALS                     *IioGlobalData,
    UINT8                           Iio,
    UINT8                           BusNumber,
    CSR_ACCESS_OP_STRUCT            *RxRecipe,
    UINT32                          RxRecipeSize
   )
{

  UINT32                        RegVal;
  UINT32                        Register;
  UINT32                        RegOffset;
  UINT32                        i;
  UINT8                         size;
  UINT8                         Dev;
  UINT8                         Fun;

  for (i = 0; i < (RxRecipeSize/sizeof(CSR_ACCESS_OP_STRUCT)); i++) {
    size = 4;

    if (RxRecipe[i].Instance != 0xF) {
        Dev = IioGlobalData->PreLinkData.PcieInfo.PortInfo[RxRecipe[i].Instance].DfxDevice;
        Fun = IioGlobalData->PreLinkData.PcieInfo.PortInfo[RxRecipe[i].Instance].DfxFunction;
    } else {
        Dev = PCIE_PORT_GLOBAL_DFX_DEV;
        Fun = PCIE_PORT_GLOBAL_DFX_FUNC;
    }

    // Skip IOSF port
    if(OemIdentifyVendorSpecificPort(IioGlobalData, BusNumber, Dev, Fun, TRUE) == TRUE)
      continue;

    RegOffset = ONLY_REGISTER_OFFSET(RxRecipe[i].RegOffset);

    if(BusNumber == 0 && Dev == 6 && Fun == 0) { //SKIP DMI PORT
      if (!(IS_RXRECIPE_FOR_HSX_DMI(RxRecipe[i].RegOffset, RxRecipe[i].AndMask)))
        continue;
    }

    //RegVal &= 0xFFFFF000;     //physical offset of V/DID
    //if( *( (volatile UINT32 *)((UINTN)RegVal) ) == 0xFFFFFFFF)   //port fused-off, hence skip
    //  continue;
    RegVal = 0;
    Register = PCI_PCIE_ADDR(BusNumber, Dev, Fun, RegOffset);

    switch(RxRecipe[i].RegSize) {
      case sizeof(UINT32):
        RegVal = IioPciExpressRead32(IioGlobalData, Register);
        break;
      case sizeof(UINT16):
        RegVal = IioPciExpressRead16(IioGlobalData, Register);
        break;
      case sizeof(UINT8):
        RegVal = IioPciExpressRead8(IioGlobalData, Register);
        break;
    }

    RegVal &= RxRecipe[i].AndMask;
    RegVal |= RxRecipe[i].OrMask;

    switch(RxRecipe[i].RegSize) {
      case sizeof(UINT32):
        IioPciExpressWrite32(IioGlobalData, Register, RegVal);
        break;
      case sizeof(UINT16):
        IioPciExpressWrite16(IioGlobalData, Register, (UINT16)RegVal);
        break;
      case sizeof(UINT8):
        IioPciExpressWrite8(IioGlobalData, Register, (UINT8)RegVal);
        break;
    }
    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "  B%d D%d F%d O%xh = 0x%x\n", BusNumber, Dev, Fun, RegOffset, RegVal);
  }
}

VOID
EarlyCtlePhaseSettings (
  IIO_GLOBALS        *IioGlobalData,
  UINT8              IioIndex
  )
{
  PCI_ROOT_BRIDGE_PCI_ADDRESS          PciAddress;
  LBC_PER_IOU_DATA_IIO_DFX_STRUCT      LbcPerIouData;
  LBC_PER_IOU_CONTROL_IIO_DFX_STRUCT   LbcPerIouControl;
  UINT32    RegAddr;
  UINT8     PortIndex, Value;
  UINT8     ClusterDfxDev , ClusterDfxFunc;
  UINT8     LaneCount = 0;
  UINT16    lane;
  BOOLEAN   Status=FALSE;

  if ( ((IioGlobalData->IioVData.CpuFamily == CPU_FAMILY_BDX) && (IioGlobalData->IioVData.CpuStepping <= B0_REV_BDX)) ||
       ((IioGlobalData->IioVData.CpuFamily == CPU_FAMILY_BDX_DE) && (IioGlobalData->IioVData.CpuStepping >= C0_REV_BDX)) ) {

    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "EarlyCtlePhaseSettings for IIO[%x]\n", IioIndex);

    for (PortIndex = 0; PortIndex < NUMBER_PORTS_PER_SOCKET; PortIndex++) {
      //
      // Get lane count based on IOU width
      //
      switch(PortIndex) {
        case PORT_0_INDEX:
          LaneCount = 4;
          break;

        case PORT_1A_INDEX:
          LaneCount = 8;
          break;

        case PORT_2A_INDEX:
        case PORT_3A_INDEX:
          LaneCount = 16;
          break;

        default:
          continue;
      }

      PciAddress.Bus      = IioGlobalData->IioVData.SocketBaseBusNumber[IioIndex];
      PciAddress.Device   = IioGlobalData->PreLinkData.PcieInfo.PortInfo[PortIndex].Device;
      PciAddress.Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[PortIndex].Function;

      if( IioVData_ConfigurePciePort(IioGlobalData, IioIndex, PortIndex) == FALSE)  //check whether the PCIe port can be configured
        continue;

      // Skip IOSF port
      if(OemIdentifyVendorSpecificPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function, FALSE) == TRUE)
        continue;

      ClusterDfxDev  = IioGlobalData->PreLinkData.PcieInfo.PortInfo[PortIndex].DfxClusterDevice;
      ClusterDfxFunc = IioGlobalData->PreLinkData.PcieInfo.PortInfo[PortIndex].DfxClusterFunction;
      //
      // verify TBC Beta Board and NTB ports 
      // 
      Status = OemOverrideMiscCtlep(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function);

      for (lane = 0; lane < LaneCount; lane++) {
        if (Status == TRUE && lane < 8 ) {
          Value = 12; // rx_st1_ctle_peak_g3_qpifull
        } else {
          Value = 15; // rx_st1_ctle_peak_g3_qpifull
        }
        //
        // Setup Load Bus Data register
        //
        RegAddr = PCI_PCIE_ADDR(PciAddress.Bus, ClusterDfxDev, ClusterDfxFunc, LBC_PER_IOU_DATA_IIO_DFX_REG);
        LbcPerIouData.Data  = IioPciExpressRead32 (IioGlobalData, RegAddr);
        LbcPerIouData.Bits.lbc_data |= 1;           // Per Bndl Control Enable for Source-Degeneration Cap (st1_ctle_peak) of Source-Degen Preamp stage
        LbcPerIouData.Bits.lbc_data |= 1 << 1;      // rx_st1_ctle_peak_g1_qpihalf --> Gen-1, QPI Half rate Per Bndl st1_ctle_peak value
        LbcPerIouData.Bits.lbc_data |= 1 << 5;      // rx_st1_ctle_peak_g2 --> Gen-2 Per Bndl st1_ctle_peak value
        LbcPerIouData.Bits.lbc_data |= Value << 9;     // rx_st1_ctle_peak_g3_qpifull --> Gen-3, QPI Full rate Per Bndl st1_ctle_peak value
        IioPciExpressWrite32 (IioGlobalData, RegAddr, LbcPerIouData.Data);
        IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "  LbcPerIouData Addr %x = 0x%x\n", RegAddr, LbcPerIouData.Data);

        //
        // Setup Load Bus Control register
        //
        RegAddr = PCI_PCIE_ADDR(PciAddress.Bus, ClusterDfxDev, ClusterDfxFunc, LBC_PER_IOU_CONTROL_IIO_DFX_REG);
        LbcPerIouControl.Data = IioPciExpressRead32 (IioGlobalData, RegAddr);
        LbcPerIouControl.Bits.lbc_ld_sel = LOAD_CTLE_SEL_2;
        LbcPerIouControl.Data &= ~LBC_LN_SEL_MASK;       // Cleans Lbc_ln_sel bits
        LbcPerIouControl.Data |= 1 << (lane + 11);       // Lbc_ln_sel[7 ... 0] Bits[26:11]
        LbcPerIouControl.Bits.lbc_req    = LBC_WRITE_COMMAND;
        LbcPerIouControl.Bits.lbc_start  = 0x1;
        IioPciExpressWrite32 (IioGlobalData, RegAddr, LbcPerIouControl.Data);
        IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "  LbcPerIouControl Addr %x = 0x%x\n", RegAddr, LbcPerIouControl.Data);
      } // Lane loop
    } // PortIdex loop
  } // Stepping loop
}

// IIO Rx Recalibration programming, before UNIPHY Rx recipe settings (if any)
VOID IioPreUniphyInitRxRecalibration (
    IIO_GLOBALS                        *IioGlobalData,
    UINT8                              Iio,
    UINT8                              BusNumber
    )
{
  PORT_CTL_IIO_DFX_HSX_BDX_STRUCT     PortCtl;

  UINT8                               j;
  UINT8                               IioDfxDev;
  UINT8                               IioDfxFunc;
  
  for(j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {
    IioDfxDev = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxDevice;
    IioDfxFunc = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxFunction;

    // Skip IOSF port
    if(OemIdentifyVendorSpecificPort(IioGlobalData, BusNumber, IioDfxDev, IioDfxFunc, TRUE) == TRUE)
       continue;

    if( (IioGlobalData->IioVData.CpuType == CPU_IVT) && (IioGlobalData->IioVData.CpuStepping == A0_REV) ){
      if(!Iio && !j)  continue;   //skip DMI port
        //
        // ivt_eco: s4541463: CLONE from ivytown: CLONE: CLONE from jaketown:
        //                    Rx Fails Gen 3 PCIe Stressed Eye
        //  [cloned to s4030514] - expected to be fixed in B0 stepping
      if (j == 0 || j == 3 || j == 7){      //since port 0 will not function as Gen3, and port 1a, 1b are fused off for IVT-EX
        PortCtl.Data = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioDfxDev, IioDfxFunc, ONLY_REGISTER_OFFSET(PORT_CTL_IIO_DFX_REG)));
        if(PortCtl.Bits.rx_dfelsb_sel == 0){
          // BIOS programming sequence to redo calibration...

          //Assert & De-assert oc_start for PCIe ports 2A & 3A

          IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusNumber, IioDfxDev, IioDfxFunc, ONLY_REGISTER_OFFSET(OC_START_IIO_DFX_REG)), 0x8000FFFF);
          //delay of 180microseconds as per the BSU
          IioStall (IioGlobalData, UNIPHY_RECIPE_DELAY);
#ifndef IA32
          S3BootScriptSaveStall(UNIPHY_RECIPE_DELAY);
          //to revert the OC_Start back to hardware value of 0
#endif // IA32

          IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusNumber, IioDfxDev, IioDfxFunc, ONLY_REGISTER_OFFSET(OC_START_IIO_DFX_REG)), 0);
	  
          IioStall (IioGlobalData, UNIPHY_RECIPE_DELAY);       //some addl. 180us delay after re-calibration
#ifndef IA32
          S3BootScriptSaveStall(UNIPHY_RECIPE_DELAY);
#endif // IA32
          IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Re-calibration performed on IIO:%d, port=%s(%d)\n", Iio, IIO_PORT_LABEL(j), j);
        }
      }
    }
  }

  //
  // 4988400: BDX A0 PO PCIE and QPI loadbus register change request
  //
  EarlyCtlePhaseSettings(IioGlobalData, Iio);

}

// IIO Rx Recalibration programming, after UNIPHY Rx recipe settings (if any)
VOID
IioPostUniphyInitRxRecalibration (
  IIO_GLOBALS                         *IioGlobalData,
  UINT8                               Iio,
  UINT8                               BusNumber
  )
{
  UINT8                               j, retries_cnt = 0, redo_rx_cal = 0;
  UINT32                              oc_done, expected_oc_done;
  UINT8                               IioDfxDev;
  UINT8                               IioDfxFunc;
  CC_BIAS_RX_IIO_DFX_GLOBAL_STRUCT    CcBiasRx;
  CAPID4_PCU_FUN3_BDX_STRUCT          Capid4_Bdx;


  for(j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {
    IioDfxDev = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxDevice;
    IioDfxFunc = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxFunction;

    // Skip IOSF port
    if(OemIdentifyVendorSpecificPort(IioGlobalData, BusNumber, IioDfxDev, IioDfxFunc, TRUE) == TRUE)
      continue;

    if (IioGlobalData->IioVData.CpuType == CPU_IVT ) {
      if(!Iio && !j)  continue;   //skip DMI port
        //
        // ivt_eco: s4541463: CLONE from ivytown: CLONE: CLONE from jaketown:
        //                    Rx Fails Gen 3 PCIe Stressed Eye
        //  [cloned to s4030514] - for all steppings
      switch(j)
      {
      case 0:   //PCIe port 0
        expected_oc_done = 0x0000000F;
        break;
      case 1:   //IOU2
        expected_oc_done = 0x000000FF;
        break;
      case 3:   //IOU0
      case 7:   //IOU1
        expected_oc_done = 0x0000FFFF;
        break;
      default:
        expected_oc_done = 0;
        break;
      }
      if (j == 0 || j == 3 || j == 7){      //since port 1a, 1b are fused off for IVT-EX
        //
        // Please note that the "rx_dfelsb_sel" are done in uniphy recipe settings
        // hence no need to perform a chack to override here since future recipe versions
        // may choose to disable it hence don't want to overwrite here
        //

        //
        //perform the rx recalibration (upto 10 retries max)
        //
        redo_rx_cal = 0;
        while(redo_rx_cal < 10){
          //Over-ride L1 to enable clocks needed for rx_cal

          IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusNumber, IioDfxDev, IioDfxFunc,
              ONLY_REGISTER_OFFSET(L1_MODE_IIO_DFX_REG)), 0x80000000);

          //Assert & De-assert oc_start for PCIe ports 2A & 3A

          IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusNumber, IioDfxDev, IioDfxFunc,
              ONLY_REGISTER_OFFSET(OC_START_IIO_DFX_REG)), 0x8000FFFF);
          //delay of 180microseconds as per the BSU
          IioStall (IioGlobalData, UNIPHY_RECIPE_DELAY);
#ifndef IA32
          S3BootScriptSaveStall(UNIPHY_RECIPE_DELAY);
          //to revert the OC_Start back to hardware value of 0
#endif // IA32

          IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusNumber, IioDfxDev, IioDfxFunc,
              ONLY_REGISTER_OFFSET(OC_START_IIO_DFX_REG)), 0);
          //as per ECO; Allow time (min 80us) for rx_cals to complete
          //before following de-assert of L1 over-ride
          IioStall (IioGlobalData, UNIPHY_RECIPE_DELAY);       //180us
#ifndef IA32
          S3BootScriptSaveStall(UNIPHY_RECIPE_DELAY);
#endif // IA32

          //Clear l1 over-ride

          IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR( BusNumber, IioDfxDev, IioDfxFunc,
              ONLY_REGISTER_OFFSET(L1_MODE_IIO_DFX_REG)), 0x00000000);

          do{
            oc_done = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioDfxDev, IioDfxFunc,
                ONLY_REGISTER_OFFSET(OC_DONE_IIO_DFX_REG)));
            retries_cnt++;
            IioStall (IioGlobalData, OC_DONE_TIMEOUT);    //10us
          }while(oc_done != expected_oc_done && retries_cnt < 100);   //time out after 1ms of polling

#ifndef IA32
          S3BootScriptSaveMemPoll(S3BootScriptWidthUint32, 
                PCI_PCIE_ADDR(BusNumber, IioDfxDev, IioDfxFunc, ONLY_REGISTER_OFFSET(OC_DONE_IIO_DFX_REG)),
                &expected_oc_done,
                &expected_oc_done, 
                OC_DONE_TIMEOUT,
                100);
#endif // IA32

          if(oc_done == expected_oc_done){
            IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Post Uniphy Recipe (v7.0) Re-calibration performed on IIO:%d, port=%s(%d) (retries=%d)\n",
                  Iio, IIO_PORT_LABEL(j), j, redo_rx_cal);
            break;
          }
          else
            redo_rx_cal++;
        }
        if(redo_rx_cal >= 10)
          IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_ERROR, "Post Uniphy Recipe (v7.0) Re-calibration failed on IIO:%d, port=%s(%d)\n", Iio, IIO_PORT_LABEL(j), j);
      }
    }
  }   //loop for all the cluster ports

  //
  //  5001845 : Need PCIE lane detection WA to be implemented in BIOS
  //  fixes LSI/Emulex/Barton Hill detection , applicable for BDX-EP A0 HCC, fixed in MCC L0 , HCC B0 (s5002122) 
  //
  Capid4_Bdx.Data = IioReadCpuCsr32(IioGlobalData, Iio, 0, CAPID4_PCU_FUN3_REG);

  if (IioGlobalData->IioVData.CpuFamily == CPU_FAMILY_BDX && IioGlobalData->IioVData.CpuStepping == A0_REV_BDX && Capid4_Bdx.Bits.physical_chop == HCC) {
    CcBiasRx.Data = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(CC_BIAS_RX_IIO_DFX_GLOBAL_REG)));
    CcBiasRx.Bits.rx_ibias_scale = 3;    //set rx_ibias_scale = 3
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(CC_BIAS_RX_IIO_DFX_GLOBAL_REG)), CcBiasRx.Data);
  } 
}

VOID
IioMaxPerfHitWorkAround( 
    IIO_GLOBALS                 *IioGlobalData,
    UINT8                       Iio
    )
{
  UINT8 BusBase;
  TSWCTL1_IIO_DFX_GLOBAL_STRUCT           TswCtl1;
  TSWCTL0_IIO_DFX_GLOBAL_HSX_BDX_STRUCT   TswCtl0; 

  if ((IioGlobalData->IioVData.CpuType == CPU_BDX)) {
    BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];

    TswCtl1.Data = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(TSWCTL1_IIO_DFX_GLOBAL_REG)));
    if (IioGlobalData->BdxSetupData.Iou1Np_Pf[Iio]) {
      TswCtl1.Bits.np_pf_disable_global_iou1 = 1;
    }

    if (IioGlobalData->BdxSetupData.Iou2Np_Pf[Iio]) {
      TswCtl1.Bits.np_pf_disable_global_iou2 = 1;
    }
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(TSWCTL1_IIO_DFX_GLOBAL_REG)), TswCtl1.Data);

    if (IioGlobalData->BdxSetupData.Iou0Np_Pf[Iio]) {
      TswCtl0.Data = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(TSWCTL0_IIO_DFX_GLOBAL_REG)));
      TswCtl0.Bits.np_pf_disable_global_iou0 = 1;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(TSWCTL0_IIO_DFX_GLOBAL_REG)), TswCtl0.Data);
    }
  }
}

VOID
DfxPcieInit (
    IIO_GLOBALS                         *IioGlobalData,
    UINT8                               Iio
  )
{
  UINT8                                   j;
  ARB_CONTROL0_IIO_DFX_GLOBAL_STRUCT      ArbCtl0;
  UINT32                                  CsrValue;
  UINT32	                              Ler_uncerrmsk;
  IIOMISCCTRL_N1_IIO_VTD_HSX_BDX_STRUCT   IioMiscCtrlN1;
  CC_SPARE_ADDL_IIO_DFX_GLOBAL_STRUCT     CcSpareAddl;  
  LER_XPUNCERRMSK_IIO_PCIE_STRUCT         Ler_Xpuncerrmsk;
  LER_RPERRMSK_IIO_PCIE_STRUCT            Ler_Rperrmsk;
  LER_CTRLSTS_IIO_PCIE_STRUCT             Ler_Ctrlsts;
  TXEQREUTPRIV_IIO_DFX_STRUCT             TxEqReutPriv;
  UINT8   IioDfxDev;
  UINT8   IioDfxFunc;
  UINT8   Device;
  UINT8   Function;
  UINT8   BusNumber = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];
  //
  // IIO Rx Recalibration programming, before UNIPHY Rx recipe settings (if any)
  //
  IioPreUniphyInitRxRecalibration(IioGlobalData, Iio, BusNumber);
  //
  // Apply IIO RX Recipe settings here
  //
  if (IioGlobalData->IioVData.CpuType == CPU_HSX) {
    // For now , HSX recipe through static tables 
    IioRxRecipeSettings(IioGlobalData, Iio, BusNumber);
  } else { // BDX process automated script generated table
    IioRxRecipeSettingsAuto(IioGlobalData, Iio);
  }
  //
  //IIO Rx Recalibration programming, after UNIPHY Rx recipe settings (if any)
  //
  IioPostUniphyInitRxRecalibration(IioGlobalData, Iio, BusNumber);
  //
  // s4987827 Program GEN3_CTLEp
  //
  Gen3ProgCTLEp(IioGlobalData, Iio, BusNumber);
  //
  // BIOS Enhancement for the UniPhy RxRst issue...
  //  s4030437: To avoid Link degrade with RxRst on non legacy socket
  //  s4987980: CC_SPARE_ADDL[1:0] must be be set for all PCIe/DMI ports on all HSX steppings
  //
  if (((IioGlobalData->IioVData.CpuType == CPU_IVT) && (Iio != 0)) || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
    CcSpareAddl.Data = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(CC_SPARE_ADDL_IIO_DFX_GLOBAL_REG)));
    CcSpareAddl.Data |= 3;    //set CC_SPARE_ADDL[0], CC_SPARE_ADDL[1]
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(CC_SPARE_ADDL_IIO_DFX_GLOBAL_REG)), CcSpareAddl.Data);
  }

  //
  // ivt_performance improvement 4030103: Recommendation of setting IIO CSR in BIOS for IVX
  //
  ArbCtl0.Data = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(ARB_CONTROL0_IIO_DFX_GLOBAL_REG)));
  ArbCtl0.Bits.pri_p_f_2slots_iou1 = 1;
  ArbCtl0.Bits.pri_p_f_2slots_iou0 = 1;
  IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(ARB_CONTROL0_IIO_DFX_GLOBAL_REG)), ArbCtl0.Data);
  
  //
  // 4166876: Configure Stop & scream if enabled
  //  
  if ( IioGlobalData->SetupData.StopAndScream ) {  
    IioMiscCtrlN1.Data = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, IIO_VTD_DEV, IIO_VTD_FUN, ONLY_REGISTER_OFFSET(IIOMISCCTRL_N1_IIO_VTD_REG)));
    IioMiscCtrlN1.Bits.poisfen = 0x01;
    IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, IIO_VTD_DEV, IIO_VTD_FUN, ONLY_REGISTER_OFFSET(IIOMISCCTRL_N1_IIO_VTD_REG)), IioMiscCtrlN1.Data);
  }

  for(j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {
    Device = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
    Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;
    IioDfxDev = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxDevice;
    IioDfxFunc = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxFunction;

    // Skip IOSF port
    if(OemIdentifyVendorSpecificPort(IioGlobalData, BusNumber, IioDfxDev, IioDfxFunc, TRUE) == TRUE)
      continue;

    if(j==0 || j==1 || j==3 || j==7){
      //
      // 4030359: PCIE: PHY Debug Signals need to be mapped to the debug ring
      //
      if (IioGlobalData->IioVData.CpuType == CPU_IVT) {
        CsrValue = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioDfxDev, IioDfxFunc,
                            ONLY_REGISTER_OFFSET(XPPRIVCNEW_IIO_DFX_REG)));
        CsrValue &= ~BIT0; // powrsvphy
        IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioDfxDev, IioDfxFunc,
            ONLY_REGISTER_OFFSET(XPPRIVCNEW_IIO_DFX_REG)), CsrValue);
      }
      if ((IioGlobalData->IioVData.CpuType == CPU_IVT) || (IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
        CsrValue = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioDfxDev, IioDfxFunc,
                         ONLY_REGISTER_OFFSET(XPPRIVC_IIO_DFX_REG)));
        CsrValue &= ~BIT8;  //powrsvphy
        IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioDfxDev, IioDfxFunc,
            ONLY_REGISTER_OFFSET(XPPRIVC_IIO_DFX_REG)), CsrValue);
      }

      //
      // ivt_performance improvement: 4031488: Enable local P2P WRITE's perf improvement in EX
      //
      if(IioGlobalData->IioVData.CpuType == CPU_IVT && IioGlobalData->IioVData.CpuStepping >= B0_REV){
        CsrValue = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioDfxDev, IioDfxFunc,
                    ONLY_REGISTER_OFFSET(XPDFXSPAREREG_IIO_DFX_REG)));
        CsrValue &= ~BIT1;  //xpdfxsparereg_dis_perfboost_pktgen = 0
        IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioDfxDev, IioDfxFunc,
            ONLY_REGISTER_OFFSET(XPDFXSPAREREG_IIO_DFX_REG)), CsrValue);
      }

      //
      // hsx_performance improvement: 4168476: IIO Perfboost Pkt Gen CSR Field Needs to Be Enabled By Default
      //
      if ((IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
        CsrValue = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioDfxDev, IioDfxFunc,
                     ONLY_REGISTER_OFFSET(XPDFXSPAREREG_IIO_DFX_REG)));
        CsrValue &= ~BIT1;  //xpdfxsparereg_dis_perfboost_pktgen = 0
        IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioDfxDev, IioDfxFunc,
          ONLY_REGISTER_OFFSET(XPDFXSPAREREG_IIO_DFX_REG)), CsrValue);
      }

      //
      // bdx_bugeco s280626 : CLONE from broadwell_server: PUSH from haswell_server: PUSH from ivytown: CLONE from ivytown: PCIe - Gen3 header bits inserted in TX data 
      // stream when RX not frame locked with in-band slave loop
      // [cloned to s4986752 ] - Permanent
      //
      if (IioGlobalData->IioVData.CpuType == CPU_HSX || IioGlobalData->IioVData.CpuType == CPU_BDX) {
        TxEqReutPriv.Data = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR( BusNumber, IioDfxDev, IioDfxFunc, ONLY_REGISTER_OFFSET(TXEQREUTPRIV_IIO_DFX_REG)));
        TxEqReutPriv.Bits.wait4_2tsinlpbkentryslave = 1;
        IioPciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR( BusNumber, IioDfxDev, IioDfxFunc, ONLY_REGISTER_OFFSET(TXEQREUTPRIV_IIO_DFX_REG)), TxEqReutPriv.Data);
      }
    }  

    //
    // 4166876: Configure Stop & scream if enabled
    //  
    if ( IioGlobalData->SetupData.StopAndScream ) {                    
      Ler_Xpuncerrmsk.Data = 0x0ff;	
      Ler_uncerrmsk = 0xffffffff;	  
      Ler_Rperrmsk.Data = 0x0ff;	
      Ler_Ctrlsts.Bits.ler_ss_enable = 0x01;   
	     
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, Device, Function,
                                        ONLY_REGISTER_OFFSET(LER_XPUNCERRMSK_IIO_PCIE_REG )), Ler_Xpuncerrmsk.Data);	
            
            
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, Device, Function,
                                        ONLY_REGISTER_OFFSET(LER_UNCERRMSK_IIO_PCIE_REG  )), Ler_uncerrmsk);	  
            
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, Device, Function,
                                        ONLY_REGISTER_OFFSET(LER_RPERRMSK_IIO_PCIE_REG  )), Ler_Rperrmsk.Data);		 
            
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, Device, Function,
                                        ONLY_REGISTER_OFFSET(LER_CTRLSTS_IIO_PCIE_REG)), Ler_Ctrlsts.Data);                 
    }
  } //end of per port DFX init settings

  //
  //bdx_eco  s282828 BDX-DE Perf Val:  PCIe x16 + PCIe x8 100% Rd BW Capped at 19.4GB/s
  //[cloned to s4988093]- permanent WA
  //
  IioMaxPerfHitWorkAround(IioGlobalData,Iio);
  
  //
  //buffer the DFX register settings which are to be used during Gen3 EQ init
  //
  UpdateVDataAfterApplyWorkAround(IioGlobalData, Iio);
}

VOID
MiscCtlepSetting(
  IIO_GLOBALS      *IioGlobalData,
  UINT8            Iio,
  UINT8            BusNumber
)
{
  BOOLEAN    Status= FALSE;
  UINT32     Data=0;
  UINT8      func=0;
  UINT8      j;
  UINT8      IioDfxDev,Device;
  UINT8      IioDfxFunc,Function;


  for (j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {

    Device     = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
    Function   = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;
    IioDfxDev  = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxDevice;
    IioDfxFunc = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxFunction;	

    Status = OemOverrideMiscCtlep(IioGlobalData, BusNumber, Device, Function);
  
    if (Status == TRUE ) {
      // Set RX_CTLE_RDEG_GEN3 = 13 
      Data = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioDfxDev, IioDfxFunc, RX_CTLE_RDEG_GEN3_IIO_DFX_REG));
      Data &= 0xFFFF0000;
      Data |= 0x0000DDDD;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioDfxDev, IioDfxFunc, RX_CTLE_RDEG_GEN3_IIO_DFX_REG), Data);    
      
      // RX_CTLE_PEAK_GEN3 = 3
      Data = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioDfxDev, IioDfxFunc, RX_CTLE_PEAK_GEN3_0_IIO_DFX_REG));
      Data &= 0xFFF00000;
      Data |= 0x00018C63;
      IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioDfxDev, IioDfxFunc, RX_CTLE_PEAK_GEN3_0_IIO_DFX_REG), Data);
      
      for (func=0; func < 2; func++ ) {    
        Data = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioDfxDev, func, TX_PH3_CURSOR_IIO_DFX_REG));
        Data &= 0xFEFFFFFF;
        Data |= 0x1000000;
        IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioDfxDev, func, TX_PH3_CURSOR_IIO_DFX_REG), Data);    
      } // func
    } // Status
  } // for j 
}

VOID
Gen3ProgCTLEp(
  IIO_GLOBALS      *IioGlobalData,
  UINT8            Iio,
  UINT8            BusNumber
  )
{
  IIO_STATUS      CTLEInfo;
  UINT8           j;
  UINT8           IioDfxDev;
  UINT8           IioDfxFunc;
  UINT8           LinkSpeed;
  UINT32          RegOffset_0 = 0 ,RegOffset_1 = 0;
  UINT32          Data,OrValueCtlep_0,OrValueCtlep_1;
  UINT32          AndValueCtlep_0, AndValueCtlep_1;

  for (LinkSpeed=GEN3_LINK_SPEED_GEN1; LinkSpeed <= GEN3_LINK_SPEED_GEN3; LinkSpeed++)
  {
    switch (LinkSpeed)
    {
      case GEN3_LINK_SPEED_GEN1:
        RegOffset_0 = ONLY_REGISTER_OFFSET(RX_CTLE_PEAK_GEN1_0_IIO_DFX_REG);
        RegOffset_1 = ONLY_REGISTER_OFFSET(RX_CTLE_PEAK_GEN1_1_IIO_DFX_REG);
        break;
      case GEN3_LINK_SPEED_GEN2:
        RegOffset_0 = ONLY_REGISTER_OFFSET(RX_CTLE_PEAK_GEN2_0_IIO_DFX_REG);
        RegOffset_1 = ONLY_REGISTER_OFFSET(RX_CTLE_PEAK_GEN2_1_IIO_DFX_REG);
        break;
      case GEN3_LINK_SPEED_GEN3:
        RegOffset_0 = ONLY_REGISTER_OFFSET(RX_CTLE_PEAK_GEN3_0_IIO_DFX_REG);
        RegOffset_1 = ONLY_REGISTER_OFFSET(RX_CTLE_PEAK_GEN3_1_IIO_DFX_REG);
        break;
    } // switch(LinkSpeed)

    for (j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {
      IioDfxDev = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxDevice;
      IioDfxFunc = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxFunction;
      CTLEInfo = OemGetCTLEInfo(IioGlobalData ,Iio , IioDfxDev ,IioDfxFunc, &OrValueCtlep_0, &OrValueCtlep_1, &AndValueCtlep_0, &AndValueCtlep_1, LinkSpeed);
      if (CTLEInfo == IIO_SUCCESS) {
        // ctlep_0
        Data = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioDfxDev, IioDfxFunc, RegOffset_0));
        Data &= AndValueCtlep_0;
        Data |= OrValueCtlep_0;
        IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioDfxDev, IioDfxFunc, RegOffset_0), Data);

        if (j >= 3 ) { // CTLEP_1 present on 2A , 3A only
          // ctlep_1
          Data = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioDfxDev, IioDfxFunc, RegOffset_1));
          Data &= AndValueCtlep_1;
          Data |= OrValueCtlep_1;
          IioPciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioDfxDev, IioDfxFunc, RegOffset_1), Data);
        }
      } // Status
    } // for NUMBER_PORTS_PER_SOCKET
  } // for LinkSpeed

  MiscCtlepSetting(IioGlobalData, Iio, BusNumber);
}
