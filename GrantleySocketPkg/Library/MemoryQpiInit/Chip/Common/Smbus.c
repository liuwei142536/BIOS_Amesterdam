/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
--*/
/*************************************************************************
 *
 * Reference Code
 *
 * ESS - Enterprise Silicon Software
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2006 - 2015 Intel Corporation All Rights Reserved.
 *
 * The source code contained or described herein and all documents
 * related to the source code ("Material") are owned by Intel
 * Corporation or its suppliers or licensors. Title to the Material
 * remains with Intel Corporation or its suppliers and licensors.
 * The Material contains trade secrets and proprietary and confidential
 * information of Intel or its suppliers and licensors. The Material
 * is protected by worldwide copyright and trade secret laws and treaty
 * provisions.  No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Intel's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other
 * intellectual property right is granted to or conferred upon you
 * by disclosure or delivery of the Materials, either expressly,
 * by implication, inducement, estoppel or otherwise. Any license
 * under such intellectual property rights must be express and
 * approved by Intel in writing.
 *
 ************************************************************************/

#include "SysFunc.h"
#define SPD_ARRAY_SIZE  512

#pragma warning(disable : 4740)
#pragma warning(disable : 4702)



//
// Local Prototypes
//
VOID   ResetSmb(PSYSHOST host, UINT8 socket, struct smbDevice dev, UINT8 byteOffset, UINT8 *data);
UINT32 ReadSmbWorkerFunction(PSYSHOST host, UINT8 socket, struct smbDevice dev, UINT16 byteOffset, UINT8 *data);
UINT32 WriteSmbWorkerFunction(PSYSHOST host, UINT8 socket, struct smbDevice dev, UINT8 byteOffset, UINT8 *data);


#ifdef RC_SIM
void ThreadReleaseGenMutex (void);
void ThreadAcquireGenMutex (void);
#endif // RC_SIM



  /*++

  Determines if Semaphore is needed to access SMBus
    
    If SBSP is trying to access local SMBus, Semaphore is NOT needed
           Access SMBus as normal
    
    If Smbus target socket, which is not SBSP, is trying to access AP SMBus, Semaphore IS needed
           Get semaphore for AP SMBus access for target AP using 
           that AP's unique SYSTEMSEMAPHORE0_UBOX_MISC_REG
    


  @param host        - Pointer to the system host (root) structure
  @param socket      - Processor socket ID
--*/

void
IsSmbusSemaphoreNeeded (
                       PSYSHOST    host,
                       UINT8       socket
                        )
{

  //
  // Current thread is SBSP
  //
  if (host->var.common.socketId == host->nvram.common.sbspSocketId) {
    if (socket != host->nvram.common.sbspSocketId) {
      //
      // Smbus target socket wants to access AP SMBUS, semaphore needed
      //
      host->var.common.smbSync = 0x01;
      GetSmbus (host, socket);
    } else {
      //
      // SMBUS target socket is SBSP, semaphore not needed
      //
      host->var.common.smbSync = 0x0;
    }
  } else {
    //
    // Current thread is not SBSP
    //
    if (socket == host->var.common.socketId) {
      //
      // Smbus target socket wants to access AP SMBUS, semaphore needed
      //
      host->var.common.smbSync = 0x01;
      GetSmbus (host, socket);
    } else {
      //
      // Smbus target thread does not equal current thread
      //
      host->var.common.smbSync = 0x0;
      RC_ASSERT (socket != host->var.common.socketId, ERR_RC_INTERNAL, 0);
    }
  }

} // IsSmbusSemaphoreNeeded


UINT8
GetSmbus (
         PSYSHOST    host,
         UINT8       socket
         )
  /*++

  Get control of a local AP system semaphore for the calling socket
  This should be used with a matching ReleaseSmbus() to gain
  exclusive ownership of a local AP system semaphore for a section of
  code that blocks/yields to other sockets
  host->var.common.smbusDepth is used to handle the nested calls to this function

  @param host  - Pointer to the system host (root) structure

  @retval 1 - success

--*/
{

  if ((host->setup.mem.options & MULTI_THREAD_MRC_EN) && (host->var.common.smbSync)) {

    if (host->var.common.smbusDepth == 0) {

#if defined(RC_SIM) && (MAX_SOCKET > 1)
      ThreadAcquireGenMutex();
#else
      AcquireReleaseAPSystemSemaphore (host, socket, SYSTEMSEMAPHORE0, ACQUIRE_SEMAPHORE);
#endif
    }

    // increment nested call depth
    host->var.common.smbusDepth++;

    // if count rollover, halt on error
    if (host->var.common.smbusDepth == 0)  {
      HaltOnError(host, 0, 0);
    }
  }
  return 1;
} //GetSmbus


void
ReleaseSmbus (
             PSYSHOST    host,
             UINT8       socket
             )
/*++

  Release ownership of a local AP system semaphore.
  Must be used to match GetSmbus()

  @param host  - Pointer to the system host (root) structure

  @retval N/A

--*/
{


  if ((host->setup.mem.options & MULTI_THREAD_MRC_EN) && (host->var.common.smbSync)) {

    if (host->var.common.smbusDepth == 1) {

#if defined(RC_SIM) && (MAX_SOCKET > 1)
      ThreadReleaseGenMutex();
#else
      AcquireReleaseAPSystemSemaphore (host, socket, SYSTEMSEMAPHORE0, RELEASE_SEMAPHORE);
#endif // RC_SIM
    }

    // decrement nested call depth
    if (host->var.common.smbusDepth != 0) {
      host->var.common.smbusDepth--;
    }
  }

} // ReleaseSmbus


