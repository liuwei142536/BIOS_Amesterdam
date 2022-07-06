/* Date Stamp: 8/23/2014 */

#ifndef QPIAGENT_DFX_PHY_IOVC_h
#define QPIAGENT_DFX_PHY_IOVC_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For HSX_HOST:                                                              */
/* QPIAGENT_DFX_PHY_IOVC_HSX_DEV 8                                            */
/* QPIAGENT_DFX_PHY_IOVC_HSX_FUN 4                                            */
/* For BDX_HOST:                                                              */
/* QPIAGENT_DFX_PHY_IOVC_BDX_DEV 8                                            */
/* QPIAGENT_DFX_PHY_IOVC_BDX_FUN 4                                            */

/* IOVC_BIAS0_QPIAGENT_DFX_PHY_IOVC_REG supported on:                         */
/*       HSX (0x00000001)                                                     */
/*       BDX (0x00000001)                                                     */
/* Register default value:              0x25                                  */
#define IOVC_BIAS0_QPIAGENT_DFX_PHY_IOVC_REG 0x00000001

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_1.xml.
 * IOVC BIAS control 0 Register
 */
typedef union {
  struct {
    UINT8 fwdctx_vref_sel : 5;
    /* fwdctx_vref_sel - Bits[4:0], RWS, default = 5'h5 
        
       	Comparator reference voltage select for QPI fwdc
       	This bit alone controls comparator reference voltage
             
     */
    UINT8 fwdc_padcomp_vrefsel : 3;
    /* fwdc_padcomp_vrefsel - Bits[7:5], RWS, default = 3'h1 
        
       	Voltage reference select for Pad comparator
       	111= 500mV
       	110= 450mV
       	101= 400mV
       	100= 350mV
       	011= 625mV
       	010= 250mV
       	001= 225mV (QPI default)
       	000= 200mV
             
     */
  } Bits;
  UINT8 Data;
} IOVC_BIAS0_QPIAGENT_DFX_PHY_IOVC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* IOVC_BIAS1_QPIAGENT_DFX_PHY_IOVC_REG supported on:                         */
/*       HSX (0x00000002)                                                     */
/*       BDX (0x00000002)                                                     */
/* Register default value:              0x18                                  */
#define IOVC_BIAS1_QPIAGENT_DFX_PHY_IOVC_REG 0x00000002

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_1.xml.
 * IOVC BIAS control 1 Register
 */
typedef union {
  struct {
    UINT8 fwdc_tx_iref_ctl : 3;
    /* fwdc_tx_iref_ctl - Bits[2:0], RWS, default = 3'h0 
        
       	Input iref adjustment. Per bundle.
       	011= 1.22* QPI iref (match bndl_vref_sel= 275mV)
       	010= 1.11* QPI iref (match bndl_vref_sel= 250mV)
       	001= 1.00* QPI iref (match bndl_vref_sel= 225mV)
       	000= 0.89* QPI iref (match bndl_vref_sel= 200mV)
       	Together with vref_ctl, this bit is used for swing control
             
     */
    UINT8 fwdc_iqsel_ovr : 1;
    /* fwdc_iqsel_ovr - Bits[3:3], RWS, default = 1'b1 
        
       	iqsel Override
             
     */
    UINT8 fwdc_rx_bias_scale : 4;
    /* fwdc_rx_bias_scale - Bits[7:4], RWS, default = 4'h1 
        
       	Bias scaling, decoding
       	00 = Iref is set to 78uA
       	01 = Iref is set to 100uA
       	10 = Iref is set to 128uA
       	11 = Iref is set to 152uA
             
     */
  } Bits;
  UINT8 Data;
} IOVC_BIAS1_QPIAGENT_DFX_PHY_IOVC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */










/* IOVC_SPARE0_QPIAGENT_DFX_PHY_IOVC_REG supported on:                        */
/*       HSX (0x00000007)                                                     */
/*       BDX (0x00000007)                                                     */
/* Register default value:              0x00                                  */
#define IOVC_SPARE0_QPIAGENT_DFX_PHY_IOVC_REG 0x00000007

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_1.xml.
 * IOVC Spare 0 1 Register
 */
