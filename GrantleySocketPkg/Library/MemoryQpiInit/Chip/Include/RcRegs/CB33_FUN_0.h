/* Date Stamp: 8/23/2014 */

#ifndef CB33_FUN_0_h
#define CB33_FUN_0_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For BDX_HOST:                                                              */
/* CB33_FUN_0_BDX_DEV 0                                                       */
/* CB33_FUN_0_BDX_FUN 0                                                       */

/* VID_CB33_FUN_0_REG supported on:                                           */
/*       BDX (0x20200000)                                                     */
/* Register default value:              0x8086                                */
#define VID_CB33_FUN_0_REG 0x18002000

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * The Vendor ID register identifies the Crystal Beach device as an Intel device 
 * and adheres to the PCI Local Bus Specification, Revision 3.0. Since this 
 * register is part of the standard PCI header, there is a VID register per PCI 
 * function. 
 */
typedef union {
  struct {
    UINT16 vid : 16;
    /* vid - Bits[15:0], RO, default = 16'h8086 
       The Vendor ID register identifies the Crystal Beach device as an Intel device 
       and adheres to the PCI Local Bus Specification, Revision 3.0. Since this 
       register is part of the standard PCI header, there is a VID register per PCI 
       function. 
     */
  } Bits;
  UINT16 Data;
} VID_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DID_CB33_FUN_0_REG supported on:                                           */
/*       BDX (0x20200002)                                                     */
/* Register default value:              0x0C50                                */
#define DID_CB33_FUN_0_REG 0x18002002

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * The Device ID register identifies the Crystal Beach device and adheres to the 
 * PCI Local Bus Specification, Revision 3.0. 
 */
typedef union {
  struct {
    UINT16 did : 16;
    /* did - Bits[15:0], ROS_V, default = 16'h0C50 
       The Device ID register identifies the Crystal Beach device and adheres to the 
       PCI Local Bus Specification, Revision 3.0. 
     */
  } Bits;
  UINT16 Data;
} DID_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* PCICMD_CB33_FUN_0_REG supported on:                                        */
/*       BDX (0x20200004)                                                     */
/* Register default value:              0x0000                                */
#define PCICMD_CB33_FUN_0_REG 0x18002004

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * The PCI Command register follows a subset of the PCI Local Bus Specification, 
 * Revision 3.0 specification. This register provides the basic control of the 
 * ability of the Crystal Beach device to initiate and respond to transactions sent 
 * to it and maintains compatibility with PCI configuration space. 
 */
typedef union {
  struct {
    UINT16 iose : 1;
    /* iose - Bits[0:0], RO, default = 1'b0 
       The PCI Command register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register provides the basic control of the 
       ability of the Crystal Beach device to initiate and respond to transactions sent 
       to it and maintains compatibility with PCI configuration space. 
     */
    UINT16 mse : 1;
    /* mse - Bits[1:1], RWS, default = 1'b0 
       The PCI Command register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register provides the basic control of the 
       ability of the Crystal Beach device to initiate and respond to transactions sent 
       to it and maintains compatibility with PCI configuration space. 
     */
    UINT16 bme : 1;
    /* bme - Bits[2:2], RWS, default = 1'b0 
       The PCI Command register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register provides the basic control of the 
       ability of the Crystal Beach device to initiate and respond to transactions sent 
       to it and maintains compatibility with PCI configuration space. 
     */
    UINT16 sce : 1;
    /* sce - Bits[3:3], RWS, default = 1'b0 
       The PCI Command register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register provides the basic control of the 
       ability of the Crystal Beach device to initiate and respond to transactions sent 
       to it and maintains compatibility with PCI configuration space. 
     */
    UINT16 mwie : 1;
    /* mwie - Bits[4:4], RO, default = 1'b0 
       The PCI Command register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register provides the basic control of the 
       ability of the Crystal Beach device to initiate and respond to transactions sent 
       to it and maintains compatibility with PCI configuration space. 
     */
    UINT16 vga_pse : 1;
    /* vga_pse - Bits[5:5], RO, default = 1'b0 
       The PCI Command register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register provides the basic control of the 
       ability of the Crystal Beach device to initiate and respond to transactions sent 
       to it and maintains compatibility with PCI configuration space. 
     */
    UINT16 pere : 1;
    /* pere - Bits[6:6], RWS, default = 1'b0 
       The PCI Command register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register provides the basic control of the 
       ability of the Crystal Beach device to initiate and respond to transactions sent 
       to it and maintains compatibility with PCI configuration space. 
     */
    UINT16 wcc : 1;
    /* wcc - Bits[7:7], RO, default = 1'b0 
       The PCI Command register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register provides the basic control of the 
       ability of the Crystal Beach device to initiate and respond to transactions sent 
       to it and maintains compatibility with PCI configuration space. 
     */
    UINT16 see : 1;
    /* see - Bits[8:8], RWS, default = 1'b0 
       The PCI Command register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register provides the basic control of the 
       ability of the Crystal Beach device to initiate and respond to transactions sent 
       to it and maintains compatibility with PCI configuration space. 
     */
    UINT16 fbe : 1;
    /* fbe - Bits[9:9], RO, default = 1'b0 
       The PCI Command register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register provides the basic control of the 
       ability of the Crystal Beach device to initiate and respond to transactions sent 
       to it and maintains compatibility with PCI configuration space. 
     */
    UINT16 intxd : 1;
    /* intxd - Bits[10:10], RWS, default = 1'b0 
       The PCI Command register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register provides the basic control of the 
       ability of the Crystal Beach device to initiate and respond to transactions sent 
       to it and maintains compatibility with PCI configuration space. 
     */
    UINT16 rsvd : 5;
    /* rsvd - Bits[15:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT16 Data;
} PCICMD_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* PCISTS_CB33_FUN_0_REG supported on:                                        */
/*       BDX (0x20200006)                                                     */
/* Register default value:              0x0010                                */
#define PCISTS_CB33_FUN_0_REG 0x18002006

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * The PCI Status register follows a subset of the PCI Local Bus Specification, 
 * Revision 3.0 specification. This register maintains compatibility with PCI 
 * configuration space. Since this register is part of the standard PCI header, 
 * there is a PCISTS register per PCI function. 
 */
typedef union {
  struct {
    UINT16 rsvd_0 : 3;
    /* rsvd_0 - Bits[2:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 ints : 1;
    /* ints - Bits[3:3], ROS_V, default = 1'b0 
       The PCI Status register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register maintains compatibility with PCI 
       configuration space. Since this register is part of the standard PCI header, 
       there is a PCISTS register per PCI function. 
     */
    UINT16 cape : 1;
    /* cape - Bits[4:4], RO, default = 1'b1 
       The PCI Status register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register maintains compatibility with PCI 
       configuration space. Since this register is part of the standard PCI header, 
       there is a PCISTS register per PCI function. 
     */
    UINT16 c66 : 1;
    /* c66 - Bits[5:5], RO, default = 1'b0 
       The PCI Status register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register maintains compatibility with PCI 
       configuration space. Since this register is part of the standard PCI header, 
       there is a PCISTS register per PCI function. 
     */
    UINT16 rsvd_6 : 1;
    /* rsvd_6 - Bits[6:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 fbc : 1;
    /* fbc - Bits[7:7], RO, default = 1'b0 
       The PCI Status register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register maintains compatibility with PCI 
       configuration space. Since this register is part of the standard PCI header, 
       there is a PCISTS register per PCI function. 
     */
    UINT16 mdpd : 1;
    /* mdpd - Bits[8:8], RW1CS, default = 1'b0 
       The PCI Status register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register maintains compatibility with PCI 
       configuration space. Since this register is part of the standard PCI header, 
       there is a PCISTS register per PCI function. 
     */
    UINT16 rsvd_9 : 2;
    /* rsvd_9 - Bits[10:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 sta : 1;
    /* sta - Bits[11:11], RW1CS, default = 1'b0 
       The PCI Status register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register maintains compatibility with PCI 
       configuration space. Since this register is part of the standard PCI header, 
       there is a PCISTS register per PCI function. 
     */
    UINT16 rta : 1;
    /* rta - Bits[12:12], RW1CS, default = 1'b0 
       The PCI Status register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register maintains compatibility with PCI 
       configuration space. Since this register is part of the standard PCI header, 
       there is a PCISTS register per PCI function. 
     */
    UINT16 rma : 1;
    /* rma - Bits[13:13], RW1CS, default = 1'b0 
       The PCI Status register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register maintains compatibility with PCI 
       configuration space. Since this register is part of the standard PCI header, 
       there is a PCISTS register per PCI function. 
     */
    UINT16 sse : 1;
    /* sse - Bits[14:14], RW1CS, default = 1'b0 
       The PCI Status register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register maintains compatibility with PCI 
       configuration space. Since this register is part of the standard PCI header, 
       there is a PCISTS register per PCI function. 
     */
    UINT16 dpe : 1;
    /* dpe - Bits[15:15], RW1CS, default = 1'b0 
       The PCI Status register follows a subset of the PCI Local Bus Specification, 
       Revision 3.0 specification. This register maintains compatibility with PCI 
       configuration space. Since this register is part of the standard PCI header, 
       there is a PCISTS register per PCI function. 
     */
  } Bits;
  UINT16 Data;
} PCISTS_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* RID_CB33_FUN_0_REG supported on:                                           */
/*       BDX (0x10200008)                                                     */
/* Register default value:              0x00                                  */
#define RID_CB33_FUN_0_REG 0x18001008

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * The Revision ID register tracks the specific revision of this component. Also 
 * see Section 34.4.2.6, 'CB Version Register (CBVER)' on page 1845, which 
 * indicates software compatibility level. 
 */
typedef union {
  struct {
    UINT8 rid : 8;
    /* rid - Bits[7:0], ROS_V, default = 8'h00 
       The Revision ID register tracks the specific revision of this component. Also 
       see Section 34.4.2.6, 'CB Version Register (CBVER)' on page 1845, which 
       indicates software compatibility level. 
     */
  } Bits;
  UINT8 Data;
} RID_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* CCR_N0_CB33_FUN_0_REG supported on:                                        */
/*       BDX (0x30200009)                                                     */
/* Register default value:              error                                 */
#define CCR_N0_CB33_FUN_0_REG 0x18003009

#ifdef BDX_HOST
#endif /*BDX_HOST */




/* HDR_CB33_FUN_0_REG supported on:                                           */
/*       BDX (0x1020000E)                                                     */
/* Register default value:              0x80                                  */
#define HDR_CB33_FUN_0_REG 0x1800100E

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * The Crystal Beach device follows the standard PCI Configuration space header 
 * format and maintains seven functions. This register adheres to the PCI Local Bus 
 * Specification, Revision 3.0. Since this register is part of the standard PCI 
 * header, there is an HDR register per PCI function. 
 */
typedef union {
  struct {
    UINT8 ht : 7;
    /* ht - Bits[6:0], RO, default = 7'h00 
       The Crystal Beach device follows the standard PCI Configuration space header 
       format and maintains seven functions. This register adheres to the PCI Local Bus 
       Specification, Revision 3.0. Since this register is part of the standard PCI 
       header, there is an HDR register per PCI function. 
     */
    UINT8 mfd : 1;
    /* mfd - Bits[7:7], RO, default = 1'b1 
       The Crystal Beach device follows the standard PCI Configuration space header 
       format and maintains seven functions. This register adheres to the PCI Local Bus 
       Specification, Revision 3.0. Since this register is part of the standard PCI 
       header, there is an HDR register per PCI function. 
     */
  } Bits;
  UINT8 Data;
} HDR_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* CBBAR_N0_CB33_FUN_0_REG supported on:                                      */
/*       BDX (0x40200010)                                                     */
/* Register default value:              0x00000004                            */
#define CBBAR_N0_CB33_FUN_0_REG 0x18004010

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * 64The CBBAR register will be shadowed internally via sideband to the RTF cluster 
 * for address decode purposes. This will be software invisible but is required for 
 * routing the transactions to the CBDMA from the fabric interface. 
 */
typedef union {
  struct {
    UINT32 msi : 1;
    /* msi - Bits[0:0], RO, default = 1'b0 
       64The CBBAR register will be shadowed internally via sideband to the RTF cluster 
       for address decode purposes. This will be software invisible but is required for 
       routing the transactions to the CBDMA from the fabric interface. 
     */
    UINT32 type : 2;
    /* type - Bits[2:1], RO, default = 2'b10 
       64The CBBAR register will be shadowed internally via sideband to the RTF cluster 
       for address decode purposes. This will be software invisible but is required for 
       routing the transactions to the CBDMA from the fabric interface. 
     */
    UINT32 pref : 1;
    /* pref - Bits[3:3], RO, default = 1'b0 
       64The CBBAR register will be shadowed internally via sideband to the RTF cluster 
       for address decode purposes. This will be software invisible but is required for 
       routing the transactions to the CBDMA from the fabric interface. 
     */
    UINT32 rsvd : 9;
    /* rsvd - Bits[12:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 baddr : 19;
    /* baddr - Bits[31:13], RWS, default = 51'h0 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       64The CBBAR register will be shadowed internally via sideband to the RTF cluster 
       for address decode purposes. This will be software invisible but is required for 
       routing the transactions to the CBDMA from the fabric interface. 
     */
  } Bits;
  UINT32 Data;
} CBBAR_N0_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* CBBAR_N1_CB33_FUN_0_REG supported on:                                      */
/*       BDX (0x40200014)                                                     */
/* Register default value:              0x00000000                            */
#define CBBAR_N1_CB33_FUN_0_REG 0x18004014

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * 64The CBBAR register will be shadowed internally via sideband to the RTF cluster 
 * for address decode purposes. This will be software invisible but is required for 
 * routing the transactions to the CBDMA from the fabric interface. 
 */
typedef union {
  struct {
    UINT32 baddr : 32;
    /* baddr - Bits[31:0], RWS, default = 51'h0 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       64The CBBAR register will be shadowed internally via sideband to the RTF cluster 
       for address decode purposes. This will be software invisible but is required for 
       routing the transactions to the CBDMA from the fabric interface. 
     */
  } Bits;
  UINT32 Data;
} CBBAR_N1_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* SVID_CB33_FUN_0_REG supported on:                                          */
/*       BDX (0x2020002C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_CB33_FUN_0_REG 0x1800202C

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * This register identifies the manufacturer of the system. This 16-bit register 
 * combined with the Device Identification Register uniquely identify any PCI 
 * device. It is present in an integrated endpoint in the PCI space. 
 */
typedef union {
  struct {
    UINT16 svid : 16;
    /* svid - Bits[15:0], RWS_O, default = 16'h8086 
       This register identifies the manufacturer of the system. This 16-bit register 
       combined with the Device Identification Register uniquely identify any PCI 
       device. It is present in an integrated endpoint in the PCI space. 
     */
  } Bits;
  UINT16 Data;
} SVID_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* SID_CB33_FUN_0_REG supported on:                                           */
/*       BDX (0x2020002E)                                                     */
/* Register default value:              0x0000                                */
#define SID_CB33_FUN_0_REG 0x1800202E

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * The Subsystem ID register adheres to the PCI Local Bus Specification, Revision 
 * 3.0. and identifies the subsystem of the CBDMA device. 
 */
typedef union {
  struct {
    UINT16 sid : 16;
    /* sid - Bits[15:0], RWS_O, default = 16'h0 
       The Subsystem ID register adheres to the PCI Local Bus Specification, Revision 
       3.0. and identifies the subsystem of the CBDMA device. 
     */
  } Bits;
  UINT16 Data;
} SID_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* CAPPTR_CB33_FUN_0_REG supported on:                                        */
/*       BDX (0x10200034)                                                     */
/* Register default value:              0x40                                  */
#define CAPPTR_CB33_FUN_0_REG 0x18001034

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT8 cptr : 8;
    /* cptr - Bits[7:0], RWS_O, default = 8'h40 
       desc is missing
     */
  } Bits;
  UINT8 Data;
} CAPPTR_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* INTL_CB33_FUN_0_REG supported on:                                          */
/*       BDX (0x1020003C)                                                     */
/* Register default value:              0x00                                  */
#define INTL_CB33_FUN_0_REG 0x1800103C

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * The Interrupt Line register communicates interrupt line routing information to 
 * the device driver. The interrupt routing information in this register indicates 
 * which input of the interrupt controller this device uses. 
 */