void
AcquireReleaseAPSystemSemaphore (
   PSYSHOST  host,
   UINT8     socket,
   UINT8     LocAPSemaphoreNum,
   UINT8     OpCode
   )
/*++
  Accquire or Release the AP system semaphore
  Called by the target socket
  Used for ensuring exclusive access to resources among sockets
       IMPORTANT:
           This function must be called in the sequence below:
                ......
              AcquireReleaseAPSystemSemaphore (host, socket, SYSTEMSEMAPHORE0, ACQUIRE_SEMAPHORE);    //  Acquire ownership of semaphore
                do_something();                                                                       //  Perform intended task
              AcquireReleaseAPSystemSemaphore (host, socket, SYSTEMSEMAPHORE0, RELEASE_SEMAPHORE);    //  Release ownership of semaphore
                ......

  @param host      - Pointer to the system host structure
  @param socket    - target socket
  @param LocAPSemaphoreNum,   - SYSTEMSEMAPHORE register number (0 or 1)
  @param OpCode - 1/0   ACQUIRE_SEMAPHORE / RELEASE_SEMAPHORE

  @retval  None

--*/
{
  UINT32                                RegAddr;
  UINT8                                 MySocId;
  SYSTEMSEMAPHORE0_UBOX_MISC_STRUCT     Reg32;

  MySocId = host->var.common.socketId;

  if (LocAPSemaphoreNum == SYSTEMSEMAPHORE0) {
    RegAddr = SYSTEMSEMAPHORE0_UBOX_MISC_REG;
  } else {
    RegAddr = SYSTEMSEMAPHORE1_UBOX_MISC_REG;
  }

  //
  // Read SYSTEMSEMAPHORE0 CSR on target socket
  //
  Reg32.Data = ReadCpuPciCfgEx (host, socket, 0, RegAddr);

  //
  // Set requesternode field
  //
  Reg32.Bits.requesternode = MySocId;

  if (OpCode == RELEASE_SEMAPHORE)   {
    //
    // Release Semaphore -  Write requesternode = socket, and bus_aqu_release = 0
    //
    Reg32.Bits.bus_aqu_release = 0;
    WriteCpuPciCfgEx (host, socket, 0, RegAddr, Reg32.Data);
    host->var.common.SmbusAPSemaphoreAcquired = 0;
  }

  if (OpCode == ACQUIRE_SEMAPHORE)   {
    //
    // Accquire Semaphore - Write requesternode = socket, and bus_aqu_release = 1
    //
    Reg32.Bits.bus_aqu_release = 1;
    WriteCpuPciCfgEx (host, socket, 0, RegAddr, Reg32.Data);
    //
    // Poll until semaphore ownership is granted
    //
    do {
      Reg32.Data = ReadCpuPciCfgEx (host, socket, 0, RegAddr);
    }
    while (!((Reg32.Bits.bus_aqu_release == 1) && (Reg32.Bits.currentnode == MySocId)));
    host->var.common.SmbusAPSemaphoreAcquired = 1;
  }
}

void
InitSmb (
  PSYSHOST host,
  UINT8    socket
  )
