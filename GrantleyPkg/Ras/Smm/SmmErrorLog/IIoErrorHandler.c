/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

Copyright (c) 2009-2017 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  IIoErrorHandler.c

Abstract:

  IIO based component hooks for event logging.

-------------------------------------------------------------------*/
#include <Token.h> // AptioV server override
#include "CommonErrorHandlerIncludes.h"

BOOLEAN  mErrorPinEnabled =FALSE;
//BOOLEAN  mErrorIOMCAEnabled = FALSE; 

// IOH_DATA structure is used in runtime when IOH UDS protocol is not available.
IOH_INFO  mIohInfo[MAX_SOCKET] = {{(UINT8) -1, (UINT8) -1, (UINT8) -1, (UINT8) -1 },
                               {(UINT8) -1, (UINT8) -1, (UINT8) -1, (UINT8) -1  },
                               {(UINT8) -1, (UINT8) -1, (UINT8) -1, (UINT8) -1  }};

SRIOV_UR_ERR_DEV_INFO    mErrDevIohInfo[MAX_NUM_ERR];

UINT8   mPciXPUnsupportedRequest;
UINT8   mNextDevNum = 00;

// Number of IOHs
UINT8   mNumIoh; 
UINT8   mPcieCorErrThreshold;


//
// ****************************************************************************************
//
//      ENABLE ERROR LOG PORTION
//      Routines that enable Error Logging for each function/subsystem
//
// ***************************************************************************************
//

VOID
ClearGlobalErrorRegisterStatus (
  UINT8 SktId)
{

  GFERRST_IIO_RAS_STRUCT iioGFERRSTReg;
  GNERRST_IIO_RAS_STRUCT iioGNERRSTReg;
  ERRPINSTS_IIO_RAS_STRUCT iioERRPINSTReg;
  GSYSST_IIO_RAS_STRUCT iioGSYSSTReg;
  GCERRST_IIO_RAS_STRUCT     iioGCERRSTReg;

  //
  // Clear the Global Non-Fatal and Fatal registers
  //

  iioGFERRSTReg.Data = mCpuCsrAccess->ReadCpuCsr(  SktId, 0, GFERRST_IIO_RAS_REG);
  mCpuCsrAccess->WriteCpuCsr(  SktId, 0, GFERRST_IIO_RAS_REG, iioGFERRSTReg.Data);

  iioGNERRSTReg.Data = mCpuCsrAccess->ReadCpuCsr(  SktId, 0, GNERRST_IIO_RAS_REG);
  mCpuCsrAccess->WriteCpuCsr(  SktId, 0, GNERRST_IIO_RAS_REG, iioGNERRSTReg.Data);

  //Clear the Global Correctable Register

  iioGCERRSTReg.Data = mCpuCsrAccess->ReadCpuCsr( SktId, 0, GCERRST_IIO_RAS_REG);
  mCpuCsrAccess->WriteCpuCsr(SktId, 0, GCERRST_IIO_RAS_REG, iioGCERRSTReg.Data);

  //
  // Global System Event Status (RO)
  //
  iioGSYSSTReg.Data = mCpuCsrAccess->ReadCpuCsr(  SktId, 0, GSYSST_IIO_RAS_REG);
  mCpuCsrAccess->WriteCpuCsr(  SktId, 0, GSYSST_IIO_RAS_REG, iioGSYSSTReg.Data);

  //
  // Clear error status registerts if we are handling errors through error pins
  //
  if(mErrorPinEnabled == TRUE)  {
    iioERRPINSTReg.Data = mCpuCsrAccess->ReadCpuCsr(  SktId, 0, ERRPINSTS_IIO_RAS_REG);
    mCpuCsrAccess->WriteCpuCsr(  SktId, 0, ERRPINSTS_IIO_RAS_REG, iioERRPINSTReg.Data);
  }
}


VOID
ClearLocalErrorRegisterStatus (
  UINT8 SktId)
{ 
  VTUNCERRSTS_IIO_VTD_HSX_BDX_STRUCT     iioVTUNCERRSTSReg = {0}; //VTUNCERRSTS_IIO_VTD_REG
  IIOERRST_IIO_RAS_HSX_BDX_STRUCT        iioERRSTReg = {0}; //IIOERRST_IIO_RAS_REG
  IRPP0ERRST_IIO_RAS_BDX_STRUCT          iioIRPP0ERRSTReg = {0}; // IRPP0ERRST_IIO_RAS_REG
  IRPP1ERRST_IIO_RAS_BDX_STRUCT          iioIRPP1ERRSTReg = {0}; //IRPP1ERRST_IIO_RAS_REG
  MIERRST_IIO_RAS_STRUCT iioMIERRSTReg = {0}; //MIERRST_IIO_RAS_REG
  CHANERR_INT_IIOCB_FUNC0_STRUCT iioCBFUNC0Reg = {0}; //CHANERR_INT_IIOCB_FUNC0_REG
  CHANERR_INT_IIOCB_FUNC1_STRUCT iioCBFUNC1Reg = {0}; //CHANERR_INT_IIOCB_FUNC1_REG
  CHANERR_INT_IIOCB_FUNC2_STRUCT iioCBFUNC2Reg = {0}; //CHANERR_INT_IIOCB_FUNC2_REG
  CHANERR_INT_IIOCB_FUNC3_STRUCT iioCBFUNC3Reg = {0}; //CHANERR_INT_IIOCB_FUNC3_REG
  CHANERR_INT_IIOCB_FUNC4_STRUCT iioCBFUNC4Reg = {0}; //CHANERR_INT_IIOCB_FUNC4_REG
  CHANERR_INT_IIOCB_FUNC5_STRUCT iioCBFUNC5Reg = {0}; //CHANERR_INT_IIOCB_FUNC5_REG
  CHANERR_INT_IIOCB_FUNC6_STRUCT iioCBFUNC6Reg = {0}; //CHANERR_INT_IIOCB_FUNC6_REG
  CHANERR_INT_IIOCB_FUNC7_STRUCT iioCBFUNC7Reg = {0}; //CHANERR_INT_IIOCB_FUNC7_REG
  DMAUNCERRSTS_IIOCB_FUNC0_STRUCT    iioDMAUNCERRSTSReg = {0};

  //Clear VT Error Status
  iioVTUNCERRSTSReg.Data = mCpuCsrAccess->ReadCpuCsr(SktId, 0, VTUNCERRSTS_IIO_VTD_REG);
  if(iioVTUNCERRSTSReg.Data != 0)
    mCpuCsrAccess->WriteCpuCsr(SktId, 0, VTUNCERRSTS_IIO_VTD_REG, iioVTUNCERRSTSReg.Data);

  //Clear MISC Error Status 
  iioMIERRSTReg.Data = mCpuCsrAccess->ReadCpuCsr(SktId, 0, MIERRST_IIO_RAS_REG);
  if(iioMIERRSTReg.Data != 0)
    mCpuCsrAccess->WriteCpuCsr(SktId, 0, MIERRST_IIO_RAS_REG, iioMIERRSTReg.Data);

   //Clear IIO Error Status 
  iioERRSTReg.Data = mCpuCsrAccess->ReadCpuCsr(SktId, 0, IIOERRST_IIO_RAS_REG);
  if(iioERRSTReg.Data != 0)
    mCpuCsrAccess->WriteCpuCsr(SktId, 0, IIOERRST_IIO_RAS_REG, iioERRSTReg.Data);

  //Clear Coherent Interface Error Status 
  iioIRPP0ERRSTReg.Data = mCpuCsrAccess->ReadCpuCsr(SktId, 0, IRPP0ERRST_IIO_RAS_REG);
  if(iioIRPP0ERRSTReg.Data != 0)
    mCpuCsrAccess->WriteCpuCsr(SktId, 0, IRPP0ERRST_IIO_RAS_REG, iioIRPP0ERRSTReg.Data);

  //Clear Coherent Interface Error Status
  iioIRPP1ERRSTReg.Data = mCpuCsrAccess->ReadCpuCsr(SktId, 0, IRPP1ERRST_IIO_RAS_REG);
  if(iioIRPP1ERRSTReg.Data != 0)
    mCpuCsrAccess->WriteCpuCsr(SktId, 0, IRPP1ERRST_IIO_RAS_REG, iioIRPP1ERRSTReg.Data);

  //Clear Uncorrectable DMA Error Status Registers
  iioDMAUNCERRSTSReg.Data = mCpuCsrAccess->ReadCpuCsr(SktId, 0, DMAUNCERRSTS_IIOCB_FUNC0_REG);
  if(iioDMAUNCERRSTSReg.Data != 0) {
    mCpuCsrAccess->WriteCpuCsr(SktId, 0, DMAUNCERRSTS_IIOCB_FUNC0_REG, iioDMAUNCERRSTSReg.Data);
  }

  //Clear CB DMA Error Status Registers
  iioCBFUNC0Reg.Data = mCpuCsrAccess->ReadCpuCsr(SktId, 0, CHANERR_INT_IIOCB_FUNC0_REG);
  if(iioCBFUNC0Reg.Data != 0)
    mCpuCsrAccess->WriteCpuCsr(SktId, 0, CHANERR_INT_IIOCB_FUNC0_REG, iioCBFUNC0Reg.Data);

  iioCBFUNC1Reg.Data = mCpuCsrAccess->ReadCpuCsr(SktId, 0, CHANERR_INT_IIOCB_FUNC1_REG);
  if(iioCBFUNC1Reg.Data != 0)
    mCpuCsrAccess->WriteCpuCsr(SktId, 0, CHANERR_INT_IIOCB_FUNC1_REG, iioCBFUNC1Reg.Data);

  iioCBFUNC2Reg.Data = mCpuCsrAccess->ReadCpuCsr(SktId, 0, CHANERR_INT_IIOCB_FUNC2_REG);
  if(iioCBFUNC2Reg.Data != 0)
    mCpuCsrAccess->WriteCpuCsr(SktId, 0, CHANERR_INT_IIOCB_FUNC2_REG, iioCBFUNC2Reg.Data);

  iioCBFUNC3Reg.Data = mCpuCsrAccess->ReadCpuCsr(SktId, 0, CHANERR_INT_IIOCB_FUNC3_REG);
  if(iioCBFUNC3Reg.Data != 0)
    mCpuCsrAccess->WriteCpuCsr(SktId, 0, CHANERR_INT_IIOCB_FUNC3_REG, iioCBFUNC3Reg.Data);

  iioCBFUNC4Reg.Data = mCpuCsrAccess->ReadCpuCsr(SktId, 0, CHANERR_INT_IIOCB_FUNC4_REG);
  if(iioCBFUNC4Reg.Data != 0)
    mCpuCsrAccess->WriteCpuCsr(SktId, 0, CHANERR_INT_IIOCB_FUNC4_REG, iioCBFUNC4Reg.Data);

  iioCBFUNC5Reg.Data = mCpuCsrAccess->ReadCpuCsr(SktId, 0, CHANERR_INT_IIOCB_FUNC5_REG);
  if(iioCBFUNC5Reg.Data != 0)
    mCpuCsrAccess->WriteCpuCsr(SktId, 0, CHANERR_INT_IIOCB_FUNC5_REG, iioCBFUNC5Reg.Data);

  iioCBFUNC6Reg.Data = mCpuCsrAccess->ReadCpuCsr(SktId, 0, CHANERR_INT_IIOCB_FUNC6_REG);
  if(iioCBFUNC6Reg.Data != 0)
    mCpuCsrAccess->WriteCpuCsr(SktId, 0, CHANERR_INT_IIOCB_FUNC6_REG, iioCBFUNC6Reg.Data);

  iioCBFUNC7Reg.Data = mCpuCsrAccess->ReadCpuCsr(SktId, 0, CHANERR_INT_IIOCB_FUNC7_REG);
  if(iioCBFUNC7Reg.Data != 0)
    mCpuCsrAccess->WriteCpuCsr(SktId, 0, CHANERR_INT_IIOCB_FUNC7_REG, iioCBFUNC7Reg.Data);
  
}

VOID
ClearIIOErrors(VOID)
{
	UINT8 RootBridgeLoop;

	for ( RootBridgeLoop =0; RootBridgeLoop < MAX_SOCKET; RootBridgeLoop++) {
	  if (mIohInfo[RootBridgeLoop].Valid != 1)
		continue;
	  ClearIIOErrorRegisterStatus(mIohInfo[RootBridgeLoop].SocketId);
	}
}

VOID
ClearIIOErrorRegisterStatus (
  UINT8 SktId)
{ 
  ClearLocalErrorRegisterStatus(SktId); 
  ClearGlobalErrorRegisterStatus(SktId);
  ClearIioPcieErrorsRegisterStatus();
}

VOID
ClearIohExtPcieDeviceStatus(
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
{
  UINT32 Data32;
  UINT16 Data16;
  UINT32  ClassCode = 0;
  LNKSTS_IIO_PCIE_STRUCT       LinkSts;
  UINT8                        Var8;

  Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(XPCORERRSTS_IIO_PCIE_REG)));
  if (Data32 & 01) {
    ClassCode = PciExpressRead32 (PCI_PCIE_ADDR(Bus,Device, Function,ONLY_REGISTER_OFFSET(RID_IIO_PCIE_REG)));
    if((ClassCode >> 8) == V_PCIE_CCR_PCI_HOST_BRIDGE) { // DMI link
      LinkSts.Data = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIEDMI_REG)));
      PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIEDMI_REG)), LinkSts.Data);
    } else {
        if((Device == NTB_DEV) && (Function == NTB_FUNC)) {
           Var8 = PciExpressRead8(PCI_PCIE_ADDR(Bus, Device, Function,R_NTB_PPD));
           if( (Var8 & B_NTB_PORT_DEF) != 0) {
            LinkSts.Data = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIENTB_REG)));
            PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIENTB_REG)), LinkSts.Data);
           }
        }else {
      LinkSts.Data = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIE_REG)));
      PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIE_REG)), LinkSts.Data); 
        }
    }
  }
  PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(XPCORERRSTS_IIO_PCIE_REG)), Data32);

  if(mAllowClearingOfUEStatus)  {
    Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(XPUNCERRSTS_IIO_PCIE_REG)));
    PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(XPUNCERRSTS_IIO_PCIE_REG)), Data32);
  }

  Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(UNCERRSTS_IIO_PCIE_REG)));//0.1.0
  PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(UNCERRSTS_IIO_PCIE_REG)), Data32);

  Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(CORERRSTS_IIO_PCIE_REG)));//0.1.0
  PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(CORERRSTS_IIO_PCIE_REG)), Data32);
 
  Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(RPERRSTS_IIO_PCIENTB_REG)));//0.3.0
  PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(RPERRSTS_IIO_PCIENTB_REG)), Data32);

  Data16 = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(XPGLBERRSTS_IIO_PCIE_REG)));
  PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(XPGLBERRSTS_IIO_PCIE_REG)), Data16);
}

