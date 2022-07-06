/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2005-2016 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

  @file MemoryQpiInit.c

--*/

//
// Warning 4740 : flow in or out of asm code suppresses global optimization
//
#pragma warning(disable : 4740)

#ifdef ME_SUPPORT_FLAG
#include "Library/IoLib.h"
#endif // ME_SUPPORT_FLAG
#include "ProcMemInit.h"
#include "IioEarlyInit.h"


#ifdef RC_SIM
#include "MemSpdSimInit.h"
#endif // RC_SIM

#ifdef SSA_FLAG
#define  MRC_HEAP_SIZE   (128*1024) //For BSSA we have the option of having a MAX HEAP SIZE of 128*1024bytes - BiosMalloc()
#endif  //SSA_FLAG

//
// Function Declarations
//
#ifdef DE_SKU
VOID SerialPortInitialize ( 
  IIO_GLOBALS                    *IioGlobalData,
  UINT8                           CPUType,
  UINT8                           Stepping

);
#endif
//
// Function Implementations
//

VOID
InitializeDefaultData (
  struct sysHost             *host
  )
/*++

   Executed by SBSP only. Initialize sysHost structure with default data.

  @param host - pointer to sysHost structure on stack

  @retval N/A

--*/
{
  UINT32  csrReg1;
  UINT32  csrReg2;
  UINT8   Hacount, SkuType, SubSkuType;
  UINT32  csrRegAddr = 0;
  struct sysNvram  *nvramPtr;
#ifdef SERIAL_DBG_MSG
  UINT8   *serialDebugMsgLvl;
#endif
  UINT64_STRUCT  msrData;
  UINT32 RegEcx, RegEdx;
  UINT8   socket, bus0, bus1;
  UINT32  RegEax, RegEbx;
  CAPID0_PCU_FUN3_BDX_STRUCT  capId0;
  host->var.common.rcVersion = RC_REVISION;
  //
  // Initialize  cpuFamily, cpuType, stepping
  //
  RegEax = 1; RegEcx = 0;
  GetCpuID(&RegEax, &RegEbx, &RegEcx, &RegEdx);

  host->var.common.cpuFamily = RegEax >> 4;
  host->var.common.stepping = (UINT8)(RegEax & 0x0F);

#ifdef RC_SIM
  // Apply overrrides, if needed
  RcSimSetSetupParamOverride (RCSIMCPUFAMILY, &host->var.common.cpuFamily);
  RcSimSetSetupParamOverride (RCSIMCPUSTEPPING, &host->var.common.stepping);
#endif // RC_SIM

  if(host->var.common.cpuFamily == CPU_FAMILY_HSX) {
     host->var.common.cpuType = CPU_HSX;
     host->var.common.MaxCoreCount = MAX_CORE_HSX;
  } else if ((host->var.common.cpuFamily == CPU_FAMILY_BDX) || (host->var.common.cpuFamily == CPU_FAMILY_BDX_DE)) {
     host->var.common.cpuType = CPU_BDX;
     host->var.common.MaxCoreCount = MAX_CORE_BDX;
  } else {
     host->var.common.cpuType = CPU_IVT;
     host->var.common.MaxCoreCount = MAX_CORE_IVT;
  }

  //
  // Get number of LLC ways
  //
  RegEax = 4; RegEcx = 3;
  GetCpuID(&RegEax, &RegEbx, &RegEcx, &RegEdx);

  //
  // Get CPU bus numbers from MSR 0x300
  //
  msrData = ReadMsrLocal(0x300);
  if ( (msrData.lo == 0) || ((msrData.lo & 0x80000000) == 0))  {
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "MSR(0x300) returns incorrect data %x\n", msrData.lo));
#endif // SERIAL_DBG_MSG
    RC_ASSERT(FALSE, ERR_MRC_STRUCT, 0);; // dead loop
  }
  bus0 = (UINT8) (msrData.lo & 0xff);
  bus1 = (UINT8) ((msrData.lo >> 8) & 0xff);
  //
  // Get SBSP's NodeID from local SAD_CONTROL CSR
  //
  OutPort32 (host, 0x0cf8, ((LEGACY_CSR_SAD_CONTROL & 0xff00ffff)|(bus1 << 16)));
  csrReg1 = InPort32 (host, 0x0cfc);
  //
  // Convert  NodeID to SocketID by removing bit2 (HA-ID).  SocketID = NodeID[3,1,0]
  //
  socket = (UINT8) ((csrReg1 & (BIT1+BIT0)) | ((csrReg1 & BIT3) >>1));
#ifdef SERIAL_DBG_MSG
  rcPrintf ((host, "SBSP socket = %d\n", socket));
#endif // SERIAL_DBG_MSG


  host->var.common.socketId = host->nvram.common.sbspSocketId = socket;
  host->var.common.socketPresentBitMap = 1 << socket;
  host->var.common.sbsp = 1;

  CoreGetSetupOptions(host);

  //
  // Use CF8/CFC to read legacy socket's IIO MMCFG CSR at bus0:dev5:Func0:0x84  and 0x88.
  // If mmcfg_base < mmcfg_limit, then mmcfg is not enabled.
  // Otherwise extract MMCFG_Base address from the CSR data
  //
  csrReg1 = GetMmcfgAddress(host, 0);
  //
  // csrReg1 = MMCFG_Base
  //
  csrReg2 = GetMmcfgAddress(host, 1);
  //
  // csrReg2 = MMCFG_Limit
  //
  if (csrReg1 >= csrReg2) {
    // IIO MMCFG range is NOT enabled. Warning???
    //
#ifdef SERIAL_DBG_MSG
    //
    // Warining if MMCFG_Base <= MMCFG_Limit
    //
    //    rcPrintf ((host, "Warning: IIO MMCFG.Base < MMCFG.Limit!\n"));
#endif
  }

  host->var.common.mmCfgBase        = csrReg1;