/*++

  Initialize socket SMBUS controller

  @param host    - Pointer to the system host (root) structure
  @param socket  - processor socket ID

  @retval N/A

--*/
{
#ifndef  YAM_ENV
  UINT32                        smbCntlReg;
  UINT32                        smbCmdReg;
  UINT32                        smbStatReg;
  UINT8                         busSeg;
  UINT8                         ch;
  UINT8                         mcId;
  UINT8                         bclk;
  UINT32                        base;
  UINT32                        enable;
  SMB_STAT_0_MC_MAIN_STRUCT     smbStat;
  SMB_PERIOD_CFG_MC_MAIN_STRUCT smbPeriod;
  MC_BIOS_REQ_PCU_FUN1_STRUCT   mcBiosReq;
  DDRCRSPDCFG2_MCIO_DDRIOEXTMCC_STRUCT    ddrCrSpdCfg2;
  UINT8                         McId = 0;
  SMBCNTL_0_MC_MAIN_HSX_BDX_STRUCT        smbCntl;
  MC_INIT_STATE_G_MC_MAIN_HSX_BDX_STRUCT  mcInitStateG;
  ddrCrSpdCfg2.Data = 0;

  OutputExtendedCheckpoint((host, STS_DIMM_DETECT, SUB_INIT_SMB, socket));

  if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot)) {
    //
    // Assert reset
    //
    mcInitStateG.Data = MemReadPciCfgMC (host, socket, 0, MC_INIT_STATE_G_MC_MAIN_REG);
    mcInitStateG.Bits.ddr_reset = 0;
    MemWritePciCfgMC (host, socket, 0, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);
    if(Is2HA(host)){
      MemWritePciCfgMC (host, socket, 1, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);
    }
  }

  //
  // Get current DCLK ratio
  //
  mcBiosReq.Data = ReadCpuPciCfgEx (host, socket, 0, MC_BIOS_REQ_PCU_FUN1_REG);

  //
  // Initialize the smbCntlDti variable
  //
  host->var.mem.socket[socket].smbCntlDti[0] = 0;
  host->var.mem.socket[socket].smbCntlDti[1] = 0;

  if (!(host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG))) {
    if (mcBiosReq.Bits.req_data == 0) {
#ifdef SERIAL_DBG_MSG
      if (checkMsgLevel(host, SDBG_MAX)) {
        rcPrintf ((host, "MC_BIOS_REQ.REQ_DATA = 0\n"));
      }
#endif // SERIAL_DBG_MSG
      //
      // Default to 1067
      //
      mcBiosReq.Bits.req_data = 8;

      //OutputWarning (host, WARN_IMC_DISABLED, 0, socket, 0, 0, 0);
    }
  }
  //
  // Set the SMB period to 100KHz in units of DCLK (rounding down to an even number)
  //
  for (mcId = 0; mcId < MAX_IMC; mcId++) {
    if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

    smbPeriod.Data = MemReadPciCfgMC (host, socket, mcId, SMB_PERIOD_CFG_MC_MAIN_REG);


    //
    // Get the period to use based on the current DCLK ratio
    //
    if (mcBiosReq.Bits.req_type){
      bclk = 100;
    } else {
      bclk = 133;
    }
    if(host->setup.mem.bclk) {
      bclk = (bclk * host->setup.mem.bclk) / 100;
    }
    smbPeriod.Bits.smb_clk_prd = mcBiosReq.Bits.req_data * 5 * bclk;
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "IMC %d SMB Clock Period = 0x%x\n", mcId, smbPeriod.Bits.smb_clk_prd));
    MemWritePciCfgMC (host, socket, mcId, SMB_PERIOD_CFG_MC_MAIN_REG, smbPeriod.Data);
  } // mcId loop

  //
  // HSD 4166509: Enable ddrcrspdcfg2 Smbus bit
  //
  if (host->var.common.cpuType == CPU_HSX) {
    if(Is2HA(host)){
      for (ch = 0; ch < MAX_CH; ch += 2){
        ddrCrSpdCfg2.Data = MemReadPciCfgEp(host, socket, ch, DDRCRSPDCFG2_MCIO_DDRIOEXTMCC_REG);
        ddrCrSpdCfg2.Bits.crspdcfg2 = (1 << 14);
        MemWritePciCfgEp(host, socket, ch, DDRCRSPDCFG2_MCIO_DDRIOEXTMCC_REG, ddrCrSpdCfg2.Data);
      }
    } else {
      ddrCrSpdCfg2.Data = MemReadPciCfgEp(host, socket, 0, DDRCRSPDCFG2_MCIO_DDRIOEXTMCC_REG);
      ddrCrSpdCfg2.Bits.crspdcfg2 = (1 << 14);
      MemWritePciCfgEp(host, socket, 0, DDRCRSPDCFG2_MCIO_DDRIOEXTMCC_REG, ddrCrSpdCfg2.Data);
    }
  } else { // BDX
    for (ch = 0; ch < MAX_CH; ch += 2){
      ddrCrSpdCfg2.Data = MemReadPciCfgEp(host, socket, ch, DDRCRSPDCFG2_MCIO_DDRIOEXTMCC_REG);
      if (ch == 0) {
        ddrCrSpdCfg2.Bits.crspdcfg2 = 0x56B0;
      } else {
        ddrCrSpdCfg2.Bits.crspdcfg2 = 0x16B0;
      }
      MemWritePciCfgEp(host, socket, ch, DDRCRSPDCFG2_MCIO_DDRIOEXTMCC_REG, ddrCrSpdCfg2.Data);
    }
  } // if HSX/BDX

  for (busSeg = 0; busSeg < 2; busSeg++) {
    if ((busSeg == 0) || (Is2HA(host))) {

      smbCmdReg   = SMBCMD_0_MC_MAIN_REG;
      smbStatReg  = SMB_STAT_0_MC_MAIN_REG;
      smbCntlReg  = SMBCNTL_0_MC_MAIN_REG;
    } else {
      smbCmdReg   = SMBCMD_1_MC_MAIN_REG;
      smbStatReg  = SMB_STAT_1_MC_MAIN_REG;
      smbCntlReg  = SMBCNTL_1_MC_MAIN_REG;
    }

    if (Is2HA(host)){
      McId = busSeg;
    } else {
      McId = 0;
    }
    //
    // Check if the first controller is busy
    //
    smbStat.Data = MemReadPciCfgMC (host, socket, McId, smbStatReg);
    if (smbStat.Bits.smb_busy) {
      smbCntl.Data = MemReadPciCfgMC (host, socket, McId, smbCntlReg);

      smbCntl.Bits.smb_dis_wrt = 0;

      //
      // Override the clock
      //
      smbCntl.Bits.smb_ckovrd = 0;

      //
      // Issue a soft reset
      //
      smbCntl.Bits.smb_soft_rst = 1;
      MemWritePciCfgMC (host, socket, McId, smbCntlReg, smbCntl.Data);
      //
      // Wait 35ms
      //
      FixedDelay (host, 35000);
      smbCntl.Bits.smb_ckovrd   = 1;
      smbCntl.Bits.smb_soft_rst = 0;
      MemWritePciCfgMC (host, socket, McId, smbCntlReg, smbCntl.Data);
    }

    //
    // Make Sure TSOD Polling is disabled
    //
    smbCntl.Data                  = MemReadPciCfgMC (host, socket, McId, smbCntlReg);
    smbCntl.Bits.smb_tsod_poll_en = 0;
    smbCntl.Bits.smb_dis_wrt = 0;

    //
    // disable TSOD polling and enable SPDCMD accesses
    //
    MemWritePciCfgMC (host, socket, McId, smbCntlReg, smbCntl.Data);
  }
  //
  // Initialize the PCH SMBUS
  //
  base = ReadPciCfg (host, 0, REG_ADDR (LPC_BUS, LPC_DEVICE, PCH_SMBUS_FUNC, R_PCH_SMBUS_BASE, sizeof (UINT32)));
  enable = ReadPciCfg (host, 0, REG_ADDR (LPC_BUS, LPC_DEVICE, PCH_SMBUS_FUNC, R_PCH_SMBUS_PCICMD, sizeof (UINT8)));
  if(enable & B_PCH_SMBUS_PCICMD_IOSE) {
    host->var.common.smbBase = (UINT16)(base & B_PCH_SMBUS_BASE_BAR);
  } else {
    host->var.common.smbBase = ICH_SMBBASE_ADDR;
    WritePciCfg(host, 0, REG_ADDR (LPC_BUS, LPC_DEVICE, PCH_SMBUS_FUNC, R_PCH_SMBUS_BASE, sizeof (UINT32)), host->var.common.smbBase | BIT0);
    WritePciCfg(host, 0, REG_ADDR (LPC_BUS, LPC_DEVICE, PCH_SMBUS_FUNC, R_PCH_SMBUS_PCICMD, sizeof (UINT8)), (UINT8)enable | B_PCH_SMBUS_PCICMD_IOSE);
    WritePciCfg(host, 0, REG_ADDR (LPC_BUS, LPC_DEVICE, PCH_SMBUS_FUNC, R_PCH_SMBUS_HOSTC, sizeof (UINT8)), B_PCH_SMBUS_HOSTC_HST_EN);
  }