VOID
WmaaErrReportingEn(UINT8 Bus, UINT8 Device, UINT8 Function)
{
  DEVCTRL_IIO_PCIE_STRUCT      iioDevCtrlReg;
  ROOTCON_IIO_PCIE_STRUCT      iioRootConReg;
  XPUNCEDMASK_IIO_PCIE_STRUCT  iioXPUncErrDMskReg;
  CORERRMSK_IIO_PCIE_STRUCT    iioAerCorErrMskReg; //0.1.0
  GSYSCTL_IIO_RAS_STRUCT       iioGSysCtlReg;
  GERRCTL_IIO_RAS_STRUCT   iioGErrCtlReg;
  SYSMAP_IIO_RAS_STRUCT        iioSysMapReg;
  CIPINTRC_IIO_VTD_STRUCT iioCipIntrCtrlReg;
  UNCERRMSK_IIO_PCIE_BDX_STRUCT    iioUncemaskReg;
  UNCERRSEV_IIO_PCIE_BDX_STRUCT    iioUncerrSevReg;
  UNCEDMASK_IIO_PCIE_BDX_STRUCT    iioUncedmaskReg;
  COREDMASK_IIO_PCIE_STRUCT        iioCoredmaskReg;
  PCIERRSV_IIO_RAS_STRUCT      iioPCIErrSvReg;
  UINT8                        RootBridgeLoop;
  UINT8                        socket;

  // DEVCTRL register 
  iioDevCtrlReg.Data = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(DEVCTRL_IIO_PCIE_REG)));
  iioDevCtrlReg.Bits.correctable_error_reporting_enable = 1;
  iioDevCtrlReg.Bits.non_fatal_error_reporting_enable = 1;
  //iioDevCtrlReg.Bits.fatal_error_reporting_enable = 1;
  iioDevCtrlReg.Bits.unsupported_request_reporting_enable = 1;
  PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(DEVCTRL_IIO_PCIE_REG)), iioDevCtrlReg.Data);

  // ROOTCON register
  iioRootConReg.Data = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(ROOTCON_IIO_PCIE_REG)));
  iioRootConReg.Data  |= (B_PCIE_ROOT_CONTROL_NF_ERR_EN | B_PCIE_ROOT_CONTROL_COR_ERR_EN); 
  PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(ROOTCON_IIO_PCIE_REG)), iioRootConReg.Data);

  // XPUNCEDMASK register
  iioXPUncErrDMskReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(XPUNCEDMASK_IIO_PCIE_REG)));
  iioXPUncErrDMskReg.Bits.sent_completion_with_completer_abort_detect_mask = 0x01;
  iioXPUncErrDMskReg.Bits.sent_completion_with_unsupported_request_detect_mask = 0x01;
  PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(XPUNCEDMASK_IIO_PCIE_REG)), iioXPUncErrDMskReg.Data);

  //iioXPUncErrMskReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(XPUNCERRMSK_0_1_0_CFG_REG)));
  //iioXPUncErrMskReg.Bits.sent_completion_with_completer_abort_mask = 01;
  //iioXPUncErrMskReg.Bits.sent_completion_with_unsupported_request_mask = 01;
  //PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(XPUNCERRMSK_0_1_0_CFG_REG)), iioXPUncErrMskReg.Data);

  // UNCEDMASK register
  iioUncedmaskReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(UNCEDMASK_IIO_PCIE_REG)));
  iioUncedmaskReg.Bits.completer_abort_detect_mask = 00;
  iioUncedmaskReg.Bits.received_an_unsupported_request_detect_mask = 00;
  PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(UNCEDMASK_IIO_PCIE_REG)), iioUncedmaskReg.Data);

  // COREDMASK register
  iioCoredmaskReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(COREDMASK_IIO_PCIE_REG)));
  iioCoredmaskReg.Bits.advisory_non_fatal_error_detect_mask = 00;
  PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(COREDMASK_IIO_PCIE_REG)), iioCoredmaskReg.Data);

  // UNCERRMSK register
  iioUncemaskReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(UNCERRMSK_IIO_PCIE_REG)));
  iioUncemaskReg.Bits.completer_abort_mask = 00;
  iioUncemaskReg.Bits.unsupported_request_error_mask = 00;
  PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(UNCERRMSK_IIO_PCIE_REG)), iioUncemaskReg.Data);

  iioUncerrSevReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(UNCERRSEV_IIO_PCIE_REG)));
  iioUncerrSevReg.Bits.unsupported_request_error_severity = 00;
  PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(UNCERRSEV_IIO_PCIE_REG)), iioUncerrSevReg.Data);

  // CORERRMSK register
  iioAerCorErrMskReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(CORERRMSK_IIO_PCIE_REG)));
  iioAerCorErrMskReg.Bits.advisory_non_fatal_error_mask = 0;
  PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(CORERRMSK_IIO_PCIE_REG)), iioAerCorErrMskReg.Data);

  for ( RootBridgeLoop =0; RootBridgeLoop < mNumIoh; RootBridgeLoop++) {
    if (mIioUds->PlatformData.IIO_resource[RootBridgeLoop].Valid != 1) continue;
     
    // PCIERRSV register
    iioPCIErrSvReg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridgeLoop].SocketId, 0, PCIERRSV_IIO_RAS_REG);
    iioPCIErrSvReg.Bits.pciecorerr_map = 0;
    iioPCIErrSvReg.Bits.pcienonfaterr_map = 1;
    iioPCIErrSvReg.Bits.pciefaterr_map = 2;
    mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridgeLoop].SocketId, 0, PCIERRSV_IIO_RAS_REG, iioPCIErrSvReg.Data);

    // GSYSCTL register
    iioGSysCtlReg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridgeLoop].SocketId, 0, GSYSCTL_IIO_RAS_REG);
    iioGSysCtlReg.Bits.sev0_en = 1;
    iioGSysCtlReg.Bits.sev1_en = 1;
    iioGSysCtlReg.Bits.sev2_en = 1;
    mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridgeLoop].SocketId, 0, GSYSCTL_IIO_RAS_REG, iioGSysCtlReg.Data);
    
    // GERRCTL register
    iioGErrCtlReg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridgeLoop].SocketId, 0, GERRCTL_IIO_RAS_REG);
//    iioGErrCtlReg.Bits.pcie_err_msk0 = 0x00;
    iioGErrCtlReg.Data &= 0xFFFF001F;
    mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridgeLoop].SocketId, 0, GERRCTL_IIO_RAS_REG, iioGErrCtlReg.Data);
  
    // SYSMAP register
    iioSysMapReg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridgeLoop].SocketId, 0, SYSMAP_IIO_RAS_REG);
    iioSysMapReg.Bits.sev0_map = V_SYSMAP_SMI_PMI;
    iioSysMapReg.Bits.sev1_map = V_SYSMAP_SMI_PMI;  
    iioSysMapReg.Bits.sev2_map = V_SYSMAP_SMI_PMI;
    mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridgeLoop].SocketId, 0, SYSMAP_IIO_RAS_REG, iioSysMapReg.Data);
    iioCipIntrCtrlReg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridgeLoop].SocketId, 0, CIPINTRC_IIO_VTD_REG);
    iioCipIntrCtrlReg.Bits.smi_mask = 00;
    mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridgeLoop].SocketId, 0, CIPINTRC_IIO_VTD_REG, iioCipIntrCtrlReg.Data);
  }

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if ((mELogHost.socketPresentBitMap & (1 << socket)) == 0) continue;
/*
    uboxSMICtrlReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(SMICTRL_UBOX_MISC_REG)));
    uboxSMICtrlReg.Bits.umcsmien = 1;
    uboxSMICtrlReg.Bits.smidis = 0;
    PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(SMICTRL_UBOX_MISC_REG)), uboxSMICtrlReg.Data); 
*/
  }
}

EFI_STATUS
MaskIIOErrorReporting(
   VOID
   )
{
    UINT8   RootBridgeLoop;
    GNERRMASK_IIO_RAS_STRUCT GnerrMask;
    GFERRMASK_IIO_RAS_STRUCT GferrMask;
    GCERRMASK_IIO_RAS_STRUCT GcerrMask;
    GERRCTL_IIO_RAS_STRUCT   GerrCtlMask; 
    GnerrMask.Data = 0x3F0FFEF;
    GferrMask.Data = 0x3F0FFEF;
    GcerrMask.Data = 0x7F0FFEF;
    GerrCtlMask.Data = 0x7F0FFEF;
    for ( RootBridgeLoop =0; RootBridgeLoop < mNumIoh; RootBridgeLoop++) {
       if(mIohInfo[RootBridgeLoop].Valid != 1) continue;
       mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridgeLoop].SocketId, 0, GNERRMASK_IIO_RAS_REG, GnerrMask.Data);
       mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridgeLoop].SocketId, 0, GFERRMASK_IIO_RAS_REG, GferrMask.Data);
       mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridgeLoop].SocketId, 0, GCERRMASK_IIO_RAS_REG, GcerrMask.Data);        
       mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridgeLoop].SocketId, 0, GERRCTL_IIO_RAS_REG, GerrCtlMask.Data); 
    }    
    return EFI_SUCCESS;
}

// AptioV server override start
EFI_STATUS
UnMaskIIOErrorReporting(
   VOID
   )
{
    UINT8   RootBridgeLoop;
    GNERRMASK_IIO_RAS_STRUCT GnerrMask;
    GFERRMASK_IIO_RAS_STRUCT GferrMask;
    GCERRMASK_IIO_RAS_STRUCT GcerrMask;
    GERRCTL_IIO_RAS_STRUCT   GerrCtlMask; 
    GnerrMask.Data = 0;  
    GferrMask.Data = 0;
    GcerrMask.Data = 0;
    GerrCtlMask.Data = 0;
    for ( RootBridgeLoop =0; RootBridgeLoop < mNumIoh; RootBridgeLoop++) {
       if(mIohInfo[RootBridgeLoop].Valid != 1) continue;
       mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridgeLoop].SocketId, 0, GNERRMASK_IIO_RAS_REG, GnerrMask.Data);
       mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridgeLoop].SocketId, 0, GFERRMASK_IIO_RAS_REG, GferrMask.Data);
       mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridgeLoop].SocketId, 0, GCERRMASK_IIO_RAS_REG, GcerrMask.Data);        
       mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridgeLoop].SocketId, 0, GERRCTL_IIO_RAS_REG, GerrCtlMask.Data); 
    }    
    return EFI_SUCCESS;
}
// AptioV server override end

EFI_STATUS
IohPcieMaskErrors(
  VOID
  )
{
  UINT8       Port;
  UINT8       Device;
  UINT8       Function;
  UINT8       RootBridgeLoop;

  UNCEDMASK_IIO_PCIE_BDX_STRUCT      iioUnCeErrDMskReg;
  COREDMASK_IIO_PCIE_STRUCT          iioCorErrDMskReg;
  for ( RootBridgeLoop =0; RootBridgeLoop < mNumIoh; RootBridgeLoop++) {
     if(mIohInfo[RootBridgeLoop].Valid != 1) continue;
     for(Port = 0; Port < NUMBER_PORTS_PER_SOCKET; Port++) {
        Device = mIohInfo[RootBridgeLoop].Port[Port].Device;
        Function = mIohInfo[RootBridgeLoop].Port[Port].Function;
        if (!PciErrLibIsDevicePresent (mIohInfo[RootBridgeLoop].BusNum, Device, Function))
            continue;
        if(Device ==0)  
            continue;
        iioUnCeErrDMskReg.Data = 0x3fF030;
        PciExpressWrite32 (PCI_PCIE_ADDR(mIohInfo[RootBridgeLoop].BusNum, Device, Function, ONLY_REGISTER_OFFSET(UNCEDMASK_IIO_PCIE_REG)), iioUnCeErrDMskReg.Data);
        iioCorErrDMskReg.Data = 0x31C1;
        PciExpressWrite32 (PCI_PCIE_ADDR(mIohInfo[RootBridgeLoop].BusNum, Device, Function, ONLY_REGISTER_OFFSET(COREDMASK_IIO_PCIE_REG)), iioCorErrDMskReg.Data);
     }
  }  
  return EFI_SUCCESS;
}

// Aptio V Server Override - Since there is an outerloop from EnableElogIoHub() we can pass the rootbridgeloop to the IohPcieErrorEnable() and remove the for loop inside the IohPcieErrorEnable().
EFI_STATUS
IohPcieErrorEnable (
 UINT8  RootBridgeLoop/*AptioV server Override VOID*/
  )
{
  UINT8       Segment;
  UINT8       Port;
  UINT8       Device;
  UINT8       Function;
  UINT8       SecondaryBusNumber;
//  UINT8       RootBridgeLoop; Aptio V Server Override - Commenting the local variable
  UINT16      AerOffset;
  MISCCTRLSTS_0_IIO_PCIE_STRUCT iioMiscCtrlStsN0Reg;
  MISCCTRLSTS_1_IIO_PCIE_BDX_STRUCT iioMiscCtrlStsN1Reg;
  CORERRMSK_IIO_PCIE_STRUCT  iioAerCorErrMskReg;
  PCIERRSV_IIO_RAS_STRUCT iioPCIErrSvReg;
  GERRCTL_IIO_RAS_STRUCT   iioGErrCtlReg; //AptioV server override: To avoid SMI flood when disabling the IIO Ports that have PCIe card installed
  UINT8 NTBLink;
  BOOLEAN LinkInNtbOrTb;
 //AptioV server override: To avoid SMI flood when disabling the IIO Ports that have PCIe card installed
  iioGErrCtlReg.Data = mCpuCsrAccess->ReadCpuCsr (mIohInfo[RootBridgeLoop].SocketId, 0, GERRCTL_IIO_RAS_REG); 

  Segment       = 00;

 /*for ( RootBridgeLoop =0; RootBridgeLoop < mNumIoh; RootBridgeLoop++)*/ { // Aptio V Server Override - Since there is an outerloop from EnableElogIoHub() we can pass the rootbridgeloop to the IohPcieErrorEnable() and remove the for loop inside the IohPcieErrorEnable().
    if(mIohInfo[RootBridgeLoop].Valid != 1) return EFI_SUCCESS;/* Aptio V Server Override - Fix for the build error continue*/;
    LinkInNtbOrTb = FALSE;
    for(Port = 0; Port < NUMBER_PORTS_PER_SOCKET; Port++) {

      Device = mIohInfo[RootBridgeLoop].Port[Port].Device;
      Function = mIohInfo[RootBridgeLoop].Port[Port].Function;
  
      //
      // Check if the root port is enabled.
      //
      if (!PciErrLibIsDevicePresent (mIohInfo[RootBridgeLoop].BusNum, Device, Function)) {
        //AptioV server override: To avoid SMI flood when disabling the IIO Ports that have PCIe card installed
        iioGErrCtlReg.Data |= (1 << (Port + 5)); //Port # PCIe error mask
        continue;
      }
      if((Device == NTB_DEV) && (Function == NTB_FUNC)){
        NTBLink = PciExpressRead8(PCI_PCIE_ADDR(mIohInfo[RootBridgeLoop].BusNum, Device, Function,R_NTB_PPD));
        if( (NTBLink & B_NTB_PORT_DEF) != 0) {
          LinkInNtbOrTb = TRUE;
        IioPcieErrorEnableOnNTB(mIohInfo[RootBridgeLoop].BusNum, Device, Function);
       }
      }
      if(Device ==0)  {  
        //
        // Enable SERR and PERR
        //
        PciErrLibEnablePciSerrPerrGeneration (mIohInfo[RootBridgeLoop].BusNum, Device, Function);
        PciErrLibEnableElogDevice(mIohInfo[RootBridgeLoop].BusNum, Device, Function);
      }

      //
      // Do not enable error logging if there are no devices behind the bridge.
      //
      SecondaryBusNumber = PciExpressRead8 (PCI_PCIE_ADDR(mIohInfo[RootBridgeLoop].BusNum, Device, Function, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));
      //if ((SecondaryBusNumber == 0) || !(PciErrLibDeviceOnBus (SecondaryBusNumber))) {
        //continue;
      //}
  
      if(Device !=0)  {
        //
        // Disable MSI generation on RAS errors from PCI-E devices
        //
        iioMiscCtrlStsN0Reg.Data = PciExpressRead32 (PCI_PCIE_ADDR(mIohInfo[RootBridgeLoop].BusNum, Device, Function, ONLY_REGISTER_OFFSET(MISCCTRLSTS_0_IIO_PCIE_REG)));
        iioMiscCtrlStsN0Reg.Bits.enable_system_error_only_for_aer = 1;
        PciExpressWrite32 (PCI_PCIE_ADDR(mIohInfo[RootBridgeLoop].BusNum, Device, Function, ONLY_REGISTER_OFFSET(MISCCTRLSTS_0_IIO_PCIE_REG)), iioMiscCtrlStsN0Reg.Data);
        if(LinkInNtbOrTb != TRUE) {
        //
        // Enable error reporting in the root port and all subordinate devices
        //
        PciErrLibEnableElogDeviceAndSubDevices (mIohInfo[RootBridgeLoop].BusNum, Device, Function);
        }
      }
      iioMiscCtrlStsN1Reg.Data = PciExpressRead32 (PCI_PCIE_ADDR(mIohInfo[RootBridgeLoop].BusNum, Device, Function, ONLY_REGISTER_OFFSET(MISCCTRLSTS_1_IIO_PCIE_REG)));
      iioMiscCtrlStsN1Reg.Bits.override_system_error_on_pcie_correctable_error_enable =1;
      iioMiscCtrlStsN1Reg.Bits.override_system_error_on_pcie_non_fatal_error_enable =1;
      iioMiscCtrlStsN1Reg.Bits.override_system_error_on_pcie_fatal_error_enable =1;
	  //
	  // BDX-DE HSD 5003287-RAS driver overriding the legacy settings of the NTB port
	  //
      //iioMiscCtrlStsN1Reg.Bits.problematic_port_for_lock_flows = 0;
      PciExpressWrite32 (PCI_PCIE_ADDR(mIohInfo[RootBridgeLoop].BusNum, Device, Function, ONLY_REGISTER_OFFSET(MISCCTRLSTS_1_IIO_PCIE_REG)), iioMiscCtrlStsN1Reg.Data);

      //Rootport specific AER setting goes here
      AerOffset = PciErrLibGetExtendedCapabilitiesOffset(mIohInfo[RootBridgeLoop].BusNum, Device, Function,PCIE_EXT_CAP_HEADER_AERC,0xFF);
       if (AerOffset != 0) {
          //
          // Program AER mask registers for uncorrectable and correctable errors,if any
          //
        iioAerCorErrMskReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(mIohInfo[RootBridgeLoop].BusNum, Device, Function, ONLY_REGISTER_OFFSET(CORERRMSK_IIO_PCIE_REG)));
        iioAerCorErrMskReg.Bits.advisory_non_fatal_error_mask = 0;
        PciExpressWrite32 (PCI_PCIE_ADDR(mIohInfo[RootBridgeLoop].BusNum, Device, Function, ONLY_REGISTER_OFFSET(CORERRMSK_IIO_PCIE_REG)), iioAerCorErrMskReg.Data);
        //
        // Program AER severity registers for uncorrectable,if any
        //

        //
        // Clear AER status registers including uncorrectable and correctable errors...
        //
        ClearIohExtPcieDeviceStatus (mIohInfo[RootBridgeLoop].BusNum, Device, Function);
       }
      
      

      //
      // Clear the status registers
      //
      PciErrLibClearPcieDeviceStatus (mIohInfo[RootBridgeLoop].BusNum, Device, Function);
  
    }

    //AptioV server override: To avoid SMI flood when disabling the IIO Ports that have PCIe card installed
	mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridgeLoop].SocketId, 0, GERRCTL_IIO_RAS_REG, iioGErrCtlReg.Data); //felixdbg-1125+

    //
    // Error Severity register: Correctable - severity 0, Nonfatal - Severity 1, Fatal - severity 2
    //
    iioPCIErrSvReg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridgeLoop].SocketId, 0, PCIERRSV_IIO_RAS_REG);
   
    iioPCIErrSvReg.Bits.pciecorerr_map = 0;
    iioPCIErrSvReg.Bits.pcienonfaterr_map = 1;
    iioPCIErrSvReg.Bits.pciefaterr_map = 2;

    mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridgeLoop].SocketId, 0, PCIERRSV_IIO_RAS_REG, iioPCIErrSvReg.Data); 

    
  }

  return EFI_SUCCESS;

}

