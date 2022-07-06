/* Date Stamp: 8/23/2014 */

#ifndef QPIAGENT_DFX_PHY_IOVB_h
#define QPIAGENT_DFX_PHY_IOVB_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For HSX_HOST:                                                              */
/* QPIAGENT_DFX_PHY_IOVB_HSX_DEV 8                                            */
/* QPIAGENT_DFX_PHY_IOVB_HSX_FUN 4                                            */
/* For BDX_HOST:                                                              */
/* QPIAGENT_DFX_PHY_IOVB_BDX_DEV 8                                            */
/* QPIAGENT_DFX_PHY_IOVB_BDX_FUN 4                                            */

/* IOVB_RX_CTL0_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x00000001)                                                     */
/*       BDX (0x00000001)                                                     */
/* Register default value:              0x00                                  */
#define IOVB_RX_CTL0_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000001

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX Control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_rtermdis : 1;
    /* rx_rtermdis - Bits[0:0], RWS, default = 1'b0 
        
       	Receive Termination Resistor Disable.
       	1 = Receive termination resistor disabled - like it is totally disconnected
       	0 = Enabled
       	Only when override is enabled will this affect AFE 
             
     */
    UINT8 rx_rtermdis_ovr : 1;
    /* rx_rtermdis_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for rx_rtermdis
             
     */
    UINT8 rx_rtermpullhighn : 2;
    /* rx_rtermpullhighn - Bits[3:2], RWS, default = 2'b0 
        
       	00=RX pull-high resistor disconnected (floating)
       	01=RX pull-high resistor tied to vss (pull low)
       	10=RX pull-high resistor tied to vccppa (pull high)
       	11=RX pull-high resistor tied to vcm (common mode)
       	Definition for NTL/RTERM leg check: controls pullup/pulldown on pad RXP/RXN
       	during NTL and rterm_legcheck modes.
       	See UniPHY MAS Table 8
             
     */
    UINT8 rx_rtermpullhighp : 2;
    /* rx_rtermpullhighp - Bits[5:4], RWS, default = 2'b0 
        
       	00= RX pull-high resistor disconnected (floating)
       	01= RX pull-high resistor tied to vss (pull low)
       	10= RX pull-high resistor tied to vccppa (pull high)
       	11= RX pull-high resistor tied to vcm (common mode)
       	Definition for NTL/RTERM leg check:
       	[1] select rxafe_sqdet during rterm_legcheck/NTL tests.
       	[0] places pad in high Z mode during NTL test, not used in rterm_legcheck.
       	See UniPHY MAS Table 8
             
     */
    UINT8 rx_dfe_summer_ctl : 2;
    /* rx_dfe_summer_ctl - Bits[7:6], RWS, default = 2'b0 
        
        DFE summer control for high-gain mode. Select how many clocks until changing to 
       low-gain. 
       	11 = 255 clocks
       	10 = 128 clocks
       	01 = 64 clocks
       	00 = 32 clocks
             
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTL0_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* IOVB_RX_CTL1_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x00000002)                                                     */
/*       BDX (0x00000002)                                                     */
/* Register default value:              0x05                                  */
#define IOVB_RX_CTL1_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000002

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX Control 1 Register
 */
