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

Module Name:

  CpuInit.c

Abstract:

  Cpu Initialization Module.

--*/

#include "ProcMemInit.h"

//
// Defines used for bounds checking variables before writing. Check
// GrantleySocketPkg\Include\UncoreCommonIncludes.h for the defaults.
//
VOID 
IOTGInitChanges (
    struct sysHost  *host
);

//
// Function Declarations
//
//
// Function Implementations
//
UINT32
GetAvailableCoresMask (
    struct sysHost             *host,
    UINT8                      socket
)
{
  UINT32  AvailCoresMask = 0;

  if (host->var.common.cpuType == CPU_BDX) {
    RESOLVED_CORES_PCU_FUN3_BDX_STRUCT ResolvedCoresPcuFunc3Bdx;
    CSR_DESIRED_CORES_PCU_FUN1_BDX_STRUCT CsrDesiredCoresPcuFunc3Bdx;
	
    ResolvedCoresPcuFunc3Bdx.Data = ReadCpuPciCfgEx (host, socket, 0, RESOLVED_CORES_PCU_FUN3_REG);
    AvailCoresMask = ResolvedCoresPcuFunc3Bdx.Bits.core_mask;
    CsrDesiredCoresPcuFunc3Bdx.Data = ReadCpuPciCfgEx (host, socket, 0, CSR_DESIRED_CORES_PCU_FUN1_REG);
    AvailCoresMask |= CsrDesiredCoresPcuFunc3Bdx.Bits.core_off_mask;
  }

  if (host->var.common.cpuType == CPU_HSX) {
    RESOLVED_CORES_PCU_FUN3_HSX_STRUCT ResolvedCoresPcuFunc3Hsx;
    CSR_DESIRED_CORES_PCU_FUN1_HSX_STRUCT CsrDesiredCoresPcuFunc3Hsx;
	
    ResolvedCoresPcuFunc3Hsx.Data = ReadCpuPciCfgEx (host, socket, 0, RESOLVED_CORES_PCU_FUN3_REG);
    AvailCoresMask = ResolvedCoresPcuFunc3Hsx.Bits.core_mask;
    CsrDesiredCoresPcuFunc3Hsx.Data = ReadCpuPciCfgEx (host, socket, 0, CSR_DESIRED_CORES_PCU_FUN1_REG);
    AvailCoresMask |= CsrDesiredCoresPcuFunc3Hsx.Bits.core_off_mask;
  }

  return AvailCoresMask;
}

UINT8
GetSmtCapability (
    struct sysHost             *host,
    UINT8                      socket
)
{
  UINT8  SmtCapability = 0;

  if ((host->var.common.cpuType == CPU_HSX) || (host->var.common.cpuType == CPU_BDX)) {
    SMT_CONTROL_PCU_FUN3_STRUCT  SmtControlPcuFunc3;
    SmtControlPcuFunc3.Data = ReadCpuPciCfgEx (host, socket, 0, SMT_CONTROL_PCU_FUN3_REG);
    SmtCapability = (UINT8) SmtControlPcuFunc3.Bits.fused_smt_capability;
  }

  return SmtCapability;
}

UINT32
GetResolvedCores (
    struct sysHost             *host,
    UINT8                      socket
)
{
  UINT32  ResolvedCoreMask = 0;


  if (host->var.common.cpuType == CPU_BDX) {
    RESOLVED_CORES_PCU_FUN3_BDX_STRUCT ResovedCoresMaskPcuFunc3Bdx;
    ResovedCoresMaskPcuFunc3Bdx.Data = ReadCpuPciCfgEx (host, socket, 0, RESOLVED_CORES_PCU_FUN3_REG);
    ResolvedCoreMask = (UINT32) ResovedCoresMaskPcuFunc3Bdx.Bits.core_mask;
  }

  if (host->var.common.cpuType == CPU_HSX) {
    RESOLVED_CORES_PCU_FUN3_HSX_STRUCT ResovedCoresMaskPcuFunc3Hsx;
    ResovedCoresMaskPcuFunc3Hsx.Data = ReadCpuPciCfgEx (host, socket, 0, RESOLVED_CORES_PCU_FUN3_REG);
    ResolvedCoreMask = (UINT32) ResovedCoresMaskPcuFunc3Hsx.Bits.core_mask;
  }

  return ResolvedCoreMask;
}
VOID Set_UncoreRatio_Msr(
    struct sysHost     *host,
    UINT8              socket,
    UINT32             UncoreRatioData
)

/*++

	Routine Description:
	  Program socket-scope MSR0x620.uncore FlexRatio field based on input for the given CPU socket

	Arguments:
	  host:     Pointer to sysHost structure on stack
	  socket:  CPU socket ID
	  UncoreFlexRatioData:   bit[6:0]= MAX_CLR_RATIO, bit[14:8] MIN_CLR_RATIO   all other bits are 0's

	Returns:
	        MSR 0x620.flex_ratio field  programmed on the given socket
	--*/
{
  UINT64_STRUCT msrReg;

  msrReg = ReadMsrPipe(host, socket, MSR_UNCORE_FREQ);
  msrReg.lo &= 0xFFFF0000;
  msrReg.lo |= UncoreRatioData;
  WriteMsrPipe(host, socket, MSR_UNCORE_FREQ, msrReg);
}
UINT32 GetUncoreRatio_Msr(
    struct sysHost     *host,
    UINT8              socket
)

/*++

    Routine Description:
      Reads socket-scope MSR0x620 [31:0] of a given CPU socket

    Arguments:
      host:    Pointer to sysHost structure on stack
      socket:  CPU socket ID

    Returns:
        MSR 0x620 bit[31:0]
    --*/
{
  UINT64_STRUCT msrReg;

  msrReg = ReadMsrPipe(host, socket, MSR_UNCORE_FREQ);
  return msrReg.lo;
}

VOID Set_FlexRatio_Msr(
    struct sysHost     *host,
    UINT8              socket,
    UINT32             FlexRatioCsrData
)

/*++

	Routine Description:
	  Program socket-scope MSR0x194.FlexRatio field based on input  FlexRatioCsrData for the given CPU socket

	Arguments:
	  host:     Pointer to sysHost structure on stack
	  socket:  CPU socket ID
	  FlexRatioCsrData:   bit[15:8]= FlexRatio,   bit[16] = Flex_Ratio_En,   all other bits are 0's

	Returns:
	        MSR 0x194.flex_ratio field  programmed on the given socket
	--*/
{
  UINT64_STRUCT msrReg;

  msrReg = ReadMsrPipe(host, socket, MSR_CLOCK_FLEX_MAX);
  msrReg.lo |= FlexRatioCsrData;
  WriteMsrPipe(host, socket, MSR_CLOCK_FLEX_MAX, msrReg);
}

UINT32 Get_FlexRatio_Msr(
    struct sysHost     *host,
    UINT8              socket
)

/*++

    Routine Description:
      Reads socket-scope MSR0x194 [31:0] of a given CPU socket

    Arguments:
      host:    Pointer to sysHost structure on stack
      socket:  CPU socket ID

    Returns:
        MSR0x194 bit[31:0]
    --*/
{
  UINT64_STRUCT msrReg;

  msrReg = ReadMsrPipe(host, socket, MSR_CLOCK_FLEX_MAX);
  return msrReg.lo;
}