EFI_STATUS
IioDmiErrorEnable (
  VOID)
{
  UINT8 Socket=0;
  UINT8 Port=0;
  UINT8 Device,Function;
  UINT16  StatusReg16,AerRegisterOffset;
  UINT8 CapabilitiesOffset;
  UINT16 PcieCapabilities;
  BOOLEAN RootPort;
  UINT32 RootControl;
  UINT32 DeviceControl;

  Device = mIohInfo[Socket].Port[Port].Device;
  Function = mIohInfo[Socket].Port[Port].Function;

  if (!PciErrLibIsDevicePresent (mIohInfo[Socket].BusNum, Device, Function)) {
    return EFI_SUCCESS;
  }
 
  //clear legacy pcie device status
  StatusReg16 = PciExpressRead16 (PCI_PCIE_ADDR(mIohInfo[Socket].BusNum, Device, Function, PCI_PRIMARY_STATUS_OFFSET));
  PciExpressWrite16 (PCI_PCIE_ADDR(mIohInfo[Socket].BusNum, Device, Function, PCI_PRIMARY_STATUS_OFFSET), StatusReg16);

  CapabilitiesOffset = PciErrLibGetCapabilitiesOffset (mIohInfo[Socket].BusNum, Device, Function);
  if (CapabilitiesOffset == 0) {
    return EFI_SUCCESS;
  }

  PcieCapabilities = PciExpressRead16 (PCI_PCIE_ADDR(mIohInfo[Socket].BusNum, Device, Function, CapabilitiesOffset + PCIE_CAPABILITY_OFFSET));
  if ((PcieCapabilities & 0xF0) == V_PCIE_CAPABILITY_DPT_ROOT_PORT) {
    RootPort = TRUE;
  } else {
    RootPort = FALSE;
  }
  //
  // Clear the PCIe device status register
  //
  StatusReg16 = PciExpressRead16 (PCI_PCIE_ADDR(mIohInfo[Socket].BusNum, Device, Function, CapabilitiesOffset + R_DMI_DEVICE_STATUS_OFFSET));
  PciExpressWrite16 (PCI_PCIE_ADDR(mIohInfo[Socket].BusNum, Device, Function, CapabilitiesOffset + R_DMI_DEVICE_STATUS_OFFSET), StatusReg16);

  //
  // Check for presence of AER 
  //
  AerRegisterOffset = PciErrLibGetExtendedCapabilitiesOffset(mIohInfo[Socket].BusNum, Device, Function,V_DMI_AER_CAPID,V_DMI_AER_VSEC_CAPID);
  if (AerRegisterOffset != 0) {
  //
  // Program AER mask registers for uncorrectable and correctable errors,if any
  //
  
  //
  // Program AER severity registers for uncorrectable,if any
  //

 
  //
  // Clear AER Status registers
  //
  ClearIohExtPcieDeviceStatus (mIohInfo[Socket].BusNum, Device, Function);
  }

  //PCIE basic error handling enable...
 
  //
  // Configure Root Control Register ..
  //
  if (RootPort) {
    RootControl = PciExpressRead16 (PCI_PCIE_ADDR(mIohInfo[Socket].BusNum, Device, Function, CapabilitiesOffset + PCIE_ROOT_CONTROL_OFFSET));
    RootControl |= (B_PCIE_ROOT_CONTROL_COR_ERR_EN | B_PCIE_ROOT_CONTROL_NF_ERR_EN | B_PCIE_ROOT_CONTROL_FAT_ERR_EN);
    PciExpressWrite16 (PCI_PCIE_ADDR(mIohInfo[Socket].BusNum, Device, Function, CapabilitiesOffset + PCIE_ROOT_CONTROL_OFFSET), RootControl);
  }

  //
  // Enable Error logging in Device Control...
  //
  DeviceControl = PciExpressRead16 (PCI_PCIE_ADDR(mIohInfo[Socket].BusNum, Device, Function, CapabilitiesOffset + R_DMI_DEVICE_CONTROL_OFFSET));
  DeviceControl |= (B_PCIE_DEVICE_CONTROL_COR_ERR_EN| B_PCIE_DEVICE_CONTROL_NF_ERR_EN |B_PCIE_DEVICE_CONTROL_FAT_ERR_EN);
  PciExpressWrite16 (PCI_PCIE_ADDR(mIohInfo[Socket].BusNum, Device, Function, CapabilitiesOffset + R_DMI_DEVICE_CONTROL_OFFSET), DeviceControl);

  
return EFI_SUCCESS;

}


VOID
InitErrDevInfo( )
{

  UINT8   i =0;

  for(i=0; i < MAX_NUM_ERR; i++) {
    mErrDevIohInfo[i].BusNum = 00;
    mErrDevIohInfo[i].Device = 00;
    mErrDevIohInfo[i].Function = 00;
    mErrDevIohInfo[i].NmiSciFlag = 00;
    mErrDevIohInfo[i].UrMaskSet = 00;
    mErrDevIohInfo[i].HdrLog1 = 00;
  }      
}

VOID
InitIOHInfo(VOID)
{

	UINT8 RootBridgeLoop;

	mNumIoh = MAX_SOCKET;
	
	for ( RootBridgeLoop =0; RootBridgeLoop < mNumIoh; RootBridgeLoop++) {
	  if (mIioUds->PlatformData.IIO_resource[RootBridgeLoop].Valid == 1) {
		mIohInfo[RootBridgeLoop].Valid = 1;
		mIohInfo[RootBridgeLoop].SocketId = mIioUds->PlatformData.IIO_resource[RootBridgeLoop].SocketID;
		mIohInfo[RootBridgeLoop].BusNum  = mIioUds->PlatformData.IIO_resource[RootBridgeLoop].BusBase;
		mIohInfo[RootBridgeLoop].Core20BusNum = mIioUds->PlatformData.IIO_resource[RootBridgeLoop].BusBase;
		mIohInfo[RootBridgeLoop].Port[0].Device = PCIE_PORT_0_DEV;
		mIohInfo[RootBridgeLoop].Port[0].Function = PCIE_PORT_0_FUNC;
		mIohInfo[RootBridgeLoop].Port[1].Device = PCIE_PORT_1A_DEV;
		mIohInfo[RootBridgeLoop].Port[1].Function = PCIE_PORT_1A_FUNC;
		mIohInfo[RootBridgeLoop].Port[2].Device = PCIE_PORT_1B_DEV;
		mIohInfo[RootBridgeLoop].Port[2].Function = PCIE_PORT_1B_FUNC;
		mIohInfo[RootBridgeLoop].Port[3].Device = PCIE_PORT_2A_DEV;
		mIohInfo[RootBridgeLoop].Port[3].Function = PCIE_PORT_2A_FUNC;
		mIohInfo[RootBridgeLoop].Port[4].Device = PCIE_PORT_2B_DEV;
		mIohInfo[RootBridgeLoop].Port[4].Function = PCIE_PORT_2B_FUNC;
		mIohInfo[RootBridgeLoop].Port[5].Device = PCIE_PORT_2C_DEV;
		mIohInfo[RootBridgeLoop].Port[5].Function = PCIE_PORT_2C_FUNC;
		mIohInfo[RootBridgeLoop].Port[6].Device = PCIE_PORT_2D_DEV;
		mIohInfo[RootBridgeLoop].Port[6].Function = PCIE_PORT_2D_FUNC;
		mIohInfo[RootBridgeLoop].Port[7].Device = PCIE_PORT_3A_DEV;
		mIohInfo[RootBridgeLoop].Port[7].Function = PCIE_PORT_3A_FUNC;
		mIohInfo[RootBridgeLoop].Port[8].Device = PCIE_PORT_3B_DEV;
		mIohInfo[RootBridgeLoop].Port[8].Function = PCIE_PORT_3B_FUNC;
		mIohInfo[RootBridgeLoop].Port[9].Device = PCIE_PORT_3C_DEV;
		mIohInfo[RootBridgeLoop].Port[9].Function = PCIE_PORT_3C_FUNC;
		mIohInfo[RootBridgeLoop].Port[10].Device = PCIE_PORT_3D_DEV;
		mIohInfo[RootBridgeLoop].Port[10].Function = PCIE_PORT_3D_FUNC;
	  } else {
		mIohInfo[RootBridgeLoop].Valid = 0;
	  }
	}
}


VOID
ClearTheURMask(UINT8 Bus, UINT8 Dev, UINT8 Func  )
{
  UNCERRMSK_IIO_PCIE_BDX_STRUCT    iioUncemaskReg;
  CORERRMSK_IIO_PCIE_STRUCT  iioAerCorErrMskReg; //0.1.0
  UINT8   i =0;

  // CORERRMSK register
  iioAerCorErrMskReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Dev, Func, ONLY_REGISTER_OFFSET(CORERRMSK_IIO_PCIE_REG)));
  iioAerCorErrMskReg.Bits.advisory_non_fatal_error_mask = 0;
  PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Dev, Func, ONLY_REGISTER_OFFSET(CORERRMSK_IIO_PCIE_REG)), iioAerCorErrMskReg.Data);

  // UNCERRMSK register
  iioUncemaskReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Dev, Func, ONLY_REGISTER_OFFSET(UNCERRMSK_IIO_PCIE_REG)));
  //iioUncemaskReg.Bits.completer_abort_mask = 00;
  iioUncemaskReg.Bits.unsupported_request_error_mask = 00;
  PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Dev, Func, ONLY_REGISTER_OFFSET(UNCERRMSK_IIO_PCIE_REG)), iioUncemaskReg.Data);

  for(i=0; i < MAX_NUM_ERR; i++) {
    if( (mErrDevIohInfo[i].BusNum == Bus) && (mErrDevIohInfo[i].Device == Dev) && (mErrDevIohInfo[i].Function == Func) )  {
      mErrDevIohInfo[i].UrMaskSet = 0;
    }
  }      

}

BOOLEAN
CheckForDeviceFound(
UINT8 Bus, UINT8 Device, UINT8 Function, UINT8 *DevNumToAdd 
)
{
  UINT8     i =0;
  BOOLEAN   FoundDev;

  FoundDev  = FALSE;
  for(i=0; i < MAX_NUM_ERR; i++) {
    if( (mErrDevIohInfo[i].BusNum == Bus) && (mErrDevIohInfo[i].Device == Device) && (mErrDevIohInfo[i].Function == Function) )  {
       FoundDev = TRUE; 
       *DevNumToAdd  = i;
       return FoundDev; 
    }
  }

  *DevNumToAdd  = mNextDevNum;
  return FoundDev; 

}

STATIC
EFI_STATUS
IohPcieErrorHandler (
 UINT8       Ioh
  )
/*++

Routine Description:

    Handle errors on TB's PEX links

Arguments:

Returns:

    EFI_SUCCESS if call is succeed

--*/
{

  UINT8       Bus;
  UINT8       Device;
  UINT8       Function;
  UINT8       Port;
  CORERRMSK_IIO_PCIE_STRUCT    iioAerCorErrMskReg; //0.1.0
  XPCORERRSTS_IIO_PCIE_BDX_STRUCT  xpCorrErrsts;
  UNCERRMSK_IIO_PCIE_BDX_STRUCT    iioUncemaskReg;
  UNCERRSTS_IIO_PCIE_BDX_STRUCT    iioUncestsReg;
  LNKSTS_IIO_PCIE_STRUCT       LinkSts;
  ERRINJCON_IIO_PCIE_STRUCT    ErrInjCon;
  ERRINJCAP_IIO_PCIE_STRUCT    ErrInjCap;
  BOOLEAN     SCIGenerate;
  BOOLEAN     NMIGenerate;
  UINT8       NextDevNum;
  BOOLEAN DevFound = FALSE;
  UINT8 NTBLink;
  SCIGenerate = FALSE;
  NMIGenerate = FALSE;
  Bus = mIohInfo[Ioh].BusNum;
  

  if(mWheaPlatformSupport != NULL && mErrLogSetupData->WheaLogging == 1)
    mWheaPlatformSupport->WheaPlatformRestartAllElog (mWheaPlatformSupport);

  mPciXPUnsupportedRequest = 00; 

  for(Port = 0; Port < NUMBER_PORTS_PER_SOCKET; Port++) {
  
    Device = mIohInfo[Ioh].Port[Port].Device;
    Function = mIohInfo[Ioh].Port[Port].Function;

    if ((Device == NTB_DEV) && (Function == NTB_FUNC)) {
      NTBLink = PciExpressRead8(PCI_PCIE_ADDR(Bus, Device, Function,R_NTB_PPD));
      if((NTBLink & B_NTB_PORT_DEF) != 0) {
      IohPcieNtbErrorHandler(Bus, Device, Function);
      continue;
     }
    }
    //
    // Check if error reporting is enabled for this device
    //
    if (!PciErrLibIsRootPortErrReportingEnabled (Bus, Device, Function) && !IioPcieErrorReportingEnabled(Bus,Device,Function)) {
      continue;
    }

    iioUncemaskReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(UNCERRMSK_IIO_PCIE_REG)));
    iioUncestsReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(UNCERRSTS_IIO_PCIE_REG)));
    iioAerCorErrMskReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(CORERRMSK_IIO_PCIE_REG)));
    xpCorrErrsts.Data = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(XPCORERRSTS_IIO_PCIE_REG)));

    // SR-IOV request
     
    if(iioUncestsReg.Bits.received_an_unsupported_request) {
      DevFound = CheckForDeviceFound(Bus, Device, Function, &NextDevNum);
      // if(mNextDevNum > MAX_NUM_ERR) return;
      if(DevFound == FALSE)  {
        mErrDevIohInfo[NextDevNum].BusNum = Bus;
        mErrDevIohInfo[NextDevNum].Device = Device;
        mErrDevIohInfo[NextDevNum].Function = Function;
        mNextDevNum++;
      }

      if( mErrDevIohInfo[NextDevNum].NmiSciFlag == V_SCI) {

        mErrDevIohInfo[NextDevNum].UrMaskSet = 1;  
        //if((iioUncemaskReg.Bits.unsupported_request_error_mask == 01) || (iioUncemaskReg.Bits.completer_abort_mask == 01))  continue;
        if(iioUncemaskReg.Bits.unsupported_request_error_mask == 01)  continue;
  
        iioAerCorErrMskReg.Bits.advisory_non_fatal_error_mask = 1;
        PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(CORERRMSK_IIO_PCIE_REG)), iioAerCorErrMskReg.Data);

        //iioUncemaskReg.Bits.completer_abort_mask = 01;
        iioUncemaskReg.Bits.unsupported_request_error_mask = 01;
        PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(UNCERRMSK_IIO_PCIE_REG)), iioUncemaskReg.Data);

        PciErrLibClearPcieDeviceStatus(Bus, Device, Function);

        mErrDevIohInfo[NextDevNum].HdrLog1        = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(HDRLOG1_IIO_PCIE_REG)));
        mErrDevIohInfo[NextDevNum].HdrLog1        &= 0xffff0000;

        mPciXPUnsupportedRequest = 01;
        SCIGenerate = TRUE;
        PciErrLibReportError (Bus, Device, Function, COR_ERROR_TYPE);
      }
      else  {
        NMIGenerate = TRUE;
        PciErrLibReportError (Bus, Device, Function, FATAL_ERROR_TYPE);
      }
    } 

    if(xpCorrErrsts.Data & 01) {
      LinkSts.Data = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIE_REG)));
      PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIE_REG)), LinkSts.Data); 
    }

    ErrInjCap.Data = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(ERRINJCAP_IIO_PCIE_REG)));
    if( (ErrInjCap.Bits.extcapid  & 0xffff) & 0xB) {   //  Verify the capability of the injection device
      ErrInjCon.Data = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(ERRINJCON_IIO_PCIE_REG)));
      ErrInjCon.Bits.cause_rcverr = 0;
      ErrInjCon.Bits.cause_ctoerr = 0;
      PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(ERRINJCON_IIO_PCIE_REG)), ErrInjCon.Data);
    }

    if( PciErrLibIsErrOnPcieRootPort (Bus, Device, Function)) {
      DEBUG ((DEBUG_ERROR, "IohPcieErrorHandler Eror in root port:  Bus = %x, Dev = %x, Func = %x, \n", Bus , Device, Function));
        if( mPciXPUnsupportedRequest != 1) PciErrLibPcieRootPortErrorHandler (Bus, Device, Function);
      }
    //
    // Clear Extended error status registers
    //
//    if( !(mPcieCorErrDetected)) { //AptioV Server Override: Clear Extended Error Status Register after handling of errors.
      ClearIohExtPcieDeviceStatus (Bus, Device, Function);
