/* Date Stamp: 8/23/2014 */

#ifndef MCIO_DDRIOGBC_h
#define MCIO_DDRIOGBC_h

#include "DataTypes.h"

/* Device and Function specifications:                                        */
/* For IVT_HOST:                                                              */
/* MCIO_DDRIOGBC_IVT_DEV 15                                                   */
/* MCIO_DDRIOGBC_IVT_FUN 6                                                    */
/* For HSX_HOST:                                                              */
/* MCIO_DDRIOGBC_HSX_DEV 26                                                   */
/* MCIO_DDRIOGBC_HSX_FUN 6                                                    */
/* For BDX_HOST:                                                              */
/* MCIO_DDRIOGBC_BDX_DEV 26                                                   */
/* MCIO_DDRIOGBC_BDX_FUN 6                                                    */

/* VID_MCIO_DDRIOGBC_REG supported on:                                        */
/*       IVT_EP (0x2017E000)                                                  */
/* Register default value:              0x8086                                */
#define VID_MCIO_DDRIOGBC_REG 0x0D002000



/* DID_MCIO_DDRIOGBC_REG supported on:                                        */
/*       IVT_EP (0x2017E002)                                                  */
/* Register default value:              0x0EAE                                */
#define DID_MCIO_DDRIOGBC_REG 0x0D002002



/* PCICMD_MCIO_DDRIOGBC_REG supported on:                                     */
/*       IVT_EP (0x2017E004)                                                  */
/* Register default value:              0x0000                                */
#define PCICMD_MCIO_DDRIOGBC_REG 0x0D002004



/* PCISTS_MCIO_DDRIOGBC_REG supported on:                                     */
/*       IVT_EP (0x2017E006)                                                  */
/* Register default value:              0x0010                                */
#define PCISTS_MCIO_DDRIOGBC_REG 0x0D002006



/* RID_MCIO_DDRIOGBC_REG supported on:                                        */
/*       IVT_EP (0x1017E008)                                                  */
/* Register default value:              0x00                                  */
#define RID_MCIO_DDRIOGBC_REG 0x0D001008



/* CCR_N0_MCIO_DDRIOGBC_REG supported on:                                     */
/*       IVT_EP (0x1017E009)                                                  */
/* Register default value:              0x00                                  */
#define CCR_N0_MCIO_DDRIOGBC_REG 0x0D001009



/* CCR_N1_MCIO_DDRIOGBC_REG supported on:                                     */
/*       IVT_EP (0x2017E00A)                                                  */
/* Register default value:              0x0880                                */
#define CCR_N1_MCIO_DDRIOGBC_REG 0x0D00200A



/* CLSR_MCIO_DDRIOGBC_REG supported on:                                       */
/*       IVT_EP (0x1017E00C)                                                  */
/* Register default value:              0x00                                  */
#define CLSR_MCIO_DDRIOGBC_REG 0x0D00100C



/* PLAT_MCIO_DDRIOGBC_REG supported on:                                       */
/*       IVT_EP (0x1017E00D)                                                  */
/* Register default value:              0x00                                  */
#define PLAT_MCIO_DDRIOGBC_REG 0x0D00100D



/* HDR_MCIO_DDRIOGBC_REG supported on:                                        */
/*       IVT_EP (0x1017E00E)                                                  */
/* Register default value:              0x80                                  */
#define HDR_MCIO_DDRIOGBC_REG 0x0D00100E



/* BIST_MCIO_DDRIOGBC_REG supported on:                                       */
/*       IVT_EP (0x1017E00F)                                                  */
/* Register default value:              0x00                                  */
#define BIST_MCIO_DDRIOGBC_REG 0x0D00100F



/* SVID_MCIO_DDRIOGBC_REG supported on:                                       */
/*       IVT_EP (0x2017E02C)                                                  */
/* Register default value:              0x8086                                */
#define SVID_MCIO_DDRIOGBC_REG 0x0D00202C



/* SDID_MCIO_DDRIOGBC_REG supported on:                                       */
/*       IVT_EP (0x2017E02E)                                                  */
/* Register default value:              0x0000                                */
#define SDID_MCIO_DDRIOGBC_REG 0x0D00202E



