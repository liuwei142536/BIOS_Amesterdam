/* Date Stamp: 8/23/2014 */

#ifndef HA_GDXC_h
#define HA_GDXC_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* HA_GDXC_IVT_DEV 14                                                         */
/* HA_GDXC_IVT_FUN 3                                                          */
/* For HSX_HOST:                                                              */
/* HA_GDXC_HSX_DEV 18                                                         */
/* HA_GDXC_HSX_FUN 3                                                          */
/* For BDX_HOST:                                                              */
/* HA_GDXC_BDX_DEV 18                                                         */
/* HA_GDXC_BDX_FUN 3                                                          */

/* VID_HA_GDXC_REG supported on:                                              */
/*       IVT_EP (0x20173000)                                                  */
/*       IVT_EX (0x20173000)                                                  */
/* Register default value:              0x8086                                */
#define VID_HA_GDXC_REG 0x03032000



/* DID_HA_GDXC_REG supported on:                                              */
/*       IVT_EP (0x20173002)                                                  */
/*       IVT_EX (0x20173002)                                                  */
/* Register default value:              0x0E72                                */
#define DID_HA_GDXC_REG 0x03032002



/* PCICMD_HA_GDXC_REG supported on:                                           */
/*       IVT_EP (0x20173004)                                                  */
/*       IVT_EX (0x20173004)                                                  */
/* Register default value:              0x0000                                */
#define PCICMD_HA_GDXC_REG 0x03032004



/* PCISTS_HA_GDXC_REG supported on:                                           */
/*       IVT_EP (0x20173006)                                                  */
/*       IVT_EX (0x20173006)                                                  */
/* Register default value:              0x0000                                */
#define PCISTS_HA_GDXC_REG 0x03032006



/* RID_HA_GDXC_REG supported on:                                              */
/*       IVT_EP (0x10173008)                                                  */
/*       IVT_EX (0x10173008)                                                  */
/* Register default value:              0x00                                  */
#define RID_HA_GDXC_REG 0x03031008



/* CCR_N0_HA_GDXC_REG supported on:                                           */
/*       IVT_EP (0x10173009)                                                  */
/*       IVT_EX (0x10173009)                                                  */
/* Register default value:              0x00                                  */
#define CCR_N0_HA_GDXC_REG 0x03031009



/* CCR_N1_HA_GDXC_REG supported on:                                           */
/*       IVT_EP (0x2017300A)                                                  */
/*       IVT_EX (0x2017300A)                                                  */
/* Register default value:              0x0880                                */
#define CCR_N1_HA_GDXC_REG 0x0303200A



/* CLS_HA_GDXC_REG supported on:                                              */
/*       IVT_EP (0x1017300C)                                                  */
/*       IVT_EX (0x1017300C)                                                  */
/* Register default value:              0x00                                  */
#define CLS_HA_GDXC_REG 0x0303100C



/* MLT_HA_GDXC_REG supported on:                                              */
/*       IVT_EP (0x1017300D)                                                  */
/*       IVT_EX (0x1017300D)                                                  */
/* Register default value:              0x00                                  */
#define MLT_HA_GDXC_REG 0x0303100D



/* HDR_HA_GDXC_REG supported on:                                              */
/*       IVT_EP (0x1017300E)                                                  */
/*       IVT_EX (0x1017300E)                                                  */
/* Register default value:              0x80                                  */
#define HDR_HA_GDXC_REG 0x0303100E



/* BIST_HA_GDXC_REG supported on:                                             */
/*       IVT_EP (0x1017300F)                                                  */
/*       IVT_EX (0x1017300F)                                                  */
/* Register default value:              0x00                                  */
#define BIST_HA_GDXC_REG 0x0303100F



/* TMBAR_N0_HA_GDXC_REG supported on:                                         */
/*       IVT_EP (0x40173010)                                                  */
/*       IVT_EX (0x40173010)                                                  */
/* Register default value:              0x00000000                            */
#define TMBAR_N0_HA_GDXC_REG 0x03034010



/* TMBAR_N1_HA_GDXC_REG supported on:                                         */
/*       IVT_EP (0x40173014)                                                  */
/*       IVT_EX (0x40173014)                                                  */
/* Register default value:              0x00000000                            */
#define TMBAR_N1_HA_GDXC_REG 0x03034014