//    } //AptioV Server Override: Clear Extended Error Status Register after handling of errors.
    
    //
    // NMI if a fatal error occurred.  
    // We treat PCI parity errors and PCIe Fatal errors since
    // data corruption may have occurred.
    //
    if (((mPciSerrDetected && mNmiOnSerr) ||(mPciPerrDetected && mNmiOnPerr) ||(NMIGenerate == TRUE) || mPcieFatalErrDetected)) {
        if (mPciXPUnsupportedRequest != 1) ElogGenerateNmiNow (); //For fatal errors,trigger NMI.
      break;
    }else if (mPcieNonFatalErrDetected){
         if((mWheaPlatformSupport != NULL) || (mErrLogSetupData->WheaLogging)){
            if (mPciXPUnsupportedRequest != 1) ElogGenerateNmiNow ();//For non-fatal errors,if whea =1,trigger NMI.
         } else {
           ClearIohExtPcieDeviceStatus (Bus, Device, Function);
           ClearGlobalErrorRegisterStatus(mIohInfo[Ioh].SocketId);
           mPcieNonFatalErrDetected = FALSE;
         }
    }
  }

  if ((SCIGenerate == TRUE || mPcieCorErrDetected ) && (mWheaPlatformSupport != NULL) && mErrLogSetupData->WheaLogging == 1)  {
    mPcieCorErrDetected = FALSE;
    mWheaPlatformSupport->WheaPlatformEndElog (mWheaPlatformSupport); 
  }

  return EFI_SUCCESS;
}

EFI_STATUS
ProcessIIOUncorrectedError (
VOID
  )
/*++

Routine Description:
    This function performs the IOH error checking and processing functions

Arguments:
    DispatchHandle        - The handle of this callback 
    CommunicationBuffer   - Pointer to the buffer that contains the communication Message
    SourceSize            - Size of the memory image to be used for handler.

Returns:
    EFI_SUCCESS

--*/
{
  EFI_STATUS  Status;

  //
  //TODO: Split IIO error handler in fatal/uncorrected and correctable flows. 

  Status = ElogIohErrorHandler ();
  return Status;
}

void
CheckForHdrLogDevAndClearSts(UINT8 Bus, UINT8 Dev, UINT8 Func  )
{
  UNCERRSTS_IIO_PCIE_BDX_STRUCT    iioUncestsReg;
  BOOLEAN ErrSts = FALSE;
  UINT8 iionum;
  UINT8 sktid;

  iioUncestsReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Dev, Func, ONLY_REGISTER_OFFSET(UNCERRSTS_IIO_PCIE_REG)));
  ErrSts = FALSE;
  if(iioUncestsReg.Bits.received_an_unsupported_request )  {
    ErrSts  = TRUE;
  }

  PciErrLibClearPcieDeviceStatus(Bus, Dev, Func);
  ClearIohExtPcieDeviceStatus(Bus, Dev, Func);
  
  for(iionum = 0; iionum < MAX_SOCKET; iionum++) {
    if(Bus == mIohInfo[iionum].BusNum) {
      sktid = mIohInfo[iionum].SocketId;
      ClearIIOErrorRegisterStatus(sktid);
      break;
    }
  }
  
}

EFI_STATUS
NmiSciUpdateHandler (
  VOID)
{
  BOOLEAN DevFound = FALSE;
  UINT8   NextDevNum, Data = 0;

  ASSERT (mAcpiParameter);

  if( (!PciErrLibIsDevicePresent (mAcpiParameter->WmaaSIBus, mAcpiParameter->WmaaSIDevice, mAcpiParameter->WmaaSIFunction)) || (mAcpiParameter->WmaaSISeg != 00) ) {
    mAcpiParameter->WmaaSISts = 01;
    return EFI_SUCCESS;
  }
  
  mAcpiParameter->WmaaSISts = 00;
  DevFound = CheckForDeviceFound(mAcpiParameter->WmaaSIBus, mAcpiParameter->WmaaSIDevice, mAcpiParameter->WmaaSIFunction, &NextDevNum);

  if(DevFound != TRUE)  {
    mErrDevIohInfo[NextDevNum].BusNum = mAcpiParameter->WmaaSIBus;
    mErrDevIohInfo[NextDevNum].Device = mAcpiParameter->WmaaSIDevice;
    mErrDevIohInfo[NextDevNum].Function = mAcpiParameter->WmaaSIFunction;
    mNextDevNum++;
  }
  
  if(mAcpiParameter->WmaaSICaseValue == 01) {
    DEBUG ((DEBUG_INFO, "WmaaSICaseValue- method -1 \n"));
    mErrDevIohInfo[NextDevNum].NmiSciFlag  = V_SCI;
    WmaaErrReportingEn(mAcpiParameter->WmaaSIBus, mAcpiParameter->WmaaSIDevice, mAcpiParameter->WmaaSIFunction);
  }else if(mAcpiParameter->WmaaSICaseValue == 02) {
    DEBUG ((DEBUG_INFO, "WmaaSICaseValue- method -2 \n"));
    mErrDevIohInfo[NextDevNum].NmiSciFlag  = V_NMI;
    CheckForHdrLogDevAndClearSts(mAcpiParameter->WmaaSIBus, mAcpiParameter->WmaaSIDevice, mAcpiParameter->WmaaSIFunction);
    WmaaErrReportingEn(mAcpiParameter->WmaaSIBus, mAcpiParameter->WmaaSIDevice, mAcpiParameter->WmaaSIFunction);
    ClearTheURMask(mAcpiParameter->WmaaSIBus, mAcpiParameter->WmaaSIDevice, mAcpiParameter->WmaaSIFunction);
  }else if(mAcpiParameter->WmaaSICaseValue == 03) {
    DEBUG ((DEBUG_INFO, "WmaaSICaseValue- method -3 \n"));  
    if( mErrDevIohInfo[NextDevNum].NmiSciFlag  == V_NMI)   {
      mAcpiParameter->WmaaSISts = 02;
      return EFI_SUCCESS;
    }
    Data = IoRead8 (0x86);  
    if(Data ==  0xef )    EFI_DEADLOOP();

    CheckForHdrLogDevAndClearSts(mAcpiParameter->WmaaSIBus, mAcpiParameter->WmaaSIDevice, mAcpiParameter->WmaaSIFunction);
    ClearTheURMask(mAcpiParameter->WmaaSIBus, mAcpiParameter->WmaaSIDevice, mAcpiParameter->WmaaSIFunction);
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EnablePoisonIoh (
  IN UINT8 ioh
  )
/*++

Routine Description:


Arguments:

    N/A

Returns:

    EFI_SUCCESS if the call is succeed

--*/
{
  UINT8       port;
  UINT8       device;
  UINT8       function;
  UNCEDMASK_IIO_PCIE_BDX_STRUCT iioUncedmaskReg;
  IIOMISCCTRL_N1_IIO_VTD_HSX_BDX_STRUCT  iioMiscCtrlN1Reg;

  for (port = 0; port < NUMBER_PORTS_PER_SOCKET; port++) {
    device = mIohInfo[ioh].Port[port].Device;
    function = mIohInfo[ioh].Port[port].Function;

    //
    // Check if the root port is enabled.
    //
    if (!PciErrLibIsDevicePresent (mIohInfo[ioh].BusNum, device, function)) {
      continue;
    }

    //
    // Set UNCEDMASK for port
    //

    iioUncedmaskReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(mIohInfo[ioh].BusNum, 
                                             device, 
                                             function, 
                                             ONLY_REGISTER_OFFSET(UNCEDMASK_IIO_PCIE_REG)));

    iioUncedmaskReg.Bits.poisoned_tlp_detect_mask = 0;

    PciExpressWrite32 (PCI_PCIE_ADDR(mIohInfo[ioh].BusNum, 
                       device, 
                       function, 
                       ONLY_REGISTER_OFFSET(UNCEDMASK_IIO_PCIE_REG)), 
                       iioUncedmaskReg.Data);

  }

  //
  // Set iio poision forward enable
  //
  iioMiscCtrlN1Reg.Data =  mCpuCsrAccess->ReadCpuCsr (ioh, 0, IIOMISCCTRL_N1_IIO_VTD_REG);

  iioMiscCtrlN1Reg.Bits.poisfen = 1;

  mCpuCsrAccess->WriteCpuCsr (ioh, 0, IIOMISCCTRL_N1_IIO_VTD_REG, iioMiscCtrlN1Reg.Data);  
  return EFI_SUCCESS;
}

//Need to wrap in an EX flag. Not supported in EP
/*EFI_STATUS
IIOEnableIoMca (
IN VOID *Buffer
)
{
  UINT64 data;
  UINT32 MCiCtlAddr;

  if (mIioUds->SystemStatus.cpuType == (CPU_IVT || CPU_HSX || CPU_BDX)) {
    
#ifdef IVT_HOST
    MCiCtlAddr = MSR_IVT_IIO_MC12_CTL;
#else
	  MCiCtlAddr = MSR_HSX_IIO_MC12_CTL;
#endif

    data = AsmReadMsr64(MCiCtlAddr); //MC12_CTL (IIO)
    data |= (BIT0 | BIT1); // Setting bit 0 & 1 Enable IIO Fatal MCA and Non-Fatal MCA respectively 
    AsmWriteMsr64 (MCiCtlAddr, data);    
  }  
  return EFI_SUCCESS;  
}

//Need to wrap in an EX flag. Not supported in EP
EFI_STATUS
IIODisableIoMca (
IN VOID *Buffer
)
{
  UINT64 data;
  UINT32 MCiCtlAddr;

  if (mIioUds->SystemStatus.cpuType == (CPU_IVT || CPU_HSX || CPU_BDX)) {
    
#ifdef IVT_HOST
    MCiCtlAddr = MSR_IVT_IIO_MC12_CTL;
#else
	  MCiCtlAddr = MSR_HSX_IIO_MC12_CTL;
#endif

    data = AsmReadMsr64(MCiCtlAddr); //MC12_CTL (IIO)
    data &= ~(BIT0 | BIT1); // Clearing bit 0 & 1 Disable IIO Fatal MCA and Non-Fatal MCA respectively 
    AsmWriteMsr64 (MCiCtlAddr, data);    
  }  
  return EFI_SUCCESS;  
}

//Need to wrap in an EX flag. Not supported in EP
EFI_STATUS
IIOLerEnable(
  IN UINT8 ioh
)*/
/*++

Routine Description:
    This function will enable all the Pcie errors, that need to be reported as LER

Arguments:

    IOH number on which LER need to be enabled

Returns:

    EFI_SUCCESS if the call is succeed

--*/
/*{
    UINT8       port;
    UINT8       device;
    UINT8       function;
    LER_XPUNCERRMSK_IIO_PCIE_STRUCT iioLerXPUnCerrMask;
    LER_RPERRMSK_IIO_PCIE_STRUCT iioLerRootPortErrorMask;
    LER_CTRLSTS_IIO_PCIE_STRUCT iioLerControl;
    UINT16 CapabilitiesOffset;
    LER_UNCERRMSK_IIO_PCIE_BDX_STRUCT iioLerUnCerrMask;
#ifdef IVT_HOST
    XPDFXSPAREREG_IIO_DFX_IVT_STRUCT XpDfxSpareReg;
#else
  XPDFXSPAREREG_IIO_DFX_HSX_BDX_STRUCT    XpDfxSpareReg;
#endif
    for (port = 0; port < NUMBER_PORTS_PER_SOCKET; port++) {
        device = mIohInfo[ioh].Port[port].Device;
        function = mIohInfo[ioh].Port[port].Function;

        //
        // if DMI port, dont enable LER
		//
        if ((mIohInfo[ioh].BusNum == 0) && (device == 0) && (function == 0))
            continue;
        //
        // Verify any PciE device exist at the Bus, Device and function
        //
        if (!PciErrLibIsDevicePresent (mIohInfo[ioh].BusNum, device, function)) {
          continue;
        }
        //
        // Check if it is a root port.
        //
        if (!PCIeIsRootPort(mIohInfo[ioh].BusNum, device, function)) {
            continue;
        }
        // Get the capability offset for LER   
        CapabilitiesOffset = PciErrLibGetLERCapabilitiesOffset(mIohInfo[ioh].BusNum, device, function);
        if (CapabilitiesOffset == 0)
            continue;
        // Below code will enable the PciE errors, which need to be considered as LER
        iioLerUnCerrMask.Bits.data_link_layer_protocol_error_mask = 0;
        iioLerUnCerrMask.Bits.surprise_down_error_mask = 0;
        iioLerUnCerrMask.Bits.poisoned_tlp_mask = 0;
        iioLerUnCerrMask.Bits.flow_control_protocol_error_mask = 0;
        iioLerUnCerrMask.Bits.completion_time_out_mask = 0;
        iioLerUnCerrMask.Bits.completer_abort_mask = 0;
        iioLerUnCerrMask.Bits.unexpected_completion_mask = 0;
        iioLerUnCerrMask.Bits.malformed_tlp_mask = 0;
        iioLerUnCerrMask.Bits.unsupported_request_error_mask = 0;
        iioLerUnCerrMask.Bits.acs_violation_mask = 0;

        PciExpressWrite32 (PCI_PCIE_ADDR(mIohInfo[ioh].BusNum, device, function, (CapabilitiesOffset + 0xC)), iioLerUnCerrMask.Data);

        iioLerXPUnCerrMask.Bits.sent_completion_with_ca_mask = 0;
        iioLerXPUnCerrMask.Bits.sent_completion_with_ur_mask = 0;
        iioLerXPUnCerrMask.Bits.received_pcie_completion_with_ca_status_mask = 0;
        iioLerXPUnCerrMask.Bits.received_pcie_completion_with_ur_status_mask = 0;
        iioLerXPUnCerrMask.Bits.outbound_poisoned_data_mask = 0;
        PciExpressWrite32 (PCI_PCIE_ADDR(mIohInfo[ioh].BusNum, device, function, (CapabilitiesOffset + 0x10)), iioLerXPUnCerrMask.Data);

        iioLerRootPortErrorMask.Bits.non_fatal_error_message_received_mask = 0;
        iioLerRootPortErrorMask.Bits.fatal_error_message_received_mask = 0;
        PciExpressWrite32 (PCI_PCIE_ADDR(mIohInfo[ioh].BusNum, device, function, (CapabilitiesOffset + 0x14)), iioLerRootPortErrorMask.Data);

		if (((mELogHost.CpuFamily == CPU_IVT) && (mELogHost.CpuStepping >= B0_REV)) || (mELogHost.CpuFamily == CPU_HSX) || (mELogHost.CpuFamily == CPU_BDX))   {
			XpDfxSpareReg.Bits.xpdfxsparereg_en_x16_ler_check = 1;
			XpDfxSpareReg.Bits.xpdfxsparereg_en_x16_ler_dump = 1;
			XpDfxSpareReg.Bits.xpdfxsparereg_en_convert_invcmpl_urcmpl = 1;
		}

        iioLerControl.Bits.ler_ss_severity_en = 0;
        iioLerControl.Bits.ler_ss_inten = 0;
        iioLerControl.Bits.ler_ss_drop_txn = 1;
        iioLerControl.Bits.ler_ss_enable = 1;
        PciExpressWrite32 (PCI_PCIE_ADDR(mIohInfo[ioh].BusNum, device, function, (CapabilitiesOffset + 0x8)), iioLerControl.Data);
    }
    return EFI_SUCCESS;
}

//Need to wrap in an EX flag. Not supported in EP
EFI_STATUS
IIOEnableViral (
  IN UINT8 IIO
  )
{
  VIRAL_IIO_RAS_STRUCT iioViralReg; //VIRAL_IIO_RAS_REG
  UINT8 IIOSocketID;

  if(IIO >= MAX_SOCKET)
    return EFI_INVALID_PARAMETER; 

  IIOSocketID = mIioUds->PlatformData.IIO_resource[IIO].SocketID;

  //
  // Set Viral Enable on the IIO
  //
  iioViralReg.Data = 0;
  iioViralReg.Data |=  mCpuCsrAccess->ReadCpuCsr(IIOSocketID, 0, VIRAL_IIO_RAS_REG) ;
  iioViralReg.Bits.iio_fatal_viral_alert_enable = 1;
  if (mErrLogSetupData->IoMcaEn == TRUE) {
	iioViralReg.Bits.iio_signal_global_fatal = 1;
  }
  iioViralReg.Bits.iio_global_viral_mask = 1;
  iioViralReg.Bits.generate_viral_alert = 0;
  iioViralReg.Bits.iio_viral_status = 0;
  iioViralReg.Bits.iio_viral_state = 0;
  mCpuCsrAccess->WriteCpuCsr(IIOSocketID, 0, VIRAL_IIO_RAS_REG, iioViralReg.Data);
  return EFI_SUCCESS; 
}*/

EFI_STATUS
IIOErrorEnable(
  IN UINT8 RootBridge
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  GERRCTL_IIO_RAS_STRUCT iioGErrCtlReg;
  GNERRMASK_IIO_RAS_STRUCT GnerrMask;
  GFERRMASK_IIO_RAS_STRUCT GferrMask;
  GCERRMASK_IIO_RAS_STRUCT GcerrMask;
  SYSMAP_IIO_RAS_STRUCT iioSysMapReg;
  ERRPINCTL_IIO_RAS_STRUCT iioErrPinCtlReg;
  GSYSCTL_IIO_RAS_STRUCT iioGSysCtlReg;
  IRPPERRSV_N0_IIO_RAS_STRUCT iioIRPERRSVN0Reg; //IRPPERRSV_N0_IIO_RAS_REG
  VTUNCERRSEV_IIO_VTD_STRUCT  iioVTUNCERRSEVReg; //VTUNCERRSEV_IIO_VTD_REG
  UINT32                      pcie_err_msk = 0;
  
  MIERRSV_IIO_RAS_STRUCT      iioMIERRSVReg; // MIERRSV_IIO_RAS_REG
  IIOERRSV_IIO_RAS_HSX_BDX_STRUCT iioIIOERRSVReg; // IIOERRSV_IIO_RAS_REG
  IIOERRCNTSEL_IIO_RAS_HSX_BDX_STRUCT iioERRCTNSELReg; //IIOERRCNTSEL_IIO_RAS_REG
  CHANERRSEV_INT_IIOCB_FUNC0_STRUCT iioCHANERRSEVF0Reg; //CHANERRSEV_INT_IIOCB_FUNC0_REG
  CHANERRSEV_INT_IIOCB_FUNC1_STRUCT iioCHANERRSEVF1Reg; //CHANERRSEV_INT_IIOCB_FUNC1_REG
  CHANERRSEV_INT_IIOCB_FUNC2_STRUCT iioCHANERRSEVF2Reg; //CHANERRSEV_INT_IIOCB_FUNC2_REG
  CHANERRSEV_INT_IIOCB_FUNC3_STRUCT iioCHANERRSEVF3Reg; //CHANERRSEV_INT_IIOCB_FUNC3_REG
  CHANERRSEV_INT_IIOCB_FUNC4_STRUCT iioCHANERRSEVF4Reg; //CHANERRSEV_INT_IIOCB_FUNC4_REG
  CHANERRSEV_INT_IIOCB_FUNC5_STRUCT iioCHANERRSEVF5Reg; //CHANERRSEV_INT_IIOCB_FUNC5_REG
  CHANERRSEV_INT_IIOCB_FUNC6_STRUCT iioCHANERRSEVF6Reg; //CHANERRSEV_INT_IIOCB_FUNC6_REG
  CHANERRSEV_INT_IIOCB_FUNC7_STRUCT iioCHANERRSEVF7Reg; //CHANERRSEV_INT_IIOCB_FUNC7_REG  
  IRPP0ERRCNT_IIO_RAS_STRUCT iioIRPP0ERRCNTReg; // IRPP0ERRCNT_IIO_RAS_REG 
  IRPP0ERRCNTSEL_IIO_RAS_STRUCT iioIRPP0ERRCNTSELReg; // IRPP0ERRCNTSEL_IIO_RAS_REG
  MIERRCNT_IIO_RAS_STRUCT   iioMIERRCNTReg; //MIERRCNT_IIO_RAS_REG
  MIERRCNTSEL_IIO_RAS_STRUCT    iioMIERRCNTSELReg; //MIERRCNTSEL_IIO_RAS_REG 
  IIOERRCNT_IIO_RAS_STRUCT iioERRCTNReg; //IIOERRCNT_IIO_RAS_REG
  IRPP0ERRCTL_IIO_RAS_BDX_STRUCT iioIRPPOERRCTLReg; //IRPP0ERRCTL_IIO_RAS_REG
  IRPP1ERRCTL_IIO_RAS_BDX_STRUCT iioIRPP1ERRCTLReg; //IRPP0ERRCTL_IIO_RAS_REG
  IIOMISCCTRL_N1_IIO_VTD_HSX_BDX_STRUCT iioMISCCTRLReg;
  CIPINTRC_IIO_VTD_STRUCT iioCipIntrCtrlReg;
  VTUNCERRMSK_IIO_VTD_STRUCT iioVTUNCERRMSKREg; //VTUNCERRMSK_IIO_VTD_REG
  MIERRCTL_IIO_RAS_STRUCT iioMIERRCTLReg; //MIERRCTL_IIO_RAS_REG
  CHANERRMSK_INT_IIOCB_FUNC0_STRUCT iioCHANERRMSKF0Reg; //CHANERRMSK_INT_IIOCB_FUNC0_REG
  //UINT16 cpuidx; 
  //EFI_AP_PROCEDURE           IoMcaFunc;

  
  if(mErrLogSetupData->ErrorPinProgrammingForIIO == V_SYSMAP_NoInband)  {
    mErrorPinEnabled = TRUE;
  }

  //if(mErrLogSetupData->EMcaLogEn == TRUE && mErrLogSetupData->IoMcaEn == TRUE) {
    //mErrorIOMCAEnabled = TRUE; 
  //}
  
  if (mErrLogSetupData->PropagateSerr == 1) {
    mNmiOnSerr = TRUE;
  }
  if (mErrLogSetupData->PropagatePerr == 1) {
    mNmiOnPerr = TRUE;  
  }

  if (mErrLogSetupData->IohErrorEn != 1 || mErrLogSetupData->SystemErrorEn != 1)
    MaskIIOErrorReporting();
  //
  // Clear Global registers device status
  //
  ClearIIOErrorRegisterStatus(mIohInfo[RootBridge].SocketId);
#if defined(BDX_HOST) && defined(DE_SKU)
   if ((mErrLogSetupData->DmiErrorEn) && (mErrLogSetupData->SystemErrorEn == 1)) {
     IioDmiErrorEnable();
   }
#endif
  if ((mErrLogSetupData->PcieErrorsEn != 0) && (mErrLogSetupData->SystemErrorEn != 0)) {
    mPcieCorErrThreshold = mErrLogSetupData->PcieCeErrThreshold;
	Status  = IohPcieErrorEnable (RootBridge);  // AptioV server override
    pcie_err_msk = 0;
	/*if (mErrLogSetupData->LerEnable) {
		Status = IIOLerEnable(RootBridge);
		if (mErrLogSetupData->DisableC4LoggingDueToLER == 0){
		  iioERRCTLReg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridge].SocketId, 0, IIOERRCTL_IIO_RAS_REG);
		  iioERRCTLReg.Bits.c4_inbound_ler_disable = 1;
		  iioERRCTLReg.Bits.c4_outbound_ler_disable = 1;
		  mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, IIOERRCTL_IIO_RAS_REG, iioERRCTLReg.Data);
		}
		else{*/
		//}	  
    //}
  } else {
#if defined(BDX_HOST) && defined(DE_SKU)
    if (mErrLogSetupData->PcieExtndErrorsEn == 0 ) {
      pcie_err_msk = 1;
    }
#else
    pcie_err_msk = 1;
    IohPcieMaskErrors();
#endif
  }

  EnableIIOExtndPcieErrors();
  
  
  if((mErrLogSetupData->SystemErrorEn == 0) && (mErrLogSetupData->IohErrorEn == 0)) {
    //Disable Inband events
    iioSysMapReg.Bits.sev0_map = V_SYSMAP_NoInband;
    iioSysMapReg.Bits.sev1_map = V_SYSMAP_NoInband;
    iioSysMapReg.Bits.sev2_map = V_SYSMAP_NoInband;
    //Program SysMap
    mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridge].SocketId, 0, SYSMAP_IIO_RAS_REG, iioSysMapReg.Data);
    return EFI_SUCCESS; //errors disable
  }
  
  //
  // Clear Global registers device status
  //
  ClearIIOErrorRegisterStatus(mIohInfo[RootBridge].SocketId);

  //
  // Program IIO Error Severities 
  //
  if ((mErrLogSetupData->SystemErrorEn == 1) && (mErrLogSetupData->IohErrorEn == 1)){
#if defined(BDX_HOST) && defined(DE_SKU)
    if (mErrLogSetupData->CoherentInterfaceErrorEn == 1) {
#endif
      //
      // Program Coherent Interface Serverity 
      //
      iioIRPERRSVN0Reg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridge].SocketId, 0, IRPPERRSV_N0_IIO_RAS_REG); 
      iioIRPERRSVN0Reg.Bits.protocol_rcvd_poison = IIO_RECOVERABLE;
      iioIRPERRSVN0Reg.Bits.wrcache_correcc_error = IIO_CORRECTABLE; 
      iioIRPERRSVN0Reg.Bits.wrcache_uncecc_error = IIO_RECOVERABLE;
      iioIRPERRSVN0Reg.Bits.csr_acc_32b_unaligned = IIO_RECOVERABLE;
      iioIRPERRSVN0Reg.Bits.protocol_rcvd_unexprsp = IIO_FATAL;
      iioIRPERRSVN0Reg.Bits.protocol_qt_overflow_underflow = IIO_FATAL;
      iioIRPERRSVN0Reg.Bits.protocol_parity_error = IIO_FATAL; 
      mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, IRPPERRSV_N0_IIO_RAS_REG, iioIRPERRSVN0Reg.Data); 