/* CAPPTR_MCIO_DDRIOGBC_REG supported on:                                     */
/*       IVT_EP (0x1017E034)                                                  */
/* Register default value:              0x40                                  */
#define CAPPTR_MCIO_DDRIOGBC_REG 0x0D001034



/* INTL_MCIO_DDRIOGBC_REG supported on:                                       */
/*       IVT_EP (0x1017E03C)                                                  */
/* Register default value:              0x00                                  */
#define INTL_MCIO_DDRIOGBC_REG 0x0D00103C



/* INTPIN_MCIO_DDRIOGBC_REG supported on:                                     */
/*       IVT_EP (0x1017E03D)                                                  */
/* Register default value:              0x00                                  */
#define INTPIN_MCIO_DDRIOGBC_REG 0x0D00103D



/* MINGNT_MCIO_DDRIOGBC_REG supported on:                                     */
/*       IVT_EP (0x1017E03E)                                                  */
/* Register default value:              0x00                                  */
#define MINGNT_MCIO_DDRIOGBC_REG 0x0D00103E



/* MAXLAT_MCIO_DDRIOGBC_REG supported on:                                     */
/*       IVT_EP (0x1017E03F)                                                  */
/* Register default value:              0x00                                  */
#define MAXLAT_MCIO_DDRIOGBC_REG 0x0D00103F



/* PXPCAP_MCIO_DDRIOGBC_REG supported on:                                     */
/*       IVT_EP (0x4017E040)                                                  */
/* Register default value:              0x00910010                            */
#define PXPCAP_MCIO_DDRIOGBC_REG 0x0D004040





/* GDCRTRAININGMODEA0_MCIO_DDRIOGBC_REG supported on:                         */
/*       IVT_EP (0x4017E108)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTRAININGMODEA0_MCIO_DDRIOGBC_REG 0x0D004108



/* GDCRTRAININGRESULT1A0_MCIO_DDRIOGBC_REG supported on:                      */
/*       IVT_EP (0x4017E10C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTRAININGRESULT1A0_MCIO_DDRIOGBC_REG 0x0D00410C



/* GDCRTRAININGRESULT2A0_MCIO_DDRIOGBC_REG supported on:                      */
/*       IVT_EP (0x4017E110)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTRAININGRESULT2A0_MCIO_DDRIOGBC_REG 0x0D004110



/* GDCRDATACOMPBA0_MCIO_DDRIOGBC_REG supported on:                            */
/*       IVT_EP (0x4017E114)                                                  */
/* Register default value:              0x24000000                            */
#define GDCRDATACOMPBA0_MCIO_DDRIOGBC_REG 0x0D004114



/* GDCRBUFFCFGA0_MCIO_DDRIOGBC_REG supported on:                              */
/*       IVT_EP (0x4017E118)                                                  */
/* Register default value:              0x03FF0001                            */
#define GDCRBUFFCFGA0_MCIO_DDRIOGBC_REG 0x0D004118



