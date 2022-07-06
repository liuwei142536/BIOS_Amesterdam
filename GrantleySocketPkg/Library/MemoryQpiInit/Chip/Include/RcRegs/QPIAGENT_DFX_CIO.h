/* Date Stamp: 8/23/2014 */

#ifndef QPIAGENT_DFX_CIO_h
#define QPIAGENT_DFX_CIO_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For all target CPUs:                                                       */
/* QPIAGENT_DFX_CIO_DEV 8                                                     */
/* QPIAGENT_DFX_CIO_FUN 4                                                     */

/* VID_QPIAGENT_DFX_CIO_REG supported on:                                     */
/*       IVT_EP (0x20144000)                                                  */
/*       IVT_EX (0x20144000)                                                  */
/* Register default value:              0x8086                                */
#define VID_QPIAGENT_DFX_CIO_REG 0x06032000



/* DID_QPIAGENT_DFX_CIO_REG supported on:                                     */
/*       IVT_EP (0x20144002)                                                  */
/*       IVT_EX (0x20144002)                                                  */
/* Register default value:              0x0E84                                */
#define DID_QPIAGENT_DFX_CIO_REG 0x06032002



/* PCICMD_QPIAGENT_DFX_CIO_REG supported on:                                  */
/*       IVT_EP (0x20144004)                                                  */
/*       IVT_EX (0x20144004)                                                  */
/* Register default value:              0x0000                                */
#define PCICMD_QPIAGENT_DFX_CIO_REG 0x06032004



/* PCISTS_QPIAGENT_DFX_CIO_REG supported on:                                  */
/*       IVT_EP (0x20144006)                                                  */
/*       IVT_EX (0x20144006)                                                  */
/* Register default value:              0x0010                                */
#define PCISTS_QPIAGENT_DFX_CIO_REG 0x06032006



/* RID_QPIAGENT_DFX_CIO_REG supported on:                                     */
/*       IVT_EP (0x10144008)                                                  */
/*       IVT_EX (0x10144008)                                                  */
/* Register default value:              0x00                                  */
#define RID_QPIAGENT_DFX_CIO_REG 0x06031008



/* CCR_N0_QPIAGENT_DFX_CIO_REG supported on:                                  */
/*       IVT_EP (0x10144009)                                                  */
/*       IVT_EX (0x10144009)                                                  */
/* Register default value:              0x00                                  */
#define CCR_N0_QPIAGENT_DFX_CIO_REG 0x06031009



/* CCR_N1_QPIAGENT_DFX_CIO_REG supported on:                                  */
/*       IVT_EP (0x2014400A)                                                  */
/*       IVT_EX (0x2014400A)                                                  */
/* Register default value:              0x0880                                */
#define CCR_N1_QPIAGENT_DFX_CIO_REG 0x0603200A



/* CLSR_QPIAGENT_DFX_CIO_REG supported on:                                    */
/*       IVT_EP (0x1014400C)                                                  */
/*       IVT_EX (0x1014400C)                                                  */
/* Register default value:              0x00                                  */
#define CLSR_QPIAGENT_DFX_CIO_REG 0x0603100C



/* PLAT_QPIAGENT_DFX_CIO_REG supported on:                                    */
/*       IVT_EP (0x1014400D)                                                  */
/*       IVT_EX (0x1014400D)                                                  */
/* Register default value:              0x00                                  */
#define PLAT_QPIAGENT_DFX_CIO_REG 0x0603100D



/* HDR_QPIAGENT_DFX_CIO_REG supported on:                                     */
/*       IVT_EP (0x1014400E)                                                  */
/*       IVT_EX (0x1014400E)                                                  */
/* Register default value:              0x80                                  */
#define HDR_QPIAGENT_DFX_CIO_REG 0x0603100E



/* BIST_QPIAGENT_DFX_CIO_REG supported on:                                    */
/*       IVT_EP (0x1014400F)                                                  */
/*       IVT_EX (0x1014400F)                                                  */
/* Register default value:              0x00                                  */
#define BIST_QPIAGENT_DFX_CIO_REG 0x0603100F



/* SVID_QPIAGENT_DFX_CIO_REG supported on:                                    */
/*       IVT_EP (0x2014402C)                                                  */
/*       IVT_EX (0x2014402C)                                                  */
/* Register default value:              0x8086                                */
#define SVID_QPIAGENT_DFX_CIO_REG 0x0603202C



/* SDID_QPIAGENT_DFX_CIO_REG supported on:                                    */
/*       IVT_EP (0x2014402E)                                                  */
/*       IVT_EX (0x2014402E)                                                  */
/* Register default value:              0x0000                                */
#define SDID_QPIAGENT_DFX_CIO_REG 0x0603202E



/* CAPPTR_QPIAGENT_DFX_CIO_REG supported on:                                  */
/*       IVT_EP (0x10144034)                                                  */
/*       IVT_EX (0x10144034)                                                  */
/* Register default value:              0x40                                  */
#define CAPPTR_QPIAGENT_DFX_CIO_REG 0x06031034



/* INTL_QPIAGENT_DFX_CIO_REG supported on:                                    */
/*       IVT_EP (0x1014403C)                                                  */
/*       IVT_EX (0x1014403C)                                                  */
/* Register default value:              0x00                                  */
#define INTL_QPIAGENT_DFX_CIO_REG 0x0603103C



/* INTPIN_QPIAGENT_DFX_CIO_REG supported on:                                  */
/*       IVT_EP (0x1014403D)                                                  */
/*       IVT_EX (0x1014403D)                                                  */
/* Register default value:              0x00                                  */
#define INTPIN_QPIAGENT_DFX_CIO_REG 0x0603103D



/* MINGNT_QPIAGENT_DFX_CIO_REG supported on:                                  */
/*       IVT_EP (0x1014403E)                                                  */
/*       IVT_EX (0x1014403E)                                                  */
/* Register default value:              0x00                                  */
#define MINGNT_QPIAGENT_DFX_CIO_REG 0x0603103E



/* MAXLAT_QPIAGENT_DFX_CIO_REG supported on:                                  */
/*       IVT_EP (0x1014403F)                                                  */
/*       IVT_EX (0x1014403F)                                                  */
/* Register default value:              0x00                                  */
#define MAXLAT_QPIAGENT_DFX_CIO_REG 0x0603103F



/* PXPCAP_QPIAGENT_DFX_CIO_REG supported on:                                  */
/*       IVT_EP (0x40144040)                                                  */
/*       IVT_EX (0x40144040)                                                  */
/* Register default value:              0x00910010                            */
#define PXPCAP_QPIAGENT_DFX_CIO_REG 0x06034040



/* PXPENHCAP_QPIAGENT_DFX_CIO_REG supported on:                               */
/*       IVT_EP (0x40144100)                                                  */
/*       IVT_EX (0x40144100)                                                  */
/* Register default value:              0x00000000                            */
#define PXPENHCAP_QPIAGENT_DFX_CIO_REG 0x06034100



/* QPILKDFXMISC_QPIAGENT_DFX_CIO_REG supported on:                            */
/*       IVT_EP (0x201441DC)                                                  */
/*       IVT_EX (0x201441DC)                                                  */
/* Register default value:              0x0080                                */
#define QPILKDFXMISC_QPIAGENT_DFX_CIO_REG 0x060321DC



/* PORT_CTL_QPIAGENT_DFX_CIO_REG supported on:                                */
/*       IVT_EP (0x40144300)                                                  */
/*       IVT_EX (0x40144300)                                                  */
/* Register default value:              0x01300200                            */
#define PORT_CTL_QPIAGENT_DFX_CIO_REG 0x06034300



/* DRIFTALARMMISC_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x40144308)                                                  */
/*       IVT_EX (0x40144308)                                                  */
/* Register default value:              0x00000000                            */
#define DRIFTALARMMISC_QPIAGENT_DFX_CIO_REG 0x06034308



/* FWDC_DLL_OVRD_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144388)                                                  */
/*       IVT_EX (0x40144388)                                                  */
/* Register default value:              0x00000000                            */
#define FWDC_DLL_OVRD_QPIAGENT_DFX_CIO_REG 0x06034388



/* FWDC_DLL_CTL_QPIAGENT_DFX_CIO_REG supported on:                            */
/*       IVT_EP (0x4014438C)                                                  */
/*       IVT_EX (0x4014438C)                                                  */
/* Register default value:              0x2735F000                            */
#define FWDC_DLL_CTL_QPIAGENT_DFX_CIO_REG 0x0603438C



/* FWDC_LCPKAMP_CFG_QPIAGENT_DFX_CIO_REG supported on:                        */
/*       IVT_EP (0x40144390)                                                  */
/*       IVT_EX (0x40144390)                                                  */
/* Register default value:              0x1DB01400                            */
#define FWDC_LCPKAMP_CFG_QPIAGENT_DFX_CIO_REG 0x06034390



/* FWDC_MISC_CTL_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144394)                                                  */
/*       IVT_EX (0x40144394)                                                  */
/* Register default value:              0x00000000                            */
#define FWDC_MISC_CTL_QPIAGENT_DFX_CIO_REG 0x06034394



/* FWDC_RX_CTL_QPIAGENT_DFX_CIO_REG supported on:                             */
/*       IVT_EP (0x40144398)                                                  */
/*       IVT_EX (0x40144398)                                                  */
/* Register default value:              0x00000100                            */
#define FWDC_RX_CTL_QPIAGENT_DFX_CIO_REG 0x06034398



/* FWDC_TX_CTL_QPIAGENT_DFX_CIO_REG supported on:                             */
/*       IVT_EP (0x4014439C)                                                  */
/*       IVT_EX (0x4014439C)                                                  */
/* Register default value:              0x0000423C                            */
#define FWDC_TX_CTL_QPIAGENT_DFX_CIO_REG 0x0603439C



