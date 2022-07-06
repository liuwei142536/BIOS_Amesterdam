//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c)  2010 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  IioRegs.h

Abstract:

  IIO constants.

Revision History:

--*/

#ifndef _IIO_REGS_H_
#define _IIO_REGS_H_

#include <Token.h> // AptioV override
#include "PlatformHost.h"



#define PCI_PCIE_ADDR(Bus,Device,Function,Offset) \
  (((Offset) & 0xfff) | (((Function) & 0x07) << 12) | (((Device) & 0x1f) << 15) | (((Bus) & 0xff) << 20))

#define IIO_BUS_NUM                               0
#define MaxIIO                                    MAX_SOCKET
#ifndef MAX_CPU_SOCKET
#define MAX_CPU_SOCKET                            MaxIIO
#endif
#define MaxPortDevices                            4
#define NUMBER_PORTS_PER_SOCKET                   11
#define MAX_TOTAL_IIO_PORTS                       MaxIIO * NUMBER_PORTS_PER_SOCKET
#define TOTAL_CBDMA3_DEVICES                      32
#define IIO_UPLINK_PORT_INDEX                     10   //Port 3D is the uplink port
#define MAX_TOTAL_CORE_HIDE                       16
//
// DMI related items
//
#define DMI_BUS_NUM                               IIO_BUS_NUM
#define DMI_DEV_NUM                               0
#define DMI_FUNC_NUM                              0

typedef enum {
  DmiTypeVc0,
  DmiTypeVc1,
  DmiTypeVcp,
  DmiTypeVcm,
  MaxDmiVcType
} DMI_VC_TYPE;

#define MaxDmiTcType                              8

//
// PCIe Port related items
//
#define PCIE_PORT_MIN                             1
#define PCIE_PORT_MAX                             10
#define PCIE_PORT_BUS_NUM                         IIO_BUS_NUM


//
// Logical enumeration used for IIO PCIE ports
// 
#define PORT_0_INDEX             0
// IOU2
#define PORT_1A_INDEX            1
#define PORT_1B_INDEX            2
// IOU0
#define PORT_2A_INDEX            3
#define PORT_2B_INDEX            4
#define PORT_2C_INDEX            5
#define PORT_2D_INDEX            6
//IOU1
#define PORT_3A_INDEX            7
#define PORT_3B_INDEX            8
#define PORT_3C_INDEX            9
#define PORT_3D_INDEX            10

#define IOU2_BIF_RANGE			2
#define IOU2_MAX_PORTS			2

#define x16_IOU_BIF_RANGE		5
#define x16_IOU_MAX_PORTS		4
//

#define PCIE_PORT_0_DEV    0x00
#define PCIE_PORT_0_FUNC    0x00

#define PCIE_PORT_1_DEV    0x01
#define PCIE_PORT_1A_FUNC   0x00
#define PCIE_PORT_1B_FUNC   0x01

#define PCIE_PORT_2_DEV    0x02
#define PCIE_PORT_2A_FUNC   0x00
#define PCIE_PORT_2B_FUNC   0x01
#define PCIE_PORT_2C_FUNC   0x02
#define PCIE_PORT_2D_FUNC   0x03

#define PCIE_PORT_3_DEV    0x03
#define PCIE_PORT_3A_FUNC   0x00
#define PCIE_PORT_3B_FUNC   0x01
#define PCIE_PORT_3C_FUNC   0x02
#define PCIE_PORT_3D_FUNC   0x03

#define PCIE_PORT_GLOBAL_DFX_DEV     0x06
#define PCIE_PORT_GLOBAL_DFX_FUNC    0x07

#define PCIE_PORT_CPU_CAPIDS_DEV     0x1E
#define PCIE_PORT_CPU_CAPIDS_FUNC    0x03

#define PCIE_PORT_0_DFX_DEV     0x06
#define PCIE_PORT_0_DFX_FUNC    0x00

#define PCIE_PORT_1A_DFX_DEV    0x06
#define PCIE_PORT_1A_DFX_FUNC   0x01
#define PCIE_PORT_1B_DFX_DEV    0x06
#define PCIE_PORT_1B_DFX_FUNC   0x02

#define PCIE_PORT_2A_DFX_DEV    0x06
#define PCIE_PORT_2A_DFX_FUNC   0x03
#define PCIE_PORT_2B_DFX_DEV    0x06
#define PCIE_PORT_2B_DFX_FUNC   0x04
#define PCIE_PORT_2C_DFX_DEV    0x06
#define PCIE_PORT_2C_DFX_FUNC   0x05
#define PCIE_PORT_2D_DFX_DEV    0x06
#define PCIE_PORT_2D_DFX_FUNC   0x06

#define PCIE_PORT_3A_DFX_DEV    0x07
#define PCIE_PORT_3A_DFX_FUNC   0x00
#define PCIE_PORT_3B_DFX_DEV    0x07
#define PCIE_PORT_3B_DFX_FUNC   0x01
#define PCIE_PORT_3C_DFX_DEV    0x07
#define PCIE_PORT_3C_DFX_FUNC   0x02
#define PCIE_PORT_3D_DFX_DEV    0x07
#define PCIE_PORT_3D_DFX_FUNC   0x03

#define PCIE_PORT_0_LINK_SEL  0x00
#define PCIE_PORT_1A_LINK_SEL  0x00
#define PCIE_PORT_1B_LINK_SEL  0x01
#define PCIE_PORT_2A_LINK_SEL  0x00
#define PCIE_PORT_2B_LINK_SEL  0x01
#define PCIE_PORT_2C_LINK_SEL  0x02
#define PCIE_PORT_2D_LINK_SEL  0x03
#define PCIE_PORT_3A_LINK_SEL  0x00
#define PCIE_PORT_3B_LINK_SEL  0x01
#define PCIE_PORT_3C_LINK_SEL  0x02
#define PCIE_PORT_3D_LINK_SEL  0x03

#define PCIE_PORT_0_SUPER_CLUSTER_PORT  0x00
#define PCIE_PORT_1A_SUPER_CLUSTER_PORT  0x01
#define PCIE_PORT_1B_SUPER_CLUSTER_PORT  0x01
#define PCIE_PORT_2A_SUPER_CLUSTER_PORT  0x03
#define PCIE_PORT_2B_SUPER_CLUSTER_PORT  0x03
#define PCIE_PORT_2C_SUPER_CLUSTER_PORT  0x03
#define PCIE_PORT_2D_SUPER_CLUSTER_PORT  0x03
#define PCIE_PORT_3A_SUPER_CLUSTER_PORT  0x07
#define PCIE_PORT_3B_SUPER_CLUSTER_PORT  0x07
#define PCIE_PORT_3C_SUPER_CLUSTER_PORT  0x07
#define PCIE_PORT_3D_SUPER_CLUSTER_PORT  0x07

#define R_PCIE_PORT_PCICMD                        0x04
#define   B_PCIE_PORT_PCICMD_PERRE                  BIT6
#define   B_PCIE_PORT_PCICMD_SERRE                  BIT8
#define   B_PCIE_PORT_PCICMD_INTDIS                 BIT10
#define R_PCIE_MISCCTRLSTS                        0x188

/*++
==================================================================================================
================================== Device 0, Function 0 - DMI Registers       ====================
==================================================================================================
--*/
#define R_PCIE_PORT_SNXTPTR                       0x41            // Subsystem ID Next Pointer
#define R_PCIE_PORT_SSVID                         0x2C            // Subsystem Vendor ID
#define R_PCIE_PORT_SSID                          0x2E            // Subsystem Device ID

#define R_PCIE_PORT_DMIRCBAR                      0x50            // DMI Root Complex Block Base Address
#define   V_DMIRCBA_SIZE                            (4*1024)      // DMI RCBA size is 4KB with Natural Alignment
#define   B_PCIE_PORT_DMIRCBAR_EN_SET              BIT0           // Enable changing for DMI Bar, must unset before LT Lock
#define R_PCIE_PORT_LNKCON2                       0xFC            // PCI Express Link Control 2 Register  

#define R_DMI_PORT_PXPLNKCTRL                    0x1B0
#define   B_DMI_PORT_PXPLNKCTRL_EXT_SYNCH          BIT7
#define   B_DMI_PORT_PXPLNKCTRL_CCCON              BIT6
#define   B_DMI_PORT_PXPLNKCTRL_RETRAIN_LINK       BIT5
#define   B_DMI_PORT_PXPLNKCTRL_LNKDIS             BIT4
#define R_DMI_PORT_PXPLNKSTS                     0x1B2
#define   B_DMI_PORT_PXPLNKSTS_LABS                BIT15
#define   B_DMI_PORT_PXPLNKSTS_LBMS                BIT14
#define   B_DMI_PORT_PXPLNKSTS_DLLA                BIT13
#define   B_DMI_PORT_PXPLNKSTS_SCCON               BIT12
#define   B_DMI_PORT_PXPLNKSTS_LINKTRAIN           BIT11
#define   B_DMI_PORT_PXPLNKSTS_NLNKWD              (BIT4 | BIT5 | BIT6 | BIT7 | BIT8 | BIT9)
#define   B_DMI_PORT_PXPLNKSTS_LNKSPD              (BIT0 | BIT1 | BIT2 | BIT3)

#define R_DMI_PORT_PXPLNKCTRL2                   0x1C0
#define   B_DMI_PORT_PXPLNKCTRL2_DEEMPH            BIT6
#define   B_DMI_PORT_PXPLNKCTRL2_ENCmpl            BIT4
#define   B_DMI_PORT_PXPLNKCTRL2_LNKSPEED          (BIT3 | BIT2 | BIT1 | BIT0)
#define     V_DMI_PORT_PXPLNKCTRL2_LS_GEN2           0x02
#define     V_DMI_PORT_PXPLNKCTRL2_LS_GEN1           0x01

#define R_XPUNCERRSTS                           0x208
#define RECEIVED_PCIE_COMPELTION_WITH_UR_STATUS BIT6



/*++
==================================================================================================
==================================                    RCRB                ========================
==================================================================================================
--*/
#define R_IIO_RCRB_DMIPVCCAP1                              0x04          // DMI Port Capability Register 1
#define R_IIO_RCRB_DMIVC0RCAP                              0x10          // DMI VC0 Resource Capability
#define R_IIO_RCRB_DMIVC0RCTL                              0x14          // DMI VC0 Resource Control
#define   B_IIO_RCRB_DMIVCxRCTL_VCx_EN                       BIT31       // Virtual Channel x Enable (VCxE)
#define   B_IIO_RCRB_DMIVCxRCTL_VCx_ID_MASK                  (7 << 24) // Bit[26:24]
#define   B_IIO_RCRB_DMIVCxRCTL_VCx_TCVCxM_MASK               0xFE
#define R_IIO_RCRB_DMIVC0RSTS                              0x1A          // DMI VC0 Resource Status
#define   B_IIO_RCRB_DMIVCxRSTS_VCx_NP                       BIT1        // Virtual Channel x Negotiation Pending (VCxNP)
#define R_IIO_RCRB_DMIVC1RCAP                              0x1C          // DMI VC1 Resource Capability
#define R_IIO_RCRB_DMIVC1RCTL                              0x20          // DMI VC1 Resource Control
#define R_IIO_RCRB_DMIVC1RSTS                              0x26          // DMI VC1 Resource Status
#define R_IIO_RCRB_DMIVCPRCAP                              0x28          // DMI VCP Resource Capability
#define R_IIO_RCRB_DMIVCPRCTL                              0x2C          // DMI VCP Resource Control
#define R_IIO_RCRB_DMIVCPRSTS                              0x32          // DMI VCP Resource Status
#define R_IIO_RCRB_DMIVCMRCAP                              0x34          // DMI VCM Resource Capability
#define R_IIO_RCRB_DMIVCMRCTL                              0x38          // DMI VCP Resource Control
#define R_IIO_RCRB_DMIVCMRSTS                              0x3E          // DMI VCM Resource Status
#define R_IIO_RCRB_DMILCTRL                                0x88          // DMI Link control
#define R_IIO_RCRB_DMILSTS                                 0x8A          // DMI Link status