/* GDCRDATADEBUGMUXCFGA0_MCIO_DDRIOGBC_REG supported on:                      */
/*       IVT_EP (0x4017E11C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRDATADEBUGMUXCFGA0_MCIO_DDRIOGBC_REG 0x0D00411C



/* GDCRTXRXBOTRANK0A0_MCIO_DDRIOGBC_REG supported on:                         */
/*       IVT_EP (0x4017E120)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXRXBOTRANK0A0_MCIO_DDRIOGBC_REG 0x0D004120



/* GDCRTXRXBOTRANK1A0_MCIO_DDRIOGBC_REG supported on:                         */
/*       IVT_EP (0x4017E124)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXRXBOTRANK1A0_MCIO_DDRIOGBC_REG 0x0D004124



/* GDCRTXRXBOTRANK2A0_MCIO_DDRIOGBC_REG supported on:                         */
/*       IVT_EP (0x4017E128)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXRXBOTRANK2A0_MCIO_DDRIOGBC_REG 0x0D004128



/* GDCRTXRXBOTRANK3A0_MCIO_DDRIOGBC_REG supported on:                         */
/*       IVT_EP (0x4017E12C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXRXBOTRANK3A0_MCIO_DDRIOGBC_REG 0x0D00412C



/* GDCRTXRXBOTRANK4A0_MCIO_DDRIOGBC_REG supported on:                         */
/*       IVT_EP (0x4017E130)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXRXBOTRANK4A0_MCIO_DDRIOGBC_REG 0x0D004130



/* GDCRTXRXBOTRANK5A0_MCIO_DDRIOGBC_REG supported on:                         */
/*       IVT_EP (0x4017E134)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXRXBOTRANK5A0_MCIO_DDRIOGBC_REG 0x0D004134



/* GDCRTXRXBOTRANK6A0_MCIO_DDRIOGBC_REG supported on:                         */
/*       IVT_EP (0x4017E138)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXRXBOTRANK6A0_MCIO_DDRIOGBC_REG 0x0D004138



/* GDCRTXRXBOTRANK7A0_MCIO_DDRIOGBC_REG supported on:                         */
/*       IVT_EP (0x4017E13C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXRXBOTRANK7A0_MCIO_DDRIOGBC_REG 0x0D00413C



/* GDCRRXTOPRANK0A0_MCIO_DDRIOGBC_REG supported on:                           */
/*       IVT_EP (0x4017E140)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRRXTOPRANK0A0_MCIO_DDRIOGBC_REG 0x0D004140



/* GDCRRXTOPRANK1A0_MCIO_DDRIOGBC_REG supported on:                           */
/*       IVT_EP (0x4017E144)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRRXTOPRANK1A0_MCIO_DDRIOGBC_REG 0x0D004144



/* GDCRRXTOPRANK2A0_MCIO_DDRIOGBC_REG supported on:                           */
/*       IVT_EP (0x4017E148)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRRXTOPRANK2A0_MCIO_DDRIOGBC_REG 0x0D004148



/* GDCRRXTOPRANK3A0_MCIO_DDRIOGBC_REG supported on:                           */
/*       IVT_EP (0x4017E14C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRRXTOPRANK3A0_MCIO_DDRIOGBC_REG 0x0D00414C



/* GDCRRXTOPRANK4A0_MCIO_DDRIOGBC_REG supported on:                           */
/*       IVT_EP (0x4017E150)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRRXTOPRANK4A0_MCIO_DDRIOGBC_REG 0x0D004150



/* GDCRRXTOPRANK5A0_MCIO_DDRIOGBC_REG supported on:                           */
/*       IVT_EP (0x4017E154)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRRXTOPRANK5A0_MCIO_DDRIOGBC_REG 0x0D004154



/* GDCRRXTOPRANK6A0_MCIO_DDRIOGBC_REG supported on:                           */
/*       IVT_EP (0x4017E158)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRRXTOPRANK6A0_MCIO_DDRIOGBC_REG 0x0D004158



/* GDCRRXTOPRANK7A0_MCIO_DDRIOGBC_REG supported on:                           */
/*       IVT_EP (0x4017E15C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRRXTOPRANK7A0_MCIO_DDRIOGBC_REG 0x0D00415C



/* GDCRTXTOPRANK0A0_MCIO_DDRIOGBC_REG supported on:                           */
/*       IVT_EP (0x4017E160)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXTOPRANK0A0_MCIO_DDRIOGBC_REG 0x0D004160



/* GDCRTXTOPRANK1A0_MCIO_DDRIOGBC_REG supported on:                           */
/*       IVT_EP (0x4017E164)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXTOPRANK1A0_MCIO_DDRIOGBC_REG 0x0D004164



/* GDCRTXTOPRANK2A0_MCIO_DDRIOGBC_REG supported on:                           */
/*       IVT_EP (0x4017E168)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXTOPRANK2A0_MCIO_DDRIOGBC_REG 0x0D004168



/* GDCRTXTOPRANK3A0_MCIO_DDRIOGBC_REG supported on:                           */
/*       IVT_EP (0x4017E16C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXTOPRANK3A0_MCIO_DDRIOGBC_REG 0x0D00416C



/* GDCRTXTOPRANK4A0_MCIO_DDRIOGBC_REG supported on:                           */
/*       IVT_EP (0x4017E170)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXTOPRANK4A0_MCIO_DDRIOGBC_REG 0x0D004170



/* GDCRTXTOPRANK5A0_MCIO_DDRIOGBC_REG supported on:                           */
/*       IVT_EP (0x4017E174)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXTOPRANK5A0_MCIO_DDRIOGBC_REG 0x0D004174



/* GDCRTXTOPRANK6A0_MCIO_DDRIOGBC_REG supported on:                           */
/*       IVT_EP (0x4017E178)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXTOPRANK6A0_MCIO_DDRIOGBC_REG 0x0D004178



/* GDCRTXTOPRANK7A0_MCIO_DDRIOGBC_REG supported on:                           */
/*       IVT_EP (0x4017E17C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRTXTOPRANK7A0_MCIO_DDRIOGBC_REG 0x0D00417C







/* GDCRCMDVREFA0_MCIO_DDRIOGBC_REG supported on:                              */
/*       IVT_EP (0x4017E30C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCMDVREFA0_MCIO_DDRIOGBC_REG 0x0D00430C



/* GDCRCMDPICODEA0_MCIO_DDRIOGBC_REG supported on:                            */
/*       IVT_EP (0x4017E310)                                                  */
/* Register default value:              0x40404040                            */
#define GDCRCMDPICODEA0_MCIO_DDRIOGBC_REG 0x0D004310



