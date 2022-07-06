/* Date Stamp: 8/23/2014 */

#ifndef PCU_FUN6_h
#define PCU_FUN6_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* PCU_FUN6_IVT_DEV 10                                                        */
/* PCU_FUN6_IVT_FUN 6                                                         */
/* For HSX_HOST:                                                              */
/* PCU_FUN6_HSX_DEV 30                                                        */
/* PCU_FUN6_HSX_FUN 6                                                         */
/* For BDX_HOST:                                                              */
/* PCU_FUN6_BDX_DEV 30                                                        */
/* PCU_FUN6_BDX_FUN 6                                                         */

/* VID_PCU_FUN6_REG supported on:                                             */
/*       IVT_EP (0x20156000)                                                  */
/*       IVT_EX (0x20156000)                                                  */
/* Register default value:              0x8086                                */
#define VID_PCU_FUN6_REG 0x0A062000



/* DID_PCU_FUN6_REG supported on:                                             */
/*       IVT_EP (0x20156002)                                                  */
/*       IVT_EX (0x20156002)                                                  */
/* Register default value:              0x0EC6                                */
#define DID_PCU_FUN6_REG 0x0A062002



/* PCICMD_PCU_FUN6_REG supported on:                                          */
/*       IVT_EP (0x20156004)                                                  */
/*       IVT_EX (0x20156004)                                                  */
/* Register default value:              0x0000                                */
#define PCICMD_PCU_FUN6_REG 0x0A062004



/* PCISTS_PCU_FUN6_REG supported on:                                          */
/*       IVT_EP (0x20156006)                                                  */
/*       IVT_EX (0x20156006)                                                  */
/* Register default value:              0x0000                                */
#define PCISTS_PCU_FUN6_REG 0x0A062006



/* RID_PCU_FUN6_REG supported on:                                             */
/*       IVT_EP (0x10156008)                                                  */
/*       IVT_EX (0x10156008)                                                  */
/* Register default value:              0x00                                  */
#define RID_PCU_FUN6_REG 0x0A061008



/* CCR_N0_PCU_FUN6_REG supported on:                                          */
/*       IVT_EP (0x10156009)                                                  */
/*       IVT_EX (0x10156009)                                                  */
/* Register default value:              0x00                                  */
#define CCR_N0_PCU_FUN6_REG 0x0A061009



/* CCR_N1_PCU_FUN6_REG supported on:                                          */
/*       IVT_EP (0x2015600A)                                                  */
/*       IVT_EX (0x2015600A)                                                  */
/* Register default value:              0x0880                                */
#define CCR_N1_PCU_FUN6_REG 0x0A06200A



/* CLSR_PCU_FUN6_REG supported on:                                            */
/*       IVT_EP (0x1015600C)                                                  */
/*       IVT_EX (0x1015600C)                                                  */
/* Register default value:              0x00                                  */
#define CLSR_PCU_FUN6_REG 0x0A06100C



/* PLAT_PCU_FUN6_REG supported on:                                            */
/*       IVT_EP (0x1015600D)                                                  */
/*       IVT_EX (0x1015600D)                                                  */
/* Register default value:              0x00                                  */
#define PLAT_PCU_FUN6_REG 0x0A06100D



/* HDR_PCU_FUN6_REG supported on:                                             */
/*       IVT_EP (0x1015600E)                                                  */
/*       IVT_EX (0x1015600E)                                                  */
/* Register default value:              0x80                                  */
#define HDR_PCU_FUN6_REG 0x0A06100E



/* BIST_PCU_FUN6_REG supported on:                                            */
/*       IVT_EP (0x1015600F)                                                  */
/*       IVT_EX (0x1015600F)                                                  */
/* Register default value:              0x00                                  */
#define BIST_PCU_FUN6_REG 0x0A06100F



/* SVID_PCU_FUN6_REG supported on:                                            */
/*       IVT_EP (0x2015602C)                                                  */
/*       IVT_EX (0x2015602C)                                                  */
/* Register default value:              0x8086                                */
#define SVID_PCU_FUN6_REG 0x0A06202C



/* SDID_PCU_FUN6_REG supported on:                                            */
/*       IVT_EP (0x2015602E)                                                  */
/*       IVT_EX (0x2015602E)                                                  */
/* Register default value:              0x0000                                */
#define SDID_PCU_FUN6_REG 0x0A06202E



/* CAPPTR_PCU_FUN6_REG supported on:                                          */
/*       IVT_EP (0x10156034)                                                  */
/*       IVT_EX (0x10156034)                                                  */
/* Register default value:              0x00                                  */
#define CAPPTR_PCU_FUN6_REG 0x0A061034