/* FWDC_STS_QPIAGENT_DFX_CIO_REG supported on:                                */
/*       IVT_EP (0x401443A0)                                                  */
/*       IVT_EX (0x401443A0)                                                  */
/* Register default value:              0x00000000                            */
#define FWDC_STS_QPIAGENT_DFX_CIO_REG 0x060343A0



/* FWDC_BIAS_0_QPIAGENT_DFX_CIO_REG supported on:                             */
/*       IVT_EP (0x401443A4)                                                  */
/*       IVT_EX (0x401443A4)                                                  */
/* Register default value:              0x10010400                            */
#define FWDC_BIAS_0_QPIAGENT_DFX_CIO_REG 0x060343A4



/* FWDC_BIAS_1_QPIAGENT_DFX_CIO_REG supported on:                             */
/*       IVT_EP (0x401443A8)                                                  */
/*       IVT_EX (0x401443A8)                                                  */
/* Register default value:              0x000000A0                            */
#define FWDC_BIAS_1_QPIAGENT_DFX_CIO_REG 0x060343A8



/* FWDC_MISC_CTL_1_QPIAGENT_DFX_CIO_REG supported on:                         */
/*       IVT_EP (0x401443AC)                                                  */
/*       IVT_EX (0x401443AC)                                                  */
/* Register default value:              0x00000000                            */
#define FWDC_MISC_CTL_1_QPIAGENT_DFX_CIO_REG 0x060343AC



/* FWDC_MISC_CTL_2_QPIAGENT_DFX_CIO_REG supported on:                         */
/*       IVT_EP (0x401443B0)                                                  */
/*       IVT_EX (0x401443B0)                                                  */
/* Register default value:              0x00000000                            */
#define FWDC_MISC_CTL_2_QPIAGENT_DFX_CIO_REG 0x060343B0



/* DFX_FWDC_NTL_CTL_QPIAGENT_DFX_CIO_REG supported on:                        */
/*       IVT_EP (0x401443C4)                                                  */
/*       IVT_EX (0x401443C4)                                                  */
/* Register default value:              0x00000000                            */
#define DFX_FWDC_NTL_CTL_QPIAGENT_DFX_CIO_REG 0x060343C4



/* LBC_PER_IOU_CONTROL_QPIAGENT_DFX_CIO_REG supported on:                     */
/*       IVT_EP (0x40144600)                                                  */
/*       IVT_EX (0x40144600)                                                  */
/* Register default value:              0x00000000                            */
#define LBC_PER_IOU_CONTROL_QPIAGENT_DFX_CIO_REG 0x06034600



/* LBC_PER_IOU_DATA_QPIAGENT_DFX_CIO_REG supported on:                        */
/*       IVT_EP (0x20144604)                                                  */
/*       IVT_EX (0x20144604)                                                  */
/* Register default value:              0x0000                                */
#define LBC_PER_IOU_DATA_QPIAGENT_DFX_CIO_REG 0x06032604



/* NTL_RX_INVERT_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144610)                                                  */
/*       IVT_EX (0x40144610)                                                  */
/* Register default value:              0x00000000                            */
#define NTL_RX_INVERT_QPIAGENT_DFX_CIO_REG 0x06034610



/* NTL_TX_INVERT_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144614)                                                  */
/*       IVT_EX (0x40144614)                                                  */
/* Register default value:              0x00000000                            */
#define NTL_TX_INVERT_QPIAGENT_DFX_CIO_REG 0x06034614



/* SHIFT_BY_ONE_QPIAGENT_DFX_CIO_REG supported on:                            */
/*       IVT_EP (0x40144640)                                                  */
/*       IVT_EX (0x40144640)                                                  */
/* Register default value:              0x00000000                            */
#define SHIFT_BY_ONE_QPIAGENT_DFX_CIO_REG 0x06034640



/* FLIP_PHASE_QPIAGENT_DFX_CIO_REG supported on:                              */
/*       IVT_EP (0x40144644)                                                  */
/*       IVT_EX (0x40144644)                                                  */
/* Register default value:              0x00000000                            */
#define FLIP_PHASE_QPIAGENT_DFX_CIO_REG 0x06034644



/* TXALIGN_EN_QPIAGENT_DFX_CIO_REG supported on:                              */
/*       IVT_EP (0x40144648)                                                  */
/*       IVT_EX (0x40144648)                                                  */
/* Register default value:              0x03FF03FF                            */
#define TXALIGN_EN_QPIAGENT_DFX_CIO_REG 0x06034648



/* RX_RTERMDIS_QPIAGENT_DFX_CIO_REG supported on:                             */
/*       IVT_EP (0x40144660)                                                  */
/*       IVT_EX (0x40144660)                                                  */
/* Register default value:              0x00000000                            */
#define RX_RTERMDIS_QPIAGENT_DFX_CIO_REG 0x06034660



/* RX_RTERMPULLHIGHN_0_QPIAGENT_DFX_CIO_REG supported on:                     */
/*       IVT_EP (0x40144664)                                                  */
/*       IVT_EX (0x40144664)                                                  */
/* Register default value:              0x00000000                            */
#define RX_RTERMPULLHIGHN_0_QPIAGENT_DFX_CIO_REG 0x06034664



/* RX_RTERMPULLHIGHN_1_QPIAGENT_DFX_CIO_REG supported on:                     */
/*       IVT_EP (0x40144668)                                                  */
/*       IVT_EX (0x40144668)                                                  */
/* Register default value:              0x00000000                            */
#define RX_RTERMPULLHIGHN_1_QPIAGENT_DFX_CIO_REG 0x06034668



/* RX_RTERMPULLHIGHP_0_QPIAGENT_DFX_CIO_REG supported on:                     */
/*       IVT_EP (0x4014466C)                                                  */
/*       IVT_EX (0x4014466C)                                                  */
/* Register default value:              0x00000000                            */
#define RX_RTERMPULLHIGHP_0_QPIAGENT_DFX_CIO_REG 0x0603466C



/* RX_RTERMPULLHIGHP_1_QPIAGENT_DFX_CIO_REG supported on:                     */
/*       IVT_EP (0x40144670)                                                  */
/*       IVT_EX (0x40144670)                                                  */
/* Register default value:              0x00000000                            */
#define RX_RTERMPULLHIGHP_1_QPIAGENT_DFX_CIO_REG 0x06034670



/* RXSQ_EN_QPIAGENT_DFX_CIO_REG supported on:                                 */
/*       IVT_EP (0x40144674)                                                  */
/*       IVT_EX (0x40144674)                                                  */
/* Register default value:              0x03FF03FF                            */
#define RXSQ_EN_QPIAGENT_DFX_CIO_REG 0x06034674



/* RXSQ_CTRL_0_QPIAGENT_DFX_CIO_REG supported on:                             */
/*       IVT_EP (0x40144678)                                                  */
/*       IVT_EX (0x40144678)                                                  */
/* Register default value:              0x12492492                            */
#define RXSQ_CTRL_0_QPIAGENT_DFX_CIO_REG 0x06034678



/* RXSQ_CTRL_1_QPIAGENT_DFX_CIO_REG supported on:                             */
/*       IVT_EP (0x4014467C)                                                  */
/*       IVT_EX (0x4014467C)                                                  */
/* Register default value:              0x12492492                            */
#define RXSQ_CTRL_1_QPIAGENT_DFX_CIO_REG 0x0603467C



/* RXSQ_DET_QPIAGENT_DFX_CIO_REG supported on:                                */
/*       IVT_EP (0x40144680)                                                  */
/*       IVT_EX (0x40144680)                                                  */
/* Register default value:              0x00000000                            */
#define RXSQ_DET_QPIAGENT_DFX_CIO_REG 0x06034680



/* RX_CTLE_OFFSET_EN_QPIAGENT_DFX_CIO_REG supported on:                       */
/*       IVT_EP (0x40144684)                                                  */
/*       IVT_EX (0x40144684)                                                  */
/* Register default value:              0x03FF03FF                            */
#define RX_CTLE_OFFSET_EN_QPIAGENT_DFX_CIO_REG 0x06034684



/* RX_CTLE_PEAK_0_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x40144688)                                                  */
/*       IVT_EX (0x40144688)                                                  */
/* Register default value:              0x000DDDDD                            */
#define RX_CTLE_PEAK_0_QPIAGENT_DFX_CIO_REG 0x06034688



/* RX_CTLE_PEAK_1_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x4014468C)                                                  */
/*       IVT_EX (0x4014468C)                                                  */
/* Register default value:              0x000DDDDD                            */
#define RX_CTLE_PEAK_1_QPIAGENT_DFX_CIO_REG 0x0603468C



/* RX_CTLE_VCM_OVRD_DIS_QPIAGENT_DFX_CIO_REG supported on:                    */
/*       IVT_EP (0x4014469C)                                                  */
/*       IVT_EX (0x4014469C)                                                  */
/* Register default value:              0x00000000                            */
#define RX_CTLE_VCM_OVRD_DIS_QPIAGENT_DFX_CIO_REG 0x0603469C



/* RX_DFE_SUMMER_CTL_0_QPIAGENT_DFX_CIO_REG supported on:                     */
/*       IVT_EP (0x401446A0)                                                  */
/*       IVT_EX (0x401446A0)                                                  */
/* Register default value:              0x000FFFFF                            */
#define RX_DFE_SUMMER_CTL_0_QPIAGENT_DFX_CIO_REG 0x060346A0