#if defined(BDX_HOST) && defined(DE_SKU)
    }
#endif
#if defined(BDX_HOST) && defined(DE_SKU)
   if (mErrLogSetupData->VtdErrorEn == 1) {
#endif
     //
     // Program VTD error severities 
     //
     iioVTUNCERRSEVReg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridge].SocketId, 0, VTUNCERRSEV_IIO_VTD_REG);
     iioVTUNCERRSEVReg.Bits.vtderr_sev = 0; // 0 - non fatal 1 - Fatal 
     iioVTUNCERRSEVReg.Bits.protmemviol_sev = 1; // 0 - non fatal 1 - Fatal 
     iioVTUNCERRSEVReg.Bits.miscerrsev = 1; // 0 - non fatal 1 - Fatal 
     iioVTUNCERRSEVReg.Bits.unsucc_ci_rdcp_sev = 0; // 0 - non fatal 1 - Fatal 
     iioVTUNCERRSEVReg.Bits.perr_tlb1_sev = 1; // 0 - non fatal 1 - Fatal 
     iioVTUNCERRSEVReg.Bits.perr_tlb0_sev = 1; // 0 - non fatal 1 - Fatal 
     iioVTUNCERRSEVReg.Bits.perr_l3_lookup_sev = 1; // 0 - non fatal 1 - Fatal 
     iioVTUNCERRSEVReg.Bits.perr_l2_lookup_sev = 1; // 0 - non fatal 1 - Fatal 
     iioVTUNCERRSEVReg.Bits.perr_l1_lookup_sev = 1; // 0 - non fatal 1 - Fatal 
     iioVTUNCERRSEVReg.Bits.perr_context_cache_sev = 1; // 0 - non fatal 1 - Fatal 
     mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, VTUNCERRSEV_IIO_VTD_REG, iioVTUNCERRSEVReg.Data);
#if defined(BDX_HOST) && defined(DE_SKU)
    }
#endif

#if defined(BDX_HOST) && defined(DE_SKU)
   if (mErrLogSetupData->MiErrorEn == 1) {
#endif
     //
     // Program Misc Error Severities
     //
     iioMIERRSVReg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridge].SocketId, 0, MIERRSV_IIO_RAS_REG);
     iioMIERRSVReg.Bits.vpp_err_sts = IIO_CORRECTABLE;
     iioMIERRSVReg.Bits.jtag_tap_sts = IIO_CORRECTABLE;
     iioMIERRSVReg.Bits.smbus_port_sts = IIO_CORRECTABLE;
     iioMIERRSVReg.Bits.cfg_reg_par = IIO_CORRECTABLE;
     mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, MIERRSV_IIO_RAS_REG, iioMIERRSVReg.Data);
#if defined(BDX_HOST) && defined(DE_SKU)
     }
#endif

#if defined(BDX_HOST) && defined(DE_SKU)
   if (mErrLogSetupData->IioCoreErrorEn == 1){
#endif
     //
     // Program IIO Core Error Severities 
     //
     iioIIOERRSVReg.Data =  mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridge].SocketId, 0, IIOERRSV_IIO_RAS_REG);
     iioIIOERRSVReg.Bits.c6 = IIO_RECOVERABLE; 
     iioIIOERRSVReg.Bits.c5_completor_abort_address_error = IIO_RECOVERABLE;
     iioIIOERRSVReg.Bits.c4_master_abort_address_error = IIO_CORRECTABLE; // Changed to IIO_CORRECTABLE to avoid C4 and ioh errors - s4031965
     mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, IIOERRSV_IIO_RAS_REG, iioIIOERRSVReg.Data);
#if defined(BDX_HOST) && defined(DE_SKU)
   }
#endif

#if defined(BDX_HOST) && defined(DE_SKU)
  if (mErrLogSetupData->DmaErrorEn == 1) {
#endif
    //
    // Program CB DMA Error Severities
    //
    iioCHANERRSEVF0Reg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRSEV_INT_IIOCB_FUNC0_REG);
    iioCHANERRSEVF0Reg.Bits.severity18 = 0; // 0 - non fatal 1 - Fatal 
    iioCHANERRSEVF0Reg.Bits.severity17 = 0; // 0 - non fatal 1 - Fatal 
    iioCHANERRSEVF0Reg.Bits.severity16 = 0; // 0 - non fatal 1 - Fatal 
    iioCHANERRSEVF0Reg.Bits.severity13_0 = 0; // 0 - non fatal 1 - Fatal     
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRSEV_INT_IIOCB_FUNC0_REG, iioCHANERRSEVF0Reg.Data);

    iioCHANERRSEVF1Reg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRSEV_INT_IIOCB_FUNC1_REG);
    iioCHANERRSEVF1Reg.Bits.severity18 = 0; // 0 - non fatal 1 - Fatal 
    iioCHANERRSEVF1Reg.Bits.severity17 = 0; // 0 - non fatal 1 - Fatal 
    iioCHANERRSEVF1Reg.Bits.severity16 = 0; // 0 - non fatal 1 - Fatal 
    iioCHANERRSEVF1Reg.Bits.severity13_0 = 0; // 0 - non fatal 1 - Fatal    
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRSEV_INT_IIOCB_FUNC1_REG, iioCHANERRSEVF1Reg.Data);
    
    iioCHANERRSEVF2Reg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRSEV_INT_IIOCB_FUNC2_REG);
    iioCHANERRSEVF2Reg.Bits.severity16 = 0; // 0 - non fatal 1 - Fatal 
    iioCHANERRSEVF2Reg.Bits.severity13_0 = 0; // 0 - non fatal 1 - Fatal    
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRSEV_INT_IIOCB_FUNC2_REG, iioCHANERRSEVF2Reg.Data);

    iioCHANERRSEVF3Reg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRSEV_INT_IIOCB_FUNC3_REG);
    iioCHANERRSEVF3Reg.Bits.severity16 = 0; // 0 - non fatal 1 - Fatal 
    iioCHANERRSEVF3Reg.Bits.severity13_0 = 0; // 0 - non fatal 1 - Fatal    
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRSEV_INT_IIOCB_FUNC3_REG, iioCHANERRSEVF3Reg.Data);

    iioCHANERRSEVF4Reg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRSEV_INT_IIOCB_FUNC4_REG);
    iioCHANERRSEVF4Reg.Bits.severity16 = 0; // 0 - non fatal 1 - Fatal 
    iioCHANERRSEVF4Reg.Bits.severity13_0 = 0; // 0 - non fatal 1 - Fatal    
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRSEV_INT_IIOCB_FUNC4_REG, iioCHANERRSEVF4Reg.Data);

    iioCHANERRSEVF5Reg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRSEV_INT_IIOCB_FUNC5_REG);
    iioCHANERRSEVF5Reg.Bits.severity16 = 0; // 0 - non fatal 1 - Fatal 
    iioCHANERRSEVF5Reg.Bits.severity13_0 = 0; // 0 - non fatal 1 - Fatal    
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRSEV_INT_IIOCB_FUNC5_REG, iioCHANERRSEVF5Reg.Data);

    iioCHANERRSEVF6Reg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRSEV_INT_IIOCB_FUNC6_REG);
    iioCHANERRSEVF6Reg.Bits.severity16 = 0; // 0 - non fatal 1 - Fatal 
    iioCHANERRSEVF6Reg.Bits.severity13_0 = 0; // 0 - non fatal 1 - Fatal     
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRSEV_INT_IIOCB_FUNC6_REG, iioCHANERRSEVF6Reg.Data);

    iioCHANERRSEVF7Reg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRSEV_INT_IIOCB_FUNC7_REG);
    iioCHANERRSEVF7Reg.Bits.severity16 = 0; // 0 - non fatal 1 - Fatal 
    iioCHANERRSEVF7Reg.Bits.severity13_0 = 0; // 0 - non fatal 1 - Fatal    
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRSEV_INT_IIOCB_FUNC7_REG, iioCHANERRSEVF7Reg.Data);
#if defined(BDX_HOST) && defined(DE_SKU)
  }
#endif
  //
  // Program Error Counters
  //
