/*++
// **************************************************************************
// *                                                                        *
// *                                                                        *
// *      ESS - Enterprise Silicon Software                                 *
// *                                                                        *
// *      Copyright (c) 2004 - 2016 Intel Corp.                             *
// *                                                                        *
// *      This program has been developed by Intel Corporation.             *
// *      Licensee has Intel's permission to incorporate this source code   *
// *      into their product, royalty free.  This source code may NOT be    *
// *      redistributed to anyone without Intel's written permission.       *
// *                                                                        *
// *      Intel specifically disclaims all warranties, express or           *
// *      implied, and all liability, including consequential and other     *
// *      indirect damages, for the use of this code, including liability   *
// *      for infringement of any proprietary rights, and including the     *
// *      warranties of merchantability and fitness for a particular        *
// *      purpose.  Intel does not assume any responsibility for any        *
// *      errors which may appear in this code nor any responsibility to    *
// *      update it.                                                        *
// *                                                                        *
// **************************************************************************
// **************************************************************************
// *                                                                        *
// *  PURPOSE:                                                              *
// *                                                                        *
// *      This file contains EV related definitions.                        *
// *                                                                        *
// **************************************************************************
--*/

#ifndef __EV_AUTO_RECIPE_H__
#define __EV_AUTO_RECIPE_H__

#define EV_AUTO_RECIPE_SIGNATURE  0x5F56455F

// Wild card definitions
//
#define WILDCARD_8     (0xFF)
#define WILDCARD_16    (0xFFFF)
#define WILDCARD_32    (0xFFFFFFFF)
#define WILDCARD_64    (0xFFFFFFFFFFFFFFFF)


typedef enum {
  TYPE_CSR = 0,
  TYPE_MSR,
  TYPE_IO,
  TYPE_QPI_IAR,
  TYPE_WINDOW2,
  ACCESS_TYPE_MAX,
} ACCESS_TYPE;

typedef enum {
  WINDOW1_IAR_IOVB = 1,
  WINDOW1_IAR_IOVB_BCAST,
  WINDOW1_IAR_IOVC,
  WINDOW1_SUB_TYPE_MAX,
} WINDOW1_SUB_TYPE;

typedef enum {
  A0_STEP = 0,
  A1_STEP,
  A2_STEP,
  A3_STEP,
  B0_STEP,
  B1_STEP,
  B2_STEP,
  B3_STEP,
  C0_STEP,
  C1_STEP,
  C2_STEP,
  C3_STEP,
  D0_STEP,
  D1_STEP,
  D2_STEP,
  D3_STEP,
  L0_STEP,
  L1_STEP,
  L2_STEP,
  L3_STEP,
  M0_STEP,
  M1_STEP,
  M2_STEP,
  M3_STEP,
  R0_STEP,
  R1_STEP,
  R2_STEP,
  R3_STEP,
  S0_STEP,
  S1_STEP,
  S2_STEP,
  S3_STEP,
  T0_STEP,
  T1_STEP,
  T2_STEP,
  T3_STEP,
  U0_STEP,
  U1_STEP,
  U2_STEP,
  U3_STEP,
  V0_STEP,
  V1_STEP,
  V2_STEP,
  V3_STEP,
  Y0_STEP,
  Y1_STEP,
  STEPPINGS_TYPE_MAX,
} STEPPINGS_TYPE;

typedef enum {
  QPI_RATE_56GT    = 0,
  QPI_RATE_64GT,
  QPI_RATE_72GT,
  QPI_RATE_80GT,
  QPI_RATE_88GT,
  QPI_RATE_96GT,
  MAX_QPI_RATE,
} QPI_RATE_TYPE;

//Place holder for future use in PCIe
typedef enum {
  PCIE_RATE_25GT    = 0,
  PCIE_RATE_50GT,
  PCIE_RATE_80GT,
  MAX_PCIE_RATE,
} PCIE_RATE_TYPE;

typedef enum {
  JKT_SOCKET = 0,
  IVT_SOCKET,
  HSX_SOCKET,
  BDX_SOCKET,
  SKX_SOCKET,
  PROCESSOR_TYPE_MAX,
} PROCESSOR_TYPE;