/* RX_DFE_SUMMER_CTL_1_QPIAGENT_DFX_CIO_REG supported on:                     */
/*       IVT_EP (0x401446A4)                                                  */
/*       IVT_EX (0x401446A4)                                                  */
/* Register default value:              0x000FFFFF                            */
#define RX_DFE_SUMMER_CTL_1_QPIAGENT_DFX_CIO_REG 0x060346A4



/* RX_AFE_OFFSETCOR_OPENLOOP_QPIAGENT_DFX_CIO_REG supported on:               */
/*       IVT_EP (0x401446A8)                                                  */
/*       IVT_EX (0x401446A8)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AFE_OFFSETCOR_OPENLOOP_QPIAGENT_DFX_CIO_REG 0x060346A8



/* RX_AFE_OFFSETCOR_GAIN_QPIAGENT_DFX_CIO_REG supported on:                   */
/*       IVT_EP (0x401446AC)                                                  */
/*       IVT_EX (0x401446AC)                                                  */
/* Register default value:              0x01550155                            */
#define RX_AFE_OFFSETCOR_GAIN_QPIAGENT_DFX_CIO_REG 0x060346AC



/* RX_SPARE_0_QPIAGENT_DFX_CIO_REG supported on:                              */
/*       IVT_EP (0x401446B0)                                                  */
/*       IVT_EX (0x401446B0)                                                  */
/* Register default value:              0x00000000                            */
#define RX_SPARE_0_QPIAGENT_DFX_CIO_REG 0x060346B0



/* RX_SPARE_1_QPIAGENT_DFX_CIO_REG supported on:                              */
/*       IVT_EP (0x401446B4)                                                  */
/*       IVT_EX (0x401446B4)                                                  */
/* Register default value:              0x00000000                            */
#define RX_SPARE_1_QPIAGENT_DFX_CIO_REG 0x060346B4



/* RX_SPARE_2_QPIAGENT_DFX_CIO_REG supported on:                              */
/*       IVT_EP (0x401446B8)                                                  */
/*       IVT_EX (0x401446B8)                                                  */
/* Register default value:              0x00000000                            */
#define RX_SPARE_2_QPIAGENT_DFX_CIO_REG 0x060346B8



/* RX_SPARE_3_QPIAGENT_DFX_CIO_REG supported on:                              */
/*       IVT_EP (0x401446BC)                                                  */
/*       IVT_EX (0x401446BC)                                                  */
/* Register default value:              0x00000000                            */
#define RX_SPARE_3_QPIAGENT_DFX_CIO_REG 0x060346BC



/* RX_LPCLK_SEL_CK0_QPIAGENT_DFX_CIO_REG supported on:                        */
/*       IVT_EP (0x401446C0)                                                  */
/*       IVT_EX (0x401446C0)                                                  */
/* Register default value:              0x00000000                            */
#define RX_LPCLK_SEL_CK0_QPIAGENT_DFX_CIO_REG 0x060346C0



/* FAREND_LPBK_EN_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x401446C4)                                                  */
/*       IVT_EX (0x401446C4)                                                  */
/* Register default value:              0x00000000                            */
#define FAREND_LPBK_EN_QPIAGENT_DFX_CIO_REG 0x060346C4



/* RX_VCM_DS_SEL_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x401446C8)                                                  */
/*       IVT_EX (0x401446C8)                                                  */
/* Register default value:              0x00000000                            */
#define RX_VCM_DS_SEL_QPIAGENT_DFX_CIO_REG 0x060346C8



/* RX_VCM_SUM_SEL_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x401446CC)                                                  */
/*       IVT_EX (0x401446CC)                                                  */
/* Register default value:              0x001F001F                            */
#define RX_VCM_SUM_SEL_QPIAGENT_DFX_CIO_REG 0x060346CC



/* RX_VREF_SEL_0_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x401446D0)                                                  */
/*       IVT_EX (0x401446D0)                                                  */
/* Register default value:              0x10842108                            */
#define RX_VREF_SEL_0_QPIAGENT_DFX_CIO_REG 0x060346D0



/* RX_VREF_SEL_1_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x401446D4)                                                  */
/*       IVT_EX (0x401446D4)                                                  */
/* Register default value:              0x00042108                            */
#define RX_VREF_SEL_1_QPIAGENT_DFX_CIO_REG 0x060346D4



/* RX_VREF_SEL_2_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x401446D8)                                                  */
/*       IVT_EX (0x401446D8)                                                  */
/* Register default value:              0x10842108                            */
#define RX_VREF_SEL_2_QPIAGENT_DFX_CIO_REG 0x060346D8



/* RX_VREF_SEL_3_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x401446DC)                                                  */
/*       IVT_EX (0x401446DC)                                                  */
/* Register default value:              0x00042108                            */
#define RX_VREF_SEL_3_QPIAGENT_DFX_CIO_REG 0x060346DC



/* IG_ACQ_QPIAGENT_DFX_CIO_REG supported on:                                  */
/*       IVT_EP (0x40144700)                                                  */
/*       IVT_EX (0x40144700)                                                  */
/* Register default value:              0x00000000                            */
#define IG_ACQ_QPIAGENT_DFX_CIO_REG 0x06034700



/* IG_TRACK_QPIAGENT_DFX_CIO_REG supported on:                                */
/*       IVT_EP (0x40144704)                                                  */
/*       IVT_EX (0x40144704)                                                  */
/* Register default value:              0x00000000                            */
#define IG_TRACK_QPIAGENT_DFX_CIO_REG 0x06034704



/* PG_ACQ_0_QPIAGENT_DFX_CIO_REG supported on:                                */
/*       IVT_EP (0x40144708)                                                  */
/*       IVT_EX (0x40144708)                                                  */
/* Register default value:              0x10410410                            */
#define PG_ACQ_0_QPIAGENT_DFX_CIO_REG 0x06034708



/* PG_ACQ_1_QPIAGENT_DFX_CIO_REG supported on:                                */
/*       IVT_EP (0x4014470C)                                                  */
/*       IVT_EX (0x4014470C)                                                  */
/* Register default value:              0x10410410                            */
#define PG_ACQ_1_QPIAGENT_DFX_CIO_REG 0x0603470C



/* PG_TRACK_0_QPIAGENT_DFX_CIO_REG supported on:                              */
/*       IVT_EP (0x40144710)                                                  */
/*       IVT_EX (0x40144710)                                                  */
/* Register default value:              0x06186186                            */
#define PG_TRACK_0_QPIAGENT_DFX_CIO_REG 0x06034710



/* PG_TRACK_1_QPIAGENT_DFX_CIO_REG supported on:                              */
/*       IVT_EP (0x40144714)                                                  */
/*       IVT_EX (0x40144714)                                                  */
/* Register default value:              0x06186186                            */
#define PG_TRACK_1_QPIAGENT_DFX_CIO_REG 0x06034714



/* CDR_PDTC_0_QPIAGENT_DFX_CIO_REG supported on:                              */
/*       IVT_EP (0x40144718)                                                  */
/*       IVT_EX (0x40144718)                                                  */
/* Register default value:              0x00000000                            */
#define CDR_PDTC_0_QPIAGENT_DFX_CIO_REG 0x06034718



/* CDR_PDTC_1_QPIAGENT_DFX_CIO_REG supported on:                              */
/*       IVT_EP (0x4014471C)                                                  */
/*       IVT_EX (0x4014471C)                                                  */
/* Register default value:              0x00000000                            */
#define CDR_PDTC_1_QPIAGENT_DFX_CIO_REG 0x0603471C



/* CDR_FORCE_ACQ_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144720)                                                  */
/*       IVT_EX (0x40144720)                                                  */
/* Register default value:              0x00000000                            */
#define CDR_FORCE_ACQ_QPIAGENT_DFX_CIO_REG 0x06034720



/* CDR_PD_DATAMODE_QPIAGENT_DFX_CIO_REG supported on:                         */
/*       IVT_EP (0x40144724)                                                  */
/*       IVT_EX (0x40144724)                                                  */
/* Register default value:              0x00000000                            */
#define CDR_PD_DATAMODE_QPIAGENT_DFX_CIO_REG 0x06034724



/* CDR_PD_MODE_0_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144728)                                                  */
/*       IVT_EX (0x40144728)                                                  */
/* Register default value:              0x00000000                            */
#define CDR_PD_MODE_0_QPIAGENT_DFX_CIO_REG 0x06034728



/* CDR_PD_MODE_1_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x4014472C)                                                  */
/*       IVT_EX (0x4014472C)                                                  */
/* Register default value:              0x00000000                            */
#define CDR_PD_MODE_1_QPIAGENT_DFX_CIO_REG 0x0603472C



/* CDR_PPM_OFFSET_EN_QPIAGENT_DFX_CIO_REG supported on:                       */
/*       IVT_EP (0x40144730)                                                  */
/*       IVT_EX (0x40144730)                                                  */
/* Register default value:              0x00000000                            */
#define CDR_PPM_OFFSET_EN_QPIAGENT_DFX_CIO_REG 0x06034730



/* CDR_PPM_OFFSET_0_QPIAGENT_DFX_CIO_REG supported on:                        */
/*       IVT_EP (0x40144734)                                                  */
/*       IVT_EX (0x40144734)                                                  */
/* Register default value:              0x00000000                            */
#define CDR_PPM_OFFSET_0_QPIAGENT_DFX_CIO_REG 0x06034734



/* CDR_PPM_OFFSET_1_QPIAGENT_DFX_CIO_REG supported on:                        */
/*       IVT_EP (0x40144738)                                                  */
/*       IVT_EX (0x40144738)                                                  */
/* Register default value:              0x00000000                            */
#define CDR_PPM_OFFSET_1_QPIAGENT_DFX_CIO_REG 0x06034738