/* GDCRCMDLOGICPHASEDELAYA0_MCIO_DDRIOGBC_REG supported on:                   */
/*       IVT_EP (0x4017E314)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCMDLOGICPHASEDELAYA0_MCIO_DDRIOGBC_REG 0x0D004314



/* GDCRCMDDEBUGMUXDFTA0_MCIO_DDRIOGBC_REG supported on:                       */
/*       IVT_EP (0x4017E318)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCMDDEBUGMUXDFTA0_MCIO_DDRIOGBC_REG 0x0D004318





/* GDCRRANKCFGA0_MCIO_DDRIOGBC_REG supported on:                              */
/*       IVT_EP (0x2017E320)                                                  */
/* Register default value:              0xF3FF                                */
#define GDCRRANKCFGA0_MCIO_DDRIOGBC_REG 0x0D002320



/* GDCRCMDPICODE2A0_MCIO_DDRIOGBC_REG supported on:                           */
/*       IVT_EP (0x4017E324)                                                  */
/* Register default value:              0x00004040                            */
#define GDCRCMDPICODE2A0_MCIO_DDRIOGBC_REG 0x0D004324





/* GDCRCKCOMP2A0_MCIO_DDRIOGBC_REG supported on:                              */
/*       IVT_EP (0x4017E388)                                                  */
/* Register default value:              0x00010491                            */
#define GDCRCKCOMP2A0_MCIO_DDRIOGBC_REG 0x0D004388



/* GDCRCKRANKUSEDA0_MCIO_DDRIOGBC_REG supported on:                           */
/*       IVT_EP (0x2017E38C)                                                  */
/* Register default value:              0x0033                                */
#define GDCRCKRANKUSEDA0_MCIO_DDRIOGBC_REG 0x0D00238C



/* GDCRCKPICODE0A0_MCIO_DDRIOGBC_REG supported on:                            */
/*       IVT_EP (0x4017E390)                                                  */
/* Register default value:              0x03000000                            */
#define GDCRCKPICODE0A0_MCIO_DDRIOGBC_REG 0x0D004390



/* GDCRCKPICODE1A0_MCIO_DDRIOGBC_REG supported on:                            */
/*       IVT_EP (0x4017E394)                                                  */
/* Register default value:              0x03000000                            */
#define GDCRCKPICODE1A0_MCIO_DDRIOGBC_REG 0x0D004394



/* GDCRCKLOGICDELAYA0_MCIO_DDRIOGBC_REG supported on:                         */
/*       IVT_EP (0x2017E398)                                                  */
/* Register default value:              0x0000                                */
#define GDCRCKLOGICDELAYA0_MCIO_DDRIOGBC_REG 0x0D002398