CPU_STATUS
ProgramProcessorUncoreRatio (
  struct sysHost             *host,
  UINT8 UncoreFreq
  )
  /*++

 (
  struct sysHost             *host
  )
/*++

Routine Description:

  Executed by SBSP only. Program uncore ratio  for all CPUs
Arguments:

  host:  pointer to sysHost structure on stack

Returns:

  MSR_UNCORE_FREQ MSR programmed
  host->var.common.resetRequired updated (if reset is needed)

--*/
{
  UINT64_STRUCT    msrReg;
  UINT8            socket = 0;
  UINT8            FuseMax = 0;
  if ( !host->setup.cpu.CpuOCSupport){
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if ((host->var.common.socketPresentBitMap & (BIT0 << socket))) {
      msrReg = ReadMsrPipe(host, socket, MSR_UNCORE_FREQ);
	  
      //HSD - 5002114: min_clr_ratio > max_clr_ratio does not follow  os_ring_min = min(os_ring_min, os_ring_max
      //The max ratio that is requested to set should be greater than or equal to the min ratio, MSR_UNCORE_FREQ[14:8].
      if (UncoreFreq < ((msrReg.lo >> 8) & 0x7F)) {
        UncoreFreq = (msrReg.lo >> 8) & 0x7F;
      }
	  //check Fuse max_clr_ratio
	  FuseMax = msrReg.lo & 0x0000007F;
#ifdef SERIAL_DBG_MSG
	  rcPrintf ((host, "\n max fuse  uncore ratio is::: %x\n", FuseMax));
#endif //SERIAL_DBG_MSG   
	  //
	  //Si workaround Si HSD#2492518-start
	  //If A1 stepping max >2.5 clip it to 2.5
	  //TOR Timeout with C-states enabled on ES2 SKUs QHUY and QHU
	  //
	  if((host->var.common.cpuType == CPU_BDX) &&(host->var.common.stepping == A0_REV_BDX)&& ( host->var.common.skuType == SKU_EP)
		  &&((host->var.common.procCom[socket].capid1 & BIT20))){
		if( !host->setup.cpu.UncoreFreqOverride){
		  if(FuseMax > 25){
			  UncoreFreq = 25;
#ifdef SERIAL_DBG_MSG
			  rcPrintf ((host, "\n clipped max fuse  uncore ratio is::: %x\n", UncoreFreq));
#endif //SERIAL_DBG_MSG

		  }else{
			  UncoreFreq = FuseMax;
		  }
	    }
	  }
	  //
	  //Si workaround Si HSD#2492518-end
	  //
	  msrReg.lo &=0xFFFFF80; //bits[6:0] are max uncore freq ratio
      msrReg.lo |= (UINT32)UncoreFreq;
	  
        WriteMsrPipe(host, socket, MSR_UNCORE_FREQ, msrReg);
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "\n  uncore ratio progammed is::: %x\n", msrReg));
#endif //SERIAL_DBG_MSG
      //save it for S3
        msrReg = ReadMsrPipe(host, socket, MSR_UNCORE_FREQ);
        host->nvram.cpu.UncoreRatioMsr = msrReg.lo;
	  
    }
  }
  }
  return CPU_SUCCESS;
}

CPU_STATUS
ProgramProcessorFlexRatio (
    struct sysHost             *host
)
/*++

 (
  struct sysHost             *host
  )
/*++

Routine Description:

  Executed by SBSP only. Program FlexRatio for all CPUs
Arguments:

  host:  pointer to sysHost structure on stack

Returns:

  FLEX_RATIO CSRs programmed
  host->var.common.resetRequired updated (if reset is needed)

--*/
{
  UINT8   socket;
  UINT8   resetNeeded = 0;
  UINT32  csrReg;
  UINT32  targetRatio;
  UINT32  commonMaxRatio;
  UINT32  commonMinRatio;
  UINT32  RatioChangeFlag;
  UINT32  MaxNonTurbo;
  UINT8   sbspSktId = GetSbspSktId(host);
  UINT32  ConfigurableTDP;
  UINT32  ConfigTDPCtrl = 0;
  UINT32  ConfigTDPCtrlVal = 0;



  // -------------------------------------------------------------------------------------------
  //  Configure all CPU sockets to a common Max Non-turbo Ratio by programming FLEX_RATIO CSR
  // -------------------------------------------------------------------------------------------

  //
  // Calculate the common Target_Ratio among all CPU sockets to ensure that
  //  Common_Min_Ratio <= Target_Ratio <= Common_Max_Ratio.
  //

  //
  // FlexRatio is based off of ProcessorCore ratio so verify that it is within range.
  //
  if(host->setup.cpu.flexRatioNext > MAX_PROCESSOR_CORE_RATIO){
    targetRatio = DEFAULT_PROCESSOR_CORE_RATIO;
  } else {
    targetRatio = (UINT32) host->setup.cpu.flexRatioNext;
  }
  commonMaxRatio = (ReadCpuPciCfgEx (host, sbspSktId, 0, PLATFORM_INFO_N0_PCU_FUN0_REG) >> 8) & 0x000000ff;
  commonMinRatio = (ReadCpuPciCfgEx (host, sbspSktId, 0, PLATFORM_INFO_N0_PCU_FUN0_REG + 4) >> 8) & 0x000000ff;

  // Check Config TDP feature
  ConfigurableTDP = (ReadCpuPciCfgEx (host, sbspSktId, 0, PLATFORM_INFO_N0_PCU_FUN0_REG + 4) >> 1) & 0x00000003;

  if ( (ConfigurableTDP > 0) & (host->setup.cpu.ConfigTDP > 0) ) {
    // Support Config TDP, program MSR_CONFIG_TDP_CONTROL
    ConfigTDPCtrl = host->setup.cpu.ConfigTDPLevel;

    //
    if (ConfigTDPCtrl >= 1) {
      //commonMaxRatio = (ReadCpuPciCfgEx (host, sbspSktId, 0, CONFIG_TDP_LEVEL1_N0_PCU_FUN3_REG) & B_CONFIG_TDP_LVL_RATIO) >> N_CONFIG_TDP_LVL_RATIO;
      commonMaxRatio = (ReadCpuPciCfgEx (host, sbspSktId, 0, CONFIG_TDP_LEVEL1_N0_PCU_FUN3_REG) >> 16) & 0x000000ff;
      csrReg = (ReadCpuPciCfgEx (host, sbspSktId, 0, CONFIG_TDP_CONTROL_PCU_FUN3_REG) & ~(0x00000003)) + 1;
      WriteCpuPciCfgEx (host, sbspSktId, 0, CONFIG_TDP_CONTROL_PCU_FUN3_REG, csrReg);
    }

    if ((ConfigTDPCtrl == 2) &(ConfigurableTDP == 2)) {
      //commonMaxRatio = (ReadCpuPciCfgEx (host, sbspSktId, 0, CONFIG_TDP_LEVEL2_N0_PCU_FUN3_REG) & B_CONFIG_TDP_LVL_RATIO) >> N_CONFIG_TDP_LVL_RATIO;
      commonMaxRatio = (ReadCpuPciCfgEx (host, sbspSktId, 0, CONFIG_TDP_LEVEL2_N0_PCU_FUN3_REG) >> 16) & 0x000000ff;
      csrReg = (ReadCpuPciCfgEx (host, sbspSktId, 0, CONFIG_TDP_CONTROL_PCU_FUN3_REG) & ~(0x00000003)) + 2;
      WriteCpuPciCfgEx (host, sbspSktId, 0, CONFIG_TDP_CONTROL_PCU_FUN3_REG, csrReg);
    }

    ConfigTDPCtrlVal = ReadCpuPciCfgEx (host, sbspSktId, 0, CONFIG_TDP_CONTROL_PCU_FUN3_REG) & 0x00000003;
  }
  //
  
  // AptioV Server Override
		host->var.common.MaxCoreToBusRatio[sbspSktId] = (UINT8) commonMaxRatio; //common max ratio calculated for SBSP processor is done above 
 		host->var.common.MinCoreToBusRatio[sbspSktId] = (UINT8) commonMinRatio; //common min ratio calculated for SBSP processor is done above 
  // AptioV Server Override

  RatioChangeFlag = 0;

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (socket == sbspSktId)   continue;  // SBSP already done above
    if ((host->var.qpi.RasInProgress == FALSE && (host->var.common.socketPresentBitMap & (BIT0 << socket))) ||
      (host->var.qpi.RasInProgress == TRUE && host->var.qpi.RasSocId == socket)) {

         csrReg = (ReadCpuPciCfgEx (host, socket, 0, PLATFORM_INFO_N0_PCU_FUN0_REG) >> 8) & 0x000000ff;

         ConfigurableTDP = (ReadCpuPciCfgEx (host, socket, 0, PLATFORM_INFO_N0_PCU_FUN0_REG + 4) >> 1) & 0x00000003;

         if ( (ConfigurableTDP > 0) & (ConfigTDPCtrlVal > 0) ) {
             csrReg = (ReadCpuPciCfgEx (host, socket, 0, CONFIG_TDP_CONTROL_PCU_FUN3_REG) & ~(0x00000003)) + ConfigTDPCtrlVal;
             WriteCpuPciCfgEx (host, socket, 0, CONFIG_TDP_CONTROL_PCU_FUN3_REG, csrReg);

              if (ConfigTDPCtrlVal == 1) {
                  csrReg = (ReadCpuPciCfgEx (host, socket, 0, CONFIG_TDP_LEVEL1_N0_PCU_FUN3_REG) >> 16) & 0xff;
              }

              if (ConfigTDPCtrlVal == 2) {
                  csrReg = (ReadCpuPciCfgEx (host, socket, 0, CONFIG_TDP_LEVEL2_N0_PCU_FUN3_REG) >> 16) & 0xff;
              }

         }
         
		 // AptioV Server Override 
         host->var.common.MaxCoreToBusRatio[socket] = (UINT8) csrReg; // max core to bus ratio for the particular socket
       	 // AptioV Server Override 
	   
         //
         // CSR bit[15:8] = MAX_NON_TURBO_LIM_RATIO, i.e., Max non-turbo ratio
         //
         if (commonMaxRatio != csrReg) {
           RatioChangeFlag = 1;

           if (commonMaxRatio > csrReg) {
             commonMaxRatio = csrReg;

             if (host->var.qpi.RasInProgress == TRUE)
              return CPU_FAILURE;   // New socket's max ratio should be greater than or equal to that of sbsp
           }
         }
         csrReg = (ReadCpuPciCfgEx (host, socket, 0, PLATFORM_INFO_N1_PCU_FUN0_REG) >> 8) & 0x000000ff;
         //
         // CSR bit[15:8] = MAX_EFFICIENCY_RATIO, i.e., Min ratio
         //
         
		 // AptioV Server Override
         host->var.common.MinCoreToBusRatio[socket] = (UINT8) csrReg; // min core to bus ratio for the particular socket
         // AptioV Server Override
		 
         if (commonMinRatio < csrReg) {
           commonMinRatio = csrReg;

           if (host->var.qpi.RasInProgress == TRUE)
            return CPU_FAILURE; // New socket's min ratio should be less than or equal to that of sbsp
         }

      //
      // Collect CPU: Max Non-turbo Ratio and Maximum Efficiency Ratio
      //
	  // AptioV Server Override Start
      // host->var.common.MaxCoreToBusRatio[socket] = (UINT8) commonMaxRatio;
      // host->var.common.MinCoreToBusRatio[socket] = (UINT8) commonMinRatio;
	  // AptioV Server Override

   }
  }

  //
  // Now commonMaxRatio, commonMinRatio  contain the common ratio values for all CPU sockets.
  //
  if ((host->setup.cpu.flexRatioEn == 0) & RatioChangeFlag)  {
    //
    // BIOS enforces common targetRatio regardless of Setup option
    //
    targetRatio = commonMaxRatio;
  }

  if (host->setup.cpu.flexRatioEn && (targetRatio < commonMaxRatio))  {
    RatioChangeFlag = 1;
  }

  //HSD 3614888: Disabling EIST and CSTATES should leave CPUs running at P1
  if (host->setup.cpu.EnableGv == 0)  {
    RatioChangeFlag = 1;
    targetRatio = commonMaxRatio;
  }

  //
  // Validata  that targetRatio is in the right range
  //
  if (host->setup.cpu.flexRatioEn && (targetRatio > commonMaxRatio)) {
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "\ntargetRatio out-of-range. Forcing %x\n", commonMaxRatio));
#endif
    targetRatio = commonMaxRatio;
  }

  if (host->setup.cpu.flexRatioEn && (targetRatio < commonMinRatio)) {
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "\ntargetRatio out-of-range. Forcing %x\n", commonMinRatio));
#endif
    targetRatio = commonMinRatio;
  }

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if ((host->var.qpi.RasInProgress == FALSE && (host->var.common.socketPresentBitMap & (BIT0 << socket)) && RatioChangeFlag ) ||
        (host->var.qpi.RasInProgress == TRUE && host->var.qpi.RasSocId == socket && RatioChangeFlag)) {

      csrReg = (ReadCpuPciCfgEx (host, socket, 0, FLEX_RATIO_N0_PCU_FUN3_REG) >> 8) & 0x000000ff;
      MaxNonTurbo = (ReadCpuPciCfgEx (host, socket, 0, PLATFORM_INFO_N0_PCU_FUN0_REG) >> 8) & 0x000000ff;

      if ((host->var.common.cpuType == CPU_HSX) || (host->var.common.cpuType == CPU_BDX)) {
        //
        // HSX HSD 4166975 workaround
        //  Issue:             FLEX_RATIO CSR is not accessible  (will be fixed in C0 stepping).
        //  Workaround:   Use MSR 0x194 instead
        //
        csrReg = (Get_FlexRatio_Msr(host, socket) >> 8) & 0x000000ff;
      }

      if ((targetRatio > commonMaxRatio) && (MaxNonTurbo == commonMaxRatio)) {
        continue;
      }

      if ((targetRatio < commonMinRatio) && (MaxNonTurbo == commonMinRatio)) {
        continue;
      }

      // csrReg contains current FlexRatio
      //if (targetRatio != csrReg) {
      if (targetRatio != MaxNonTurbo) {
        //
        // read FLEX_RATIO CSR
        //
        csrReg = ReadCpuPciCfgEx (host, socket, 0, FLEX_RATIO_N0_PCU_FUN3_REG);
        csrReg = (csrReg & 0xfffe00ff) | (targetRatio << 8) | BIT16;
        //
        // CSR bit[15:8] = desired FLEX_RATIO, bit[16]=enable
        // program target flex ratio
        //
        WriteCpuPciCfgEx (host, socket, 0, FLEX_RATIO_N0_PCU_FUN3_REG, csrReg);
        //
        // need a reset
        //
        resetNeeded |= POST_RESET_WARM;
        //
        // HSX HSD 4166975 workaround
        //    Issue:             FLEX_RATIO CSR is not accessible  (will be fixed in C0 stepping).
        //    Workaround:   Use MSR 0x194 instead
        //
        if ((host->var.common.cpuType == CPU_HSX) || (host->var.common.cpuType == CPU_BDX)) {
          csrReg = Get_FlexRatio_Msr(host, socket);
          csrReg = (csrReg & 0xfffe00ff) | (targetRatio << 8) | BIT16;
          Set_FlexRatio_Msr (host, socket, csrReg);
        }
      }
    }
  }

  //
  // save current FLEX_RATIO CSR for S3 resume
  //
  host->nvram.cpu.flexRatioCsr = ReadCpuPciCfgEx (host, sbspSktId, 0, FLEX_RATIO_N0_PCU_FUN3_REG);

  if (resetNeeded) {
    host->var.common.resetRequired |= resetNeeded;
  }

  return CPU_SUCCESS;
}