#ifdef RC_SIM
  {
    // Initialize physical bus # for all sockets
    UINT8 n;
    for (n = 0; n < MAX_SOCKET; n++) {
      host->var.common.busIio[n]    = 0x00 + (n * 0x20);
      host->var.common.busUncore[n] = 0x1f + (n * 0x20);
    }
  }
#else
  host->var.common.busIio[socket]   = bus0;
  host->var.common.busUncore[socket]= bus1;
#endif

    //
    // from this point on, SBSP at legacy socket can utilize ReadCpuPciCfgEx/WriteCpuPciCfgEx functions for CSR access
    //


  //
  // load local CAPID0 thru CAPID4 CSRs to host->var.common.procCom
  //
#ifdef YAM_ENV
  host->var.common.procCom[socket].capid0 = 0;
  host->var.common.procCom[socket].capid1 = 0;
  host->var.common.procCom[socket].capid2 = 0;
  host->var.common.procCom[socket].capid3 = 0;
  host->var.common.procCom[socket].capid4 = 0;
  host->var.common.procCom[socket].capid5 = 0;
#else
  host->var.common.procCom[socket].capid0 = ReadCpuPciCfgEx(host, socket, 0, CAPID0_PCU_FUN3_REG);
  host->var.common.procCom[socket].capid1 = ReadCpuPciCfgEx(host, socket, 0, CAPID1_PCU_FUN3_REG);
  host->var.common.procCom[socket].capid2 = ReadCpuPciCfgEx(host, socket, 0, CAPID2_PCU_FUN3_REG);
  host->var.common.procCom[socket].capid3 = ReadCpuPciCfgEx(host, socket, 0, CAPID3_PCU_FUN3_REG);
  host->var.common.procCom[socket].capid4 = ReadCpuPciCfgEx(host, socket, 0, CAPID4_PCU_FUN3_REG);
  host->var.common.procCom[socket].capid5 = ReadCpuPciCfgEx(host, socket, 0, CAPID5_PCU_FUN3_REG);
#ifdef RC_SIM
  // Apply overrrides, if needed
  RcSimSetSetupParamOverride (RCSIMCAPID0, &host->var.common.procCom[socket].capid0);
  RcSimSetSetupParamOverride (RCSIMCAPID1, &host->var.common.procCom[socket].capid1);
  RcSimSetSetupParamOverride (RCSIMCAPID2, &host->var.common.procCom[socket].capid2);
  RcSimSetSetupParamOverride (RCSIMCAPID3, &host->var.common.procCom[socket].capid3);
  RcSimSetSetupParamOverride (RCSIMCAPID4, &host->var.common.procCom[socket].capid4);
  RcSimSetSetupParamOverride (RCSIMCAPID5, &host->var.common.procCom[socket].capid5);
#endif // RC_SIM
#endif // SIM_BUILD

#ifdef DE_SKU
    if (host->var.common.procCom[socket].capid0 & BIT5) {
      host->var.common.procCom[socket].capid0 &= ~BIT1;
    }
    host->var.common.procCom[socket].capid3 &= ~0x0f;
    host->var.common.procCom[socket].capid3 |= 0xC;
#ifdef RC_SIM
    // Force DE SKU in capid0
    host->var.common.procCom[socket].capid0 |= BIT5;
#endif // RC_SIM
#endif

    //
    // Check socket type (R or B2)
    //

    capId0.Data = host->var.common.procCom[socket].capid0;

    if (capId0.Bits.de_sktb2_en) {
      host->var.common.socketType = SOCKET_EN;
    } else if(capId0.Bits.de_sktr_de1s) { // DE Processor detect
      host->var.common.socketType = SOCKET_DE;
    } else {
      host->var.common.socketType = SOCKET_EP;
    }

  if (host->var.common.cpuType == CPU_BDX)
    csrRegAddr = CSR_BDX_EMULATION_FLAG_CFG_REG;
  else
    csrRegAddr = CSR_EMULATION_FLAG_UBOX_CFG_REG;

   //
   // init emulation flag in sysHost structure
   //
  csrReg2 = ReadCpuPciCfgEx (host, socket, 0, csrRegAddr);

  //
  // read SoftSDV/VP flag from CSR
  //
  if (csrReg2 == 0xFFFFFFFF) {
    host->var.common.emulation = 0;
  } else {
    host->var.common.emulation = (UINT8) (csrReg2 & 0xff);
  }

  host->var.mem.socket[0].imcEnabled[0] = 1;
  host->var.mem.socket[1].imcEnabled[0] = 1;
  host->var.mem.socket[2].imcEnabled[0] = 1;
  host->var.mem.socket[3].imcEnabled[0] = 1;

  // SBSP SKU type
  GetSocketSkuType (host, socket, &SkuType, &SubSkuType);
  // Get the number of HA present in SBSP.
  GetHACount (host, socket, &Hacount);

  if(Hacount == 2) {
    host->var.mem.numChPerHA = 2;
    if(SkuType == SKU_EXA) {
      host->var.common.cpuSubType = CPU_IVT_EX;
    } else {
      if (host->var.common.cpuType == CPU_BDX) {
        host->var.common.cpuSubType = CPU_BDX_2HA;
      } else {
        host->var.common.cpuSubType = CPU_HSX_2HA;
      }
      host->var.mem.socket[0].imcEnabled[1] = 1;
      host->var.mem.socket[1].imcEnabled[1] = 1;
      host->var.mem.socket[2].imcEnabled[1] = 1;
      host->var.mem.socket[3].imcEnabled[1] = 1;
    }
  } else {
    host->var.mem.numChPerHA = 4;
    host->var.common.cpuSubType = CPU_HSX_1HA;
    if (host->var.common.cpuType == CPU_BDX) {
      host->var.common.cpuSubType = CPU_BDX_1HA;
      if(IsDESku(host)) host->var.mem.numChPerHA = 2;
    }
  }

  if(!IsDESku(host)) {
    host->var.mem.numChPerMC = host->var.mem.numChPerHA;
  } else {
    //
    // DE SKU has only 2 CHs per socket/ha. numChPerMC is used to calculate the MCID(mcid = ch/numChPerMc).
    // Set it to MAX_CH for DE SKUs so that mcId will always be 0
    //
    host->var.mem.numChPerMC = MAX_CH;
  }