#endif //YAM_ENV
}


UINT8 SendPchSmbCmd (
  PSYSHOST host,
  UINT16 byteOffset,
  UINT8 slaveAddress,
  UINT8 protocolStart,
  UINT8 *data
  )
/*++

  Initialize socket SMBUS controller

  @param host  - Pointer to the system host (root) structure
  @param socket  - processor socket ID

  @retval N/A

--*/
{
  UINT8   status;
  UINT32  startCount;
  UINT8   index;
  UINT8   sizeInBytes;

  status = SUCCESS;

  if (!(host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG))) {
    // Wait for host not busy
    startCount = GetCount(host);
    do {
      status = InPort8(host, host->var.common.smbBase + HOST_STATUS_REGISTER);
      if (!(status & HST_STS_HOST_BUSY)) break;

    } while (GetDelay(host, startCount) < SMB_TIMEOUT);
  } else {
    status = InPort8(host, host->var.common.smbBase + HOST_STATUS_REGISTER);
  }
  //
  // Issue the command
  //
  if (!(status & (HST_STS_HOST_BUSY | HST_STS_FAILED))) {
    OutPort8(host,(host->var.common.smbBase + HOST_STATUS_REGISTER), SMBUS_B_HSTS_ALL);  // Clear all status bits
    OutPort8(host,(host->var.common.smbBase + HOST_COMMAND_REGISTER), (UINT8)byteOffset);       // Byte number
    OutPort8(host,(host->var.common.smbBase + XMIT_SLAVE_ADDRESS_REGISTER), slaveAddress);  // Slave address
    // Initialize write data
    if (!(slaveAddress & SMBUS_B_RW_SEL)) {
      if (protocolStart == WORD_DATA) {
        // Assume big-endian format
        OutPort8(host,(host->var.common.smbBase + HOST_DATA_0_REGISTER), *(data + 1));      // Msb data
        OutPort8(host,(host->var.common.smbBase + HOST_DATA_1_REGISTER), *data);            // Lsb data
      }
      else if (protocolStart == SR_DATA) {
        OutPort8(host,(host->var.common.smbBase + HOST_COMMAND_REGISTER), *data); // Byte data
      }
      else if (protocolStart == BLOCK_DATA) {
        sizeInBytes = *data;
        OutPort8(host,(host->var.common.smbBase + HOST_DATA_0_REGISTER), sizeInBytes); //# of bytes to transfer
        OutPort8(host,(host->var.common.smbBase + AUXILIARY_CONTROL_REGISTER), SMBUS_B_E32B); //Enable the 32 byte buffer in the Aux Cntrl Reg
        InPort8(host,(host->var.common.smbBase + HOST_CONTROL_REGISTER));  // Read the control register to reset the buffer pointer
        for (index = 1; index <= sizeInBytes; index++) {
          OutPort8(host,(host->var.common.smbBase + HOST_BLOCK_DATA_BYTE_REGISTER), *(data + index)); // Byte data
        }
      }
      else {
        OutPort8(host,(host->var.common.smbBase + HOST_DATA_0_REGISTER), *data);  // Byte data
      }
    } else {
    //
    // if reading a block, then need to enable the 32 byte buffer
    //
      if (protocolStart == BLOCK_DATA) {
        OutPort8(host,(host->var.common.smbBase + AUXILIARY_CONTROL_REGISTER), SMBUS_B_E32B); //Enable the 32 byte buffer in the Aux Cntrl Reg
        InPort8(host,(host->var.common.smbBase + HOST_CONTROL_REGISTER));  // Read the control register to reset the buffer pointer
      }
    }

    OutPort8(host,(host->var.common.smbBase + HOST_CONTROL_REGISTER), protocolStart);  // Protocol start

    if (!(host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG))) {
      // Wait for command completion
      startCount = GetCount(host);
      do {
        status = InPort8(host,(host->var.common.smbBase + HOST_STATUS_REGISTER));
        if (status & (HST_STS_FAILED | HST_STS_BUS_ERR | HST_STS_DEV_ERR | HST_STS_INTR)) break;
      } while (GetDelay(host, startCount) < SMB_TIMEOUT);
    } else {
      status = InPort8(host,(host->var.common.smbBase + HOST_STATUS_REGISTER));
    }

    // Get read data
    if ((status & BIT1) && (slaveAddress & SMBUS_B_RW_SEL)) {

      if (protocolStart == WORD_DATA) {
        // Assume big-endian format
        *(data + 1) = InPort8(host,(host->var.common.smbBase + HOST_DATA_0_REGISTER));
        *data = InPort8(host,(host->var.common.smbBase + HOST_DATA_1_REGISTER));
      } else if (protocolStart == BLOCK_DATA) {
        InPort8(host,(host->var.common.smbBase + HOST_CONTROL_REGISTER));  // Read the control register to reset the buffer pointer
        sizeInBytes = InPort8(host,(host->var.common.smbBase + HOST_DATA_0_REGISTER)); //# of bytes to transfer
        //
        // Add error checking not supported
        //
        *data = sizeInBytes;
        for (index = 1; index <= sizeInBytes; index++) {
          *(data + index) = InPort8(host,(host->var.common.smbBase + HOST_BLOCK_DATA_BYTE_REGISTER)); // Byte data
        }
      } else {
        *data = InPort8(host,(host->var.common.smbBase + HOST_DATA_0_REGISTER));
      }
    }

    // Clear all status bits
    OutPort8(host,(host->var.common.smbBase + HOST_STATUS_REGISTER), SMBUS_B_HSTS_ALL);
  }
  // Return last status
  return status;
}