typedef union {
  struct {
    UINT8 spare0 : 8;
    /* spare0 - Bits[7:0], RWS, default = 8'b0 
        
       	spare 0 
             
     */
  } Bits;
  UINT8 Data;
} IOVC_SPARE0_QPIAGENT_DFX_PHY_IOVC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






/* IOVC_DLL_CTL0_QPIAGENT_DFX_PHY_IOVC_REG supported on:                      */
/*       HSX (0x0000000B)                                                     */
/*       BDX (0x0000000B)                                                     */
/* Register default value:              0x00                                  */
#define IOVC_DLL_CTL0_QPIAGENT_DFX_PHY_IOVC_REG 0x0000000B

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_1.xml.
 * IOVC DLL Control 0 Register
 */
typedef union {
  struct {
    UINT8 fwdc_dllctl : 8;
    /* fwdc_dllctl - Bits[7:0], RWS, default = 8'b0 
        
       	From LSB to MSB:
       	[2:0] DLL/DCC lpf resistor control (3 bits)
       	[3] Not used
       	[4] Increase DLL timer by 2X
       	[5] Increase DLL calibration cycle by 2X
       	[7:6] Not used
             
     */
  } Bits;
  UINT8 Data;
} IOVC_DLL_CTL0_QPIAGENT_DFX_PHY_IOVC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* IOVC_DLL_CTL2_QPIAGENT_DFX_PHY_IOVC_REG supported on:                      */
/*       HSX (0x0000000D)                                                     */
/*       BDX (0x0000000D)                                                     */
/* Register default value:              0x39                                  */
#define IOVC_DLL_CTL2_QPIAGENT_DFX_PHY_IOVC_REG 0x0000000D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_1.xml.
 * IOVC DLL Control 2 Register
 */
typedef union {
  struct {
    UINT8 fwdc_dllvsetctl : 4;
    /* fwdc_dllvsetctl - Bits[3:0], RWS, default = 4'h9 
        
       	DLL startup voltage control. Also used in DLL open loop mode
       	0000 - Min voltage (~ 300 mV)
       	1111 - Max voltage (~ 750 mV)
       	Step size ~ 30 mV
             
     */
    UINT8 fwdc_dllcpctl : 4;
    /* fwdc_dllcpctl - Bits[7:4], RWS, default = 4'h3 
        
       	used to control the DLL bandwidth by controlling the charge pump current
             
     */
  } Bits;
  UINT8 Data;
} IOVC_DLL_CTL2_QPIAGENT_DFX_PHY_IOVC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






/* IOVC_LCPKAMP_CFG1_QPIAGENT_DFX_PHY_IOVC_REG supported on:                  */
/*       HSX (0x00000010)                                                     */
/*       BDX (0x00000010)                                                     */
/* Register default value:              0x28                                  */
#define IOVC_LCPKAMP_CFG1_QPIAGENT_DFX_PHY_IOVC_REG 0x00000010

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_1.xml.
 * IOVC LCPKAMP Cfg 1 Register
 */