VOID
SetActiveCoresAndSmtEnableDisable (
    struct sysHost             *host
)
/*++

Routine Description:

  Executed by SBSP only. Program DesiredCores, SMTDisable for all CPUs
Arguments:

  host:  pointer to sysHost structure on stack

Returns:

  CORE_OFF_MASK CSRs programmed
  host->var.common.resetRequired updated (if reset is needed)

--*/

{
  UINT32  CoreDisableMask[MAX_SOCKET];	//Aptio V Server Override
  UINT8   socket;
  UINT8   resetNeeded = 0;
  UINT8   smtCapable  = 0;
  UINT8   smtEnabled  = 0;
  UINT32  availCoresMask;
  UINT32  resolvedCoresMask;
  UINT32  bistResultMask;
  UINT32  activeCoreOption;

  CSR_DESIRED_CORES_PCU_FUN1_BDX_STRUCT  csrDesiredCoresCurrent, csrDesiredCoresNew;

  //
  // -----------------------------------------------------------------------------------------------------
  // Configure number of active(enabled) cores and SMT(HT) enable/disable based on input parameters
  //   - Assume that this is executed on a cold boot path (after a power good reset)
  //   - Read CSR RESOLVED_CORES_MASK[7:0] and RESOLVED_CORES_MASK2[7:0] (for IVT CPU) bitmap to determine
  //      how many/which cores are available in a CPU pacakge.
  //     (example, a bitmap of 00111111b indicates that 6 cores are available)
  //   - Use input parameter host->setup.cpu.CoreDisableMask[]  for each socket independently
  //   - Use input parameter host->setup.cpu.smtEnable and RESOLVED_CORES_MASK[24] SMT_CAPABILITY for setting smtDisable bit.
  //      Note that smt enable/disable symetry is maintained among all CPU sockets.
  // -----------------------------------------------------------------------------------------------------
  //

  //
  // skip if SSDV
  //
  
  if (host->var.common.emulation)  { return; }

//Aptio V Server Override - Start
//There are 4 mechanisms for determine how many CPUs to disable.
//  1. CoresToEnable - Setup question user requests how may cores to enable.
//  2. MeCoreDisable - ME requests how many cores to disable through remote node manager.
//  3. CoreDisableMask - Hidden setup question that may be manipulated by OEM BMC.
//  4. BIST errors
//Policy is to disable cores based upon the maximum cores between all mechnisms.
//Example: 1. 2 cores to disable. 2. 4 cores to disable. 3. 3 cores to disable. 4. 1 core to disable.
//Then policy is to disable 4 cores.
//Order of cores to disable are calculated by the following:
//1. All cores of the CoreDisableMask are disabled.
//2. The rest of cores to be disabled requested by user, if any:
//   a. All cores not disabled, with failed BIST starting at the highest core.
//   b. The rest of the cores starting at the highest core.
//3. Any core not requested to disable, but has BIST error will be disabled in Intel original code. 
  
  for (socket = 0; socket < MAX_SOCKET; socket++) {
         UINT8 CoresToEnable = host->setup.cpu.CoresToEnable[socket];   //0 = All cores.
		 
         CoreDisableMask[socket] = host->setup.cpu.CoreDisableMask[socket];
         
         //If no user requests for core disable, then skip checking. BIST errors will be checked later.
         if (CoresToEnable == 0 && host->setup.cpu.MeCoreDisable == 0 && CoreDisableMask[socket] == 0) continue;

         //Check if socket is populated or being on-lined(not supported in Grantley).
         if ((host->var.qpi.RasInProgress == FALSE && (host->var.common.socketPresentBitMap & (BIT0 << socket))) ||
            (host->var.qpi.RasInProgress == TRUE && host->var.qpi.RasSocId == socket)) {
                  availCoresMask = GetAvailableCoresMask(host, socket);  //Check how many cores supported in system.
                  resolvedCoresMask = GetResolvedCores(host, socket);    //Check which cores are currently enabled.
                  UINT32 BistResultMask;
                  UINT32 Mask;
                  UINT8  NumCores = 0;
                  UINT8  CoresToDisable;
                  UINT32 DisableMask;

                  resolvedCoresMask &= availCoresMask;
                  
                  Mask = availCoresMask;
                  while (Mask) { //Number of Cores = Number of bits set.
                     if (Mask & 1) ++NumCores;
                     Mask >>= 1;
                 }
                  
                  //If CoresToEnable is 0, then set as max number of cores.
                  //If CoresToEnable requested is greater max number of cores, set as max number of cores.
                  if (CoresToEnable == 0 || CoresToEnable > NumCores) CoresToEnable = NumCores;
                             
                 //Calcualte cores to be disabled.
                 CoresToDisable = NumCores - CoresToEnable;

                 //ME requests more cores to disable then setup, use ME.
                 if (host->setup.cpu.MeCoreDisable > CoresToDisable) CoresToDisable = host->setup.cpu.MeCoreDisable;

                 //Always leave one core enabled if all cores requested to be disabled.
                 if (CoresToDisable >= NumCores) CoresToDisable = NumCores - 1;
                 
                 //Subtract out CoresToDisable count by cores already disabled from CoreDisableMask.
                 DisableMask = CoreDisableMask[socket];
                 while (DisableMask) {
                     if ((DisableMask & 1) == 1) {
                         if (CoresToDisable > 0) --CoresToDisable;
                         else break;
                     }
                     DisableMask >>= 1;
                 }

                 //BIST from register 1 passes is changed 0 passes in BistResultMask
                 BistResultMask = (~ReadCpuPciCfgEx (host, socket, 0, BIST_RESULTS_UBOX_DFX_REG)) & resolvedCoresMask; 
                 availCoresMask &= ~BistResultMask;   //Mask of cores with BIST errors, since they will be first to be disabled.
                                  
                 //Disable user requested cores with BIST errors that wasn't disabled by CoreDisableMask already.
                 Mask = 0x80000000;  //Start with highest cores first.
                 while (BistResultMask && CoresToDisable) {
                     if (BistResultMask & Mask && (CoreDisableMask[socket] & Mask) == 0) {
                         CoreDisableMask[socket] |= Mask;    //Disable Core
                         BistResultMask &= ~Mask;    //Mark this core is disabled.
                         --CoresToDisable;           //Mark that core has been disabled.
                     }
                     Mask >>= 1;
                 }
                         
                 //Disable the rest of user requested disabled cores.
                 Mask = 0x80000000; //Start with highest cores first.
                 while (availCoresMask && CoresToDisable) {
                     if (availCoresMask & Mask && (CoreDisableMask[socket] & Mask) == 0) {
                         CoreDisableMask[socket] |= Mask;    //Disable Core
                         availCoresMask &= ~Mask;    //Mark this core is disabled.
                         --CoresToDisable;           //Mark that core has been disabled.
                     }
                     Mask >>= 1;
                 }
             }
      }
  //Aptio V Server Override - End
//APTIOV server override Grangeville  
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if ((host->var.qpi.RasInProgress == FALSE && (host->var.common.socketPresentBitMap & (BIT0 << socket))) ||
      (host->var.qpi.RasInProgress == TRUE && host->var.qpi.RasSocId == socket)) {
    
      bistResultMask    = 0;
      csrDesiredCoresNew.Data = 0;
      resetNeeded       = 0;

      availCoresMask    = GetAvailableCoresMask(host, socket);
      smtCapable        = GetSmtCapability(host, socket);
      resolvedCoresMask = GetResolvedCores(host, socket) & availCoresMask;

      csrDesiredCoresCurrent.Data = ReadCpuPciCfgEx(host, socket, 0, CSR_DESIRED_CORES_PCU_FUN1_REG);
      smtEnabled = csrDesiredCoresCurrent.Bits.smt_disable ? 0 : 1;

      if(host->setup.cpu.CheckCpuBist){
        bistResultMask = ReadCpuPciCfgEx (host, socket, 0, BIST_RESULTS_UBOX_DFX_REG) & resolvedCoresMask;
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "BIST Results = %08X\n", bistResultMask));
#endif // SERIAL_DBG_MSG

        //convert Bist fail to 1 and mask out bits that due to core already disabled through Desired_core
        bistResultMask ^= resolvedCoresMask;
        //
        // HSX-A0 workaround hsd 4166291:  Ignore (invalid) BIST result
        if ((host->var.common.cpuType == CPU_HSX) && (host->var.common.stepping == A0_REV_HSX)) {
          bistResultMask = 0;
        }
		 // BDX-A0,A1 workaround : Ignore (invalid) BIST result
		if ((host->var.common.cpuType == CPU_BDX) && (host->var.common.stepping == A0_REV_BDX)) {
        bistResultMask = 0;
		}
		
      }

      // Aptio V Server Override - Start: Use the derived CoreDisableMask[socket] from above Aptio V override instead of taking 
      // setup value again
      activeCoreOption = CoreDisableMask[socket] & availCoresMask;
      // Aptio V Server Override - End

      //
      // If BIST failure is found, create error log entry
      //
      if ( bistResultMask) {
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "  ::  S%2d BIST error found and logged\n", socket));
#endif // SERIAL_DBG_MSG
        LogError (host, ERROR_CPU_BIST, ERROR_CPU_BIST_MINOR_SOME, (socket << 24) | bistResultMask);
      }

      //
      // Check if all cores of the socket failed BIST
      //
      if ( bistResultMask == resolvedCoresMask ) {
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "::CPU on S%2d BIST errors cause all cores disabled!\n", socket));
#endif
        FatalError (host, ERROR_CPU_BIST, ERROR_CPU_BIST_MINOR_ALL);
        while (bistResultMask); // deadloop
      }

      csrDesiredCoresNew.Data = 0;
      if((activeCoreOption != csrDesiredCoresCurrent.Bits.core_off_mask) ||   //if option differs than current setting
          (bistResultMask != 0)) {                                             //Or active core BIST failed
        if(csrDesiredCoresCurrent.Bits.lock) { //no need to continue
          host->var.common.resetRequired |= POST_RESET_POWERGOOD;
          break;
        }

        csrDesiredCoresNew.Bits.core_off_mask = (activeCoreOption | bistResultMask);
        resetNeeded |= POST_RESET_WARM;

      }

      if((smtEnabled != host->setup.cpu.smtEnable) && smtCapable){  //if reg setting differs with option and HW capable
        if(csrDesiredCoresCurrent.Bits.lock) { //no need to continue
          host->var.common.resetRequired |= POST_RESET_POWERGOOD;
          break;
        }
        csrDesiredCoresNew.Bits.smt_disable = (~host->setup.cpu.smtEnable & 1);
        resetNeeded |= POST_RESET_WARM;
      }

      if(resetNeeded) {
        host->var.common.resetRequired |= resetNeeded;
      } else { // not change on core_off_mask
        csrDesiredCoresNew.Data = csrDesiredCoresCurrent.Data;
      }

      if ((csrDesiredCoresCurrent.Bits.lock == 0) &&                   // CSR is not locked (will defer locking this CSR before hand off to OS)
          (csrDesiredCoresCurrent.Data != csrDesiredCoresNew.Data)) {   // new data != current data
        //
        // Write new data to CSR CSR_DESIRED_CORES
        //
        WriteCpuPciCfgEx (host, socket, 0, CSR_DESIRED_CORES_PCU_FUN1_REG, csrDesiredCoresNew.Data);
      }

      // save current DESIRED_CORES CSR for S3 resume
      host->nvram.cpu.desiredCoresCsr[socket] = csrDesiredCoresNew.Data;
    }  //end if socket present

  } //end for socket loop

  return;
}