/* GDCRCLKDLLDFTA0_MCIO_DDRIOGBC_REG supported on:                            */
/*       IVT_EP (0x2017E3A4)                                                  */
/* Register default value:              0x0000                                */
#define GDCRCLKDLLDFTA0_MCIO_DDRIOGBC_REG 0x0D0023A4



/* GDCRCLKVSSHIA0_MCIO_DDRIOGBC_REG supported on:                             */
/*       IVT_EP (0x4017E3A8)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCLKVSSHIA0_MCIO_DDRIOGBC_REG 0x0D0043A8



/* GDCRCLKVMCFGA0_MCIO_DDRIOGBC_REG supported on:                             */
/*       IVT_EP (0x1017E3AC)                                                  */
/* Register default value:              0x00                                  */
#define GDCRCLKVMCFGA0_MCIO_DDRIOGBC_REG 0x0D0013AC









/* GDCRCOMPOVR2A0_MCIO_DDRIOGBC_REG supported on:                             */
/*       IVT_EP (0x4017E408)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCOMPOVR2A0_MCIO_DDRIOGBC_REG 0x0D004408



/* GDCRCOMPOVR3A0_MCIO_DDRIOGBC_REG supported on:                             */
/*       IVT_EP (0x4017E40C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCOMPOVR3A0_MCIO_DDRIOGBC_REG 0x0D00440C



/* GDCRCOMPOVR4A0_MCIO_DDRIOGBC_REG supported on:                             */
/*       IVT_EP (0x4017E410)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCOMPOVR4A0_MCIO_DDRIOGBC_REG 0x0D004410



/* GDCRCOMPOVR5A0_MCIO_DDRIOGBC_REG supported on:                             */
/*       IVT_EP (0x4017E414)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCOMPOVR5A0_MCIO_DDRIOGBC_REG 0x0D004414



/* GDCRCOMPOVR6A0_MCIO_DDRIOGBC_REG supported on:                             */
/*       IVT_EP (0x4017E418)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCOMPOVR6A0_MCIO_DDRIOGBC_REG 0x0D004418



/* GDCRCOMPOVROFST1A0_MCIO_DDRIOGBC_REG supported on:                         */
/*       IVT_EP (0x4017E41C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCOMPOVROFST1A0_MCIO_DDRIOGBC_REG 0x0D00441C



/* GDCRCOMPOVROFST2A0_MCIO_DDRIOGBC_REG supported on:                         */
/*       IVT_EP (0x4017E420)                                                  */
/* Register default value:              0x03F00000                            */
#define GDCRCOMPOVROFST2A0_MCIO_DDRIOGBC_REG 0x0D004420



/* GDCRCOMPOVROFST3A0_MCIO_DDRIOGBC_REG supported on:                         */
/*       IVT_EP (0x4017E424)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCOMPOVROFST3A0_MCIO_DDRIOGBC_REG 0x0D004424



/* GDCRCOMPOVROFST4A0_MCIO_DDRIOGBC_REG supported on:                         */
/*       IVT_EP (0x4017E428)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCOMPOVROFST4A0_MCIO_DDRIOGBC_REG 0x0D004428



/* GDCRCOMPCFGSPDA0_MCIO_DDRIOGBC_REG supported on:                           */
/*       IVT_EP (0x4017E42C)                                                  */
/* Register default value:              0x00010B5A                            */
#define GDCRCOMPCFGSPDA0_MCIO_DDRIOGBC_REG 0x0D00442C







/* GDCRMSCCTLA0_MCIO_DDRIOGBC_REG supported on:                               */
/*       IVT_EP (0x4017E43C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRMSCCTLA0_MCIO_DDRIOGBC_REG 0x0D00443C



/* GDCRCTLCOMPTXA0_MCIO_DDRIOGBC_REG supported on:                            */
/*       IVT_EP (0x4017E704)                                                  */
/* Register default value:              0x1F803820                            */
#define GDCRCTLCOMPTXA0_MCIO_DDRIOGBC_REG 0x0D004704