/*++
==================================================================================================
================================== Device 1-3, Function 0 - PCIE Registers   ====================
==================================================================================================
--*/

// NOTE: Although not really applicaple for most UP/DP configuration the DMI port could be configured 
//       as a PCIE port.  In that configuration.

#define R_PCIE_PORT_IOBAS                         0x1C
#define   B_PCIE_PORT_MOREIO                        (BIT2|BIT3)
#define R_PCIE_PORT_CAPPTR                        0x34
#define R_PCIE_PORT_BCTRL                         0x3E
#define   B_PCIE_PORT_BCTRL_SBR                   BIT6
#define R_PCIE_PORT_SCAPID                        0x40
#define R_PCIE_PORT_SNXTPTR                       0x41
#define R_PCIE_PORT_SVID_OTHER                    0x44
#define R_PCIE_PORT_SID_OTHER                     0x46

#define R_PCIE_PORT_MSINXTPTR                     0x61
#define R_PCIE_PORT_MSICTRL                       0x62
#define   B_PCIE_PORT_MSICTRL_MSIEN                 BIT0
#define R_PCIE_PORT_MSIAR                         0x64
#define R_PCIE_PORT_MSGDAT                        0x68
#define R_PCIE_PORT_MSIMSK                        0x6C
#define R_PCIE_PORT_MSIPENDING                    0x70

#define R_PCIE_PORT_PXPNXTPTR                     0x91
#define R_PCIE_PORT_PXPCAP                        0x92
#define   B_PCIE_PORT_PXPCAP_SLOTIMPL               BIT8
#define   B_PCIE_PORT_PXPCAP_DPT                    (BIT4|BIT5|BIT6|BIT7)
#define R_PCIE_PORT_PXPDEVCAP                     0x94
#define R_PCIE_PORT_PXPDEVCTRL                    0x98
#define   B_PCIE_PORT_PXPDEVCTRL_ENNOSNP            BIT11
#define   B_PCIE_PORT_PXPDEVCTRL_MPS                (BIT5|BIT6|BIT7)
#define     V_PCIE_PORT_PXPDEVCTRL_MPS_128            (0<<5)
#define     V_PCIE_PORT_PXPDEVCTRL_MPS_256            (1<<5)
#define   B_PCIE_PORT_PXPDEVCTRL_CERE               BIT0
#define   B_PCIE_PORT_PXPDEVCTRL_NERE               BIT1
#define   B_PCIE_PORT_PXPDEVCTRL_FERE               BIT2
#define   B_PCIE_PORT_PXPDEVCTRL_URRE               BIT3
#define R_PCIE_PORT_PXPDEVSTS                     0x9A
#define   B_PCIE_PORT_PXPDEVCTRL_CERS               BIT0
#define   B_PCIE_PORT_PXPDEVCTRL_NERS               BIT1
#define   B_PCIE_PORT_PXPDEVCTRL_FERS               BIT2
#define   B_PCIE_PORT_PXPDEVSTS_UNSUPREQT           BIT3
#define R_PCIE_PORT_PXPLNKCAP                     0x9C
#define   B_PCIE_PORT_PXPLNKCAP_PORTNUM             (BIT31 | BIT30 | BIT29 | BIT28 |BIT27 | BIT26 | BIT25 | BIT24)
#define   B_PCIE_PORT_PXPLNKCAP_L1EXITL             (BIT17 | BIT16 | BIT15)
#define     V_PCIE_PORT_PXPLNKCAP_L1EXITL_SHIFT       15
#define   B_PCIE_PORT_PXPLNKCAP_L0SEXITL            (BIT14 | BIT13 | BIT12)
#define     V_PCIE_PORT_PXPLNKCAP_L0SEXITL_SHIFT      12
#define   B_PCIE_PORT_PXPLNKCAP_ASPMCTRL            (BIT10 | BIT11)
#define     B_PCIE_PORT_PXPLNKCAP_ASPMCTRL_L0S        (BIT0<<10)
#define     B_PCIE_PORT_PXPLNKCAP_ASPMCTRL_L1         (BIT1<<10)
#define   B_PCIE_PORT_PXPLNKCAP_MAXLNKWH            (BIT9 | BIT8 | BIT7 | BIT6 | BIT5  | BIT4)
#define   B_PCIE_PORT_PXPLNKCAP_LNKSPEED            (BIT3 | BIT2 | BIT1 | BIT0)
#define     V_PCIE_PORT_PXPLNKCAP_LS_GEN3             0x03
#define     V_PCIE_PORT_PXPLNKCAP_LS_GEN2             0x02
#define     V_PCIE_PORT_PXPLNKCAP_LS_GEN1             0x01
#define R_PCIE_PORT_PXPLNKCTRL                    0xA0
#define   B_PCIE_PORT_PXPLNKCTRL_EXT_SYNCH          BIT7
#define   B_PCIE_PORT_PXPLNKCTRL_CCCON              BIT6
#define   B_PCIE_PORT_PXPLNKCTRL_RETRAIN_LINK       BIT5
#define   B_PCIE_PORT_PXPLNKCTRL_LNKDIS             BIT4
#define R_PCIE_PORT_PXPLNKSTS                     0xA2
#define   B_PCIE_PORT_PXPLNKSTS_LABS                BIT15
#define   B_PCIE_PORT_PXPLNKSTS_LBMS                BIT14
#define   B_PCIE_PORT_PXPLNKSTS_DLLA                BIT13
#define   B_PCIE_PORT_PXPLNKSTS_SCCON               BIT12
#define   B_PCIE_PORT_PXPLNKSTS_NLNKWD              (BIT4 | BIT5 | BIT6 | BIT7 | BIT8 | BIT9)
#define   B_PCIE_PORT_PXPLNKSTS_LNKSPD              (BIT0 | BIT1 | BIT2 | BIT3)


#define R_PCIE_PORT_PXPSLOTCAP                    0xA4
#define   B_PCIE_PORT_PXPSLOTCAP_PSN                (BIT31 | BIT30 | BIT29 | BIT28 | BIT27 | BIT26 | BIT25 | BIT24)
#define   B_PCIE_PORT_PXPSLOTCAP_EIP                BIT17
#define   B_PCIE_PORT_PXPSLOTCAP_SPLS               (BIT16 | BIT15)
#define   B_PCIE_PORT_PXPSLOTCAP_SPLV               (BIT14 | BIT13 | BIT12 | BIT11 | BIT10 | BIT9 | BIT8 | BIT7)
#define   B_PCIE_PORT_PXPSLOTCAP_HPC                BIT6
#define   B_PCIE_PORT_PXPSLOTCAP_HPS                BIT5
#define   B_PCIE_PORT_PXPSLOTCAP_PIP                BIT4
#define   B_PCIE_PORT_PXPSLOTCAP_AIP                BIT3
#define   B_PCIE_PORT_PXPSLOTCAP_MRLSP              BIT2
#define   B_PCIE_PORT_PXPSLOTCAP_PCP                BIT1
#define   B_PCIE_PORT_PXPSLOTCAP_ABP                BIT0

#define R_PCIE_PORT_PXPSLOTCTRL                   0xA8
#define   B_PCIE_PORT_PXPSLOTCTRL_PWRLED            (BIT8 | BIT9)
#define     V_PCIE_PORT_PXPSLOTCTRL_PWRLED_OFF        (3<<8)
#define     V_PCIE_PORT_PXPSLOTCTRL_PWRLED_BLINK      (2<<8)
#define     V_PCIE_PORT_PXPSLOTCTRL_PWRLED_ON         (1<<8)
#define   B_PCIE_PORT_PXPSLOTCTRL_PWRCTRL           BIT10
#define     V_PCIE_PORT_PXPSLOTCTRL_PWRCTRL_OFF       (1<<10)
#define     V_PCIE_PORT_PXPSLOTCTRL_PWRCTRL_ON        (0<<10)
#define   B_PCIE_PORT_PXPSLOTCTRL_ATNLED            (BIT6 | BIT7)
#define     V_PCIE_PORT_PXPSLOTCTRL_ATNLED_OFF        (3<<6)
#define     V_PCIE_PORT_PXPSLOTCTRL_ATNLED_BLINK      (2<<6)
#define     V_PCIE_PORT_PXPSLOTCTRL_ATNLED_ON         (1<<6)
#define   B_PCIE_PORT_PXPSLOTCTRL_HPINTEN           BIT5
#define   B_PCIE_PORT_PXPSLOTCTRL_CCIEN             BIT4
#define   B_PCIE_PORT_PXPSLOTCTRL_PRSINTEN          BIT3
#define   B_PCIE_PORT_PXPSLOTCTRL_MRLINTEN          BIT2
#define   B_PCIE_PORT_PXPSLOTCTRL_PWRINTEN          BIT1
#define   B_PCIE_PORT_PXPSLOTCTRL_ATNINTEN          BIT0

#define R_PCIE_PORT_PXPSLOTSTS                    0xAA
#define   B_PCIE_PORT_PXPSLOTSTS_DLLSCS             BIT8
#define   B_PCIE_PORT_PXPSLOTSTS_EMLS               BIT7
#define   B_PCIE_PORT_PXPSLOTSTS_PDS                BIT6
#define   B_PCIE_PORT_PXPSLOTSTS_MRLSS              BIT5
#define   B_PCIE_PORT_PXPSLOTSTS_CMDCOMP            BIT4
#define   B_PCIE_PORT_PXPSLOTSTS_PRSDC              BIT3
#define   B_PCIE_PORT_PXPSLOTSTS_MRLSC              BIT2
#define   B_PCIE_PORT_PXPSLOTSTS_PWRFD              BIT1
#define   B_PCIE_PORT_PXPSLOTSTS_ABP                BIT0

#define R_PCIE_PORT_ROOTCON                       0xAC
#define   B_PCIE_PORT_ROOTCON_CRSSWVISEN            BIT4
#define   B_PCIE_PORT_ROOTCON_SECE                  BIT0
#define   B_PCIE_PORT_ROOTCON_SENE                  BIT1
#define   B_PCIE_PORT_ROOTCON_SEFE                  BIT2