typedef enum {
  EN_SKU_TYPE = 0,
  EP_SKU_TYPE,
  EX_SKU_TYPE,
  DE_SKU_TYPE,
  PROCESSOR_SKU_TYPE_MAX,
} PROCESSOR_SKU_TYPE;

//
// Steppings
//
#define Ax_REV  1 << A3_STEP | 1 << A2_STEP | 1 << A1_STEP | 1 << A0_STEP
#define Bx_REV  1 << B3_STEP | 1 << B2_STEP | 1 << B1_STEP | 1 << B0_STEP
#define Cx_REV  1 << C3_STEP | 1 << C2_STEP | 1 << C1_STEP | 1 << C0_STEP
#define Dx_REV  1 << D3_STEP | 1 << D2_STEP | 1 << D1_STEP | 1 << D0_STEP

#define Lx_REV  1 << L3_STEP | 1 << L2_STEP | 1 << L1_STEP | 1 << L0_STEP
#define Mx_REV  1 << M3_STEP | 1 << M2_STEP | 1 << M1_STEP | 1 << M0_STEP
#define Rx_REV  1 << R3_STEP | 1 << R2_STEP | 1 << R1_STEP | 1 << R0_STEP
#define Sx_REV  1 << S3_STEP | 1 << S2_STEP | 1 << S1_STEP | 1 << S0_STEP

#define Tx_REV  ((UINT64)1 << T3_STEP) | ((UINT64)1 << T2_STEP) | ((UINT64)1 << T1_STEP) | ((UINT64)1 << T0_STEP)
#define Ux_REV  ((UINT64)1 << U3_STEP) | ((UINT64)1 << U2_STEP) | ((UINT64)1 << U1_STEP) | ((UINT64)1 << U0_STEP)
#define Vx_REV  ((UINT64)1 << V3_STEP) | ((UINT64)1 << V2_STEP) | ((UINT64)1 << V1_STEP) | ((UINT64)1 << V0_STEP)

//#define Tx_REV  1 << T3_STEP | 1 << T2_STEP | 1 << T1_STEP | 1 << T0_STEP
//#define Ux_REV  1 << U3_STEP | 1 << U2_STEP | 1 << U1_STEP | 1 << U0_STEP
//#define Vx_REV  1 << V3_STEP | 1 << V2_STEP | 1 << V1_STEP | 1 << V0_STEP

//
//  Macros to get Window 1 access's sub-type and address
//
#define WINDOW1_ACCESS_SUB_TYPE(x)	((x & 0xF0000000) >> 28)
#define WINDOW1_ACCESS_REG_ADDR(x)	(x & 0xFF)

//
//  Macros to form Window 1 based address from sub-type and reg addr
//
#define WINDOW1_ADDR(subtype, regaddr)  ((subtype << 28) | (regaddr & 0xFF))

typedef struct {
  UINT8           AccessType;            // One of ACCESS_TYPE
  UINT8           RegisterSize;
  UINT32          RegisterAddress;       // 32 bit MMCFG encoded address
  UINT32          FieldMask;             // 32 bit field mask to AND field to zero
  UINT32          SocBitmap;             // * = -1, Else set bit for each valid socket to program
  UINT32          PortBitmap;            // * = -1, Else set bit for each port
  UINT32          BitRates;              // Bitmask of rates. See Enum *_RATE_TYPE
  UINT64          Steppings;             //   1 << A0_REV_IVT   * = -1    //Will need to evaluate how we handle this dynamically
  UINT32          ProcessorType;         //   IVT_SOCKET
  UINT16          Skus;                  //   1 << EP_SKU    * = -1
  UINT16          Chops;                 //   Ignore for now
  UINT32          Data;                  // Value to write i.e data << start bit
} EV_RECIPE_ENTRY;

#define REV_STR_CHARS                    8
#define HELP_STR_CHARS                   64

typedef struct {
  UINT32  Signature;                         // "_VE_"
  CHAR8   Revision[REV_STR_CHARS];           // 1
  UINT16  NumberEntries;                     //
  UINT8   DebugMode;                         // 0 or 1
  CHAR8   HelperString[HELP_STR_CHARS];      // fill in uniPhy, table rev and build date 
} EV_RECIPE_HEADER;

#endif