#ifdef IA32

VOID
CollectPbspData (
    struct sysHost             *host
)
/*++

Routine Description:

    Executed by SBSP only. Collect package BSP data that is needed to be passed along via sysHost structure

Arguments:

  host:  pointer to sysHost structure on stack

Returns:

  VOID

--*/
{
  UINT8   socket;
  UINT32  csrReg;

  //
  // Read APIC ID  and stepping from scratch pad register of each PBSP and store them into sysHost struct
  //
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (host->var.common.socketPresentBitMap & (BIT0 << socket)) {
      csrReg = ReadCpuPciCfgEx (host, socket, 0,SR_PBSP_CHECKIN_CSR);
      //
      // CSR bit[23:8] = APIC ID at reset
      //
      host->var.cpu.packageBspApicID[socket] = (csrReg >> 8) & 0xffff;

      //
      // CSR bit[30:27] = CPU Stepping
      //
      host->var.cpu.packageBspStepping[socket] = (UINT8) ((csrReg >> 27) & 0x0f);

      //
      // Clear the APIC ID and Stepping data in scratch pad CSR
      //
      csrReg &= 0x870000ff;
      WriteCpuPciCfgEx (host, socket, 0, SR_PBSP_CHECKIN_CSR, csrReg);
    }
  }

}