typedef union {
  struct {
    UINT8 intl : 8;
    /* intl - Bits[7:0], RWS, default = 8'h0 
       The Interrupt Line register communicates interrupt line routing information to 
       the device driver. The interrupt routing information in this register indicates 
       which input of the interrupt controller this device uses. 
     */
  } Bits;
  UINT8 Data;
} INTL_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* INTP_CB33_FUN_0_REG supported on:                                          */
/*       BDX (0x1020003D)                                                     */
/* Register default value:              0x01                                  */
#define INTP_CB33_FUN_0_REG 0x1800103D

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * The INTP register identifies legacy interrupts for INTA, INTB, INTC and INTD as 
 * determined by BIOS/firmware. 
 */
typedef union {
  struct {
    UINT8 intp : 8;
    /* intp - Bits[7:0], RWS_L, default = 8'h01 
       The INTP register identifies legacy interrupts for INTA, INTB, INTC and INTD as 
       determined by BIOS/firmware. 
     */
  } Bits;
  UINT8 Data;
} INTP_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* EXPCAPLST_CB33_FUN_0_REG supported on:                                     */
/*       BDX (0x20200040)                                                     */
/* Register default value:              0x8010                                */
#define EXPCAPLST_CB33_FUN_0_REG 0x18002040

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT16 capid : 8;
    /* capid - Bits[7:0], RO, default = 8'h10 
       desc is missing
     */
    UINT16 np : 8;
    /* np - Bits[15:8], RO, default = 8'h80 
       desc is missing
     */
  } Bits;
  UINT16 Data;
} EXPCAPLST_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* EXPCAP_CB33_FUN_0_REG supported on:                                        */
/*       BDX (0x20200042)                                                     */
/* Register default value:              0x0092                                */
#define EXPCAP_CB33_FUN_0_REG 0x18002042

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * This register stores the version number of the capability item and other base 
 * information contained in the capability structure. 
 */
typedef union {
  struct {
    UINT16 vn : 4;
    /* vn - Bits[3:0], RO, default = 4'h2 
       This register stores the version number of the capability item and other base 
       information contained in the capability structure. 
     */
    UINT16 dt : 4;
    /* dt - Bits[7:4], RO, default = 4'b1001 
       This register stores the version number of the capability item and other base 
       information contained in the capability structure. 
     */
    UINT16 si : 1;
    /* si - Bits[8:8], RO, default = 1'b0 
       This register stores the version number of the capability item and other base 
       information contained in the capability structure. 
     */
    UINT16 imn : 5;
    /* imn - Bits[13:9], RO, default = 5'h0 
       This register stores the version number of the capability item and other base 
       information contained in the capability structure. 
     */
    UINT16 rsvd : 2;
    /* rsvd - Bits[15:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT16 Data;
} EXPCAP_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DEVCAP_CB33_FUN_0_REG supported on:                                        */
/*       BDX (0x40200044)                                                     */
/* Register default value:              0x10008001                            */
#define DEVCAP_CB33_FUN_0_REG 0x18004044

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 mpss : 3;
    /* mpss - Bits[2:0], RO, default = 3'b001 
       desc is missing
     */
    UINT32 pfs : 2;
    /* pfs - Bits[4:3], RO, default = 2'b00 
       desc is missing
     */
    UINT32 etfg : 1;
    /* etfg - Bits[5:5], RO, default = 1'b0 
       desc is missing
     */
    UINT32 epl0al : 3;
    /* epl0al - Bits[8:6], RO, default = 3'b000 
       desc is missing
     */
    UINT32 epl1al : 3;
    /* epl1al - Bits[11:9], RO, default = 3'b000 
       desc is missing
     */
    UINT32 undefined : 3;
    /* undefined - Bits[14:12], RO, default = 3'h0 
       desc is missing
     */
    UINT32 rber : 1;
    /* rber - Bits[15:15], RO, default = 1'b1 
       desc is missing
     */
    UINT32 rsvd_16 : 2;
    /* rsvd_16 - Bits[17:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 csplv : 8;
    /* csplv - Bits[25:18], RO, default = 8'h0 
       desc is missing
     */
    UINT32 cspls : 2;
    /* cspls - Bits[27:26], RO, default = 2'b00 
       desc is missing
     */
    UINT32 flr : 1;
    /* flr - Bits[28:28], RWS, default = 1'b1 
       desc is missing
     */
    UINT32 rsvd_29 : 3;
    /* rsvd_29 - Bits[31:29], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DEVCAP_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DEVCTL_CB33_FUN_0_REG supported on:                                        */
/*       BDX (0x20200048)                                                     */
/* Register default value:              0x1000                                */
#define DEVCTL_CB33_FUN_0_REG 0x18002048

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT16 cere : 1;
    /* cere - Bits[0:0], RWS, default = 1'b0 
       desc is missing
     */
    UINT16 nfere : 1;
    /* nfere - Bits[1:1], RWS, default = 1'b0 
       desc is missing
     */
    UINT16 fere : 1;
    /* fere - Bits[2:2], RWS, default = 1'b0 
       desc is missing
     */
    UINT16 urre : 1;
    /* urre - Bits[3:3], RWS, default = 1'b0 
       desc is missing
     */
    UINT16 enro : 1;
    /* enro - Bits[4:4], RO, default = 1'b0 
       desc is missing
     */
    UINT16 mps : 3;
    /* mps - Bits[7:5], RWS, default = 3'b000 
       desc is missing
     */
    UINT16 etfe : 1;
    /* etfe - Bits[8:8], RO, default = 1'b0 
       desc is missing
     */
    UINT16 pfe : 1;
    /* pfe - Bits[9:9], RO, default = 1'b0 
       desc is missing
     */
    UINT16 auxpme : 1;
    /* auxpme - Bits[10:10], RO, default = 1'b0 
       desc is missing
     */
    UINT16 enosnp : 1;
    /* enosnp - Bits[11:11], RWS, default = 1'b0 
       desc is missing
     */
    UINT16 mrrs : 3;
    /* mrrs - Bits[14:12], RO, default = 3'b001 
       desc is missing
     */
    UINT16 iflr : 1;
    /* iflr - Bits[15:15], RWS_LV, default = 1'b0 
       desc is missing
     */
  } Bits;
  UINT16 Data;
} DEVCTL_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DEVSTS_CB33_FUN_0_REG supported on:                                        */
/*       BDX (0x1020004A)                                                     */
/* Register default value:              0x00                                  */
#define DEVSTS_CB33_FUN_0_REG 0x1800104A

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT8 ced : 1;
    /* ced - Bits[0:0], RW1CS, default = 1'b0 
       desc is missing
     */
    UINT8 nfed : 1;
    /* nfed - Bits[1:1], RW1CS, default = 1'b0 
       desc is missing
     */
    UINT8 fed : 1;
    /* fed - Bits[2:2], RW1CS, default = 1'b0 
       desc is missing
     */
    UINT8 urd : 1;
    /* urd - Bits[3:3], RW1CS, default = 1'b0 
       desc is missing
     */
    UINT8 apd : 1;
    /* apd - Bits[4:4], RO, default = 1'b0 
       desc is missing
     */
    UINT8 tp : 1;
    /* tp - Bits[5:5], ROS_V, default = 1'b0 
       desc is missing
     */
    UINT8 rsvd : 2;
    /* rsvd - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} DEVSTS_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DEVCAP2_CB33_FUN_0_REG supported on:                                       */
/*       BDX (0x40200064)                                                     */
/* Register default value:              0x00000010                            */
#define DEVCAP2_CB33_FUN_0_REG 0x18004064

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 ctrs : 4;
    /* ctrs - Bits[3:0], RWS_L, default = 4'b0000 
       desc is missing
     */
    UINT32 ctds : 1;
    /* ctds - Bits[4:4], RO, default = 1'b1 
       desc is missing
     */
    UINT32 ari : 1;
    /* ari - Bits[5:5], RO, default = 1'b0 
       desc is missing
     */
    UINT32 ars : 1;
    /* ars - Bits[6:6], RO, default = 1'b0 
       desc is missing
     */
    UINT32 ad32acs : 1;
    /* ad32acs - Bits[7:7], RO, default = 1'b0 
       desc is missing
     */
    UINT32 ad64acs : 1;
    /* ad64acs - Bits[8:8], RO, default = 1'b0 
       desc is missing
     */
    UINT32 ad128acs : 1;
    /* ad128acs - Bits[9:9], RO, default = 1'b0 
       desc is missing
     */
    UINT32 nroeprpass : 1;
    /* nroeprpass - Bits[10:10], RO, default = 1'b0 
       desc is missing
     */
    UINT32 ltbwrms : 1;
    /* ltbwrms - Bits[11:11], RO, default = 1'b0 
       desc is missing
     */
    UINT32 tphcs : 2;
    /* tphcs - Bits[13:12], RO, default = 2'b00 
       desc is missing
     */
    UINT32 rsvd_14 : 4;
    /* rsvd_14 - Bits[17:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 obffs : 2;
    /* obffs - Bits[19:18], RO, default = 2'b00 
       desc is missing
     */
    UINT32 rsvd_20 : 12;
    /* rsvd_20 - Bits[31:20], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} DEVCAP2_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* DEVCTL2_CB33_FUN_0_REG supported on:                                       */
/*       BDX (0x20200068)                                                     */
/* Register default value:              0x0000                                */
#define DEVCTL2_CB33_FUN_0_REG 0x18002068

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT16 ctv : 4;
    /* ctv - Bits[3:0], RWS, default = 4'b0000 
       desc is missing
     */
    UINT16 ctd : 1;
    /* ctd - Bits[4:4], RWS, default = 1'b0 
       desc is missing
     */
    UINT16 arie : 1;
    /* arie - Bits[5:5], RO, default = 1'b0 
       desc is missing
     */
    UINT16 are : 1;
    /* are - Bits[6:6], RO, default = 1'b0 
       desc is missing
     */
    UINT16 aeb : 1;
    /* aeb - Bits[7:7], RO, default = 1'b0 
       desc is missing
     */
    UINT16 idore : 1;
    /* idore - Bits[8:8], RO, default = 1'b0 
       desc is missing
     */
    UINT16 idoce : 1;
    /* idoce - Bits[9:9], RO, default = 1'b0 
       desc is missing
     */
    UINT16 ltbwrme : 1;
    /* ltbwrme - Bits[10:10], RO, default = 1'b0 
       desc is missing
     */
    UINT16 rsvd_11 : 2;
    /* rsvd_11 - Bits[12:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 obffe : 2;
    /* obffe - Bits[14:13], RO, default = 2'b00 
       desc is missing
     */
    UINT16 rsvd_15 : 1;
    /* rsvd_15 - Bits[15:15], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT16 Data;
} DEVCTL2_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* CBCTL_CB33_FUN_0_REG supported on:                                         */
/*       BDX (0x4020007C)                                                     */
/* Register default value:              0x00000000                            */
#define CBCTL_CB33_FUN_0_REG 0x1800407C

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * The Crystal Beach device driver/Software must enable the respective bits during 
 * boot time for the CBDMA engine to function. This register field is coded with 
 * NFLRST attribute to indicate it is not reset by FLR. 
 */
