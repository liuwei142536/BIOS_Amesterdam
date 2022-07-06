//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/** @file

  Code File for CPU Power Management

  Copyright (c) 2006 - 2013, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  Module Name:  CsrPkgCstEntryCriteriaMask.c

**/

#include "CpuPpmIncludes.h"
#include "MCDDC_CTL.h"

#define PCIE_PORTS_MAX      0x0B	       // maximum of 11 pcie ports per socket   NOTE: Device 0 & 1 is shared the same bit


VOID
ProgramCsrPkgCstEntryCriteriaMask(
  EFI_CPU_PM_STRUCT   *ppm
		)
{
  UINT32 data;
  UINT32 tempdata;
  UINT8  node, mc, ch, SocketNumber;
  UINT64 RegAddr = 0; 
  UINT32 *PkgCstEntryCriteriaMask = NULL;  
  UINT8  Size = 4;
  BOOLEAN CheckStatus = FALSE;
  EFI_CPU_CSR_ACCESS_PROTOCOL *CpuCsrAccess;

  // DEBUG((EFI_D_ERROR, "NODE PRESENT BITMAP, mmCfgBase, dimmTypePresent, BoardId %x, %x, %x, %x\n", 
      //ppm->Info->SocketPresentBitMap, ppm->Info->mmCfgBase, ppm->Info->dimmTypePresent, ppm->Info->BoardId));

  SocketNumber = ppm->Info->SocketNumber;
  CpuCsrAccess = ppm->CpuCsrAccess;

  PkgCstEntryCriteriaMask = (UINT32 *)(ppm->Setup->Cst.PkgCstEntryCriteriaMask); 

  // Read (CSR)
  //PKG_CST_ENTRY_CRITERIA_MASK_PCU_FUN2_REG 
  data = CpuCsrAccess->ReadCpuCsr(SocketNumber, 0, PKG_CST_ENTRY_CRITERIA_MASK_PCU_FUN2_REG);

	// loop through all 4 channels and if SREF_EN isset on any exit out of the loop
	// If SREF_EN set on any channel the enable DRAM_IN_SR for the corresponding socket
	CheckStatus = FALSE;

  if(!(ppm->Setup->AdvPwrMgtCtlFlags & PCD_CPU_PKG_CST_ENTRY_VAL_CTL)) { //is AUTO
    data = 0;
	  // first we need to determine what memory channels channes are present
	  // We will do this by checking DIMMMTR_0, Bit14, to verify the channel is active.
	  // This is necessary becuse HW will indicate that SR is enabled for none active channels
	  // NOTE: The assumption here is that CIIMM ) must be popluate first on any channel. This is a requirement.
    for(mc = 0; mc < ppm->Info->TotHa[0]; mc++) {
      node = SKTMC_TO_NODE(SocketNumber, mc);
      if(ppm->Info->SystemMemoryMap->NodeInfo[node].Enabled == 0) {
        continue;
      }
      for(ch = 0; ch < MAX_CH; ch++) {
        tempdata = CpuCsrAccess->ReadMemCpuCsr(node, ch, DIMMMTR_0_MCDDC_DESC_REG);
 
        // check if bit14 set to determine if we need to consider this channel
        if ((tempdata & POP_CH_EN) == POP_CH_EN ) {
          tempdata = CpuCsrAccess->ReadMemCpuCsr(node, ch, PM_SREF_MCDDC_CTL_REG);

          if ((tempdata & CHK_SREF_EN) == CHK_SREF_EN ) {
            CheckStatus = TRUE;
          } // end check to verify Opprotunistic SR enabled
        }
      }
    }

    data &= ~DRAM_IN_SR;
    if (CheckStatus == TRUE) {
      data |= DRAM_IN_SR;
    }	

    // Check QPI 0 to see if links are set for L1 or L0s if so set corresponding bit in 
    data &= ~(QPI_0_IN_L0S + QPI_0_IN_L1);
    tempdata = CpuCsrAccess->ReadCpuCsr(SocketNumber, 0, QPILS_QPIAGENT_LL_REG);
    if (((tempdata & QPI_LNK_STATUS_MASK) == CHK_QPI_NORM_OP) && (tempdata != 0xFFFFFFFF)) {
       tempdata = CpuCsrAccess->ReadCpuCsr(SocketNumber, 0, QPILCL_QPIAGENT_LL_REG);
       if (((tempdata & CHK_L1_ENABLE) == CHK_L1_ENABLE) && (tempdata != 0xFFFFFFFF)){
         data |= QPI_0_IN_L1;
       } else {
        if (((tempdata & CHK_L0S_ENABLE) == CHK_L0S_ENABLE) && (tempdata != 0xFFFFFFFF)){
          data |= QPI_0_IN_L0S;
        }
       }
     } //end if QPI0
     
    // Check QPI 1 to see if links are set for L1 or L0s if so set corresponding bit in PKG_CST_ENTRY_CRITERIA_MASK
    data &= ~(QPI_1_IN_L0S + QPI_1_IN_L1);
    tempdata = CpuCsrAccess->ReadCpuCsr(SocketNumber, 1, QPILS_QPIAGENT_LL_REG);
    if (((tempdata & QPI_LNK_STATUS_MASK) == CHK_QPI_NORM_OP) && (tempdata != 0xFFFFFFFF)) {
       tempdata = CpuCsrAccess->ReadCpuCsr(SocketNumber, 1, QPILCL_QPIAGENT_LL_REG); 
       if (((tempdata & CHK_L1_ENABLE) == CHK_L1_ENABLE) && (tempdata != 0xFFFFFFFF)){
         data |= QPI_1_IN_L1;
       } else {
         if (((tempdata & CHK_L0S_ENABLE) == CHK_L0S_ENABLE) && (tempdata != 0xFFFFFFFF)){
          data |= QPI_1_IN_L0S;
         }
       }
    } //end if QPI1

    // Check QPI 2 to see if links are set for L1 or L0s if so set corresponding bit in PKG_CST_ENTRY_CRITERIA_MASK
    data &= ~(QPI_2_IN_L0S + QPI_2_IN_L1);
    tempdata = CpuCsrAccess->ReadCpuCsr(SocketNumber, 2, QPILS_QPIAGENT_LL_REG);
	
    if (((tempdata & QPI_LNK_STATUS_MASK) == CHK_QPI_NORM_OP) && (tempdata != 0xFFFFFFFF)) {
      tempdata = CpuCsrAccess->ReadCpuCsr(SocketNumber, 2, QPILCL_QPIAGENT_LL_REG);
	 
      if (((tempdata & CHK_L1_ENABLE) == CHK_L1_ENABLE) && (tempdata != 0xFFFFFFFF)){
        data |= QPI_2_IN_L1;
      	} else {
        	if (((tempdata & CHK_L0S_ENABLE) == CHK_L0S_ENABLE )&& (tempdata != 0xFFFFFFFF)){
          	data |= QPI_2_IN_L0S;
        	}
      	  }
    }  
  } else { //is Manual
    // If here then user has selected manual configuration of SAPMCTL overriding normal BIOS settings
    
    tempdata = 0;
    //Make Sure the PcdCpuPkgCstEntryCriteriaMaskSktx variable valueis defaulted to 0. only bits for QPI's are set based on user configuration.
    // the rest of the bits have to be left as such in the csr.
    // Program CSR_PKG_CST_ENTRY_CRITERIA_MASK.QPI_0_IN_L0s & CSR_PKG_CST_ENTRY_CRITERIA_MASK.QPI_0_IN_L1 based upon user data
    data &= ~(QPI_0_IN_L0S + QPI_0_IN_L1); // clear QPI_0_IN_L0S(Bit22) & QPI_0_IN_L1 (Bit24)
	
    // Program CSR_PKG_CST_ENTRY_CRITERIA_MASK.QPI_1_IN_L0s & CSR_PKG_CST_ENTRY_CRITERIA_MASK.QPI_1_IN_L1 based upon user data
    data &= ~(QPI_1_IN_L0S + QPI_1_IN_L1); // clear QPI_1_IN_L0S(Bit23) & QPI_1_IN_L1 (Bit25)

    data &= ~(QPI_2_IN_L0S + QPI_2_IN_L1);  //clear QPI2 
  
    PkgCstEntryCriteriaMask = (UINT32 *)(ppm->Setup->Cst.PkgCstEntryCriteriaMask);  
    tempdata = PkgCstEntryCriteriaMask[SocketNumber];

    data |= tempdata;
  }
  
  //DEBUG ( (EFI_D_ERROR, ":: after DR_SR check data = 0, Data = %X \n\n", data) );
	
  //- This is required for PC3/PC6 to work "right" with the above change
  //- sv.socket0.uncore0.pkg_cst_entry_criteria_mask.dram_in_sr = 0
  //- Dev 0xA - Fun 0x2 - Offset 0x7C - Bit 28
  // Based on recent architect reviews (refer HSD 4030217 CR 253) removed set up control for DramInSr. Bsade on HSD 4030329, defaulted the DraminSR to 0 to make the PC3/PC6 to work correct
  data &= ~DRAM_IN_SR;
  
  // Apply HSX W/A (s4987208,s4969779) PKGC: Need to set QPI L1 as an entry criteria always
  data |=  (QPI_0_IN_L1  + QPI_1_IN_L1  + QPI_2_IN_L1);  //Force all L1 bits to '1' regardless of port's L1 state
  data &= ~(QPI_0_IN_L0S + QPI_1_IN_L0S + QPI_2_IN_L0S); //Force all L0s bits to '0'. L0s not supported so no bits should be set but just make sure.
    
  // Program CSR_PKG_CST_ENTRY_CRITERIA_MASK
  CpuCsrAccess->WriteCpuCsr(SocketNumber, 0, PKG_CST_ENTRY_CRITERIA_MASK_PCU_FUN2_REG , data);
  RegAddr = CpuCsrAccess->GetCpuCsrAddress(SocketNumber, 0, PKG_CST_ENTRY_CRITERIA_MASK_PCU_FUN2_REG, &Size);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &data );

}