/* CDR_PPM_OFFSET_2_QPIAGENT_DFX_CIO_REG supported on:                        */
/*       IVT_EP (0x4014473C)                                                  */
/*       IVT_EX (0x4014473C)                                                  */
/* Register default value:              0x00000000                            */
#define CDR_PPM_OFFSET_2_QPIAGENT_DFX_CIO_REG 0x0603473C



/* CDR_PPM_OFFSET_3_QPIAGENT_DFX_CIO_REG supported on:                        */
/*       IVT_EP (0x40144740)                                                  */
/*       IVT_EX (0x40144740)                                                  */
/* Register default value:              0x00000000                            */
#define CDR_PPM_OFFSET_3_QPIAGENT_DFX_CIO_REG 0x06034740



/* CDR_PMOD_EN_QPIAGENT_DFX_CIO_REG supported on:                             */
/*       IVT_EP (0x40144744)                                                  */
/*       IVT_EX (0x40144744)                                                  */
/* Register default value:              0x00000000                            */
#define CDR_PMOD_EN_QPIAGENT_DFX_CIO_REG 0x06034744



/* CDR_PMOD_STEP_0_QPIAGENT_DFX_CIO_REG supported on:                         */
/*       IVT_EP (0x40144748)                                                  */
/*       IVT_EX (0x40144748)                                                  */
/* Register default value:              0x00000000                            */
#define CDR_PMOD_STEP_0_QPIAGENT_DFX_CIO_REG 0x06034748



/* CDR_PMOD_STEP_1_QPIAGENT_DFX_CIO_REG supported on:                         */
/*       IVT_EP (0x4014474C)                                                  */
/*       IVT_EX (0x4014474C)                                                  */
/* Register default value:              0x00000000                            */
#define CDR_PMOD_STEP_1_QPIAGENT_DFX_CIO_REG 0x0603474C



/* CDR_PMOD_STEP_2_QPIAGENT_DFX_CIO_REG supported on:                         */
/*       IVT_EP (0x40144750)                                                  */
/*       IVT_EX (0x40144750)                                                  */
/* Register default value:              0x00000000                            */
#define CDR_PMOD_STEP_2_QPIAGENT_DFX_CIO_REG 0x06034750



/* CDR_PMOD_STEP_3_QPIAGENT_DFX_CIO_REG supported on:                         */
/*       IVT_EP (0x40144754)                                                  */
/*       IVT_EX (0x40144754)                                                  */
/* Register default value:              0x00000000                            */
#define CDR_PMOD_STEP_3_QPIAGENT_DFX_CIO_REG 0x06034754



/* CDR_PMOD_PEAK_0_QPIAGENT_DFX_CIO_REG supported on:                         */
/*       IVT_EP (0x40144758)                                                  */
/*       IVT_EX (0x40144758)                                                  */
/* Register default value:              0x00000000                            */
#define CDR_PMOD_PEAK_0_QPIAGENT_DFX_CIO_REG 0x06034758



/* CDR_PMOD_PEAK_1_QPIAGENT_DFX_CIO_REG supported on:                         */
/*       IVT_EP (0x4014475C)                                                  */
/*       IVT_EX (0x4014475C)                                                  */
/* Register default value:              0x00000000                            */
#define CDR_PMOD_PEAK_1_QPIAGENT_DFX_CIO_REG 0x0603475C



/* CDR_ACQ_LENGTH_0_QPIAGENT_DFX_CIO_REG supported on:                        */
/*       IVT_EP (0x40144760)                                                  */
/*       IVT_EX (0x40144760)                                                  */
/* Register default value:              0x00055555                            */
#define CDR_ACQ_LENGTH_0_QPIAGENT_DFX_CIO_REG 0x06034760



/* CDR_ACQ_LENGTH_1_QPIAGENT_DFX_CIO_REG supported on:                        */
/*       IVT_EP (0x40144764)                                                  */
/*       IVT_EX (0x40144764)                                                  */
/* Register default value:              0x00055555                            */
#define CDR_ACQ_LENGTH_1_QPIAGENT_DFX_CIO_REG 0x06034764



/* CDR_BNDL_MODE_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144768)                                                  */
/*       IVT_EX (0x40144768)                                                  */
/* Register default value:              0x00000000                            */
#define CDR_BNDL_MODE_QPIAGENT_DFX_CIO_REG 0x06034768



/* CDR_OPEN_LOOP_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x4014476C)                                                  */
/*       IVT_EX (0x4014476C)                                                  */
/* Register default value:              0x00000000                            */
#define CDR_OPEN_LOOP_QPIAGENT_DFX_CIO_REG 0x0603476C



/* AGC_OPEN_LOOP_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144770)                                                  */
/*       IVT_EX (0x40144770)                                                  */
/* Register default value:              0x00000000                            */
#define AGC_OPEN_LOOP_QPIAGENT_DFX_CIO_REG 0x06034770



/* AGC_FORCE_ACQ_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144774)                                                  */
/*       IVT_EX (0x40144774)                                                  */
/* Register default value:              0x00000000                            */
#define AGC_FORCE_ACQ_QPIAGENT_DFX_CIO_REG 0x06034774



/* AGC_UPDATE_RATE_ACQ_QPIAGENT_DFX_CIO_REG supported on:                     */
/*       IVT_EP (0x40144778)                                                  */
/*       IVT_EX (0x40144778)                                                  */
/* Register default value:              0x00000000                            */
#define AGC_UPDATE_RATE_ACQ_QPIAGENT_DFX_CIO_REG 0x06034778



/* AGC_UPDATE_RATE_TRACK_QPIAGENT_DFX_CIO_REG supported on:                   */
/*       IVT_EP (0x4014477C)                                                  */
/*       IVT_EX (0x4014477C)                                                  */
/* Register default value:              0x00000000                            */
#define AGC_UPDATE_RATE_TRACK_QPIAGENT_DFX_CIO_REG 0x0603477C



/* AGC_GAIN_ACQ_QPIAGENT_DFX_CIO_REG supported on:                            */
/*       IVT_EP (0x40144780)                                                  */
/*       IVT_EX (0x40144780)                                                  */
/* Register default value:              0x03FF03FF                            */
#define AGC_GAIN_ACQ_QPIAGENT_DFX_CIO_REG 0x06034780



/* AGC_GAIN_TRACK_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x40144784)                                                  */
/*       IVT_EX (0x40144784)                                                  */
/* Register default value:              0x00000000                            */
#define AGC_GAIN_TRACK_QPIAGENT_DFX_CIO_REG 0x06034784



/* AGC_ACQ_LENGTH_0_QPIAGENT_DFX_CIO_REG supported on:                        */
/*       IVT_EP (0x40144788)                                                  */
/*       IVT_EX (0x40144788)                                                  */
/* Register default value:              0x000AAAAA                            */
#define AGC_ACQ_LENGTH_0_QPIAGENT_DFX_CIO_REG 0x06034788



/* AGC_ACQ_LENGTH_1_QPIAGENT_DFX_CIO_REG supported on:                        */
/*       IVT_EP (0x4014478C)                                                  */
/*       IVT_EX (0x4014478C)                                                  */
/* Register default value:              0x000AAAAA                            */
#define AGC_ACQ_LENGTH_1_QPIAGENT_DFX_CIO_REG 0x0603478C



/* RX_AGC_USE_ERR_DATA_QPIAGENT_DFX_CIO_REG supported on:                     */
/*       IVT_EP (0x40144790)                                                  */
/*       IVT_EX (0x40144790)                                                  */
/* Register default value:              0x03FF03FF                            */
#define RX_AGC_USE_ERR_DATA_QPIAGENT_DFX_CIO_REG 0x06034790



/* DFE_FORCE_ACQ_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144794)                                                  */
/*       IVT_EX (0x40144794)                                                  */
/* Register default value:              0x00000000                            */
#define DFE_FORCE_ACQ_QPIAGENT_DFX_CIO_REG 0x06034794



/* DFE_GAIN_ACQ_QPIAGENT_DFX_CIO_REG supported on:                            */
/*       IVT_EP (0x40144798)                                                  */
/*       IVT_EX (0x40144798)                                                  */
/* Register default value:              0x03FF03FF                            */
#define DFE_GAIN_ACQ_QPIAGENT_DFX_CIO_REG 0x06034798



/* DFE_GAIN_TRACK_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x4014479C)                                                  */
/*       IVT_EX (0x4014479C)                                                  */
/* Register default value:              0x00000000                            */
#define DFE_GAIN_TRACK_QPIAGENT_DFX_CIO_REG 0x0603479C



/* DFE_OPEN_LOOP_TAP_0_QPIAGENT_DFX_CIO_REG supported on:                     */
/*       IVT_EP (0x401447A0)                                                  */
/*       IVT_EX (0x401447A0)                                                  */
/* Register default value:              0x00000000                            */
#define DFE_OPEN_LOOP_TAP_0_QPIAGENT_DFX_CIO_REG 0x060347A0



/* DFE_OPEN_LOOP_TAP_1_QPIAGENT_DFX_CIO_REG supported on:                     */
/*       IVT_EP (0x401447A4)                                                  */
/*       IVT_EX (0x401447A4)                                                  */
/* Register default value:              0x00000000                            */
#define DFE_OPEN_LOOP_TAP_1_QPIAGENT_DFX_CIO_REG 0x060347A4



/* DFE_OPEN_LOOP_TAP_2_QPIAGENT_DFX_CIO_REG supported on:                     */
/*       IVT_EP (0x401447A8)                                                  */
/*       IVT_EX (0x401447A8)                                                  */
/* Register default value:              0x00000000                            */
#define DFE_OPEN_LOOP_TAP_2_QPIAGENT_DFX_CIO_REG 0x060347A8