typedef union {
  struct {
    UINT32 cben : 1;
    /* cben - Bits[0:0], RWS_L, default = 1'b0 
       The Crystal Beach device driver/Software must enable the respective bits during 
       boot time for the CBDMA engine to function. This register field is coded with 
       NFLRST attribute to indicate it is not reset by FLR. 
     */
    UINT32 raid5en : 1;
    /* raid5en - Bits[1:1], RWS_L, default = 1'b0 
       The Crystal Beach device driver/Software must enable the respective bits during 
       boot time for the CBDMA engine to function. This register field is coded with 
       NFLRST attribute to indicate it is not reset by FLR. 
     */
    UINT32 raid6en : 1;
    /* raid6en - Bits[2:2], RWS_L, default = 1'b0 
       The Crystal Beach device driver/Software must enable the respective bits during 
       boot time for the CBDMA engine to function. This register field is coded with 
       NFLRST attribute to indicate it is not reset by FLR. 
     */
    UINT32 difen : 1;
    /* difen - Bits[3:3], RWS_L, default = 1'b0 
       The Crystal Beach device driver/Software must enable the respective bits during 
       boot time for the CBDMA engine to function. This register field is coded with 
       NFLRST attribute to indicate it is not reset by FLR. 
     */
    UINT32 saden : 1;
    /* saden - Bits[4:4], RWS_L, default = 1'b0 
       The Crystal Beach device driver/Software must enable the respective bits during 
       boot time for the CBDMA engine to function. This register field is coded with 
       NFLRST attribute to indicate it is not reset by FLR. 
     */
    UINT32 rgf2pnsel : 1;
    /* rgf2pnsel - Bits[5:5], RWS_L, default = 1'b0 
       The Crystal Beach device driver/Software must enable the respective bits during 
       boot time for the CBDMA engine to function. This register field is coded with 
       NFLRST attribute to indicate it is not reset by FLR. 
     */
    UINT32 mcen : 1;
    /* mcen - Bits[6:6], RWS_L, default = 1'b0 
       The Crystal Beach device driver/Software must enable the respective bits during 
       boot time for the CBDMA engine to function. This register field is coded with 
       NFLRST attribute to indicate it is not reset by FLR. 
     */
    UINT32 ntbmden : 1;
    /* ntbmden - Bits[7:7], RWS_L, default = 1'b0 
       The Crystal Beach device driver/Software must enable the respective bits during 
       boot time for the CBDMA engine to function. This register field is coded with 
       NFLRST attribute to indicate it is not reset by FLR. 
     */
    UINT32 future : 24;
    /* future - Bits[31:8], RWS, default = 24'b0 
       The Crystal Beach device driver/Software must enable the respective bits during 
       boot time for the CBDMA engine to function. This register field is coded with 
       NFLRST attribute to indicate it is not reset by FLR. 
     */
  } Bits;
  UINT32 Data;
} CBCTL_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* PMCAPLST_CB33_FUN_0_REG supported on:                                      */
/*       BDX (0x20200080)                                                     */
/* Register default value:              0x9001                                */
#define PMCAPLST_CB33_FUN_0_REG 0x18002080

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT16 capabilityid : 8;
    /* capabilityid - Bits[7:0], RO, default = 8'h01 
       desc is missing
     */
    UINT16 nextpointer : 8;
    /* nextpointer - Bits[15:8], RO, default = 8'h90 
       desc is missing
     */
  } Bits;
  UINT16 Data;
} PMCAPLST_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* PMCAP_CB33_FUN_0_REG supported on:                                         */
/*       BDX (0x20200082)                                                     */
/* Register default value:              0x0003                                */
#define PMCAP_CB33_FUN_0_REG 0x18002082

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT16 ver : 3;
    /* ver - Bits[2:0], RO, default = 3'h3 
       desc is missing
     */
    UINT16 pmeclk : 1;
    /* pmeclk - Bits[3:3], RO, default = 1'b0 
       desc is missing
     */
    UINT16 rsvd : 1;
    /* rsvd - Bits[4:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 dsi : 1;
    /* dsi - Bits[5:5], RO, default = 1'b0 
       desc is missing
     */
    UINT16 ac : 3;
    /* ac - Bits[8:6], RO, default = 3'b000 
       desc is missing
     */
    UINT16 d1s : 1;
    /* d1s - Bits[9:9], RO, default = 1'b0 
       desc is missing
     */
    UINT16 d2s : 1;
    /* d2s - Bits[10:10], RO, default = 1'b0 
       desc is missing
     */
    UINT16 pmes : 5;
    /* pmes - Bits[15:11], RO, default = 5'h0 
       desc is missing
     */
  } Bits;
  UINT16 Data;
} PMCAP_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* PMCSR_CB33_FUN_0_REG supported on:                                         */
/*       BDX (0x40200084)                                                     */
/* Register default value:              0x00000008                            */
#define PMCSR_CB33_FUN_0_REG 0x18004084

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 ps : 2;
    /* ps - Bits[1:0], RWS_R, default = 2'h0 
       desc is missing
     */
    UINT32 rsvd_2 : 1;
    /* rsvd_2 - Bits[2:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 nsr : 1;
    /* nsr - Bits[3:3], RWS_L, default = 1'b1 
       desc is missing
     */
    UINT32 rsvd_4 : 4;
    /* rsvd_4 - Bits[7:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 pmeen : 1;
    /* pmeen - Bits[8:8], RWS, default = 1'b0 
       desc is missing
     */
    UINT32 ds : 4;
    /* ds - Bits[12:9], RO, default = 4'h0 
       desc is missing
     */
    UINT32 dc : 2;
    /* dc - Bits[14:13], RO, default = 2'h0 
       desc is missing
     */
    UINT32 pmests : 1;
    /* pmests - Bits[15:15], RO, default = 1'b0 
       desc is missing
     */
    UINT32 rsvd_16 : 6;
    /* rsvd_16 - Bits[21:16], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 b23en : 1;
    /* b23en - Bits[22:22], RO, default = 1'b0 
       desc is missing
     */
    UINT32 bpcc_en : 1;
    /* bpcc_en - Bits[23:23], RO, default = 1'b0 
       desc is missing
     */
    UINT32 data : 8;
    /* data - Bits[31:24], RO, default = 8'h00 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} PMCSR_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MSICAPLST_CB33_FUN_0_REG supported on:                                     */
/*       BDX (0x20200090)                                                     */
/* Register default value:              0xAC05                                */
#define MSICAPLST_CB33_FUN_0_REG 0x18002090

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT16 capid : 8;
    /* capid - Bits[7:0], RO, default = 8'h05 
       desc is missing
     */
    UINT16 np : 8;
    /* np - Bits[15:8], RO, default = 8'hAC 
       desc is missing
     */
  } Bits;
  UINT16 Data;
} MSICAPLST_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MSICTL_CB33_FUN_0_REG supported on:                                        */
/*       BDX (0x20200092)                                                     */
/* Register default value:              0x0100                                */
#define MSICTL_CB33_FUN_0_REG 0x18002092

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT16 msie : 1;
    /* msie - Bits[0:0], RWS, default = 1'b0 
       desc is missing
     */
    UINT16 mmc : 3;
    /* mmc - Bits[3:1], RO, default = 3'b000 
       desc is missing
     */
    UINT16 mmen : 3;
    /* mmen - Bits[6:4], RWS, default = 3'b000 
       desc is missing
     */
    UINT16 ad64c : 1;
    /* ad64c - Bits[7:7], RO, default = 1'b0 
       desc is missing
     */
    UINT16 pvm : 1;
    /* pvm - Bits[8:8], RO, default = 1'b1 
       desc is missing
     */
    UINT16 reserved : 7;
    /* reserved - Bits[15:9], RO, default = 7'h00 
       desc is missing
     */
  } Bits;
  UINT16 Data;
} MSICTL_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MSIADDR_CB33_FUN_0_REG supported on:                                       */
/*       BDX (0x40200094)                                                     */
/* Register default value:              0x00000000                            */
#define MSIADDR_CB33_FUN_0_REG 0x18004094

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 reserved : 2;
    /* reserved - Bits[1:0], RO, default = 2'h0 
       desc is missing
     */
    UINT32 addr_dm : 1;
    /* addr_dm - Bits[2:2], RWS, default = 1'h0 
       desc is missing
     */
    UINT32 addr_rh : 1;
    /* addr_rh - Bits[3:3], RWS, default = 1'h0 
       desc is missing
     */
    UINT32 addr_eid : 8;
    /* addr_eid - Bits[11:4], RWS, default = 8'h00 
       desc is missing
     */
    UINT32 addr_did : 8;
    /* addr_did - Bits[19:12], RWS, default = 8'h00 
       desc is missing
     */
    UINT32 addr_msb : 12;
    /* addr_msb - Bits[31:20], RWS, default = 12'h0 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} MSIADDR_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MSIDATA_CB33_FUN_0_REG supported on:                                       */
/*       BDX (0x20200098)                                                     */
/* Register default value:              0x0000                                */
#define MSIDATA_CB33_FUN_0_REG 0x18002098

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT16 iv : 8;
    /* iv - Bits[7:0], RWS, default = 8'h0 
       desc is missing
     */
    UINT16 dm : 4;
    /* dm - Bits[11:8], RWS, default = 4'h0 
       desc is missing
     */
    UINT16 future : 2;
    /* future - Bits[13:12], RWS, default = 2'h0 
       desc is missing
     */
    UINT16 lvl : 1;
    /* lvl - Bits[14:14], RWS, default = 1'h0 
       desc is missing
     */
    UINT16 tm : 1;
    /* tm - Bits[15:15], RWS, default = 1'h0 
       desc is missing
     */
  } Bits;
  UINT16 Data;
} MSIDATA_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MSIMSK_CB33_FUN_0_REG supported on:                                        */
/*       BDX (0x1020009C)                                                     */
/* Register default value:              0x00                                  */
#define MSIMSK_CB33_FUN_0_REG 0x1800109C

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * The MSI Mask Bit register enables software to disable message sending on a 
 * per-vector basis. 
 */
typedef union {
  struct {
    UINT8 mskb : 2;
    /* mskb - Bits[1:0], RWS, default = 2'b00 
       The MSI Mask Bit register enables software to disable message sending on a 
       per-vector basis. 
     */
    UINT8 rsvd : 6;
    /* rsvd - Bits[7:2], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} MSIMSK_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MSIPENDING_CB33_FUN_0_REG supported on:                                    */
/*       BDX (0x402000A0)                                                     */
/* Register default value:              0x00000000                            */
#define MSIPENDING_CB33_FUN_0_REG 0x180040A0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * The Mask Pending register enables software to defer message sending on a 
 * per-vector basis. 
 */
typedef union {
  struct {
    UINT32 pb : 1;
    /* pb - Bits[0:0], ROS_V, default = 1'h0 
       The Mask Pending register enables software to defer message sending on a 
       per-vector basis. 
     */
    UINT32 rsvd : 31;
    /* rsvd - Bits[31:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} MSIPENDING_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MSIXCAPLST_CB33_FUN_0_REG supported on:                                    */
/*       BDX (0x202000AC)                                                     */
/* Register default value:              0x0011                                */
#define MSIXCAPLST_CB33_FUN_0_REG 0x180020AC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT16 capid : 8;
    /* capid - Bits[7:0], RO, default = 8'h11 
       desc is missing
     */
    UINT16 np : 8;
    /* np - Bits[15:8], RO, default = 8'h00 
       desc is missing
     */
  } Bits;
  UINT16 Data;
} MSIXCAPLST_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MSIXCTL_CB33_FUN_0_REG supported on:                                       */
/*       BDX (0x202000AE)                                                     */
/* Register default value:              0x0000                                */
#define MSIXCTL_CB33_FUN_0_REG 0x180020AE

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT16 ts : 11;
    /* ts - Bits[10:0], RWS_O, default = 11'h0 
       desc is missing
     */
    UINT16 rsvd : 3;
    /* rsvd - Bits[13:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 fnmsk : 1;
    /* fnmsk - Bits[14:14], RWS, default = 1'b0 
       desc is missing
     */
    UINT16 msixe : 1;
    /* msixe - Bits[15:15], RWS, default = 1'b0 
       desc is missing
     */
  } Bits;
  UINT16 Data;
} MSIXCTL_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MSIXTOTB_CB33_FUN_0_REG supported on:                                      */
/*       BDX (0x402000B0)                                                     */
/* Register default value:              0x00001000                            */
#define MSIXTOTB_CB33_FUN_0_REG 0x180040B0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * This register 'MSI-X Table Offset/Table BIR Register (MSIXTOTB)'when read as a 
 * 32bit value would yield 0x1000 and SW can suitably extract the table offset by 
 * masking off the lower 3 bits. 
 */
typedef union {
  struct {
    UINT32 tablebir2 : 3;
    /* tablebir2 - Bits[2:0], RO, default = 3'h0 
       This register 'MSI-X Table Offset/Table BIR Register (MSIXTOTB)'when read as a 
       32bit value would yield 0x1000 and SW can suitably extract the table offset by 
       masking off the lower 3 bits. 
     */
    UINT32 tableoffset31 : 29;
    /* tableoffset31 - Bits[31:3], RO, default = 29'h0000200 
       This register 'MSI-X Table Offset/Table BIR Register (MSIXTOTB)'when read as a 
       32bit value would yield 0x1000 and SW can suitably extract the table offset by 
       masking off the lower 3 bits. 
     */
  } Bits;
  UINT32 Data;
} MSIXTOTB_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MSIXTPOPB_CB33_FUN_0_REG supported on:                                     */
/*       BDX (0x402000B4)                                                     */
/* Register default value:              0x00001800                            */
#define MSIXTPOPB_CB33_FUN_0_REG 0x180040B4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * 32Note that MSI and MSI-X capability are mutually exclusive. An implementation 
 * may provide for both or just one. However, software is required only to enable 
 * one. If neither MSI nor MSI-X are enabled, then the CB device uses legacy 
 * interrupts. 
 * [br]For MSI-X, the CB device provides registers for one interrupt message vector 
 * per DMA channel. However, when the MSI-X Vector Control bit in the 'Interrupt 
 * Control Register (INTRCTRL)' register is not set (see Section 34.4.2.4, on page 
 * 1844), the DMA channel uses the first interrupt vector for all types (i.e. does 
 * not distinguish between completions, errors). 
 * [br]The following is the decode mechanism for routing completion and error 
 * interrupts by indexing the appropriate entries. Refer to Section , 'MSIXTBA' on 
 * page 1866 and its subsequent registers. 
 */