#ifdef SERIAL_DBG_MSG
  if ((csrReg2 & QUIET_MODE) && (csrReg2 != 0xFFFFFFFF)) {
    serialDebugMsgLvl   = (UINT8 *) &host->setup.common.serialDebugMsgLvl;
    *serialDebugMsgLvl  = SDBG_MIN;
  }
#endif

  //
  // Check for optional OEM NVRAM image
  //
  nvramPtr = (struct sysNvram *) host->setup.common.nvramPtr;
  if ((nvramPtr != NULL) && (&host->nvram != nvramPtr)) {
      MemCopy((UINT8 *)&host->nvram, (UINT8 *)nvramPtr, sizeof(host->nvram));
  }

  GetMemSetupOptionsCore (host);
  //
  // init the scaling to 100
  //
#ifdef RC_SIM_FEEDBACK
  host->var.mem.eyeSizeScaling = 100;
#endif //RC_SIM_FEEDBACK
}

VOID
InitializePlatformData (
  struct sysHost             *host
  )
/*++

  Initialize Platform Data

  @param host - pointer to sysHost structure on stack

  @retval N/A

--*/
{
  UINT32 csrRegAddr = 0;
  UINT32  csrReg;
  //
  // Call platform hook for platform-specific init
  //
  OemInitializePlatformDataCore (host);

  host->memFlows = host->setup.mem.memFlows & ~MF_RT_OPT_EN;
  host->memFlowsExt = host->setup.mem.memFlowsExt;

  if (host->var.common.cpuType == CPU_BDX)
    csrRegAddr = CSR_BDX_EMULATION_FLAG_CFG_REG;
  else
    csrRegAddr = CSR_EMULATION_FLAG_UBOX_CFG_REG;

  csrReg = ReadCpuPciCfgEx (host, 0, 0, csrRegAddr);

  switch (host->var.common.emulation) {
  case SOFT_SDV_FLAG:
    if (csrReg & DDR_TRAINING_EN) {
      // Set the bit telling SoftSDV to use random training results
      csrReg |= RANDOM_TRAINING;
      WriteCpuPciCfgEx (host, 0, 0, csrRegAddr, csrReg);
    } else {
      host->memFlows &= ~(MF_X_OVER_EN | MF_REC_EN_EN | MF_RD_DQS_EN | MF_WR_LVL_EN | MF_WR_FLYBY_EN | MF_WR_DQ_EN | MF_CMDCLK_EN | MF_RD_ADV_EN | MF_WR_ADV_EN | MF_RD_VREF_EN | MF_WR_VREF_EN | MF_SENSE_AMP_EN | MF_MEMTEST_EN);
      host->memFlows &= ~(MF_E_RD_VREF_EN | MF_E_WR_VREF_EN | MF_TX_EQ_EN | MF_IMODE_EN);
      host->memFlowsExt &= ~(MF_EXT_RX_CTLE_EN);
    }
    break;

  case VP_FLAG:
    host->memFlows &= ~(MF_X_OVER_EN | MF_REC_EN_EN | MF_RD_DQS_EN | MF_WR_LVL_EN | MF_WR_FLYBY_EN | MF_WR_DQ_EN | MF_CMDCLK_EN | MF_RD_ADV_EN | MF_WR_ADV_EN | MF_RD_VREF_EN | MF_WR_VREF_EN | MF_SENSE_AMP_EN);
    break;

  case SIMICS_FLAG:
    if (csrReg & DDR_TRAINING_EN) {
      host->memFlows = MF_SENSE_AMP_EN | MF_REC_EN_EN | MF_RD_DQS_EN | MF_WR_LVL_EN | MF_WR_FLYBY_EN | MF_WR_DQ_EN;
    } else {
      host->memFlows &= ~(MF_X_OVER_EN | MF_SENSE_AMP_EN | MF_E_CMDCLK_EN | MF_E_CTLCLK_EN | MF_REC_EN_EN | MF_RD_DQS_EN | MF_WR_LVL_EN | MF_WR_FLYBY_EN |
                          MF_WR_DQ_EN | MF_CMDCLK_EN | MF_RD_ADV_EN | MF_WR_ADV_EN | MF_RD_VREF_EN | MF_WR_VREF_EN | MF_RT_OPT_EN |
                          MF_MEMTEST_EN | MF_RX_DESKEW_EN | MF_TX_DESKEW_EN | MF_E_RD_VREF_EN | MF_E_WR_VREF_EN | MF_TX_EQ_EN | MF_TX_EQ_EN | MF_L_CMDCLK_EN | MF_CMD_VREF_EN | MF_MEMINIT_EN);
      host->memFlowsExt &= ~(MF_EXT_RX_CTLE_EN);
    }
    break;
  }

  host->var.common.gpioBase = (UINT16)ReadPciCfg (host, 0, GPIO_BASE_REG) & 0xFFF0;
  host->var.common.rcbaBase = ReadPciCfg (host, 0, RCBA_BASE_REG) & 0xFFFFFFF0;

#ifdef RC_SIM
  RcSimSetSetupParamOverride (RCSIMMEMFLOWS, &host->memFlows);
  RcSimSetSetupParamOverride (RCSIMMEMFLOWSEXT, &host->memFlowsExt);
#endif // RC_SIM

#ifdef SERIAL_DBG_MSG
  rcPrintf ((host, "memFlows = 0x%x, memFlowsExt = 0x%x!\n", host->memFlows, host->memFlowsExt));
  //
  // Setup the global com port address to start with Console debug port
  //
  host->var.common.globalComPort = host->setup.common.consoleComPort;

  rcPrintf ((host, "Running on "));
  switch (host->var.common.emulation) {
  case 0:
    rcPrintf ((host, "hardware"));
    break;

  case SOFT_SDV_FLAG:
    rcPrintf ((host, "SoftSDV"));
    break;

  case VP_FLAG:
    rcPrintf ((host, "Virtual Platform"));
    break;

  case SIMICS_FLAG:
    rcPrintf ((host, "Simics"));
    break;

  case RTL_SIM_FLAG:
    rcPrintf ((host, "RTL Simulator"));
    break;

  }
  rcPrintf ((host, "\nRevision: %x\n", host->var.common.emulation));

  host->var.common.rcWriteRegDump = 1;
#ifdef SERIAL_DBG_MSG
  host->var.mem.serialDebugMsgLvl = host->setup.common.serialDebugMsgLvl;
#endif
#endif
}