/* INTL_PCU_FUN6_REG supported on:                                            */
/*       IVT_EP (0x1015603C)                                                  */
/*       IVT_EX (0x1015603C)                                                  */
/* Register default value:              0x00                                  */
#define INTL_PCU_FUN6_REG 0x0A06103C



/* INTPIN_PCU_FUN6_REG supported on:                                          */
/*       IVT_EP (0x1015603D)                                                  */
/*       IVT_EX (0x1015603D)                                                  */
/* Register default value:              0x00                                  */
#define INTPIN_PCU_FUN6_REG 0x0A06103D



/* MINGNT_PCU_FUN6_REG supported on:                                          */
/*       IVT_EP (0x1015603E)                                                  */
/*       IVT_EX (0x1015603E)                                                  */
/* Register default value:              0x00                                  */
#define MINGNT_PCU_FUN6_REG 0x0A06103E



/* MAXLAT_PCU_FUN6_REG supported on:                                          */
/*       IVT_EP (0x1015603F)                                                  */
/*       IVT_EX (0x1015603F)                                                  */
/* Register default value:              0x00                                  */
#define MAXLAT_PCU_FUN6_REG 0x0A06103F



/* GDXC_FCLK_MONITORING_CONFIG_PCU_FUN6_REG supported on:                     */
/*       IVT_EP (0x40156060)                                                  */
/*       IVT_EX (0x40156060)                                                  */
/* Register default value:              0x00000000                            */
#define GDXC_FCLK_MONITORING_CONFIG_PCU_FUN6_REG 0x0A064060



/* GDXC_FCLK_MSG_DEST_QUALIFIER_PCU_FUN6_REG supported on:                    */
/*       IVT_EP (0x40156064)                                                  */
/*       IVT_EX (0x40156064)                                                  */
/* Register default value:              0x00000000                            */
#define GDXC_FCLK_MSG_DEST_QUALIFIER_PCU_FUN6_REG 0x0A064064



/* GDXC_FCLK_MSG_OPCODE_QUALIFIER_PCU_FUN6_REG supported on:                  */
/*       IVT_EP (0x40156068)                                                  */
/*       IVT_EX (0x40156068)                                                  */
/* Register default value:              0xFF50FF50                            */
#define GDXC_FCLK_MSG_OPCODE_QUALIFIER_PCU_FUN6_REG 0x0A064068



/* GDXC_FCLK_MSG_SOURCE_QUALIFIER_PCU_FUN6_REG supported on:                  */
/*       IVT_EP (0x4015606C)                                                  */
/*       IVT_EX (0x4015606C)                                                  */
/* Register default value:              0x00000000                            */
#define GDXC_FCLK_MSG_SOURCE_QUALIFIER_PCU_FUN6_REG 0x0A06406C



/* BGFCFG_F2U_PCU_FUN6_REG supported on:                                      */
/*       IVT_EP (0x40156070)                                                  */
/*       IVT_EX (0x40156070)                                                  */
/* Register default value:              0x00000000                            */
#define BGFCFG_F2U_PCU_FUN6_REG 0x0A064070



/* GDXC_FCLK_RST_CTRL_PCU_FUN6_REG supported on:                              */
/*       IVT_EP (0x40156074)                                                  */
/*       IVT_EX (0x40156074)                                                  */
/* Register default value:              0x00000000                            */
#define GDXC_FCLK_RST_CTRL_PCU_FUN6_REG 0x0A064074



/* GDXC_FCLK_BGF_CONFIG_PCU_FUN6_REG supported on:                            */
/*       IVT_EP (0x40156078)                                                  */
/*       IVT_EX (0x40156078)                                                  */
/* Register default value:              0x00000000                            */
#define GDXC_FCLK_BGF_CONFIG_PCU_FUN6_REG 0x0A064078



/* GDXC_MSG_TYPE_MATCH_DATA_PCU_FUN6_REG supported on:                        */
/*       IVT_EP (0x40156080)                                                  */
/*       IVT_EX (0x40156080)                                                  */
/* Register default value:              0x00000000                            */
#define GDXC_MSG_TYPE_MATCH_DATA_PCU_FUN6_REG 0x0A064080



/* GDXC_MSG_TYPE_MATCH_MASK_PCU_FUN6_REG supported on:                        */
/*       IVT_EP (0x40156088)                                                  */
/*       IVT_EX (0x40156088)                                                  */
/* Register default value:              0x000000FF                            */
#define GDXC_MSG_TYPE_MATCH_MASK_PCU_FUN6_REG 0x0A064088



/* PCUNOAPMONTRIGMASK_PCU_FUN6_REG supported on:                              */
/*       IVT_EP (0x4015608C)                                                  */
/*       IVT_EX (0x4015608C)                                                  */
/* Register default value:              0x0003FFFF                            */
#define PCUNOAPMONTRIGMASK_PCU_FUN6_REG 0x0A06408C