#define R_PCIE_PORT_PXPDEVCAP2                    0xB4
#define   B_PCIE_PORT_LTR                           BIT11
#define   B_PCIE_PORT_ARI_EN                        BIT5


#define R_PCIE_PORT_PXPDEVCTRL2                   0xB8
#define   B_PCIE_PORT_PXPDEVCTRL2_LTR_EN            BIT10
#define   B_PCIE_PORT_PXPDEVCTRL2_ARI               BIT5
#define   B_PCIE_PORT_PXPDEVCTRL2_CTPE              BIT4
#define     V_PCIE_PORT_PXPDEVCTRL2_CTPE_EN           (1<<4)
#define   B_PCIE_PORT_PXPDEVCTRL2_CTV               (BIT0 | BIT1 | BIT2 | BIT3)

#define R_PCIE_PORT_PXPLINKCAP2                   0xBC
#define   B_PCIE_PORT_PXPLNKCAP2_LNKSPDVEC         (BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7)
#define     V_PCIE_PORT_PXPLNKCAP2_LNKSPDVEC_3G    BIT3
#define     V_PCIE_PORT_PXPLNKCAP2_LNKSPDVEC_2G    BIT2
#define     V_PCIE_PORT_PXPLNKCAP2_LNKSPDVEC_1G    BIT1
/* LINK supports G3, G2 and G1 */
#define     V_PCIE_PORT_PXPLNKCAP2_LNKSPDVEC_3G_TOP    ( BIT3 | BIT2 | BIT1)
/* LINK supports G2 and G1 */
#define     V_PCIE_PORT_PXPLNKCAP2_LNKSPDVEC_2G_TOP    ( BIT2 | BIT1 )
/* LINK only supports G1   */
#define     V_PCIE_PORT_PXPLNKCAP2_LNKSPDVEC_1G_TOP    BIT1

#define R_PCIE_PORT_PXPLNKCTRL2                   0xC0
#define   B_PCIE_PORT_PXPLNKCTRL2_DEEMPH            BIT6
#define   B_PCIE_PORT_PXPLNKCTRL2_ENCmpl            BIT4
#define   B_PCIE_PORT_PXPLNKCTRL2_LNKSPEED          (BIT3 | BIT2 | BIT1 | BIT0)
#define     V_PCIE_PORT_PXPLNKCTRL2_LS_GEN3           0x03
#define     V_PCIE_PORT_PXPLNKCTRL2_LS_GEN2           0x02
#define     V_PCIE_PORT_PXPLNKCTRL2_LS_GEN1           0x01

#define R_PCIE_PORT_PXPPMCAP                      0xE0
#define R_PCIE_PORT_PXPPMCSR                      0xE4
#define   B_PCIE_PORT_PXPPMCSR_PWR_STATE              (BIT1 | BIT0)
#define     V_PCIE_PORT_PXPPMCSR_PWR_STATE_D0         0x00
#define     V_PCIE_PORT_PXPPMCSR_PWR_STATE_D3HOT      0x03

#define R_ACSCTRL                                 0x116
#define R_PCIE_PORT_ACSCTRL                       R_ACSCTRL
#define   B_PCIE_PORT_ACSCTRL_U                     BIT4
#define   B_PCIE_PORT_ACSCTRL_C                     BIT3
#define   B_PCIE_PORT_ACSCTRL_R                     BIT2
#define   B_PCIE_PORT_ACSCTRL_B                     BIT1
#define   B_PCIE_PORT_ACSCTRL_V                     BIT0

#define R_PCIE_PORT_APICBASE                      0x140
#define   B_PCIE_PORT_APICBASE_ENABLE               BIT0
#define R_PCIE_PORT_APICLIMIT                     0x142
#define R_PCIE_PORT_UNCERRSTS                     0x14C
#define   B_PCIE_PORT_UNSUPPORTED_REQUEST           BIT20

#define R_PCIE_PORT_CORERRMSK                     0x15C
#define   B_PCIE_CORERRMSK_ANFE_MASK               BIT13

#define R_PCIE_PORT_RPERRCMD                      0x174
#define   B_PCIE_CORRECTABLE_ERR_RPT_EN           BIT0
#define   B_PCIE_NONFATAL_ERR_RPT_EN              BIT1
#define   B_PCIE_FATAL_ERR_RPT_EN                 BIT2

#define R_PCIE_PORT_PCIEIPERFCTRLSTS              0x180
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_TPHDIS                             BIT41
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_DCA_ReqID_Override                 BIT40
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_max_read_completion_combine_size   BIT35
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_outstanding_requests_gen1          BIT16   //BIT 20:16 Outstanding Requests for Gen1
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_outstanding_requests_gen2          BIT8    //BIT 13:8 Outstanding Requests for Gen2
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_UseAllocatingFlowWr                BIT7
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_vcp_roen_nswr                      BIT6
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_vcp_nsen_rd                        BIT5
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_SIS          BIT4
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_NSOW         BIT3
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_NSOR         BIT2
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_SSP          BIT1
#define   B_PCIE_PORT_PCIEIPERFCTRLSTS_SSCTL        BIT0

#define R_PCIE_PORT_PCIEMISCCTRLSTS               0x188
#ifdef  DE_SKU
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_dis_send_pme_to_port   BIT62
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_mimic_pme_to_ack_rcvd  BIT63
#endif  //DE_SKU
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_Locked_Rd_TO        BIT49
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_Rcv_PME_TO_ACK      BIT48
#define   B_PROBLEMATIC_PORT_FOR_LOCK_FLOW                BIT38
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_Dis_MCTP_BC_to_lnk  BIT37
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_FormFact            BIT36
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_MSIFATEN            BIT35
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_MSINFATEN           BIT34
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_MSICOREN            BIT33
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_ACPI_PMEEN          BIT32
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_L0sTx           BIT31
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_InboundIOEN         BIT30
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_TO_DIS              BIT28
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_SIOLnkBWSts         BIT27
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_EOI             BIT26
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_INB_P2PWrtDis   BIT25
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_INB_P2PRdDis    BIT24
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_PHold           BIT23
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_PME_Ack_TOCtl       (BIT8 | BIT7)
#define   V_PCIE_PORT_PCIEMISCCTRLSTS_PME_AckTOCtl        0x7
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_PME_TO_ACK_TIMEOUT  BIT6
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_Send_PME_OFF        BIT5
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_SysERR_only4_AER    BIT4
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_EN_ACPI_HotPlug     BIT3
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_EN_ACPI_PM          BIT2
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_Inbound_CFGEN       BIT1
#define   B_PCIE_PORT_PCIEMISCCTRLSTS_Set_HostBrdg_CC     BIT0

#define R_PCIE_PORT_PCIEIOUxBIFCTRL               0x190     // registe is only applicapble in device 1, 3, & 7
#define   B_PCIE_PORT_PCIEIOUxBIFCTRL_SBIF          BIT3    // This is the bit to initiate the bifurcuatin on the port
#define   B_PCIE_PORT_PCIEIOUxBIFCTRL_LNKCFG       (BIT0 + BIT1 + BIT2)
#ifdef DE_SKU			//applicable only for the microserver IOSF RP of IOU0
#define	  B_IOSF_PORT_CBDMA_ENABLE					BIT4	//enable the CBDMA port on IOSF 0,2,0
#define	  B_IOSF_PORT_CPM_ENABLE					BIT4	//NS Comms Sku enable the CPM port on IOSF 0,2,0
#define	  B_IOSF_PORT_GBE_ENABLE					BIT5	//enable the GbE port on IOSF 0,2,2
#define   B_IOSF_PORT_GBE1_CPM_ENABLE              BIT6    //NS enable the second GbE (Comms Sku) or CPM (Storage Sku) port on IOSF 0,2,3

#endif	//DE_SKU
#define R_PCIE_PORT_XPCORERRSTS                   0x200
#define   B_PCIE_PORT_XPCORERRSTS_LBS               BIT1
#define R_PCIE_PORT_XPCORERRMSK                   0x204
#define   B_PCIE_PORT_XPCORERRMSK_LBM               BIT1
#define R_PCIE_PORT_XPUNCERRSTS                   0x208
#define   V_PCIE_PORT_XPUNCERRSTS                   (BIT1 + BIT3 + BIT4 + BIT5 + BIT6 + BIT8 + BIT9)
#define R_PCIE_PORT_XPUNCERRMSK                   0x20c
#define   V_PCIE_PORT_XPUNCERRMSK                   (BIT1 + BIT3 + BIT4 + BIT5 + BIT6 + BIT8 + BIT9)
#define R_PCIE_PORT_XPUNCERRSEV                   0x210
#define   V_PCIE_PORT_XPUNCERRSEV                   (BIT1 + BIT3 + BIT4 + BIT5 + BIT6 + BIT8 + BIT9)
#define R_PCIE_PORT_XPUNCERRPTR                   0x214
#define R_PCIE_PORT_UNCEDMASK                     0x218
#define R_PCIE_PORT_COREDMASK                     0x21c
#define   V_PCIE_PORT_COREDMASK                     (BIT1 + BIT6 + BIT7 + BIT8 + BIT12 + BIT13)
#define R_PCIE_PORT_RPEDMASK                      0x220
#define R_PCIE_PORT_XPUNCEDMASK                   0x224
#define R_PCIE_PORT_XPCOREDMASK                   0x228
#define R_PCIE_PORT_XPGLBERRSTS                   0x230
#define   B_PCIE_PORT_XPGLBERRSTS_FAER              BIT0
#define   B_PCIE_PORT_XPGLBERRSTS_NAER              BIT1
#define   B_PCIE_PORT_XPGLBERRSTS_CAER              BIT2
#define R_PCIE_PORT_XPGLBERRPTR                   0x232

#define R_PCIE_PORT_LN0LN3EQ                      0x25C

#define R_NTB_PPD                                 0xD4
#define   B_NTB_PORT_DEF                           (BIT0 | BIT1)

#define R_NTB_PORT_PXPLNKSTS                     0x1A2
#define   B_NTB_PORT_PXPLNKSTS_DLLA                BIT13
#define   B_NTB_PORT_PXPLNKSTS_SCCON               BIT12
#define   B_NTB_PORT_PXPLNKSTS_NLNKWD              (BIT4 | BIT5 | BIT6 | BIT7 | BIT8 | BIT9)
#define   B_NTB_PORT_PXPLNKSTS_LNKSPD              (BIT0 | BIT1 | BIT2 | BIT3)

#define IIO_NTB_DEVICE_NUM      0x3
#define IIO_NTB_FUNC_NUM        0x0
#define NTB_PORT_DEF_TRANSPARENT  0
#define NTB_PORT_DEF_NTB_NTB      1
#define NTB_PORT_DEF_NTB_RP       2
#define NTB_PORT_DEF_NTB_AUTO     3


#define IOAT_DEVICE_NUM     0x04
#define IOAT_FUNC_START     0x00
#define IOAT_TOTAL_FUNCS    0x08
#define R_IIO_CB_DEVCON                   0x98
#define   B_IIO_CB_DEVCON_RELAX_OR_EN     BIT4
#define R_IIO_CB_DEVCFG                   0x60