/* DFE_OPEN_LOOP_TAP_3_QPIAGENT_DFX_CIO_REG supported on:                     */
/*       IVT_EP (0x401447AC)                                                  */
/*       IVT_EX (0x401447AC)                                                  */
/* Register default value:              0x00000000                            */
#define DFE_OPEN_LOOP_TAP_3_QPIAGENT_DFX_CIO_REG 0x060347AC



/* DFE_ACQ_LENGTH_0_QPIAGENT_DFX_CIO_REG supported on:                        */
/*       IVT_EP (0x401447B0)                                                  */
/*       IVT_EX (0x401447B0)                                                  */
/* Register default value:              0x000FFFFF                            */
#define DFE_ACQ_LENGTH_0_QPIAGENT_DFX_CIO_REG 0x060347B0



/* DFE_ACQ_LENGTH_1_QPIAGENT_DFX_CIO_REG supported on:                        */
/*       IVT_EP (0x401447B4)                                                  */
/*       IVT_EX (0x401447B4)                                                  */
/* Register default value:              0x000FFFFF                            */
#define DFE_ACQ_LENGTH_1_QPIAGENT_DFX_CIO_REG 0x060347B4



/* DFE_UPDATE_RATE_ACQ_QPIAGENT_DFX_CIO_REG supported on:                     */
/*       IVT_EP (0x401447B8)                                                  */
/*       IVT_EX (0x401447B8)                                                  */
/* Register default value:              0x00000000                            */
#define DFE_UPDATE_RATE_ACQ_QPIAGENT_DFX_CIO_REG 0x060347B8



/* DFE_UPDATE_RATE_TRACK_QPIAGENT_DFX_CIO_REG supported on:                   */
/*       IVT_EP (0x401447BC)                                                  */
/*       IVT_EX (0x401447BC)                                                  */
/* Register default value:              0x02AA02AA                            */
#define DFE_UPDATE_RATE_TRACK_QPIAGENT_DFX_CIO_REG 0x060347BC



/* TX_RTERM_DIS_QPIAGENT_DFX_CIO_REG supported on:                            */
/*       IVT_EP (0x401447D0)                                                  */
/*       IVT_EX (0x401447D0)                                                  */
/* Register default value:              0x00000000                            */
#define TX_RTERM_DIS_QPIAGENT_DFX_CIO_REG 0x060347D0



/* TX_RTERM_PULLHIGH_N_0_QPIAGENT_DFX_CIO_REG supported on:                   */
/*       IVT_EP (0x401447D4)                                                  */
/*       IVT_EX (0x401447D4)                                                  */
/* Register default value:              0x000FFFFF                            */
#define TX_RTERM_PULLHIGH_N_0_QPIAGENT_DFX_CIO_REG 0x060347D4



/* TX_RTERM_PULLHIGH_N_1_QPIAGENT_DFX_CIO_REG supported on:                   */
/*       IVT_EP (0x401447D8)                                                  */
/*       IVT_EX (0x401447D8)                                                  */
/* Register default value:              0x000FFFFF                            */
#define TX_RTERM_PULLHIGH_N_1_QPIAGENT_DFX_CIO_REG 0x060347D8



/* TX_RTERM_PULLHIGH_P_0_QPIAGENT_DFX_CIO_REG supported on:                   */
/*       IVT_EP (0x401447DC)                                                  */
/*       IVT_EX (0x401447DC)                                                  */
/* Register default value:              0x000FFFFF                            */
#define TX_RTERM_PULLHIGH_P_0_QPIAGENT_DFX_CIO_REG 0x060347DC



/* TX_RTERM_PULLHIGH_P_1_QPIAGENT_DFX_CIO_REG supported on:                   */
/*       IVT_EP (0x401447E0)                                                  */
/*       IVT_EX (0x401447E0)                                                  */
/* Register default value:              0x000FFFFF                            */
#define TX_RTERM_PULLHIGH_P_1_QPIAGENT_DFX_CIO_REG 0x060347E0



/* TXEQ_LVL0_0_QPIAGENT_DFX_CIO_REG supported on:                             */
/*       IVT_EP (0x401447E4)                                                  */
/*       IVT_EX (0x401447E4)                                                  */
/* Register default value:              0x3FFFFFFF                            */
#define TXEQ_LVL0_0_QPIAGENT_DFX_CIO_REG 0x060347E4



/* TXEQ_LVL0_1_QPIAGENT_DFX_CIO_REG supported on:                             */
/*       IVT_EP (0x401447E8)                                                  */
/*       IVT_EX (0x401447E8)                                                  */
/* Register default value:              0x3FFFFFFF                            */
#define TXEQ_LVL0_1_QPIAGENT_DFX_CIO_REG 0x060347E8



/* TXEQ_LVL1_0_QPIAGENT_DFX_CIO_REG supported on:                             */
/*       IVT_EP (0x401447EC)                                                  */
/*       IVT_EX (0x401447EC)                                                  */
/* Register default value:              0x3FFFFFFF                            */
#define TXEQ_LVL1_0_QPIAGENT_DFX_CIO_REG 0x060347EC



/* TXEQ_LVL1_1_QPIAGENT_DFX_CIO_REG supported on:                             */
/*       IVT_EP (0x401447F0)                                                  */
/*       IVT_EX (0x401447F0)                                                  */
/* Register default value:              0x3FFFFFFF                            */
#define TXEQ_LVL1_1_QPIAGENT_DFX_CIO_REG 0x060347F0



/* TXEQ_LVL2_0_QPIAGENT_DFX_CIO_REG supported on:                             */
/*       IVT_EP (0x401447F4)                                                  */
/*       IVT_EX (0x401447F4)                                                  */
/* Register default value:              0x3FFFFFFF                            */
#define TXEQ_LVL2_0_QPIAGENT_DFX_CIO_REG 0x060347F4



/* TXEQ_LVL2_1_QPIAGENT_DFX_CIO_REG supported on:                             */
/*       IVT_EP (0x401447F8)                                                  */
/*       IVT_EX (0x401447F8)                                                  */
/* Register default value:              0x3FFFFFFF                            */
#define TXEQ_LVL2_1_QPIAGENT_DFX_CIO_REG 0x060347F8



/* TXEQ_LVL3_0_QPIAGENT_DFX_CIO_REG supported on:                             */
/*       IVT_EP (0x401447FC)                                                  */
/*       IVT_EX (0x401447FC)                                                  */
/* Register default value:              0x3FFFFFFF                            */
#define TXEQ_LVL3_0_QPIAGENT_DFX_CIO_REG 0x060347FC



/* TXEQ_LVL3_1_QPIAGENT_DFX_CIO_REG supported on:                             */
/*       IVT_EP (0x40144800)                                                  */
/*       IVT_EX (0x40144800)                                                  */
/* Register default value:              0x3FFFFFFF                            */
#define TXEQ_LVL3_1_QPIAGENT_DFX_CIO_REG 0x06034800



/* TX_RX_DET_QPIAGENT_DFX_CIO_REG supported on:                               */
/*       IVT_EP (0x40144804)                                                  */
/*       IVT_EX (0x40144804)                                                  */
/* Register default value:              0x03FF03FF                            */
#define TX_RX_DET_QPIAGENT_DFX_CIO_REG 0x06034804



/* TX_SPARE_0_QPIAGENT_DFX_CIO_REG supported on:                              */
/*       IVT_EP (0x40144808)                                                  */
/*       IVT_EX (0x40144808)                                                  */
/* Register default value:              0x22222222                            */
#define TX_SPARE_0_QPIAGENT_DFX_CIO_REG 0x06034808



/* TX_SPARE_1_QPIAGENT_DFX_CIO_REG supported on:                              */
/*       IVT_EP (0x4014480C)                                                  */
/*       IVT_EX (0x4014480C)                                                  */
/* Register default value:              0x00000022                            */
#define TX_SPARE_1_QPIAGENT_DFX_CIO_REG 0x0603480C



/* TX_SPARE_2_QPIAGENT_DFX_CIO_REG supported on:                              */
/*       IVT_EP (0x40144810)                                                  */
/*       IVT_EX (0x40144810)                                                  */
/* Register default value:              0x22222222                            */
#define TX_SPARE_2_QPIAGENT_DFX_CIO_REG 0x06034810



/* TX_SPARE_3_QPIAGENT_DFX_CIO_REG supported on:                              */
/*       IVT_EP (0x40144814)                                                  */
/*       IVT_EX (0x40144814)                                                  */
/* Register default value:              0x00000022                            */
#define TX_SPARE_3_QPIAGENT_DFX_CIO_REG 0x06034814



/* TX_LCL_ICOMP_DIS_QPIAGENT_DFX_CIO_REG supported on:                        */
/*       IVT_EP (0x40144818)                                                  */
/*       IVT_EX (0x40144818)                                                  */
/* Register default value:              0x00000000                            */
#define TX_LCL_ICOMP_DIS_QPIAGENT_DFX_CIO_REG 0x06034818



/* TX_DRVSW_CTL_QPIAGENT_DFX_CIO_REG supported on:                            */
/*       IVT_EP (0x4014481C)                                                  */
/*       IVT_EX (0x4014481C)                                                  */
/* Register default value:              0x00000000                            */
#define TX_DRVSW_CTL_QPIAGENT_DFX_CIO_REG 0x0603481C



/* TX_DRVSW_ON_0_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144820)                                                  */
/*       IVT_EX (0x40144820)                                                  */
/* Register default value:              0x00055555                            */
#define TX_DRVSW_ON_0_QPIAGENT_DFX_CIO_REG 0x06034820



/* TX_DRVSW_ON_1_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144824)                                                  */
/*       IVT_EX (0x40144824)                                                  */
/* Register default value:              0x00055555                            */
#define TX_DRVSW_ON_1_QPIAGENT_DFX_CIO_REG 0x06034824