UINT32
WriteBios2PcuMailboxCommand (
  struct sysHost    *host,
  UINT8  socket,
  UINT32 dwordCommand,
  UINT32 dworddata
  )
/*++

   Writes the given command to BIOS to PCU Mailbox Interface CSR register

  @param host - pointer to sysHost structure on stack
  @param socket  - CPU Socket Node number (Socket ID)
  @param dwordCommand - Pcu mailbox command to write
  @param dworddata - Pcu mailbox data

  @retval error code from the Pcu mailbox (0 = NO ERROR)

--*/
{
    UINT32  MAILBOXSTATUS;

    //
    // Poll Mailbox BUSY_RUN flag until clear (ready)
    //
    while (ReadCpuPciCfgEx (host, socket, 0, BIOS_MAILBOX_INTERFACE) & BIT31) {
#ifdef RC_SIM_FEEDBACK
      break;
#endif
    }


    //
    // Write the given command to mailbox
    //
    WriteCpuPciCfgEx (host, socket, 0, BIOS_MAILBOX_DATA, (UINT32) dworddata);
    WriteCpuPciCfgEx (host, socket, 0, BIOS_MAILBOX_INTERFACE, (UINT32) (dwordCommand | BIT31));

    //
    // Poll Mailbox BUSY_RUN flag until clear (ready)
    //

    while (ReadCpuPciCfgEx (host, socket, 0, BIOS_MAILBOX_INTERFACE) & BIT31) {
#ifdef RC_SIM_FEEDBACK
      break;
#endif
    }


    //
    // Read Mailbox data
    //

    //MAILBOXDATA = ReadCpuPciCfgEx (host, socket, 0, BIOS_MAILBOX_DATA);
    MAILBOXSTATUS = ReadCpuPciCfgEx (host, socket, 0, BIOS_MAILBOX_INTERFACE);
#ifdef RC_SIM_FEEDBACK
    MAILBOXSTATUS = 0;
#endif

    return MAILBOXSTATUS;

}

VOID
HaltCpuLocal (
  VOID
  )
/*++

  Halts the active processor

  @param None

  @retval N/A

--*/
{
#ifdef IA32
  _asm
  {
hcl_loop:
    //
    // halt and wait for the reset sequence to finish
    //
    hlt
    jmp hcl_loop
  }
#else
  EfiHalt();
#endif
}


VOID
CheckSupportedCpu (
  struct sysHost             *host
  )