UINT32
ReadSmb (
  PSYSHOST         host,
  UINT8            socket,
  struct smbDevice dev,
  UINT16            byteOffset,
  UINT8            *data
  )
/*++

  Read SMBUS device at a given device and offset

  @param host        - Pointer to the system host (root) structure
  @param socket      - Processor socket ID
  @param dev         - Device address on SMBUS
  @param byteOffset  - Offset in Device
  @param data        - Pointer to store value
                       NOTE: For a PCH block read, the array must be 33bytes in length,
                       and the first data byte pointed to is the number of bytes read.
                       This implementation only allows a maximum of 32bytes to be read.

  @retval 0 - Success
  @retval 1 - failure

--*/
{
  UINT32                    status;
  UINT32                    smbCntlReg;
  SMBCNTL_0_MC_MAIN_HSX_BDX_STRUCT  smbCntl;
  status  = ReadSmbWorkerFunction (host, socket, dev, byteOffset, data);

  //
  // Determines if semaphore is needed to read SMBus
  //
  IsSmbusSemaphoreNeeded (host, socket);


  if (status == RETRY){
    //
    // if SMBus is busy force a soft reset and retry transaction
    //

    if ((dev.address.busSegment == 0) || (Is2HA(host))){
      smbCntlReg  = SMBCNTL_0_MC_MAIN_REG;
    } else {
      smbCntlReg  = SMBCNTL_1_MC_MAIN_REG;
    }

    smbCntl.Data = MemReadPciCfgMC (host, socket, dev.mcId, smbCntlReg);
    //
    // If slave device is hung, software can write this bit to 1 and
    // the SMB_CKOVRD to 0 (for more than 35ms) to force hung the SMB
    // slaves to time-out and put it in idle state without using power
    // good reset or warm reset.
    //

    //
    // Override the clock
    //
    smbCntl.Bits.smb_ckovrd = 0;

    //
    // Issue a soft reset
    //
    smbCntl.Bits.smb_soft_rst = 1;
    MemWritePciCfgMC (host, socket, dev.mcId, smbCntlReg, smbCntl.Data);
    //
    // Wait 35ms
    //
    FixedDelay (host, 35000);
    smbCntl.Bits.smb_ckovrd   = 1;
    smbCntl.Bits.smb_soft_rst = 0;
    MemWritePciCfgMC (host, socket, dev.mcId, smbCntlReg, smbCntl.Data);

    status  = ReadSmbWorkerFunction (host, socket, dev, byteOffset, data);
  }

  //
  // Release Semaphore
  //
  if (host->var.common.SmbusAPSemaphoreAcquired == 1) {
      ReleaseSmbus(host, socket);
      host->var.common.smbSync = 0x0;
  }

  return status;
}

UINT32
ReadSmbWorkerFunction (
  PSYSHOST         host,
  UINT8            socket,
  struct smbDevice dev,
  UINT16            byteOffset,
  UINT8            *data
  )