#if defined(BDX_HOST) && defined(DE_SKU)
  if (mErrLogSetupData->CoherentInterfaceErrorEn == 1) {
#endif
    //
    // Clear IRP Error Counters
    //
    iioIRPP0ERRCNTReg.Data = 0xff;
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, IRPP0ERRCNT_IIO_RAS_REG, iioIRPP0ERRCNTReg.Data); 
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, IRPP1ERRCNT_IIO_RAS_REG, iioIRPP0ERRCNTReg.Data);

    //
    // Program IRP Error Counter
    //
    iioIRPP0ERRCNTSELReg.Data = 0; 
    iioIRPP0ERRCNTSELReg.Bits.irp_error_count_select = 0x7FFFF;
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, IRPP0ERRCNTSEL_IIO_RAS_REG, iioIRPP0ERRCNTSELReg.Data);
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, IRPP1ERRCNTSEL_IIO_RAS_REG, iioIRPP0ERRCNTSELReg.Data);
#if defined(BDX_HOST) && defined(DE_SKU)
  }
#endif

#if defined(BDX_HOST) && defined(DE_SKU)
  if (mErrLogSetupData->MiErrorEn == 1){
#endif
    //
    // Clear IIO Misc Error Counters
    //
    iioMIERRCNTReg.Data = 0xff;
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, MIERRCNT_IIO_RAS_REG,iioMIERRCNTReg.Data);
   //
   // Program  IIO Misc Error Counter
   //
    iioMIERRCNTSELReg.Data = 0;
    iioMIERRCNTSELReg.Bits.dfx_inj_err = 1;
    iioMIERRCNTSELReg.Bits.vpp_err_sts = 1;
    iioMIERRCNTSELReg.Bits.jtag_tap_sts = 1;
    iioMIERRCNTSELReg.Bits.cfg_reg_par = 1;
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, MIERRCNTSEL_IIO_RAS_REG,iioMIERRCNTSELReg.Data);
#if defined(BDX_HOST) && defined(DE_SKU)
  }       
#endif

#if defined(BDX_HOST) && defined(DE_SKU)
  if (mErrLogSetupData->IioCoreErrorEn == 1){
#endif
    //
    // Clear IIO Core Error Counters
    //
    iioERRCTNReg.Data = 0xff; 
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, IIOERRCNT_IIO_RAS_REG,iioERRCTNReg.Data);

    //
    // Program  IIO Core Error Counter
    //    
    iioERRCTNSELReg.Data = 0;
    iioERRCTNSELReg.Bits.c6 = 1;
    iioERRCTNSELReg.Bits.c5 = 1;
    iioERRCTNSELReg.Bits.c4 = 1;
    iioERRCTNSELReg.Bits.thirteen_msi_address_error_select = 1;
    iioERRCTNSELReg.Bits.twofive_core_header_queue_parity_error_select = 1;
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, IIOERRCNTSEL_IIO_RAS_REG,iioERRCTNSELReg.Data);
#if defined(BDX_HOST) && defined(DE_SKU)
  }       
#endif
  //
  // Program IIO Local Ctl(Mask) Registers
  //

#if defined(BDX_HOST) && defined(DE_SKU)
  if (mErrLogSetupData->CoherentInterfaceErrorEn == 1){
#endif
    //
    // Program IRP CTL 
    // 0 - disable : 1 - enable
    //
    iioIRPPOERRCTLReg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridge].SocketId, 0, IRPP0ERRCTL_IIO_RAS_REG); 
    iioIRPPOERRCTLReg.Bits.protocol_parity_error = mErrLogSetupData->irpp0_parityError;
    iioIRPPOERRCTLReg.Bits.protocol_qt_overflow_underflow = mErrLogSetupData->irpp0_qtOverflow;
    iioIRPPOERRCTLReg.Bits.protocol_rcvd_unexprsp = mErrLogSetupData->irpp0_unexprsp;
    iioIRPPOERRCTLReg.Bits.csr_acc_32b_unaligned = mErrLogSetupData->irpp0_csraccunaligned;

    iioIRPPOERRCTLReg.Bits.wrcache_uncecc_error_cs0 = mErrLogSetupData->irpp0_uncecc;
    iioIRPPOERRCTLReg.Bits.wrcache_correcc_error_cs0 = mErrLogSetupData->irpp0_crrecc;
    iioIRPPOERRCTLReg.Bits.wrcache_uncecc_error_cs1 = mErrLogSetupData->irpp0_uncecc;
    iioIRPPOERRCTLReg.Bits.wrcache_correcc_error_cs1 = mErrLogSetupData->irpp0_crrecc;
    iioIRPPOERRCTLReg.Bits.protocol_rcvd_poison = mErrLogSetupData->irpp0_rcvdpoison;
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, IRPP0ERRCTL_IIO_RAS_REG, iioIRPPOERRCTLReg.Data);
  
    iioIRPP1ERRCTLReg.Bits.protocol_parity_error = mErrLogSetupData->irpp1_parityError;
    iioIRPP1ERRCTLReg.Bits.protocol_qt_overflow_underflow = mErrLogSetupData->irpp1_qtOverflow;
    iioIRPP1ERRCTLReg.Bits.protocol_rcvd_unexprsp = mErrLogSetupData->irpp1_unexprsp;
    iioIRPP1ERRCTLReg.Bits.csr_acc_32b_unaligned = mErrLogSetupData->irpp1_csraccunaligned;
    iioIRPP1ERRCTLReg.Bits.wrcache_uncecc_error_cs0 = mErrLogSetupData->irpp1_uncecc;
    iioIRPP1ERRCTLReg.Bits.wrcache_correcc_error_cs0 = mErrLogSetupData->irpp1_crrecc; 
    iioIRPP1ERRCTLReg.Bits.wrcache_uncecc_error_cs1 = mErrLogSetupData->irpp1_uncecc;
    iioIRPP1ERRCTLReg.Bits.wrcache_correcc_error_cs1 = mErrLogSetupData->irpp1_crrecc;  
    iioIRPP1ERRCTLReg.Bits.protocol_rcvd_poison = mErrLogSetupData->irpp1_rcvdpoison;
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, IRPP1ERRCTL_IIO_RAS_REG, iioIRPP1ERRCTLReg.Data);
#if defined(BDX_HOST) && defined(DE_SKU)
  }
#endif

#if defined(BDX_HOST) && defined(DE_SKU)
  if (mErrLogSetupData->VtdErrorEn == 1){
#endif
    //
    // Program VTUNCERRMSK  - 1 to mask - 0 to unmask 
    //
    iioVTUNCERRMSKREg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridge].SocketId, 0, VTUNCERRMSK_IIO_VTD_REG);
    iioVTUNCERRMSKREg.Bits.vtderr_msk = 1; 
    iioVTUNCERRMSKREg.Bits.protmemviol_msk = 0; 
    iioVTUNCERRMSKREg.Bits.miscerrm = 0; 
    iioVTUNCERRMSKREg.Bits.unsucc_ci_rdcp_msk = 0;
    iioVTUNCERRMSKREg.Bits.perr_tlb1_msk = 0;
    iioVTUNCERRMSKREg.Bits.perr_tlb0_msk = 0;
    iioVTUNCERRMSKREg.Bits.perr_l3_lookup_msk = 0;
    iioVTUNCERRMSKREg.Bits.perr_l2_lookup_msk = 0;
    iioVTUNCERRMSKREg.Bits.perr_l1_lookup_msk = 0;
    iioVTUNCERRMSKREg.Bits.perr_context_cache_msk = 0;
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, VTUNCERRMSK_IIO_VTD_REG, iioVTUNCERRMSKREg.Data);
#if defined(BDX_HOST) && defined(DE_SKU)
  }
#endif

#if defined(BDX_HOST) && defined(DE_SKU)
  if (mErrLogSetupData->MiErrorEn == 1){
#endif
    //0 - disable : 1 - enable
    iioMIERRCTLReg.Data = mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridge].SocketId, 0, MIERRCTL_IIO_RAS_REG); 
    iioMIERRCTLReg.Bits.vpp_err_sts = 1;
    iioMIERRCTLReg.Bits.jtag_tap_sts = 1;
    iioMIERRCTLReg.Bits.cfg_reg_par = 1;
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, MIERRCTL_IIO_RAS_REG, iioMIERRCTLReg.Data);
#if defined(BDX_HOST) && defined(DE_SKU)
  }
#endif
#if defined(BDX_HOST) && defined(DE_SKU)
  if (mErrLogSetupData->DmaErrorEn == 1){
#endif
    //This register is a bit for bit mask for the CHANERR register. 0 - enable : 1 - disable
    iioCHANERRMSKF0Reg.Data =  mCpuCsrAccess->ReadCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRMSK_INT_IIOCB_FUNC0_REG);
    iioCHANERRMSKF0Reg.Bits.mask13_0 = 0;
    iioCHANERRMSKF0Reg.Bits.mask16 = 0;
    iioCHANERRMSKF0Reg.Bits.mask17 = 0;
    iioCHANERRMSKF0Reg.Bits.mask18 = 0;    
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRMSK_INT_IIOCB_FUNC0_REG, iioCHANERRMSKF0Reg.Data);
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRMSK_INT_IIOCB_FUNC1_REG, iioCHANERRMSKF0Reg.Data);
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRMSK_INT_IIOCB_FUNC2_REG, iioCHANERRMSKF0Reg.Data);
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRMSK_INT_IIOCB_FUNC3_REG, iioCHANERRMSKF0Reg.Data);
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRMSK_INT_IIOCB_FUNC4_REG, iioCHANERRMSKF0Reg.Data);
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRMSK_INT_IIOCB_FUNC5_REG, iioCHANERRMSKF0Reg.Data);
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRMSK_INT_IIOCB_FUNC6_REG, iioCHANERRMSKF0Reg.Data);
    mCpuCsrAccess->WriteCpuCsr(mIohInfo[RootBridge].SocketId, 0, CHANERRMSK_INT_IIOCB_FUNC7_REG, iioCHANERRMSKF0Reg.Data);
	
	
  GnerrMask.Data = 0;
  GferrMask.Data = 0;
  GcerrMask.Data = 0;
  mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridge].SocketId, 0, GNERRMASK_IIO_RAS_REG, GnerrMask.Data);
  mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridge].SocketId, 0, GFERRMASK_IIO_RAS_REG, GferrMask.Data);
  mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridge].SocketId, 0, GCERRMASK_IIO_RAS_REG, GcerrMask.Data);
  
#if defined(BDX_HOST) && defined(DE_SKU)
  }
#endif
 }
  //
  // Global Error Control
  // Note: Currently enabled for IOH PCI-Ex errors. Do it for other bits when supporting CSI VT-D etc.
  //
  iioGErrCtlReg.Data = mCpuCsrAccess->ReadCpuCsr (mIohInfo[RootBridge].SocketId, 0, GERRCTL_IIO_RAS_REG);

  //Enable all errors. 0 enable - 1 disable
  iioGErrCtlReg.Bits.csi_err_msk0 = 0;
  iioGErrCtlReg.Bits.csi_err_msk1 = 0;
  iioGErrCtlReg.Bits.csip_err_msk0 = 0;
  iioGErrCtlReg.Bits.csip_err_msk1 = 0;
  iioGErrCtlReg.Bits.dma_err_msk = 0;
//  iioGErrCtlReg.Bits.dmi_err_msk = pcie_err_msk; // AptioV Server Override: Mask the Errors only when PcieErrorsEn is Disabled from setup
  iioGErrCtlReg.Bits.ioh_err_msk = 1; //Temporal WA 4032217-jherna3-disable error escalation, c4  is causing smi storm
  iioGErrCtlReg.Bits.mi_err_msk = 0; 
  iioGErrCtlReg.Bits.vtd_err_msk = 0;
  iioGErrCtlReg.Bits.therm_err_msk = 0; //is it used? there is not local registers for this error. 
// AptioV Server Override Start: Mask the Errors only when PcieErrorsEn is Disabled from setup
  if ((mErrLogSetupData->PcieErrorsEn == 0) || (mErrLogSetupData->SystemErrorEn == 0)) {
    iioGErrCtlReg.Bits.dmi_err_msk = pcie_err_msk;
    iioGErrCtlReg.Bits.pcie_err_msk0 = pcie_err_msk;
    iioGErrCtlReg.Bits.pcie_err_msk1 = pcie_err_msk; 
    iioGErrCtlReg.Bits.pcie_err_msk2 = pcie_err_msk;
    iioGErrCtlReg.Bits.pcie_err_msk3 = pcie_err_msk; 
    iioGErrCtlReg.Bits.pcie_err_msk4 = pcie_err_msk; 
    iioGErrCtlReg.Bits.pcie_err_msk5 = pcie_err_msk; 
    iioGErrCtlReg.Bits.pcie_err_msk6 = pcie_err_msk; 
    iioGErrCtlReg.Bits.pcie_err_msk7 = pcie_err_msk; 
    iioGErrCtlReg.Bits.pcie_err_msk8 = pcie_err_msk;
    iioGErrCtlReg.Bits.pcie_err_msk9 = pcie_err_msk; 
    iioGErrCtlReg.Bits.pcie_err_msk10 = pcie_err_msk; 
  }
// AptioV Server Override End: Mask the Errors only when PcieErrorsEn is Disabled from setup
  mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridge].SocketId, 0, GERRCTL_IIO_RAS_REG, iioGErrCtlReg.Data);


  //
  // System Error Event Map Register
  //
  iioSysMapReg.Data = mCpuCsrAccess->ReadCpuCsr (mIohInfo[RootBridge].SocketId, 0, SYSMAP_IIO_RAS_REG);
  iioCipIntrCtrlReg.Data = mCpuCsrAccess->ReadCpuCsr (mIohInfo[RootBridge].SocketId, 0, CIPINTRC_IIO_VTD_REG);
  iioMISCCTRLReg.Data = mCpuCsrAccess->ReadCpuCsr (mIohInfo[RootBridge].SocketId, 0, IIOMISCCTRL_N1_IIO_VTD_REG);
  iioErrPinCtlReg.Data = mCpuCsrAccess->ReadCpuCsr (mIohInfo[RootBridge].SocketId, 0,  ERRPINCTL_IIO_RAS_REG);
      
  // Global System Event Control Register - Error enable for Severity 0,1,2 
  //
  iioGSysCtlReg.Data = mCpuCsrAccess->ReadCpuCsr (mIohInfo[RootBridge].SocketId, 0,  GSYSCTL_IIO_RAS_REG);
 
  if((mErrLogSetupData->SystemErrorEn != 0) && (mErrLogSetupData->IohErrorEn != 0)) {
    
    mLogIohErrors = 1;
    iioGSysCtlReg.Bits.sev0_en = 1;
    iioGSysCtlReg.Bits.sev1_en = 1;
    iioGSysCtlReg.Bits.sev2_en = 1;

    if (mErrorPinEnabled == FALSE)  {

      //disable error pin reporting 
      iioErrPinCtlReg.Bits.pin0 = V_ERRPINCTL_DIS;
      iioErrPinCtlReg.Bits.pin1 = V_ERRPINCTL_DIS;
      iioErrPinCtlReg.Bits.pin2 = V_ERRPINCTL_DIS;  
      
      /*if (mErrorIOMCAEnabled == TRUE) {

        IoMcaFunc = IIOEnableIoMca;
        
        //Set enable bit on IIOMISCCTRL to enable IO_MCA
        iioMISCCTRLReg.Bits.enable_io_mca = 1;        
          
        //Disable Inband events
        iioSysMapReg.Bits.sev0_map = V_SYSMAP_NoInband;
        iioSysMapReg.Bits.sev1_map = V_SYSMAP_NoInband;
        iioSysMapReg.Bits.sev2_map = V_SYSMAP_NoInband;
        
      } else {

        IoMcaFunc = IIODisableIoMca; 

        //Clear enable bit on IIOMISCCTRL to disable IO_MCA
        iioMISCCTRLReg.Bits.enable_io_mca = 0;*/

        //Enable SMI IIO inband event
        iioSysMapReg.Bits.sev0_map = V_SYSMAP_SMI_PMI;
        iioSysMapReg.Bits.sev1_map = V_SYSMAP_SMI_PMI;
        iioSysMapReg.Bits.sev2_map = V_SYSMAP_SMI_PMI;
       
      //}
      
    } else {

      //IoMcaFunc = IIODisableIoMca; 

      //Clear enable bit on IIOMISCCTRL to disable IO_MCA
      //iioMISCCTRLReg.Bits.enable_io_mca = 0;
        
      //
      // Error Pin Control Register - enable for error severity
      //
      iioErrPinCtlReg.Bits.pin0 = V_ERRPINCTL_SEV;
      iioErrPinCtlReg.Bits.pin1 = V_ERRPINCTL_SEV;
      iioErrPinCtlReg.Bits.pin2 = V_ERRPINCTL_SEV;
      
      iioSysMapReg.Bits.sev0_map = V_SYSMAP_NoInband;
      iioSysMapReg.Bits.sev1_map = V_SYSMAP_NoInband;
      iioSysMapReg.Bits.sev2_map = V_SYSMAP_NoInband;
    }
  }else { 
  
    mLogIohErrors = 0;
    iioGSysCtlReg.Bits.sev0_en = 0;
    iioGSysCtlReg.Bits.sev1_en = 0;
    iioGSysCtlReg.Bits.sev2_en = 0;

    
    iioErrPinCtlReg.Bits.pin0 = V_ERRPINCTL_DIS;
    iioErrPinCtlReg.Bits.pin1 = V_ERRPINCTL_DIS;
    iioErrPinCtlReg.Bits.pin2 = V_ERRPINCTL_DIS; 

    //IoMcaFunc = IIODisableIoMca;

    //Clear enable bit on IIOMISCCTRL to disable IO_MCA
    //iioMISCCTRLReg.Bits.enable_io_mca = 0;

    //Disable Inband events
    iioSysMapReg.Bits.sev0_map = V_SYSMAP_NoInband;
    iioSysMapReg.Bits.sev1_map = V_SYSMAP_NoInband;
    iioSysMapReg.Bits.sev2_map = V_SYSMAP_NoInband;

  }
  
  //program error pin control 
  mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridge].SocketId, 0, ERRPINCTL_IIO_RAS_REG, iioErrPinCtlReg.Data);

  // Scope is package level. No need to do on each thread.
  // Program MC_CTL to enable Fatal and non fatal MC reporting
  /*for (cpuidx = 0; cpuidx < mSmst->NumberOfCpus; cpuidx++) {
    if(cpuidx == mSmst->CurrentlyExecutingCpu) {
      IoMcaFunc(NULL);
    } else {
      mSmst->SmmStartupThisAp(IoMcaFunc,
                              cpuidx, 
                              NULL
                             );
    }
  }

  //Program iiomiscctrl to either disable or enable IOMCA
  mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridge].SocketId, 0, IIOMISCCTRL_N1_IIO_VTD_REG, iioMISCCTRLReg.Data);*/

  //Program IIOCIPINTRC
  mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridge].SocketId, 0, CIPINTRC_IIO_VTD_REG, iioCipIntrCtrlReg.Data);

  //Program SysMap
  mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridge].SocketId, 0, SYSMAP_IIO_RAS_REG, iioSysMapReg.Data);

  //Program gsysctl 
  mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridge].SocketId, 0, GSYSCTL_IIO_RAS_REG, iioGSysCtlReg.Data); 

  return Status;
}