#ifdef DE_SKU
#define R_BWD_CB33_DEVCON                 0x48
#define R_BWD_CB33_DEVCFG                 0x7C
#define   B_BWD_CB33_NTBMDEN              BIT7
#define   B_BWD_CB33_MCEN                 BIT6
#define   B_BWD_CB33_DIFEN                BIT3
#define   B_BWD_CB33_RAID6EN              BIT2
#define   B_BWD_CB33_CBEN                 BIT0
#define R_BWD_CB33_ERRCORMSK              0x114
#define   B_BWD_CB33_ANFEM                BIT13
#define R_BWD_CB33_IOSFDEVCLKGCTL         0xEC
#define   B_BWD_CB33_IOSFDEVCLKGCTL_ICGE  BIT15
#define R_BWD_CB33_SBDEVCLKGCTL           0xEE
#define   B_BWD_CB33_SBDEVCLKGCTL_ICGE    BIT15
#endif  //DE_SKU

#ifdef DE_SKU
#define R_IIO_CB33_DEVCON                 R_BWD_CB33_DEVCON
#define R_IIO_CB33_DEVCFG                 R_BWD_CB33_DEVCFG
#else
#define R_IIO_CB33_DEVCON                 R_IIO_CB_DEVCON
#define R_IIO_CB33_DEVCFG                 R_IIO_CB_DEVCFG
#endif
/*++
==================================================================================================
================================== Device 5, Function 0  =========================================
==================================================================================================
--*/
//
// Dev 5, Func 0 - Intel VT, Address Map, System Management, Miscellaneous Registers
//
#define CORE05_BUS_NUM                            IIO_BUS_NUM
#define CORE05_DEV_NUM                            5
#define CORE05_FUNC_NUM                           0
#define CORE05_MAX_FUNC                           4

#define SECBUS_TEMP_DEV_NUM                       0
#define SECBUS_TEMP_FUNC_NUM                      0

#define IIO_DFX_VTD_DEV                           7
#define IIO_DFX_VTD_FUNC                          4

#define IIO_VTD_DEV                               CORE05_DEV_NUM
#define IIO_VTD_FUN                               CORE05_FUNC_NUM

#define NTB_DEV                                   3
#define NTB_FUNC                                  0

#define R_IIO_HDRTYPECTRL                      0x80              // PCI Header Type Control
#define   B_IIO_HDR_TYPE_DEVICE1                 BIT0				  // Set Header Type to Single Function (clear MFD bit) device 1
#define   B_IIO_HDR_TYPE_DEVICE2                 BIT1
#define   B_IIO_HDR_TYPE_DEVICE3                 BIT2
#define R_IIO_MMCFG                            0x84              // MMCFG Address Range
#define R_IIO_MMCFG_B0                         0x90              // MMCFG Address Range //AptioV server override: Dynamic mmcfg base address change
#define R_IIO_TOMMIOL_OB                       0xA4              // Outbound memory address blocking
#define R_IIO_TSEGCRTL                         0xA8              // TSeg Address Range
#define R_IIO_GENPROTRANGE1Base0               0xB0              // Generic Protected Memory Range 1 Base Address 0
#define R_IIO_GENPROTRANGE1Base1               0xB4              // Generic Protected Memory Range 1 Base Address 1
#define R_IIO_GENPROTRANGE1Limit0              0xB8              // Generic Protected Memory Range 1 Limit Address 0
#define R_IIO_GENPROTRANGE1Limit1              0xBC              // Generic Protected Memory Range 1 Limit Address 1
#define R_IIO_GENPROTRANGE2Base0               0xC0              // Generic Protected Memory Range 2 Base Address 0
#define R_IIO_GENPROTRANGE2Base1               0xC4              // Generic Protected Memory Range 2 Base Address 1
#define R_IIO_GENPROTRANGE2Limit0              0xC8              // Generic Protected Memory Range 2 Limit Address 0
#define R_IIO_GENPROTRANGE2Limit1              0xCC              // Generic Protected Memory Range 2 Limit Address 1
#define R_IIO_TOLM                             0xD0              // Top of Low Memory
#define R_IIO_TOHM                             0xD4              // Top of High Memory
#define R_IIO_NCMEMBASE                        0xE0              // NCMEM Base
#define R_IIO_NCMEMLIMIT                       0xE8              // NCMEM Limit

#define R_IIO_MENCMEM_BASE                     0xF0              // ME Non-Coherent Memory Base Address
#define R_IIO_MENCMEM_LIMIT                    0xF8              // ME Non-Coherent Memory Limit Address

#define R_IIO_CPUBUSNO                         0x108             // CPU Internal Bus Numbers
#define   B_IIO_CPUBUSNO_CLAIM_PCI_CON           BIT16           // Valid

#define R_IIO_LMMIOL                           0x10C             // Local MMIO Low Base
#define R_IIO_LMMIIOBASE                       0x110             // Local MMIO High Base
#define R_IIO_LMMIIOLIMIT                      0x112             // Local MMIO High Limit
#define R_IIO_LMMIIOBASEU                      0x114             // Local MMIO High Base U
#define R_IIO_LMMIIOLIMITU                     0x118             // Local MMIO High Limit U
#define R_IIO_GENPROTRANGE0Base                0x120             // Generic Protected Memory Range 0 Base Address
#define R_IIO_GENPROTRANGE0Base_1              0x124             // Generic Protected Memory Range 0 Base Address
#define R_IIO_GENPROTRANGE0Limit               0x128             // Generic Protected Memory Range 0 Limit Address
#define R_IIO_GENPROTRANGE0Limit_1             0x12C             // Generic Protected Memory Range 0 Limit Address

#define R_IIO_CIPCTRL                          0x140             // Coherent Interface Protocol Control
#define   B_IIO_CIP_FLU_WRITE                    BIT31           // Flush Currently Pending Writes to dram from Write Cache
#define   B_IIO_CIP_WRI_UPDATE_DIS               BIT28           // Disable WriteUpdate Flow
#define   B_IIO_CIP_EXRTID_MODE                  BIT2            // Extended RTID Mode Enable
#define   B_IIO_CIP_WRI_COMBIN_DIS               BIT1            // Disable write combining
#define   V_IIO_CIP_PCIRDCURRENT                 0               // PCIRdCurrent/DRd.UC mode select
#define   V_IIO_CIP_DRDUC                        1

#define R_IIO_CIPSTS                           0x144             // Coherent Interface Protocol Status
#define R_IIO_CIPDCASAD                        0x148             // Coherent Interface Protocol DCA Source Address
#define   B_IIO_CIP_TPH_DCA_EN                   BIT0            // Enable TPH/DCA
#define R_IIO_CIPINTRC                         0x14C             // Coherent Interface Protocol Interrupt Control
#define   B_IIO_CIP_SMI_MASK                     BIT10           // SMI Mask
#define R_IIO_CIPINTRS                         0x150             // Coherent interface Protocol Interrupt Status


#define R_IIO_VTBAR                            0x180             // Base Address Register for Intel VT-d Registers
#define   B_IIO_VT_BAR_EN                        BIT0            // Intel VT-d Base Address Enable
#define   V_VT_D_IIO_BAR_SIZE                    (8*1024)        // VT-D IIO BAR is 8KB with Natural Alignment
#define R_IIO_VTGENCTRL                        0x184             // Intel VT-d General Control
#define   V_IIO_VTCTRL_LOCK_VTD_SET            BIT15             // VT-D Bar lock bit
#define   V_IIO_VTCTRL_LIMITS_CLEAR              0x0FF00         // Clear bits [7:0]
#define   V_IIO_VTCTRL_VTIGPAL_47BIT             0x07
#define   V_IIO_VTCTRL_VTHPAL_38BIT              0x03
#define   V_IIO_VTCTRL_VTHPAL_39BIT              0x04
#define   V_IIO_VTCTRL_VTHPAL_46BIT              0x0A
#define   V_IIO_VTCTRL_VTHPAL_48BIT              0x0C
#define   V_IIO_VTCTRL_VTHPAL_51BIT              0x0F
#define   V_IIO_VTCTRL_VTNGPAL_48BIT             0x08
#define R_IIO_VTISOCHCTRL                      0x188            // Intel VT-d Isoch Related Control
#define   B_IIO_VTISOCHCTRL_AZALIA_ON_VCP        BIT8           // Azalia on VCP bit
#define R_IIO_VTGENCTRL2                       0x18C            // Intel VT-d General Control 2
#define   B_IIO_VTGENCTRL2_PrefCtrl              (BIT6 | BIT5)  // Prefetch Control
#define R_IIO_IOTLBPARTITION                   0x194            // IOTLB Partitioning Control
#define   B_IIO_IOT_PARTITION_EN                 BIT0           // IOTLB Partitioning Enable
#define R_IIO_VTUNCERRSTS                      0x1A8            // Uncorrectable Error Status
#define R_IIO_VTUNCERRMSK                      0x1AC            // Intel VT Uncorrectable Error Mask
#define   B_IIO_VTUNCERRMSK_MASK                 BIT31          // Mask reporting Intel VT-d defined errors to IIO core logic
#define   B_MISCERRM                              BIT7
#define   B_VTDERR_MASK                           BIT31
#define R_IIO_VTUNCERRSEV                      0x1B0            // Intel VT Uncorrectable Error Severity
#define R_IIO_VTUNCERRPTR                      0x1B4            // Intel VT Uncorrectable Error Pointer

#define R_IIO_IIOMISCCTRL                      0x1C0            // IIO MISC Control
#define   B_IIO_MISC_POI_FORWARD_END             BIT37          // Poison Forwarding Enable
#define   B_LOCK_THAWING_MODE                    BIT12          // Lock Thawing Mode
#define   B_IIO_MISC_DIS_RO_CBDMA                BIT22          // Disable RO on writes from CB DMA
#define   B_IIO_MISC_DIS_INBOUNT_IO              BIT18          // Disable inbound RO for VC0/VCp writes
#define   B_IIO_MISC_EN_1K                       BIT2           // EN1K
#define   B_IIO_DIS_UNUSE_PORT                   BIT1           // Disable Unused Ports (place in L2)
#define   B_IIO_MISC_ISA_HOLE_EN                 BIT0           // Enable ISA Hole

#define R_IRP_MISC_DFX0                         0x800
#define R_IRP_MISC_DFX1                         0x804
#define R_IRP_MISC_DFX2                    0x808             // IRP MISC DF2
#define  R_IIO_IRP_MISC_DFX1                   0x804            // Coherent Interface Miscellaneous DFx 1
#define    B_IIO_IRP_MISC_DFX1_TIMEOUT         (BIT11|BIT10)    // Config Retry Timeout

#define CORE05_REG_ADDR(x) \
    ((UINT64) ((((UINTN) CORE05_DEV_NUM) << 16) + (((UINTN) CORE05_FUNC_NUM) << 8) + ((UINTN) ((x)))))

#define CORE05_PCIE_ADDR(Offset) \
          (((Offset) & 0xFFF) | (((CORE05_FUNC_NUM) & 0x07) << 12) | (((CORE05_DEV_NUM) & 0x1F) << 15) | (((CORE05_BUS_NUM) & 0xFF) << 20))