/* 
  This function porgram PCIE_IN_Lx fields in PKG_CST_CRITERIA_MASK CSR 
*/
VOID
EFIAPI
SetupPCIEPkgCstEntryCriteria (
  EFI_CPU_PM_STRUCT   *ppm
  )
{
  UINT32  lnkSts;
  UINT32  lnkCap;
  UINT32  pkgCstEntryCriteria;
//
//4987134 start
//
 
  UINTN  RegAddr = 0; 
//
//4987134 end
//
  UINT8   Bus = 0, Device = 0, Function = 0, Size = 4;
  UINT8   LegacySocketId, port, SocketNumber;
  EFI_CPU_CSR_ACCESS_PROTOCOL *CpuCsrAccess;


  SocketNumber = ppm->Info->SocketNumber;
  CpuCsrAccess = ppm->CpuCsrAccess;
    
  if(!(ppm->Setup->AdvPwrMgtCtlFlags & PCD_CPU_PKG_CST_ENTRY_VAL_CTL)) { //is AUTO
    // get legacy socket id from SAD_TARGET.Legacy_PCH_Target (updated by pcode)
    LegacySocketId = (UINT8)((CpuCsrAccess->ReadCpuCsr(SocketNumber, 0, SAD_TARGET_CBOBC_IOSAD_REG) >> 3) & 0x7);
    pkgCstEntryCriteria = CpuCsrAccess->ReadCpuCsr(SocketNumber, 0, PKG_CST_ENTRY_CRITERIA_MASK_PCU_FUN2_REG);
    pkgCstEntryCriteria &= MASK_PCIE_BITS;  //clear bit21:0 PCIE_Lx
    Bus = ppm->Info->IioBusNumber[SocketNumber];

    for (port = 0; port < PCIE_PORTS_MAX; port++) {

      if((LegacySocketId == SocketNumber) && (port == 0)) //skip DMI
        continue;
      
      if(port == 0) {
        Device = 0;
        Function = 0;
      } else if((port >= 1) && (port <= 2)) { //Port1A/1B
        Device = 1;
        Function = port - 1;
      } else if((port >= 3) && (port <= 6)) { //port 2A - 2D
        Device = 2;
        Function = port - 3;
      } else if((port >= 7) && (port <= 10)) { //port 3A - 3D
        Device = 3;
        Function = port - 7;
      } else { //invalid port
        DEBUG ( (EFI_D_ERROR, ":: Invalid Port Index: %02d\n", port));
        ASSERT(0); 
      }

      Size = 2;
      if(CpuCsrAccess->ReadPciCsr(Bus, Device, Function, VID_IIO_PCIE_REG, Size) != 0x8086) 
        continue;

      //check if PCIE root port active
      Size = 2;
	    lnkSts = CpuCsrAccess->ReadPciCsr(Bus, Device, Function, LNKSTS_IIO_PCIE_REG, Size);

      if(lnkSts & BIT13) {  //link is active
        Size = 4;
        lnkCap = CpuCsrAccess->ReadPciCsr(Bus, Device, Function, LNKCAP_IIO_PCIE_REG, Size);
        lnkCap &= BIT11 + BIT10;
        lnkCap >>= 10;
        switch(lnkCap) {
          case 3:     //L0s and L1 supported
            pkgCstEntryCriteria |= ((PCIE_IN_L1  + PCIE_IN_L0S) << port);
            break;
          case 2:     //L1 supported
             pkgCstEntryCriteria |= (PCIE_IN_L1 << port);
           break;
          case 1:     //L0s suported
             pkgCstEntryCriteria |= (PCIE_IN_L0S << port);
            break;
          case 0:     //ASPM disabled
          default: 
            break;
        }
      }
    } //end for PCIE port 

    // Program CSR_PKG_CST_ENTRY_CRITERIA_MASK
    //PKG_CST_ENTRY_CRITERIA_MASK_PCU_FUN2_REG
    CpuCsrAccess->WriteCpuCsr(SocketNumber, 0, PKG_CST_ENTRY_CRITERIA_MASK_PCU_FUN2_REG , pkgCstEntryCriteria);
    RegAddr = CpuCsrAccess->GetCpuCsrAddress(SocketNumber, 0, PKG_CST_ENTRY_CRITERIA_MASK_PCU_FUN2_REG, &Size);
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &pkgCstEntryCriteria );
  } //end if AUTO

  return;

}