//AptioV Server override - Start: Register SW SMI to enable PCI error logging
/*
EFI_STATUS
IIOErrorEnableCallBack(  
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
)
*/
EFI_STATUS
IIOErrorEnableCallBack (
  IN        EFI_HANDLE                      DispatchHandle,
  IN CONST  EFI_SMM_SW_REGISTER_CONTEXT     *RegisterContext,
  IN OUT    EFI_SMM_SW_CONTEXT              *SwContext,
  IN OUT    UINTN                           *CommBufferSize )
//AptioV Server override - End: Register SW SMI to enable PCI error logging
{
  UINT8      RootBridgeLoop;
  EFI_STATUS Status = EFI_SUCCESS;
   DEBUG((DEBUG_INFO,"IIOErrorEnableCallBack: entered.... \n"));

  if ((mErrLogSetupData->PcieErrorsEn != 0) && (mErrLogSetupData->SystemErrorEn != 0)){
    if(mErrLogSetupData->PCIECorrErrorEn != 0)
        mPcieCorrErrorEn = TRUE;
    if(mErrLogSetupData->PCIEUnCorrErrorEn != 0)
        mPcieUnCorrErrorEn = TRUE;
    if(mErrLogSetupData->PCIEFatalErrorEn != 0)
        mPcieFatalErrorEn = TRUE;
   }
  
  if(mErrLogSetupData->IohErrorEn == 0) {
  	return Status;
  	}
  //
  // Loop over all RootBridges
  //
  for ( RootBridgeLoop =0; RootBridgeLoop < mNumIoh; RootBridgeLoop++) {
    if (mIohInfo[RootBridgeLoop].Valid != 1)
      continue;
    Status = IIOErrorEnable(RootBridgeLoop);
  } 
  //
  // Enable PCH Error reporting (Like ICH, SIOH etc).
  //
  EnableElogPCH ();	

  return Status;
}

EFI_STATUS
WheaHESTCallBack(  
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
 )
{
  EFI_WHEA_SUPPORT_PROTOCOL     *WheaDrv;
  EFI_STATUS                     Status;
  UINT8                          RootBridgeLoop,PortIndex,Port;
  UINT16                         Data16,AerRegisterOffset;
  UINT8                          Bus,Device,Function,i;
  UINT8                          *Ecrc = NULL;
  UINT8                          NumPcieErrSources = 0;
  UINTN                          varSize;
  SYSTEM_CONFIGURATION           setupData;
  UINT32                         CapabilitiesOffset,Data32;
  PCIE_ROOT_AER_SOURCE           PlatformAER[MAX_TOTAL_IIO_PORTS];
  SYSTEM_GENERIC_ERROR_SOURCE    PciErrSources[NUMBER_PORTS_PER_SOCKET*MAX_SOCKET];

  varSize = sizeof(SYSTEM_CONFIGURATION);
  Status = gRT->GetVariable( L"IntelSetup", &gEfiSetupVariableGuid, NULL, &varSize, &setupData );  //AptioV server override - Replace Setup with IntelRcSetup
  if(EFI_ERROR(Status)) return Status;
  
  Status = gBS->LocateProtocol (&gEfiWheaSupportProtocolGuid, NULL, &WheaDrv);
  if (EFI_ERROR(Status))return Status;
  
  for ( RootBridgeLoop =0; RootBridgeLoop < MAX_SOCKET; RootBridgeLoop++) {
    PortIndex = 0;
    if (mIioUds->PlatformData.IIO_resource[RootBridgeLoop].Valid == 1) {
      for(Port = 0; Port < NUMBER_PORTS_PER_SOCKET; Port++) {
        Bus = mIohInfo[RootBridgeLoop].BusNum;
        Device = mIohInfo[RootBridgeLoop].Port[Port].Device;
        Function = mIohInfo[RootBridgeLoop].Port[Port].Function;

        if((mIohInfo[RootBridgeLoop].BusNum == 0) && (Device == 0) && (Function == 0)) continue;

        Data16 = PciExpressRead16 (PCI_PCIE_ADDR(mIohInfo[RootBridgeLoop].BusNum, Device, Function, PCI_VENDOR_ID_OFFSET));
        if (Data16 == 0xFFFF) continue;
        PCIEDEVFUNC2PORTINDEX(PortIndex,Device, Function);
        if (((RootBridgeLoop*NUMBER_PORTS_PER_SOCKET)+PortIndex) >= MAX_TOTAL_IIO_PORTS) return EFI_SUCCESS;
         Ecrc = &(setupData.Ecrc[0]);
          if(Ecrc[(RootBridgeLoop*NUMBER_PORTS_PER_SOCKET)+PortIndex] ==1){
            PlatformAER[NumPcieErrSources].Bus = Bus;
            PlatformAER[NumPcieErrSources].Dev = Device;
            PlatformAER[NumPcieErrSources].Func = Function;

            //Get Devctrl register
            CapabilitiesOffset = PciErrLibGetCapabilitiesOffset (Bus, Device, Function);
            Data16 = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function,CapabilitiesOffset+PCIE_DEVICE_CONTROL_OFFSET ));
            PlatformAER[NumPcieErrSources].DevControl = Data16;
            //Get AER registers
            AerRegisterOffset = PciErrLibGetExtendedCapabilitiesOffset (Bus, Device, Function,V_PCIE_EXT_CAP_HEADER_AERC,0);
            if (AerRegisterOffset != 0) {
              Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_UEM));
              PlatformAER[NumPcieErrSources].UEMask = Data32;
              Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_UESEV));
              PlatformAER[NumPcieErrSources].UESeverity = Data32;
              Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_CEM));
              PlatformAER[NumPcieErrSources].CEMask = Data32;
              Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_REC));
              PlatformAER[NumPcieErrSources].AerRootCmd = Data32;
              Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_AER_CTRL));
              PlatformAER[NumPcieErrSources].AerCapControl = Data32;
            }
            PciErrSources[NumPcieErrSources].Type = PCIE_ERROR_PORT_ROOT_PORT;
            PciErrSources[NumPcieErrSources].SourceId = 0;
            PciErrSources[NumPcieErrSources].Flags = 3;
            PciErrSources[NumPcieErrSources].NumOfRecords = 1;
            PciErrSources[NumPcieErrSources].MaxSecPerRecord = 1;
            PciErrSources[NumPcieErrSources].SourceData = &PlatformAER[NumPcieErrSources];
            NumPcieErrSources = NumPcieErrSources + 1;
           }
         }
      }
    }

  //Add HEST AER entries
  for (i = 0; i < NumPcieErrSources; i++) {
   Status = WheaDrv->AddErrorSource(
               WheaDrv,
               PcieRootAer,
               PciErrSources[i].Flags,
               TRUE,
               &PciErrSources[i].SourceId,
               PciErrSources[i].NumOfRecords, // No of error records
               PciErrSources[i].MaxSecPerRecord, // No of error sections in the record
               PciErrSources[i].SourceData
               );
  }
  return EFI_SUCCESS;
 }

VOID
SetupIioCipIntrCtrl (
  UINT8             RootBridgeLoop
  )
/*++

Routine Description:

  IVT-EX sighting 4031306 : CLONE from ivytown: IVT B0 - RAS - SMI not getting delivered to UBox when in VIRAL & SMI is generated on fatal error.
  
Arguments:

    inp : RootBridgeLoop

Returns:

    none

--*/
{

  CIPINTRC_IIO_VTD_STRUCT iioCipIntrCtrlReg;

  iioCipIntrCtrlReg.Data = mCpuCsrAccess->ReadCpuCsr (mIohInfo[RootBridgeLoop].SocketId, 0, CIPINTRC_IIO_VTD_REG);
  iioCipIntrCtrlReg.Bits.smi_mask = 00;
  iioCipIntrCtrlReg.Bits.smi_msi_en = 01;   

  //Program IIOCIPINTRC
  mCpuCsrAccess->WriteCpuCsr (mIohInfo[RootBridgeLoop].SocketId, 0, CIPINTRC_IIO_VTD_REG, iioCipIntrCtrlReg.Data);

}

EFI_STATUS
EnableElogIoHub (
  VOID
  )
/*++

Routine Description:


Arguments:

    N/A

Returns:

    EFI_SUCCESS if the call is succeed

--*/
{
  EFI_STATUS Status;
  UINT8 RootBridgeLoop;
  VOID  *Registration;
  //AptioV Server override - Start: Register SW SMI to enable PCI error logging
  EFI_SMM_SW_DISPATCH2_PROTOCOL   *SwDispatch = NULL;
  EFI_SMM_SW_REGISTER_CONTEXT     SwContext;
  EFI_HANDLE                      SwHandle;
  //AptioV Server override - End: Register SW SMI to enable PCI error logging
 

  Status = EFI_SUCCESS;

  for ( RootBridgeLoop =0; RootBridgeLoop < mNumIoh; RootBridgeLoop++) {
    if (mIohInfo[RootBridgeLoop].Valid != 1)
      continue;
    // this function clears the iio smi mask bits and enable SMI_MSI enable bits.
    SetupIioCipIntrCtrl(RootBridgeLoop);
  }
 //AptioV Server override - Start: Register SW SMI to enable PCI error logging
    /*
    //
    // Setup call back after pci space initialized
    //
    Status = gSmst->SmmRegisterProtocolNotify (
                      &gEfiSmmReadyToLockProtocolGuid,
                      IIOErrorEnableCallBack,
                      &Registration
                      );
     */
    
      Status = gSmst->SmmLocateProtocol (
                  &gEfiSmmSwDispatch2ProtocolGuid,
                  NULL,
                  &SwDispatch);
      ASSERT_EFI_ERROR (Status);
      DEBUG((DEBUG_INFO,"SmmLocateProtocol gEfiSmmSwDispatch2ProtocolGuid: Status: %r .... \n", Status));
      if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_LOAD, "Couldn't find SmgSwDispatch2 protocol: %r\n", Status));
          return Status;
      }

      //
      // Register the SW SMI to enable PCI error logging.
      //
      SwContext.SwSmiInputValue = PCIE_ELOG_SWSMI;
      Status = SwDispatch->Register (
                  SwDispatch,
                  IIOErrorEnableCallBack,
                  &SwContext,
                  &SwHandle);
      DEBUG((DEBUG_INFO,"Register IIOErrorEnableCallBack: Status: %r .... \n", Status));
      if (EFI_ERROR (Status)) {
          DEBUG ((EFI_D_LOAD, "Couldn't register the PCI ELOG SW SMI handler.Status: %r\n", Status));
          return Status;
      }
  //AptioV Server override - End: Register SW SMI to enable PCI error logging
 //[BDX-WA for Ecrc issue]  HSD-500193
 if (mErrLogSetupData->WheaSupport == 1) {
      Status = gSmst->SmmRegisterProtocolNotify (
                            &gEfiSmmReadyToLockProtocolGuid,
                            WheaHESTCallBack,
                            &Registration
                            );
      }

  return Status;
}

BOOLEAN
CheckForIIOErrors (
  VOID
)
{
  UINT8       RootBridgeLoop;
  GSYSST_IIO_RAS_STRUCT iioGSysStReg;
  UINT8 SocketId;
  BOOLEAN ErrorsExist = FALSE;

  if(mErrLogSetupData->IohErrorEn == 1) {
  
     // NMI raised for this error in previous SMI
    if((mPcieFatalErrDetected | mPcieNonFatalErrDetected) && ((mWheaPlatformSupport != NULL) && mErrLogSetupData->WheaLogging == 1)){
      return FALSE;
    }
    for ( RootBridgeLoop =0; RootBridgeLoop < MAX_SOCKET; RootBridgeLoop++) {
      if(mIioUds->PlatformData.IIO_resource[RootBridgeLoop].Valid != 1) continue;

      SocketId = mIioUds->PlatformData.IIO_resource[RootBridgeLoop].SocketID;
  
      iioGSysStReg.Data = mCpuCsrAccess->ReadCpuCsr(SocketId,0,GSYSST_IIO_RAS_REG);

      if(iioGSysStReg.Bits.sev0 == 1 || iioGSysStReg.Bits.sev1 == 1 || iioGSysStReg.Bits.sev2 == 1) {
        ErrorsExist |= TRUE;
      }
    }
  } else {
    ErrorsExist = FALSE;
  }

  return ErrorsExist;  
}



EFI_STATUS
ElogIohErrorHandler (
  VOID
  )