/* GDCRCOMPOFFSET_RXA0_MCIO_DDRIOGBC_REG supported on:                        */
/*       IVT_EP (0x4017E70C)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCOMPOFFSET_RXA0_MCIO_DDRIOGBC_REG 0x0D00470C



/* GDCRCTLVREFRXBIASDCDETECTA0_MCIO_DDRIOGBC_REG supported on:                */
/*       IVT_EP (0x4017E710)                                                  */
/* Register default value:              0x00050000                            */
#define GDCRCTLVREFRXBIASDCDETECTA0_MCIO_DDRIOGBC_REG 0x0D004710



/* GDCRCTLPICODE03A0_MCIO_DDRIOGBC_REG supported on:                          */
/*       IVT_EP (0x4017E714)                                                  */
/* Register default value:              0x40404040                            */
#define GDCRCTLPICODE03A0_MCIO_DDRIOGBC_REG 0x0D004714



/* GDCRCTLPICODE47A0_MCIO_DDRIOGBC_REG supported on:                          */
/*       IVT_EP (0x4017E718)                                                  */
/* Register default value:              0x40404040                            */
#define GDCRCTLPICODE47A0_MCIO_DDRIOGBC_REG 0x0D004718



/* GDCRCTLPICODE810A0_MCIO_DDRIOGBC_REG supported on:                         */
/*       IVT_EP (0x4017E71C)                                                  */
/* Register default value:              0x00404040                            */
#define GDCRCTLPICODE810A0_MCIO_DDRIOGBC_REG 0x0D00471C





/* GDCRCTLDEBUGMUXDFTA0_MCIO_DDRIOGBC_REG supported on:                       */
/*       IVT_EP (0x4017E724)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCTLDEBUGMUXDFTA0_MCIO_DDRIOGBC_REG 0x0D004724



/* GDCRCTLRANKCNFGXTALK1A0_MCIO_DDRIOGBC_REG supported on:                    */
/*       IVT_EP (0x4017E728)                                                  */
/* Register default value:              0x000003FF                            */
#define GDCRCTLRANKCNFGXTALK1A0_MCIO_DDRIOGBC_REG 0x0D004728



/* GDCRCTLXTALK2ANDVMSEODTA0_MCIO_DDRIOGBC_REG supported on:                  */
/*       IVT_EP (0x4017E72C)                                                  */
/* Register default value:              0x10000000                            */
#define GDCRCTLXTALK2ANDVMSEODTA0_MCIO_DDRIOGBC_REG 0x0D00472C



/* GDCRCTLVSSHIA0_MCIO_DDRIOGBC_REG supported on:                             */
/*       IVT_EP (0x4017E730)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCTLVSSHIA0_MCIO_DDRIOGBC_REG 0x0D004730



/* GDCRCTLVSSHIDBGCNTLA0_MCIO_DDRIOGBC_REG supported on:                      */
/*       IVT_EP (0x4017E734)                                                  */
/* Register default value:              0x00000500                            */
#define GDCRCTLVSSHIDBGCNTLA0_MCIO_DDRIOGBC_REG 0x0D004734



/* GDCRCTLACIOLBA0_MCIO_DDRIOGBC_REG supported on:                            */
/*       IVT_EP (0x4017E738)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCTLACIOLBA0_MCIO_DDRIOGBC_REG 0x0D004738



/* GDCRVMSEERRORA0_MCIO_DDRIOGBC_REG supported on:                            */
/*       IVT_EP (0x4017E73C)                                                  */
/* Register default value:              0x03FFFF00                            */
#define GDCRVMSEERRORA0_MCIO_DDRIOGBC_REG 0x0D00473C



/* GDCRCTLLOGICDELAYSELECTA0_MCIO_DDRIOGBC_REG supported on:                  */
/*       IVT_EP (0x4017E740)                                                  */
/* Register default value:              0x00000000                            */
#define GDCRCTLLOGICDELAYSELECTA0_MCIO_DDRIOGBC_REG 0x0D004740





#endif /* MCIO_DDRIOGBC_h */