/*++
==================================================================================================
================================== Device 5, Function 2 =========================================
==================================================================================================
--*/

//
// MISC IIO control and status & Global error map registers
//
#define CORE05_FUNC2_NUM                          2
#define CORE05F2_REG_ADDR(x) \
    ((UINT64) ((((UINTN) CORE05_DEV_NUM) << 16) + (((UINTN) CORE05_FUNC2_NUM) << 8) + ((UINTN) ((x)))))
//
// sr0-7 Sticky scratch pad registers
//
#define R_IIO_IRPPERRSV                               0x80                          // IRP Protocol Error Severity
#define R_IIO_IIOERRSV                                0x88                          // IIO Core Error Severity
#define R_IIO_PCIERRSV                                0x94                          // PCI-E Error Severity Map Register
#define   V_IIO_PCIERRSV0                               00                          // Error Severity 0
#define   V_IIO_PCIERRSV1                               01                          // Error Severity 1
#define   V_IIO_PCIERRSV2                               02                          // Error Severity 2
#define R_IIO_SYSMAP                                  0x9C                          // System Error Event Map Register
#define   V_IIO_SYSMAP_NoInband                         000                         // No inband message
#define   V_IIO_SYSMAP_SMI_PMI                          001                         // Generate SMI/PMI
#define   V_IIO_SYSMAP_NMI                              002                         // Generate NMI
#define   V_IIO_SYSMAP_CPEI                             003                         // Generate CPEI
#define   V_IIO_SYSMAP_MCA                              004                         // Generate MCA
#define R_IIO_ERRPINCTL                               0xA4                          // Error Pin Control
#define   V_IIO_ERRPINCTL_DIS                           00                          // Pin Assertion Control
#define   V_IIO_ERRPINCTL_DATA                          01
#define   V_IIO_ERRPINCTL_SEV                           02
#define   V_IIO_ERRPINCTL_RSVD                          03
#define R_IIO_ERRPINST                                0xA8                          //Error Pin Status
#define   B_IIO_ERRPINST_ERR0                           BIT0                        //Pin status
#define   B_IIO_ERRPINST_ERR1                           BIT1
#define   B_IIO_ERRPINST_ERR2                           BIT2
#define R_IIO_VPPCTL                                  0xB0                          // VPP Control
#define   B_IIO_VPPCTL_Enb                              BIT44                       // VPP enable
#define   B_IIO_VPPCTL_RPrt                             BIT0                        // VPP Root Port
#define   B_IIO_VPPCTL_AAddr                            (BIT3 | BIT2 | BIT1)        // VPP Assigned Address

#define R_IIO_VPPSTS                                  0xB8                          // VPP Status
#define R_IIO_GNERRST                                 0x1C0                         // Global Non-Fatal Error Status
#define R_IIO_GFERRST                                 0x1C4                         // Global Fatal Error Status
#define   B_IIO_GERRST_PCIE0                            BIT5                        // PCI-E [0] Fatal Error Status
#define   B_IIO_GERRST_PCIE1                            BIT6                        // PCI-E [1] Fatal Error Status
#define   B_IIO_GERRST_PCIE2                            BIT7                        // PCI-E [2] Fatal Error Status
#define   B_IIO_GERRST_PCIE3                            BIT8                        // PCI-E [3] Fatal Error Status
#define   B_IIO_GERRST_PCIE4                            BIT9                        // PCI-E [4] Fatal Error Status
#define   B_IIO_GERRST_PCIE5                            BIT10                       // PCI-E [5] Fatal Error Status
#define   B_IIO_GERRST_PCIE6                            BIT11                       // PCI-E [6] Fatal Error Status
#define   B_IIO_GERRST_PCIE7                            BIT12                       // PCI-E [7] Fatal Error Status
#define   B_IIO_GERRST_PCIE8                            BIT13                       // PCI-E [8] Fatal Error Status
#define   B_IIO_GERRST_PCIE9                            BIT14                       // PCI-E [9] Fatal Error Status
#define   B_IIO_GERRST_PCIE10                           BIT15                       // PCI-E [10] Fatal Error Status
#define   B_IIO_GERRST_DMI                              BIT20                       // DMI Fatal Error Status
#define R_IIO_GERRCTL                                 0x1C8                         // Global Error Control
#define   B_IIO_GERRCTL_PCIE0                           BIT5                        // PCI-E [0] Error Enable
#define   B_IIO_GERRCTL_PCIE1                           BIT6                        // PCI-E [1] Error Enable
#define   B_IIO_GERRCTL_PCIE2                           BIT7                        // PCI-E [2] Error Enable
#define   B_IIO_GERRCTL_PCIE3                           BIT8                        // PCI-E [3] Error Enable
#define   B_IIO_GERRCTL_PCIE4                           BIT9                        // PCI-E [4] Error Enable
#define   B_IIO_GERRCTL_PCIE5                           BIT10                       // PCI-E [5] Error Enable
#define   B_IIO_GERRCTL_PCIE6                           BIT11                       // PCI-E [6] Error Enable
#define   B_IIO_GERRCTL_PCIE7                           BIT12                       // PCI-E [7] Error Enable
#define   B_IIO_GERRCTL_PCIE8                           BIT13                       // PCI-E [8] Error Enable
#define   B_IIO_GERRCTL_PCIE9                           BIT14                       // PCI-E [9] Error Enable
#define   B_IIO_GERRCTL_PCIE10                          BIT15                       // PCI-E [10] Error Enable
#define   B_IIO_GERRCTL_DMI                             BIT20                       // DMI Error Enable
#define R_IIO_GSYSST                                  0x1CC                         // Global System Event Control Register
#define   V_IIO_GSYSCTL_S0STS                           BIT0                        // Severity0 Error Status
#define   V_IIO_GSYSCTL_S1STS                           BIT1                        // Severity1 Error Status
#define   V_IIO_GSYSCTL_S2STS                           BIT2                        // Severity2 Error Status
#define   V_IIO_GSYSCTL_S3STS                           BIT3                        // Severity3 Error Status
#define   V_IIO_GSYSCTL_S4STS                           BIT4                        // Severity4 Error Status
#define R_IIO_GSYSCTL                                 0x1D0                         // Global System Event Control Register
#define   V_IIO_GSYSCTL_S0EN                            BIT0                        // Severity0 Error enable
#define   V_IIO_GSYSCTL_S1EN                            BIT1                        // Severity1 Error enable
#define   V_IIO_GSYSCTL_S2EN                            BIT2                        // Severity2 Error enable
#define   V_IIO_GSYSCTL_S3EN                            BIT3                        // Severity3 Error enable
#define   V_IIO_GSYSCTL_S4EN                            BIT4                        // Severity4 Error enable
#define R_IIO_GFFERRST                                0x1DC                         // Global Fatal FERR Status
#define R_IIO_GFNERRST                                0x1E8                         // Global Fatal NERR Status
#define R_IIO_GNFERRST                                0x1EC                         // Global Non-Fatal FERR Status
#define R_IIO_GNNERRST                                0x1F8                         // Global Non-Fatal NERR Status
#define R_IIO_IRPP0ERRST                              0x230                         // IRP Protocol Error Status
#define R_IIO_IRPP0ERRCTL                             0x234                         // IRP Protocol Error Control
#define   B_IIO_IPRRCTL_PAR_ERR_EN                      BIT14                       // Protocol Parity Error (DB)
#define   B_IIO_IRPPCTL_QUE_TAB_FLOW_EN                 BIT13                       // Protocol Queue/Table Overflow or Underflow (DA)
#define   B_IIO_IRPPCTL_LAY_REC_RES_EN                  BIT10                       // Protocol Layer Received Unexpected Response/Completion (D7)
#define   B_IIO_IRPPCTL_CSR_ACC_EN                      BIT4                        // CSR access crossing 32-bit boundary (C3)
#define   B_IIO_IRPPCTL_CACHE_UNCOR_END                 BIT3                        // Write Cache Un-correctable ECC (C2)
#define   B_IIO_IRPPCTL_LAY_EN                          BIT2                        // Protocol Layer Received Poisoned Packet (C1)
#define   B_IIO_IRPPCTL_WRI_CACHE_COR_EN                BIT1                        // Write Cache Correctable ECC (B4)
#define R_IIO_IRPP0FFERRST                            0x238                         // IRP Protocol Fatal FERR Status
#define R_IIO_IRPP0FNERRST                            0x23C                         // IRP Protocol Fatal NERR Status
#define R_IIO_IRPP0FFERRHD0                           0x240                         // IRP Protocol Fatal FERR Header Log 0
#define R_IIO_IRPP0FFERRHD1                           0x244             
#define R_IIO_IRPP0FFERRHD2                           0x248
#define R_IIO_IRPP0FFERRHD3                           0x24C
#define R_IIO_IRPP0NFERRST                            0x250                         // IRP Protocol Non-Fatal FERR Status
#define R_IIO_IRPP0NNERRST                            0x254                         // IRP Protocol Non-Fatal NERR Status
#define R_IIO_IRPP0NFERRHD0                           0x258                         // IRP Protocol Non-Fatal FERR Header Log 0
#define R_IIO_IRPP0NFERRHD1                           0x25C
#define R_IIO_IRPP0NFERRHD2                           0x260
#define R_IIO_IRPP0NFERRHD3                           0x264
#define R_IIO_IRPP0ERRCNTSEL                          0x268                        // IRP Protocol Error Counter Select 
#define R_IIO_IRPP0ERRCNT                             0x26C                        // IRP Protocol Error Counter
#define R_IIO_IRPP1ERRST                              0x2B0                        // IRP Protocol Error Status
#define R_IIO_IRPP1ERRCTL                             0x2B4                        // IRP Protocol Error Control
#define   B_IIO_IRPERR_PRO_PAR_ERR_EN                   BIT14                      // Protocol Parity Error (DB)
#define   B_IIO_IPRERR_PRO_QUETAB_FLOW_EN               BIT13                      // Protocol Queue/Table Overflow or Underflow (DA)
#define   B_IIO_IPRERR_PRO_REC_UNRES_EN                 BIT10                      // Protocol Layer Received Unexpected Response/Completion (D7)
#define   B_IIO_IPRERR_CSR_CRO32BIT_BOUN_EN             BIT4                       // CSR access crossing 32-bit boundary (C3)
#define   B_IIO_IPRERR_WRI_CAC_UNCOR_ECC_EN             BIT3                       // Write Cache Un-correctable ECC (C2)
#define   B_IIO_IPRERR_PRO_LAY_REC_POIPAC_EN            BIT2                       // Protocol Layer Received Poisoned Packet (C1)
#define   B_IIO_IPRERR_WRI_CAC_CORECC_EN                BIT1                       // Write Cache Correctable ECC (B4)
#define R_IIO_IRPP1FFERRST                            0x2B8                        // IRP Protocol Fatal FERR Status
#define R_IIO_IRPP1FNERRST                            0x2BC                        // IRP Protocol Fatal NERR Status
#define R_IIO_IRPP1FFERRHD0                           0x2C0                        // IRP Protocol Fatal FERR Header Log 0
#define R_IIO_IRPP1FFERRHD1                           0x2C4
#define R_IIO_IRPP1FFERRHD2                           0x2C8
#define R_IIO_IRPP1FFERRHD3                           0x2CC
#define R_IIO_IRPP1NFERRST                            0x2D0                        // IRP Protocol Non-Fatal FERR Status
#define R_IIO_IRPP1NNERRST                            0x2D4                        // IRP Protocol Non-Fatal NERR Status
#define R_IIO_IRPP1NFERRHD0                           0x2D8                        // IRP Protocol Non-Fatal FERR Header Log 0
#define R_IIO_IRPP1NFERRHD1                           0x2DC 
#define R_IIO_IRPP1NFERRHD2                           0x2E0
#define R_IIO_IRPP1NFERRHD3                           0x2E4
#define R_IIO_IRPP1ERRCNTSEL                          0x2E8                        // IRP Protocol Error Counter Select
#define R_IIO_IRPP1ERRCNT                             0x2EC                        // IRP Protocol Error Counter
#define R_IIO_IIOERRST                                0x300                        // IIO Core Error Status
#define R_IIO_IIOERRCTL                               0x304                        // IIO error control
#define R_IIO_IIOFFERRST                              0x308                        // IIO Core Fatal FERR Status
#define R_IIO_IIOFFERRHD0                             0x30C                        // IIO Core Fatal FERR Header
#define R_IIO_IIOFFERRHD1                             0x310
#define R_IIO_IIOFFERRHD2                             0x314
#define R_IIO_IIOFFERRHD3                             0x318
#define R_IIO_IIOFNERRST                              0x31C                        // IIO Core Fatal NERR Status
#define R_IIO_IIONFERRST                              0x320                        // IIO Core Non-Fatal FERR Status
#define R_IIO_IIONFERRHD0                             0x324                        // IIO Core Non-Fatal FERR Header
#define R_IIO_IIONFERRHD1                             0x328
#define R_IIO_IIONFERRHD2                             0x32C
#define R_IIO_IIONFERRHD3                             0x330
#define R_IIO_IIONNERRST                              0x334                        // IIO Core Non-Fatal NERR Status
#define R_IIO_IIOERRCNTSEL                            0x33C                        // IIO Core Error Counter Selection
#define R_IIO_IIOERRCNT                               0x340                        // IIO Core Error Counter