/*++

  Read SMBUS device at a given device and offset

  @param host        - Pointer to the system host (root) structure
  @param socket      - Processor socket ID
  @param dev         - Device address on SMBUS
  @param byteOffset  - Offset in Device
  @param data        - Pointer to store value
                       NOTE: For a PCH block read, the array must be 33bytes in length,
                       and the first data byte pointed to is the number of bytes read.
                       This implementation only allows a maximum of 32bytes to be read.
                       NOTE: This function assumes TSOD polling is disabled.

  @retval 0 - Success
  @retval 1 - failure

--*/
{
  UINT32                    startCount = 0;
  UINT32                    rval;
  UINT32                    smbCntlReg;
  UINT32                    smbCmdReg;
  UINT32                    smbStatReg;
  UINT8                     status = 0;
  UINT8                     retry;
  UINT8                     protocol;
  SMBCMD_0_MC_MAIN_STRUCT   smbCmd;
  SMB_STAT_0_MC_MAIN_STRUCT smbStat;
  SMBCNTL_0_MC_MAIN_HSX_BDX_STRUCT  smbCntl;
  TSOD_CONTROL_PCU_FUN1_STRUCT      pcuTsodCtrlCfg;

  rval    = SUCCESS;


  //
  // Call the OEM hook to read the SMBbus.  Return if SUCCESS.
  //
  switch (MemReadSmb (host, socket, dev, byteOffset, data)) {
    //
    // No hook available
    //
    case 0:
      break;

    //
    // read was successful
    //
    case 1:
      return SUCCESS;

    //
    // read failed
    //
    case 2:
      return FAILURE;
  }

  if (dev.address.controller == PROCESSOR) {


    if ((dev.address.busSegment == 0) || (Is2HA(host))) {

        smbCmdReg   = SMBCMD_0_MC_MAIN_REG;
        smbStatReg  = SMB_STAT_0_MC_MAIN_REG;
        smbCntlReg  = SMBCNTL_0_MC_MAIN_REG;
    } else {
        smbCmdReg   = SMBCMD_1_MC_MAIN_REG;
        smbStatReg  = SMB_STAT_1_MC_MAIN_REG;
        smbCntlReg  = SMBCNTL_1_MC_MAIN_REG;
    }

    //
    // Ensure TSOD polling is disabled before continuing
    //
    pcuTsodCtrlCfg.Data = ReadCpuPciCfgEx (host, socket, 0, TSOD_CONTROL_PCU_FUN1_REG);
    if (pcuTsodCtrlCfg.Bits.tsod_polling_interval > 0) {
#ifdef SERIAL_DBG_MSG
      if (checkMsgLevel(host, SDBG_MINMAX)) {
        rcPrintf ((host, "read CPU SMBUS with TSOD polling enabled!"));
      }
#endif  // SERIAL_DBG_MSG
      FatalError (host, 0, 0);
    }
    //
    // Initialize the smb_dti field if needed
    //
    smbCntl.Data = MemReadPciCfgMC (host, socket, dev.mcId, smbCntlReg);
    if (host->var.mem.socket[socket].smbCntlDti[dev.address.busSegment] != dev.address.deviceType){
      smbCntl.Bits.smb_dti = dev.address.deviceType;
      MemWritePciCfgMC (host, socket, dev.mcId, smbCntlReg, smbCntl.Data);
      host->var.mem.socket[socket].smbCntlDti[dev.address.busSegment] = (UINT8) dev.address.deviceType;
    }

    //
    // Initialize to 0
    //
    smbCmd.Data = 0;

    //
    // Byte to read
    //
    smbCmd.Bits.smb_ba = (UINT32) byteOffset;

    //
    // Address of device to read
    //
    smbCmd.Bits.smb_sa = (UINT32) dev.address.strapAddress;

    //
    // clear write command bits for read command
    //
    smbCmd.Bits.smb_wrt_cmd = 0;

    //
    // Write pointer must be clear on a read
    //
    smbCmd.Bits.smb_wrt_pntr = 0;

    //
    // set byte or word access bit
    //
    if (dev.compId == MTS) {
      smbCmd.Bits.smb_word_access = 1;
    } else {
      smbCmd.Bits.smb_word_access = 0;
    }

    //
    // use "pointer" mode instead of random byte protocol
    //
    if (dev.compId == DCP_AD5247) {
      smbCmd.Bits.smb_pntr_sel = 1;
    } else {
      smbCmd.Bits.smb_pntr_sel = 0;
    }

    //
    // Set bit to trigger the read
    //
    smbCmd.Bits.smb_cmd_trigger = 1;

    //
    // Wait for host not busy
    //
    if (!(host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG))) {
      startCount = GetCount (host);
    }

    if (!(host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG))) {
      do {
        smbStat.Data = MemReadPciCfgMC (host, socket, dev.mcId, smbStatReg);
        if (!smbStat.Bits.smb_busy) break;
#ifdef RC_SIM_FEEDBACK
        break;
#endif
        // Wait for timeout
      } while (GetDelay (host, startCount) < SMB_TIMEOUT);
    } else {
      do {
        smbStat.Data = MemReadPciCfgMC (host, socket, dev.mcId, smbStatReg);
        if (!smbStat.Bits.smb_busy) break;
        // Wait for timeout
      } while (smbStat.Bits.smb_busy);
    }

    //
    // Send command
    //
    MemWritePciCfgMC (host, socket, dev.mcId, smbCmdReg, smbCmd.Data);

    //
    // Wait for host not busy
    //
    if (!(host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG))) {
      startCount = GetCount (host);
    }

    if (!(host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG))) {
      do {
        smbStat.Data = MemReadPciCfgMC (host, socket, dev.mcId, smbStatReg);
        if (!smbStat.Bits.smb_busy) break;
#ifdef RC_SIM_FEEDBACK
        break;
#endif
       // Wait for timeout
      } while (GetDelay (host, startCount) < SMB_TIMEOUT);
    } else {
      do {
        smbStat.Data = MemReadPciCfgMC (host, socket, dev.mcId, smbStatReg);
        if (!smbStat.Bits.smb_busy) break;
        // Wait for timeout
      } while (smbStat.Bits.smb_busy);
    }
    //
    // Wait for the data
    //
    while (!smbStat.Bits.smb_rdo && !smbStat.Bits.smb_sbe) {
      smbStat.Data = MemReadPciCfgMC (host, socket, dev.mcId, smbStatReg);
#ifdef RC_SIM_FEEDBACK
      break;
#endif
    }
    //
    // If read data is valid
    //
    if (smbStat.Bits.smb_rdo && !smbStat.Bits.smb_sbe) {
      if (dev.compId == MTS) {
        *data = (UINT8) smbStat.Bits.smb_rdata;  //lsb
        *(data + 1) = (UINT8) (smbStat.Bits.smb_rdata >> 8); //msb
      } else {
        *data = (UINT8) smbStat.Bits.smb_rdata;
      }
    } else {
      rval = RETRY;
    }

  } else if (dev.address.controller == PCH) {
    //
    // Determine which protocol to use
    //
    if (dev.compId == DCP_AD5247) {
      protocol = SR_DATA;
    } else protocol = BYTE_DATA;
    //
    // Issue the command, handling errors
    //
    for (retry = 0; retry < SMB_RETRY_LIMIT; retry++) {
      //
      // Program PCH SMBus controller for given protocol
      //
      status = SendPchSmbCmd(host, byteOffset, (UINT8)((dev.address.deviceType << 4) | (dev.address.strapAddress << 1) | BIT0), protocol, data);

      // Check for bus error or host busy
      if (status & (HST_STS_FAILED | HST_STS_BUS_ERR | HST_STS_HOST_BUSY)) {
         FixedDelay (host, SMB_TIMEOUT);
      }
      else break;
    }
    if (status & (HST_STS_FAILED | HST_STS_BUS_ERR | HST_STS_DEV_ERR | HST_STS_HOST_BUSY)) rval = FAILURE;
  } else {
    rval = FAILURE;  // supported controller not detected
  }
  //
  // Save the values in these registers for S3 resume.
  //
  host->nvram.mem.socket[socket].smbCmd0 = MemReadPciCfgMC (host, socket, 0, SMBCMD_0_MC_MAIN_REG);

  if (Is2HA(host)){
    host->nvram.mem.socket[socket].smbCmd1 = MemReadPciCfgMC (host, socket, 1, SMBCMD_0_MC_MAIN_REG);
  } else {
    host->nvram.mem.socket[socket].smbCmd1 = MemReadPciCfgMC (host, socket, 0, SMBCMD_1_MC_MAIN_REG);
  }