#if 0  // this function now defined in CpuAccess.c
VOID
GetCpuID (
    UINT32*   RegEax,
    UINT32*   RegEbx,
    UINT32*   RegEcx,
    UINT32*   RegEdx )
{
  //#ifdef IA32
  _asm {
     pushad

     mov   esi, RegEax
     mov   eax, dword ptr [esi]
     mov   esi, RegEcx
     mov   ecx, dword ptr [esi]

     cpuid

     mov   esi, RegEax
     mov   dword ptr [esi], eax
     mov   esi, RegEbx
     mov   dword ptr [esi], ebx
     mov   esi, RegEcx
     mov   dword ptr [esi], ecx
     mov   esi, RegEdx
     mov   dword ptr [esi], edx

     popad
  }
//#else
// EFI_CPUID_REGISTER cpuIdRegisters;
//  EfiCpuid (*RegEax, &cpuIdRegisters);
//  *RegEax = cpuIdRegisters.RegEax;
//  *RegEbx = cpuIdRegisters.RegEbx;
//  *RegEcx = cpuIdRegisters.RegEcx;
//  *RegEdx = cpuIdRegisters.RegEdx;
//#endif
  return;
}
#endif


VOID
ReadMsrData(
    UINT32 MsrReg,
    UINT64_STRUCT* data64
)

{
//#ifdef IA32
  _asm {
     pushad

     mov  ecx, MsrReg
     rdmsr
     mov  esi, data64
     mov  dword ptr [esi], eax
     mov  dword ptr [esi+4], edx

     popad
 }
//#else
//  {
//     UINT64 value;
//     value = EfiReadMsr(MsrReg);
//     (*data64).lo = (UINT32)value;
//     (*data64).hi = (UINT32)(value >> 32);
//  }
//#endif
 return;
}




VOID
CheckVtIfNeedReset (
    struct sysHost      *host
)
/*++

Routine Description:

  Check if VT/TXT is supported, and if a power-good reset is needed
  to unlock the MSR to allow changing the current setting.

Arguments:

  pointer to sysHost struct

Returns:

  host->var.common.resetRequired updated (if reset is needed)

--*/
{
  UINT32  RegEax;
  UINT32  RegEbx;
  UINT32  RegEcx;
  UINT32  RegEdx;
  UINT64_STRUCT  MsrIa32FeatureControl;
  BOOLEAN CurrentVmxState;

  //
  // Note all logical threads' VT/TXT feature MSRs are configured identically on cold boot path.
  // Here we check if the current config needs to be changed (i.e.,mismatch with BIOS Setup),
  // and if a power-good reset is needed to "unlock" the MSR for that change.
  //

  //
  // Check if VT is supported via CPUID Function 1, which returns
  //    ECX[6] Intel TXT support feature flag
  //    ECX[5] Intel VT  support feature flag
  //

  RegEax = 1; RegEcx = 0;
  GetCpuID (&RegEax, &RegEbx, &RegEcx, &RegEdx);
  if (!(RegEcx & BIT5)) {
    return;
  }

  ReadMsrData (MSR_IA32_FEATURE_CONTROL, &MsrIa32FeatureControl);

  //
  // MSR_IA32_FEATURE_CONTROL Definition:
  //    Bit 2   Enable VMX outside SMX operation (R/WL): This bit enables Intelr
  //            Virtualization Technology in an environment that may not include
  //            Intelr Trusted Execution Technology support.
  //
  //    Bit 1   Enable VMX inside SMX operation (R/WL): This bit enables Intelr
  //            Virtualization Technology in an environment that includes Intelr
  //            Trusted Execution Technology support.
  //
  //    Bit 0   Lock (R/WO) (1 = locked) When set, locks this MSR from being witten,
  //            writes to this bit will result in GP(0) until an S5 reset occurs.
  //            BIOS must ensure this lock bit is set before boot to OS.
  //


  //
  // If unlocked, no reset needed.
  //
  if ((MsrIa32FeatureControl.lo & BIT0) == 0) {
    return;
  }

  CurrentVmxState = FALSE;
  if ( MsrIa32FeatureControl.lo & BIT2 ) {
    CurrentVmxState = TRUE;
  }

  //
  // Need a reset only if this MSR is locked and VMX state has to be changed.
  //

  if (CurrentVmxState != (BOOLEAN)host->setup.cpu.vtEnable) {
    //
    // We need a power good reset to unlock MSR_IA32_FEATURE_CONTROL.
    //
    host->var.common.resetRequired |= POST_RESET_POWERGOOD;
  }
}
//
VOID
RestoreProcessorUncoreFrequencyOnS3Resume(
    struct sysHost * host
) 
/*++

Routine Description:

  Executed by SBSP only.
  Restore UNCORE_RATIO  for all CPUs on S3 resume path.

Arguments:

  host:  pointer to sysHost structure on stack
         host->nvram.cpu contains valid data to be restored to MSRs.

Returns:

  UNCORE_RATIO restored from sysHost->nvram.cpu structure

--*/
{
  UINT8   socket;
  UINT32 UncoreRatioMsr;


  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (host->var.common.socketPresentBitMap & (BIT0 << socket)) {
      //
      // restore Uncore RATIO MSR
      //
      UncoreRatioMsr = GetUncoreRatio_Msr(host, socket);
	 if(host->setup.cpu.UncoreFreqOverride){ //value from setup
      if (UncoreRatioMsr != host->nvram.cpu.UncoreRatioMsr) {
        //
        //BDX workaround during S3 resume to set uncore frequency
        //
        if ((host->var.common.cpuType == CPU_BDX) && (host->var.common.skuType == SKU_EP)){
          Set_UncoreRatio_Msr (host, socket, host->nvram.cpu.UncoreRatioMsr);
          }
        }
      }
    }//if
  }
}
//

VOID
RestoreProcessorCsrsOnS3Resume (
    struct sysHost             *host
)