/*++
==================================================================================================
================================== Device 5, Function 4 =========================================
==================================================================================================
--*/


#define CORE05_FUNC4_NUM                          4
#define CORE05F4_REG_ADDR(x) \
    ((UINT64) ((((UINTN) CORE05_DEV_NUM) << 16) + (((UINTN) CORE05_FUNC4_NUM) << 8) + ((UINTN) ((x)))))

//
// IOxAPIC 
//
#define APIC_BUS_NUM                              IIO_BUS_NUM
#define APIC_DEV_NUM                              CORE05_DEV_NUM
#define APIC_FUNC_NUM                             CORE05_FUNC4_NUM


#define APIC_REG_ADDR(x) \
    ((UINT64) ((((UINTN) APIC_DEV_NUM) << 16) + (((UINTN) APIC_FUNC_NUM) << 8) + ((UINTN) ((x)))))

#define R_APIC_MBAR                                    0x10          // IOxAPIC Base Address  
#define R_APIC_ABAR                                    0x40          // I/OxAPIC Alternate BAR
#define   B_APIC_ABAR_ABAR_EN                            BIT15       // ABAR Enable
#define R_APIC_PMCAP                                   0xE0          // Power Management Capabilities
#define R_APIC_PMCSR                                   0xE4          // Power Management Control and Status
#define R_APIC_RDINDEX                                 0x80          // Alternate Index to read Indirect I/OxAPIC Registers
#define R_APIC_RDWINDOW                                0x90          // Alternate Window to read Indirect I/OxAPIC Registers
#define R_APIC_IOAPICTETPC                             0xA0          // IOxAPIC Table Entry Target Programmable Control
#define R_APIC_IOADSELS0                               0x288         // IOxAPIC DSELS Register 0
#define R_APIC_IOADSELS1                               0x28C         // IOxAPIC DSELS Register 1
#define R_APIC_IOINTSRC0                               0x2A0         // IO Interrupt Source Register 0
#define R_APIC_IOINTSRC1                               0x2A4         // IO Interrupt Source Register 1
#define R_APIC_IOREMINTCNT                             0x2A8         // Remote IO Interrupt Count
#define R_APIC_IOREMGPECNT                             0x2AC         // Remote IO GPE Count
#define R_APIC_IOXAPICPARERRINJCTL                     0x2C0         // IOxAPIC Parity Error Injection Control
#define R_APIC_FAUXGV                                  0x2C4         // FauxGV
#define   B_APIC_FAUX_GV_EN                              BIT0        // Faux GV Enable


/*++
==================================================================================================
================================== Device 5, Function 6 =========================================
==================================================================================================
--*/

#define R_IIO_IRP_PMONFILTER                          0x80          // IRP Perfmon Filter Controls
#define R_IIO_IPXPENHCAP                              0x100         // PCI Express Capability
#define R_IIO_CSIDATAPERFSTAT                         0x104         // Performance Monitor Master Status
#define R_IIO_PERFCON                                 0x108         // Performance Monitoring Control
#define   B_IIO_PERFCON_GLO_COUN_EN                     BIT1        // Global Count Enable
#define   B_IIO_PERFCON_GLO_RESET                       BIT0        // Global Reset
#define R_IIO_TSWPMDL0                                0x200         // DP PM Data Low Bits
#define R_IIO_TSWPMDL1                                0x204
#define R_IIO_TSWPMCL0                                0x208         // DP PM Compare Low Bits
#define R_IIO_TSWPMCL1                                0x20C
#define R_IIO_TSWPMDH                                 0x210         // DP PM Data High Bits
#define R_IIO_TSWPMCH                                 0x212         // DP PM Compare High Bits
#define R_IIO_TSWPMR0                                 0x214         // DP PM Compare High Bits
#define   B_IIO_TSWPMR_NOT_GRT_CMP                      BIT30       // NOTGRTCMP
#define   B_IIO_TSWPMR_FRC_ADD0                         BIT29       // FRCPMDADDZ
#define   B_IIO_TSWPMR_LAT_COUN_EN                      BIT28       // LATCENSEL
#define   B_IIO_TSWPMR_SEL_PUL_RESET                    BIT27       // RSTPULSEN
#define   B_IIO_TSWPMR_INV_EVE_SIG                      BIT16       // EVPOLINV
#define   B_IIO_TSWPMR_PM_OUT_PULSE                     BIT5        // PMSSIG
#define   B_IIO_TSWPMR_COUN_RESET                       BIT0        // CNTRST
#define R_IIO_TSWPMR1                                 0x218                         
#define R_IIO_TSWPMEVL0                               0x21C         // DP PM Events Low
#define R_IIO_TSWPMEVL1                               0x220                         
#define R_IIO_TSWPMEVH0                               0x224         // DP PM Events High
#define R_IIO_TSWPMEVH1                               0x228
#define R_IIO_TSWPMER0                                0x22C         // DP PM Resource Events
#define R_IIO_TSWPMER1                                0x230         //
#define R_IIO_VTDPMDL_0                               0x280         // Intel VT-d PM Data Low Bits
#define R_IIO_VTDPMDL_1                               0x284         // Intel VT-d PM Compare Low Bits
#define R_IIO_VTDPMCL_0                               0x288         // Intel VT-d PM Compare Low Bits
#define R_IIO_VTDPMCL_1                               0x28C         // Intel VT-d PM Compare Low Bits
#define R_IIO_VTDPMDH                                 0x290         // Intel VT-d PM Data High Bits
#define R_IIO_VTDPMCH                                 0x292         // Intel VT-d PM Compare High Bits
#define R_IIO_VTDPMR_0                                0x294         // Intel VT-d PM Response Control
#define R_IIO_VTDPMR_1                                0x298         // Intel VT-d PM Response Control
#define   B_IIO_VTDPMR_NOT_GRT_CMP                      BIT30       // Not greater than comparison
#define   B_IIO_VTDPMR_FRC_ADD0                         BIT29       // Force PMD counter to add zero to input
#define   B_IIO_VTDPMR_LAT_COUN_EN                      BIT28       // Latched Count Enable Select
#define   B_IIO_VTDPMR_SEL_PUL_RESET                    BIT27       // Reset Pulse Enable
#define   B_IIO_VTDPMR_INV_EVE_SIG                      BIT16       // Event Polarity Invert
#define   B_IIO_VTDPMR_PM_OUT_PULSE                     BIT5        // PM Status Signal Output
#define   B_IIO_VTDPMR_COUN_RESET                       BIT0        // Counter Reset
#define R_IIO_VTDPMEVL_0                              0x29C         // Intel VT-d PM Events Low
#define R_IIO_VTDPMEVL_1                              0x2A0         // Intel VT-d PM Events Low
#define R_IIO_VTDPMEVH_0                              0x2A4         // Intel VT-d PM Events High
#define R_IIO_VTDPMEVH1                               0x2A8         // Intel VT-d PM Events High
#define R_IIO_VTDPMER0                                0x2AC         // Intel VT-d PM Resource Events
#define R_IIO_VTDPMER1                                0x2B0         // Intel VT-d PM Resource Events


/*++
==================================================================================================
============================ Device 6, Function 0 - 6, Device7 Function 0 - 3 ====================
==================================================================================================
--*/
//D6:F0
#define R_REUTPHCTR                                 0x12C
#define R_DBG2STAT                                  0x4B4
#define R_DBGCLST1                                  0x4BC
#define R_IG_ACQ                                    0x700         
#define R_IG_TRACK                                  0x704
#define R_DFE_GAIN_ACQ                              0x798
#define R_RX_VREF_SEL_NOTGEN1                       0x6EC
#define R_RX_VREF_SEL_GEN1                          0x6E0
#define R_RX_AGC_USE_ERR_DATA                       0x790
#define R_DFE_FORCE_ACQ                             0x794

// D6:F1
#define R_BNDL_SPARE_0                              0xA38
#define R_BNDL_SPARE_1                              0xA3C