/* TX_DTSW_CTL_0_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144828)                                                  */
/*       IVT_EX (0x40144828)                                                  */
/* Register default value:              0x44444444                            */
#define TX_DTSW_CTL_0_QPIAGENT_DFX_CIO_REG 0x06034828



/* TX_DTSW_CTL_1_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x4014482C)                                                  */
/*       IVT_EX (0x4014482C)                                                  */
/* Register default value:              0x00000044                            */
#define TX_DTSW_CTL_1_QPIAGENT_DFX_CIO_REG 0x0603482C



/* TX_DTSW_CTL_2_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144830)                                                  */
/*       IVT_EX (0x40144830)                                                  */
/* Register default value:              0x44444444                            */
#define TX_DTSW_CTL_2_QPIAGENT_DFX_CIO_REG 0x06034830



/* TX_DTSW_CTL_3_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144834)                                                  */
/*       IVT_EX (0x40144834)                                                  */
/* Register default value:              0x00000044                            */
#define TX_DTSW_CTL_3_QPIAGENT_DFX_CIO_REG 0x06034834



/* TX_IREF_CTL_0_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144838)                                                  */
/*       IVT_EX (0x40144838)                                                  */
/* Register default value:              0x00000000                            */
#define TX_IREF_CTL_0_QPIAGENT_DFX_CIO_REG 0x06034838



/* TX_IREF_CTL_1_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x4014483C)                                                  */
/*       IVT_EX (0x4014483C)                                                  */
/* Register default value:              0x00000000                            */
#define TX_IREF_CTL_1_QPIAGENT_DFX_CIO_REG 0x0603483C



/* TX_VREF_SEL_0_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144840)                                                  */
/*       IVT_EX (0x40144840)                                                  */
/* Register default value:              0x01EF7BDE                            */
#define TX_VREF_SEL_0_QPIAGENT_DFX_CIO_REG 0x06034840



/* TX_VREF_SEL_1_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144844)                                                  */
/*       IVT_EX (0x40144844)                                                  */
/* Register default value:              0x01EF7BDE                            */
#define TX_VREF_SEL_1_QPIAGENT_DFX_CIO_REG 0x06034844



/* TX_RXLPCLK_SEL_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x40144848)                                                  */
/*       IVT_EX (0x40144848)                                                  */
/* Register default value:              0x00000000                            */
#define TX_RXLPCLK_SEL_QPIAGENT_DFX_CIO_REG 0x06034848



/* TX_SENSE_LANE_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x4014484C)                                                  */
/*       IVT_EX (0x4014484C)                                                  */
/* Register default value:              0x00000000                            */
#define TX_SENSE_LANE_QPIAGENT_DFX_CIO_REG 0x0603484C



/* OC_START_QPIAGENT_DFX_CIO_REG supported on:                                */
/*       IVT_EP (0x40144868)                                                  */
/*       IVT_EX (0x40144868)                                                  */
/* Register default value:              0x00000000                            */
#define OC_START_QPIAGENT_DFX_CIO_REG 0x06034868



/* OC_DONE_QPIAGENT_DFX_CIO_REG supported on:                                 */
/*       IVT_EP (0x4014486C)                                                  */
/*       IVT_EX (0x4014486C)                                                  */
/* Register default value:              0x00000000                            */
#define OC_DONE_QPIAGENT_DFX_CIO_REG 0x0603486C



/* L0S_RX_QPIAGENT_DFX_CIO_REG supported on:                                  */
/*       IVT_EP (0x40144870)                                                  */
/*       IVT_EX (0x40144870)                                                  */
/* Register default value:              0x00000000                            */
#define L0S_RX_QPIAGENT_DFX_CIO_REG 0x06034870



/* L0S_TX_QPIAGENT_DFX_CIO_REG supported on:                                  */
/*       IVT_EP (0x40144874)                                                  */
/*       IVT_EX (0x40144874)                                                  */
/* Register default value:              0x00000000                            */
#define L0S_TX_QPIAGENT_DFX_CIO_REG 0x06034874



/* L1_MODE_QPIAGENT_DFX_CIO_REG supported on:                                 */
/*       IVT_EP (0x40144878)                                                  */
/*       IVT_EX (0x40144878)                                                  */
/* Register default value:              0x00000000                            */
#define L1_MODE_QPIAGENT_DFX_CIO_REG 0x06034878



/* RX_LANE_CAL_RST_QPIAGENT_DFX_CIO_REG supported on:                         */
/*       IVT_EP (0x4014487C)                                                  */
/*       IVT_EX (0x4014487C)                                                  */
/* Register default value:              0x00000000                            */
#define RX_LANE_CAL_RST_QPIAGENT_DFX_CIO_REG 0x0603487C



/* RX_RESET_QPIAGENT_DFX_CIO_REG supported on:                                */
/*       IVT_EP (0x40144880)                                                  */
/*       IVT_EX (0x40144880)                                                  */
/* Register default value:              0x00000000                            */
#define RX_RESET_QPIAGENT_DFX_CIO_REG 0x06034880



/* TX_RESET_QPIAGENT_DFX_CIO_REG supported on:                                */
/*       IVT_EP (0x40144884)                                                  */
/*       IVT_EX (0x40144884)                                                  */
/* Register default value:              0x00000000                            */
#define TX_RESET_QPIAGENT_DFX_CIO_REG 0x06034884



/* BNDL_RST_QPIAGENT_DFX_CIO_REG supported on:                                */
/*       IVT_EP (0x40144888)                                                  */
/*       IVT_EX (0x40144888)                                                  */
/* Register default value:              0x00000000                            */
#define BNDL_RST_QPIAGENT_DFX_CIO_REG 0x06034888



/* DFX_BDL_SEL_QPIAGENT_DFX_CIO_REG supported on:                             */
/*       IVT_EP (0x4014488C)                                                  */
/*       IVT_EX (0x4014488C)                                                  */
/* Register default value:              0x00000000                            */
#define DFX_BDL_SEL_QPIAGENT_DFX_CIO_REG 0x0603488C



/* EYEMON_TRIG_QPIAGENT_DFX_CIO_REG supported on:                             */
/*       IVT_EP (0x40144894)                                                  */
/*       IVT_EX (0x40144894)                                                  */
/* Register default value:              0x00000000                            */
#define EYEMON_TRIG_QPIAGENT_DFX_CIO_REG 0x06034894



/* TXEQHS_LVL0_0_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x401448A0)                                                  */
/*       IVT_EX (0x401448A0)                                                  */
/* Register default value:              0x3FFFFFFF                            */
#define TXEQHS_LVL0_0_QPIAGENT_DFX_CIO_REG 0x060348A0



/* TXEQHS_LVL0_1_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x401448A4)                                                  */
/*       IVT_EX (0x401448A4)                                                  */
/* Register default value:              0x3FFFFFFF                            */
#define TXEQHS_LVL0_1_QPIAGENT_DFX_CIO_REG 0x060348A4



/* TXEQHS_LVL1_0_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x401448A8)                                                  */
/*       IVT_EX (0x401448A8)                                                  */
/* Register default value:              0x3FFFFFFF                            */
#define TXEQHS_LVL1_0_QPIAGENT_DFX_CIO_REG 0x060348A8



/* TXEQHS_LVL1_1_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x401448AC)                                                  */
/*       IVT_EX (0x401448AC)                                                  */
/* Register default value:              0x3FFFFFFF                            */
#define TXEQHS_LVL1_1_QPIAGENT_DFX_CIO_REG 0x060348AC



/* TXEQHS_LVL2_0_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x401448B0)                                                  */
/*       IVT_EX (0x401448B0)                                                  */
/* Register default value:              0x3FFFFFFF                            */
#define TXEQHS_LVL2_0_QPIAGENT_DFX_CIO_REG 0x060348B0



/* TXEQHS_LVL2_1_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x401448B4)                                                  */
/*       IVT_EX (0x401448B4)                                                  */
/* Register default value:              0x3FFFFFFF                            */
#define TXEQHS_LVL2_1_QPIAGENT_DFX_CIO_REG 0x060348B4



/* TXEQHS_LVL3_0_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x401448B8)                                                  */
/*       IVT_EX (0x401448B8)                                                  */
/* Register default value:              0x3FFFFFFF                            */
#define TXEQHS_LVL3_0_QPIAGENT_DFX_CIO_REG 0x060348B8



/* TXEQHS_LVL3_1_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x401448BC)                                                  */
/*       IVT_EX (0x401448BC)                                                  */
/* Register default value:              0x3FFFFFFF                            */
#define TXEQHS_LVL3_1_QPIAGENT_DFX_CIO_REG 0x060348BC



/* HS_RXCTL_QPIAGENT_DFX_CIO_REG supported on:                                */
/*       IVT_EP (0x401448C0)                                                  */
/*       IVT_EX (0x401448C0)                                                  */
/* Register default value:              0x0000008D                            */
#define HS_RXCTL_QPIAGENT_DFX_CIO_REG 0x060348C0



/* HS_CLK_MISC_QPIAGENT_DFX_CIO_REG supported on:                             */
/*       IVT_EP (0x401448C4)                                                  */
/*       IVT_EX (0x401448C4)                                                  */
/* Register default value:              0x00000000                            */
#define HS_CLK_MISC_QPIAGENT_DFX_CIO_REG 0x060348C4



/* RX_DFE_SNAP_0_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144900)                                                  */
/*       IVT_EX (0x40144900)                                                  */
/* Register default value:              0x00000000                            */
#define RX_DFE_SNAP_0_QPIAGENT_DFX_CIO_REG 0x06034900