/* SVID_HA_GDXC_REG supported on:                                             */
/*       IVT_EP (0x2017302C)                                                  */
/*       IVT_EX (0x2017302C)                                                  */
/* Register default value:              0x0000                                */
#define SVID_HA_GDXC_REG 0x0303202C



/* SID_HA_GDXC_REG supported on:                                              */
/*       IVT_EP (0x2017302E)                                                  */
/*       IVT_EX (0x2017302E)                                                  */
/* Register default value:              0x0000                                */
#define SID_HA_GDXC_REG 0x0303202E



/* CAPPOINT_HA_GDXC_REG supported on:                                         */
/*       IVT_EP (0x10173034)                                                  */
/*       IVT_EX (0x10173034)                                                  */
/* Register default value:              0x00                                  */
#define CAPPOINT_HA_GDXC_REG 0x03031034



/* INTRLINE_HA_GDXC_REG supported on:                                         */
/*       IVT_EP (0x1017303C)                                                  */
/*       IVT_EX (0x1017303C)                                                  */
/* Register default value:              0x00                                  */
#define INTRLINE_HA_GDXC_REG 0x0303103C



/* INTRPIN_HA_GDXC_REG supported on:                                          */
/*       IVT_EP (0x1017303D)                                                  */
/*       IVT_EX (0x1017303D)                                                  */
/* Register default value:              0x00                                  */
#define INTRPIN_HA_GDXC_REG 0x0303103D



/* MINGNT_HA_GDXC_REG supported on:                                           */
/*       IVT_EP (0x1017303E)                                                  */
/*       IVT_EX (0x1017303E)                                                  */
/* Register default value:              0x00                                  */
#define MINGNT_HA_GDXC_REG 0x0303103E



/* MAXLAT_HA_GDXC_REG supported on:                                           */
/*       IVT_EP (0x1017303F)                                                  */
/*       IVT_EX (0x1017303F)                                                  */
/* Register default value:              0x00                                  */
#define MAXLAT_HA_GDXC_REG 0x0303103F



/* SWINDEX_HA_GDXC_REG supported on:                                          */
/*       IVT_EP (0x40173040)                                                  */
/*       IVT_EX (0x40173040)                                                  */
/* Register default value:              0x00000000                            */
#define SWINDEX_HA_GDXC_REG 0x03034040



/* SWMMDATA_HA_GDXC_REG supported on:                                         */
/*       IVT_EP (0x40173044)                                                  */
/*       IVT_EX (0x40173044)                                                  */
/* Register default value:              0x00000000                            */
#define SWMMDATA_HA_GDXC_REG 0x03034044



/* SWEXTRIG1O_HA_GDXC_REG supported on:                                       */
/*       IVT_EP (0x40173060)                                                  */
/*       IVT_EX (0x40173060)                                                  */
/* Register default value:              0x00000000                            */
#define SWEXTRIG1O_HA_GDXC_REG 0x03034060



/* SWEXTRIG2O_HA_GDXC_REG supported on:                                       */
/*       IVT_EP (0x40173064)                                                  */
/*       IVT_EX (0x40173064)                                                  */
/* Register default value:              0x00000000                            */
#define SWEXTRIG2O_HA_GDXC_REG 0x03034064



/* SWTSM1CSR_HA_GDXC_REG supported on:                                        */
/*       IVT_EP (0x40173068)                                                  */
/*       IVT_EX (0x40173068)                                                  */
/* Register default value:              0x00000000                            */
#define SWTSM1CSR_HA_GDXC_REG 0x03034068



/* SWTSM2CSR_HA_GDXC_REG supported on:                                        */
/*       IVT_EP (0x4017306C)                                                  */
/*       IVT_EX (0x4017306C)                                                  */
/* Register default value:              0x00000000                            */
#define SWTSM2CSR_HA_GDXC_REG 0x0303406C



/* SWTSM1ADTR_HA_GDXC_REG supported on:                                       */
/*       IVT_EP (0x40173070)                                                  */
/*       IVT_EX (0x40173070)                                                  */
/* Register default value:              0x00000000                            */
#define SWTSM1ADTR_HA_GDXC_REG 0x03034070