/*++

     Check if unsupported CPU/steppings are installed, and if so halt system

  @param host - pointer to sysHost structure on stack

  @retval N/A

--*/
{

   BOOLEAN  unsupported = TRUE;
   if (host->var.common.cpuFamily == CPU_FAMILY_HSX || host->var.common.cpuFamily == CPU_FAMILY_BDX || host->var.common.cpuFamily == CPU_FAMILY_BDX_DE)  {

       if ( (host->var.common.cpuFamily == CPU_FAMILY_HSX  && host->var.common.stepping <= B0_REV_HSX ) || (host->var.common.cpuFamily == CPU_FAMILY_BDX && host->var.common.stepping <= C0_REV_BDX) || (host->var.common.cpuFamily == CPU_FAMILY_BDX_DE && host->var.common.stepping <= Y0_REV_BDX)) {
           //
           // Normal case - BIOS supports current stepping N and N-1
           //
           unsupported = FALSE;
#ifdef SERIAL_DBG_MSG
           rcPrintf ((host, "\n CPU Stepping %2d\n Found", host->var.common.stepping));
#endif
        }

       if ( (host->var.common.cpuFamily == CPU_FAMILY_HSX  && host->var.common.stepping == B0_REV_HSX+1 ) || (host->var.common.cpuFamily == CPU_FAMILY_BDX  && host->var.common.stepping == C0_REV_BDX+1) || (host->var.common.cpuFamily == CPU_FAMILY_BDX_DE && host->var.common.stepping == Y0_REV_BDX+1)) {
           //
           // Abnormal case -  Warn for stepping N+1
           //
           unsupported = FALSE;
#ifdef SERIAL_DBG_MSG
           rcPrintf ((host, "\n Warning: Newer CPU Stepping %2d\n Found", host->var.common.stepping));
#endif
        }
    }

    if (unsupported) {
          //
      // Error case -  Unsupported CPU family and/or  stepping. Halt system
          //
#ifdef SERIAL_DBG_MSG
          rcPrintf ((host, "\nUnsupported CPU family %x and/or Stepping %2d\n", host->var.common.cpuFamily, host->var.common.stepping));
#endif
          HaltCpuLocal();
    }
}



VOID
PrepareForWarmReset (
  struct sysHost  *host
  )
/*++

  Executed by System BSP only.
  Common software programming steps needed before warm reset and CPU-only reset.

  @param host - pointer to sysHost structure on stack

  @retval N/A

--*/
{


   //
   // Set "BIOS_RESET_CPL" flag bit of all CPU sockets in no particular order
   //

  SetBIOSInitDone(host);
#ifdef SERIAL_DBG_MSG
  rcPrintf ((host, "BIOS done set\n"));
#endif
}


VOID
CheckAndHandleResetRequests (
  struct sysHost             *host
  )
/*++

  Executed by System BSP only.
  Check global reset reqeust flag and trigger the proper type of reset.

  @param host - pointer to sysHost structure on stack

  @retval N/A

--*/
{
  UINT8   socket;
  UINT32  csrReg;

  //
  //  Call OEM hook before triggering a reset
  //
  OemCheckAndHandleResetRequestsCore(host);

  //
  // do nothing but return if no reset is required
  //
  if (host->var.common.resetRequired == 0) {
    return ;

  }

  if (host->var.common.resetRequired) {
    host->var.common.resetRequired |= POST_RESET_WARM;
  }

  // set "Comlepted Cold Reset Flow" flag in sticky scratchpad register SSR07, at 1:11:3:5ch
  // set DCU_MODE_select bit per Setup data

  if (host->var.common.resetRequired & (POST_RESET_WARM)) {
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      if (host->var.common.socketPresentBitMap & (BIT0 << socket)) {
        //
        // if socket is present
        //
        csrReg = ReadCpuPciCfgEx (host, socket, 0, BIOSSCRATCHPAD7_UBOX_MISC_REG);
        if (csrReg & BIT5) {
          continue;
          //
          // skip if bit already set
          //
        } else {

          // pass DCU_Mode select setup option via Scratch Pad register Bit24 for next warm reboot
          if (host->setup.cpu.dcuModeSelect) {
           csrReg |= BIT24;
          }

          // pass debugInterfaceEn setup option via Scratch Pad register Bit27 for next warm reboot
          if (host->setup.cpu.debugInterfaceEn == 1) {
	     csrReg |= BIT27;
	   } else if (host->setup.cpu.debugInterfaceEn == 0) {
	     csrReg &= (~BIT27);
	   }


          //
          // set "Comlepted Cold Reset Flow" bit
          //
          WriteCpuPciCfgEx (host, socket, 0, BIOSSCRATCHPAD7_UBOX_MISC_REG, (csrReg | BIT5));
        }
      }
    }
  }



#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
  //
  //  Call ME BIOS hook before triggering a reset
  //
  MEBIOSCheckAndHandleResetRequestsCore(host);
#endif // ME_SUPPORT_FLAG

  //
  // Trigger requested type of reset in the overriding order of:
  //     Power-Good-Reset
  //     Warm-Reset
  //
  if (host->var.common.resetRequired & POST_RESET_POWERGOOD) {
    //
    // Power-Good Reset (aka Cold Reset)
    //
#ifdef ME_SUPPORT_FLAG
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "\nPrepare for POWER GOOD RESET.\n"));
#endif
    // Prepare DMICTL bit1:0 for reset R_PCH_RCRB_DMIC - 0x2234)
    MmioAnd32(host->var.common.rcbaBase + 0x2234, ~((UINT32)(BIT0 + BIT1)));
    // Prepare RST_CPL1 for reset
    SetBIOSInitDone(host);
#endif // ME_SUPPORT_FLAG

#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "Issue POWER GOOD RESET!\n\n\n\n"));
#endif
    OutPort8 (host, 0xcf9, 0x0e);
    HaltCpuLocal ();
  } else if (host->var.common.resetRequired & POST_RESET_WARM) {
    //
    // Platform Warm Reset
    //
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "Issue WARM RESET!\n\n\n\n"));
#endif
    PrepareForWarmReset(host);
    //
    // Checkpoint to indicate Warm Reset for QPIRC is about to happen.
    // Note this reset is also common to MRC and CPURC
    // STS_QPI_COMPLETE = 0xaf MINOR_STS_ABOUT_TO_RESET = 0x42
    //
    OutputCheckpoint (host, 0xAF, 0x42, 0x00);
    OutPort8 (host, 0xcf9, 0x06);
    HaltCpuLocal ();
  }
}