/* RX_DFE_SNAP_1_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144904)                                                  */
/*       IVT_EX (0x40144904)                                                  */
/* Register default value:              0x00000000                            */
#define RX_DFE_SNAP_1_QPIAGENT_DFX_CIO_REG 0x06034904



/* RX_DFE_SNAP_2_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144908)                                                  */
/*       IVT_EX (0x40144908)                                                  */
/* Register default value:              0x00000000                            */
#define RX_DFE_SNAP_2_QPIAGENT_DFX_CIO_REG 0x06034908



/* RX_DFE_SNAP_3_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x4014490C)                                                  */
/*       IVT_EX (0x4014490C)                                                  */
/* Register default value:              0x00000000                            */
#define RX_DFE_SNAP_3_QPIAGENT_DFX_CIO_REG 0x0603490C



/* RX_DFE_SNAP_4_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144910)                                                  */
/*       IVT_EX (0x40144910)                                                  */
/* Register default value:              0x00000000                            */
#define RX_DFE_SNAP_4_QPIAGENT_DFX_CIO_REG 0x06034910



/* RX_DFE_SNAP_5_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144914)                                                  */
/*       IVT_EX (0x40144914)                                                  */
/* Register default value:              0x00000000                            */
#define RX_DFE_SNAP_5_QPIAGENT_DFX_CIO_REG 0x06034914



/* RX_DFE_SNAP_6_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144918)                                                  */
/*       IVT_EX (0x40144918)                                                  */
/* Register default value:              0x00000000                            */
#define RX_DFE_SNAP_6_QPIAGENT_DFX_CIO_REG 0x06034918



/* RX_DFE_SNAP_7_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x4014491C)                                                  */
/*       IVT_EX (0x4014491C)                                                  */
/* Register default value:              0x00000000                            */
#define RX_DFE_SNAP_7_QPIAGENT_DFX_CIO_REG 0x0603491C



/* RX_DFE_SNAP_8_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144920)                                                  */
/*       IVT_EX (0x40144920)                                                  */
/* Register default value:              0x00000000                            */
#define RX_DFE_SNAP_8_QPIAGENT_DFX_CIO_REG 0x06034920



/* RX_DFE_SNAP_9_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144924)                                                  */
/*       IVT_EX (0x40144924)                                                  */
/* Register default value:              0x00000000                            */
#define RX_DFE_SNAP_9_QPIAGENT_DFX_CIO_REG 0x06034924



/* RX_DFE_SNAP_10_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x40144928)                                                  */
/*       IVT_EX (0x40144928)                                                  */
/* Register default value:              0x00000000                            */
#define RX_DFE_SNAP_10_QPIAGENT_DFX_CIO_REG 0x06034928



/* RX_DFE_SNAP_11_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x4014492C)                                                  */
/*       IVT_EX (0x4014492C)                                                  */
/* Register default value:              0x00000000                            */
#define RX_DFE_SNAP_11_QPIAGENT_DFX_CIO_REG 0x0603492C



/* RX_DFE_SNAP_12_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x40144930)                                                  */
/*       IVT_EX (0x40144930)                                                  */
/* Register default value:              0x00000000                            */
#define RX_DFE_SNAP_12_QPIAGENT_DFX_CIO_REG 0x06034930



/* RX_DFE_SNAP_13_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x40144934)                                                  */
/*       IVT_EX (0x40144934)                                                  */
/* Register default value:              0x00000000                            */
#define RX_DFE_SNAP_13_QPIAGENT_DFX_CIO_REG 0x06034934



/* RX_DFE_SNAP_14_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x40144938)                                                  */
/*       IVT_EX (0x40144938)                                                  */
/* Register default value:              0x00000000                            */
#define RX_DFE_SNAP_14_QPIAGENT_DFX_CIO_REG 0x06034938



/* RX_DFE_SNAP_15_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x4014493C)                                                  */
/*       IVT_EX (0x4014493C)                                                  */
/* Register default value:              0x00000000                            */
#define RX_DFE_SNAP_15_QPIAGENT_DFX_CIO_REG 0x0603493C



/* RX_DFE_SNAP_16_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x40144940)                                                  */
/*       IVT_EX (0x40144940)                                                  */
/* Register default value:              0x00000000                            */
#define RX_DFE_SNAP_16_QPIAGENT_DFX_CIO_REG 0x06034940



/* RX_DFE_SNAP_17_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x40144944)                                                  */
/*       IVT_EX (0x40144944)                                                  */
/* Register default value:              0x00000000                            */
#define RX_DFE_SNAP_17_QPIAGENT_DFX_CIO_REG 0x06034944



/* RX_DFE_SNAP_18_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x40144948)                                                  */
/*       IVT_EX (0x40144948)                                                  */
/* Register default value:              0x00000000                            */
#define RX_DFE_SNAP_18_QPIAGENT_DFX_CIO_REG 0x06034948



/* RX_DFE_SNAP_19_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x4014494C)                                                  */
/*       IVT_EX (0x4014494C)                                                  */
/* Register default value:              0x00000000                            */
#define RX_DFE_SNAP_19_QPIAGENT_DFX_CIO_REG 0x0603494C



/* RX_AGC_SNAP_0_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144950)                                                  */
/*       IVT_EX (0x40144950)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AGC_SNAP_0_QPIAGENT_DFX_CIO_REG 0x06034950



/* RX_AGC_SNAP_1_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144954)                                                  */
/*       IVT_EX (0x40144954)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AGC_SNAP_1_QPIAGENT_DFX_CIO_REG 0x06034954



/* RX_AGC_SNAP_2_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144958)                                                  */
/*       IVT_EX (0x40144958)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AGC_SNAP_2_QPIAGENT_DFX_CIO_REG 0x06034958



/* RX_AGC_SNAP_3_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x4014495C)                                                  */
/*       IVT_EX (0x4014495C)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AGC_SNAP_3_QPIAGENT_DFX_CIO_REG 0x0603495C



/* RX_AGC_SNAP_4_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144960)                                                  */
/*       IVT_EX (0x40144960)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AGC_SNAP_4_QPIAGENT_DFX_CIO_REG 0x06034960



/* RX_AGC_SNAP_5_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144964)                                                  */
/*       IVT_EX (0x40144964)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AGC_SNAP_5_QPIAGENT_DFX_CIO_REG 0x06034964



/* RX_AGC_SNAP_6_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144968)                                                  */
/*       IVT_EX (0x40144968)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AGC_SNAP_6_QPIAGENT_DFX_CIO_REG 0x06034968



/* RX_AGC_SNAP_7_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x4014496C)                                                  */
/*       IVT_EX (0x4014496C)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AGC_SNAP_7_QPIAGENT_DFX_CIO_REG 0x0603496C



/* RX_AGC_SNAP_8_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144970)                                                  */
/*       IVT_EX (0x40144970)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AGC_SNAP_8_QPIAGENT_DFX_CIO_REG 0x06034970



/* RX_AGC_SNAP_9_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144974)                                                  */
/*       IVT_EX (0x40144974)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AGC_SNAP_9_QPIAGENT_DFX_CIO_REG 0x06034974



/* RX_AGC_SNAP_10_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x40144978)                                                  */
/*       IVT_EX (0x40144978)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AGC_SNAP_10_QPIAGENT_DFX_CIO_REG 0x06034978



/* RX_AGC_SNAP_11_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x4014497C)                                                  */
/*       IVT_EX (0x4014497C)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AGC_SNAP_11_QPIAGENT_DFX_CIO_REG 0x0603497C



/* RX_AGC_SNAP_12_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x40144980)                                                  */
/*       IVT_EX (0x40144980)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AGC_SNAP_12_QPIAGENT_DFX_CIO_REG 0x06034980



/* RX_AGC_SNAP_13_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x40144984)                                                  */
/*       IVT_EX (0x40144984)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AGC_SNAP_13_QPIAGENT_DFX_CIO_REG 0x06034984



/* RX_AGC_SNAP_14_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x40144988)                                                  */
/*       IVT_EX (0x40144988)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AGC_SNAP_14_QPIAGENT_DFX_CIO_REG 0x06034988



/* RX_AGC_SNAP_15_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x4014498C)                                                  */
/*       IVT_EX (0x4014498C)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AGC_SNAP_15_QPIAGENT_DFX_CIO_REG 0x0603498C



/* RX_AGC_SNAP_16_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x40144990)                                                  */
/*       IVT_EX (0x40144990)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AGC_SNAP_16_QPIAGENT_DFX_CIO_REG 0x06034990



/* RX_AGC_SNAP_17_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x40144994)                                                  */
/*       IVT_EX (0x40144994)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AGC_SNAP_17_QPIAGENT_DFX_CIO_REG 0x06034994



/* RX_AGC_SNAP_18_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x40144998)                                                  */
/*       IVT_EX (0x40144998)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AGC_SNAP_18_QPIAGENT_DFX_CIO_REG 0x06034998



/* RX_AGC_SNAP_19_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x4014499C)                                                  */
/*       IVT_EX (0x4014499C)                                                  */
/* Register default value:              0x00000000                            */
#define RX_AGC_SNAP_19_QPIAGENT_DFX_CIO_REG 0x0603499C



/* TX_FIR_DELTA_CM1_G0_QPIAGENT_DFX_CIO_REG supported on:                     */
/*       IVT_EP (0x401449A0)                                                  */
/*       IVT_EX (0x401449A0)                                                  */
/* Register default value:              0x00000000                            */
#define TX_FIR_DELTA_CM1_G0_QPIAGENT_DFX_CIO_REG 0x060349A0



/* TX_FIR_DELTA_CM1_G1_QPIAGENT_DFX_CIO_REG supported on:                     */
/*       IVT_EP (0x401449A4)                                                  */
/*       IVT_EX (0x401449A4)                                                  */
/* Register default value:              0x00000000                            */
#define TX_FIR_DELTA_CM1_G1_QPIAGENT_DFX_CIO_REG 0x060349A4