/* SWTSM1AKTR_HA_GDXC_REG supported on:                                       */
/*       IVT_EP (0x40173074)                                                  */
/*       IVT_EX (0x40173074)                                                  */
/* Register default value:              0x00000000                            */
#define SWTSM1AKTR_HA_GDXC_REG 0x03034074



/* SWTSM1IVTR_HA_GDXC_REG supported on:                                       */
/*       IVT_EP (0x40173078)                                                  */
/*       IVT_EX (0x40173078)                                                  */
/* Register default value:              0x00000000                            */
#define SWTSM1IVTR_HA_GDXC_REG 0x03034078



/* SWTSM1BLTR_HA_GDXC_REG supported on:                                       */
/*       IVT_EP (0x4017307C)                                                  */
/*       IVT_EX (0x4017307C)                                                  */
/* Register default value:              0x00000000                            */
#define SWTSM1BLTR_HA_GDXC_REG 0x0303407C



/* SWTSM2ADTR_HA_GDXC_REG supported on:                                       */
/*       IVT_EP (0x40173080)                                                  */
/*       IVT_EX (0x40173080)                                                  */
/* Register default value:              0x00000000                            */
#define SWTSM2ADTR_HA_GDXC_REG 0x03034080



/* SWTSM2AKTR_HA_GDXC_REG supported on:                                       */
/*       IVT_EP (0x40173084)                                                  */
/*       IVT_EX (0x40173084)                                                  */
/* Register default value:              0x00000000                            */
#define SWTSM2AKTR_HA_GDXC_REG 0x03034084



/* SWTSM2IVTR_HA_GDXC_REG supported on:                                       */
/*       IVT_EP (0x40173088)                                                  */
/*       IVT_EX (0x40173088)                                                  */
/* Register default value:              0x00000000                            */
#define SWTSM2IVTR_HA_GDXC_REG 0x03034088



/* SWTSM2BLTR_HA_GDXC_REG supported on:                                       */
/*       IVT_EP (0x4017308C)                                                  */
/*       IVT_EX (0x4017308C)                                                  */
/* Register default value:              0x00000000                            */
#define SWTSM2BLTR_HA_GDXC_REG 0x0303408C



/* SWOEMCSR_HA_GDXC_REG supported on:                                         */
/*       IVT_EP (0x40173090)                                                  */
/*       IVT_EX (0x40173090)                                                  */
/* Register default value:              0x00000000                            */
#define SWOEMCSR_HA_GDXC_REG 0x03034090



/* SWBLCAP1_HA_GDXC_REG supported on:                                         */
/*       IVT_EP (0x40173094)                                                  */
/*       IVT_EX (0x40173094)                                                  */
/* Register default value:              0x00000000                            */
#define SWBLCAP1_HA_GDXC_REG 0x03034094



/* SWBLCAP2_HA_GDXC_REG supported on:                                         */
/*       IVT_EP (0x40173098)                                                  */
/*       IVT_EX (0x40173098)                                                  */
/* Register default value:              0x00000000                            */
#define SWBLCAP2_HA_GDXC_REG 0x03034098



/* SEINDEX_HA_GDXC_REG supported on:                                          */
/*       IVT_EP (0x401730A0)                                                  */
/*       IVT_EX (0x401730A0)                                                  */
/* Register default value:              0x00000000                            */
#define SEINDEX_HA_GDXC_REG 0x030340A0



/* SEMMDATA_HA_GDXC_REG supported on:                                         */
/*       IVT_EP (0x401730A4)                                                  */
/*       IVT_EX (0x401730A4)                                                  */
/* Register default value:              0x00000000                            */
#define SEMMDATA_HA_GDXC_REG 0x030340A4



/* SEEXTRIG1O_HA_GDXC_REG supported on:                                       */
/*       IVT_EP (0x401730C0)                                                  */
/*       IVT_EX (0x401730C0)                                                  */
/* Register default value:              0x00000000                            */
#define SEEXTRIG1O_HA_GDXC_REG 0x030340C0



/* SEEXTRIG2O_HA_GDXC_REG supported on:                                       */
/*       IVT_EP (0x401730C4)                                                  */
/*       IVT_EX (0x401730C4)                                                  */
/* Register default value:              0x00000000                            */
#define SEEXTRIG2O_HA_GDXC_REG 0x030340C4