/* GDXC_PAYLOAD_MATCH_DATA_N0_PCU_FUN6_REG supported on:                      */
/*       IVT_EP (0x40156090)                                                  */
/*       IVT_EX (0x40156090)                                                  */
/* Register default value:              0x00000000                            */
#define GDXC_PAYLOAD_MATCH_DATA_N0_PCU_FUN6_REG 0x0A064090



/* GDXC_PAYLOAD_MATCH_DATA_N1_PCU_FUN6_REG supported on:                      */
/*       IVT_EP (0x40156094)                                                  */
/*       IVT_EX (0x40156094)                                                  */
/* Register default value:              0x00000000                            */
#define GDXC_PAYLOAD_MATCH_DATA_N1_PCU_FUN6_REG 0x0A064094



/* GDXC_PAYLOAD_MATCH_MASK_N0_PCU_FUN6_REG supported on:                      */
/*       IVT_EP (0x40156098)                                                  */
/*       IVT_EX (0x40156098)                                                  */
/* Register default value:              0xFFFFFFFFFFFFFFFF                    */
#define GDXC_PAYLOAD_MATCH_MASK_N0_PCU_FUN6_REG 0x0A064098



/* GDXC_PAYLOAD_MATCH_MASK_N1_PCU_FUN6_REG supported on:                      */
/*       IVT_EP (0x4015609C)                                                  */
/*       IVT_EX (0x4015609C)                                                  */
/* Register default value:              0xFFFFFFFFFFFFFFFF                    */
#define GDXC_PAYLOAD_MATCH_MASK_N1_PCU_FUN6_REG 0x0A06409C



/* PCUDBGBUSMATCH_PCU_FUN6_REG supported on:                                  */
/*       IVT_EP (0x401560C0)                                                  */
/*       IVT_EX (0x401560C0)                                                  */
/* Register default value:              0x00000000                            */
#define PCUDBGBUSMATCH_PCU_FUN6_REG 0x0A0640C0



/* PCUDBGBUSMASK_PCU_FUN6_REG supported on:                                   */
/*       IVT_EP (0x401560C4)                                                  */
/*       IVT_EX (0x401560C4)                                                  */
/* Register default value:              0x00000000                            */
#define PCUDBGBUSMASK_PCU_FUN6_REG 0x0A0640C4



/* PCUDBGBUSMUXCTRL_PCU_FUN6_REG supported on:                                */
/*       IVT_EP (0x401560C8)                                                  */
/*       IVT_EX (0x401560C8)                                                  */
/* Register default value:              0x00000380                            */
#define PCUDBGBUSMUXCTRL_PCU_FUN6_REG 0x0A0640C8



/* PCUDBGBUSCNTRL0_PCU_FUN6_REG supported on:                                 */
/*       IVT_EP (0x401560D0)                                                  */
/*       IVT_EX (0x401560D0)                                                  */
/* Register default value:              0x00000000                            */
#define PCUDBGBUSCNTRL0_PCU_FUN6_REG 0x0A0640D0



/* PCUDBGBUSCNTRL1_PCU_FUN6_REG supported on:                                 */
/*       IVT_EP (0x201560D4)                                                  */
/*       IVT_EX (0x201560D4)                                                  */
/* Register default value:              0x0001                                */
#define PCUDBGBUSCNTRL1_PCU_FUN6_REG 0x0A0620D4



/* PCUDBGBUSCNTRL2_PCU_FUN6_REG supported on:                                 */
/*       IVT_EP (0x201560D6)                                                  */
/*       IVT_EX (0x201560D6)                                                  */
/* Register default value:              0x0000                                */
#define PCUDBGBUSCNTRL2_PCU_FUN6_REG 0x0A0620D6



/* PCUDBGBUSCNTRL3_PCU_FUN6_REG supported on:                                 */
/*       IVT_EP (0x401560D8)                                                  */
/*       IVT_EX (0x401560D8)                                                  */
/* Register default value:              0x00000000                            */
#define PCUDBGBUSCNTRL3_PCU_FUN6_REG 0x0A0640D8



/* PCUDBGBUSCNTRL4_PCU_FUN6_REG supported on:                                 */
/*       IVT_EP (0x401560DC)                                                  */
/*       IVT_EX (0x401560DC)                                                  */
/* Register default value:              0x00000000                            */
#define PCUDBGBUSCNTRL4_PCU_FUN6_REG 0x0A0640DC



/* NOA_SINAI_CFG_CTRL_PCU_FUN6_REG supported on:                              */
/*       IVT_EP (0x401560E0)                                                  */
/*       IVT_EX (0x401560E0)                                                  */
/* Register default value:              0x00000000                            */
#define NOA_SINAI_CFG_CTRL_PCU_FUN6_REG 0x0A0640E0



#endif /* PCU_FUN6_h */