typedef union {
  struct {
    UINT32 tablebir2 : 3;
    /* tablebir2 - Bits[2:0], RO, default = 3'h0 
       32Note that MSI and MSI-X capability are mutually exclusive. An implementation 
       may provide for both or just one. However, software is required only to enable 
       one. If neither MSI nor MSI-X are enabled, then the CB device uses legacy 
       interrupts. 
       [br]For MSI-X, the CB device provides registers for one interrupt message vector 
       per DMA channel. However, when the MSI-X Vector Control bit in the 'Interrupt 
       Control Register (INTRCTRL)' register is not set (see Section 34.4.2.4, on page 
       1844), the DMA channel uses the first interrupt vector for all types (i.e. does 
       not distinguish between completions, errors). 
       [br]The following is the decode mechanism for routing completion and error 
       interrupts by indexing the appropriate entries. Refer to Section , 'MSIXTBA' on 
       page 1866 and its subsequent registers. 
     */
    UINT32 tableoffset31 : 29;
    /* tableoffset31 - Bits[31:3], RO, default = 29'h0000300 
       32Note that MSI and MSI-X capability are mutually exclusive. An implementation 
       may provide for both or just one. However, software is required only to enable 
       one. If neither MSI nor MSI-X are enabled, then the CB device uses legacy 
       interrupts. 
       [br]For MSI-X, the CB device provides registers for one interrupt message vector 
       per DMA channel. However, when the MSI-X Vector Control bit in the 'Interrupt 
       Control Register (INTRCTRL)' register is not set (see Section 34.4.2.4, on page 
       1844), the DMA channel uses the first interrupt vector for all types (i.e. does 
       not distinguish between completions, errors). 
       [br]The following is the decode mechanism for routing completion and error 
       interrupts by indexing the appropriate entries. Refer to Section , 'MSIXTBA' on 
       page 1866 and its subsequent registers. 
     */
  } Bits;
  UINT32 Data;
} MSIXTPOPB_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* PLKCTL_CB33_FUN_0_REG supported on:                                        */
/*       BDX (0x202000EA)                                                     */
/* Register default value:              0x0000                                */
#define PLKCTL_CB33_FUN_0_REG 0x180020EA

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT16 cl : 1;
    /* cl - Bits[0:0], RWS_KL, default = 1'b0 
       desc is missing
     */
    UINT16 rsvd : 15;
    /* rsvd - Bits[15:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT16 Data;
} PLKCTL_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* IOSFDEVCLKGCTL_CB33_FUN_0_REG supported on:                                */
/*       BDX (0x202000EC)                                                     */
/* Register default value:              0x0010                                */
#define IOSFDEVCLKGCTL_CB33_FUN_0_REG 0x180020EC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * The IOSFDEVCLKGCTL registered is defined for a single endpoint interface in the 
 * IOSF. It is connected to Fn0 and the rest of the functions/channels are aliased 
 * to it. It is not affected by FLR. 
 */
typedef union {
  struct {
    UINT16 ict : 8;
    /* ict - Bits[7:0], RWS, default = 8'h10 
       The IOSFDEVCLKGCTL registered is defined for a single endpoint interface in the 
       IOSF. It is connected to Fn0 and the rest of the functions/channels are aliased 
       to it. It is not affected by FLR. 
     */
    UINT16 rsvd : 7;
    /* rsvd - Bits[14:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 icge : 1;
    /* icge - Bits[15:15], RWS, default = 1'h0 
       The IOSFDEVCLKGCTL registered is defined for a single endpoint interface in the 
       IOSF. It is connected to Fn0 and the rest of the functions/channels are aliased 
       to it. It is not affected by FLR. 
     */
  } Bits;
  UINT16 Data;
} IOSFDEVCLKGCTL_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* SBDEVCLKGCTL_CB33_FUN_0_REG supported on:                                  */
/*       BDX (0x202000EE)                                                     */
/* Register default value:              0x8010                                */
#define SBDEVCLKGCTL_CB33_FUN_0_REG 0x180020EE

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * The SBDEVCLKGCTL registered is defined for a single endpoint interface in the 
 * IOSF. It is connected to Fn0 and the rest of the functions/channels are aliased 
 * to it. It is not affected by FLR. 
 */
typedef union {
  struct {
    UINT16 ict : 8;
    /* ict - Bits[7:0], RWS, default = 8'h10 
       The SBDEVCLKGCTL registered is defined for a single endpoint interface in the 
       IOSF. It is connected to Fn0 and the rest of the functions/channels are aliased 
       to it. It is not affected by FLR. 
     */
    UINT16 rsvd : 7;
    /* rsvd - Bits[14:8], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 icge : 1;
    /* icge - Bits[15:15], RWS, default = 1'h1 
       The SBDEVCLKGCTL registered is defined for a single endpoint interface in the 
       IOSF. It is connected to Fn0 and the rest of the functions/channels are aliased 
       to it. It is not affected by FLR. 
     */
  } Bits;
  UINT16 Data;
} SBDEVCLKGCTL_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* NTBDBSADR_N0_CB33_FUN_0_REG supported on:                                  */
/*       BDX (0x402000F0)                                                     */
/* Register default value:              0x00000000                            */
#define NTBDBSADR_N0_CB33_FUN_0_REG 0x180040F0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 ntbdbadr : 32;
    /* ntbdbadr - Bits[31:0], RWS, default = 64'h0 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} NTBDBSADR_N0_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* NTBDBSADR_N1_CB33_FUN_0_REG supported on:                                  */
/*       BDX (0x402000F4)                                                     */
/* Register default value:              0x00000000                            */
#define NTBDBSADR_N1_CB33_FUN_0_REG 0x180040F4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 ntbdbadr : 32;
    /* ntbdbadr - Bits[31:0], RWS, default = 64'h0 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} NTBDBSADR_N1_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* CBSPADS_CB33_FUN_0_REG supported on:                                       */
/*       BDX (0x402000FC)                                                     */
/* Register default value:              0x00000001                            */
#define CBSPADS_CB33_FUN_0_REG 0x180040FC

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 spad : 32;
    /* spad - Bits[31:0], RWS, default = 32'h1 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} CBSPADS_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* AERCAPHDR_CB33_FUN_0_REG supported on:                                     */
/*       BDX (0x40200100)                                                     */
/* Register default value:              0x15010001                            */
#define AERCAPHDR_CB33_FUN_0_REG 0x18004100

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 excapid : 16;
    /* excapid - Bits[15:0], RO, default = 16'h0001 
       desc is missing
     */
    UINT32 cv : 4;
    /* cv - Bits[19:16], RO, default = 4'h1 
       desc is missing
     */
    UINT32 nco : 12;
    /* nco - Bits[31:20], RWS_L, default = 12'h150 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} AERCAPHDR_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* ERRUNCSTS_CB33_FUN_0_REG supported on:                                     */
/*       BDX (0x40200104)                                                     */
/* Register default value:              0x00000000                            */
#define ERRUNCSTS_CB33_FUN_0_REG 0x18004104

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * This register reports the error status of individual uncorrectable error sources 
 * for the CBDMA block on the IOSF interface. An individual error status bit that 
 * is set to 1 indicates that a particular error occurred. Software can clear an 
 * error status by writing a 1 to the respective bit. 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 5;
    /* rsvd_0 - Bits[4:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 slde : 1;
    /* slde - Bits[5:5], RO, default = 1'b0 
       This register reports the error status of individual uncorrectable error sources 
       for the CBDMA block on the IOSF interface. An individual error status bit that 
       is set to 1 indicates that a particular error occurred. Software can clear an 
       error status by writing a 1 to the respective bit. 
     */
    UINT32 rsvd_6 : 6;
    /* rsvd_6 - Bits[11:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ptlpe : 1;
    /* ptlpe - Bits[12:12], RW1CS, default = 1'b0 
       This register reports the error status of individual uncorrectable error sources 
       for the CBDMA block on the IOSF interface. An individual error status bit that 
       is set to 1 indicates that a particular error occurred. Software can clear an 
       error status by writing a 1 to the respective bit. 
     */
    UINT32 fce : 1;
    /* fce - Bits[13:13], RO, default = 1'b0 
       This register reports the error status of individual uncorrectable error sources 
       for the CBDMA block on the IOSF interface. An individual error status bit that 
       is set to 1 indicates that a particular error occurred. Software can clear an 
       error status by writing a 1 to the respective bit. 
     */
    UINT32 cte : 1;
    /* cte - Bits[14:14], RW1CS, default = 1'b0 
       This register reports the error status of individual uncorrectable error sources 
       for the CBDMA block on the IOSF interface. An individual error status bit that 
       is set to 1 indicates that a particular error occurred. Software can clear an 
       error status by writing a 1 to the respective bit. 
     */
    UINT32 cae : 1;
    /* cae - Bits[15:15], RW1CS, default = 1'b0 
       This register reports the error status of individual uncorrectable error sources 
       for the CBDMA block on the IOSF interface. An individual error status bit that 
       is set to 1 indicates that a particular error occurred. Software can clear an 
       error status by writing a 1 to the respective bit. 
     */
    UINT32 uce : 1;
    /* uce - Bits[16:16], RW1CS, default = 1'b0 
       This register reports the error status of individual uncorrectable error sources 
       for the CBDMA block on the IOSF interface. An individual error status bit that 
       is set to 1 indicates that a particular error occurred. Software can clear an 
       error status by writing a 1 to the respective bit. 
     */
    UINT32 roe : 1;
    /* roe - Bits[17:17], RO, default = 1'b0 
       This register reports the error status of individual uncorrectable error sources 
       for the CBDMA block on the IOSF interface. An individual error status bit that 
       is set to 1 indicates that a particular error occurred. Software can clear an 
       error status by writing a 1 to the respective bit. 
     */
    UINT32 mtlpe : 1;
    /* mtlpe - Bits[18:18], RW1CS, default = 1'b0 
       This register reports the error status of individual uncorrectable error sources 
       for the CBDMA block on the IOSF interface. An individual error status bit that 
       is set to 1 indicates that a particular error occurred. Software can clear an 
       error status by writing a 1 to the respective bit. 
     */
    UINT32 ecrce : 1;
    /* ecrce - Bits[19:19], RO, default = 1'b0 
       This register reports the error status of individual uncorrectable error sources 
       for the CBDMA block on the IOSF interface. An individual error status bit that 
       is set to 1 indicates that a particular error occurred. Software can clear an 
       error status by writing a 1 to the respective bit. 
     */
    UINT32 ure : 1;
    /* ure - Bits[20:20], RW1CS, default = 1'b0 
       This register reports the error status of individual uncorrectable error sources 
       for the CBDMA block on the IOSF interface. An individual error status bit that 
       is set to 1 indicates that a particular error occurred. Software can clear an 
       error status by writing a 1 to the respective bit. 
     */
    UINT32 acse : 1;
    /* acse - Bits[21:21], RO, default = 1'b0 
       This register reports the error status of individual uncorrectable error sources 
       for the CBDMA block on the IOSF interface. An individual error status bit that 
       is set to 1 indicates that a particular error occurred. Software can clear an 
       error status by writing a 1 to the respective bit. 
     */
    UINT32 uie : 1;
    /* uie - Bits[22:22], RW1CS, default = 1'b0 
       This register reports the error status of individual uncorrectable error sources 
       for the CBDMA block on the IOSF interface. An individual error status bit that 
       is set to 1 indicates that a particular error occurred. Software can clear an 
       error status by writing a 1 to the respective bit. 
     */
    UINT32 rsvd_23 : 9;
    /* rsvd_23 - Bits[31:23], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} ERRUNCSTS_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* ERRUNCMSK_CB33_FUN_0_REG supported on:                                     */