/* SETSM1CSR_HA_GDXC_REG supported on:                                        */
/*       IVT_EP (0x401730C8)                                                  */
/*       IVT_EX (0x401730C8)                                                  */
/* Register default value:              0x00000000                            */
#define SETSM1CSR_HA_GDXC_REG 0x030340C8



/* SETSM2CSR_HA_GDXC_REG supported on:                                        */
/*       IVT_EP (0x401730CC)                                                  */
/*       IVT_EX (0x401730CC)                                                  */
/* Register default value:              0x00000000                            */
#define SETSM2CSR_HA_GDXC_REG 0x030340CC



/* SETSM1ADTR_HA_GDXC_REG supported on:                                       */
/*       IVT_EP (0x401730D0)                                                  */
/*       IVT_EX (0x401730D0)                                                  */
/* Register default value:              0x00000000                            */
#define SETSM1ADTR_HA_GDXC_REG 0x030340D0



/* SETSM1AKTR_HA_GDXC_REG supported on:                                       */
/*       IVT_EP (0x401730D4)                                                  */
/*       IVT_EX (0x401730D4)                                                  */
/* Register default value:              0x00000000                            */
#define SETSM1AKTR_HA_GDXC_REG 0x030340D4



/* SETSM1IVTR_HA_GDXC_REG supported on:                                       */
/*       IVT_EP (0x401730D8)                                                  */
/*       IVT_EX (0x401730D8)                                                  */
/* Register default value:              0x00000000                            */
#define SETSM1IVTR_HA_GDXC_REG 0x030340D8



/* SETSM1BLTR_HA_GDXC_REG supported on:                                       */
/*       IVT_EP (0x401730DC)                                                  */
/*       IVT_EX (0x401730DC)                                                  */
/* Register default value:              0x00000000                            */
#define SETSM1BLTR_HA_GDXC_REG 0x030340DC



/* SETSM2ADTR_HA_GDXC_REG supported on:                                       */
/*       IVT_EP (0x401730E0)                                                  */
/*       IVT_EX (0x401730E0)                                                  */
/* Register default value:              0x00000000                            */
#define SETSM2ADTR_HA_GDXC_REG 0x030340E0



/* SETSM2AKTR_HA_GDXC_REG supported on:                                       */
/*       IVT_EP (0x401730E4)                                                  */
/*       IVT_EX (0x401730E4)                                                  */
/* Register default value:              0x00000000                            */
#define SETSM2AKTR_HA_GDXC_REG 0x030340E4



/* SETSM2IVTR_HA_GDXC_REG supported on:                                       */
/*       IVT_EP (0x401730E8)                                                  */
/*       IVT_EX (0x401730E8)                                                  */
/* Register default value:              0x00000000                            */
#define SETSM2IVTR_HA_GDXC_REG 0x030340E8



/* SETSM2BLTR_HA_GDXC_REG supported on:                                       */
/*       IVT_EP (0x401730EC)                                                  */
/*       IVT_EX (0x401730EC)                                                  */
/* Register default value:              0x00000000                            */
#define SETSM2BLTR_HA_GDXC_REG 0x030340EC



/* SEOEMCSR_HA_GDXC_REG supported on:                                         */
/*       IVT_EP (0x401730F0)                                                  */
/*       IVT_EX (0x401730F0)                                                  */
/* Register default value:              0x00000000                            */
#define SEOEMCSR_HA_GDXC_REG 0x030340F0



/* SEBLCAP1_HA_GDXC_REG supported on:                                         */
/*       IVT_EP (0x401730F4)                                                  */
/*       IVT_EX (0x401730F4)                                                  */
/* Register default value:              0x00000000                            */
#define SEBLCAP1_HA_GDXC_REG 0x030340F4



/* SEBLCAP2_HA_GDXC_REG supported on:                                         */
/*       IVT_EP (0x401730F8)                                                  */
/*       IVT_EX (0x401730F8)                                                  */
/* Register default value:              0x00000000                            */
#define SEBLCAP2_HA_GDXC_REG 0x030340F8



/* SOEMCSR_HA_GDXC_REG supported on:                                          */
/*       IVT_EP (0x401730FC)                                                  */
/*       IVT_EX (0x401730FC)                                                  */
/* Register default value:              0x00000000                            */
#define SOEMCSR_HA_GDXC_REG 0x030340FC



#endif /* HA_GDXC_h */