typedef union {
  struct {
    UINT8 fwdc_lcampcapctl : 5;
    /* fwdc_lcampcapctl - Bits[4:0], RWS, default = 5'h8 
        
       	LC peak amplifier capacitor load control signals.
       	8 Gbps: 0x8 (default)
       	9.6 Gbps: 0x2
       	6.4 Gbps: 0x1F
             
     */
    UINT8 fwdc_lcampen : 1;
    /* fwdc_lcampen - Bits[5:5], RWS, default = 1'b1 
        
        Enable signal for LC peak amplifier. When this path is enabled, the other 
       parallel 
       	forwarded clock path is disabled
       	0 = LC peak amplifier is disabled
       	1 = LC peak amplifier is enabled
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVC_LCPKAMP_CFG1_QPIAGENT_DFX_PHY_IOVC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


















/* IOVC_TX_CTL2_QPIAGENT_DFX_PHY_IOVC_REG supported on:                       */
/*       HSX (0x00000019)                                                     */
/*       BDX (0x00000019)                                                     */
/* Register default value:              0x00                                  */
#define IOVC_TX_CTL2_QPIAGENT_DFX_PHY_IOVC_REG 0x00000019

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_1.xml.
 * IOVC TX Control 2 Register
 */
typedef union {
  struct {
    UINT8 fwdc_tx_rxlpclk_sel : 2;
    /* fwdc_tx_rxlpclk_sel - Bits[1:0], RWS, default = 2'b0 
        
       	Clock selection for RX to TX loopback
       	00= TX clock is selected (normal operation, default)
       	01= reserved
       	10= rx0lpclk buffered output is selected
       	11= rx1lpclk buffered output is selected
             
     */
    UINT8 fwdc_tx_spare : 4;
    /* fwdc_tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	fwdc_tx_spare[3:1]: Reserved
       	fwdc_tx_spare[0]: Icomp tracking mode:
        0 = (default) open loop mode, open icomp loop after acquisition. An offset 
       (specified by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp gain is 
       determined by bndl_icomp_gain 
             
     */
    UINT8 fwdc_txdcc_fast_mode_only : 1;
    /* fwdc_txdcc_fast_mode_only - Bits[6:6], RWS, default = 1'b0 
        
       	Tx DCC Fast mode only
             
     */
    UINT8 fwdc_drvsw_ctl_ovr : 1;
    /* fwdc_drvsw_ctl_ovr - Bits[7:7], RWS, default = 1'b0 
        
       	When set to 1, This bit enable fwdc_tx_drsw_ctl to override the agent control.
             
     */
  } Bits;
  UINT8 Data;
} IOVC_TX_CTL2_QPIAGENT_DFX_PHY_IOVC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* IOVC_IREF_OVR_QPIAGENT_DFX_PHY_IOVC_REG supported on:                      */
/*       HSX (0x0000001B)                                                     */
/*       BDX (0x0000001B)                                                     */
/* Register default value:              0x00                                  */
#define IOVC_IREF_OVR_QPIAGENT_DFX_PHY_IOVC_REG 0x0000001B

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_1.xml.
 * IOVC_IREF Override Register
 */
typedef union {
  struct {
    UINT8 ism_iref_ovr_en : 1;
    /* ism_iref_ovr_en - Bits[0:0], RWS, default = 1'b0 
        
       	When set, it enables the ISM to override the iref value with ism_iref
             
     */
    UINT8 ism_iref : 3;
    /* ism_iref - Bits[3:1], RWS, default = 3'b0 
        
       	ISM iref override value
             
     */
    UINT8 rsvd : 4;
    /* rsvd - Bits[7:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVC_IREF_OVR_QPIAGENT_DFX_PHY_IOVC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




















/* IOVC_DLL_CALSM_CTL0_QPIAGENT_DFX_PHY_IOVC_REG supported on:                */
/*       HSX (0x00000025)                                                     */
/*       BDX (0x00000025)                                                     */
/* Register default value:              0x0D                                  */
#define IOVC_DLL_CALSM_CTL0_QPIAGENT_DFX_PHY_IOVC_REG 0x00000025

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_1.xml.
 * IOVC DLL CALSM Control 0 Register
 */
typedef union {
  struct {
    UINT8 wsel : 3;
    /* wsel - Bits[2:0], RWS, default = 3'b101 
        
       	WSEL control
             
     */
    UINT8 votecnt : 2;
    /* votecnt - Bits[4:3], RWS, default = 2'b1 
        
       	VOTECNT control
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVC_DLL_CALSM_CTL0_QPIAGENT_DFX_PHY_IOVC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* IOVC_TXDCC_CALSM_CTL0_QPIAGENT_DFX_PHY_IOVC_REG supported on:              */
/*       HSX (0x00000026)                                                     */
/*       BDX (0x00000026)                                                     */
/* Register default value:              0x0E                                  */
#define IOVC_TXDCC_CALSM_CTL0_QPIAGENT_DFX_PHY_IOVC_REG 0x00000026

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_1.xml.
 * IOVC TxDCC CALSM Control 0 Register
 */
typedef union {
  struct {
    UINT8 wsel : 3;
    /* wsel - Bits[2:0], RWS, default = 3'b110 
        
       	WSEL control
             
     */
    UINT8 votecnt : 2;
    /* votecnt - Bits[4:3], RWS, default = 2'b1 
        
       	VOTECNT control
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVC_TXDCC_CALSM_CTL0_QPIAGENT_DFX_PHY_IOVC_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


#endif /* QPIAGENT_DFX_PHY_IOVC_h */