/* TX_FIR_DELTA_C0_G0_QPIAGENT_DFX_CIO_REG supported on:                      */
/*       IVT_EP (0x401449A8)                                                  */
/*       IVT_EX (0x401449A8)                                                  */
/* Register default value:              0x00000000                            */
#define TX_FIR_DELTA_C0_G0_QPIAGENT_DFX_CIO_REG 0x060349A8



/* TX_FIR_DELTA_C0_G1_QPIAGENT_DFX_CIO_REG supported on:                      */
/*       IVT_EP (0x401449AC)                                                  */
/*       IVT_EX (0x401449AC)                                                  */
/* Register default value:              0x00000000                            */
#define TX_FIR_DELTA_C0_G1_QPIAGENT_DFX_CIO_REG 0x060349AC



/* TX_FIR_DELTA_CP1_G0_QPIAGENT_DFX_CIO_REG supported on:                     */
/*       IVT_EP (0x401449B0)                                                  */
/*       IVT_EX (0x401449B0)                                                  */
/* Register default value:              0x00000000                            */
#define TX_FIR_DELTA_CP1_G0_QPIAGENT_DFX_CIO_REG 0x060349B0



/* TX_FIR_DELTA_CP1_G1_QPIAGENT_DFX_CIO_REG supported on:                     */
/*       IVT_EP (0x401449B4)                                                  */
/*       IVT_EX (0x401449B4)                                                  */
/* Register default value:              0x00000000                            */
#define TX_FIR_DELTA_CP1_G1_QPIAGENT_DFX_CIO_REG 0x060349B4



/* TX_FIR_VALID_QPIAGENT_DFX_CIO_REG supported on:                            */
/*       IVT_EP (0x401449B8)                                                  */
/*       IVT_EX (0x401449B8)                                                  */
/* Register default value:              0x00000000                            */
#define TX_FIR_VALID_QPIAGENT_DFX_CIO_REG 0x060349B8



/* TX_ADAPTATION_DONE_QPIAGENT_DFX_CIO_REG supported on:                      */
/*       IVT_EP (0x401449BC)                                                  */
/*       IVT_EX (0x401449BC)                                                  */
/* Register default value:              0x00000000                            */
#define TX_ADAPTATION_DONE_QPIAGENT_DFX_CIO_REG 0x060349BC



/* PIVCM1SEL_QPIAGENT_DFX_CIO_REG supported on:                               */
/*       IVT_EP (0x40144A00)                                                  */
/*       IVT_EX (0x40144A00)                                                  */
/* Register default value:              0x00000000                            */
#define PIVCM1SEL_QPIAGENT_DFX_CIO_REG 0x06034A00



/* PIVCM2SEL_QPIAGENT_DFX_CIO_REG supported on:                               */
/*       IVT_EP (0x40144A04)                                                  */
/*       IVT_EX (0x40144A04)                                                  */
/* Register default value:              0x00000000                            */
#define PIVCM2SEL_QPIAGENT_DFX_CIO_REG 0x06034A04



/* RT_BIN_OFFSET_RX_0_QPIAGENT_DFX_CIO_REG supported on:                      */
/*       IVT_EP (0x40144A08)                                                  */
/*       IVT_EX (0x40144A08)                                                  */
/* Register default value:              0x00000000                            */
#define RT_BIN_OFFSET_RX_0_QPIAGENT_DFX_CIO_REG 0x06034A08



/* RT_BIN_OFFSET_RX_1_QPIAGENT_DFX_CIO_REG supported on:                      */
/*       IVT_EP (0x40144A0C)                                                  */
/*       IVT_EX (0x40144A0C)                                                  */
/* Register default value:              0x00000000                            */
#define RT_BIN_OFFSET_RX_1_QPIAGENT_DFX_CIO_REG 0x06034A0C



/* RT_BIN_OFFSET_TX_0_QPIAGENT_DFX_CIO_REG supported on:                      */
/*       IVT_EP (0x40144A10)                                                  */
/*       IVT_EX (0x40144A10)                                                  */
/* Register default value:              0x00000000                            */
#define RT_BIN_OFFSET_TX_0_QPIAGENT_DFX_CIO_REG 0x06034A10



/* RT_BIN_OFFSET_TX_1_QPIAGENT_DFX_CIO_REG supported on:                      */
/*       IVT_EP (0x40144A14)                                                  */
/*       IVT_EX (0x40144A14)                                                  */
/* Register default value:              0x00000000                            */
#define RT_BIN_OFFSET_TX_1_QPIAGENT_DFX_CIO_REG 0x06034A14



/* RT_MODE_SEL_RX_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x40144A18)                                                  */
/*       IVT_EX (0x40144A18)                                                  */
/* Register default value:              0x00000000                            */
#define RT_MODE_SEL_RX_QPIAGENT_DFX_CIO_REG 0x06034A18



/* RT_MODE_SEL_TX_QPIAGENT_DFX_CIO_REG supported on:                          */
/*       IVT_EP (0x40144A1C)                                                  */
/*       IVT_EX (0x40144A1C)                                                  */
/* Register default value:              0x00000000                            */
#define RT_MODE_SEL_TX_QPIAGENT_DFX_CIO_REG 0x06034A1C



/* TX_ADAPT_DELTA_SW_QPIAGENT_DFX_CIO_REG supported on:                       */
/*       IVT_EP (0x40144A20)                                                  */
/*       IVT_EX (0x40144A20)                                                  */
/* Register default value:              0x00000000                            */
#define TX_ADAPT_DELTA_SW_QPIAGENT_DFX_CIO_REG 0x06034A20



/* BNDL_SPARE_0_QPIAGENT_DFX_CIO_REG supported on:                            */
/*       IVT_EP (0x40144A2C)                                                  */
/*       IVT_EX (0x40144A2C)                                                  */
/* Register default value:              0x00180018                            */
#define BNDL_SPARE_0_QPIAGENT_DFX_CIO_REG 0x06034A2C



/* BNDL_SPARE_1_QPIAGENT_DFX_CIO_REG supported on:                            */
/*       IVT_EP (0x40144A30)                                                  */
/*       IVT_EX (0x40144A30)                                                  */
/* Register default value:              0x00180018                            */
#define BNDL_SPARE_1_QPIAGENT_DFX_CIO_REG 0x06034A30



/* BNDL_SPARE_2_QPIAGENT_DFX_CIO_REG supported on:                            */
/*       IVT_EP (0x40144A34)                                                  */
/*       IVT_EX (0x40144A34)                                                  */
/* Register default value:              0x00180018                            */
#define BNDL_SPARE_2_QPIAGENT_DFX_CIO_REG 0x06034A34



/* BNDL_SPARE_3_QPIAGENT_DFX_CIO_REG supported on:                            */
/*       IVT_EP (0x40144A38)                                                  */
/*       IVT_EX (0x40144A38)                                                  */
/* Register default value:              0x00180018                            */
#define BNDL_SPARE_3_QPIAGENT_DFX_CIO_REG 0x06034A38



/* BNDL_SPARE_4_QPIAGENT_DFX_CIO_REG supported on:                            */
/*       IVT_EP (0x40144A3C)                                                  */
/*       IVT_EX (0x40144A3C)                                                  */
/* Register default value:              0x00180018                            */
#define BNDL_SPARE_4_QPIAGENT_DFX_CIO_REG 0x06034A3C



/* ENBL_CDR_HANG_DET_QPIAGENT_DFX_CIO_REG supported on:                       */
/*       IVT_EP (0x40144A40)                                                  */
/*       IVT_EX (0x40144A40)                                                  */
/* Register default value:              0x00000000                            */
#define ENBL_CDR_HANG_DET_QPIAGENT_DFX_CIO_REG 0x06034A40



/* FFE_BKCH_THRESHOLD_QPIAGENT_DFX_CIO_REG supported on:                      */
/*       IVT_EP (0x40144A4C)                                                  */
/*       IVT_EX (0x40144A4C)                                                  */
/* Register default value:              0x01550155                            */
#define FFE_BKCH_THRESHOLD_QPIAGENT_DFX_CIO_REG 0x06034A4C



/* TXEQ_LATCH_EN_QPIAGENT_DFX_CIO_REG supported on:                           */
/*       IVT_EP (0x40144A54)                                                  */
/*       IVT_EX (0x40144A54)                                                  */
/* Register default value:              0x03FF03FF                            */
#define TXEQ_LATCH_EN_QPIAGENT_DFX_CIO_REG 0x06034A54



/* PSA_CFG_QPIAGENT_DFX_CIO_REG supported on:                                 */
/*       IVT_EP (0x40144A58)                                                  */
/*       IVT_EX (0x40144A58)                                                  */
/* Register default value:              0x01550155                            */
#define PSA_CFG_QPIAGENT_DFX_CIO_REG 0x06034A58



/* PSA_CFGCFO_HALFSPEED_QPIAGENT_DFX_CIO_REG supported on:                    */
/*       IVT_EP (0x40144A5C)                                                  */
/*       IVT_EX (0x40144A5C)                                                  */
/* Register default value:              0x01550155                            */
#define PSA_CFGCFO_HALFSPEED_QPIAGENT_DFX_CIO_REG 0x06034A5C



/* TX_SKEW_CTRL_QPIAGENT_DFX_CIO_REG supported on:                            */
/*       IVT_EP (0x40144A68)                                                  */
/*       IVT_EX (0x40144A68)                                                  */
/* Register default value:              0x001F001F                            */
#define TX_SKEW_CTRL_QPIAGENT_DFX_CIO_REG 0x06034A68



#endif /* QPIAGENT_DFX_CIO_h */