VOID
ProcMemInit (
  struct sysHost             *host
  )
/*++

 Invocation of Memory and QPI initialization

  @param - Pointer to sysHost structure
  @param - (EFI BIOS only) host->var.common.oemVariable = PeiServices, i.e., General purpose services available to every PEIM.

  host - pointer to sysHost structure on stack
  @retval N/A

--*/
{

#ifdef SSA_FLAG
  UINT8           heap[MRC_HEAP_SIZE];
#endif  // SSA_FLAG

#ifndef RC_SIM
  IioEarlyPreDataLinkInit (host);
#endif //RC_SIM
  //
  // Initialize host structure defaults
  //
#ifdef SERIAL_DBG_MSG
  if (host->setup.common.serialDebugMsgLvl) {
    rcPrintf ((host, "BIOSSIM: InitializeDefaultData() \n"));
  }
#endif // SERIAL_DBG_MSG
  InitializeDefaultData (host);


  //
  // Initialize the power management timer base address
  //
  InitPmTimer (host);

  //
  // Initialize platform segment of sysHost
  //

#ifdef SERIAL_DBG_MSG
  if (host->setup.common.serialDebugMsgLvl) {
    rcPrintf ((host, "BIOSSIM: InitializePlatformData() \n"));
  }
#endif // SERIAL_DBG_MSG
  InitializePlatformData (host);

  //
  // Initialize Heap
  //
#ifdef SERIAL_DBG_MSG
  if (host->setup.common.serialDebugMsgLvl) {
    rcPrintf ((host, "BIOSSIM: InitHeap() \n"));
  }
#endif // SERIAL_DBG_MSG

#ifdef SSA_FLAG
//Setting the heap size before calling InitHeap()
  if (host->setup.mem.enableBiosSsaLoader) {  //Only initialize the Heap if BSSA is enabled via setup
    host->var.common.heapBase = (UINT32)heap;
    host->var.common.heapSize = MRC_HEAP_SIZE;
  }
#endif  //SSA_FLAG

  InitHeap (host);

  //
  // Initialize Cpu Timer
  //
  InitCpuTimer (host);

#ifdef SERIAL_DBG_MSG
  //
  // Oem Initialize Serial Debug
  //
  if (host->setup.common.serialDebugMsgLvl) {
    rcPrintf ((host, "BIOSSIM: InitUSBDebug() \n"));
  }
  InitUSBDebug (host);
#endif  // SERIAL_DBG_MSG

#ifdef SERIAL_DBG_MSG
  if (host->setup.common.serialDebugMsgLvl) {
    if (host->var.common.cpuType == CPU_HSX) {
      rcPrintf ((host, "\nHSX (%s) processor detected\n", (host->var.common.cpuSubType == CPU_HSX_2HA ? "2HA" : "1HA")));
    } else if (host->var.common.cpuType == CPU_BDX) {
      rcPrintf ((host, "\nBDX (%s) processor detected\n", (host->var.common.cpuSubType == CPU_BDX_2HA ? "2HA" : "1HA")));
    } else {
      rcPrintf ((host, "\nIVT processor detected\n"));
    }
  }
#endif

  //
  //  Check CPU stepping
  //
  CheckSupportedCpu(host);

  //
  // Save the RC revision to scratchpad and output if necessary
  //
  WriteCpuPciCfgEx (host, 0, 0, SR_RC_REVISION_CSR, RC_REVISION);
#ifdef SERIAL_DBG_MSG
  if (host->setup.common.serialDebugMsgLvl) {
    rcPrintf ((host, "\nCCMRC Version: %08X \n", CCMRC_REVISION));
    rcPrintf ((host, "\nMRC Sync Number: %d \n", MRC_SYNC_CL));
    rcPrintf ((host, "\nRC Version: %08X \n", RC_REVISION));
#ifndef RC_SIM_FEEDBACK
    rcPrintf ((host, "host = %08X  (pointer to sysHost structure)\n", (UINT32) host));
#endif
    if (host->var.common.usbDebugPort.Ready) {
      rcPrintf ((host, "USB Serial Debug Enabled\n\n"));
    } else {
      rcPrintf ((host, "Legacy Serial Debug Enabled\n\n"));
    }
  }
#endif
  //
  // Initialize the gpio base address
  //
  InitGpio (host);
#ifdef RC_BEGIN_END_DEBUG_HOOKS
  OutPort8(host, 0x98, 0xde);
#endif
  //
  // Invoke QPI initialization (QPIRC)
  //
#ifdef SERIAL_DBG_MSG
  rcPrintf ((host, "QPI Init starting..."));
#endif
  QpiInit (host);
#ifdef SERIAL_DBG_MSG
  rcPrintf ((host, "QPI Init completed! Reset Requested: %x\n", host->var.common.resetRequired));
#endif


  if (host->var.common.resetRequired == 0) {
    IioEarlyLinkInit(host);
  }

  //
  // If reset pending, clr this bit so SetBIOSInitDone() will not set BIOS_REST_CPL_1 early
  //
  if (host->var.common.resetRequired != 0) {
    WriteCpuPciCfgEx (host, 0, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG, (~BIT4 & (ReadCpuPciCfgEx (host, 0, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG))));
  }

  SetBIOSInitDone(host);

//
  //
  // Initialize Pipe Architecture for use by MRC
  //
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "Pipe Init starting..."));
#endif
    InitializePipe (host);
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "Pipe Init completed! Reset Requested: %x\n", host->var.common.resetRequired));
#endif

  //
  // CPU feature early config (Flex Ratio, Desired Cores, SMT etc. that needs a reset to take effect)
  //