/*++

Routine Description:

  Executed by SBSP only.
  Restore FLEX_RATIO, DESIRED_CORES CSRS for all CPUs on S3 resume path.

Arguments:

  host:  pointer to sysHost structure on stack
         host->nvram.cpu contains valid data to be restored to CSRs.

Returns:

  FLEX_RATIO, DESIRED_CORES CSRs restored from sysHost->nvram.cpu structure
  host->var.common.resetRequired updated (if reset is needed)

--*/
{
  UINT8   socket;
  UINT8   resetNeeded =0;
  UINT32  flexRatioN0PcuFun3;
  UINT32  csrDesiredCoresPcuFun1;


  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (host->var.common.socketPresentBitMap & (BIT0 << socket)) {
      //
      // restore FLEX_RATIO CSR
      //
      flexRatioN0PcuFun3 = ReadCpuPciCfgEx (host, socket, 0, FLEX_RATIO_N0_PCU_FUN3_REG);
      if (flexRatioN0PcuFun3 != host->nvram.cpu.flexRatioCsr) {
        WriteCpuPciCfgEx (host, socket, 0, FLEX_RATIO_N0_PCU_FUN3_REG, host->nvram.cpu.flexRatioCsr);
        //
        // HSX HSD 4166975 workaround
        //      Issue:               FLEX_RATIO CSR is not writeable  (will be fixed in C0 stepping).
        //      Workaround: Use MSR 0x194 instead
        //
        if ((host->var.common.cpuType == CPU_HSX) || (host->var.common.cpuType == CPU_BDX)) {
          Set_FlexRatio_Msr (host, socket, host->nvram.cpu.flexRatioCsr);
        }

        resetNeeded |= POST_RESET_WARM;
      }
      //
      // restore DESIRED_CORES CSR
      //
      csrDesiredCoresPcuFun1 = ReadCpuPciCfgEx (host, socket, 0, CSR_DESIRED_CORES_PCU_FUN1_REG);
      if (csrDesiredCoresPcuFun1 != host->nvram.cpu.desiredCoresCsr[socket]) {
        WriteCpuPciCfgEx (host, socket, 0, CSR_DESIRED_CORES_PCU_FUN1_REG, host->nvram.cpu.desiredCoresCsr[socket]);
        resetNeeded |= POST_RESET_WARM;
      }
    }
  }
  if (resetNeeded) {
    host->var.common.resetRequired |= resetNeeded;
  }
}

VOID
SetBIOSInitDone (
    struct sysHost  *host
)
/*++

Routine Description:
  Executed by System BSP only.
  Common software programming steps needed before warm reset and CPU-only reset.

Arguments:

  host - pointer to sysHost structure on stack

Note:

Do not add anything to this routine.  It is called twice for two different purposes.  It is called once prior to
doing the first rest to lock in our frequencies & ratio changes.  BIOS must provide PCODE enough information to
process any RESET_WARN flows.  The seconded time this is called is before we exit PEI.  At this point there is a
separate set of registers that must be programmed prior to BIOS_INIT_Done being set.  This second time we program
this bit Pcode is now ready to start processing PM flows.  Please note that these steps cannot be combined because
when BIOS_INIT_DONE bit is set PCODE locks the BIOS_2_PCODE MBX.

Returns:
  VOID
--*/
{

  UINT8   socket;
  BIOSSCRATCHPAD1_UBOX_MISC_STRUCT biosScratchPad1;
  UINT8 sbspSktId = GetSbspSktId(host);

  biosScratchPad1.Data = ReadCpuPciCfgEx (host, sbspSktId, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG);
  if (biosScratchPad1.Data & BIT4) {

    //
    // Set "BIOS_RESET_CPL" flag bit of all PBSP sockets
    //
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      if (socket == sbspSktId) continue;  // skip SBSP
      if (host->var.common.socketPresentBitMap & (BIT0 << socket)) {
        SetRstCpl(host, socket);
      }
    }

    //
    //  Lastly the SBSP socket
    //
    SetRstCpl(host, sbspSktId);
    //
    // Program DMICTRL...We only want to do this for SBSP... clear bits 0:1 of DMICTRL on Legacy socket only
    //
    ClearDmictrlAutoPmCplt(host, sbspSktId);
  } else {
    biosScratchPad1.Data |= BIT4;
    WriteCpuPciCfgEx (host, sbspSktId, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG, biosScratchPad1.Data);
    return;
  }

} // end routine


UINT8
GetAvailableLlcWays(
    struct sysHost   *host ,
    UINT8 socket
)

{
  UINT8 data8 = 0;

  //
  // Read cachesz of CAPID0 of the socket to determine number of LLC ways per Cbo slice
  //
  //  Fuse value   LLC Size per slice (Enabled ways per slice)
  //   '000:      0.5 M (4 lower ways)
  //   '001:      1 M (8 lower ways)
  //   '010:      1.5 M (12 lower ways)
  //   '011:      2 M (16 lower ways)
  //   '100:      2.5M (20 lower ways)
  //

  if ((host->var.common.cpuType == CPU_HSX) || (host->var.common.cpuType == CPU_BDX)) {

    CAPID0_PCU_FUN3_HSX_STRUCT  pcu_capid0;

    pcu_capid0.Data = ReadCpuPciCfgEx (host, socket, 0, CAPID0_PCU_FUN3_REG);
    //
    // HSX s4904800  capid0.llc_way_en issue w/a
    //
    if (pcu_capid0.Bits.llc_way_en == 0) {
      pcu_capid0.Bits.llc_way_en = 4;    // force '000b to '100
    }

    data8 = (UINT8) (pcu_capid0.Bits.llc_way_en +1) * 4;  // HSX does not have cachesz field?
  }

  return data8;

}


UINT8
GetAvailableSlices(
    struct sysHost   *host ,
    UINT8 socket
)

{
  UINT8 data8,i;
  UINT32 data32;

  data32 = data8 = 0;
  //
  // Read PCU CAPID5 CSR to get number of slices enabled
  //

  if (host->var.common.cpuType == CPU_BDX) {
    CAPID5_PCU_FUN3_BDX_STRUCT  pcu_capid5_Bdx;
    pcu_capid5_Bdx.Data = ReadCpuPciCfgEx (host, socket, 0, CAPID5_PCU_FUN3_REG);
    data32 = (UINT32)pcu_capid5_Bdx.Bits.llc_slice_en;
  }

  if (host->var.common.cpuType == CPU_HSX) {
    CAPID5_PCU_FUN3_HSX_STRUCT  pcu_capid5_Hsx;
    pcu_capid5_Hsx.Data = ReadCpuPciCfgEx (host, socket, 0, CAPID5_PCU_FUN3_REG);
    data32 = (UINT32)pcu_capid5_Hsx.Bits.llc_slice_en;
  }

  //
  // If a bit=1 then the slice is enabled. Count the number of '1's in the bitmask
  //
  for (i=0; i<host->var.common.MaxCoreCount; i++) {
    if (data32 & 1) {
      data8 += 1;
    }
    data32 >>= 1;
  }


  return data8;
}



UINT8
GetLlcWaysTaken (
    struct sysHost   *host ,
    UINT8 NumOfSlices
)
/*
The maximum allowable value for ocla_max_way = Number_Available_Ways minus [(One Way for C6 usage) plus (Three Ways for Isochronous usage) plus (One Way for non-Isochronous usage) plus (Number_Ways_NEM)].

For HSX:
    i.   The maximum NEM Way usage is dependent on Slice Count (i.e., number of LLC ways available)  and determined by a lookup table
    ii.  C6 does not need to use one LLC way

 */

{

  //     Table of HSX Slices v.s. NEM Ways ( Note: a 0 element means n/a)
  //  Fused Slice Count          0     1      2    3   4    5   6  7    8   9   10   11   12  13   14    15   16  17  18
  UINT8 HSX_NEM_Ways[19] = {0, 20, 10, 8, 5, 6, 6, 5, 3, 5, 4,  0,  3, 0,  4,  0,  2, 4, 3};
  UINT8 BDX_NEM_Ways[25] = {0,  0, 12,10, 6, 7, 6, 5, 3, 5, 4,  5,  3, 5,  4,  3,  2, 4, 3, 3, 3, 0, 4, 0, 2};
  UINT8 data8;

   if (host->var.common.cpuFamily == CPU_FAMILY_BDX_DE)
   {
     data8 = 1 + BDX_NEM_Ways[NumOfSlices];  // LLC ways used by non-isoch, and NEM (BDX-DE doesn't have isochronous)
   } 
   else if (host->var.common.cpuFamily == CPU_FAMILY_BDX)
   { // BDX-EP
     data8 = 3 + 1 + BDX_NEM_Ways[NumOfSlices];  // LLC ways used by isoch , non-isoch, and NEM 
   }
   else 
   { // HSX
     data8 = 3 + 1 + HSX_NEM_Ways[NumOfSlices];  // LLC ways used by isoch , non-isoch, and NEM
   }

   return data8;
}

UINT64_STRUCT
SetIotLlcConfig (
    struct sysHost  *host,
    UINT8     Socket,
    UINT8     OclaMaxTorEntry,
    UINT8     OclaLlcWay
)
/*++

  Routine Description:

   Executed by SBSP only. Call BIOS/VCU Mailbox interface to perform IOT LLC configuration setup.

  Arguments:

          host:  pointer to sysHost structure on stack
          Socket:   CPU socketID
         OclaMaxtorEntry, OclaLlcWay :   IOT LLC config parameters for the given  socket

  Returns:

     64-bit return code from the VCU mailbox:
    Lo Dword
       [15:0] =  Command Response Code (success, timeout, etc)
       [31:16] = rsvd
    Hi Dword
       [31:0] = Return data if applicable

 */