/*       BDX (0x40200108)                                                     */
/* Register default value:              0x00000000                            */
#define ERRUNCMSK_CB33_FUN_0_REG 0x18004108

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * This register controls the reporting of individual uncorrectable errors by 
 * device to the host bridge via a PCI Express error message. This register also 
 * controls the logging of the header. A masked error (respective bit set in the 
 * mask register) is not reported to the host bridge nor is the header logged 
 * (status bits unaffected by the mask bit). There is a mask bit per bit of the 
 * Uncorrectable Error Status Register. 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 4;
    /* rsvd_0 - Bits[3:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 dlpem : 1;
    /* dlpem - Bits[4:4], RO, default = 1'b0 
       This register controls the reporting of individual uncorrectable errors by 
       device to the host bridge via a PCI Express error message. This register also 
       controls the logging of the header. A masked error (respective bit set in the 
       mask register) is not reported to the host bridge nor is the header logged 
       (status bits unaffected by the mask bit). There is a mask bit per bit of the 
       Uncorrectable Error Status Register. 
     */
    UINT32 sldem : 1;
    /* sldem - Bits[5:5], RO, default = 1'b0 
       This register controls the reporting of individual uncorrectable errors by 
       device to the host bridge via a PCI Express error message. This register also 
       controls the logging of the header. A masked error (respective bit set in the 
       mask register) is not reported to the host bridge nor is the header logged 
       (status bits unaffected by the mask bit). There is a mask bit per bit of the 
       Uncorrectable Error Status Register. 
     */
    UINT32 rsvd_6 : 6;
    /* rsvd_6 - Bits[11:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ptlpem : 1;
    /* ptlpem - Bits[12:12], RWS, default = 1'b0 
       This register controls the reporting of individual uncorrectable errors by 
       device to the host bridge via a PCI Express error message. This register also 
       controls the logging of the header. A masked error (respective bit set in the 
       mask register) is not reported to the host bridge nor is the header logged 
       (status bits unaffected by the mask bit). There is a mask bit per bit of the 
       Uncorrectable Error Status Register. 
     */
    UINT32 fcem : 1;
    /* fcem - Bits[13:13], RO, default = 1'b0 
       This register controls the reporting of individual uncorrectable errors by 
       device to the host bridge via a PCI Express error message. This register also 
       controls the logging of the header. A masked error (respective bit set in the 
       mask register) is not reported to the host bridge nor is the header logged 
       (status bits unaffected by the mask bit). There is a mask bit per bit of the 
       Uncorrectable Error Status Register. 
     */
    UINT32 ctem : 1;
    /* ctem - Bits[14:14], RWS, default = 1'b0 
       This register controls the reporting of individual uncorrectable errors by 
       device to the host bridge via a PCI Express error message. This register also 
       controls the logging of the header. A masked error (respective bit set in the 
       mask register) is not reported to the host bridge nor is the header logged 
       (status bits unaffected by the mask bit). There is a mask bit per bit of the 
       Uncorrectable Error Status Register. 
     */
    UINT32 caem : 1;
    /* caem - Bits[15:15], RWS, default = 1'b0 
       This register controls the reporting of individual uncorrectable errors by 
       device to the host bridge via a PCI Express error message. This register also 
       controls the logging of the header. A masked error (respective bit set in the 
       mask register) is not reported to the host bridge nor is the header logged 
       (status bits unaffected by the mask bit). There is a mask bit per bit of the 
       Uncorrectable Error Status Register. 
     */
    UINT32 ucem : 1;
    /* ucem - Bits[16:16], RWS, default = 1'b0 
       This register controls the reporting of individual uncorrectable errors by 
       device to the host bridge via a PCI Express error message. This register also 
       controls the logging of the header. A masked error (respective bit set in the 
       mask register) is not reported to the host bridge nor is the header logged 
       (status bits unaffected by the mask bit). There is a mask bit per bit of the 
       Uncorrectable Error Status Register. 
     */
    UINT32 roem : 1;
    /* roem - Bits[17:17], RO, default = 1'b0 
       This register controls the reporting of individual uncorrectable errors by 
       device to the host bridge via a PCI Express error message. This register also 
       controls the logging of the header. A masked error (respective bit set in the 
       mask register) is not reported to the host bridge nor is the header logged 
       (status bits unaffected by the mask bit). There is a mask bit per bit of the 
       Uncorrectable Error Status Register. 
     */
    UINT32 mtlpem : 1;
    /* mtlpem - Bits[18:18], RWS, default = 1'b0 
       This register controls the reporting of individual uncorrectable errors by 
       device to the host bridge via a PCI Express error message. This register also 
       controls the logging of the header. A masked error (respective bit set in the 
       mask register) is not reported to the host bridge nor is the header logged 
       (status bits unaffected by the mask bit). There is a mask bit per bit of the 
       Uncorrectable Error Status Register. 
     */
    UINT32 ecrcem : 1;
    /* ecrcem - Bits[19:19], RO, default = 1'b0 
       This register controls the reporting of individual uncorrectable errors by 
       device to the host bridge via a PCI Express error message. This register also 
       controls the logging of the header. A masked error (respective bit set in the 
       mask register) is not reported to the host bridge nor is the header logged 
       (status bits unaffected by the mask bit). There is a mask bit per bit of the 
       Uncorrectable Error Status Register. 
     */
    UINT32 urem : 1;
    /* urem - Bits[20:20], RWS, default = 1'b0 
       This register controls the reporting of individual uncorrectable errors by 
       device to the host bridge via a PCI Express error message. This register also 
       controls the logging of the header. A masked error (respective bit set in the 
       mask register) is not reported to the host bridge nor is the header logged 
       (status bits unaffected by the mask bit). There is a mask bit per bit of the 
       Uncorrectable Error Status Register. 
     */
    UINT32 acsem : 1;
    /* acsem - Bits[21:21], RO, default = 1'b0 
       This register controls the reporting of individual uncorrectable errors by 
       device to the host bridge via a PCI Express error message. This register also 
       controls the logging of the header. A masked error (respective bit set in the 
       mask register) is not reported to the host bridge nor is the header logged 
       (status bits unaffected by the mask bit). There is a mask bit per bit of the 
       Uncorrectable Error Status Register. 
     */
    UINT32 uiem : 1;
    /* uiem - Bits[22:22], RWS, default = 1'b0 
       This register controls the reporting of individual uncorrectable errors by 
       device to the host bridge via a PCI Express error message. This register also 
       controls the logging of the header. A masked error (respective bit set in the 
       mask register) is not reported to the host bridge nor is the header logged 
       (status bits unaffected by the mask bit). There is a mask bit per bit of the 
       Uncorrectable Error Status Register. 
     */
    UINT32 mcem : 1;
    /* mcem - Bits[23:23], RO, default = 1'b0 
       This register controls the reporting of individual uncorrectable errors by 
       device to the host bridge via a PCI Express error message. This register also 
       controls the logging of the header. A masked error (respective bit set in the 
       mask register) is not reported to the host bridge nor is the header logged 
       (status bits unaffected by the mask bit). There is a mask bit per bit of the 
       Uncorrectable Error Status Register. 
     */
    UINT32 aebem : 1;
    /* aebem - Bits[24:24], RO, default = 1'b0 
       This register controls the reporting of individual uncorrectable errors by 
       device to the host bridge via a PCI Express error message. This register also 
       controls the logging of the header. A masked error (respective bit set in the 
       mask register) is not reported to the host bridge nor is the header logged 
       (status bits unaffected by the mask bit). There is a mask bit per bit of the 
       Uncorrectable Error Status Register. 
     */
    UINT32 rsvd_25 : 7;
    /* rsvd_25 - Bits[31:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} ERRUNCMSK_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* ERRUNCSEV_CB33_FUN_0_REG supported on:                                     */
/*       BDX (0x4020010C)                                                     */
/* Register default value:              0x00440000                            */
#define ERRUNCSEV_CB33_FUN_0_REG 0x1800410C

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * This register controls whether an individual uncorrectable error is reported as 
 * a fatal error. An uncorrectable error is reported as fatal when the 
 * corresponding error bit in this register is set. When the bit is cleared, the 
 * corresponding error is considered non-fatal. 
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 4;
    /* rsvd_0 - Bits[3:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 dlpes : 1;
    /* dlpes - Bits[4:4], RO, default = 1'b0 
       This register controls whether an individual uncorrectable error is reported as 
       a fatal error. An uncorrectable error is reported as fatal when the 
       corresponding error bit in this register is set. When the bit is cleared, the 
       corresponding error is considered non-fatal. 
     */
    UINT32 sldes : 1;
    /* sldes - Bits[5:5], RO, default = 1'b0 
       This register controls whether an individual uncorrectable error is reported as 
       a fatal error. An uncorrectable error is reported as fatal when the 
       corresponding error bit in this register is set. When the bit is cleared, the 
       corresponding error is considered non-fatal. 
     */
    UINT32 rsvd_6 : 6;
    /* rsvd_6 - Bits[11:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ptlpes : 1;
    /* ptlpes - Bits[12:12], RWS, default = 1'b0 
       This register controls whether an individual uncorrectable error is reported as 
       a fatal error. An uncorrectable error is reported as fatal when the 
       corresponding error bit in this register is set. When the bit is cleared, the 
       corresponding error is considered non-fatal. 
     */
    UINT32 fces : 1;
    /* fces - Bits[13:13], RO, default = 1'b0 
       This register controls whether an individual uncorrectable error is reported as 
       a fatal error. An uncorrectable error is reported as fatal when the 
       corresponding error bit in this register is set. When the bit is cleared, the 
       corresponding error is considered non-fatal. 
     */
    UINT32 ctes : 1;
    /* ctes - Bits[14:14], RWS, default = 1'b0 
       This register controls whether an individual uncorrectable error is reported as 
       a fatal error. An uncorrectable error is reported as fatal when the 
       corresponding error bit in this register is set. When the bit is cleared, the 
       corresponding error is considered non-fatal. 
     */
    UINT32 caes : 1;
    /* caes - Bits[15:15], RWS, default = 1'b0 
       This register controls whether an individual uncorrectable error is reported as 
       a fatal error. An uncorrectable error is reported as fatal when the 
       corresponding error bit in this register is set. When the bit is cleared, the 
       corresponding error is considered non-fatal. 
     */
    UINT32 uces : 1;
    /* uces - Bits[16:16], RWS, default = 1'b0 
       This register controls whether an individual uncorrectable error is reported as 
       a fatal error. An uncorrectable error is reported as fatal when the 
       corresponding error bit in this register is set. When the bit is cleared, the 
       corresponding error is considered non-fatal. 
     */
    UINT32 roes : 1;
    /* roes - Bits[17:17], RO, default = 1'b0 
       This register controls whether an individual uncorrectable error is reported as 
       a fatal error. An uncorrectable error is reported as fatal when the 
       corresponding error bit in this register is set. When the bit is cleared, the 
       corresponding error is considered non-fatal. 
     */
    UINT32 mtlpes : 1;
    /* mtlpes - Bits[18:18], RWS, default = 1'b1 
       This register controls whether an individual uncorrectable error is reported as 
       a fatal error. An uncorrectable error is reported as fatal when the 
       corresponding error bit in this register is set. When the bit is cleared, the 
       corresponding error is considered non-fatal. 
     */
    UINT32 ecrces : 1;
    /* ecrces - Bits[19:19], RO, default = 1'b0 
       This register controls whether an individual uncorrectable error is reported as 
       a fatal error. An uncorrectable error is reported as fatal when the 
       corresponding error bit in this register is set. When the bit is cleared, the 
       corresponding error is considered non-fatal. 
     */
    UINT32 ures : 1;
    /* ures - Bits[20:20], RWS, default = 1'b0 
       This register controls whether an individual uncorrectable error is reported as 
       a fatal error. An uncorrectable error is reported as fatal when the 
       corresponding error bit in this register is set. When the bit is cleared, the 
       corresponding error is considered non-fatal. 
     */
    UINT32 acses : 1;
    /* acses - Bits[21:21], RO, default = 1'b0 
       This register controls whether an individual uncorrectable error is reported as 
       a fatal error. An uncorrectable error is reported as fatal when the 
       corresponding error bit in this register is set. When the bit is cleared, the 
       corresponding error is considered non-fatal. 
     */
    UINT32 uies : 1;
    /* uies - Bits[22:22], RWS, default = 1'b1 
       This register controls whether an individual uncorrectable error is reported as 
       a fatal error. An uncorrectable error is reported as fatal when the 
       corresponding error bit in this register is set. When the bit is cleared, the 
       corresponding error is considered non-fatal. 
     */
    UINT32 mces : 1;
    /* mces - Bits[23:23], RO, default = 1'b0 
       This register controls whether an individual uncorrectable error is reported as 
       a fatal error. An uncorrectable error is reported as fatal when the 
       corresponding error bit in this register is set. When the bit is cleared, the 
       corresponding error is considered non-fatal. 
     */
    UINT32 aebes : 1;
    /* aebes - Bits[24:24], RO, default = 1'b0 
       This register controls whether an individual uncorrectable error is reported as 
       a fatal error. An uncorrectable error is reported as fatal when the 
       corresponding error bit in this register is set. When the bit is cleared, the 
       corresponding error is considered non-fatal. 
     */
    UINT32 rsvd_25 : 7;
    /* rsvd_25 - Bits[31:25], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} ERRUNCSEV_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* ERRCORSTS_CB33_FUN_0_REG supported on:                                     */
/*       BDX (0x20200110)                                                     */
/* Register default value:              0x0000                                */
#define ERRCORSTS_CB33_FUN_0_REG 0x18002110

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * This register reports the error status of individual correctable error sources 
 * on a PCI Express device. An individual error status bit set to 1 indicates that 
 * a particular error has occurred. Software can clear the error status by writing 
 * a 1 to the respective bit. 
 */
typedef union {
  struct {
    UINT16 re : 1;
    /* re - Bits[0:0], RO, default = 1'b0 
       This register reports the error status of individual correctable error sources 
       on a PCI Express device. An individual error status bit set to 1 indicates that 
       a particular error has occurred. Software can clear the error status by writing 
       a 1 to the respective bit. 
     */
    UINT16 rsvd_1 : 5;
    /* rsvd_1 - Bits[5:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 btlpe : 1;
    /* btlpe - Bits[6:6], RO, default = 1'b0 
       This register reports the error status of individual correctable error sources 
       on a PCI Express device. An individual error status bit set to 1 indicates that 
       a particular error has occurred. Software can clear the error status by writing 
       a 1 to the respective bit. 
     */
    UINT16 bdllpe : 1;
    /* bdllpe - Bits[7:7], RO, default = 1'b0 
       This register reports the error status of individual correctable error sources 
       on a PCI Express device. An individual error status bit set to 1 indicates that 
       a particular error has occurred. Software can clear the error status by writing 
       a 1 to the respective bit. 
     */
    UINT16 rnre : 1;
    /* rnre - Bits[8:8], RO, default = 1'b0 
       This register reports the error status of individual correctable error sources 
       on a PCI Express device. An individual error status bit set to 1 indicates that 
       a particular error has occurred. Software can clear the error status by writing 
       a 1 to the respective bit. 
     */
    UINT16 rsvd_9 : 3;
    /* rsvd_9 - Bits[11:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 rtte : 1;
    /* rtte - Bits[12:12], RO, default = 1'b0 
       This register reports the error status of individual correctable error sources 
       on a PCI Express device. An individual error status bit set to 1 indicates that 
       a particular error has occurred. Software can clear the error status by writing 
       a 1 to the respective bit. 
     */
    UINT16 anfe : 1;
    /* anfe - Bits[13:13], RW1CS, default = 1'b0 
       This register reports the error status of individual correctable error sources 
       on a PCI Express device. An individual error status bit set to 1 indicates that 
       a particular error has occurred. Software can clear the error status by writing 
       a 1 to the respective bit. 
     */
    UINT16 cie : 1;
    /* cie - Bits[14:14], RO, default = 1'b0 
       This register reports the error status of individual correctable error sources 
       on a PCI Express device. An individual error status bit set to 1 indicates that 
       a particular error has occurred. Software can clear the error status by writing 
       a 1 to the respective bit. 
     */
    UINT16 hloe : 1;
    /* hloe - Bits[15:15], RO, default = 1'b0 
       This register reports the error status of individual correctable error sources 
       on a PCI Express device. An individual error status bit set to 1 indicates that 
       a particular error has occurred. Software can clear the error status by writing 
       a 1 to the respective bit. 
     */
  } Bits;
  UINT16 Data;
} ERRCORSTS_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* ERRCORMSK_CB33_FUN_0_REG supported on:                                     */
/*       BDX (0x20200114)                                                     */
/* Register default value:              0x0000                                */
#define ERRCORMSK_CB33_FUN_0_REG 0x18002114

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * This register controls the reporting of individual correctable errors via 
 * ERR_COR message. A masked error (respective bit set in mask register) is not 
 * reported to the host bridge by the switch. There is a mask bit corresponding to 
 * every bit in the Correctable Error Status Register. 
 */