#ifdef RC_SIM_SMBUS
  if (dev.compId == SPD) {
    *data = tempData;
    rval = tempRval;
  }
#endif // SIM_BUILD

  return rval;
}

/**

  Write SMBUS dev/offset with specified data

  @param host        - Pointer to the system host (root) structure
  @param socket      - Processor socket ID
  @param dev         - SMBUS device address
  @param byteOffset  - byte offset within device
  @param data        - pointer to data to write
                       NOTE: For a PCH block write, the first data byte pointed to must be
                       the number of bytes to write.  This implementation only allows a
                       maximum of 32bytes to be written.

  @retval 0 - success
  @retval 1 - failure

**/
UINT32
WriteSmb (
  PSYSHOST         host,
  UINT8            socket,
  struct smbDevice dev,
  UINT8            byteOffset,
  UINT8            *data
  )
{
  UINT32                    rval = SUCCESS;
  UINT32                    startCount = 0;
  UINT32                    smbCmdReg;
  UINT32                    smbStatReg;
  UINT32                    smbCntlReg;
  UINT8                     status = 0;
  UINT8                     retry;
  UINT8                     protocol;
  SMBCMD_0_MC_MAIN_STRUCT   smbCmd;
  SMB_STAT_0_MC_MAIN_STRUCT smbStat;
  SMBCNTL_0_MC_MAIN_HSX_BDX_STRUCT  smbCntl;
  TSOD_CONTROL_PCU_FUN1_STRUCT      pcuTsodCtrlCfg;

 
  //
  // Determines if semaphore is needed to read SMBus
  //
  IsSmbusSemaphoreNeeded (host, socket);
  
  
  //
  // Call the OEM hook to write the SMBbus.  Return if SUCCESS.
  //
  switch (MemWriteSmb (host, dev, byteOffset, data)) {
    //
    // No hook available
    //
    case 0:
      break;

    //
    // SPD read was successful
    //
    case 1:
      return SUCCESS;

    //
    // SPD read failed
    //
    case 2:
      return FAILURE;
  }

  if (dev.address.controller == PROCESSOR) {

    if ((dev.address.busSegment == 0) || Is2HA(host)) {
      smbCmdReg   = SMBCMD_0_MC_MAIN_REG;
      smbStatReg  = SMB_STAT_0_MC_MAIN_REG;
      smbCntlReg  = SMBCNTL_0_MC_MAIN_REG;
    } else {
      smbCmdReg   = SMBCMD_1_MC_MAIN_REG;
      smbStatReg  = SMB_STAT_1_MC_MAIN_REG;
      smbCntlReg  = SMBCNTL_1_MC_MAIN_REG;
    }

    //
    // Ensure TSOD polling is disabled before continuing
    //
    pcuTsodCtrlCfg.Data = ReadCpuPciCfgEx (host, socket, 0, TSOD_CONTROL_PCU_FUN1_REG);
    if (pcuTsodCtrlCfg.Bits.tsod_polling_interval > 0) {
#ifdef SERIAL_DBG_MSG
      if (checkMsgLevel(host, SDBG_MINMAX)) {
        rcPrintf ((host, "write to CPU SMBUS with TSOD polling enabled!"));
      }
#endif  // SERIAL_DBG_MSG
      FatalError (host, 0, 0);
    }

    smbCntl.Data = MemReadPciCfgMC (host, socket, dev.mcId, smbCntlReg);
    smbCntl.Bits.smb_dis_wrt = 0;
    //
    // Initialize the smb_dti field if needed
    //
    smbCntl.Bits.smb_dti = dev.address.deviceType;
    MemWritePciCfgMC (host, socket, dev.mcId, smbCntlReg, smbCntl.Data);
    host->var.mem.socket[socket].smbCntlDti[dev.address.busSegment] = (UINT8) dev.address.deviceType;

    //
    // Byte to write
    //
    smbCmd.Bits.smb_ba = (UINT32) byteOffset;

    //
    // Address of device to write
    //
    smbCmd.Bits.smb_sa = (UINT32) dev.address.strapAddress;

    //
    // data to write
    //
    if (dev.compId == MTS) {
      smbCmd.Bits.smb_wdata = (UINT32)*data + ((UINT32)*(data + 1) << 8);
    } else {
      smbCmd.Bits.smb_wdata = (UINT32) *data;
    }
    //
    // set write command bits
    //
    smbCmd.Bits.smb_wrt_cmd = 1;

    //
    // Do a send byte command for the AD5247, else do a regular SMBUS write
    //
    if (dev.compId == DCP_AD5247) {
      smbCmd.Bits.smb_wrt_pntr = 1;
    } else {
      smbCmd.Bits.smb_wrt_pntr = 0;
    }

    //
    // set byte or word access bit
    //
    if (dev.compId == MTS) {
      smbCmd.Bits.smb_word_access = 1;
    } else {
      smbCmd.Bits.smb_word_access = 0;
    }

    //
    // Clear the pointer select bit
    //
    smbCmd.Bits.smb_pntr_sel = 0;

    //
    // Set bit to trigger the write
    //
    smbCmd.Bits.smb_cmd_trigger = 1;

    //
    // Wait for host not busy
    //
    if (!(host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG))) {
      startCount = GetCount (host);
    }

    if (!(host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG))) {
      do {
        smbStat.Data = MemReadPciCfgMC (host, socket, dev.mcId, smbStatReg);
        if (!smbStat.Bits.smb_busy) break;
#ifdef RC_SIM_FEEDBACK
        break;
#endif
        // Wait for timeout
      } while (GetDelay (host, startCount) < SMB_TIMEOUT);
    } else {
      do {
        smbStat.Data = MemReadPciCfgMC (host, socket, dev.mcId, smbStatReg);
        if (!smbStat.Bits.smb_busy) break;
        // Wait for timeout
      } while (smbStat.Bits.smb_busy);
    }
    //
    // Send command
    //
    MemWritePciCfgMC (host, socket, dev.mcId, smbCmdReg, smbCmd.Data);

    //
    // Wait for the write to complete
    //
    do {
      smbStat.Data = MemReadPciCfgMC (host, socket, dev.mcId, smbStatReg);
#ifdef RC_SIM_FEEDBACK
      break;
#endif
   } while (!smbStat.Bits.smb_wod && !smbStat.Bits.smb_sbe);

  } else if (dev.address.controller == PCH) {
    //
    // Determine which protocol to use
    //
    if (dev.compId == DCP_AD5247) {
      protocol = SR_DATA;
    } else protocol = BYTE_DATA;
    //
    // Issue the command, handling errors
    //
    for (retry = 0; retry < SMB_RETRY_LIMIT; retry++) {
      //
      // Program PCH SMBus controller for given protocol
      //
      status = SendPchSmbCmd(host, byteOffset, (UINT8)((dev.address.deviceType << 4) | (dev.address.strapAddress << 1)), protocol, data);

      // Check for bus error or host busy
      if (status & (HST_STS_FAILED | HST_STS_BUS_ERR | HST_STS_HOST_BUSY)) {
         FixedDelay (host, SMB_TIMEOUT);
      }
      else break;
    }
    if (status & (HST_STS_FAILED | HST_STS_BUS_ERR | HST_STS_DEV_ERR | HST_STS_HOST_BUSY)) rval = FAILURE;
  } else {
    rval = FAILURE;  // Only supporting PROCESSOR SMBUS controllers for now
  }
  //
  // Save the values in these registers for S3 resume.
  //
  host->nvram.mem.socket[socket].smbCmd0 = MemReadPciCfgMC (host, socket, 0, SMBCMD_0_MC_MAIN_REG);
  if (Is2HA(host)){
    host->nvram.mem.socket[socket].smbCmd1 = MemReadPciCfgMC (host, socket, 1, SMBCMD_0_MC_MAIN_REG);
  } else {
    host->nvram.mem.socket[socket].smbCmd1 = MemReadPciCfgMC (host, socket, 0, SMBCMD_1_MC_MAIN_REG);
  }

  //
  // Release Semaphore
  //
  if (host->var.common.SmbusAPSemaphoreAcquired == 1) {
      ReleaseSmbus(host, socket);
      host->var.common.smbSync = 0x0;
  }
  
  return rval;
}

void
GetSmbAddress (
        PSYSHOST  host,
        UINT8     socket,
        UINT8     ch,
        UINT8     dimm,
        struct smbDevice *dev
        )
/*++

  Returns the SMBus strap address of the DIMM slot

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number
  @param dimm    - Dimm number
  @param dev     - Pointer to smbDevice

  @retval N/A ( MC ID is returned in the dev pointer )

--*/
{
  dev->address.busSegment = (ch & BIT1) >> 1;
  dev->address.strapAddress = dimm + ((ch & BIT0) * 4);
  if (IsDESku(host)
  ) {
    dev->address.strapAddress = dimm + ((ch & BIT0) * 2);
  }
  dev->mcId = GetMCID(host, socket, ch);
} // GetSmbAddress