typedef union {
  struct {
    UINT8 rxsq_en : 1;
    /* rxsq_en - Bits[0:0], RWS, default = 1'b1 
        
       	RX Squelch Exit Detect Enable.
       	1 = AFE squelch exit detect circuit enabled.
       	0 = disabled.
        This input is ignored if internal signal: sqdet_offscal_en = 1 or SQ_ONEDETECT 
       = 1.  
       	In both cases, squelch detector is forced on.
             
     */
    UINT8 rxsq_ctrl : 3;
    /* rxsq_ctrl - Bits[3:1], RWS, default = 3'h2 
        
        select squelch detect / acjtag / ntl / rxterm_leg_check comparator reference 
       level 
       	Note: Set rxsd_lvl[2:0] =000 during squelch detect offset calibration.
       	Reference UniPHY tables:
       	Table 2 : squelch detect comparator reference select
       	Table 3 : boundary scan/ACJTAG comparators reference select
       	Table 4 : NTL comparators reference select
       	Table 5 : rterm_legcheck comparators reference select
             
     */
    UINT8 rx_spare : 4;
    /* rx_spare - Bits[7:4], RWS, default = 4'b0 
        
       	Bit0 = 1 = CDR, AGC, DFE loops forced enable. 0 = oc_finished to enable the
       	loops (might take this functionality out...)
             
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTL1_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* IOVB_RX_CTLE0_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x00000004)                                                     */
/*       BDX (0x00000004)                                                     */
/* Register default value:              0x0F                                  */
#define IOVB_RX_CTLE0_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000004

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_ctle_offset_en : 1;
    /* rx_ctle_offset_en - Bits[0:0], RWS, default = 1'b1 
        
       	1 = Enable Receive CTLE offset correction
       	0 = disable
       	Note: this is needed on a lane basis for HVM testing
             
     */
    UINT8 rx_ctle_peak : 5;
    /* rx_ctle_peak - Bits[5:1], RWS, default = 5'h7 
        
        Full speed encoded number to degeneration capacitance atoms to turn on in the 
       first stage of the CTLE 
       	00000 = Disable equalization, boost DC gain.
             
     */
    UINT8 rx_ctle_vcm_ovrd_dis : 1;
    /* rx_ctle_vcm_ovrd_dis - Bits[6:6], RWS, default = 1'b0 
        
       	1 = Disable CTLE vcm override
       	0= enable CTLE internal common mode
       	This is per bundle control.
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE0_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* IOVB_RX_VREF0_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x00000005)                                                     */
/*       BDX (0x00000005)                                                     */
/* Register default value on HSX:       0x18                                  */
/* Register default value on BDX:       0x98                                  */
#define IOVB_RX_VREF0_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000005
#ifdef HSX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file HSX\1.8.4.WINDOW_0.xml.
 * IOVB RX VREF Control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_vref_sel : 4;
    /* rx_vref_sel - Bits[3:0], RWS, default = 4'h8 
        
        Select reference voltage level for error samplers from 0 to 150mV with step 
       10mV. 
       	Default 1000 = 80mV for non PCIe-Gen1 modes (incl. QPI).
       	Default 1111 = 150mV for PCIe Gen1 mode.
             
     */
    UINT8 rx_vcm_sum_sel : 1;
    /* rx_vcm_sum_sel - Bits[4:4], RWS, default = 1'b1 
        
       	select input CM level of RX summer for offset calibration:
       	0=250mV
       	1=300mV
       	Note: This is a bundle level signal
             
     */
    UINT8 rx_vcm_ds_sel : 2;
    /* rx_vcm_ds_sel - Bits[6:5], RWS, default = 2'b0 
        
       	select input CM level of RX data sampler for offset calibration:
       	00=250mV
       	01=300mV
       	10/11=reserved
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_VREF0_0_0_QPIAGENT_DFX_PHY_IOVB_HSX_STRUCT;
#endif /* ASM_INC */
#endif /* HSX_HOST */

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX VREF Control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_vref_sel : 4;
    /* rx_vref_sel - Bits[3:0], RWS, default = 4'h8 
        
        Select reference voltage level for error samplers from 0 to 150mV with step 
       10mV. 
       	Default 1000 = 80mV for non PCIe-Gen1 modes (incl. QPI).
       	Default 1111 = 150mV for PCIe Gen1 mode.
             
     */
    UINT8 rx_vcm_sum_sel : 1;
    /* rx_vcm_sum_sel - Bits[4:4], RWS, default = 1'b1 
        
       	select input CM level of RX summer for offset calibration:
       	0=250mV
       	1=300mV
       	Note: This is a bundle level signal
             
     */
    UINT8 rx_vcm_ds_sel : 2;
    /* rx_vcm_ds_sel - Bits[6:5], RWS, default = 2'b0 
        
       	select input CM level of RX data sampler for offset calibration:
       	00=250mV
       	01=300mV
       	10/11=reserved
       	Note: This is a bundle level signal
             
     */
    UINT8 rx_vcm_sum_sel_extnd : 1;
    /* rx_vcm_sum_sel_extnd - Bits[7:7], RWS, default = 1'b1 
        
       	select input CM level of RX summer for offset calibration:
       	0=250mV
       	1=300mV
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_VREF0_0_0_QPIAGENT_DFX_PHY_IOVB_BDX_STRUCT;
#endif /* ASM_INC */
#endif /* BDX_HOST */



/* IOVB_SPARE0_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                    */
/*       HSX (0x00000006)                                                     */
/*       BDX (0x00000006)                                                     */
/* Register default value:              0x00                                  */
#define IOVB_SPARE0_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000006

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB spare 0 Register
 */
typedef union {
  struct {
    UINT8 spare0 : 8;
    /* spare0 - Bits[7:0], RWS, default = 8'h0 
        
       	Bundle spare0
             
     */
  } Bits;
  UINT8 Data;
} IOVB_SPARE0_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* IOVB_IG0_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                       */
/*       HSX (0x00000007)                                                     */
/*       BDX (0x00000007)                                                     */
/* Register default value:              0x14                                  */
#define IOVB_IG0_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000007

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Inegral Gain Control 0 Register
 */
typedef union {
  struct {
    UINT8 ig_acq : 3;
    /* ig_acq - Bits[2:0], RWS, default = 3'h4 
        
       	Integral Gain Control Select for acquisition Mode
       	111 = reserved
       	110 = reserved
       	101 = 12/4096
       	100 = 10/4096
       	011 = 8/4096
       	010 = 4/4096
       	001 = 2/4096
       	000 = 0/4096
       	Note: This is a bundle level signal
             
     */
    UINT8 ig_track : 3;
    /* ig_track - Bits[5:3], RWS, default = 3'h2 
        
        Integral Gain Control Select for Tracking Mode. Definition is same as 
       acquisition 
       	mode. By having these inputs the same as the ACQ inputs, you can force the CDR
       	to stay in acquisition mode.
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_IG0_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* IOVB_PG0_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                       */
/*       HSX (0x00000008)                                                     */
/*       BDX (0x00000008)                                                     */
/* Register default value:              0x10                                  */
#define IOVB_PG0_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000008

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Proportional Gain Control 0 Register
 */
typedef union {
  struct {
    UINT8 pg_acq : 6;
    /* pg_acq - Bits[5:0], RWS, default = 6'h10 
        
       	Proportional Gain Control Select for acquisition Mode
       	11_1111 = 63/32
       	11_1110 = 62/32
       	10_0000 = 32/32
       	01_0000 = 16/32
       	00_0001 = 1/32
       	00_0000 = 0/32
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_PG0_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* IOVB_PG1_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                       */
/*       HSX (0x00000009)                                                     */
/*       BDX (0x00000009)                                                     */
/* Register default value:              0x10                                  */
#define IOVB_PG1_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000009

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Proportional Gain Control 1 Register
 */
typedef union {
  struct {
    UINT8 pg_track : 6;
    /* pg_track - Bits[5:0], RWS, default = 6'h10 
        
       	Proportional Gain Control Select for Tracking Mode. Definition is same as
       	acquisition mode.
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_PG1_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */








/* IOVB_CDR3_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                      */
/*       HSX (0x0000000D)                                                     */
/*       BDX (0x0000000D)                                                     */
/* Register default value:              0x40                                  */
#define IOVB_CDR3_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x0000000D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB CDR Control 3 Register
 */
typedef union {
  struct {
    UINT8 cdr_pmod_peak : 6;
    /* cdr_pmod_peak - Bits[5:0], RWS, default = 6'b0 
        
       	Phase Modulation depth
       	0 - 63
       	Note: This is a bundle level signal
             
     */
    UINT8 cdr_acq_length : 2;
    /* cdr_acq_length - Bits[7:6], RWS, default = 2'h1 
        
        Length of time is measured from squelch exit detect active. If squelch exit 
       detect 
       	is disabled, then time is measured from PLL lock (?)
       	00 = 32 (x8 UI)
       	01 = 128 (x8 UI) -- default value
       	10 = 1024 (x8 UI)
       	11 = 4096 (x8 UI)
             
     */
  } Bits;
  UINT8 Data;
} IOVB_CDR3_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* IOVB_AGC0_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                      */
/*       HSX (0x0000000F)                                                     */
/*       BDX (0x0000000F)                                                     */
/* Register default value:              0x00                                  */
#define IOVB_AGC0_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x0000000F

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB AGC Control 0 Register
 */
typedef union {
  struct {
    UINT8 agc_open_loop : 1;
    /* agc_open_loop - Bits[0:0], RWS, default = 1'b0 
        
       	1 = Ignore the AGC error information
       	0 = AGC error information is used to control the gain
             
     */
    UINT8 agc_open_loop_ovr : 1;
    /* agc_open_loop_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	Override enable for agc_open_loop
             
     */
    UINT8 agc_force_acq : 1;
    /* agc_force_acq - Bits[2:2], RWS, default = 1'b0 
        
       	1 = Force the AGC State Machine into training mode
       	0 = The AGC state machine controls the mode
             
     */
    UINT8 agc_update_rate_acq : 2;
    /* agc_update_rate_acq - Bits[4:3], RWS, default = 2'b0 
        
       	00 = 32 cycles
       	01 = 64
       	10 = 128
       	11 = 256
       	Note: This is a bundle level signal
             
     */
    UINT8 agc_update_rate_track : 2;
    /* agc_update_rate_track - Bits[6:5], RWS, default = 2'b0 
        
       	00 = 32 cycles
       	01 = 64
       	10 = 128
       	11 = 256
       	Note: This is a bundle level signal
             
     */
    UINT8 rx_agc_use_err_data : 1;
    /* rx_agc_use_err_data - Bits[7:7], RWS, default = 1'b0 
        
       	AGC update control
       	1 = use error and data info
       	0 = use error info only
             
     */
  } Bits;
  UINT8 Data;
} IOVB_AGC0_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* IOVB_AGC1_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                      */
/*       HSX (0x00000010)                                                     */
/*       BDX (0x00000010)                                                     */
/* Register default value:              0x13                                  */
#define IOVB_AGC1_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000010

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB AGC Control 1 Register
 */
typedef union {
  struct {
    UINT8 agc_gain_acq : 2;
    /* agc_gain_acq - Bits[1:0], RWS, default = 2'h3 
        
       	00 = +/- 1 (i.e. 0.01563 dB)
       	01 = +/- 2 (i.e. 0.03125 dB)
       	10 = +/- 32 (i.e. 0.5 dB)
       	11 = +/- 64 (i.e. 1.0 dB)
       	Note: This is a bundle level signal
             
     */
    UINT8 agc_gain_track : 2;
    /* agc_gain_track - Bits[3:2], RWS, default = 2'b0 
        
       	00 = +/- 1 (i.e. 0.01563 dB)
       	01 = +/- 2 (i.e. 0.03125 dB)
       	10 = +/- 32 (i.e. 0.5 dB)
       	11 = +/- 64 (i.e. 1.0 dB)
       	Note: This is a bundle level signal
             
     */
    UINT8 agc_acq_length : 2;
    /* agc_acq_length - Bits[5:4], RWS, default = 2'h1 
        
        Length of time measured from squelch exit detect active. If squelch exit detect 
       is 
       	disabled, then time is measured from PLL lock (?).
       	00 = 32 (x32UI)
       	01 = 64 (x32UI)  default value
       	10 = 256 (x32UI)
       	11 = 1024 (x32UI)
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_AGC1_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






/* IOVB_DFE2_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                      */
/*       HSX (0x00000013)                                                     */
/*       BDX (0x00000013)                                                     */
/* Register default value:              0x17                                  */
#define IOVB_DFE2_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000013

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB DFE Control 2 Register
 */
typedef union {
  struct {
    UINT8 rx_dfesum_mfc_10gen : 2;
    /* rx_dfesum_mfc_10gen - Bits[1:0], RWS, default = 2'h3 
        
       	Dfesum_mfc_10gen: control the mfc in 10G mode.
       	00: disconnect mfc from summer output
       	01/10: connect one mfc to summer output
       	11: connect two mfc to summer output.
             
     */
    UINT8 rx_dfelsb_sel : 2;
    /* rx_dfelsb_sel - Bits[3:2], RWS, default = 2'h1 
        
       	Select DFE I-DAC LSB current value:
       	00=2uA,
       	01=4uA,
       	10=6uA,
       	11=8uA
             
     */
    UINT8 bkch_threshold : 2;
    /* bkch_threshold - Bits[5:4], RWS, default = 2'h1 
        
       	bkch threshold
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_DFE2_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* IOVB_TX_CTL0_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x00000014)                                                     */
/*       BDX (0x00000014)                                                     */
/* Register default value:              0x01                                  */
#define IOVB_TX_CTL0_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000014

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX Control 0 Register
 */
typedef union {
  struct {
    UINT8 tx_align_en : 1;
    /* tx_align_en - Bits[0:0], RWS, default = 1'b1 
        
       	Enable TX data alignment (TX FIFO enable?)
             
     */
    UINT8 tx_align_en_ovr : 1;
    /* tx_align_en_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for tx_align_en
             
     */
    UINT8 tx_spare : 4;
    /* tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	tx_spare[3:1]: Reserved
       	tx_spare[0]: Icomp tracking mode:
        0= (default) open loop mode, open icomp loop after acquisition. An (specified 
       by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp determined 
       by bndl_icomp_gain 
             
     */
    UINT8 tx_lcl_icomp_dis : 1;
    /* tx_lcl_icomp_dis - Bits[6:6], RWS, default = 1'b0 
        
       	Local icomp en.
       	0 =start the local TX block icomp loop. Should be enabled for normal operation.
        1 =stop the local TX block icomp loop. For debug or HVM mode to check the RTerm 
       legs. 
        Note: This signal is overidden by the power-up sequence logic in the common 
       block (comdig) and will not affect the icomp loop until the icomp calibration 
       has completed. Need the local loop to accurately measure the legs. 
             
     */
    UINT8 tx_sense_lane : 1;
    /* tx_sense_lane - Bits[7:7], RWS, default = 1'b0 
        
       	Lane 0 identifier for QPI
       	0= this bundle does not contain QPI lane 0
       	1= this bundle contains QPI lane 0
        When bit= 1, the lane will not enter L1 state, but the lane can still be in L0s 
       and perform Rx Detect 
        (NOTE: This behavior is not entirely correct for QPI, so the QPI agent will not 
       use it but instead will simply not assert L0s on its lane 0.) 
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_CTL0_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* IOVB_TXEQ_LVL0_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                 */
/*       HSX (0x00000016)                                                     */
/*       BDX (0x00000016)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_LVL0_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000016

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_lvl0 : 6;
    /* txeq_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_LVL0_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* IOVB_TXEQ_LVL1_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                 */
/*       HSX (0x00000017)                                                     */
/*       BDX (0x00000017)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_LVL1_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000017

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Level1 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_lvl1 : 6;
    /* txeq_lvl1 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Level1 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_LVL1_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* IOVB_TXEQ_LVL2_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                 */
/*       HSX (0x00000018)                                                     */
/*       BDX (0x00000018)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_LVL2_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000018

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Level2 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_lvl2 : 6;
    /* txeq_lvl2 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Level2 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_LVL2_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* IOVB_TXEQ_LVL3_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                 */
/*       HSX (0x00000019)                                                     */
/*       BDX (0x00000019)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_LVL3_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000019

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Level3 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_lvl3 : 6;
    /* txeq_lvl3 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Level3 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_LVL3_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






/* IOVB_AFE_CTL0_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x0000001C)                                                     */
/*       BDX (0x0000001C)                                                     */
/* Register default value:              0x40                                  */
#define IOVB_AFE_CTL0_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x0000001C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB AFE Control 0 Register
 */
typedef union {
  struct {
    UINT8 oc_start : 1;
    /* oc_start - Bits[0:0], RWS, default = 1'b0 
        
        Offset Correction Start. Needs to be driven active high at reset and also when 
       the slow clock mode is exited for the QPI AFE. 0 = Don't start offset 
       correction. 1 = Start (re-start) offset correction.. The rising edge will start 
       (re-start) offset correction in PCIe mode. In QPI mode also need forward clock 
       detect to be active. Note: This is a handshake signal which needs a minimum 
       pulse width of four 100 MHz REFCLK periods for the edges to be properly 
       detected. 
             
     */
    UINT8 oc_start_ovr : 1;
    /* oc_start_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	oc_start override enable
             
     */
    UINT8 dfx_bdl_sel : 1;
    /* dfx_bdl_sel - Bits[2:2], RWS, default = 1'b0 
        
       	1 = Select/enable the DFXP_data, Trace_data, and Dmon_data for the specified
       	0 = don't select the bundle
       	Note: Only one bundle can be selected at a time.
       	Note: This is a bundle level signal
             
     */
    UINT8 eyemon_trig : 1;
    /* eyemon_trig - Bits[3:3], RWS_V, default = 1'b0 
        
        This signal initiates the start of the eyemon capture cycle. It should stay 
       asserted 
        hi until the signal eyemon_trig_clr is asserted hi. The clearing of this signal 
       will in 
       	turn clear the eyemon_trig_clr output.
       	Note: This is a bundle level signal
             
     */
    UINT8 nearend_lpbk_en : 1;
    /* nearend_lpbk_en - Bits[4:4], RWS, default = 1'b0 
        
        1 = Loop back Agent data. The TX input from the agent is driven back to the 
       Agent on the RX outputs. Where is this specifically done?? 
       	0 = no loop back 
             
     */
    UINT8 farend_lpbk_en : 1;
    /* farend_lpbk_en - Bits[5:5], RWS, default = 1'b0 
        
       	1 = Loop back receive data input on the bumps (package pins) back to the
       	transmit output bumps (package pins)
       	0 = no loop back
             
     */
    UINT8 psa_cfg : 2;
    /* psa_cfg - Bits[7:6], RWS, default = 2'b1 
        
       	PSA cfg
             
     */
  } Bits;
  UINT8 Data;
} IOVB_AFE_CTL0_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */








/* IOVB_IREF_OVR_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x00000020)                                                     */
/*       BDX (0x00000020)                                                     */
/* Register default value:              0x00                                  */
#define IOVB_IREF_OVR_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000020

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB_IREF Override Register
 */
typedef union {
  struct {
    UINT8 ism_iref_ovr_en : 1;
    /* ism_iref_ovr_en - Bits[0:0], RWS, default = 1'b0 
        
       	When set, it enables the ISM to override the iref value with ism_iref
             
     */
    UINT8 ism_iref : 5;
    /* ism_iref - Bits[5:1], RWS, default = 5'b0 
        
       	ISM iref override value
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_IREF_OVR_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */














/* IOVB_TXEQ_HALF_LVL0_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x00000027)                                                     */
/*       BDX (0x00000027)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL0_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000027

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl0 : 6;
    /* txeq_half_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL0_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* IOVB_TXEQ_HALF_LVL1_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x00000028)                                                     */
/*       BDX (0x00000028)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL1_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000028

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level1 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl1 : 6;
    /* txeq_half_lvl1 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level1 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL1_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* IOVB_TXEQ_HALF_LVL2_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x00000029)                                                     */
/*       BDX (0x00000029)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL2_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000029

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level2 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl2 : 6;
    /* txeq_half_lvl2 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level2 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL2_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* IOVB_TXEQ_HALF_LVL3_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x0000002A)                                                     */
/*       BDX (0x0000002A)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL3_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x0000002A

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level3 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl3 : 6;
    /* txeq_half_lvl3 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level3 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL3_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* IOVB_TX_VREF_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x0000002C)                                                     */
/*       BDX (0x0000002C)                                                     */
/* Register default value:              0x1E                                  */
#define IOVB_TX_VREF_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x0000002C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX VREF Register
 */
typedef union {
  struct {
    UINT8 tx_vref_sel : 5;
    /* tx_vref_sel - Bits[4:0], RWS, default = 5'h1E 
        
       	Comparator reference voltage select
       	This bit alone controls comparator reference voltage
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_VREF_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */


/* IOVB_TX_IREF_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x0000002D)                                                     */
/*       BDX (0x0000002D)                                                     */
/* Register default value:              0x00                                  */
#define IOVB_TX_IREF_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x0000002D

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX IREF Register
 */
typedef union {
  struct {
    UINT8 tx_iref_ctl : 5;
    /* tx_iref_ctl - Bits[4:0], RWS, default = 5'h0 
        
       	Input iref adjustment
       	100 ~ 111= reserved
       	011= 1.22* QPI iref (match bndl_vref_sel= 275mV)
       	010= 1.11* QPI iref (match bndl_vref_sel= 250mV)
       	001= 1.00* QPI iref (match bndl_vref_sel= 225mV)
       	000= 0.89* QPI iref (match bndl_vref_sel= 200mV)
       	Together with bndl_vref_ctl, this bit is used for swing control
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_IREF_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */














/* IOVB_RX_CTLE2_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x00000034)                                                     */
/*       BDX (0x00000034)                                                     */
/* Register default value:              0x44                                  */
#define IOVB_RX_CTLE2_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000034

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 2 Register
 */
typedef union {
  struct {
    UINT8 ctle_rdeg_fullspeed : 4;
    /* ctle_rdeg_fullspeed - Bits[3:0], RWS, default = 4'h4 
        
        full speed encoded number to degeneration resistance atoms to turn on in the 
       first stage of the CTLE 
             
     */
    UINT8 ctle_rdeg_halfspeed : 4;
    /* ctle_rdeg_halfspeed - Bits[7:4], RWS, default = 4'h4 
        
        half speed encoded number to degeneration resistance atoms to turn on in the 
       first stage of the CTLE 
             
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE2_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */




/* IOVB_DFE_BLEED0_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                */
/*       BDX (0x00000036)                                                     */
/* Register default value:              0x00                                  */
#define IOVB_DFE_BLEED0_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000036

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB DFE Bleeder Control 0 Register
 */
typedef union {
  struct {
    UINT8 dcbleed_en_sink : 2;
    /* dcbleed_en_sink - Bits[1:0], RWS, default = 2'h0 
        
       	DFE Bleed Enable Sink
             
     */
    UINT8 dcbleed_en_sink_ref : 2;
    /* dcbleed_en_sink_ref - Bits[3:2], RWS, default = 2'h0 
        
       	Enable DFE Bleed Sink Reference
       	Note: This is a bundle level signal
             
     */
    UINT8 gain_boost : 2;
    /* gain_boost - Bits[5:4], RWS, default = 2'h0 
        
       	Gain Boost
       	Note: This is a bundle level signal
             
     */
    UINT8 ratesel_en_cap : 1;
    /* ratesel_en_cap - Bits[6:6], RWS, default = 1'b0 
        
       	Rate Selection enable capacitor
       	Note: This is a bundle level signal
             
     */
    UINT8 ratesel_disable_twg : 1;
    /* ratesel_disable_twg - Bits[7:7], RWS, default = 1'b0 
        
       	Rate Selection disable TWG
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_DFE_BLEED0_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* IOVB_DFE_BLEED1_0_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                */
/*       BDX (0x00000037)                                                     */
/* Register default value:              0x10                                  */
#define IOVB_DFE_BLEED1_0_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000037

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB DFE Bleeder Control 1 Register
 */
typedef union {
  struct {
    UINT8 ratesel_bias_tune : 2;
    /* ratesel_bias_tune - Bits[1:0], RWS, default = 2'h0 
        
       	Rate select bias tune
             
     */
    UINT8 mixer_degen_dis : 1;
    /* mixer_degen_dis - Bits[2:2], RWS, default = 1'b0 
        
       	Mixer Degeneration disable
       	Note: This is a bundle level signal
             
     */
    UINT8 rload_slowck : 1;
    /* rload_slowck - Bits[3:3], RWS, default = 1'b0 
        
       	RLOAD Slow clock
       	Note: This is a bundle level signal
             
     */
    UINT8 dcbleed_pd : 1;
    /* dcbleed_pd - Bits[4:4], RWS, default = 1'b1 
        
       	DFE Bleeder Power Down
          Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_DFE_BLEED1_0_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */








/* IOVB_RX_CTLE0_1_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x00000044)                                                     */
/*       BDX (0x00000044)                                                     */
/* Register default value:              0x0F                                  */
#define IOVB_RX_CTLE0_1_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000044

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_ctle_offset_en : 1;
    /* rx_ctle_offset_en - Bits[0:0], RWS, default = 1'b1 
        
       	1 = Enable Receive CTLE offset correction
       	0 = disable
       	Note: this is needed on a lane basis for HVM testing
             
     */
    UINT8 rx_ctle_peak : 5;
    /* rx_ctle_peak - Bits[5:1], RWS, default = 5'h7 
        
        Full speed encoded number to degeneration capacitance atoms to turn on in the 
       first stage of the CTLE 
       	00000 = Disable equalization, boost DC gain.
             
     */
    UINT8 rx_ctle_vcm_ovrd_dis : 1;
    /* rx_ctle_vcm_ovrd_dis - Bits[6:6], RWS, default = 1'b0 
        
       	1 = Disable CTLE vcm override
       	0= enable CTLE internal common mode
       	This is per bundle control.
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE0_1_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */
































/* IOVB_TX_CTL0_1_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x00000054)                                                     */
/*       BDX (0x00000054)                                                     */
/* Register default value:              0x01                                  */
#define IOVB_TX_CTL0_1_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000054

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX Control 0 Register
 */
typedef union {
  struct {
    UINT8 tx_align_en : 1;
    /* tx_align_en - Bits[0:0], RWS, default = 1'b1 
        
       	Enable TX data alignment (TX FIFO enable?)
             
     */
    UINT8 tx_align_en_ovr : 1;
    /* tx_align_en_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for tx_align_en
             
     */
    UINT8 tx_spare : 4;
    /* tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	tx_spare[3:1]: Reserved
       	tx_spare[0]: Icomp tracking mode:
        0= (default) open loop mode, open icomp loop after acquisition. An (specified 
       by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp determined 
       by bndl_icomp_gain 
             
     */
    UINT8 tx_lcl_icomp_dis : 1;
    /* tx_lcl_icomp_dis - Bits[6:6], RWS, default = 1'b0 
        
       	Local icomp en.
       	0 =start the local TX block icomp loop. Should be enabled for normal operation.
        1 =stop the local TX block icomp loop. For debug or HVM mode to check the RTerm 
       legs. 
        Note: This signal is overidden by the power-up sequence logic in the common 
       block (comdig) and will not affect the icomp loop until the icomp calibration 
       has completed. Need the local loop to accurately measure the legs. 
             
     */
    UINT8 tx_sense_lane : 1;
    /* tx_sense_lane - Bits[7:7], RWS, default = 1'b0 
        
       	Lane 0 identifier for QPI
       	0= this bundle does not contain QPI lane 0
       	1= this bundle contains QPI lane 0
        When bit= 1, the lane will not enter L1 state, but the lane can still be in L0s 
       and perform Rx Detect 
        (NOTE: This behavior is not entirely correct for QPI, so the QPI agent will not 
       use it but instead will simply not assert L0s on its lane 0.) 
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_CTL0_1_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






































/* IOVB_TXEQ_HALF_LVL0_1_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x00000067)                                                     */
/*       BDX (0x00000067)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL0_1_0_QPIAGENT_DFX_PHY_IOVB_REG 0x00000067

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl0 : 6;
    /* txeq_half_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL0_1_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */










/* IOVB_TX_VREF_1_0_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x0000006C)                                                     */
/*       BDX (0x0000006C)                                                     */
/* Register default value:              0x1E                                  */
#define IOVB_TX_VREF_1_0_QPIAGENT_DFX_PHY_IOVB_REG 0x0000006C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX VREF Register
 */
typedef union {
  struct {
    UINT8 tx_vref_sel : 5;
    /* tx_vref_sel - Bits[4:0], RWS, default = 5'h1E 
        
       	Comparator reference voltage select
       	This bit alone controls comparator reference voltage
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_VREF_1_0_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






























/* IOVB_RX_CTLE0_0_1_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x00000084)                                                     */
/*       BDX (0x00000084)                                                     */
/* Register default value:              0x0F                                  */
#define IOVB_RX_CTLE0_0_1_QPIAGENT_DFX_PHY_IOVB_REG 0x00000084

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_ctle_offset_en : 1;
    /* rx_ctle_offset_en - Bits[0:0], RWS, default = 1'b1 
        
       	1 = Enable Receive CTLE offset correction
       	0 = disable
       	Note: this is needed on a lane basis for HVM testing
             
     */
    UINT8 rx_ctle_peak : 5;
    /* rx_ctle_peak - Bits[5:1], RWS, default = 5'h7 
        
        Full speed encoded number to degeneration capacitance atoms to turn on in the 
       first stage of the CTLE 
       	00000 = Disable equalization, boost DC gain.
             
     */
    UINT8 rx_ctle_vcm_ovrd_dis : 1;
    /* rx_ctle_vcm_ovrd_dis - Bits[6:6], RWS, default = 1'b0 
        
       	1 = Disable CTLE vcm override
       	0= enable CTLE internal common mode
       	This is per bundle control.
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE0_0_1_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */
































/* IOVB_TX_CTL0_0_1_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x00000094)                                                     */
/*       BDX (0x00000094)                                                     */
/* Register default value:              0x01                                  */
#define IOVB_TX_CTL0_0_1_QPIAGENT_DFX_PHY_IOVB_REG 0x00000094

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX Control 0 Register
 */
typedef union {
  struct {
    UINT8 tx_align_en : 1;
    /* tx_align_en - Bits[0:0], RWS, default = 1'b1 
        
       	Enable TX data alignment (TX FIFO enable?)
             
     */
    UINT8 tx_align_en_ovr : 1;
    /* tx_align_en_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for tx_align_en
             
     */
    UINT8 tx_spare : 4;
    /* tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	tx_spare[3:1]: Reserved
       	tx_spare[0]: Icomp tracking mode:
        0= (default) open loop mode, open icomp loop after acquisition. An (specified 
       by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp determined 
       by bndl_icomp_gain 
             
     */
    UINT8 tx_lcl_icomp_dis : 1;
    /* tx_lcl_icomp_dis - Bits[6:6], RWS, default = 1'b0 
        
       	Local icomp en.
       	0 =start the local TX block icomp loop. Should be enabled for normal operation.
        1 =stop the local TX block icomp loop. For debug or HVM mode to check the RTerm 
       legs. 
        Note: This signal is overidden by the power-up sequence logic in the common 
       block (comdig) and will not affect the icomp loop until the icomp calibration 
       has completed. Need the local loop to accurately measure the legs. 
             
     */
    UINT8 tx_sense_lane : 1;
    /* tx_sense_lane - Bits[7:7], RWS, default = 1'b0 
        
       	Lane 0 identifier for QPI
       	0= this bundle does not contain QPI lane 0
       	1= this bundle contains QPI lane 0
        When bit= 1, the lane will not enter L1 state, but the lane can still be in L0s 
       and perform Rx Detect 
        (NOTE: This behavior is not entirely correct for QPI, so the QPI agent will not 
       use it but instead will simply not assert L0s on its lane 0.) 
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_CTL0_0_1_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






































/* IOVB_TXEQ_HALF_LVL0_0_1_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x000000A7)                                                     */
/*       BDX (0x000000A7)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL0_0_1_QPIAGENT_DFX_PHY_IOVB_REG 0x000000A7

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl0 : 6;
    /* txeq_half_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL0_0_1_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */










/* IOVB_TX_VREF_0_1_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x000000AC)                                                     */
/*       BDX (0x000000AC)                                                     */
/* Register default value:              0x1E                                  */
#define IOVB_TX_VREF_0_1_QPIAGENT_DFX_PHY_IOVB_REG 0x000000AC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX VREF Register
 */
typedef union {
  struct {
    UINT8 tx_vref_sel : 5;
    /* tx_vref_sel - Bits[4:0], RWS, default = 5'h1E 
        
       	Comparator reference voltage select
       	This bit alone controls comparator reference voltage
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_VREF_0_1_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






























/* IOVB_RX_CTLE0_1_1_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x000000C4)                                                     */
/*       BDX (0x000000C4)                                                     */
/* Register default value:              0x0F                                  */
#define IOVB_RX_CTLE0_1_1_QPIAGENT_DFX_PHY_IOVB_REG 0x000000C4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_ctle_offset_en : 1;
    /* rx_ctle_offset_en - Bits[0:0], RWS, default = 1'b1 
        
       	1 = Enable Receive CTLE offset correction
       	0 = disable
       	Note: this is needed on a lane basis for HVM testing
             
     */
    UINT8 rx_ctle_peak : 5;
    /* rx_ctle_peak - Bits[5:1], RWS, default = 5'h7 
        
        Full speed encoded number to degeneration capacitance atoms to turn on in the 
       first stage of the CTLE 
       	00000 = Disable equalization, boost DC gain.
             
     */
    UINT8 rx_ctle_vcm_ovrd_dis : 1;
    /* rx_ctle_vcm_ovrd_dis - Bits[6:6], RWS, default = 1'b0 
        
       	1 = Disable CTLE vcm override
       	0= enable CTLE internal common mode
       	This is per bundle control.
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE0_1_1_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */
































/* IOVB_TX_CTL0_1_1_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x000000D4)                                                     */
/*       BDX (0x000000D4)                                                     */
/* Register default value:              0x01                                  */
#define IOVB_TX_CTL0_1_1_QPIAGENT_DFX_PHY_IOVB_REG 0x000000D4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX Control 0 Register
 */
typedef union {
  struct {
    UINT8 tx_align_en : 1;
    /* tx_align_en - Bits[0:0], RWS, default = 1'b1 
        
       	Enable TX data alignment (TX FIFO enable?)
             
     */
    UINT8 tx_align_en_ovr : 1;
    /* tx_align_en_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for tx_align_en
             
     */
    UINT8 tx_spare : 4;
    /* tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	tx_spare[3:1]: Reserved
       	tx_spare[0]: Icomp tracking mode:
        0= (default) open loop mode, open icomp loop after acquisition. An (specified 
       by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp determined 
       by bndl_icomp_gain 
             
     */
    UINT8 tx_lcl_icomp_dis : 1;
    /* tx_lcl_icomp_dis - Bits[6:6], RWS, default = 1'b0 
        
       	Local icomp en.
       	0 =start the local TX block icomp loop. Should be enabled for normal operation.
        1 =stop the local TX block icomp loop. For debug or HVM mode to check the RTerm 
       legs. 
        Note: This signal is overidden by the power-up sequence logic in the common 
       block (comdig) and will not affect the icomp loop until the icomp calibration 
       has completed. Need the local loop to accurately measure the legs. 
             
     */
    UINT8 tx_sense_lane : 1;
    /* tx_sense_lane - Bits[7:7], RWS, default = 1'b0 
        
       	Lane 0 identifier for QPI
       	0= this bundle does not contain QPI lane 0
       	1= this bundle contains QPI lane 0
        When bit= 1, the lane will not enter L1 state, but the lane can still be in L0s 
       and perform Rx Detect 
        (NOTE: This behavior is not entirely correct for QPI, so the QPI agent will not 
       use it but instead will simply not assert L0s on its lane 0.) 
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_CTL0_1_1_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






































/* IOVB_TXEQ_HALF_LVL0_1_1_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x000000E7)                                                     */
/*       BDX (0x000000E7)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL0_1_1_QPIAGENT_DFX_PHY_IOVB_REG 0x000000E7

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl0 : 6;
    /* txeq_half_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL0_1_1_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */










/* IOVB_TX_VREF_1_1_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x000000EC)                                                     */
/*       BDX (0x000000EC)                                                     */
/* Register default value:              0x1E                                  */
#define IOVB_TX_VREF_1_1_QPIAGENT_DFX_PHY_IOVB_REG 0x000000EC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX VREF Register
 */
typedef union {
  struct {
    UINT8 tx_vref_sel : 5;
    /* tx_vref_sel - Bits[4:0], RWS, default = 5'h1E 
        
       	Comparator reference voltage select
       	This bit alone controls comparator reference voltage
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_VREF_1_1_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






























/* IOVB_RX_CTLE0_0_2_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x00000104)                                                     */
/*       BDX (0x00000104)                                                     */
/* Register default value:              0x0F                                  */
#define IOVB_RX_CTLE0_0_2_QPIAGENT_DFX_PHY_IOVB_REG 0x00000104

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_ctle_offset_en : 1;
    /* rx_ctle_offset_en - Bits[0:0], RWS, default = 1'b1 
        
       	1 = Enable Receive CTLE offset correction
       	0 = disable
       	Note: this is needed on a lane basis for HVM testing
             
     */
    UINT8 rx_ctle_peak : 5;
    /* rx_ctle_peak - Bits[5:1], RWS, default = 5'h7 
        
        Full speed encoded number to degeneration capacitance atoms to turn on in the 
       first stage of the CTLE 
       	00000 = Disable equalization, boost DC gain.
             
     */
    UINT8 rx_ctle_vcm_ovrd_dis : 1;
    /* rx_ctle_vcm_ovrd_dis - Bits[6:6], RWS, default = 1'b0 
        
       	1 = Disable CTLE vcm override
       	0= enable CTLE internal common mode
       	This is per bundle control.
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE0_0_2_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */
































/* IOVB_TX_CTL0_0_2_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x00000114)                                                     */
/*       BDX (0x00000114)                                                     */
/* Register default value:              0x01                                  */
#define IOVB_TX_CTL0_0_2_QPIAGENT_DFX_PHY_IOVB_REG 0x00000114

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX Control 0 Register
 */
typedef union {
  struct {
    UINT8 tx_align_en : 1;
    /* tx_align_en - Bits[0:0], RWS, default = 1'b1 
        
       	Enable TX data alignment (TX FIFO enable?)
             
     */
    UINT8 tx_align_en_ovr : 1;
    /* tx_align_en_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for tx_align_en
             
     */
    UINT8 tx_spare : 4;
    /* tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	tx_spare[3:1]: Reserved
       	tx_spare[0]: Icomp tracking mode:
        0= (default) open loop mode, open icomp loop after acquisition. An (specified 
       by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp determined 
       by bndl_icomp_gain 
             
     */
    UINT8 tx_lcl_icomp_dis : 1;
    /* tx_lcl_icomp_dis - Bits[6:6], RWS, default = 1'b0 
        
       	Local icomp en.
       	0 =start the local TX block icomp loop. Should be enabled for normal operation.
        1 =stop the local TX block icomp loop. For debug or HVM mode to check the RTerm 
       legs. 
        Note: This signal is overidden by the power-up sequence logic in the common 
       block (comdig) and will not affect the icomp loop until the icomp calibration 
       has completed. Need the local loop to accurately measure the legs. 
             
     */
    UINT8 tx_sense_lane : 1;
    /* tx_sense_lane - Bits[7:7], RWS, default = 1'b0 
        
       	Lane 0 identifier for QPI
       	0= this bundle does not contain QPI lane 0
       	1= this bundle contains QPI lane 0
        When bit= 1, the lane will not enter L1 state, but the lane can still be in L0s 
       and perform Rx Detect 
        (NOTE: This behavior is not entirely correct for QPI, so the QPI agent will not 
       use it but instead will simply not assert L0s on its lane 0.) 
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_CTL0_0_2_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






































/* IOVB_TXEQ_HALF_LVL0_0_2_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x00000127)                                                     */
/*       BDX (0x00000127)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL0_0_2_QPIAGENT_DFX_PHY_IOVB_REG 0x00000127

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl0 : 6;
    /* txeq_half_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL0_0_2_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */










/* IOVB_TX_VREF_0_2_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x0000012C)                                                     */
/*       BDX (0x0000012C)                                                     */
/* Register default value:              0x1E                                  */
#define IOVB_TX_VREF_0_2_QPIAGENT_DFX_PHY_IOVB_REG 0x0000012C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX VREF Register
 */
typedef union {
  struct {
    UINT8 tx_vref_sel : 5;
    /* tx_vref_sel - Bits[4:0], RWS, default = 5'h1E 
        
       	Comparator reference voltage select
       	This bit alone controls comparator reference voltage
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_VREF_0_2_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






























/* IOVB_RX_CTLE0_1_2_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x00000144)                                                     */
/*       BDX (0x00000144)                                                     */
/* Register default value:              0x0F                                  */
#define IOVB_RX_CTLE0_1_2_QPIAGENT_DFX_PHY_IOVB_REG 0x00000144

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_ctle_offset_en : 1;
    /* rx_ctle_offset_en - Bits[0:0], RWS, default = 1'b1 
        
       	1 = Enable Receive CTLE offset correction
       	0 = disable
       	Note: this is needed on a lane basis for HVM testing
             
     */
    UINT8 rx_ctle_peak : 5;
    /* rx_ctle_peak - Bits[5:1], RWS, default = 5'h7 
        
        Full speed encoded number to degeneration capacitance atoms to turn on in the 
       first stage of the CTLE 
       	00000 = Disable equalization, boost DC gain.
             
     */
    UINT8 rx_ctle_vcm_ovrd_dis : 1;
    /* rx_ctle_vcm_ovrd_dis - Bits[6:6], RWS, default = 1'b0 
        
       	1 = Disable CTLE vcm override
       	0= enable CTLE internal common mode
       	This is per bundle control.
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE0_1_2_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */
































/* IOVB_TX_CTL0_1_2_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x00000154)                                                     */
/*       BDX (0x00000154)                                                     */
/* Register default value:              0x01                                  */
#define IOVB_TX_CTL0_1_2_QPIAGENT_DFX_PHY_IOVB_REG 0x00000154

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX Control 0 Register
 */
typedef union {
  struct {
    UINT8 tx_align_en : 1;
    /* tx_align_en - Bits[0:0], RWS, default = 1'b1 
        
       	Enable TX data alignment (TX FIFO enable?)
             
     */
    UINT8 tx_align_en_ovr : 1;
    /* tx_align_en_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for tx_align_en
             
     */
    UINT8 tx_spare : 4;
    /* tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	tx_spare[3:1]: Reserved
       	tx_spare[0]: Icomp tracking mode:
        0= (default) open loop mode, open icomp loop after acquisition. An (specified 
       by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp determined 
       by bndl_icomp_gain 
             
     */
    UINT8 tx_lcl_icomp_dis : 1;
    /* tx_lcl_icomp_dis - Bits[6:6], RWS, default = 1'b0 
        
       	Local icomp en.
       	0 =start the local TX block icomp loop. Should be enabled for normal operation.
        1 =stop the local TX block icomp loop. For debug or HVM mode to check the RTerm 
       legs. 
        Note: This signal is overidden by the power-up sequence logic in the common 
       block (comdig) and will not affect the icomp loop until the icomp calibration 
       has completed. Need the local loop to accurately measure the legs. 
             
     */
    UINT8 tx_sense_lane : 1;
    /* tx_sense_lane - Bits[7:7], RWS, default = 1'b0 
        
       	Lane 0 identifier for QPI
       	0= this bundle does not contain QPI lane 0
       	1= this bundle contains QPI lane 0
        When bit= 1, the lane will not enter L1 state, but the lane can still be in L0s 
       and perform Rx Detect 
        (NOTE: This behavior is not entirely correct for QPI, so the QPI agent will not 
       use it but instead will simply not assert L0s on its lane 0.) 
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_CTL0_1_2_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






































/* IOVB_TXEQ_HALF_LVL0_1_2_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x00000167)                                                     */
/*       BDX (0x00000167)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL0_1_2_QPIAGENT_DFX_PHY_IOVB_REG 0x00000167

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl0 : 6;
    /* txeq_half_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL0_1_2_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */










/* IOVB_TX_VREF_1_2_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x0000016C)                                                     */
/*       BDX (0x0000016C)                                                     */
/* Register default value:              0x1E                                  */
#define IOVB_TX_VREF_1_2_QPIAGENT_DFX_PHY_IOVB_REG 0x0000016C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX VREF Register
 */
typedef union {
  struct {
    UINT8 tx_vref_sel : 5;
    /* tx_vref_sel - Bits[4:0], RWS, default = 5'h1E 
        
       	Comparator reference voltage select
       	This bit alone controls comparator reference voltage
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_VREF_1_2_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






























/* IOVB_RX_CTLE0_0_3_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x00000184)                                                     */
/*       BDX (0x00000184)                                                     */
/* Register default value:              0x0F                                  */
#define IOVB_RX_CTLE0_0_3_QPIAGENT_DFX_PHY_IOVB_REG 0x00000184

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_ctle_offset_en : 1;
    /* rx_ctle_offset_en - Bits[0:0], RWS, default = 1'b1 
        
       	1 = Enable Receive CTLE offset correction
       	0 = disable
       	Note: this is needed on a lane basis for HVM testing
             
     */
    UINT8 rx_ctle_peak : 5;
    /* rx_ctle_peak - Bits[5:1], RWS, default = 5'h7 
        
        Full speed encoded number to degeneration capacitance atoms to turn on in the 
       first stage of the CTLE 
       	00000 = Disable equalization, boost DC gain.
             
     */
    UINT8 rx_ctle_vcm_ovrd_dis : 1;
    /* rx_ctle_vcm_ovrd_dis - Bits[6:6], RWS, default = 1'b0 
        
       	1 = Disable CTLE vcm override
       	0= enable CTLE internal common mode
       	This is per bundle control.
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE0_0_3_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */
































/* IOVB_TX_CTL0_0_3_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x00000194)                                                     */
/*       BDX (0x00000194)                                                     */
/* Register default value:              0x01                                  */
#define IOVB_TX_CTL0_0_3_QPIAGENT_DFX_PHY_IOVB_REG 0x00000194

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX Control 0 Register
 */
typedef union {
  struct {
    UINT8 tx_align_en : 1;
    /* tx_align_en - Bits[0:0], RWS, default = 1'b1 
        
       	Enable TX data alignment (TX FIFO enable?)
             
     */
    UINT8 tx_align_en_ovr : 1;
    /* tx_align_en_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for tx_align_en
             
     */
    UINT8 tx_spare : 4;
    /* tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	tx_spare[3:1]: Reserved
       	tx_spare[0]: Icomp tracking mode:
        0= (default) open loop mode, open icomp loop after acquisition. An (specified 
       by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp determined 
       by bndl_icomp_gain 
             
     */
    UINT8 tx_lcl_icomp_dis : 1;
    /* tx_lcl_icomp_dis - Bits[6:6], RWS, default = 1'b0 
        
       	Local icomp en.
       	0 =start the local TX block icomp loop. Should be enabled for normal operation.
        1 =stop the local TX block icomp loop. For debug or HVM mode to check the RTerm 
       legs. 
        Note: This signal is overidden by the power-up sequence logic in the common 
       block (comdig) and will not affect the icomp loop until the icomp calibration 
       has completed. Need the local loop to accurately measure the legs. 
             
     */
    UINT8 tx_sense_lane : 1;
    /* tx_sense_lane - Bits[7:7], RWS, default = 1'b0 
        
       	Lane 0 identifier for QPI
       	0= this bundle does not contain QPI lane 0
       	1= this bundle contains QPI lane 0
        When bit= 1, the lane will not enter L1 state, but the lane can still be in L0s 
       and perform Rx Detect 
        (NOTE: This behavior is not entirely correct for QPI, so the QPI agent will not 
       use it but instead will simply not assert L0s on its lane 0.) 
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_CTL0_0_3_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






































/* IOVB_TXEQ_HALF_LVL0_0_3_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x000001A7)                                                     */
/*       BDX (0x000001A7)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL0_0_3_QPIAGENT_DFX_PHY_IOVB_REG 0x000001A7

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl0 : 6;
    /* txeq_half_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL0_0_3_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */










/* IOVB_TX_VREF_0_3_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x000001AC)                                                     */
/*       BDX (0x000001AC)                                                     */
/* Register default value:              0x1E                                  */
#define IOVB_TX_VREF_0_3_QPIAGENT_DFX_PHY_IOVB_REG 0x000001AC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX VREF Register
 */
typedef union {
  struct {
    UINT8 tx_vref_sel : 5;
    /* tx_vref_sel - Bits[4:0], RWS, default = 5'h1E 
        
       	Comparator reference voltage select
       	This bit alone controls comparator reference voltage
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_VREF_0_3_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






























/* IOVB_RX_CTLE0_1_3_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x000001C4)                                                     */
/*       BDX (0x000001C4)                                                     */
/* Register default value:              0x0F                                  */
#define IOVB_RX_CTLE0_1_3_QPIAGENT_DFX_PHY_IOVB_REG 0x000001C4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_ctle_offset_en : 1;
    /* rx_ctle_offset_en - Bits[0:0], RWS, default = 1'b1 
        
       	1 = Enable Receive CTLE offset correction
       	0 = disable
       	Note: this is needed on a lane basis for HVM testing
             
     */
    UINT8 rx_ctle_peak : 5;
    /* rx_ctle_peak - Bits[5:1], RWS, default = 5'h7 
        
        Full speed encoded number to degeneration capacitance atoms to turn on in the 
       first stage of the CTLE 
       	00000 = Disable equalization, boost DC gain.
             
     */
    UINT8 rx_ctle_vcm_ovrd_dis : 1;
    /* rx_ctle_vcm_ovrd_dis - Bits[6:6], RWS, default = 1'b0 
        
       	1 = Disable CTLE vcm override
       	0= enable CTLE internal common mode
       	This is per bundle control.
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE0_1_3_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */
































/* IOVB_TX_CTL0_1_3_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x000001D4)                                                     */
/*       BDX (0x000001D4)                                                     */
/* Register default value:              0x01                                  */
#define IOVB_TX_CTL0_1_3_QPIAGENT_DFX_PHY_IOVB_REG 0x000001D4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX Control 0 Register
 */
typedef union {
  struct {
    UINT8 tx_align_en : 1;
    /* tx_align_en - Bits[0:0], RWS, default = 1'b1 
        
       	Enable TX data alignment (TX FIFO enable?)
             
     */
    UINT8 tx_align_en_ovr : 1;
    /* tx_align_en_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for tx_align_en
             
     */
    UINT8 tx_spare : 4;
    /* tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	tx_spare[3:1]: Reserved
       	tx_spare[0]: Icomp tracking mode:
        0= (default) open loop mode, open icomp loop after acquisition. An (specified 
       by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp determined 
       by bndl_icomp_gain 
             
     */
    UINT8 tx_lcl_icomp_dis : 1;
    /* tx_lcl_icomp_dis - Bits[6:6], RWS, default = 1'b0 
        
       	Local icomp en.
       	0 =start the local TX block icomp loop. Should be enabled for normal operation.
        1 =stop the local TX block icomp loop. For debug or HVM mode to check the RTerm 
       legs. 
        Note: This signal is overidden by the power-up sequence logic in the common 
       block (comdig) and will not affect the icomp loop until the icomp calibration 
       has completed. Need the local loop to accurately measure the legs. 
             
     */
    UINT8 tx_sense_lane : 1;
    /* tx_sense_lane - Bits[7:7], RWS, default = 1'b0 
        
       	Lane 0 identifier for QPI
       	0= this bundle does not contain QPI lane 0
       	1= this bundle contains QPI lane 0
        When bit= 1, the lane will not enter L1 state, but the lane can still be in L0s 
       and perform Rx Detect 
        (NOTE: This behavior is not entirely correct for QPI, so the QPI agent will not 
       use it but instead will simply not assert L0s on its lane 0.) 
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_CTL0_1_3_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






































/* IOVB_TXEQ_HALF_LVL0_1_3_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x000001E7)                                                     */
/*       BDX (0x000001E7)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL0_1_3_QPIAGENT_DFX_PHY_IOVB_REG 0x000001E7

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl0 : 6;
    /* txeq_half_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL0_1_3_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */










/* IOVB_TX_VREF_1_3_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x000001EC)                                                     */
/*       BDX (0x000001EC)                                                     */
/* Register default value:              0x1E                                  */
#define IOVB_TX_VREF_1_3_QPIAGENT_DFX_PHY_IOVB_REG 0x000001EC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX VREF Register
 */
typedef union {
  struct {
    UINT8 tx_vref_sel : 5;
    /* tx_vref_sel - Bits[4:0], RWS, default = 5'h1E 
        
       	Comparator reference voltage select
       	This bit alone controls comparator reference voltage
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_VREF_1_3_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






























/* IOVB_RX_CTLE0_0_4_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x00000204)                                                     */
/*       BDX (0x00000204)                                                     */
/* Register default value:              0x0F                                  */
#define IOVB_RX_CTLE0_0_4_QPIAGENT_DFX_PHY_IOVB_REG 0x00000204

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_ctle_offset_en : 1;
    /* rx_ctle_offset_en - Bits[0:0], RWS, default = 1'b1 
        
       	1 = Enable Receive CTLE offset correction
       	0 = disable
       	Note: this is needed on a lane basis for HVM testing
             
     */
    UINT8 rx_ctle_peak : 5;
    /* rx_ctle_peak - Bits[5:1], RWS, default = 5'h7 
        
        Full speed encoded number to degeneration capacitance atoms to turn on in the 
       first stage of the CTLE 
       	00000 = Disable equalization, boost DC gain.
             
     */
    UINT8 rx_ctle_vcm_ovrd_dis : 1;
    /* rx_ctle_vcm_ovrd_dis - Bits[6:6], RWS, default = 1'b0 
        
       	1 = Disable CTLE vcm override
       	0= enable CTLE internal common mode
       	This is per bundle control.
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE0_0_4_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */
































/* IOVB_TX_CTL0_0_4_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x00000214)                                                     */
/*       BDX (0x00000214)                                                     */
/* Register default value:              0x01                                  */
#define IOVB_TX_CTL0_0_4_QPIAGENT_DFX_PHY_IOVB_REG 0x00000214

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX Control 0 Register
 */
typedef union {
  struct {
    UINT8 tx_align_en : 1;
    /* tx_align_en - Bits[0:0], RWS, default = 1'b1 
        
       	Enable TX data alignment (TX FIFO enable?)
             
     */
    UINT8 tx_align_en_ovr : 1;
    /* tx_align_en_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for tx_align_en
             
     */
    UINT8 tx_spare : 4;
    /* tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	tx_spare[3:1]: Reserved
       	tx_spare[0]: Icomp tracking mode:
        0= (default) open loop mode, open icomp loop after acquisition. An (specified 
       by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp determined 
       by bndl_icomp_gain 
             
     */
    UINT8 tx_lcl_icomp_dis : 1;
    /* tx_lcl_icomp_dis - Bits[6:6], RWS, default = 1'b0 
        
       	Local icomp en.
       	0 =start the local TX block icomp loop. Should be enabled for normal operation.
        1 =stop the local TX block icomp loop. For debug or HVM mode to check the RTerm 
       legs. 
        Note: This signal is overidden by the power-up sequence logic in the common 
       block (comdig) and will not affect the icomp loop until the icomp calibration 
       has completed. Need the local loop to accurately measure the legs. 
             
     */
    UINT8 tx_sense_lane : 1;
    /* tx_sense_lane - Bits[7:7], RWS, default = 1'b0 
        
       	Lane 0 identifier for QPI
       	0= this bundle does not contain QPI lane 0
       	1= this bundle contains QPI lane 0
        When bit= 1, the lane will not enter L1 state, but the lane can still be in L0s 
       and perform Rx Detect 
        (NOTE: This behavior is not entirely correct for QPI, so the QPI agent will not 
       use it but instead will simply not assert L0s on its lane 0.) 
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_CTL0_0_4_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






































/* IOVB_TXEQ_HALF_LVL0_0_4_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x00000227)                                                     */
/*       BDX (0x00000227)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL0_0_4_QPIAGENT_DFX_PHY_IOVB_REG 0x00000227

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl0 : 6;
    /* txeq_half_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL0_0_4_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */










/* IOVB_TX_VREF_0_4_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x0000022C)                                                     */
/*       BDX (0x0000022C)                                                     */
/* Register default value:              0x1E                                  */
#define IOVB_TX_VREF_0_4_QPIAGENT_DFX_PHY_IOVB_REG 0x0000022C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX VREF Register
 */
typedef union {
  struct {
    UINT8 tx_vref_sel : 5;
    /* tx_vref_sel - Bits[4:0], RWS, default = 5'h1E 
        
       	Comparator reference voltage select
       	This bit alone controls comparator reference voltage
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_VREF_0_4_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






























/* IOVB_RX_CTLE0_1_4_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x00000244)                                                     */
/*       BDX (0x00000244)                                                     */
/* Register default value:              0x0F                                  */
#define IOVB_RX_CTLE0_1_4_QPIAGENT_DFX_PHY_IOVB_REG 0x00000244

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_ctle_offset_en : 1;
    /* rx_ctle_offset_en - Bits[0:0], RWS, default = 1'b1 
        
       	1 = Enable Receive CTLE offset correction
       	0 = disable
       	Note: this is needed on a lane basis for HVM testing
             
     */
    UINT8 rx_ctle_peak : 5;
    /* rx_ctle_peak - Bits[5:1], RWS, default = 5'h7 
        
        Full speed encoded number to degeneration capacitance atoms to turn on in the 
       first stage of the CTLE 
       	00000 = Disable equalization, boost DC gain.
             
     */
    UINT8 rx_ctle_vcm_ovrd_dis : 1;
    /* rx_ctle_vcm_ovrd_dis - Bits[6:6], RWS, default = 1'b0 
        
       	1 = Disable CTLE vcm override
       	0= enable CTLE internal common mode
       	This is per bundle control.
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE0_1_4_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */
































/* IOVB_TX_CTL0_1_4_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x00000254)                                                     */
/*       BDX (0x00000254)                                                     */
/* Register default value:              0x01                                  */
#define IOVB_TX_CTL0_1_4_QPIAGENT_DFX_PHY_IOVB_REG 0x00000254

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX Control 0 Register
 */
typedef union {
  struct {
    UINT8 tx_align_en : 1;
    /* tx_align_en - Bits[0:0], RWS, default = 1'b1 
        
       	Enable TX data alignment (TX FIFO enable?)
             
     */
    UINT8 tx_align_en_ovr : 1;
    /* tx_align_en_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for tx_align_en
             
     */
    UINT8 tx_spare : 4;
    /* tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	tx_spare[3:1]: Reserved
       	tx_spare[0]: Icomp tracking mode:
        0= (default) open loop mode, open icomp loop after acquisition. An (specified 
       by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp determined 
       by bndl_icomp_gain 
             
     */
    UINT8 tx_lcl_icomp_dis : 1;
    /* tx_lcl_icomp_dis - Bits[6:6], RWS, default = 1'b0 
        
       	Local icomp en.
       	0 =start the local TX block icomp loop. Should be enabled for normal operation.
        1 =stop the local TX block icomp loop. For debug or HVM mode to check the RTerm 
       legs. 
        Note: This signal is overidden by the power-up sequence logic in the common 
       block (comdig) and will not affect the icomp loop until the icomp calibration 
       has completed. Need the local loop to accurately measure the legs. 
             
     */
    UINT8 tx_sense_lane : 1;
    /* tx_sense_lane - Bits[7:7], RWS, default = 1'b0 
        
       	Lane 0 identifier for QPI
       	0= this bundle does not contain QPI lane 0
       	1= this bundle contains QPI lane 0
        When bit= 1, the lane will not enter L1 state, but the lane can still be in L0s 
       and perform Rx Detect 
        (NOTE: This behavior is not entirely correct for QPI, so the QPI agent will not 
       use it but instead will simply not assert L0s on its lane 0.) 
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_CTL0_1_4_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






































/* IOVB_TXEQ_HALF_LVL0_1_4_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x00000267)                                                     */
/*       BDX (0x00000267)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL0_1_4_QPIAGENT_DFX_PHY_IOVB_REG 0x00000267

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl0 : 6;
    /* txeq_half_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL0_1_4_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */










/* IOVB_TX_VREF_1_4_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x0000026C)                                                     */
/*       BDX (0x0000026C)                                                     */
/* Register default value:              0x1E                                  */
#define IOVB_TX_VREF_1_4_QPIAGENT_DFX_PHY_IOVB_REG 0x0000026C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX VREF Register
 */
typedef union {
  struct {
    UINT8 tx_vref_sel : 5;
    /* tx_vref_sel - Bits[4:0], RWS, default = 5'h1E 
        
       	Comparator reference voltage select
       	This bit alone controls comparator reference voltage
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_VREF_1_4_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






























/* IOVB_RX_CTLE0_0_5_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x00000284)                                                     */
/*       BDX (0x00000284)                                                     */
/* Register default value:              0x0F                                  */
#define IOVB_RX_CTLE0_0_5_QPIAGENT_DFX_PHY_IOVB_REG 0x00000284

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_ctle_offset_en : 1;
    /* rx_ctle_offset_en - Bits[0:0], RWS, default = 1'b1 
        
       	1 = Enable Receive CTLE offset correction
       	0 = disable
       	Note: this is needed on a lane basis for HVM testing
             
     */
    UINT8 rx_ctle_peak : 5;
    /* rx_ctle_peak - Bits[5:1], RWS, default = 5'h7 
        
        Full speed encoded number to degeneration capacitance atoms to turn on in the 
       first stage of the CTLE 
       	00000 = Disable equalization, boost DC gain.
             
     */
    UINT8 rx_ctle_vcm_ovrd_dis : 1;
    /* rx_ctle_vcm_ovrd_dis - Bits[6:6], RWS, default = 1'b0 
        
       	1 = Disable CTLE vcm override
       	0= enable CTLE internal common mode
       	This is per bundle control.
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE0_0_5_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */
































/* IOVB_TX_CTL0_0_5_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x00000294)                                                     */
/*       BDX (0x00000294)                                                     */
/* Register default value:              0x01                                  */
#define IOVB_TX_CTL0_0_5_QPIAGENT_DFX_PHY_IOVB_REG 0x00000294

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX Control 0 Register
 */
typedef union {
  struct {
    UINT8 tx_align_en : 1;
    /* tx_align_en - Bits[0:0], RWS, default = 1'b1 
        
       	Enable TX data alignment (TX FIFO enable?)
             
     */
    UINT8 tx_align_en_ovr : 1;
    /* tx_align_en_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for tx_align_en
             
     */
    UINT8 tx_spare : 4;
    /* tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	tx_spare[3:1]: Reserved
       	tx_spare[0]: Icomp tracking mode:
        0= (default) open loop mode, open icomp loop after acquisition. An (specified 
       by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp determined 
       by bndl_icomp_gain 
             
     */
    UINT8 tx_lcl_icomp_dis : 1;
    /* tx_lcl_icomp_dis - Bits[6:6], RWS, default = 1'b0 
        
       	Local icomp en.
       	0 =start the local TX block icomp loop. Should be enabled for normal operation.
        1 =stop the local TX block icomp loop. For debug or HVM mode to check the RTerm 
       legs. 
        Note: This signal is overidden by the power-up sequence logic in the common 
       block (comdig) and will not affect the icomp loop until the icomp calibration 
       has completed. Need the local loop to accurately measure the legs. 
             
     */
    UINT8 tx_sense_lane : 1;
    /* tx_sense_lane - Bits[7:7], RWS, default = 1'b0 
        
       	Lane 0 identifier for QPI
       	0= this bundle does not contain QPI lane 0
       	1= this bundle contains QPI lane 0
        When bit= 1, the lane will not enter L1 state, but the lane can still be in L0s 
       and perform Rx Detect 
        (NOTE: This behavior is not entirely correct for QPI, so the QPI agent will not 
       use it but instead will simply not assert L0s on its lane 0.) 
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_CTL0_0_5_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






































/* IOVB_TXEQ_HALF_LVL0_0_5_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x000002A7)                                                     */
/*       BDX (0x000002A7)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL0_0_5_QPIAGENT_DFX_PHY_IOVB_REG 0x000002A7

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl0 : 6;
    /* txeq_half_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL0_0_5_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */










/* IOVB_TX_VREF_0_5_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x000002AC)                                                     */
/*       BDX (0x000002AC)                                                     */
/* Register default value:              0x1E                                  */
#define IOVB_TX_VREF_0_5_QPIAGENT_DFX_PHY_IOVB_REG 0x000002AC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX VREF Register
 */
typedef union {
  struct {
    UINT8 tx_vref_sel : 5;
    /* tx_vref_sel - Bits[4:0], RWS, default = 5'h1E 
        
       	Comparator reference voltage select
       	This bit alone controls comparator reference voltage
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_VREF_0_5_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






























/* IOVB_RX_CTLE0_1_5_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x000002C4)                                                     */
/*       BDX (0x000002C4)                                                     */
/* Register default value:              0x0F                                  */
#define IOVB_RX_CTLE0_1_5_QPIAGENT_DFX_PHY_IOVB_REG 0x000002C4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_ctle_offset_en : 1;
    /* rx_ctle_offset_en - Bits[0:0], RWS, default = 1'b1 
        
       	1 = Enable Receive CTLE offset correction
       	0 = disable
       	Note: this is needed on a lane basis for HVM testing
             
     */
    UINT8 rx_ctle_peak : 5;
    /* rx_ctle_peak - Bits[5:1], RWS, default = 5'h7 
        
        Full speed encoded number to degeneration capacitance atoms to turn on in the 
       first stage of the CTLE 
       	00000 = Disable equalization, boost DC gain.
             
     */
    UINT8 rx_ctle_vcm_ovrd_dis : 1;
    /* rx_ctle_vcm_ovrd_dis - Bits[6:6], RWS, default = 1'b0 
        
       	1 = Disable CTLE vcm override
       	0= enable CTLE internal common mode
       	This is per bundle control.
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE0_1_5_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */
































/* IOVB_TX_CTL0_1_5_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x000002D4)                                                     */
/*       BDX (0x000002D4)                                                     */
/* Register default value:              0x01                                  */
#define IOVB_TX_CTL0_1_5_QPIAGENT_DFX_PHY_IOVB_REG 0x000002D4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX Control 0 Register
 */
typedef union {
  struct {
    UINT8 tx_align_en : 1;
    /* tx_align_en - Bits[0:0], RWS, default = 1'b1 
        
       	Enable TX data alignment (TX FIFO enable?)
             
     */
    UINT8 tx_align_en_ovr : 1;
    /* tx_align_en_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for tx_align_en
             
     */
    UINT8 tx_spare : 4;
    /* tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	tx_spare[3:1]: Reserved
       	tx_spare[0]: Icomp tracking mode:
        0= (default) open loop mode, open icomp loop after acquisition. An (specified 
       by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp determined 
       by bndl_icomp_gain 
             
     */
    UINT8 tx_lcl_icomp_dis : 1;
    /* tx_lcl_icomp_dis - Bits[6:6], RWS, default = 1'b0 
        
       	Local icomp en.
       	0 =start the local TX block icomp loop. Should be enabled for normal operation.
        1 =stop the local TX block icomp loop. For debug or HVM mode to check the RTerm 
       legs. 
        Note: This signal is overidden by the power-up sequence logic in the common 
       block (comdig) and will not affect the icomp loop until the icomp calibration 
       has completed. Need the local loop to accurately measure the legs. 
             
     */
    UINT8 tx_sense_lane : 1;
    /* tx_sense_lane - Bits[7:7], RWS, default = 1'b0 
        
       	Lane 0 identifier for QPI
       	0= this bundle does not contain QPI lane 0
       	1= this bundle contains QPI lane 0
        When bit= 1, the lane will not enter L1 state, but the lane can still be in L0s 
       and perform Rx Detect 
        (NOTE: This behavior is not entirely correct for QPI, so the QPI agent will not 
       use it but instead will simply not assert L0s on its lane 0.) 
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_CTL0_1_5_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






































/* IOVB_TXEQ_HALF_LVL0_1_5_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x000002E7)                                                     */
/*       BDX (0x000002E7)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL0_1_5_QPIAGENT_DFX_PHY_IOVB_REG 0x000002E7

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl0 : 6;
    /* txeq_half_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL0_1_5_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */










/* IOVB_TX_VREF_1_5_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x000002EC)                                                     */
/*       BDX (0x000002EC)                                                     */
/* Register default value:              0x1E                                  */
#define IOVB_TX_VREF_1_5_QPIAGENT_DFX_PHY_IOVB_REG 0x000002EC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX VREF Register
 */
typedef union {
  struct {
    UINT8 tx_vref_sel : 5;
    /* tx_vref_sel - Bits[4:0], RWS, default = 5'h1E 
        
       	Comparator reference voltage select
       	This bit alone controls comparator reference voltage
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_VREF_1_5_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






























/* IOVB_RX_CTLE0_0_6_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x00000304)                                                     */
/*       BDX (0x00000304)                                                     */
/* Register default value:              0x0F                                  */
#define IOVB_RX_CTLE0_0_6_QPIAGENT_DFX_PHY_IOVB_REG 0x00000304

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_ctle_offset_en : 1;
    /* rx_ctle_offset_en - Bits[0:0], RWS, default = 1'b1 
        
       	1 = Enable Receive CTLE offset correction
       	0 = disable
       	Note: this is needed on a lane basis for HVM testing
             
     */
    UINT8 rx_ctle_peak : 5;
    /* rx_ctle_peak - Bits[5:1], RWS, default = 5'h7 
        
        Full speed encoded number to degeneration capacitance atoms to turn on in the 
       first stage of the CTLE 
       	00000 = Disable equalization, boost DC gain.
             
     */
    UINT8 rx_ctle_vcm_ovrd_dis : 1;
    /* rx_ctle_vcm_ovrd_dis - Bits[6:6], RWS, default = 1'b0 
        
       	1 = Disable CTLE vcm override
       	0= enable CTLE internal common mode
       	This is per bundle control.
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE0_0_6_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */
































/* IOVB_TX_CTL0_0_6_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x00000314)                                                     */
/*       BDX (0x00000314)                                                     */
/* Register default value:              0x01                                  */
#define IOVB_TX_CTL0_0_6_QPIAGENT_DFX_PHY_IOVB_REG 0x00000314

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX Control 0 Register
 */
typedef union {
  struct {
    UINT8 tx_align_en : 1;
    /* tx_align_en - Bits[0:0], RWS, default = 1'b1 
        
       	Enable TX data alignment (TX FIFO enable?)
             
     */
    UINT8 tx_align_en_ovr : 1;
    /* tx_align_en_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for tx_align_en
             
     */
    UINT8 tx_spare : 4;
    /* tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	tx_spare[3:1]: Reserved
       	tx_spare[0]: Icomp tracking mode:
        0= (default) open loop mode, open icomp loop after acquisition. An (specified 
       by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp determined 
       by bndl_icomp_gain 
             
     */
    UINT8 tx_lcl_icomp_dis : 1;
    /* tx_lcl_icomp_dis - Bits[6:6], RWS, default = 1'b0 
        
       	Local icomp en.
       	0 =start the local TX block icomp loop. Should be enabled for normal operation.
        1 =stop the local TX block icomp loop. For debug or HVM mode to check the RTerm 
       legs. 
        Note: This signal is overidden by the power-up sequence logic in the common 
       block (comdig) and will not affect the icomp loop until the icomp calibration 
       has completed. Need the local loop to accurately measure the legs. 
             
     */
    UINT8 tx_sense_lane : 1;
    /* tx_sense_lane - Bits[7:7], RWS, default = 1'b0 
        
       	Lane 0 identifier for QPI
       	0= this bundle does not contain QPI lane 0
       	1= this bundle contains QPI lane 0
        When bit= 1, the lane will not enter L1 state, but the lane can still be in L0s 
       and perform Rx Detect 
        (NOTE: This behavior is not entirely correct for QPI, so the QPI agent will not 
       use it but instead will simply not assert L0s on its lane 0.) 
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_CTL0_0_6_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






































/* IOVB_TXEQ_HALF_LVL0_0_6_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x00000327)                                                     */
/*       BDX (0x00000327)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL0_0_6_QPIAGENT_DFX_PHY_IOVB_REG 0x00000327

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl0 : 6;
    /* txeq_half_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL0_0_6_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */










/* IOVB_TX_VREF_0_6_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x0000032C)                                                     */
/*       BDX (0x0000032C)                                                     */
/* Register default value:              0x1E                                  */
#define IOVB_TX_VREF_0_6_QPIAGENT_DFX_PHY_IOVB_REG 0x0000032C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX VREF Register
 */
typedef union {
  struct {
    UINT8 tx_vref_sel : 5;
    /* tx_vref_sel - Bits[4:0], RWS, default = 5'h1E 
        
       	Comparator reference voltage select
       	This bit alone controls comparator reference voltage
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_VREF_0_6_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






























/* IOVB_RX_CTLE0_1_6_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x00000344)                                                     */
/*       BDX (0x00000344)                                                     */
/* Register default value:              0x0F                                  */
#define IOVB_RX_CTLE0_1_6_QPIAGENT_DFX_PHY_IOVB_REG 0x00000344

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_ctle_offset_en : 1;
    /* rx_ctle_offset_en - Bits[0:0], RWS, default = 1'b1 
        
       	1 = Enable Receive CTLE offset correction
       	0 = disable
       	Note: this is needed on a lane basis for HVM testing
             
     */
    UINT8 rx_ctle_peak : 5;
    /* rx_ctle_peak - Bits[5:1], RWS, default = 5'h7 
        
        Full speed encoded number to degeneration capacitance atoms to turn on in the 
       first stage of the CTLE 
       	00000 = Disable equalization, boost DC gain.
             
     */
    UINT8 rx_ctle_vcm_ovrd_dis : 1;
    /* rx_ctle_vcm_ovrd_dis - Bits[6:6], RWS, default = 1'b0 
        
       	1 = Disable CTLE vcm override
       	0= enable CTLE internal common mode
       	This is per bundle control.
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE0_1_6_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */
































/* IOVB_TX_CTL0_1_6_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x00000354)                                                     */
/*       BDX (0x00000354)                                                     */
/* Register default value:              0x01                                  */
#define IOVB_TX_CTL0_1_6_QPIAGENT_DFX_PHY_IOVB_REG 0x00000354

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX Control 0 Register
 */
typedef union {
  struct {
    UINT8 tx_align_en : 1;
    /* tx_align_en - Bits[0:0], RWS, default = 1'b1 
        
       	Enable TX data alignment (TX FIFO enable?)
             
     */
    UINT8 tx_align_en_ovr : 1;
    /* tx_align_en_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for tx_align_en
             
     */
    UINT8 tx_spare : 4;
    /* tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	tx_spare[3:1]: Reserved
       	tx_spare[0]: Icomp tracking mode:
        0= (default) open loop mode, open icomp loop after acquisition. An (specified 
       by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp determined 
       by bndl_icomp_gain 
             
     */
    UINT8 tx_lcl_icomp_dis : 1;
    /* tx_lcl_icomp_dis - Bits[6:6], RWS, default = 1'b0 
        
       	Local icomp en.
       	0 =start the local TX block icomp loop. Should be enabled for normal operation.
        1 =stop the local TX block icomp loop. For debug or HVM mode to check the RTerm 
       legs. 
        Note: This signal is overidden by the power-up sequence logic in the common 
       block (comdig) and will not affect the icomp loop until the icomp calibration 
       has completed. Need the local loop to accurately measure the legs. 
             
     */
    UINT8 tx_sense_lane : 1;
    /* tx_sense_lane - Bits[7:7], RWS, default = 1'b0 
        
       	Lane 0 identifier for QPI
       	0= this bundle does not contain QPI lane 0
       	1= this bundle contains QPI lane 0
        When bit= 1, the lane will not enter L1 state, but the lane can still be in L0s 
       and perform Rx Detect 
        (NOTE: This behavior is not entirely correct for QPI, so the QPI agent will not 
       use it but instead will simply not assert L0s on its lane 0.) 
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_CTL0_1_6_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






































/* IOVB_TXEQ_HALF_LVL0_1_6_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x00000367)                                                     */
/*       BDX (0x00000367)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL0_1_6_QPIAGENT_DFX_PHY_IOVB_REG 0x00000367

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl0 : 6;
    /* txeq_half_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL0_1_6_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */










/* IOVB_TX_VREF_1_6_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x0000036C)                                                     */
/*       BDX (0x0000036C)                                                     */
/* Register default value:              0x1E                                  */
#define IOVB_TX_VREF_1_6_QPIAGENT_DFX_PHY_IOVB_REG 0x0000036C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX VREF Register
 */
typedef union {
  struct {
    UINT8 tx_vref_sel : 5;
    /* tx_vref_sel - Bits[4:0], RWS, default = 5'h1E 
        
       	Comparator reference voltage select
       	This bit alone controls comparator reference voltage
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_VREF_1_6_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






























/* IOVB_RX_CTLE0_0_7_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x00000384)                                                     */
/*       BDX (0x00000384)                                                     */
/* Register default value:              0x0F                                  */
#define IOVB_RX_CTLE0_0_7_QPIAGENT_DFX_PHY_IOVB_REG 0x00000384

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_ctle_offset_en : 1;
    /* rx_ctle_offset_en - Bits[0:0], RWS, default = 1'b1 
        
       	1 = Enable Receive CTLE offset correction
       	0 = disable
       	Note: this is needed on a lane basis for HVM testing
             
     */
    UINT8 rx_ctle_peak : 5;
    /* rx_ctle_peak - Bits[5:1], RWS, default = 5'h7 
        
        Full speed encoded number to degeneration capacitance atoms to turn on in the 
       first stage of the CTLE 
       	00000 = Disable equalization, boost DC gain.
             
     */
    UINT8 rx_ctle_vcm_ovrd_dis : 1;
    /* rx_ctle_vcm_ovrd_dis - Bits[6:6], RWS, default = 1'b0 
        
       	1 = Disable CTLE vcm override
       	0= enable CTLE internal common mode
       	This is per bundle control.
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE0_0_7_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */
































/* IOVB_TX_CTL0_0_7_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x00000394)                                                     */
/*       BDX (0x00000394)                                                     */
/* Register default value:              0x01                                  */
#define IOVB_TX_CTL0_0_7_QPIAGENT_DFX_PHY_IOVB_REG 0x00000394

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX Control 0 Register
 */
typedef union {
  struct {
    UINT8 tx_align_en : 1;
    /* tx_align_en - Bits[0:0], RWS, default = 1'b1 
        
       	Enable TX data alignment (TX FIFO enable?)
             
     */
    UINT8 tx_align_en_ovr : 1;
    /* tx_align_en_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for tx_align_en
             
     */
    UINT8 tx_spare : 4;
    /* tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	tx_spare[3:1]: Reserved
       	tx_spare[0]: Icomp tracking mode:
        0= (default) open loop mode, open icomp loop after acquisition. An (specified 
       by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp determined 
       by bndl_icomp_gain 
             
     */
    UINT8 tx_lcl_icomp_dis : 1;
    /* tx_lcl_icomp_dis - Bits[6:6], RWS, default = 1'b0 
        
       	Local icomp en.
       	0 =start the local TX block icomp loop. Should be enabled for normal operation.
        1 =stop the local TX block icomp loop. For debug or HVM mode to check the RTerm 
       legs. 
        Note: This signal is overidden by the power-up sequence logic in the common 
       block (comdig) and will not affect the icomp loop until the icomp calibration 
       has completed. Need the local loop to accurately measure the legs. 
             
     */
    UINT8 tx_sense_lane : 1;
    /* tx_sense_lane - Bits[7:7], RWS, default = 1'b0 
        
       	Lane 0 identifier for QPI
       	0= this bundle does not contain QPI lane 0
       	1= this bundle contains QPI lane 0
        When bit= 1, the lane will not enter L1 state, but the lane can still be in L0s 
       and perform Rx Detect 
        (NOTE: This behavior is not entirely correct for QPI, so the QPI agent will not 
       use it but instead will simply not assert L0s on its lane 0.) 
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_CTL0_0_7_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






































/* IOVB_TXEQ_HALF_LVL0_0_7_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x000003A7)                                                     */
/*       BDX (0x000003A7)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL0_0_7_QPIAGENT_DFX_PHY_IOVB_REG 0x000003A7

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl0 : 6;
    /* txeq_half_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL0_0_7_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */










/* IOVB_TX_VREF_0_7_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x000003AC)                                                     */
/*       BDX (0x000003AC)                                                     */
/* Register default value:              0x1E                                  */
#define IOVB_TX_VREF_0_7_QPIAGENT_DFX_PHY_IOVB_REG 0x000003AC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX VREF Register
 */
typedef union {
  struct {
    UINT8 tx_vref_sel : 5;
    /* tx_vref_sel - Bits[4:0], RWS, default = 5'h1E 
        
       	Comparator reference voltage select
       	This bit alone controls comparator reference voltage
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_VREF_0_7_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






























/* IOVB_RX_CTLE0_1_7_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x000003C4)                                                     */
/*       BDX (0x000003C4)                                                     */
/* Register default value:              0x0F                                  */
#define IOVB_RX_CTLE0_1_7_QPIAGENT_DFX_PHY_IOVB_REG 0x000003C4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_ctle_offset_en : 1;
    /* rx_ctle_offset_en - Bits[0:0], RWS, default = 1'b1 
        
       	1 = Enable Receive CTLE offset correction
       	0 = disable
       	Note: this is needed on a lane basis for HVM testing
             
     */
    UINT8 rx_ctle_peak : 5;
    /* rx_ctle_peak - Bits[5:1], RWS, default = 5'h7 
        
        Full speed encoded number to degeneration capacitance atoms to turn on in the 
       first stage of the CTLE 
       	00000 = Disable equalization, boost DC gain.
             
     */
    UINT8 rx_ctle_vcm_ovrd_dis : 1;
    /* rx_ctle_vcm_ovrd_dis - Bits[6:6], RWS, default = 1'b0 
        
       	1 = Disable CTLE vcm override
       	0= enable CTLE internal common mode
       	This is per bundle control.
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE0_1_7_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */
































/* IOVB_TX_CTL0_1_7_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x000003D4)                                                     */
/*       BDX (0x000003D4)                                                     */
/* Register default value:              0x01                                  */
#define IOVB_TX_CTL0_1_7_QPIAGENT_DFX_PHY_IOVB_REG 0x000003D4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX Control 0 Register
 */
typedef union {
  struct {
    UINT8 tx_align_en : 1;
    /* tx_align_en - Bits[0:0], RWS, default = 1'b1 
        
       	Enable TX data alignment (TX FIFO enable?)
             
     */
    UINT8 tx_align_en_ovr : 1;
    /* tx_align_en_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for tx_align_en
             
     */
    UINT8 tx_spare : 4;
    /* tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	tx_spare[3:1]: Reserved
       	tx_spare[0]: Icomp tracking mode:
        0= (default) open loop mode, open icomp loop after acquisition. An (specified 
       by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp determined 
       by bndl_icomp_gain 
             
     */
    UINT8 tx_lcl_icomp_dis : 1;
    /* tx_lcl_icomp_dis - Bits[6:6], RWS, default = 1'b0 
        
       	Local icomp en.
       	0 =start the local TX block icomp loop. Should be enabled for normal operation.
        1 =stop the local TX block icomp loop. For debug or HVM mode to check the RTerm 
       legs. 
        Note: This signal is overidden by the power-up sequence logic in the common 
       block (comdig) and will not affect the icomp loop until the icomp calibration 
       has completed. Need the local loop to accurately measure the legs. 
             
     */
    UINT8 tx_sense_lane : 1;
    /* tx_sense_lane - Bits[7:7], RWS, default = 1'b0 
        
       	Lane 0 identifier for QPI
       	0= this bundle does not contain QPI lane 0
       	1= this bundle contains QPI lane 0
        When bit= 1, the lane will not enter L1 state, but the lane can still be in L0s 
       and perform Rx Detect 
        (NOTE: This behavior is not entirely correct for QPI, so the QPI agent will not 
       use it but instead will simply not assert L0s on its lane 0.) 
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_CTL0_1_7_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






































/* IOVB_TXEQ_HALF_LVL0_1_7_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x000003E7)                                                     */
/*       BDX (0x000003E7)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL0_1_7_QPIAGENT_DFX_PHY_IOVB_REG 0x000003E7

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl0 : 6;
    /* txeq_half_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL0_1_7_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */










/* IOVB_TX_VREF_1_7_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x000003EC)                                                     */
/*       BDX (0x000003EC)                                                     */
/* Register default value:              0x1E                                  */
#define IOVB_TX_VREF_1_7_QPIAGENT_DFX_PHY_IOVB_REG 0x000003EC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX VREF Register
 */
typedef union {
  struct {
    UINT8 tx_vref_sel : 5;
    /* tx_vref_sel - Bits[4:0], RWS, default = 5'h1E 
        
       	Comparator reference voltage select
       	This bit alone controls comparator reference voltage
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_VREF_1_7_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






























/* IOVB_RX_CTLE0_0_8_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x00000404)                                                     */
/*       BDX (0x00000404)                                                     */
/* Register default value:              0x0F                                  */
#define IOVB_RX_CTLE0_0_8_QPIAGENT_DFX_PHY_IOVB_REG 0x00000404

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_ctle_offset_en : 1;
    /* rx_ctle_offset_en - Bits[0:0], RWS, default = 1'b1 
        
       	1 = Enable Receive CTLE offset correction
       	0 = disable
       	Note: this is needed on a lane basis for HVM testing
             
     */
    UINT8 rx_ctle_peak : 5;
    /* rx_ctle_peak - Bits[5:1], RWS, default = 5'h7 
        
        Full speed encoded number to degeneration capacitance atoms to turn on in the 
       first stage of the CTLE 
       	00000 = Disable equalization, boost DC gain.
             
     */
    UINT8 rx_ctle_vcm_ovrd_dis : 1;
    /* rx_ctle_vcm_ovrd_dis - Bits[6:6], RWS, default = 1'b0 
        
       	1 = Disable CTLE vcm override
       	0= enable CTLE internal common mode
       	This is per bundle control.
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE0_0_8_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */
































/* IOVB_TX_CTL0_0_8_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x00000414)                                                     */
/*       BDX (0x00000414)                                                     */
/* Register default value:              0x01                                  */
#define IOVB_TX_CTL0_0_8_QPIAGENT_DFX_PHY_IOVB_REG 0x00000414

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX Control 0 Register
 */
typedef union {
  struct {
    UINT8 tx_align_en : 1;
    /* tx_align_en - Bits[0:0], RWS, default = 1'b1 
        
       	Enable TX data alignment (TX FIFO enable?)
             
     */
    UINT8 tx_align_en_ovr : 1;
    /* tx_align_en_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for tx_align_en
             
     */
    UINT8 tx_spare : 4;
    /* tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	tx_spare[3:1]: Reserved
       	tx_spare[0]: Icomp tracking mode:
        0= (default) open loop mode, open icomp loop after acquisition. An (specified 
       by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp determined 
       by bndl_icomp_gain 
             
     */
    UINT8 tx_lcl_icomp_dis : 1;
    /* tx_lcl_icomp_dis - Bits[6:6], RWS, default = 1'b0 
        
       	Local icomp en.
       	0 =start the local TX block icomp loop. Should be enabled for normal operation.
        1 =stop the local TX block icomp loop. For debug or HVM mode to check the RTerm 
       legs. 
        Note: This signal is overidden by the power-up sequence logic in the common 
       block (comdig) and will not affect the icomp loop until the icomp calibration 
       has completed. Need the local loop to accurately measure the legs. 
             
     */
    UINT8 tx_sense_lane : 1;
    /* tx_sense_lane - Bits[7:7], RWS, default = 1'b0 
        
       	Lane 0 identifier for QPI
       	0= this bundle does not contain QPI lane 0
       	1= this bundle contains QPI lane 0
        When bit= 1, the lane will not enter L1 state, but the lane can still be in L0s 
       and perform Rx Detect 
        (NOTE: This behavior is not entirely correct for QPI, so the QPI agent will not 
       use it but instead will simply not assert L0s on its lane 0.) 
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_CTL0_0_8_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






































/* IOVB_TXEQ_HALF_LVL0_0_8_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x00000427)                                                     */
/*       BDX (0x00000427)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL0_0_8_QPIAGENT_DFX_PHY_IOVB_REG 0x00000427

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl0 : 6;
    /* txeq_half_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL0_0_8_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */










/* IOVB_TX_VREF_0_8_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x0000042C)                                                     */
/*       BDX (0x0000042C)                                                     */
/* Register default value:              0x1E                                  */
#define IOVB_TX_VREF_0_8_QPIAGENT_DFX_PHY_IOVB_REG 0x0000042C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX VREF Register
 */
typedef union {
  struct {
    UINT8 tx_vref_sel : 5;
    /* tx_vref_sel - Bits[4:0], RWS, default = 5'h1E 
        
       	Comparator reference voltage select
       	This bit alone controls comparator reference voltage
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_VREF_0_8_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






























/* IOVB_RX_CTLE0_1_8_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x00000444)                                                     */
/*       BDX (0x00000444)                                                     */
/* Register default value:              0x0F                                  */
#define IOVB_RX_CTLE0_1_8_QPIAGENT_DFX_PHY_IOVB_REG 0x00000444

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_ctle_offset_en : 1;
    /* rx_ctle_offset_en - Bits[0:0], RWS, default = 1'b1 
        
       	1 = Enable Receive CTLE offset correction
       	0 = disable
       	Note: this is needed on a lane basis for HVM testing
             
     */
    UINT8 rx_ctle_peak : 5;
    /* rx_ctle_peak - Bits[5:1], RWS, default = 5'h7 
        
        Full speed encoded number to degeneration capacitance atoms to turn on in the 
       first stage of the CTLE 
       	00000 = Disable equalization, boost DC gain.
             
     */
    UINT8 rx_ctle_vcm_ovrd_dis : 1;
    /* rx_ctle_vcm_ovrd_dis - Bits[6:6], RWS, default = 1'b0 
        
       	1 = Disable CTLE vcm override
       	0= enable CTLE internal common mode
       	This is per bundle control.
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE0_1_8_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */
































/* IOVB_TX_CTL0_1_8_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x00000454)                                                     */
/*       BDX (0x00000454)                                                     */
/* Register default value:              0x01                                  */
#define IOVB_TX_CTL0_1_8_QPIAGENT_DFX_PHY_IOVB_REG 0x00000454

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX Control 0 Register
 */
typedef union {
  struct {
    UINT8 tx_align_en : 1;
    /* tx_align_en - Bits[0:0], RWS, default = 1'b1 
        
       	Enable TX data alignment (TX FIFO enable?)
             
     */
    UINT8 tx_align_en_ovr : 1;
    /* tx_align_en_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for tx_align_en
             
     */
    UINT8 tx_spare : 4;
    /* tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	tx_spare[3:1]: Reserved
       	tx_spare[0]: Icomp tracking mode:
        0= (default) open loop mode, open icomp loop after acquisition. An (specified 
       by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp determined 
       by bndl_icomp_gain 
             
     */
    UINT8 tx_lcl_icomp_dis : 1;
    /* tx_lcl_icomp_dis - Bits[6:6], RWS, default = 1'b0 
        
       	Local icomp en.
       	0 =start the local TX block icomp loop. Should be enabled for normal operation.
        1 =stop the local TX block icomp loop. For debug or HVM mode to check the RTerm 
       legs. 
        Note: This signal is overidden by the power-up sequence logic in the common 
       block (comdig) and will not affect the icomp loop until the icomp calibration 
       has completed. Need the local loop to accurately measure the legs. 
             
     */
    UINT8 tx_sense_lane : 1;
    /* tx_sense_lane - Bits[7:7], RWS, default = 1'b0 
        
       	Lane 0 identifier for QPI
       	0= this bundle does not contain QPI lane 0
       	1= this bundle contains QPI lane 0
        When bit= 1, the lane will not enter L1 state, but the lane can still be in L0s 
       and perform Rx Detect 
        (NOTE: This behavior is not entirely correct for QPI, so the QPI agent will not 
       use it but instead will simply not assert L0s on its lane 0.) 
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_CTL0_1_8_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






































/* IOVB_TXEQ_HALF_LVL0_1_8_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x00000467)                                                     */
/*       BDX (0x00000467)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL0_1_8_QPIAGENT_DFX_PHY_IOVB_REG 0x00000467

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl0 : 6;
    /* txeq_half_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL0_1_8_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */










/* IOVB_TX_VREF_1_8_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x0000046C)                                                     */
/*       BDX (0x0000046C)                                                     */
/* Register default value:              0x1E                                  */
#define IOVB_TX_VREF_1_8_QPIAGENT_DFX_PHY_IOVB_REG 0x0000046C

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX VREF Register
 */
typedef union {
  struct {
    UINT8 tx_vref_sel : 5;
    /* tx_vref_sel - Bits[4:0], RWS, default = 5'h1E 
        
       	Comparator reference voltage select
       	This bit alone controls comparator reference voltage
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_VREF_1_8_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






























/* IOVB_RX_CTLE0_0_9_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x00000484)                                                     */
/*       BDX (0x00000484)                                                     */
/* Register default value:              0x0F                                  */
#define IOVB_RX_CTLE0_0_9_QPIAGENT_DFX_PHY_IOVB_REG 0x00000484

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_ctle_offset_en : 1;
    /* rx_ctle_offset_en - Bits[0:0], RWS, default = 1'b1 
        
       	1 = Enable Receive CTLE offset correction
       	0 = disable
       	Note: this is needed on a lane basis for HVM testing
             
     */
    UINT8 rx_ctle_peak : 5;
    /* rx_ctle_peak - Bits[5:1], RWS, default = 5'h7 
        
        Full speed encoded number to degeneration capacitance atoms to turn on in the 
       first stage of the CTLE 
       	00000 = Disable equalization, boost DC gain.
             
     */
    UINT8 rx_ctle_vcm_ovrd_dis : 1;
    /* rx_ctle_vcm_ovrd_dis - Bits[6:6], RWS, default = 1'b0 
        
       	1 = Disable CTLE vcm override
       	0= enable CTLE internal common mode
       	This is per bundle control.
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE0_0_9_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */
































/* IOVB_TX_CTL0_0_9_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x00000494)                                                     */
/*       BDX (0x00000494)                                                     */
/* Register default value:              0x01                                  */
#define IOVB_TX_CTL0_0_9_QPIAGENT_DFX_PHY_IOVB_REG 0x00000494

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX Control 0 Register
 */
typedef union {
  struct {
    UINT8 tx_align_en : 1;
    /* tx_align_en - Bits[0:0], RWS, default = 1'b1 
        
       	Enable TX data alignment (TX FIFO enable?)
             
     */
    UINT8 tx_align_en_ovr : 1;
    /* tx_align_en_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for tx_align_en
             
     */
    UINT8 tx_spare : 4;
    /* tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	tx_spare[3:1]: Reserved
       	tx_spare[0]: Icomp tracking mode:
        0= (default) open loop mode, open icomp loop after acquisition. An (specified 
       by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp determined 
       by bndl_icomp_gain 
             
     */
    UINT8 tx_lcl_icomp_dis : 1;
    /* tx_lcl_icomp_dis - Bits[6:6], RWS, default = 1'b0 
        
       	Local icomp en.
       	0 =start the local TX block icomp loop. Should be enabled for normal operation.
        1 =stop the local TX block icomp loop. For debug or HVM mode to check the RTerm 
       legs. 
        Note: This signal is overidden by the power-up sequence logic in the common 
       block (comdig) and will not affect the icomp loop until the icomp calibration 
       has completed. Need the local loop to accurately measure the legs. 
             
     */
    UINT8 tx_sense_lane : 1;
    /* tx_sense_lane - Bits[7:7], RWS, default = 1'b0 
        
       	Lane 0 identifier for QPI
       	0= this bundle does not contain QPI lane 0
       	1= this bundle contains QPI lane 0
        When bit= 1, the lane will not enter L1 state, but the lane can still be in L0s 
       and perform Rx Detect 
        (NOTE: This behavior is not entirely correct for QPI, so the QPI agent will not 
       use it but instead will simply not assert L0s on its lane 0.) 
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_CTL0_0_9_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






































/* IOVB_TXEQ_HALF_LVL0_0_9_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x000004A7)                                                     */
/*       BDX (0x000004A7)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL0_0_9_QPIAGENT_DFX_PHY_IOVB_REG 0x000004A7

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl0 : 6;
    /* txeq_half_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL0_0_9_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */










/* IOVB_TX_VREF_0_9_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x000004AC)                                                     */
/*       BDX (0x000004AC)                                                     */
/* Register default value:              0x1E                                  */
#define IOVB_TX_VREF_0_9_QPIAGENT_DFX_PHY_IOVB_REG 0x000004AC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX VREF Register
 */
typedef union {
  struct {
    UINT8 tx_vref_sel : 5;
    /* tx_vref_sel - Bits[4:0], RWS, default = 5'h1E 
        
       	Comparator reference voltage select
       	This bit alone controls comparator reference voltage
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_VREF_0_9_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






























/* IOVB_RX_CTLE0_1_9_QPIAGENT_DFX_PHY_IOVB_REG supported on:                  */
/*       HSX (0x000004C4)                                                     */
/*       BDX (0x000004C4)                                                     */
/* Register default value:              0x0F                                  */
#define IOVB_RX_CTLE0_1_9_QPIAGENT_DFX_PHY_IOVB_REG 0x000004C4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB RX CTLE control 0 Register
 */
typedef union {
  struct {
    UINT8 rx_ctle_offset_en : 1;
    /* rx_ctle_offset_en - Bits[0:0], RWS, default = 1'b1 
        
       	1 = Enable Receive CTLE offset correction
       	0 = disable
       	Note: this is needed on a lane basis for HVM testing
             
     */
    UINT8 rx_ctle_peak : 5;
    /* rx_ctle_peak - Bits[5:1], RWS, default = 5'h7 
        
        Full speed encoded number to degeneration capacitance atoms to turn on in the 
       first stage of the CTLE 
       	00000 = Disable equalization, boost DC gain.
             
     */
    UINT8 rx_ctle_vcm_ovrd_dis : 1;
    /* rx_ctle_vcm_ovrd_dis - Bits[6:6], RWS, default = 1'b0 
        
       	1 = Disable CTLE vcm override
       	0= enable CTLE internal common mode
       	This is per bundle control.
             
     */
    UINT8 rsvd : 1;
    /* rsvd - Bits[7:7], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_RX_CTLE0_1_9_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */
































/* IOVB_TX_CTL0_1_9_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x000004D4)                                                     */
/*       BDX (0x000004D4)                                                     */
/* Register default value:              0x01                                  */
#define IOVB_TX_CTL0_1_9_QPIAGENT_DFX_PHY_IOVB_REG 0x000004D4

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX Control 0 Register
 */
typedef union {
  struct {
    UINT8 tx_align_en : 1;
    /* tx_align_en - Bits[0:0], RWS, default = 1'b1 
        
       	Enable TX data alignment (TX FIFO enable?)
             
     */
    UINT8 tx_align_en_ovr : 1;
    /* tx_align_en_ovr - Bits[1:1], RWS, default = 1'b0 
        
       	override enable for tx_align_en
             
     */
    UINT8 tx_spare : 4;
    /* tx_spare - Bits[5:2], RWS, default = 4'b0 
        
       	tx_spare[3:1]: Reserved
       	tx_spare[0]: Icomp tracking mode:
        0= (default) open loop mode, open icomp loop after acquisition. An (specified 
       by bndl_icomp_gain) can be added to the locked icomp code 
        1 = tracking mode, icomp loop continues on after acquisition. Icomp determined 
       by bndl_icomp_gain 
             
     */
    UINT8 tx_lcl_icomp_dis : 1;
    /* tx_lcl_icomp_dis - Bits[6:6], RWS, default = 1'b0 
        
       	Local icomp en.
       	0 =start the local TX block icomp loop. Should be enabled for normal operation.
        1 =stop the local TX block icomp loop. For debug or HVM mode to check the RTerm 
       legs. 
        Note: This signal is overidden by the power-up sequence logic in the common 
       block (comdig) and will not affect the icomp loop until the icomp calibration 
       has completed. Need the local loop to accurately measure the legs. 
             
     */
    UINT8 tx_sense_lane : 1;
    /* tx_sense_lane - Bits[7:7], RWS, default = 1'b0 
        
       	Lane 0 identifier for QPI
       	0= this bundle does not contain QPI lane 0
       	1= this bundle contains QPI lane 0
        When bit= 1, the lane will not enter L1 state, but the lane can still be in L0s 
       and perform Rx Detect 
        (NOTE: This behavior is not entirely correct for QPI, so the QPI agent will not 
       use it but instead will simply not assert L0s on its lane 0.) 
       	Note: This is a bundle level signal
             
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_CTL0_1_9_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */






































/* IOVB_TXEQ_HALF_LVL0_1_9_QPIAGENT_DFX_PHY_IOVB_REG supported on:            */
/*       HSX (0x000004E7)                                                     */
/*       BDX (0x000004E7)                                                     */
/* Register default value:              0x3F                                  */
#define IOVB_TXEQ_HALF_LVL0_1_9_QPIAGENT_DFX_PHY_IOVB_REG 0x000004E7

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB Transmit Equalization Half Speed Level0 Cofficients
 */
typedef union {
  struct {
    UINT8 txeq_half_lvl0 : 6;
    /* txeq_half_lvl0 - Bits[5:0], RWS, default = 6'h3F 
        
       	Transmit Equalization Half Speed Level0 coefficients for FIR settings
       	See UniPHY MAS tables for settings.
        NOTE: The CSRs controlling txeq_lvl* have no effect in Slow Mode (QPI), during 
       which the QPI agent hardwires the value to 3Fh. 
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TXEQ_HALF_LVL0_1_9_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */










/* IOVB_TX_VREF_1_9_QPIAGENT_DFX_PHY_IOVB_REG supported on:                   */
/*       HSX (0x000004EC)                                                     */
/*       BDX (0x000004EC)                                                     */
/* Register default value:              0x1E                                  */
#define IOVB_TX_VREF_1_9_QPIAGENT_DFX_PHY_IOVB_REG 0x000004EC

#if defined(HSX_HOST) || defined(BDX_HOST)
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.8.4.WINDOW_0.xml.
 * IOVB TX VREF Register
 */
typedef union {
  struct {
    UINT8 tx_vref_sel : 5;
    /* tx_vref_sel - Bits[4:0], RWS, default = 5'h1E 
        
       	Comparator reference voltage select
       	This bit alone controls comparator reference voltage
       	Note: This is a bundle level signal
             
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} IOVB_TX_VREF_1_9_QPIAGENT_DFX_PHY_IOVB_STRUCT;
#endif /* ASM_INC */
#endif /* defined(HSX_HOST) || defined(BDX_HOST) */
























#endif /* QPIAGENT_DFX_PHY_IOVB_h */