{
  UINT32           cmd32, data32;
  UINT64_STRUCT    RetCode;
  UINT64_STRUCT    RetCode_save;



  //Note: Command CSR [23:16] = Index.  Set Index=0   for all the following mailbox commands


  //
  // Open Sequence
  //
  cmd32 =  VCODE_API_OPCODE_OPEN_SEQUENCE;
  data32 = VCODE_API_SEQ_ID_IOT_LLC_SETUP;
  RetCode = WriteBios2VcuMailboxCommand(host, Socket, cmd32, data32);
  if (RetCode.lo != VCODE_MAILBOX_CC_SUCCESS)  {
    goto VcuApiError;
  }

  //
  // Set Parameters      bit[15:8] = MaxTorEntries, bit[7:0] = Number of LLC ways
  //
  cmd32 =  VCODE_API_OPCODE_SET_PARAMETER;
  data32 = (OclaMaxTorEntry << 8) | OclaLlcWay;
  RetCode = WriteBios2VcuMailboxCommand(host, Socket, cmd32, data32);
  if (RetCode.lo != VCODE_MAILBOX_CC_SUCCESS)  {
    goto VcuApiError;
  }

  //
  // Send IOT_LLC_SETUP command
  //
  cmd32 =  VCODE_API_OPCODE_IOT_LLC_SETUP;
  data32 = 0;
  RetCode = WriteBios2VcuMailboxCommand(host, Socket, cmd32, data32);
  if (RetCode.lo != VCODE_MAILBOX_CC_SUCCESS)  {
    goto VcuApiError;
  }

  RetCode_save = RetCode;     // save return code of IOT_LLC_SETUP API call

  //
  // Close Sequence
  //
  cmd32 =  VCODE_API_OPCODE_CLOSE_SEQUENCE;
  data32 = VCODE_API_SEQ_ID_IOT_LLC_SETUP;
  RetCode = WriteBios2VcuMailboxCommand(host, Socket, cmd32, data32);

  RetCode = RetCode_save;     // prepare return code
  return RetCode;


  VcuApiError:

  return RetCode;

}




VOID
Config_IOT_LLC (
    struct sysHost             *host
)
/*++

Routine Description:

 Executed by SBSP only. Program IOT/OCLA config based on Setup data.

 This function  reserves some of the LLC ways for the IOT while still in NEM mode which also uses LLC for CAR.
 All CBOs of a CPU socket will be programmed the same way.

 It's possible that the IOT ways and the NEM ways will end up overlapping because the LLC may have allocated
 one of the NEM cache lines into the way that you reserved for the IOT since some of those allocations will have
 occurred before the Cbo knew which ways were reserved for the IOT.

 But hits to those LLC ways will still be handled correctly even after those ways have been reserved for the IOT.
 So the data that NEM cached will not be lost, and NEM will still function. We just have to make sure that IOT
 doesn't start debug trace fill into the IOT reserved ways until after NEM mode is completed. Validation team
 agrees to this.

 Since these OCLA config registers are sticky. we program them only on power-good reset path, assuming  no
 IOT/OCLA trace will occur until a warm reset takes place. We leave these registers alone on the warm reset path.
 Every time the related Setup option get changed and saved, a Power-Good reset is expected.

Arguments:

  host:  pointer to sysHost structure on stack

Returns:

  host->var.common.resetRequired updated (if reset is needed)

 */

{
  UINT8   socket;
  UINT8   NumOfLlcWays, NumOfSlices, LlcWaysTaken, OclaMaxTorEntry,OclaLlcWay;
  UINT8   resetNeeded = 0;
  UINT64_STRUCT   RetCode;
  BOOLEAN DoNothing = FALSE;

  //
  // Do nothing if not HSX or BDX CPU
  //
  if ((host->var.common.cpuType != CPU_HSX) && (host->var.common.cpuType != CPU_BDX))
    DoNothing = TRUE;

  //
  // Do nothing if running on emulation (SSDV)
  //
  if (host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG))
    DoNothing = TRUE;

  //HSX TODO:   use API to detect if IOT config is needed or ITP override is in effect?

  if (DoNothing)
    return;

  //
  // Loop  thru all sockets
  //
  for ( socket=0; socket<MAX_SOCKET; socket++ )   {
    if ((host->var.common.socketPresentBitMap & (BIT0 << socket)) == 0)
      continue;
    // skip if IOT config is disabled in Setup
    if (host->setup.cpu.IotEn[socket] == 0)
      continue;

    NumOfLlcWays = GetAvailableLlcWays(host, socket);
    NumOfSlices =  GetAvailableSlices(host, socket);
    LlcWaysTaken = GetLlcWaysTaken(host, NumOfSlices);

    //
    // Validate the Setup data, skip if data is invalid
    //

    OclaMaxTorEntry = host->setup.cpu.OclaTorEntry[socket];
    OclaLlcWay = host->setup.cpu.OclaWay[socket];

    if  (OclaMaxTorEntry > OCLA_TOR_ENTRY_MAX ) {
      continue;
    }

    if  (OclaLlcWay > (UINT8)(NumOfLlcWays - LlcWaysTaken))  {
      continue;
    }

    //
    // Call VCU Mailbox API to set up IOT LLC config
    //
    RetCode = SetIotLlcConfig(host, socket, OclaMaxTorEntry, OclaLlcWay);

    if (RetCode.lo != VCODE_MAILBOX_CC_SUCCESS){
#ifdef SERIAL_DBG_MSG
      rcPrintf ((host, "::VCU API return-code indicates error occured\n"));
#endif
    }

    if ((RetCode.lo == VCODE_MAILBOX_CC_SUCCESS) && (RetCode.hi == 1)) {
      if (((host->var.common.cpuType == CPU_HSX) && (host->var.common.stepping > A0_REV_HSX)) || (host->var.common.cpuType == CPU_BDX)) {  // skip if HSX-A0 that causes endless resets
        //
        // IOT/LLC config setting has changed. A warm reset is needed
        //
        resetNeeded |= POST_RESET_WARM;
      }
    }

  }

  if (resetNeeded)  {
    host->var.common.resetRequired |= resetNeeded;
  }

}



VOID
CpuInit (
    struct sysHost             *host
)
/*++

Routine Description:

  Executed by SBSP only. Configure CPU features that require a reset to take effect

Arguments:

  host:  pointer to sysHost structure on stack

Returns:

  VOID
  Related CPU CSRs are programmed as needed.
  host->var.common.resetRequired is updated as needed

--*/
{
	UINT8   CpuSkuType, CpuSubSkuType, socket = 0;
	UINT8 UncoreFreq = 16; 
	
		

  GetSocketSkuType(host, socket, &CpuSkuType, &CpuSubSkuType);

  //
  // read package BSP data into sysHost structure
  // (can only be done after CPU topology has been discovered by QPI RC )
  //
  CollectPbspData (host);

  if (host->var.common.bootMode == S3Resume) {    //S3 resume path

    RestoreProcessorUncoreFrequencyOnS3Resume(host); //uncore frequency changes during S3

    RestoreProcessorCsrsOnS3Resume(host);
  }

  else {                            // normal boot path

    CheckVtIfNeedReset(host);
    if (host->var.common.resetRequired & POST_RESET_POWERGOOD) {
      return;
    }

    ProgramProcessorFlexRatio(host);
    //
    //Uncore ratio pogramming for BDX-EP A0 only as A0 is having issues with higher uncore ratio
    //
    if(host->setup.cpu.UncoreFreqOverride){//value from setup
	 UncoreFreq = host->setup.cpu.UncoreFreq;
	 ProgramProcessorUncoreRatio(host,UncoreFreq);
  	}else { //AUTO,fuse values,V0-->1.6,ML A1 workaround-->Si workaround HSD#2492518
  		if ((((host->var.common.cpuType == CPU_BDX) && (host->var.common.stepping == C0_REV_BDX) && (CpuSkuType == SKU_DE || CpuSkuType == SKU_NS) 
			&&((host->var.common.procCom[socket].capid1 & BIT20)==0)))){
			  ProgramProcessorUncoreRatio(host,UncoreFreq);
  			}else if((host->var.common.cpuType == CPU_BDX) && (host->var.common.stepping == A0_REV_BDX) && (host->var.common.skuType == SKU_EP) && (host->var.common.procCom[socket].capid1 & BIT20)){
			 //ML A1 workaround-->Si workaround HSD#2492518
				ProgramProcessorUncoreRatio(host,UncoreFreq);
			}
  	}
    SetActiveCoresAndSmtEnableDisable(host);
  }

  Config_IOT_LLC (host);
  //IOTG changes
   IOTGInitChanges(host);
  //
  //
  // CPU MISC configuration
  //
  CpuInitMISC (host);
}