typedef union {
  struct {
    UINT16 rem : 1;
    /* rem - Bits[0:0], RO, default = 1'b0 
       This register controls the reporting of individual correctable errors via 
       ERR_COR message. A masked error (respective bit set in mask register) is not 
       reported to the host bridge by the switch. There is a mask bit corresponding to 
       every bit in the Correctable Error Status Register. 
     */
    UINT16 rsvd_1 : 5;
    /* rsvd_1 - Bits[5:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 btlpem : 1;
    /* btlpem - Bits[6:6], RO, default = 1'b0 
       This register controls the reporting of individual correctable errors via 
       ERR_COR message. A masked error (respective bit set in mask register) is not 
       reported to the host bridge by the switch. There is a mask bit corresponding to 
       every bit in the Correctable Error Status Register. 
     */
    UINT16 bdllpem : 1;
    /* bdllpem - Bits[7:7], RO, default = 1'b0 
       This register controls the reporting of individual correctable errors via 
       ERR_COR message. A masked error (respective bit set in mask register) is not 
       reported to the host bridge by the switch. There is a mask bit corresponding to 
       every bit in the Correctable Error Status Register. 
     */
    UINT16 rnrem : 1;
    /* rnrem - Bits[8:8], RO, default = 1'b0 
       This register controls the reporting of individual correctable errors via 
       ERR_COR message. A masked error (respective bit set in mask register) is not 
       reported to the host bridge by the switch. There is a mask bit corresponding to 
       every bit in the Correctable Error Status Register. 
     */
    UINT16 rsvd_9 : 3;
    /* rsvd_9 - Bits[11:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 rttem : 1;
    /* rttem - Bits[12:12], RO, default = 1'b0 
       This register controls the reporting of individual correctable errors via 
       ERR_COR message. A masked error (respective bit set in mask register) is not 
       reported to the host bridge by the switch. There is a mask bit corresponding to 
       every bit in the Correctable Error Status Register. 
     */
    UINT16 anfem : 1;
    /* anfem - Bits[13:13], RWS, default = 1'b0 
       This register controls the reporting of individual correctable errors via 
       ERR_COR message. A masked error (respective bit set in mask register) is not 
       reported to the host bridge by the switch. There is a mask bit corresponding to 
       every bit in the Correctable Error Status Register. 
     */
    UINT16 ciem : 1;
    /* ciem - Bits[14:14], RO, default = 1'b0 
       This register controls the reporting of individual correctable errors via 
       ERR_COR message. A masked error (respective bit set in mask register) is not 
       reported to the host bridge by the switch. There is a mask bit corresponding to 
       every bit in the Correctable Error Status Register. 
     */
    UINT16 hloem : 1;
    /* hloem - Bits[15:15], RO, default = 1'b0 
       This register controls the reporting of individual correctable errors via 
       ERR_COR message. A masked error (respective bit set in mask register) is not 
       reported to the host bridge by the switch. There is a mask bit corresponding to 
       every bit in the Correctable Error Status Register. 
     */
  } Bits;
  UINT16 Data;
} ERRCORMSK_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* AERCAPCTL_CB33_FUN_0_REG supported on:                                     */
/*       BDX (0x20200118)                                                     */
/* Register default value:              0x0000                                */
#define AERCAPCTL_CB33_FUN_0_REG 0x18002118

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * This register gives the status and control for ECRC checks and also the pointer 
 * to the first uncorrectable error that happened. 
 */
typedef union {
  struct {
    UINT16 fep : 5;
    /* fep - Bits[4:0], ROS_V, default = 5'h0 
       This register gives the status and control for ECRC checks and also the pointer 
       to the first uncorrectable error that happened. 
     */
    UINT16 egc : 1;
    /* egc - Bits[5:5], RO, default = 1'b0 
       This register gives the status and control for ECRC checks and also the pointer 
       to the first uncorrectable error that happened. 
     */
    UINT16 ege : 1;
    /* ege - Bits[6:6], RO, default = 1'b0 
       This register gives the status and control for ECRC checks and also the pointer 
       to the first uncorrectable error that happened. 
     */
    UINT16 ecc : 1;
    /* ecc - Bits[7:7], RO, default = 1'b0 
       This register gives the status and control for ECRC checks and also the pointer 
       to the first uncorrectable error that happened. 
     */
    UINT16 ece : 1;
    /* ece - Bits[8:8], RO, default = 1'b0 
       This register gives the status and control for ECRC checks and also the pointer 
       to the first uncorrectable error that happened. 
     */
    UINT16 mhrc : 1;
    /* mhrc - Bits[9:9], RO, default = 1'b0 
       This register gives the status and control for ECRC checks and also the pointer 
       to the first uncorrectable error that happened. 
     */
    UINT16 mhre : 1;
    /* mhre - Bits[10:10], RO, default = 1'b0 
       This register gives the status and control for ECRC checks and also the pointer 
       to the first uncorrectable error that happened. 
     */
    UINT16 rsvd : 5;
    /* rsvd - Bits[15:11], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT16 Data;
} AERCAPCTL_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* AERHDRLOG1_CB33_FUN_0_REG supported on:                                    */
/*       BDX (0x4020011C)                                                     */
/* Register default value:              0x00000000                            */
#define AERHDRLOG1_CB33_FUN_0_REG 0x1800411C

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * 32This register reports the occurrence of the first error the uncorrectable 
 * error status sources for the CBDMA block on the IOSF interface. This register 
 * gets locked after the recording of the first error. 
 */
typedef union {
  struct {
    UINT32 tlphdrlog : 32;
    /* tlphdrlog - Bits[31:0], ROS_V, default = 32'b0 
       32This register reports the occurrence of the first error the uncorrectable 
       error status sources for the CBDMA block on the IOSF interface. This register 
       gets locked after the recording of the first error. 
     */
  } Bits;
  UINT32 Data;
} AERHDRLOG1_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* AERHDRLOG2_CB33_FUN_0_REG supported on:                                    */
/*       BDX (0x40200120)                                                     */
/* Register default value:              0x00000000                            */
#define AERHDRLOG2_CB33_FUN_0_REG 0x18004120

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * 32This register reports the occurrence of the first error the uncorrectable 
 * error status sources for the CBDMA block on the IOSF interface. This register 
 * gets locked after the recording of the first error. 
 */
typedef union {
  struct {
    UINT32 tlphdrlog : 32;
    /* tlphdrlog - Bits[31:0], ROS_V, default = 32'b0 
       32This register reports the occurrence of the first error the uncorrectable 
       error status sources for the CBDMA block on the IOSF interface. This register 
       gets locked after the recording of the first error. 
     */
  } Bits;
  UINT32 Data;
} AERHDRLOG2_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* AERHDRLOG3_CB33_FUN_0_REG supported on:                                    */
/*       BDX (0x40200124)                                                     */
/* Register default value:              0x00000000                            */
#define AERHDRLOG3_CB33_FUN_0_REG 0x18004124

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * 32This register reports the occurrence of the first error the uncorrectable 
 * error status sources for the CBDMA block on the IOSF interface. This register 
 * gets locked after the recording of the first error. 
 */
typedef union {
  struct {
    UINT32 tlphdrlog : 32;
    /* tlphdrlog - Bits[31:0], ROS_V, default = 32'b0 
       32This register reports the occurrence of the first error the uncorrectable 
       error status sources for the CBDMA block on the IOSF interface. This register 
       gets locked after the recording of the first error. 
     */
  } Bits;
  UINT32 Data;
} AERHDRLOG3_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* AERHDRLOG4_CB33_FUN_0_REG supported on:                                    */
/*       BDX (0x40200128)                                                     */
/* Register default value:              0x00000000                            */
#define AERHDRLOG4_CB33_FUN_0_REG 0x18004128

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * 32This register reports the occurrence of the first error the uncorrectable 
 * error status sources for the CBDMA block on the IOSF interface. This register 
 * gets locked after the recording of the first error. 
 */
typedef union {
  struct {
    UINT32 tlphdrlog : 32;
    /* tlphdrlog - Bits[31:0], ROS_V, default = 32'b0 
       32This register reports the occurrence of the first error the uncorrectable 
       error status sources for the CBDMA block on the IOSF interface. This register 
       gets locked after the recording of the first error. 
     */
  } Bits;
  UINT32 Data;
} AERHDRLOG4_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* ERRUNCDETMSK_CB33_FUN_0_REG supported on:                                  */
/*       BDX (0x40200140)                                                     */
/* Register default value:              0x00000000                            */
#define ERRUNCDETMSK_CB33_FUN_0_REG 0x18004140

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * A value of 1 masks the field and prevents detection as well as logging.
 */
typedef union {
  struct {
    UINT32 rsvd_0 : 4;
    /* rsvd_0 - Bits[3:0], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 dlpedm : 1;
    /* dlpedm - Bits[4:4], RWS, default = 1'b0 
       A value of 1 masks the field and prevents detection as well as logging.
     */
    UINT32 sldedm : 1;
    /* sldedm - Bits[5:5], RO, default = 1'b0 
       A value of 1 masks the field and prevents detection as well as logging.
     */
    UINT32 rsvd_6 : 6;
    /* rsvd_6 - Bits[11:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 ptlpedm : 1;
    /* ptlpedm - Bits[12:12], RWS, default = 1'b0 
       A value of 1 masks the field and prevents detection as well as logging.
     */
    UINT32 fcedm : 1;
    /* fcedm - Bits[13:13], RO, default = 1'b0 
       A value of 1 masks the field and prevents detection as well as logging.
     */
    UINT32 ctedm : 1;
    /* ctedm - Bits[14:14], RWS, default = 1'b0 
       A value of 1 masks the field and prevents detection as well as logging.
     */
    UINT32 caedm : 1;
    /* caedm - Bits[15:15], RWS, default = 1'b0 
       A value of 1 masks the field and prevents detection as well as logging.
     */
    UINT32 ucedm : 1;
    /* ucedm - Bits[16:16], RWS, default = 1'b0 
       A value of 1 masks the field and prevents detection as well as logging.
     */
    UINT32 roedm : 1;
    /* roedm - Bits[17:17], RO, default = 1'b0 
       A value of 1 masks the field and prevents detection as well as logging.
     */
    UINT32 mtlpedm : 1;
    /* mtlpedm - Bits[18:18], RWS, default = 1'b0 
       A value of 1 masks the field and prevents detection as well as logging.
     */
    UINT32 ecrcedm : 1;
    /* ecrcedm - Bits[19:19], RO, default = 1'b0 
       A value of 1 masks the field and prevents detection as well as logging.
     */
    UINT32 uredm : 1;
    /* uredm - Bits[20:20], RWS, default = 1'b0 
       A value of 1 masks the field and prevents detection as well as logging.
     */
    UINT32 acsedm : 1;
    /* acsedm - Bits[21:21], RO, default = 1'b0 
       A value of 1 masks the field and prevents detection as well as logging.
     */
    UINT32 uiedm : 1;
    /* uiedm - Bits[22:22], RWS, default = 1'b0 
       A value of 1 masks the field and prevents detection as well as logging.
     */
    UINT32 mcedm : 1;
    /* mcedm - Bits[23:23], RO, default = 1'b0 
       A value of 1 masks the field and prevents detection as well as logging.
     */
    UINT32 rsvd_24 : 8;
    /* rsvd_24 - Bits[31:24], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} ERRUNCDETMSK_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* ERRCORDETMSK_CB33_FUN_0_REG supported on:                                  */
/*       BDX (0x20200144)                                                     */
/* Register default value:              0x0000                                */
#define ERRCORDETMSK_CB33_FUN_0_REG 0x18002144

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT16 redm : 1;
    /* redm - Bits[0:0], RO, default = 1'b0 
       desc is missing
     */
    UINT16 rsvd_1 : 5;
    /* rsvd_1 - Bits[5:1], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 btlpedm : 1;
    /* btlpedm - Bits[6:6], RO, default = 1'b0 
       desc is missing
     */
    UINT16 bdllpedm : 1;
    /* bdllpedm - Bits[7:7], RO, default = 1'b0 
       desc is missing
     */
    UINT16 rnredm : 1;
    /* rnredm - Bits[8:8], RO, default = 1'b0 
       desc is missing
     */
    UINT16 rsvd_9 : 3;
    /* rsvd_9 - Bits[11:9], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 rttedm : 1;
    /* rttedm - Bits[12:12], RO, default = 1'b0 
       desc is missing
     */
    UINT16 anfedm : 1;
    /* anfedm - Bits[13:13], RWS, default = 1'b0 
       desc is missing
     */
    UINT16 ciedm : 1;
    /* ciedm - Bits[14:14], RWS, default = 1'b0 
       desc is missing
     */
    UINT16 hloedm : 1;
    /* hloedm - Bits[15:15], RO, default = 1'b0 
       desc is missing
     */
  } Bits;
  UINT16 Data;
} ERRCORDETMSK_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* CHANERR_AERMSK_CB33_FUN_0_REG supported on:                                */
/*       BDX (0x40200148)                                                     */
/* Register default value:              0x00000000                            */
#define CHANERR_AERMSK_CB33_FUN_0_REG 0x18004148

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * The Channel Error AER Mask Register controls which DMA error conditions cause an 
 * interrupt on the AER path. THis is independent of the native CBDMA error 
 * escalation path. When a bit in the CHANERR_AERMSK register is set to one, then 
 * the corresponding bit in the CHANERR register for the given channel is inhibited 
 * from generating an interrupt on the AER path. Refer to the CBDMA RAS 
 * architecture in the base chapter. 
 */
typedef union {
  struct {
    UINT32 chanerr_aermsk1 : 28;
    /* chanerr_aermsk1 - Bits[27:0], RWS, default = 28'b0 
       The Channel Error AER Mask Register controls which DMA error conditions cause an 
       interrupt on the AER path. THis is independent of the native CBDMA error 
       escalation path. When a bit in the CHANERR_AERMSK register is set to one, then 
       the corresponding bit in the CHANERR register for the given channel is inhibited 
       from generating an interrupt on the AER path. Refer to the CBDMA RAS 
       architecture in the base chapter. 
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} CHANERR_AERMSK_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MCSTCAPHDR_CB33_FUN_0_REG supported on:                                    */
/*       BDX (0x40200150)                                                     */
/* Register default value:              0x00010012                            */
#define MCSTCAPHDR_CB33_FUN_0_REG 0x18004150

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 ecid : 16;
    /* ecid - Bits[15:0], RO, default = 16'h0012 
       desc is missing
     */
    UINT32 cv : 4;
    /* cv - Bits[19:16], RO, default = 4'h1 
       desc is missing
     */
    UINT32 nco : 12;
    /* nco - Bits[31:20], RWS_L, default = 12'h0 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} MCSTCAPHDR_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MCSTCAP_CB33_FUN_0_REG supported on:                                       */
/*       BDX (0x20200154)                                                     */
/* Register default value:              0x000F                                */
#define MCSTCAP_CB33_FUN_0_REG 0x18002154

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT16 mcmg : 6;
    /* mcmg - Bits[5:0], RO, default = 6'hF 
       desc is missing
     */
    UINT16 rsvd_6 : 2;
    /* rsvd_6 - Bits[7:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 mcwinsz : 6;
    /* mcwinsz - Bits[13:8], RWS_L, default = 6'h0 
       desc is missing
     */
    UINT16 rsvd_14 : 1;
    /* rsvd_14 - Bits[14:14], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 mcers : 1;
    /* mcers - Bits[15:15], RO, default = 1'b0 
       desc is missing
     */
  } Bits;
  UINT16 Data;
} MCSTCAP_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MCSTCTL_CB33_FUN_0_REG supported on:                                       */
/*       BDX (0x20200156)                                                     */
/* Register default value:              0x0000                                */
#define MCSTCTL_CB33_FUN_0_REG 0x18002156

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT16 mcng : 6;
    /* mcng - Bits[5:0], RWS, default = 6'h0 
       desc is missing
     */
    UINT16 rsvd : 9;
    /* rsvd - Bits[14:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 mcen : 1;
    /* mcen - Bits[15:15], RWS, default = 1'h0 
       desc is missing
     */
  } Bits;
  UINT16 Data;
} MCSTCTL_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MCSTBAR_CB33_FUN_0_REG supported on:                                       */
/*       BDX (0x40200158)                                                     */
/* Register default value:              0x0000000C                            */
#define MCSTBAR_CB33_FUN_0_REG 0x18004158

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * 32This register is 4K aligned and hence only the top 20 bits are useable for the 
 * base address. 
 */
