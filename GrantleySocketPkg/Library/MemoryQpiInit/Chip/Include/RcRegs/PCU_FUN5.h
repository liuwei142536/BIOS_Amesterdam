/* Date Stamp: 8/23/2014 */

#ifndef PCU_FUN5_h
#define PCU_FUN5_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* PCU_FUN5_IVT_DEV 10                                                        */
/* PCU_FUN5_IVT_FUN 5                                                         */
/* For HSX_HOST:                                                              */
/* PCU_FUN5_HSX_DEV 30                                                        */
/* PCU_FUN5_HSX_FUN 5                                                         */
/* For BDX_HOST:                                                              */
/* PCU_FUN5_BDX_DEV 30                                                        */
/* PCU_FUN5_BDX_FUN 5                                                         */

/* SVID_PCU_FUN5_REG supported on:                                            */
/*       IVT_EP (0x2015502C)                                                  */
/*       IVT_EX (0x2015502C)                                                  */
/*       HSX (0x201F502C)                                                     */
/*       BDX (0x201F502C)                                                     */
/* Register default value:              0x8086                                */
#define SVID_PCU_FUN5_REG 0x0A05202C
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.5.CFG.xml.
 * PCI Subsystem Vendor ID Register
 */
typedef union {
  struct {
    UINT16 subsystem_vendor_identification_number : 16;
    /* subsystem_vendor_identification_number - Bits[15:0], RWS_O, default = 16'h8086 
       The default value specifies Intel but can be set to any value once after reset.
     */
  } Bits;
  UINT16 Data;
} SVID_PCU_FUN5_STRUCT;
#endif /* ASM_INC */


/* SDID_PCU_FUN5_REG supported on:                                            */
/*       IVT_EP (0x2015502E)                                                  */
/*       IVT_EX (0x2015502E)                                                  */
/*       HSX (0x201F502E)                                                     */
/*       BDX (0x201F502E)                                                     */
/* Register default value:              0x0000                                */
#define SDID_PCU_FUN5_REG 0x0A05202E
#ifndef ASM_INC
/* Struct format extracted from XML file BDX\1.30.5.CFG.xml.
 * PCI Subsystem device ID Register
 */
typedef union {
  struct {
    UINT16 subsystem_device_identification_number : 16;
    /* subsystem_device_identification_number - Bits[15:0], RWS_O, default = 16'b0000000000000000 
       Assigned by the subsystem vendor to uniquely identify the subsystem
     */
  } Bits;
  UINT16 Data;
} SDID_PCU_FUN5_STRUCT;
#endif /* ASM_INC */


/* VID_PCU_FUN5_REG supported on:                                             */
/*       IVT_EP (0x20155000)                                                  */
/*       IVT_EX (0x20155000)                                                  */
/* Register default value:              0x8086                                */
#define VID_PCU_FUN5_REG 0x0A052000



/* DID_PCU_FUN5_REG supported on:                                             */
/*       IVT_EP (0x20155002)                                                  */
/*       IVT_EX (0x20155002)                                                  */
/* Register default value:              0x0EC5                                */
#define DID_PCU_FUN5_REG 0x0A052002



/* PCICMD_PCU_FUN5_REG supported on:                                          */
/*       IVT_EP (0x20155004)                                                  */
/*       IVT_EX (0x20155004)                                                  */
/* Register default value:              0x0000                                */
#define PCICMD_PCU_FUN5_REG 0x0A052004



/* PCISTS_PCU_FUN5_REG supported on:                                          */
/*       IVT_EP (0x20155006)                                                  */
/*       IVT_EX (0x20155006)                                                  */
/* Register default value:              0x0000                                */
#define PCISTS_PCU_FUN5_REG 0x0A052006



/* RID_PCU_FUN5_REG supported on:                                             */
/*       IVT_EP (0x10155008)                                                  */
/*       IVT_EX (0x10155008)                                                  */
/* Register default value:              0x00                                  */
#define RID_PCU_FUN5_REG 0x0A051008



/* CCR_N0_PCU_FUN5_REG supported on:                                          */
/*       IVT_EP (0x10155009)                                                  */
/*       IVT_EX (0x10155009)                                                  */
/* Register default value:              0x00                                  */
#define CCR_N0_PCU_FUN5_REG 0x0A051009



/* CCR_N1_PCU_FUN5_REG supported on:                                          */
/*       IVT_EP (0x2015500A)                                                  */
/*       IVT_EX (0x2015500A)                                                  */
/* Register default value:              0x0880                                */
#define CCR_N1_PCU_FUN5_REG 0x0A05200A



/* CLSR_PCU_FUN5_REG supported on:                                            */
/*       IVT_EP (0x1015500C)                                                  */
/*       IVT_EX (0x1015500C)                                                  */
/* Register default value:              0x00                                  */
#define CLSR_PCU_FUN5_REG 0x0A05100C



/* PLAT_PCU_FUN5_REG supported on:                                            */
/*       IVT_EP (0x1015500D)                                                  */
/*       IVT_EX (0x1015500D)                                                  */
/* Register default value:              0x00                                  */
#define PLAT_PCU_FUN5_REG 0x0A05100D



/* HDR_PCU_FUN5_REG supported on:                                             */
/*       IVT_EP (0x1015500E)                                                  */
/*       IVT_EX (0x1015500E)                                                  */
/* Register default value:              0x80                                  */
#define HDR_PCU_FUN5_REG 0x0A05100E



/* BIST_PCU_FUN5_REG supported on:                                            */
/*       IVT_EP (0x1015500F)                                                  */
/*       IVT_EX (0x1015500F)                                                  */
/* Register default value:              0x00                                  */
#define BIST_PCU_FUN5_REG 0x0A05100F



/* CAPPTR_PCU_FUN5_REG supported on:                                          */
/*       IVT_EP (0x10155034)                                                  */
/*       IVT_EX (0x10155034)                                                  */
/* Register default value:              0x00                                  */
#define CAPPTR_PCU_FUN5_REG 0x0A051034



/* INTL_PCU_FUN5_REG supported on:                                            */
/*       IVT_EP (0x1015503C)                                                  */
/*       IVT_EX (0x1015503C)                                                  */
/* Register default value:              0x00                                  */
#define INTL_PCU_FUN5_REG 0x0A05103C



/* INTPIN_PCU_FUN5_REG supported on:                                          */
/*       IVT_EP (0x1015503D)                                                  */
/*       IVT_EX (0x1015503D)                                                  */
/* Register default value:              0x00                                  */
#define INTPIN_PCU_FUN5_REG 0x0A05103D



/* MINGNT_PCU_FUN5_REG supported on:                                          */
/*       IVT_EP (0x1015503E)                                                  */
/*       IVT_EX (0x1015503E)                                                  */
/* Register default value:              0x00                                  */
#define MINGNT_PCU_FUN5_REG 0x0A05103E



/* MAXLAT_PCU_FUN5_REG supported on:                                          */
/*       IVT_EP (0x1015503F)                                                  */
/*       IVT_EX (0x1015503F)                                                  */
/* Register default value:              0x00                                  */
#define MAXLAT_PCU_FUN5_REG 0x0A05103F



#endif /* PCU_FUN5_h */