// D6:F3
#define R_REUTENGLTRCON                             0x11C
#define R_PORT_CTL                                  0x300
#define R_TXEQREUTPRIV                              0x3F0
#define R_UNIPHYPMCTL                               0x3F4
#define R_TXEQREUTPRIV1                             0x3FC
#define R_CLSPHYCTL1                                0x464
#define R_CLSPHYCTL6                                0x478
#define R_CLSPHYCTL8                                0x480
#define R_PRTPHYCTL                                 0x48C
#define R_PXPSQCNT                                  0x490
#define R_PXPPHYCTL6                                0x4A8
#define R_DBGCLST0                                  0x4B8
#define R_LTSSMFPLAOUT0                             0x5B0
#define R_LTSSMFPLAOUT1                             0x5B4
#define R_LTSSMMATCH0                               0x5B8
#define R_LTSSMMATCH1                               0x5BC
#define R_LTSSMMASK0                                0x5C0
#define R_LTSSMFPLAOUT3                             0x5C4
#define R_LTSSMFPLAMATCH3                           0x5C8
#define R_LTSSMFPLAMASK3                            0x5CC
#define R_LTSSMFPLAOUT4                             0x5D4
#define R_LTSSMFPLAMATCH4                           0x5D8
#define R_LTSSMFPLAMASK4                            0x5DC
#define R_RXSQ_CTRL_0                               0x678
#define R_RXSQ_CTRL_1                               0x67C
#define R_RX_CTLE_PEAK_GEN2                         0x694
#define R_RX_CTLE_PEAK_GEN3                         0x698
#define R_RX_DFE_SUMMER_CTL                         0x6A0
#define R_RX_AFE_OFFSETCOR_GAIN                     0x6AC
#define R_RX_SPARE_0                                0x6B0         
#define R_PG_ACQ_0                                  0x708
#define R_PG_ACQ_1                                  0x70C
#define R_PG_TRACK_0                                0x710         
#define R_PG_TRACK_1                                0x714         
#define R_CDR_PPM_OFFSET_EN                         0x730
#define R_CDR_PPM_OFFSET_0                          0x734
#define R_CDR_PPM_OFFSET_1                          0x738
#define R_CDR_PMOD_STEP_0                           0x748
#define R_CDR_PMOD_STEP_1                           0x74C
#define R_CDR_PMOD_PEAK_0                           0x758
#define R_CDR_PMOD_PEAK_1                           0x75C
#define R_CDR_ACQ_LENGTH                            0x760
#define R_AGC_FORCE_ACQ                             0x774
#define R_AGC_UPDATE_RATE_TRACK                     0x77C
#define R_AGC_ACQ_LENGTH                            0x788
#define R_RX_AGC_USE_ERR_DATA                       0x790
#define R_TX_SPARE_0                                0x808         
#define R_TX_SPARE_1                                0x80C         
#define R_OC_START                                  0x868
#define R_RX_LANE_CAL_RST                           0x87C
#define R_BNDL_SPARE_2                              0xA40
#define R_BNDL_SPARE_3                              0xA44


// D6:F7
#define R_DFX_HVM                                   0x11C
#define R_CCC_BIAS_TX                               0x60C
#define R_CC_ICOMP                                  0x628       
#define R_CC_MISC_CTL                               0x630
#define R_CC_TX_CTL                                 0x638
#define R_CCPWR                                     0x63C

/*++
==================================================================================================
================================== Device 10, Function 0 =========================================
==================================================================================================
--*/

// It is uncore device. Use uncore bus to access
#define R_IIO_MEM_TRML_ESTIMATION_CONFIG              0x4C          // Memory thermal estimation configuration
#define   B_IIO_CON_IMC_DISABLE                         BIT2        // Disable IMC           
#define   B_IIO_CON_BAND_EST_DISABLE                    BIT1        // Disable Bandwidth Estimation
#define   B_IIO_CON_PECI_CONT_DISABLE                   BIT0        // Disable PECI Control
#define R_IIO_MEM_TRML_ESTIMATION_CONFIG2             0x50          // Memory Thermal Estimation Configuration 2
#define R_IIO_MEM_TRML_TEMPERATURE_REPORT             0x60          // thermal status of the memory
#define R_IIO_MEM_ACCUMULATED_BW_CH_0                 0x64          // measurement proportional to the weighted DRAM BW for the channel
#define R_IIO_MEM_ACCUMULATED_BW_CH_1                 0x68                          
#define R_IIO_MEM_ACCUMULATED_BW_CH_2                 0x6C
#define R_IIO_MEM_ACCUMULATED_BW_CH_3                 0x70
#define R_IIO_PRIP_NRG_STTS                           0x7C          // Primary Plane Energy Status
#define R_IIO_PACKAGE_POWER_SKU                       0x84          // Package Power SKU
#define R_IIO_PACKAGE_POWER_SKU_UNIT                  0x8C          // Package Power SKU Unit
#define R_IIO_PACKAGE_ENERGY_STATUS                   0x90          // Package Energy Status
#define R_IIO_PLATFORM_ID                             0xA0          // Platform ID
#define R_IIO_PLATFORM_INFO                           0xA8          // Platform Information
#define R_IIO_PACKAGE_TEMPERATURE                     0xC8          // Package_Temperature
#define R_IIO_PP0_TEMPERATURE                         0xCC          // PP0_temperature
#define R_IIO_PCU_REFERENCE_CLOCK                     0xD4          // count BCLK cycles
#define R_IIO_TEMPERATURE_TARGET                      0xE4          // Temperature Target
#define R_IIO_TURBO_POWER_LIMIT                       0xE8          // Turbo Power Limit
#define   B_IIO_TURBO_PACK_POW_LOCK                     BIT63       // Package Power Limit Lock
#define   B_IIO_TURBO_PACK_POW_LIM2_EN                  BIT47       // Package Power Limit 2 Enable
#define   B_IIO_TURBO_PACK_POW_LIM1_EN                  BIT15       // Package Power Limit 1 Enable
#define R_IIO_PRIP_TURBO_PWR_LIM                      0xF0          // Primary Plane Turbo Power Limitation
#define   B_IIO_PRIP_PRI_PWR_LIM_LOCK                   BIT31       // Primary Plane Power Limit Lock
#define   B_IIO_PRIP_PWR_CLA_LIM_SET                    BIT16       // Power plane clamping limitation over TDP setting
#define   B_IIO_PRIP_PWR_LIM_CTL_EN                     BIT15       // Power Limitation Control Enable
#define R_IIO_PRIMARY_PLANE_CURRENT_CONFIG_CONTROL    0xF8          // Primary Plane Current Configuration Control


/*++
==================================================================================================
================================== Device 10, Function 1 =========================================
==================================================================================================
--*/

// It is uncore device. Use uncore bus to access

#define PCU1_DEV_NUM                           30
#define PCU1_FUNC_NUM                          01
#define R_IIO_BIOS_RESET_CPL                          0x94
#define   B_IIO_RST_CPL0                                BIT0       
#define   B_IIO_RST_CPL1                                BIT1       
#define   B_IIO_RST_CPL2                                BIT2       
#define   B_IIO_RST_CPL3                                BIT3       
#define   B_IIO_RST_CPL4                                BIT4       
#define   B_IIO_RST_CPL0_ACK                            BIT8       
#define   B_IIO_RST_CPL1_ACK                            BIT9       
#define   B_IIO_RST_CPL2_ACK                            BIT10       
#define   B_IIO_RST_CPL3_ACK                            BIT11      
#define   B_IIO_RST_CPL4_ACK                            BIT12      
/*++
==================================================================================================
================================== Device 10, Function 3 =========================================
==================================================================================================
--*/
// It is uncore device. Use uncore bus to access
#define PCU22_DEV_NUM                           30
#define PCU22_FUNC_NUM                          03
#define R_IIO_PCU_DEVHIDE_FUN0                        0x40          // Device hide Register
#define R_IIO_PCU_DEVHIDE_FUN1                        0x44          // Device hide Register
#define R_IIO_PCU_DEVHIDE_FUN2                        0x48          // Device hide Register
#define R_IIO_PCU_DEVHIDE_FUN3                        0x4C          // Device hide Register
#define R_IIO_PCU_DEVHIDE_FUN4                        0x50          // Device hide Register
#define R_IIO_PCU_DEVHIDE_FUN5                        0x54          // Device hide Register
#define R_IIO_PCU_DEVHIDE_FUN6                        0x58          // Device hide Register
#define R_IIO_PCU_DEVHIDE_FUN7                        0x5C          // Device hide Register

#define R_IIO_PCU_CORE_MASK                           0xB0


/*++
==================================================================================================
================================== Device 10, Function 7 =========================================
==================================================================================================
--*/
// It is uncore device. Use uncore bus to access

#define PCU7_DEV_NUM                           30
#define PCU7_FUNC_NUM                          07

#define R_PMC_UART_ENABLE							0x40
#define		B_ENABLE_UART_COM_PORT0					BIT0
#define		B_ENABLE_UART_COM_PORT1					BIT1
#define R_PMC_WAKE_ON_LAN_PULSE_TIMER				0x48
#define		B_PULSE_ENABLE							BIT0
#define		B_TIMER									(BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7)
#define R_PMC_LAN_PORT_SEL							0x4C
#define 	B_ENABLE_LAN_PORT0						BIT0
#define		B_ENABLE_LAN_PORT1						BIT1

/*++
==================================================================================================
================================== Device 11, Function 0 =========================================
==================================================================================================
--*/

// It is uncore device. Use uncore bus to access
#define R_IIO_CPUNODEID                               0x40          // Node ID Configuration Register
#define R_IIO_CPUENABLE                               0x44          // Processor Enable Register
#define R_IIO_INTCONTROL                              0x48          // Interrupt Control Register
#define   B_IIO_INTCTL_IA32_FLAT_CLU_EN                 BIT18       // IA32 Logical Flat or Cluster Mode Override Enable
#define   V_IIO_INTCTL_FLAT_MODE                        00
#define   V_IIO_INTCTL_CLU_MODE                         01
#define   B_IIO_INTCTL_CLU_CHE_SAM_EN                   BIT16       // Cluster Check Sampling Mode
#define   B_IIO_INTCTL_FRC_X2APIC                       BIT1        // Force to X2 APIC Mode
#define   B_IIO_INTCTL_EXT_APIC_EN                      BIT0        // Extended APIC Enable
#define R_IIO_LOCKCONTROL                             0x50          // Lock Control Register
#define   B_IIO_COMPAT_EN                               BIT4        // Compatibility Mode
#define   B_IIO_LOCK_DISABLE                            BIT0        // Lock Disable
#define R_IIO_GIDNIDMAP                               0x54          // Node ID Mapping Register
#define R_IIO_CORECOUNT                               0x60          // Number of Cores
#define R_IIO_UBOXERRSTS                              0x64          // Error Status Register

/*++
==================================================================================================
================================== Device 11, Function 3 =========================================
==================================================================================================
--*/

#define UBOX_MISC_DEV_NUM                           16
#define UBOX_MISC_FUNC_NUM                          07

#define R_IIO_DEVHIDE_FUN0                            0xB0          // Device hide Register
#define R_IIO_DEVHIDE_FUN1                            0xB4          // Device hide Register
#define R_IIO_DEVHIDE_FUN2                            0xB8          // Device hide Register
#define R_IIO_DEVHIDE_FUN3                            0xBC          // Device hide Register
#define R_IIO_DEVHIDE_FUN4                            0xC0          // Device hide Register
#define R_IIO_DEVHIDE_FUN5                            0xC4          // Device hide Register
#define R_IIO_DEVHIDE_FUN6                            0xC8          // Device hide Register
#define R_IIO_DEVHIDE_FUN7                            0xCC          // Device hide Register
#define R_IIO_SMICTRL                                 0xD8          // SMI Control Register