typedef union {
  struct {
    UINT32 mcip : 6;
    /* mcip - Bits[5:0], RWS, default = 6'h0C 
       32This register is 4K aligned and hence only the top 20 bits are useable for the 
       base address. 
     */
    UINT32 rsvd : 6;
    /* rsvd - Bits[11:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 l32adr : 20;
    /* l32adr - Bits[31:12], RWS, default = 20'h0 
       32This register is 4K aligned and hence only the top 20 bits are useable for the 
       base address. 
     */
  } Bits;
  UINT32 Data;
} MCSTBAR_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MCSTUBAR_CB33_FUN_0_REG supported on:                                      */
/*       BDX (0x4020015C)                                                     */
/* Register default value:              0x00000000                            */
#define MCSTUBAR_CB33_FUN_0_REG 0x1800415C

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 u32adr : 32;
    /* u32adr - Bits[31:0], RWS, default = 32'h0 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} MCSTUBAR_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MCSTRCV_CB33_FUN_0_REG supported on:                                       */
/*       BDX (0x20200160)                                                     */
/* Register default value:              0x0000                                */
#define MCSTRCV_CB33_FUN_0_REG 0x18002160

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * The MC_Receive register provides a bit vector denoting which Multicast groups 
 * the Function should accept 
 */
typedef union {
  struct {
    UINT16 mcr : 16;
    /* mcr - Bits[15:0], RWS, default = 16'h0 
       The MC_Receive register provides a bit vector denoting which Multicast groups 
       the Function should accept 
     */
  } Bits;
  UINT16 Data;
} MCSTRCV_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MCSTBLKALL_CB33_FUN_0_REG supported on:                                    */
/*       BDX (0x20200168)                                                     */
/* Register default value:              0x0000                                */
#define MCSTBLKALL_CB33_FUN_0_REG 0x18002168

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * The incoming transaction will be routed to its destination (memory) while the 
 * spawned transaction (if a multicast hit occurred) will be sent to the other port 
 * (NTB), which will be controlled by the MC Receive setting in the NTB (iep) 
 * assuming that the chosen MC group is unblocked in this register. 
 */
typedef union {
  struct {
    UINT16 mcba : 16;
    /* mcba - Bits[15:0], RWS, default = 16'h0 
       The incoming transaction will be routed to its destination (memory) while the 
       spawned transaction (if a multicast hit occurred) will be sent to the other port 
       (NTB), which will be controlled by the MC Receive setting in the NTB (iep) 
       assuming that the chosen MC group is unblocked in this register. 
     */
  } Bits;
  UINT16 Data;
} MCSTBLKALL_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MCSTOLBAR_CB33_FUN_0_REG supported on:                                     */
/*       BDX (0x40200178)                                                     */
/* Register default value:              0x00000000                            */
#define MCSTOLBAR_CB33_FUN_0_REG 0x18004178

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * 32if (MC_Overlay_Size andlt; 6) then
 * [br]Egress_TLP_Addr = Ingress_TLP_Addr,
 * [br]else 
 * [br]Egress_TLP_Addr = MC_Overlay_BAR[63:MC_Overlay_Size], 
 * Ingress_TLP_Addr[MC_Overlay_Size-1:0], 
 * [br]end
 */
typedef union {
  struct {
    UINT32 mcovrsz : 6;
    /* mcovrsz - Bits[5:0], RWS, default = 6'h00 
       32if (MC_Overlay_Size andlt; 6) then
       [br]Egress_TLP_Addr = Ingress_TLP_Addr,
       [br]else 
       [br]Egress_TLP_Addr = MC_Overlay_BAR[63:MC_Overlay_Size], 
       Ingress_TLP_Addr[MC_Overlay_Size-1:0], 
       [br]end
     */
    UINT32 rsvd : 6;
    /* rsvd - Bits[11:6], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT32 mcovrbar : 20;
    /* mcovrbar - Bits[31:12], RWS, default = 20'h0 
       32if (MC_Overlay_Size andlt; 6) then
       [br]Egress_TLP_Addr = Ingress_TLP_Addr,
       [br]else 
       [br]Egress_TLP_Addr = MC_Overlay_BAR[63:MC_Overlay_Size], 
       Ingress_TLP_Addr[MC_Overlay_Size-1:0], 
       [br]end
     */
  } Bits;
  UINT32 Data;
} MCSTOLBAR_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* MCSTUOLBAR_CB33_FUN_0_REG supported on:                                    */
/*       BDX (0x4020017C)                                                     */
/* Register default value:              0x00000000                            */
#define MCSTUOLBAR_CB33_FUN_0_REG 0x1800417C

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 mcovrbaru32 : 32;
    /* mcovrbaru32 - Bits[31:0], RWS, default = 32'h0 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} MCSTUOLBAR_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* FERR_CHANERR_CB33_FUN_0_REG supported on:                                  */
/*       BDX (0x40200180)                                                     */
/* Register default value:              0x00000000                            */
#define FERR_CHANERR_CB33_FUN_0_REG 0x18004180

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * 32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
 * with DWBES set, then later another RAID P Error is detected on the same channel 
 * (the second error may be in the same descriptor or a different descriptor), the 
 * NERR_CHANERR will be set for the second RAID P Error. This is true for all 
 * errors effected by DWBES. For errors that halt the channel it may be possible to 
 * detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
 * first error, which will halt the channel, is detected it takes a few clocks in 
 * the RTL to get into the halt flow state machine. If a second error occurs before 
 * the channel is in the halt flow then it will be logged in NERR_CHANERR. 
 */
typedef union {
  struct {
    UINT32 xsadderr_dma0 : 1;
    /* xsadderr_dma0 - Bits[0:0], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 xdadderr_dma1 : 1;
    /* xdadderr_dma1 - Bits[1:1], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 ndaderr_dma2 : 1;
    /* ndaderr_dma2 - Bits[2:2], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 derr_dma3 : 1;
    /* derr_dma3 - Bits[3:3], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 chaderr_dma4 : 1;
    /* chaderr_dma4 - Bits[4:4], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 chancmd_error_dma5 : 1;
    /* chancmd_error_dma5 - Bits[5:5], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 cuncorerr_dma6 : 1;
    /* cuncorerr_dma6 - Bits[6:6], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 duncorerr_dma7 : 1;
    /* duncorerr_dma7 - Bits[7:7], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 rderr_dma8 : 1;
    /* rderr_dma8 - Bits[8:8], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 wderr_dma9 : 1;
    /* wderr_dma9 - Bits[9:9], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 dcerr_dma10 : 1;
    /* dcerr_dma10 - Bits[10:10], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 dsxerr_dma11 : 1;
    /* dsxerr_dma11 - Bits[11:11], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 cmpadderr_dma12 : 1;
    /* cmpadderr_dma12 - Bits[12:12], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 intcfgerr_dma13 : 1;
    /* intcfgerr_dma13 - Bits[13:13], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 sederr_dma14 : 1;
    /* sederr_dma14 - Bits[14:14], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 uaerr_dma15 : 1;
    /* uaerr_dma15 - Bits[15:15], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 cxperr_dma16 : 1;
    /* cxperr_dma16 - Bits[16:16], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 xqerr_dma17 : 1;
    /* xqerr_dma17 - Bits[17:17], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 dcnterr_dma18 : 1;
    /* dcnterr_dma18 - Bits[18:18], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 differr_dma19 : 1;
    /* differr_dma19 - Bits[19:19], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 gtverr_dma20 : 1;
    /* gtverr_dma20 - Bits[20:20], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 atverr_dma21 : 1;
    /* atverr_dma21 - Bits[21:21], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 rtverr_dma22 : 1;
    /* rtverr_dma22 - Bits[22:22], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 bberr_dma23 : 1;
    /* bberr_dma23 - Bits[23:23], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 rdifferr_dma24 : 1;
    /* rdifferr_dma24 - Bits[24:24], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 rgtverr_dma25 : 1;
    /* rgtverr_dma25 - Bits[25:25], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 ratverr_dma26 : 1;
    /* ratverr_dma26 - Bits[26:26], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 rrtverr_dma27 : 1;
    /* rrtverr_dma27 - Bits[27:27], ROS_V, default = 1'b0 
       32Illustrative example: if FERR_CHANERR is set due to a RAID P Error (*DMA16) 
       with DWBES set, then later another RAID P Error is detected on the same channel 
       (the second error may be in the same descriptor or a different descriptor), the 
       NERR_CHANERR will be set for the second RAID P Error. This is true for all 
       errors effected by DWBES. For errors that halt the channel it may be possible to 
       detect both FERR_CHANERR and NERR_CHANERR on the same descriptor. After the 
       first error, which will halt the channel, is detected it takes a few clocks in 
       the RTL to get into the halt flow state machine. If a second error occurs before 
       the channel is in the halt flow then it will be logged in NERR_CHANERR. 
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} FERR_CHANERR_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* FERR_DMACOUNT_CB33_FUN_0_REG supported on:                                 */
/*       BDX (0x20200184)                                                     */
/* Register default value:              0x0000                                */
#define FERR_DMACOUNT_CB33_FUN_0_REG 0x18002184

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * This register records the DMACOUNT register when an FERR occurred.
 */
typedef union {
  struct {
    UINT16 numberofdescriptorstoprocess : 16;
    /* numberofdescriptorstoprocess - Bits[15:0], ROS_V, default = 16'b0 
       This register records the DMACOUNT register when an FERR occurred.
     */
  } Bits;
  UINT16 Data;
} FERR_DMACOUNT_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* FERR_INTDMACOUNT_CB33_FUN_0_REG supported on:                              */
/*       BDX (0x20200186)                                                     */
/* Register default value:              0x0000                                */
#define FERR_INTDMACOUNT_CB33_FUN_0_REG 0x18002186

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * This register records the DMACOUNT register when an FERR occurred.
 */
typedef union {
  struct {
    UINT16 numberofdescriptorstoprocess : 16;
    /* numberofdescriptorstoprocess - Bits[15:0], ROS_V, default = 16'b0 
       This register records the DMACOUNT register when an FERR occurred.
     */
  } Bits;
  UINT16 Data;
} FERR_INTDMACOUNT_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* FERR_CHANSTS_N0_CB33_FUN_0_REG supported on:                               */
/*       BDX (0x40200188)                                                     */
/* Register default value:              0x00000000                            */
#define FERR_CHANSTS_N0_CB33_FUN_0_REG 0x18004188

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 dmaxsts : 3;
    /* dmaxsts - Bits[2:0], ROS_V, default = 3'h0 
       desc is missing
     */
    UINT32 reserved : 3;
    /* reserved - Bits[5:3], ROS_V, default = 3'b0 
       desc is missing
     */
    UINT32 cmpda : 26;
    /* cmpda - Bits[31:6], ROS_V, default = 58'h0 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} FERR_CHANSTS_N0_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* FERR_CHANSTS_N1_CB33_FUN_0_REG supported on:                               */
/*       BDX (0x4020018C)                                                     */
/* Register default value:              0x00000000                            */
#define FERR_CHANSTS_N1_CB33_FUN_0_REG 0x1800418C

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT32 cmpda : 32;
    /* cmpda - Bits[31:0], ROS_V, default = 58'h0 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       desc is missing
     */
  } Bits;
  UINT32 Data;
} FERR_CHANSTS_N1_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* FERR_DESC_CTRL_CB33_FUN_0_REG supported on:                                */
/*       BDX (0x40200190)                                                     */
/* Register default value:              0x00000000                            */
#define FERR_DESC_CTRL_CB33_FUN_0_REG 0x18004190

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * This register contains the desc_ctrl fields extracted from the current 
 * descriptor when an error happened. 
 */