VOID
PpinInit (
    struct sysHost     *host
)
/*++

Routine Description:

  Configure PPIN MSR on all sockets

Arguments:

  host:  pointer to sysHost structure on stack

Returns:

  VOID
--*/

{
  UINT64_STRUCT msrReg;
  UINT8         PpinEnable = 0;
  UINT8         socket;

  //
  // To enable PPIN, Bit1 of PPIN_CTRL_MSR needs to be set to 1
  //
  if(host->setup.cpu.PpinControl) {
    PpinEnable = 0x02;
  }
  else {
// AptioV override start: Lock the PPIN_CNTL_MSR if PPIN is disabled in setup.  
//  PpinEnable = 0;
    PpinEnable = 1;
// AptioV override end: Lock the PPIN_CNTL_MSR if PPIN is disabled in setup.  
  }

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (host->var.common.socketPresentBitMap & (BIT0 << socket)) {

      //
      // Check if CPU supports PPIN
      //
      msrReg = ReadMsrPipe(host, socket, MSR_PLATFORM_INFO);
      if((msrReg.lo & BIT23) == 0)  continue; // CPU is not PPIN capable

      //
      // Check if PPIN_CTRL MSR is locked
      //
      msrReg = ReadMsrPipe(host, socket, MSR_PPIN_CTL);
      if (msrReg.lo & BIT0)  continue;  // PPIN_CTRL MSR is locked

      //
      // Program PPIN_CTRL MSR
      //
      msrReg.lo = (msrReg.lo & 0xFFFFFFFC) | PpinEnable;
      WriteMsrPipe(host, socket, MSR_PPIN_CTL, msrReg);
    }
  }
}

VOID
CpuInitMISC (
    struct sysHost             *host
)
/*++

Routine Description:

  Executed by SBSP only. Configure CPU registers that are required to be configured for PCODE usage prior to setting BIOS_INIT_Done.

Arguments:

  host:  pointer to sysHost structure on stack

Returns:

  VOID
  Related CPU CSRs are programmed as needed.
  host->var.common.resetRequired is updated as needed

--*/
{

  UINT8   socket;
  //TODO: Needs to sync up GetSbspSktId() with EX
  UINT8   sbspSktId = GetSbspSktId(host);
  DMICTRL_N0_IIO_PCIEDMI_STRUCT DMICTRL;
  UINT32 TempData = 0;
  UINT32 PcodeMailboxStatus = 0;

  //
  // Enable or disable PPIN
  //
  PpinInit (host);

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (host->var.common.socketPresentBitMap & (BIT0 << socket)) {

      //
      // Program DMICTRL...We only want to do this for SBSP... clear bits 0:1 of DMICTRL on Legacy socket only
      //
      if (socket == sbspSktId) {
        DMICTRL.Data = ReadCpuPciCfgEx (host, socket, 0, DMICTRL_N0_IIO_PCIEDMI_REG);
        DMICTRL.Bits.abort_inbound_requests = 0;
        //BIOS Done-Fix DMICTRL.Bits.auto_complete_pm = 0;
        WriteCpuPciCfgEx (host, socket, 0, DMICTRL_N0_IIO_PCIEDMI_REG, DMICTRL.Data);
      } // if socket 0


      if (host->var.common.cpuFamily == CPU_FAMILY_IVT) {
        //
        // Program SAPMCTL... clear Bits 29, 27 and 24 of SAPMCTL on all sockets
        //
        TempData = ReadCpuPciCfgEx (host, socket, 0, SAPMCTL_PCU_FUN1_REG);
        TempData &= ~(BIT29 | BIT27 | BIT24);   // qpi_l0s_pll_sen_enable, iio_l0s_pll_sen_enable, qpi2_l0s_pll_sen_enable
        WriteCpuPciCfgEx (host, socket, 0, SAPMCTL_PCU_FUN1_REG, TempData);
      }

      if (!(host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG))) {
        OemPostCpuInit (host, socket);
      }

      if (!(host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG))) {
        //4164479 - Power Management: UFS (Separate uncore frequency)
        PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG, 0);
        if (PcodeMailboxStatus == 0) {
          TempData = ReadCpuPciCfgEx (host, socket, 0, BIOS_MAILBOX_DATA_PCU_FUN1_REG);
        }

        if (host->setup.cpu.UFSDisable) {
          //
          // Program MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG (0x6), Bit[28] = UFS Disable
          //
          TempData &= ~BIT28;
          TempData |= (host->setup.cpu.UFSDisable << 28);
          PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, (UINT32) MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG, TempData);
          PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG, 0);
          if (PcodeMailboxStatus == 0) {
            TempData = ReadCpuPciCfgEx (host, socket, 0, BIOS_MAILBOX_DATA_PCU_FUN1_REG);
          }

        }
      }
    } // end check if socket present
  } // end for{} loop

}


VOID
SetRstCpl (
    struct sysHost     *host,
    UINT8              socket
)
/*++

Routine Description:

  Executed by SBSP only. Program BIOS_RESET_CPL register.

Arguments:

  host:  pointer to sysHost structure on stack

Returns:

  VOID
  Related CPU CSRs are programmed as needed.
--*/

{
  BIOS_RESET_CPL_PCU_FUN1_STRUCT BIOSRESETCPL;

  BIOSRESETCPL.Data = ReadCpuPciCfgEx (host, socket, 0, BIOS_RESET_CPL_PCU_FUN1_REG);
  BIOSRESETCPL.Bits.rst_cpl1 = 1;
  WriteCpuPciCfgEx (host, socket, 0, BIOS_RESET_CPL_PCU_FUN1_REG,BIOSRESETCPL.Data);
  if (!(host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG))) {//NS emulation //| SIMICS_FLAG
    while (BIOSRESETCPL.Bits.pcode_init_done1 != 1) {
      BIOSRESETCPL.Data = ReadCpuPciCfgEx (host, socket, 0, BIOS_RESET_CPL_PCU_FUN1_REG);
    }  //wait for PCU acknowledgement
  }
}


VOID
ClearDmictrlAutoPmCplt (
    struct sysHost      *host,
    UINT8               socket
)
/*++

Routine Description:

  Executed by SBSP only. cLEAR dmictrl.AUTO_COMPLETE_PM

Arguments:

  host:  pointer to sysHost structure on stack

Returns:

  VOID
  Related CPU CSRs are programmed as needed.
--*/
{
  DMICTRL_N0_IIO_PCIEDMI_STRUCT DMICTRL;

  DMICTRL.Data = ReadCpuPciCfgEx (host, socket, 0, DMICTRL_N0_IIO_PCIEDMI_REG);
  DMICTRL.Bits.auto_complete_pm = 0;
  WriteCpuPciCfgEx (host, socket,  0, DMICTRL_N0_IIO_PCIEDMI_REG, DMICTRL.Data);
}
//IOTG changes
VOID 
IOTGInitChanges (
    struct sysHost  *host
)
/*++

Routine Description:
  Executed by System BSP only.

Arguments:

  host - pointer to sysHost structure on stack

Note:

Do not add anything to this routine.  It is called twice for two different purposes.  It is called once prior to
doing the first to set scratch pad register bit to be used by IOTG pcode feature.  BIOS must provide PCODE enough information to
process any RESET_WARN flows.  The seconded time this is called is before we exit PEI.  

Returns:
  VOID
--*/
{

  UINT8   socket;
  BIOSSCRATCHPAD1_UBOX_MISC_STRUCT biosScratchPad1;
   if (host->setup.cpu.CpuIOTGEnable) { //if enabled in setup.
	for (socket = 0; socket < MAX_SOCKET; socket++) {
      if (host->var.common.socketPresentBitMap & (BIT0 << socket)) {
		biosScratchPad1.Data = ReadCpuPciCfgEx (host, socket, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG);
		biosScratchPad1.Data |= 0x200;
		WriteCpuPciCfgEx (host, socket, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG, biosScratchPad1.Data);
		biosScratchPad1.Data = ReadCpuPciCfgEx (host, socket, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG);
		
	  }
	}
  }
}	
	//
#endif // IA32