#ifdef SERIAL_DBG_MSG
  rcPrintf ((host, "CPU Feature Early Config starting..."));
#endif
  CpuInit (host);
#ifdef SERIAL_DBG_MSG
  rcPrintf ((host, "CPU Feature Early Config completed! Reset Requested: %x\n", host->var.common.resetRequired));
#endif

  if (host->var.common.resetRequired == 0) {
    //
    // Collect previous boot errors if any
    //
    CollectPrevBootFatalErrors (host);
  }
  //
  // Invoke memory initialization (MRC)
  //
#ifdef SERIAL_DBG_MSG
  rcPrintf ((host, "START_MRC_RUN\n"));
#endif
  MemoryInit (host);
#ifdef SERIAL_DBG_MSG
  rcPrintf ((host, "Reset Requested: %x\n", host->var.common.resetRequired));
#endif

#ifndef RC_SIM
  if (host->var.common.resetRequired == 0) {
    IioEarlyPostLinkInit(host);
  }
#endif //RC_SIM

  //
  // Exit Pipe Architecture
  //
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "Pipe Exit starting..."));
#endif
    ExitPipe (host);
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "Pipe Exit completed! Reset Requested: %x\n", host->var.common.resetRequired));
#endif
#ifdef SERIAL_DBG_MSG
  //
  // Display Warning Log
  //
  DisplayWarningLog(host);
#endif  // SERIAL_DBG_MSG


  //
  // Check and Handle requested resets
  //
#ifdef SERIAL_DBG_MSG
  rcPrintf ((host, "Checking for Reset Requests ...  "));
#endif
  CheckAndHandleResetRequests (host);
  //
  // If control comes here, warm boot path has completed.
  // Return and continue with the reset of BIOS POST...
  //
#ifdef SERIAL_DBG_MSG
  rcPrintf ((host, "None \nContinue with system BIOS POST ...\n\n"));
#endif


  //
  // Publish sysHost information to be used in Post Phase
  //
  OemPublishDataForPostCore (host);

  //
  // A platform PEIM should enable R/W access to E/F segment in the S3 boot path
  // otherwise, this AP wakeup buffer can't be accessed during CPU S3 operation.
  //
  if (host->var.common.bootMode == S3Resume) {
    UINT8                                 Socket;
    PAM0123_CBOBC_MEMSAD_HSX_BDX_STRUCT   Pam0123Reg;
    PAM456_CBOBC_MEMSAD_STRUCT            Pam456Reg;
    for (Socket = 0; Socket < MAX_SOCKET; ++Socket) {
      if(host->var.common.socketPresentBitMap & (1 << Socket)) {
        Pam0123Reg.Data = ReadCpuPciCfgEx (host, Socket, 0, PAM0123_CBOBC_MEMSAD_REG);
        Pam0123Reg.Bits.pam0_hienable = 3;
        WriteCpuPciCfgEx (host, Socket, 0, PAM0123_CBOBC_MEMSAD_REG, Pam0123Reg.Data);
        Pam456Reg.Data = ReadCpuPciCfgEx (host, Socket, 0, PAM456_CBOBC_MEMSAD_REG);
        Pam456Reg.Bits.pam5_loenable = 3;
        Pam456Reg.Bits.pam5_hienable = 3;
        Pam456Reg.Bits.pam6_loenable = 3;
        Pam456Reg.Bits.pam6_hienable = 3;
        WriteCpuPciCfgEx (host, Socket, 0, PAM456_CBOBC_MEMSAD_REG, Pam456Reg.Data);
      }
    }
  }
}

#ifdef SERIAL_DBG_MSG
VOID
DisplayWarningLog (
  PSYSHOST host
  )
/*++

 Displays any entries found in the warning log

  @param - Pointer to sysHost structure

  @retval N/A

--*/
{
  UINT8 entry;

  //
  // Return if debug messages are disabled
  //
  if (host->var.mem.serialDebugMsgLvl == 0) return;

  //
  // Return if no entries
  //
  if (host->var.common.status.index == 0) return;

  for (entry = 0; entry < host->var.common.status.index; entry++) {
    rcPrintf ((host,"Entry %d: Warning Code = 0x%X, Minor Warning Code = 0x%X, Data = 0x%X\n", entry,
            (host->var.common.status.log[entry].code >> 8) & 0xFF, host->var.common.status.log[entry].code & 0xFF,
            host->var.common.status.log[entry].data));

    //
    // Check if this is a memory related warning
    //
    if ((((host->var.common.status.log[entry].code >> 8) & 0xFF) >= 0xE8) &&
        (((host->var.common.status.log[entry].code >> 8) & 0xFF) <= 0xEF)) {

      if (((host->var.common.status.log[entry].data >> 24) & 0xFF) != 0xFF) {
        rcPrintf ((host, "Socket = %d", (host->var.common.status.log[entry].data >> 24) & 0xFF));
      }

      if (((host->var.common.status.log[entry].data >> 16) & 0xFF) != 0xFF) {
        rcPrintf ((host, " Channel = %d", (host->var.common.status.log[entry].data >> 16) & 0xFF));
      }

      if (((host->var.common.status.log[entry].data >> 8) & 0xFF) != 0xFF) {
        rcPrintf ((host, " DIMM = %d", (host->var.common.status.log[entry].data >> 8) & 0xFF));
      }

      if ((host->var.common.status.log[entry].data & 0xFF) != 0xFF) {
        rcPrintf ((host, " Rank = %d", host->var.common.status.log[entry].data & 0xFF));
      }

      rcPrintf ((host, "\n\n"));
    }
  } // entry loop
} // DisplayWarningLog
#endif  // SERIAL_DBG_MSG