/*++

Routine Description:

    IIO error main handler routine.

Arguments:

    None 

Returns:

    EFI_SUCCESS if the call is succeed.

--*/
{

  EFI_STATUS  Status;
  UINT8       RootBridgeLoop;
  GSYSST_IIO_RAS_STRUCT iioGSysStReg;
  GSYSCTL_IIO_RAS_STRUCT iioGSysCtlReg;
  GFERRST_IIO_RAS_STRUCT iioGFERRSTReg;
  GNERRST_IIO_RAS_STRUCT iioGNERRSTReg;
  GERRCTL_IIO_RAS_STRUCT iioGErrCtlReg;
  GCERRST_IIO_RAS_STRUCT     iioGCERRSTReg;
  UINT8 pcieferr;
  UINT8 pcienerr;
  UINT8 pciecerr;
  UINT8 SocketId;
  

  Status = EFI_SUCCESS;

  for ( RootBridgeLoop =0; RootBridgeLoop < mNumIoh; RootBridgeLoop++) {
    if(mIioUds->PlatformData.IIO_resource[RootBridgeLoop].Valid != 1) continue;

    SocketId = mIioUds->PlatformData.IIO_resource[RootBridgeLoop].SocketID;
  
    iioGSysStReg.Data = mCpuCsrAccess->ReadCpuCsr(SocketId,0,GSYSST_IIO_RAS_REG);

    //
    // Global System Event Status - Checking Severity[0-2] status . Add severity 3, 4 if needed.
    //

    if ((iioGSysStReg.Bits.sev0 == 0) &&
        (iioGSysStReg.Bits.sev1 == 0) && 
        (iioGSysStReg.Bits.sev2 == 0)) {
      continue;
    }

    iioGSysCtlReg.Data = mCpuCsrAccess->ReadCpuCsr(SocketId, 0, GSYSCTL_IIO_RAS_REG);

    //
    // Checking Severity[0-2] enable.  Add severity 3, 4 if needed.
    //

    if ((iioGSysCtlReg.Bits.sev0_en == 0) &&
        (iioGSysCtlReg.Bits.sev1_en == 0) &&
        (iioGSysCtlReg.Bits.sev2_en == 0)) {
      continue;
    }
  
    //
    // Global Fatal Error Status
    //
    iioGFERRSTReg.Data = mCpuCsrAccess->ReadCpuCsr(SocketId, 0, GFERRST_IIO_RAS_REG); 
    iioGNERRSTReg.Data = mCpuCsrAccess->ReadCpuCsr(SocketId, 0, GNERRST_IIO_RAS_REG); 
    iioGCERRSTReg.Data = mCpuCsrAccess->ReadCpuCsr(SocketId, 0, GCERRST_IIO_RAS_REG);
    iioGErrCtlReg.Data = mCpuCsrAccess->ReadCpuCsr(SocketId, 0, GERRCTL_IIO_RAS_REG);

    //if(mErrorIOMCAEnabled == FALSE) {
      //
      // Check if it is Pci-ex/DMI error.
      //
      pcieferr = iioGFERRSTReg.Bits.dmi    | 
                 iioGFERRSTReg.Bits.pcie0  | 
                 iioGFERRSTReg.Bits.pcie1  | 
                 iioGFERRSTReg.Bits.pcie2  |
                 iioGFERRSTReg.Bits.pcie3  |
                 iioGFERRSTReg.Bits.pcie4  |
                 iioGFERRSTReg.Bits.pcie5  |
                 iioGFERRSTReg.Bits.pcie6  |
                 iioGFERRSTReg.Bits.pcie7  |
                 iioGFERRSTReg.Bits.pcie8  |
                 iioGFERRSTReg.Bits.pcie9  |
                 iioGFERRSTReg.Bits.pcie10 ;
      if(pcieferr)
        DEBUG ((DEBUG_ERROR, "ElogIohErrorHandler : SocketId =%x, pcieferr = %x \n", SocketId, pcieferr ));

      pcienerr = iioGNERRSTReg.Bits.dmi    | 
                 iioGNERRSTReg.Bits.pcie0  | 
                 iioGNERRSTReg.Bits.pcie1  | 
                 iioGNERRSTReg.Bits.pcie2  |
                 iioGNERRSTReg.Bits.pcie3  |
                 iioGNERRSTReg.Bits.pcie4  |
                 iioGNERRSTReg.Bits.pcie5  |
                 iioGNERRSTReg.Bits.pcie6  |
                 iioGNERRSTReg.Bits.pcie7  |
                 iioGNERRSTReg.Bits.pcie8  |
                 iioGNERRSTReg.Bits.pcie9  |
                 iioGNERRSTReg.Bits.pcie10 ;

      if(pcienerr)
        DEBUG ((DEBUG_ERROR, "ElogIohErrorHandler : SocketId =%x, pcienerr = %x \n", SocketId, pcienerr ));

      pciecerr = iioGCERRSTReg.Bits.dmi    | 
                 iioGCERRSTReg.Bits.pcie0  | 
                 iioGCERRSTReg.Bits.pcie1  | 
                 iioGCERRSTReg.Bits.pcie2  |
                 iioGCERRSTReg.Bits.pcie3  |
                 iioGCERRSTReg.Bits.pcie4  |
                 iioGCERRSTReg.Bits.pcie5  |
                 iioGCERRSTReg.Bits.pcie6  |
                 iioGCERRSTReg.Bits.pcie7  |
                 iioGCERRSTReg.Bits.pcie8  |
                 iioGCERRSTReg.Bits.pcie9  |
                 iioGCERRSTReg.Bits.pcie10 ;

      if(pciecerr)
        DEBUG ((DEBUG_ERROR, "ElogIohErrorHandler : SocketId =%x, pciecerr = %x \n", SocketId, pciecerr ));

      if ((pcieferr != 0) || 
         (pcienerr != 0)  ||
         (pciecerr != 0)) {
        IohPcieErrorHandler(RootBridgeLoop);
      }
    //}
//AptioV Server Override Start: Changes done to Support IOHCore/QPI/VTD Errors
      if (iioGFERRSTReg.Bits.tras_csi0 | iioGFERRSTReg.Bits.tras_csi1 | iioGNERRSTReg.Bits.csi0_err  | iioGNERRSTReg.Bits.csi1_err) {
    	  HandleIohQpiErrors (mIohInfo[RootBridgeLoop].BusNum, RootBridgeLoop);
      }

      if (iioGFERRSTReg.Bits.ioh | iioGNERRSTReg.Bits.ioh) {
    	  HandleIohCoreErrors (mIohInfo[RootBridgeLoop].BusNum, RootBridgeLoop);
      }

      if (iioGFERRSTReg.Bits.vtd | iioGNERRSTReg.Bits.vtd) {
    	  HandleIohVtdErrors (mIohInfo[RootBridgeLoop].BusNum, RootBridgeLoop);
      }
//AptioV Server Override End: Changes done to Support IOHCore/QPI/VTD Errors

    //
    // Clear Global registers device status
    //
     //ClearIioPcieErrorsRegisterStatus();
    ClearIIOErrorRegisterStatus(SocketId);
  }
  
  return EFI_SUCCESS;
}


VOID
ClearIioPcieErrorsRegisterStatus(
 VOID
)
{
  UINT8 Bus, RPBus;
  UINT8 Device,RPDevice;
  UINT8 Function,RPFunction;
  UINT8 Port;
  UINT8 SecondaryBusNumber,SubordinateBusNumber;
  UINT16 AerRegisterOffset;
  UINT32 StatusReg;
  BOOLEAN IsMultiFuncDev[PCI_MAX_FUNC+1];
  UINT8  RootBridgeLoop;


  for ( RootBridgeLoop =0; RootBridgeLoop < mNumIoh; RootBridgeLoop++) {
   if(mIioUds->PlatformData.IIO_resource[RootBridgeLoop].Valid != 1) continue;

  
  for(Port = 0; Port < NUMBER_PORTS_PER_SOCKET; Port++) {
    
    Bus = mIohInfo[RootBridgeLoop].BusNum;
    Device = mIohInfo[RootBridgeLoop].Port[Port].Device;
    Function = mIohInfo[RootBridgeLoop].Port[Port].Function;
    RPBus = Bus;
    RPDevice = Device;
    RPFunction = Function;
 
    //
    // Check if the root port is enabled.
    //
    if (!PciErrLibIsDevicePresent (Bus, Device, Function)) {
      continue;
    }

    if (PciErrLibIsPciBridgeDevice (Bus, Device, Function)) {

        SecondaryBusNumber = PciExpressRead8 ( PCI_PCIE_ADDR(Bus,
                                           Device,
                                           Function,
                                           PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET
                                         ));

        SubordinateBusNumber = PciExpressRead8 ( PCI_PCIE_ADDR(Bus,
                                             Device,
                                             Function,
                                             PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET
                                           ));

      for (Bus = SecondaryBusNumber; Bus <= SubordinateBusNumber; Bus++) {
        if((SecondaryBusNumber == 00) && (SubordinateBusNumber == 00)) break;
      
        for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
          for (Function = 0; Function <= PCI_MAX_FUNC; Function++) {
            IsMultiFuncDev[Function]= FALSE;
            if (PciErrLibIsDevicePresent(Bus,Device,Function)) {
              IsMultiFuncDev[Function]= TRUE;
              PciErrLibClearDeviceStatus(Bus,Device,Function);
              PciErrLibClearLegacyPciDeviceStatus(Bus,Device,Function);
              AerRegisterOffset = PciErrLibGetExtendedCapabilitiesOffset (Bus, Device, Function,V_PCIE_EXT_CAP_HEADER_AERC,0);
              if (AerRegisterOffset != 0) {
              //
              // Clear AER status registers including uncorrectable and correctable errors...
              //
              StatusReg = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_UES));
              PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_UES), StatusReg);
  
              StatusReg = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_CES));
              PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_CES), StatusReg);
              }
             }
           }
           for (Function=0;Function <= PCI_MAX_FUNC; Function++) {
            if (IsMultiFuncDev[Function] == TRUE){
              AerRegisterOffset = PciErrLibGetExtendedCapabilitiesOffset (Bus, Device, Function,V_PCIE_EXT_CAP_HEADER_AERC,0);
              if (AerRegisterOffset != 0) {
               //
               // Clear AER UES 
               //
                StatusReg = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_UES));
                PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_UES), StatusReg);
                //
                // Clear AER CES 
                //
                StatusReg = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_CES));
                PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_CES), StatusReg);
              }
             }
            }

            }
          }
      
        }
    PciErrLibClearDeviceStatus(RPBus,RPDevice,RPFunction);
    PciErrLibClearLegacyPciDeviceStatus(RPBus,RPDevice,RPFunction);
    ClearIohExtPcieDeviceStatus(RPBus,RPDevice,RPFunction);
    
      }

    }
    
  }

VOID
EnableIIOExtndPcieErrors(
VOID)
{
UINT8  RootBridgeLoop;
UINT8  Port,Device,Function;
  XPCORERRMSK_IIO_PCIE_BDX_STRUCT iioXPCorErrMskReg;
  XPUNCEDMASK_IIO_PCIE_STRUCT iioXPUncErrDMskReg;
  
  #ifdef GRANGEVILLE_FLAG 
  XPCOREDMASK_IIO_PCIE_BDX_STRUCT  iioXPCorErrDMskReg;
  #endif

  for ( RootBridgeLoop =0; RootBridgeLoop < mNumIoh; RootBridgeLoop++) {
    if(mIohInfo[RootBridgeLoop].Valid != 1) continue;

    for(Port = 0; Port < NUMBER_PORTS_PER_SOCKET; Port++) {

      Device = mIohInfo[RootBridgeLoop].Port[Port].Device;
      Function = mIohInfo[RootBridgeLoop].Port[Port].Function;
      if ((mErrLogSetupData->PcieExtndErrorsEn == 1) || (mErrLogSetupData->PcieErrorsEn == 1)){
        //
        // XP Correctable Error Mask Register
        //
        iioXPCorErrMskReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(mIohInfo[RootBridgeLoop].BusNum, Device, Function, ONLY_REGISTER_OFFSET(XPCORERRMSK_IIO_PCIE_REG)));
        iioXPCorErrMskReg.Bits.pci_link_bandwidth_changed_mask = 0;
        PciExpressWrite32 (PCI_PCIE_ADDR(mIohInfo[RootBridgeLoop].BusNum, Device, Function, ONLY_REGISTER_OFFSET(XPCORERRMSK_IIO_PCIE_REG)), iioXPCorErrMskReg.Data);
  
        // XP Un-Correctable Error Mask Register - Mask bit6 (received_pcie_completion_with_ur_detect_mask)
        //
        iioXPUncErrDMskReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(mIohInfo[RootBridgeLoop].BusNum, Device, Function, ONLY_REGISTER_OFFSET(XPUNCEDMASK_IIO_PCIE_REG)));
        iioXPUncErrDMskReg.Bits.received_pcie_completion_with_ur_detect_mask = 1;  //  mask Bit6
        iioXPUncErrDMskReg.Bits.sent_completion_with_unsupported_request_detect_mask = 1;
        iioXPUncErrDMskReg.Bits.sent_completion_with_completer_abort_detect_mask = 1;
        PciExpressWrite32 (PCI_PCIE_ADDR(mIohInfo[RootBridgeLoop].BusNum, Device, Function, ONLY_REGISTER_OFFSET(XPUNCEDMASK_IIO_PCIE_REG)), iioXPUncErrDMskReg.Data);
      }
#ifdef GRANGEVILLE_FLAG     
  
     if (mErrLogSetupData->PcieExtndErrorsEn == 0) {
          //
          // XP Correctable detect Error Mask Register
          //
          iioXPCorErrDMskReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(mIohInfo[RootBridgeLoop].BusNum, Device, Function, ONLY_REGISTER_OFFSET(XPCOREDMASK_IIO_PCIE_REG)));
          iioXPCorErrDMskReg.Bits.pci_link_bandwidth_changed_detect_mask=1;
          PciExpressWrite32 (PCI_PCIE_ADDR(mIohInfo[RootBridgeLoop].BusNum, Device, Function, ONLY_REGISTER_OFFSET(XPCOREDMASK_IIO_PCIE_REG)), iioXPCorErrDMskReg.Data);
          //
          // XP Un-Correctable detect Error Mask Register
          //
          iioXPUncErrDMskReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(mIohInfo[RootBridgeLoop].BusNum, Device, Function, ONLY_REGISTER_OFFSET(XPUNCEDMASK_IIO_PCIE_REG)));
          iioXPUncErrDMskReg.Bits.received_pcie_completion_with_ur_detect_mask = 1; 
          iioXPUncErrDMskReg.Bits.sent_completion_with_unsupported_request_detect_mask = 1;
          iioXPUncErrDMskReg.Bits.sent_completion_with_completer_abort_detect_mask = 1;
          iioXPUncErrDMskReg.Bits.outbound_switch_fifo_data_parity_error_detect_mask = 1;
          iioXPUncErrDMskReg.Bits.received_pcie_completion_with_ca_detect_mask = 1;
          iioXPUncErrDMskReg.Bits.received_msi_writes_greater_than_a_dword_data_detect_mask = 1;
          iioXPUncErrDMskReg.Bits.outbound_poisoned_data_detect_mask = 1;
          PciExpressWrite32 (PCI_PCIE_ADDR(mIohInfo[RootBridgeLoop].BusNum, Device, Function, ONLY_REGISTER_OFFSET(XPUNCEDMASK_IIO_PCIE_REG)), iioXPUncErrDMskReg.Data);
        }
#endif
      ClearIohExtPcieDeviceStatus (mIohInfo[RootBridgeLoop].BusNum, Device, Function);

      }
    }

        

}

VOID
IioPcieErrorEnableOnNTB(
IN UINT8 Bus,
IN UINT8 Device,
IN UINT8 Function
)
{ 
  UINT16 StatusReg16;
  UINT16      AerRegisterOffset;
  UINT32      StatusReg;
  UINT8       CapabilitiesOffset;
  UINT16      DeviceControl;
    StatusReg16 = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET));
    PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET), StatusReg16);
    PciErrLibEnablePciSerrPerrGeneration (Bus, Device, Function);
    if (PciErrLibIsPcieDevice (Bus, Device, Function)) {
    CapabilitiesOffset = PciErrLibGetCapabilitiesOffset (Bus, Device, Function);
    if (CapabilitiesOffset == 0) {
    return;
  }
  PciErrLibClearLegacyPciDeviceStatus (Bus,Device,Function);
  PciErrLibClearDeviceStatus(Bus,Device,Function);
  DeviceControl = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_CONTROL_OFFSET));
  if(mPcieCorrErrorEn == TRUE)
    DeviceControl |= (B_PCIE_DEVICE_CONTROL_COR_ERR_EN);
  if(mPcieUnCorrErrorEn == TRUE)
    DeviceControl |= (B_PCIE_DEVICE_CONTROL_NF_ERR_EN);
  if(mPcieFatalErrorEn == TRUE)
    DeviceControl |= (B_PCIE_DEVICE_CONTROL_FAT_ERR_EN);
    DeviceControl &= ~(B_PCIE_DEVICE_CONTROL_UNSUP_REQ_EN);
  PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_CONTROL_OFFSET), DeviceControl);
  AerRegisterOffset = PciErrLibGetExtendedCapabilitiesOffset (Bus, Device, Function,V_NTB_AER_CAPID,V_NTB_AER_VSEC_CAPID);
  if (AerRegisterOffset != 0) {
    StatusReg = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_UES));
    PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_UES), StatusReg);
    StatusReg = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_CES));
    PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_CES), StatusReg);
        }
    ClearIohExtPcieDeviceStatus (Bus, Device, Function);
  }
}
VOID
IohPcieNtbErrorHandler(
  IN UINT8 Bus,
  IN UINT8 Device,
  IN UINT8 Function
  )
{
  if(!PciErrLibIsDevicePresent (Bus, Device, Function) ) {
    return;
    }
  if ( PciErrLibIsPcieDevice (Bus, Device, Function) ) {
    if (!PciErrLibIsRootPortErrReportingEnabled (Bus, Device, Function)) return;
      PciErrLibPcieDeviceErrorHandler (Bus, Device, Function);
    }
  if(mPcieCorErrDetected){ //
    PciErrLibClearDeviceStatus(Bus, Device, Function);
     ClearIohExtPcieDeviceStatus (Bus, Device, Function);
  }
  if ((mPciPerrDetected && mNmiOnPerr) || (mPciSerrDetected && mNmiOnSerr)|| mPcieFatalErrDetected|| mPcieNonFatalErrDetected) {
    if (mPciXPUnsupportedRequest != 1) ElogGenerateNmiNow (); //For fatal/Non-fatal errors,trigger NMI.
   }
  if (( mPcieCorErrDetected ) && (mWheaPlatformSupport != NULL) && mErrLogSetupData->WheaLogging == 1)  {
    mPcieCorErrDetected=FALSE;
    mWheaPlatformSupport->WheaPlatformEndElog (mWheaPlatformSupport); 
  } 
}

BOOLEAN
IioPcieErrorReportingEnabled(
  IN UINT8 Bus,
  IN UINT8 Device,
  IN UINT8 Function
  )
{
  UINT8     CapabilitiesOffset;
  UINT16    PcieCapabilities;
  BOOLEAN   IsEnabled = FALSE;
  MISCCTRLSTS_1_IIO_PCIE_BDX_STRUCT iioMiscCtrlStsN1Reg;

  //
  // Check if a device is present at the specified location.
  //
  if (!PciErrLibIsDevicePresent (Bus, Device, Function)) {
    return FALSE;
  }
  
  //
  // Get the capabilities offset and verify this is a root port
  //
  CapabilitiesOffset = PciErrLibGetCapabilitiesOffset (Bus, Device, Function);
  if (CapabilitiesOffset == 0) {
    return FALSE;
  }
  
  PcieCapabilities = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_CAPABILITY_OFFSET));
  if ((PcieCapabilities & 0xF0) != V_PCIE_CAPABILITY_DPT_ROOT_PORT) {
    return FALSE;
  }
  
  iioMiscCtrlStsN1Reg.Data = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(MISCCTRLSTS_1_IIO_PCIE_REG)));
  
  if(iioMiscCtrlStsN1Reg.Bits.override_system_error_on_pcie_correctable_error_enable == 1 ||
  iioMiscCtrlStsN1Reg.Bits.override_system_error_on_pcie_non_fatal_error_enable == 1 ||
  iioMiscCtrlStsN1Reg.Bits.override_system_error_on_pcie_fatal_error_enable == 1) IsEnabled = TRUE;
  
  return IsEnabled;

}