/*++
==================================================================================================
============================ Device 30, Function 3 ===============================================
==================================================================================================
--*/
//D30:F3
#define R_CPU_CAPID1                              0x88              // CAPID1, various PCIe fuses
#define R_CPU_CAPID2                              0x8C              // CAPID2, various PCIe fuses
#define R_CPU_CAPID4                              0x94              // CAPID4, CPU Stepping ID. 

/*++
==================================================================================================
============================ Device 31, Function 2 ===============================================
==================================================================================================
--*/
#define VCU2_DEV_NUM                              31
#define VCU2_FUNC_NUM                             2

/*++
==================================================================================================
================================== Fixed chipset memory map address ==============================
==================================================================================================
--*/

#define ABSEG_CPU_START_ADDR                      0x000A0000
#define ABSEG_CPU_LENGTH                          0x00020000
#define HSEG_CPU_START_ADDR                       0xFEDA0000

/*++
==================================================================================================
================================== PAM register control bits in Uncore============================
==================================================================================================
--*/
// PAM register control bits in Uncore
// It is uncore device. Use uncore bus to access
#define CBO_DEV_NUM                               12
#define CBO_FUNC_NUM                              06
// It is uncore device. Use uncore bus to access
#define ROMLEY_DEV05                     5
#define ROMLEY_FUNC00                    0
#define R_CQ1_CR_PAM0                             0x40
#define R_CQ1_CR_PAM1                             0x41
#define R_CQ1_CR_PAM2                             0x42
#define R_CQ1_CR_PAM3                             0x43
#define R_CQ1_CR_PAM4                             0x44
#define R_CQ1_CR_PAM5                             0x45
#define R_CQ1_CR_PAM6                             0x46
#define   V_CQ1_PAM_LOENABLE_DRAM_DIS     0
#define   V_CQ1_PAM_LOENABLE_DRAM_RD      1
#define   V_CQ1_PAM_LOENABLE_DRAM_WR      2
#define   V_CQ1_PAM_LOENABLE_DRAM_RW      3
#define   V_CQ1_PAM_DMIENABLE_DRAM_DIS    (0<<4)
#define   V_CQ1_PAM_DMIENABLE_DRAM_RD     (1<<4)
#define   V_CQ1_PAM_DMIENABLE_DRAM_WR     (2<<4)
#define   V_CQ1_PAM_DMIENABLE_DRAM_RW     (3<<4)

#define R_UNCORE_CR_SMRAM2                        0x4C    // Upper Byte of Word location 4C -- UNCORE_CR_SMRAM
#define   B_CQ1_SMRAM_G_SMRAME                      BIT3
#define   B_CQ1_SMRAM_D_LOCK                        BIT4
#define   B_CQ1_SMRAM_D_CLOSE                       BIT5
#define   B_CQ1_SMRAM_D_OPEN                        BIT6

#define R_CQ1_CR_PCIEXBAR                         0x50


#define   V_PAM_LOENABLE_DRAM_DIS     0
#define   V_PAM_LOENABLE_DRAM_RD      1
#define   V_PAM_LOENABLE_DRAM_WR      2
#define   V_PAM_LOENABLE_DRAM_RW      3
#define   V_PAM_DMIENABLE_DRAM_DIS    (0<<4)
#define   V_PAM_DMIENABLE_DRAM_RD     (1<<4)
#define   V_PAM_DMIENABLE_DRAM_WR     (2<<4)
#define   V_PAM_DMIENABLE_DRAM_RW     (3<<4)



#define SMRAMC_G_SMRAME_OFFSET  (0x3)
#define SMRAMC_G_SMRAME_WIDTH   (0x1)
#define SMRAMC_G_SMRAME_MASK    (0x8)
#define SMRAMC_G_SMRAME_DEFAULT (0x0)

#define SMRAMC_D_LCK_OFFSET  (0x4)
#define SMRAMC_D_LCK_WIDTH   (0x1)
#define SMRAMC_D_LCK_MASK    (0x10)
#define SMRAMC_D_LCK_DEFAULT (0x0)


/*++
==================================================================================================
================================== Device TBD, Function TBD - TBD             ====================
==================================================================================================
--*/

#ifdef LT_FLAG
#define LT_DPR_SIZE                               0x00300000
#define LT_DPR_SIZE_REG                           0x00030
#define LT_DEV_MEM_SIZE                           0x00200000
#define LT_DEV_MEM_ALIGNMENT                      0x00100000
#define SINIT_MAX_SIZE                            0x00020000
#define LT_HEAP_SIZE                              0x000E0000

#define LT_PUB_BASE                               0xFED30000
#define LT_SPAD_HIGH                              0xFED300A4
#define LT_SPAD_EXIST                             0xFED30010
#define LPB_DPR_BAR                               0x0330

#define PLATFORM_TXT_FIT_FAILSAFE_TYPE            9
#define PLATFORM_TXT_FIT_BIOSACM_TYPE             2
#endif


//
// VT-D Specific Defines
//
#define R_VTD_VERSION                             0x00
#define R_VTD_CAP                                 0x08
#define R_VTD_CAP_LOW                             0x08
#define R_VTD_CAP_HIGH                            0x0C
#define R_VTD_EXT_CAP_LOW                         0x10
#define R_VTD_EXT_CAP_HIGH                        0x14

#define V_VTD_REMAP_ENGINE_SIZE                   0x1000
#define VT_D_CHIPSET_BASE_ADDRESS                 0xFE710000

#define VTD1_BASE_OFFSET                          0x1000
#define R_VTD1_VERSION                            VTD1_BASE_OFFSET  + 0x00
#define R_VTD1_CAP                                VTD1_BASE_OFFSET + 0x08
#define R_VTD1_CAP_LOW                            VTD1_BASE_OFFSET + 0x08
#define R_VTD1_CAP_HIGH                           VTD1_BASE_OFFSET + 0x0C
#define R_VTD1_EXT_CAP_LOW                        VTD1_BASE_OFFSET + 0x10
#define R_VTD1_EXT_CAP_HIGH                       VTD1_BASE_OFFSET + 0x14

//
// ME-HECI defines
//
#define HECI1_DEV_NUM                             0x12
#define HECI1_FUNC_NUM                            0x00
#define HECI2_DEV_NUM                             0x12
#define HECI2_FUNC_NUM                            0x01

#define V_HECI_VID                                V_INTEL_VID
#define V_HECI1_DID_CLIENT                        0x3414    // HECI-1 Client SKU
#define V_HECI2_DID_CLIENT                        0x3415    // HECI-2 Client SKU
#define V_HECI1_DID_SERVER                        0x3434    // HECI-1 Server SKU
#define V_HECI2_DID_SERVER                        0x3435    // HECI-2 Server SKU

#define R_HECIMBAR                                0x10
#define R_FWSTATE                                 0x40
#define   B_CUR_STATE                               (BIT3 | BIT2 | BIT1 | BIT0)
#define     V_CUR_STATE_MEFW_HARDM_DISABLED           0x00
#define     V_CUR_STATE_MEFW_INITIALIZING             0x01
#define     V_CUR_STATE_MEFW_RECOVERY                 0x02
#define     V_CUR_STATE_MEFW_NPTM_ACTIVE              0x03
#define     V_CUR_STATE_MEFW_NPTM_INACTIVE            0x04
#define     V_CUR_STATE_MEFW_SOFTM_DISABLED           0x08
#define     V_CUR_STATE_MEFW_VALIDATION               0x0F

#define   B_ERR_CODE                                (BIT15 | BIT14 | BIT13 | BIT12)
#define     V_ERR_CORE_MEFW_NOERROR                   0x00
#define     V_ERR_CORE_MEFW_UNCATEGORIZED             0x01
#define     V_ERR_CORE_MEFW_DISABLED                  0x02
#define     V_ERR_CORE_MEFW_IMG_FAILURE               0x03

#define   B_EXT_STAT                                ( BIT31 | BIT30 | BIT29 | BIT28 | BIT27 | BIT26 | BIT25 | BIT24 | \
                                                      BIT23 | BIT22 | BIT21 | BIT20 | BIT19 | BIT18 | BIT17 | BIT16)

#define R_HIDM                                    0xA0

//
// ME-HECI MBAR registers
//
#define HECI_DEV                                  22
#define HECI_FUN                                  0
#define R_HECI_HOST_CB_WW                         0x00
#define R_HECI_HOST_H_CSR                         0x04
#define R_HECI_ME_CB_RW                           0x08
#define R_HECI_ME_CSR_HA                          0x0C
#define R_HECI_GEN_STS                            0x4C

//
// LBC commands and other defines
//
#define LBC_READ_COMMAND                            0x1
#define LBC_WRITE_COMMAND                           0x2
#define LBC_LN_SEL_MASK                             (0xFFFF << 11)
#define LOAD_BUS_ADDRESS_0                          0x2f
#define LOAD_BUS_ADDRESS_1                          0x38
#define LOAD_CTLE_SEL_2                             46
#define V_LANE_ERR_CTLE                             BIT8
#define B_CTLEOP_OFFSCAL                            (BIT6 | BIT5 | BIT4 | BIT3 | BIT2)
#define B_CTLEOP_RANGE                              (BIT1 | BIT0)
#define V_CTLEOP_RANGE_R0                           0x0
#define V_CTLEOP_RANGE_R2                           0x2
//
// Support Unions and structures for the purpose of data manipulation - Start
//
typedef struct {
    UINT32  CSIFEQSEL_STRAP:2;                    // 1:0
    UINT32  SMBUSID_STRAP:1;                      // 2
    UINT32  BMCINIT_STRAP:1;                      // 3
    UINT32  CSIBLCSEl_STRAP:1;                    // 4
    UINT32  PESBLCSEL_STRAP:1;                    // 5
    UINT32  DFX1n0_STRAP:2;                       // 7:6
    UINT32  DFX3n2_STRAP:2;                       // 9:8
    UINT32  PCIECFG_STRAP:6;                      // 15:10
    UINT32  DMIMODE_STRAP:1;                      // 16
    UINT32  LEGACYIOH_STRAP:1;                    // 17
    UINT32  TLS_STRAP:1;                          // 18 
    UINT32  ITPM_STRAP:1;                         // 19 
    UINT32  ISAFE_STRAP:1;                        // 20
    UINT32  ISSHIFT_STRAP:1;                      // 21
    UINT32  EARLYSHIFT_STRAP:1;                   // 22
    UINT32  DISPULTSO_STRAP:1;                    // 23
    UINT32  DCSMODE_STRAP:1;                      // 24
    UINT32  DUALIOH_STRAP:1;                      // 25
    UINT32  MP_STRAP:1;                           // 26
    UINT32  Reserved27_STRAP:1;                   // 27
    UINT32  NodeID2NodeID:1;                      // 29
    UINT32  CSINodeID3:1;                         // 28
    UINT32  Reserved31n30_STRAP:2;                // 31:30
  } IIOMISCSS_D20F0_BITS;

typedef union {
    IIOMISCSS_D20F0_BITS      Bits;
    UINT32                    Dword;
} IIOMISCSS_D20F0;

// Support Unions and structures for the purpose of data manipulation - End

#define PCI_EXPRESS_BASE_ADDRESS  ((VOID *) (UINTN) 0x80000000)
#endif