typedef union {
  struct {
    UINT32 desc_ctrl : 32;
    /* desc_ctrl - Bits[31:0], ROS_V, default = 32'h0 
       This register contains the desc_ctrl fields extracted from the current 
       descriptor when an error happened. 
     */
  } Bits;
  UINT32 Data;
} FERR_DESC_CTRL_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* FERR_FRSDIF_CB33_FUN_0_REG supported on:                                   */
/*       BDX (0x10200194)                                                     */
/* Register default value:              0x00                                  */
#define FERR_FRSDIF_CB33_FUN_0_REG 0x18001194

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * This register records the first occurrence of the RAID source number that 
 * encountered a DIF error (including Guard, App, Ref tag, All F detect errors 
 * [DMA19-22]) . It applies to all opcodes involving RAID w/DIF supported by 
 * Briarwood. When Software clears the corresponding CHANERR bits, this register 
 * will be cleared by hardware 
 */
typedef union {
  struct {
    UINT8 frsnum : 5;
    /* frsnum - Bits[4:0], ROS_V, default = 5'h0 
       This register records the first occurrence of the RAID source number that 
       encountered a DIF error (including Guard, App, Ref tag, All F detect errors 
       [DMA19-22]) . It applies to all opcodes involving RAID w/DIF supported by 
       Briarwood. When Software clears the corresponding CHANERR bits, this register 
       will be cleared by hardware 
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} FERR_FRSDIF_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* FERR_CADDR_N0_CB33_FUN_0_REG supported on:                                 */
/*       BDX (0x40200198)                                                     */
/* Register default value:              0x00000000                            */
#define FERR_CADDR_N0_CB33_FUN_0_REG 0x18004198

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * This register records the address of the last descriptor when error happened
 */
typedef union {
  struct {
    UINT32 cdaddr : 32;
    /* cdaddr - Bits[31:0], ROS_V, default = 64'h0 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This register records the address of the last descriptor when error happened
     */
  } Bits;
  UINT32 Data;
} FERR_CADDR_N0_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* FERR_CADDR_N1_CB33_FUN_0_REG supported on:                                 */
/*       BDX (0x4020019C)                                                     */
/* Register default value:              0x00000000                            */
#define FERR_CADDR_N1_CB33_FUN_0_REG 0x1800419C

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * This register records the address of the last descriptor when error happened
 */
typedef union {
  struct {
    UINT32 cdaddr : 32;
    /* cdaddr - Bits[31:0], ROS_V, default = 64'h0 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This register records the address of the last descriptor when error happened
     */
  } Bits;
  UINT32 Data;
} FERR_CADDR_N1_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* FERR_NADDR_N0_CB33_FUN_0_REG supported on:                                 */
/*       BDX (0x402001A0)                                                     */
/* Register default value:              0x00000000                            */
#define FERR_NADDR_N0_CB33_FUN_0_REG 0x180041A0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * This register records the next address of the current descriptor chain which 
 * encountered an error. 
 */
typedef union {
  struct {
    UINT32 nxt_addr : 32;
    /* nxt_addr - Bits[31:0], ROS_V, default = 64'h0 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This register records the next address of the current descriptor chain which 
       encountered an error. 
     */
  } Bits;
  UINT32 Data;
} FERR_NADDR_N0_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* FERR_NADDR_N1_CB33_FUN_0_REG supported on:                                 */
/*       BDX (0x402001A4)                                                     */
/* Register default value:              0x00000000                            */
#define FERR_NADDR_N1_CB33_FUN_0_REG 0x180041A4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * This register records the next address of the current descriptor chain which 
 * encountered an error. 
 */
typedef union {
  struct {
    UINT32 nxt_addr : 32;
    /* nxt_addr - Bits[31:0], ROS_V, default = 64'h0 
       This field was split. It's value spans this field and the field of similar name 
       in the next/preceding struct. 
       This register records the next address of the current descriptor chain which 
       encountered an error. 
     */
  } Bits;
  UINT32 Data;
} FERR_NADDR_N1_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* NERR_CHANERR_CB33_FUN_0_REG supported on:                                  */
/*       BDX (0x402001A8)                                                     */
/* Register default value:              0x00000000                            */
#define NERR_CHANERR_CB33_FUN_0_REG 0x180041A8

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * The Next Error Channel Error Register records the second error condition 
 * occurring within a given DMA channel for the function as described earlier. 
 */
typedef union {
  struct {
    UINT32 xsadderr_dma0 : 1;
    /* xsadderr_dma0 - Bits[0:0], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 xdadderr_dma1 : 1;
    /* xdadderr_dma1 - Bits[1:1], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 ndaderr_dma2 : 1;
    /* ndaderr_dma2 - Bits[2:2], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 derr_dma3 : 1;
    /* derr_dma3 - Bits[3:3], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 chaderr_dma4 : 1;
    /* chaderr_dma4 - Bits[4:4], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 chancmd_error_dma5 : 1;
    /* chancmd_error_dma5 - Bits[5:5], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 cuncorerr_dma6 : 1;
    /* cuncorerr_dma6 - Bits[6:6], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 duncorerr_dma7 : 1;
    /* duncorerr_dma7 - Bits[7:7], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 rderr_dma8 : 1;
    /* rderr_dma8 - Bits[8:8], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 wderr_dma9 : 1;
    /* wderr_dma9 - Bits[9:9], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 dcerr_dma10 : 1;
    /* dcerr_dma10 - Bits[10:10], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 dsxerr_dma11 : 1;
    /* dsxerr_dma11 - Bits[11:11], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 cmpadderr_dma12 : 1;
    /* cmpadderr_dma12 - Bits[12:12], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 intcfgerr_dma13 : 1;
    /* intcfgerr_dma13 - Bits[13:13], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 sederr_dma15 : 1;
    /* sederr_dma15 - Bits[14:14], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 uaerr_dma15 : 1;
    /* uaerr_dma15 - Bits[15:15], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 cxperr_dma16 : 1;
    /* cxperr_dma16 - Bits[16:16], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 xqerr_dma17 : 1;
    /* xqerr_dma17 - Bits[17:17], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 dcnterr_dma18 : 1;
    /* dcnterr_dma18 - Bits[18:18], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 differr_dma19 : 1;
    /* differr_dma19 - Bits[19:19], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 gtverr_dma20 : 1;
    /* gtverr_dma20 - Bits[20:20], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 atverr_dma21 : 1;
    /* atverr_dma21 - Bits[21:21], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 rtverr_dma22 : 1;
    /* rtverr_dma22 - Bits[22:22], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 bberr_dma23 : 1;
    /* bberr_dma23 - Bits[23:23], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 rdifferr_dma24 : 1;
    /* rdifferr_dma24 - Bits[24:24], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 rgtverr_dma25 : 1;
    /* rgtverr_dma25 - Bits[25:25], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 ratverr_dma26 : 1;
    /* ratverr_dma26 - Bits[26:26], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 rrtverr_dma27 : 1;
    /* rrtverr_dma27 - Bits[27:27], ROS_V, default = 1'b0 
       The Next Error Channel Error Register records the second error condition 
       occurring within a given DMA channel for the function as described earlier. 
     */
    UINT32 rsvd : 4;
    /* rsvd - Bits[31:28], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT32 Data;
} NERR_CHANERR_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* CBERRINJCTL_CB33_FUN_0_REG supported on:                                   */
/*       BDX (0x202001B0)                                                     */
/* Register default value:              0x0000                                */
#define CBERRINJCTL_CB33_FUN_0_REG 0x180021B0

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * This register controls error injection on the CBDMA internal storage for 
 * corrupting data per function. 
 */
typedef union {
  struct {
    UINT16 errinjen : 1;
    /* errinjen - Bits[0:0], RWS, default = 1'b0 
       This register controls error injection on the CBDMA internal storage for 
       corrupting data per function. 
     */
    UINT16 eirfs : 1;
    /* eirfs - Bits[1:1], RWS, default = 1'b0 
       This register controls error injection on the CBDMA internal storage for 
       corrupting data per function. 
     */
    UINT16 rspdonedis : 1;
    /* rspdonedis - Bits[2:2], RWS, default = 1'b0 
       This register controls error injection on the CBDMA internal storage for 
       corrupting data per function. 
     */
    UINT16 eips : 1;
    /* eips - Bits[3:3], RWS, default = 1'b0 
       This register controls error injection on the CBDMA internal storage for 
       corrupting data per function. 
     */
    UINT16 rsvd : 4;
    /* rsvd - Bits[7:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
    UINT16 dpf : 8;
    /* dpf - Bits[15:8], RWS, default = 8'h00 
       This register controls error injection on the CBDMA internal storage for 
       corrupting data per function. 
     */
  } Bits;
  UINT16 Data;
} CBERRINJCTL_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* CBPERRSTSLOG_CB33_FUN_0_REG supported on:                                  */
/*       BDX (0x102001B2)                                                     */
/* Register default value:              0x00                                  */
#define CBPERRSTSLOG_CB33_FUN_0_REG 0x180011B2

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * This register records a log of the parity error detected and the status of the 
 * error injection in the CBDMA function. 
 */
typedef union {
  struct {
    UINT8 ped : 1;
    /* ped - Bits[0:0], RW1CS, default = 1'b0 
       This register records a log of the parity error detected and the status of the 
       error injection in the CBDMA function. 
     */
    UINT8 pedcr : 1;
    /* pedcr - Bits[1:1], RW1CS, default = 1'b0 
       This register records a log of the parity error detected and the status of the 
       error injection in the CBDMA function. 
     */
    UINT8 pecbr : 1;
    /* pecbr - Bits[2:2], RW1CS, default = 1'b0 
       This register records a log of the parity error detected and the status of the 
       error injection in the CBDMA function. 
     */
    UINT8 pewbr : 1;
    /* pewbr - Bits[3:3], RW1CS, default = 1'b0 
       This register records a log of the parity error detected and the status of the 
       error injection in the CBDMA function. 
     */
    UINT8 errinjsts : 1;
    /* errinjsts - Bits[4:4], RW1CS, default = 1'b0 
       This register records a log of the parity error detected and the status of the 
       error injection in the CBDMA function. 
     */
    UINT8 rsvd : 3;
    /* rsvd - Bits[7:5], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} CBPERRSTSLOG_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* CBPERRDEF_CB33_FUN_0_REG supported on:                                     */
/*       BDX (0x102001B4)                                                     */
/* Register default value:              0x00                                  */
#define CBPERRDEF_CB33_FUN_0_REG 0x180011B4

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 * This register is used for masking the injected parity error in the CBDMA 
 * function. 
 */
typedef union {
  struct {
    UINT8 disped : 1;
    /* disped - Bits[0:0], RWS, default = 1'b0 
       This register is used for masking the injected parity error in the CBDMA 
       function. 
     */
    UINT8 dispedcr : 1;
    /* dispedcr - Bits[1:1], RWS, default = 1'b0 
       This register is used for masking the injected parity error in the CBDMA 
       function. 
     */
    UINT8 dispecbr : 1;
    /* dispecbr - Bits[2:2], RWS, default = 1'b0 
       This register is used for masking the injected parity error in the CBDMA 
       function. 
     */
    UINT8 dispewbr : 1;
    /* dispewbr - Bits[3:3], RWS, default = 1'b0 
       This register is used for masking the injected parity error in the CBDMA 
       function. 
     */
    UINT8 rsvd : 4;
    /* rsvd - Bits[7:4], n/a, default = n/a 
       Padding added by header generation tool.
     */
  } Bits;
  UINT8 Data;
} CBPERRDEF_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


/* CBEINJRSP_CB33_FUN_0_REG supported on:                                     */
/*       BDX (0x102001B6)                                                     */
/* Register default value:              0xCA                                  */
#define CBEINJRSP_CB33_FUN_0_REG 0x180011B6

#ifdef BDX_HOST
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\2.0.0.CFG.xml.
 */
typedef union {
  struct {
    UINT8 start : 4;
    /* start - Bits[3:0], RWS, default = 4'b1010 
       desc is missing
     */
    UINT8 stop : 4;
    /* stop - Bits[7:4], RWS, default = 4'b1100 
       desc is missing
     */
  } Bits;
  UINT8 Data;
} CBEINJRSP_CB33_FUN_0_STRUCT;
#endif /* ASM_INC */
#endif /*BDX_HOST */


#endif /* CB33_FUN_0_h */