UINT64_STRUCT
WriteBios2VcuMailboxCommand (
  struct sysHost    *host,
  UINT8  socket,
  UINT32 dwordCommand,
  UINT32 dworddata
  )
/*++

   First  writes the given 32-bit data to VCU MAILBOX_DATA CSR,
   Then writes the given 32-bit Command to VCU MAILBOX_INTERFACE CSR,

  @param host - pointer to sysHost structure on stack
  @param socket  - CPU Socket number (Socket ID)
  @param dwordCommand - VCU mailbox command to be written
  @param dworddata - VCU mailbox data to be written

   @retval 64-bit return code from the VCU mailbox:
    Lo Dword
       [15:0] =  Command Response Code (success, timeout, etc)
       [31:16] = rsvd
    Hi Dword
       [31:0] = Return data if applicable

--*/
{
    UINT8           RetryCount;
  UINT32          cmd32 =0;
  UINT32          data32 =0;
    UINT64_STRUCT   MAILBOXSTATUS;

#ifdef SERIAL_DBG_MSG
  rcPrintf ((host, "     ** WriteBios2VcuMailboxCommand(Socket%2d)\n", socket));
#endif


    MAILBOXSTATUS.hi = MAILBOXSTATUS.lo = 0;

  for (RetryCount = 0; RetryCount < BIOS_VCU_MAILBOX_TIMEOUT_RETRY; RetryCount++) {

        //
        // Poll Mailbox RUN_BUSY flag until clear (ready)
        //
        while (ReadCpuPciCfgEx (host, socket, 0, CSR_MAILBOX_INTERFACE_VCU_FUN_0_REG) & BIT31) {}

        //
        // Write the given data / command to mailbox, setting RUN_BUSY bit to 1
        //
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "   **  Write 0x%x to VCU_MAILBOX_DATA CSR)\n", dworddata));
#endif
        WriteCpuPciCfgEx (host, socket, 0, CSR_MAILBOX_DATA_VCU_FUN_0_REG, (UINT32) dworddata);

#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "   **  Write 0x%x to VCU_MAILBOX_INTERFACE CSR)\n", (BIT31 | dwordCommand) ));
#endif
        WriteCpuPciCfgEx (host, socket, 0, CSR_MAILBOX_INTERFACE_VCU_FUN_0_REG, (UINT32) (BIT31 | dwordCommand));

        //
        // Poll Mailbox RUN_BUSY flag until clear (ready)
        //

        while (ReadCpuPciCfgEx (host, socket, 0, CSR_MAILBOX_INTERFACE_VCU_FUN_0_REG) & BIT31) {}

        //
        // Read return code from Command field of MAILBOX_INTERFACE[15:0]
        //
        cmd32 = ReadCpuPciCfgEx (host, socket, 0, CSR_MAILBOX_INTERFACE_VCU_FUN_0_REG) & 0xffff;

#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "   **  Read return code from VCU_MAILBOX_INTERFACE CSR [15:0]): 0x%x\n", cmd32));
#endif

        //
        // Read return data from MAILBOX_DATA[31:0]
        //
        data32 = ReadCpuPciCfgEx (host, socket, 0, CSR_MAILBOX_DATA_VCU_FUN_0_REG);
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "   **  Read return code from VCU_MAILBOX_DATA CSR [31:0]): 0x%x\n", data32));
#endif

        //
        // if timeout then retry, else stop
        //
        if (cmd32 == VCODE_MAILBOX_CC_TIMEOUT ) {
          continue;
        }else if (cmd32 == VCODE_MAILBOX_CC_ILLEGAL) {
          rcPrintf ((host, "     ** MailBox Wait for 5 sec for next retry \n"));
          FixedDelay(host, 5000000);	// 5 sec delay
          continue;        
        } else  break;
    }

  MAILBOXSTATUS.lo = cmd32;
  MAILBOXSTATUS.hi = data32;
    return MAILBOXSTATUS;

}

#if !(defined(IA32) || defined(SIM_BUILD))
/**

  SAD target CF8 value for get SBSP.
  @retval SAD target CF8 value

**/
UINT32
SadTargetCf8Chip
  ( void
  )
{
  UINT32  Data32;
//Grangeville AptioV server override - CPP Check
  Data32 = (0x80000000 | (1 << 16) | (29 << 11) | (1 << 8) | 0xF0);  // CSR 1:29:1:0xF0

  return Data32;
} // SadTargetCf8Chip
#endif // IA32 || SIM_BUILD

/**
  Get SBSP id for legacy remote SBSP

  @param   -  pointer to host struct
  @param   -  legacy Pch Target

  @retval legacy remote SBSP

**/

UINT8
LegacyRemoteSBSPChip (
  PSYSHOST host,
  UINT32 legacyPchTarget
  )
{
  return (legacyPchTarget & 0x7);
} // LegacyRemoteSBSPChip
