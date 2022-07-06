//
// This file contains an 'Intel Pre-EFI Module' and is licensed
// for Intel CPUs and Chipsets under the terms of your license
// agreement with Intel or your vendor.  This file may be
// modified by the user, subject to additional terms of the
// license agreement
//
#ifndef _rcregs_h
#define _rcregs_h

// AptioV server override - fix build error
#include "RcRegs/CBOBC_COH.h"
#include "RcRegs/CBOBC_IOSAD.h"
#include "RcRegs/CBOBC_MEMSAD.h"
#include "RcRegs/CBO_CFG.h" 
#include "RcRegs/HA_CFG.h"
#include "RcRegs/HA_DFX.h"
#include "RcRegs/HA_GDXC.h"
#include "RcRegs/HA_PMON.h"
#include "RcRegs/IIOCB_FUNC0.h"
#include "RcRegs/IIOCB_FUNC1.h"
#include "RcRegs/IIOCB_FUNC2.h"
#include "RcRegs/IIOCB_FUNC3.h"
#include "RcRegs/IIOCB_FUNC4.h"
#include "RcRegs/IIOCB_FUNC5.h"
#include "RcRegs/IIOCB_FUNC6.h"
#include "RcRegs/IIOCB_FUNC7.h"
#include "RcRegs/IIO_DFX.h"
#include "RcRegs/IIO_DFX_GLOBAL.h"
#include "RcRegs/IIO_DFX_VTD.h"
#include "RcRegs/IIO_PCIEDMI.h"
#include "RcRegs/IIO_IOAPIC.h"
#include "RcRegs/IIO_PCIE.h"
#include "RcRegs/IIO_PCIENTB.h"
#include "RcRegs/IIO_PERFMON.h"
#include "RcRegs/IIO_RAS.h"
#include "RcRegs/IIO_VTD.h"
#include "RcRegs/IIO_MEMHP.h"
#include "RcRegs/MCDDC_CTL.h"
#include "RcRegs/MCDDC_DESC.h"
#include "RcRegs/MCDDC_DP.h"
#include "RcRegs/MCIO_DDRIO.h"
#include "RcRegs/MCIO_DDRIOEXT.h"
#include "RcRegs/MCIO_DDRIOEXTGBC.h"
#include "RcRegs/MCIO_DDRIOEXTMCC.h"
#include "RcRegs/MCIO_DDRIOGBC.h"
#include "RcRegs/MCIO_DDRIOMCC.h"
#include "RcRegs/MC_MAIN.h"
#include "RcRegs/MC_MAINEXT.h"
#include "RcRegs/PCU_FUN0.h"
#include "RcRegs/PCU_FUN1.h"
#include "RcRegs/PCU_FUN2.h"
#include "RcRegs/PCU_FUN3.h"
#include "RcRegs/PCU_FUN4.h"
#include "RcRegs/PCU_FUN5.h"
#include "RcRegs/PCU_FUN6.h"
#include "RcRegs/PCU_FUN7.h"
#include "RcRegs/QPIAGENT_DFX_CIO.h"
#include "RcRegs/QPIAGENT_DFX_COMMON.h"
#include "RcRegs/QPIAGENT_DFX_LINK.h"
#include "RcRegs/QPIAGENT_DFX_PHY.h"
#include "RcRegs/QPIAGENT_DFX_PHY_IOVB.h"
#include "RcRegs/QPIAGENT_DFX_PHY_IOVC.h"
#include "RcRegs/QPIAGENT_DFX_PHY_PQD.h"
#include "RcRegs/QPIAGENT_LL.h"
#include "RcRegs/QPIAGENT_PMON.h"
#include "RcRegs/QPIAGENT_REUT.h"
#include "RcRegs/R2PCIE_CFG.h"
#include "RcRegs/R2PCIE_PMON.h"
#include "RcRegs/R3QPI_CFG.h"
#include "RcRegs/R3QPI_LNK0PMON.h"
#include "RcRegs/R3QPI_LNK1PMON.h"
#include "RcRegs/UBOX_CFG.h"
#include "RcRegs/UBOX_DFX.h"
#include "RcRegs/UBOX_MISC.h"
#include "RcRegs/VCU_FUN_0.h"
#include "RcRegs/VCU_FUN_2.h"
#include "RcRegs/SBO_CFG.h"
#include "RcRegs/CB33_FUN_0.h"
#include "RcRegs/GbE_PF0.h"
#include "RcRegs/GbE_VF0.h"
#include "RcRegs/IIO_DFX_IOSF0.h"
#include "RcRegs/IIO_DFX_IOSF2.h"
#include "RcRegs/IIO_PCIEIOSF0.h"
#include "RcRegs/IIO_PCIEIOSF2.h"
#include "RcRegs/PCU_PMC.h"

// Registers not defined in ConfigDB -- START
// Scratch register usage
//

#ifndef ASM_INC
// register at B:D:F: 1:11:0, box:function 8:0 (UBOX_CFG) , offset 0xB0. with SoftSDV/VP flag
// Scratch register usage
//
#define CSR_EMULATION_FLAG_UBOX_CFG_REG 0x080040B0

//
// For BDX, this register changed to B:D:F 0:0:0, box:function 14:0, offset 0xFC.
//
#define CSR_BDX_EMULATION_FLAG_CFG_REG  0x0e0040FC 

//#define   EMULATION_INFO_UBOX_CFG_REG   0x401580B0
#define   EMULATION_INFO_UBOX_CFG_REG     0x080040B0

// Registers not defined in ConfigDB -- END


//
// ConfigDB has as SV ONLY, but shouldnt be -- START
//

#define   UNKNOWN1_0_6_7_CFG_REG   0x40037640
#endif // ASM_INC

#endif //_rcregs_h

