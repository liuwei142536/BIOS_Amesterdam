/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++
  
  Copyright (c) 2001 - 2015 Intel Corporation.  All rights reserved.

  This software and associated documentation (if any) is furnished under 
  a license and may only be used or copied in accordance with the terms 
  of the license.  Except as permitted by such license, no part of this 
  software or documentation may be reproduced, stored in a retrieval system, 
  or transmitted in any form or by any means without the express written 
  consent of Intel Corporation.
  
  Module Name:
  
    SpsNm.asl
  
  Abstract:

    This file defines ASL code for Node Manager (NM) support.
  
--*/
#include <AcpiOemElinks.h> // AptioV Server override: Program OEM ID and OEM table ID based on SDL token

//AptioV Server Override: Disable Power Meter
#define POWER_METER 0 // Can temporary disable Power Meter if needed
#if 0
#define DbgTrace(a) Store(a, Debug)
#else
#define DbgTrace(a)
#endif

DefinitionBlock("SpsNm.aml", "SSDT", 2, "INTEL", "SpsNm   ", 2)
{
  External(\HWPE, IntObj)
  External(\PETE, IntObj)
  External(\PSLM, PkgObj)
  External(\TSLM, PkgObj)
  
  External(\PURE, IntObj)
  External(\PURA, IntObj)
  External(\PURB, IntObj)
  External(\_SB.PRAD, DeviceObj)
  
  External(\_SB.SCK0, DeviceObj)
  External(\_SB.SCK0.CP00, DeviceObj)
  External(\_SB.SCK0.CP01, DeviceObj)
  External(\_SB.SCK0.CP02, DeviceObj)
  External(\_SB.SCK0.CP03, DeviceObj)
  External(\_SB.SCK0.CP04, DeviceObj)
  External(\_SB.SCK0.CP05, DeviceObj)
  External(\_SB.SCK0.CP06, DeviceObj)
  External(\_SB.SCK0.CP07, DeviceObj)
  External(\_SB.SCK0.CP08, DeviceObj)
  External(\_SB.SCK0.CP09, DeviceObj)
  External(\_SB.SCK0.CP0A, DeviceObj)
  External(\_SB.SCK0.CP0B, DeviceObj)
  External(\_SB.SCK0.CP0C, DeviceObj)
  External(\_SB.SCK0.CP0D, DeviceObj)
  External(\_SB.SCK0.CP0E, DeviceObj)
  External(\_SB.SCK0.CP0F, DeviceObj)
  External(\_SB.SCK0.CP10, DeviceObj)
  External(\_SB.SCK0.CP11, DeviceObj)
  External(\_SB.SCK0.CP12, DeviceObj)
  External(\_SB.SCK0.CP13, DeviceObj)
  External(\_SB.SCK0.CP14, DeviceObj)
  External(\_SB.SCK0.CP15, DeviceObj)
  External(\_SB.SCK0.CP16, DeviceObj)
  External(\_SB.SCK0.CP17, DeviceObj)
  External(\_SB.SCK0.CP18, DeviceObj)
  External(\_SB.SCK0.CP19, DeviceObj)
  External(\_SB.SCK0.CP1A, DeviceObj)
  External(\_SB.SCK0.CP1B, DeviceObj)
  External(\_SB.SCK0.CP1C, DeviceObj)
  External(\_SB.SCK0.CP1D, DeviceObj)
  External(\_SB.SCK0.CP1E, DeviceObj)
  External(\_SB.SCK0.CP1F, DeviceObj)
  External(\_SB.SCK0.CP20, DeviceObj)
  External(\_SB.SCK0.CP21, DeviceObj)
  External(\_SB.SCK0.CP22, DeviceObj)
  External(\_SB.SCK0.CP23, DeviceObj)
  External(\_SB.SCK0.CP24, DeviceObj)
  External(\_SB.SCK0.CP25, DeviceObj)
  External(\_SB.SCK0.CP26, DeviceObj)
  External(\_SB.SCK0.CP27, DeviceObj)
  External(\_SB.SCK0.CP28, DeviceObj)
  External(\_SB.SCK0.CP29, DeviceObj)
  External(\_SB.SCK0.CP2A, DeviceObj)
  External(\_SB.SCK0.CP2B, DeviceObj)
  External(\_SB.SCK0.CP2C, DeviceObj)
  External(\_SB.SCK0.CP2D, DeviceObj)
  External(\_SB.SCK0.CP2E, DeviceObj)
  External(\_SB.SCK0.CP2F, DeviceObj)
  External(\_SB.SCK1, DeviceObj)
  External(\_SB.SCK1.CP00, DeviceObj)
  External(\_SB.SCK1.CP01, DeviceObj)
  External(\_SB.SCK1.CP02, DeviceObj)
  External(\_SB.SCK1.CP03, DeviceObj)
  External(\_SB.SCK1.CP04, DeviceObj)
  External(\_SB.SCK1.CP05, DeviceObj)
  External(\_SB.SCK1.CP06, DeviceObj)
  External(\_SB.SCK1.CP07, DeviceObj)
  External(\_SB.SCK1.CP08, DeviceObj)
  External(\_SB.SCK1.CP09, DeviceObj)
  External(\_SB.SCK1.CP0A, DeviceObj)
  External(\_SB.SCK1.CP0B, DeviceObj)
  External(\_SB.SCK1.CP0C, DeviceObj)
  External(\_SB.SCK1.CP0D, DeviceObj)
  External(\_SB.SCK1.CP0E, DeviceObj)
  External(\_SB.SCK1.CP0F, DeviceObj)
  External(\_SB.SCK1.CP10, DeviceObj)
  External(\_SB.SCK1.CP11, DeviceObj)
  External(\_SB.SCK1.CP12, DeviceObj)
  External(\_SB.SCK1.CP13, DeviceObj)
  External(\_SB.SCK1.CP14, DeviceObj)
  External(\_SB.SCK1.CP15, DeviceObj)
  External(\_SB.SCK1.CP16, DeviceObj)
  External(\_SB.SCK1.CP17, DeviceObj)
  External(\_SB.SCK1.CP18, DeviceObj)
  External(\_SB.SCK1.CP19, DeviceObj)
  External(\_SB.SCK1.CP1A, DeviceObj)
  External(\_SB.SCK1.CP1B, DeviceObj)
  External(\_SB.SCK1.CP1C, DeviceObj)
  External(\_SB.SCK1.CP1D, DeviceObj)
  External(\_SB.SCK1.CP1E, DeviceObj)
  External(\_SB.SCK1.CP1F, DeviceObj)
  External(\_SB.SCK1.CP20, DeviceObj)
  External(\_SB.SCK1.CP21, DeviceObj)
  External(\_SB.SCK1.CP22, DeviceObj)
  External(\_SB.SCK1.CP23, DeviceObj)
  External(\_SB.SCK1.CP24, DeviceObj)
  External(\_SB.SCK1.CP25, DeviceObj)
  External(\_SB.SCK1.CP26, DeviceObj)
  External(\_SB.SCK1.CP27, DeviceObj)
  External(\_SB.SCK1.CP28, DeviceObj)
  External(\_SB.SCK1.CP29, DeviceObj)
  External(\_SB.SCK1.CP2A, DeviceObj)
  External(\_SB.SCK1.CP2B, DeviceObj)
  External(\_SB.SCK1.CP2C, DeviceObj)
  External(\_SB.SCK1.CP2D, DeviceObj)
  External(\_SB.SCK1.CP2E, DeviceObj)
  External(\_SB.SCK1.CP2F, DeviceObj)
  External(\_SB.SCK2, DeviceObj)
  External(\_SB.SCK2.CP00, DeviceObj)
  External(\_SB.SCK2.CP01, DeviceObj)
  External(\_SB.SCK2.CP02, DeviceObj)
  External(\_SB.SCK2.CP03, DeviceObj)
  External(\_SB.SCK2.CP04, DeviceObj)
  External(\_SB.SCK2.CP05, DeviceObj)
  External(\_SB.SCK2.CP06, DeviceObj)
  External(\_SB.SCK2.CP07, DeviceObj)
  External(\_SB.SCK2.CP08, DeviceObj)
  External(\_SB.SCK2.CP09, DeviceObj)
  External(\_SB.SCK2.CP0A, DeviceObj)
  External(\_SB.SCK2.CP0B, DeviceObj)
  External(\_SB.SCK2.CP0C, DeviceObj)
  External(\_SB.SCK2.CP0D, DeviceObj)
  External(\_SB.SCK2.CP0E, DeviceObj)
  External(\_SB.SCK2.CP0F, DeviceObj)
  External(\_SB.SCK2.CP10, DeviceObj)
  External(\_SB.SCK2.CP11, DeviceObj)
  External(\_SB.SCK2.CP12, DeviceObj)
  External(\_SB.SCK2.CP13, DeviceObj)
  External(\_SB.SCK2.CP14, DeviceObj)
  External(\_SB.SCK2.CP15, DeviceObj)
  External(\_SB.SCK2.CP16, DeviceObj)
  External(\_SB.SCK2.CP17, DeviceObj)
  External(\_SB.SCK2.CP18, DeviceObj)
  External(\_SB.SCK2.CP19, DeviceObj)
  External(\_SB.SCK2.CP1A, DeviceObj)
  External(\_SB.SCK2.CP1B, DeviceObj)
  External(\_SB.SCK2.CP1C, DeviceObj)
  External(\_SB.SCK2.CP1D, DeviceObj)
  External(\_SB.SCK2.CP1E, DeviceObj)
  External(\_SB.SCK2.CP1F, DeviceObj)
  External(\_SB.SCK2.CP20, DeviceObj)
  External(\_SB.SCK2.CP21, DeviceObj)
  External(\_SB.SCK2.CP22, DeviceObj)
  External(\_SB.SCK2.CP23, DeviceObj)
  External(\_SB.SCK2.CP24, DeviceObj)
  External(\_SB.SCK2.CP25, DeviceObj)
  External(\_SB.SCK2.CP26, DeviceObj)
  External(\_SB.SCK2.CP27, DeviceObj)
  External(\_SB.SCK2.CP28, DeviceObj)
  External(\_SB.SCK2.CP29, DeviceObj)
  External(\_SB.SCK2.CP2A, DeviceObj)
  External(\_SB.SCK2.CP2B, DeviceObj)
  External(\_SB.SCK2.CP2C, DeviceObj)
  External(\_SB.SCK2.CP2D, DeviceObj)
  External(\_SB.SCK2.CP2E, DeviceObj)
  External(\_SB.SCK2.CP2F, DeviceObj)
  External(\_SB.SCK3, DeviceObj)
  External(\_SB.SCK3.CP00, DeviceObj)
  External(\_SB.SCK3.CP01, DeviceObj)
  External(\_SB.SCK3.CP02, DeviceObj)
  External(\_SB.SCK3.CP03, DeviceObj)
  External(\_SB.SCK3.CP04, DeviceObj)
  External(\_SB.SCK3.CP05, DeviceObj)
  External(\_SB.SCK3.CP06, DeviceObj)
  External(\_SB.SCK3.CP07, DeviceObj)
  External(\_SB.SCK3.CP08, DeviceObj)
  External(\_SB.SCK3.CP09, DeviceObj)
  External(\_SB.SCK3.CP0A, DeviceObj)
  External(\_SB.SCK3.CP0B, DeviceObj)
  External(\_SB.SCK3.CP0C, DeviceObj)
  External(\_SB.SCK3.CP0D, DeviceObj)
  External(\_SB.SCK3.CP0E, DeviceObj)
  External(\_SB.SCK3.CP0F, DeviceObj)
  External(\_SB.SCK3.CP10, DeviceObj)
  External(\_SB.SCK3.CP11, DeviceObj)
  External(\_SB.SCK3.CP12, DeviceObj)
  External(\_SB.SCK3.CP13, DeviceObj)
  External(\_SB.SCK3.CP14, DeviceObj)
  External(\_SB.SCK3.CP15, DeviceObj)
  External(\_SB.SCK3.CP16, DeviceObj)
  External(\_SB.SCK3.CP17, DeviceObj)
  External(\_SB.SCK3.CP18, DeviceObj)
  External(\_SB.SCK3.CP19, DeviceObj)
  External(\_SB.SCK3.CP1A, DeviceObj)
  External(\_SB.SCK3.CP1B, DeviceObj)
  External(\_SB.SCK3.CP1C, DeviceObj)
  External(\_SB.SCK3.CP1D, DeviceObj)
  External(\_SB.SCK3.CP1E, DeviceObj)
  External(\_SB.SCK3.CP1F, DeviceObj)
  External(\_SB.SCK3.CP20, DeviceObj)
  External(\_SB.SCK3.CP21, DeviceObj)
  External(\_SB.SCK3.CP22, DeviceObj)
  External(\_SB.SCK3.CP23, DeviceObj)
  External(\_SB.SCK3.CP24, DeviceObj)
  External(\_SB.SCK3.CP25, DeviceObj)
  External(\_SB.SCK3.CP26, DeviceObj)
  External(\_SB.SCK3.CP27, DeviceObj)
  External(\_SB.SCK3.CP28, DeviceObj)
  External(\_SB.SCK3.CP29, DeviceObj)
  External(\_SB.SCK3.CP2A, DeviceObj)
  External(\_SB.SCK3.CP2B, DeviceObj)
  External(\_SB.SCK3.CP2C, DeviceObj)
  External(\_SB.SCK3.CP2D, DeviceObj)
  External(\_SB.SCK3.CP2E, DeviceObj)
  External(\_SB.SCK3.CP2F, DeviceObj)

#if 0
  External(\_SB.PCI0, DeviceObj)
  Scope(\_SB.PCI0)
  {
    Device(HEC1)
    {
      Name(_ADR, 0x00160000)
#else
  External(\_SB.PCI0, DeviceObj)
  External(\_SB.PCI0.HECI, DeviceObj)
  External(\_SB.PCI0.HEC2, DeviceObj)
  Scope(\_SB.PCI0.HECI)
  {
#endif
      //
      // HECI-1 HFS register
      // It holds ME Firmware Status #1
      //
      OperationRegion(MFS1, PCI_Config, 0x40, 0x4)
      Field(MFS1, DWordAcc, NoLock, Preserve)
      {
        MECS, 4, // ME current state
        RVD1, 12,
        OPMD, 4,
        RVD2, 12
      }
      
      //
      // Device status handler method - called by OSPM before INI
      // This device reports state of the HECI-1 device.
      // If ME firmware is not operational HECI-1 is reported
      // present but disabled. If ME is operational HECI-1 is
      // present and functioning, but hidden from UI.
      //
      Method(_STA, 0, NotSerialized)
      {
#if AMT_SUPPORT
        If (LNotEqual(OPMD, 15)) // if it is non-SPS running in ME
        {
          Return(0x0F)
        }
#endif
        If (LEqual(MECS, 5))
        {
          Return(0x0B) // functioning hidden from UI
        }
        Else
        {
          Return(0x09) // present but disabled
        }
      }

      //
      // Define the region for HECI-1 registers mapped to HECI-1 MBAR.
      // PCIBARTarget type seems to be not implemented in several OSes,
      // but it is not a problem since we do not actually use it.
      // PCIBARTarget is used only to declare HECI-1 as ACPI only device.
      //
      //OperationRegion(H1CS, PCIBARTarget, 0x10, 0x10)
      OperationRegion(H1CS, SystemMemory, 0x8000000080000000, 0x10)
      Field(H1CS, DWordAcc, NoLock, Preserve)
      {
        CBWW,32, // write window - R_HECI_HOST_CB_WW
        // Bits of R_HECI_HOST_H_CSR (Host Control Status Register)
        HIE, 1,
        HIS, 1,
        HIG, 1,
        HRD, 1,
        HRS, 1,
           , 3,
        HRP, 8, // host read pointer
        HWP, 8, // host write pointer
        HBD, 8, // host buffer depth
        CBRW,32, // read window - R_HECI_ME_CB_RW
        // Bits of R_HECI_ME_CSR_HA (ME Control Status Register - Host Access)
        MIE, 1,
        MIS, 1,
        MIG, 1,
        MRD, 1,
        MRS, 1,
           , 3,
        MRP, 8, // ME read pointer
        MWP, 8, // ME write pointer
        MBD, 8  // ME buffer depth
      }
#if 0
    } // Device(HEC1)
    
    Device(HEC2)
    {
      Name(_ADR, 0x00160001)
#else
  } // Scope(\_SB.PCI0.HECI)
  
  Scope(\_SB.PCI0.HEC2)
  {
#endif
      //
      // HECI-2 HFS register
      // It holds requested NM Firmware Status
      //
      OperationRegion(NMFS, PCI_Config, 0x40, 0x4)
      Field(NMFS, DWordAcc, NoLock, Preserve)
      {
            ,31,
        NMEN, 1
      }
      
      //
      // HECI-2 GS_SHDW2 register
      // It holds requested P-state limits
      //
      OperationRegion(H2RP, PCI_Config, 0x60, 0x4)
      Field(H2RP, DWordAcc, NoLock, Preserve)
      {
        RPS0, 4,
        RPS1, 4,
        RPS2, 4,
        RPS3, 4,
        RPS4, 4,
        RPS5, 4,
        RPS6, 4,
        RPS7, 4
      }
      
      //
      // HECI-2 GS_SHDW3 register
      // It holds requested T-state limits
      //
      OperationRegion(H2RT, PCI_Config, 0x64, 0x4)
      Field(H2RT, DWordAcc, NoLock, Preserve)
      {
        RTS0, 4,
        RTS1, 4,
        RTS2, 4,
        RTS3, 4,
        RTS4, 4,
        RTS5, 4,
        RTS6, 4,
        RTS7, 4
      }
      
      //
      // HECI-2 H_GS2 register
      // It holds current P-state limits
      //
      OperationRegion(H2CP, PCI_Config, 0x70, 0x4)
      Field(H2CP, DWordAcc, NoLock, Preserve)
      {
        CPS0, 4,
        CPS1, 4,
        CPS2, 4,
        CPS3, 4,
        CPS4, 4,
        CPS5, 4,
        CPS6, 4,
        CPS7, 4
      }
      
      //
      // HECI-2 H_GS3 register
      // It holds current T-state limits
      //
      OperationRegion(H2CT, PCI_Config, 0x74, 0x4)
      Field(H2CT, DWordAcc, NoLock, Preserve)
      {
        CTS0, 4,
        CTS1, 4,
        CTS2, 4,
        CTS3, 4,
        CTS4, 4,
        CTS5, 4,
        CTS6, 4,
        CTS7, 4
      }
      
      //
      // HECI-2 HIDM register
      // It holds HECI-2 interrutp delivery mode
      //
      OperationRegion(H2IM, PCI_Config, 0xA0, 0x1)
      Field(H2IM, ByteAcc, NoLock, Preserve)
      {
        HIDM, 8
      }
      //
      // Define the region for HECI-2 registers mapped to HECI-2 MBAR.
      //
      //OperationRegion(HCSR, PCIBARTarget, 0x10, 0x10)
      // The PCIBARTarget is not implemented in several OSes.
      // The address below is just stub to be updated in UpdateAslCode().
      //
      OperationRegion(H2CS, SystemMemory, 0x8000000080000000, 0x10)
      Field(H2CS, DWordAcc, Lock, Preserve)
      {
        CBWW,32, // write window - R_HECI_HOST_CB_WW
        // Bits of R_HECI_HOST_H_CSR (Host Control Status Register)
        HIE, 1,
        HIS, 1,
        HIG, 1,
        HRD, 1,
        HRS, 1,
           , 3,
        HRP, 8, // host read pointer
        HWP, 8, // host write pointer
        HBD, 8, // host buffer depth
        CBRW,32, // read window - R_HECI_ME_CB_RW
        // Bits of R_HECI_ME_CSR_HA (ME Control Status Register - Host Access)
        MIE, 1,
        MIS, 1,
        MIG, 1,
        MRD, 1,
        MRS, 1,
           , 3,
        MRP, 8, // ME read pointer
        MWP, 8, // ME write pointer
        MBD, 8  // ME buffer depth
      }
      
      //
      // Device status handler method - called by OSPM before _INI
      // This device reports state of the HECI-2 device.
      // First check if HECI-2 is enabled for ACPI. If not do not use it.
      // Then if ME firmware is not operational HECI-2 is reported
      // present but disabled. If ME is operational HECI-2 is present and
      // functioning, but hidden from UI.
      //
      Method(_STA, 0, NotSerialized)
      {
        If (LOr(LNotEqual(HIDM, 0x01), LNot(NMEN)))
        {
          Return(0x00) // HECI-2 is not working with ACPI
        }
        If (LEqual(\_SB.PCI0.HECI.MECS, 5))
        {
          Return(0x0B) // functioning hidden from UI
        }
        Else
        {
          Return(0x01) // present but disabled
        }
      }
      
      //
      // Init method enables HECI-2 on OS boot
      //
      Method(_INI, 0, Serialized)
      {
        If (CondRefOf(\_OSI, Local0))
        {
          DbgTrace("H2: Loading SpsPrAgg")
          If (\_OSI("Processor Aggregator Device"))
          {
            Store(One, \PURE)
            LoadTable("PRAD", ACPI_OEM_ID_MAK, ACPI_OEM_TBL_ID_MAK,,,) // AptioV Server override: Program OEM ID and OEM table ID based on SDL token
          }
        }
        HWAK()
      }
      
      //
      // Wake-up handler method - called from platform _WAK() method
      //
      Method(HWAK, 0, Serialized)
      {
        DbgTrace("H2: HWAK")
        If (LEqual(HIDM, 0x01)) // If SCI interrupt
        {
          Store(1, HRD) // set host ready
          Store(1, HIE) // set interrupt enable
          Store(1, HIG) // kick ME - I'm ready now
        }
      }
      
      //
      // Prepare-to-sleep handler method - called from platform _PTS() method
      //
      Method(HPTS, 0, Serialized)
      {
        DbgTrace("H2: HPTS")
        If (LEqual(HIDM, 0x01)) // If SCI interrupt
        {
          Store(0, HIE) // clear interrupts enable
          Store(0, HRD) //clear host ready
          Store(1, HIG) // kick ME - I'm not ready any more
        }
      }
      
      //
      // Dynamic Cores NotiFy method - sends message via HECI-2
      // Used by Processor Aggregator Device to notify about
      // CPU threads idling status.
      // Arg0 - The body of the message to send.
      //        It contains message sequence number and cores number
      //
      // #define HECIHDR_PROCUTL (0x80040011)
      Method(DCNF, 1, Serialized)
      {
        DbgTrace("H2: DCNF")
        If (LEqual(HIDM, 0x01)) // If SCI interrupt
        {
          Store(0x80040011, CBWW)
          Store(Arg0, CBWW)
          Store(1, HIG)
        }
      }

      //
      // HECI-2 queue read method. It retireves and dispatches ME requests.
      //
      Method(H2RD, 0, Serialized)
      {
        If (LNotEqual(HIDM, 0x01)) // If not SCI interrupt
        {
          Return(0xFF);
        }
        If (MRS) // If reset request from ME - respond with host reset
        {
          DbgTrace("H2: Reset requested")
          Store(1, HRS)
          Store(1, HIG)
        }
        Else
        {
          If (MRD) // If ME is ready
          {
            If (LNot(HRD)) // ME signals ready, make this side ready too
            {
              DbgTrace("H2: Host ready")
              Store(0, HRS)
              Store(1, HRD)
              Store(1, HIG) // kick ME - I'm ready now
            }
            If (LNotEqual(MWP, MRP)) // If buffer not empty
            {
              Store(CBRW, Local1) // Local1 = message header
              Store(CBRW, Local0) // Local0 = TState:PState:SeqNo:Command
              DbgTrace("H2: Got message:")
              DbgTrace(Local1)
              DbgTrace(Local0)
              Store(1, HIG) // message consumed kick ME - queue is not full
              And(Local0, 0xff, Local5) // extract command and store in Local5
              If (LEqual(Local5, 0x00)) // if performance change request
              {
                DbgTrace("H2: Got Performance Change")
                If (\HWPE)
                {
                  //
                  // If Hardware P-states are enabled send
                  // NACK for Performance Change message.
                  //
                  Store(Local1, CBWW)
                  Or(Local0, 0x01, CBWW) // send NACK NM_CMD_MAXPTSTATE_HWPM_NACK
                  Store(1, HIG)
                }
                Else
                {
                  //
                  // Store the current limits and notify processor objects
                  // to recalculate limits.
                  //
                  Store(RPS0, Index(\PSLM, 0))
                  Store(RTS0, Index(\TSLM, 0))
                  Store(RPS1, Index(\PSLM, 1))
                  Store(RTS1, Index(\TSLM, 1))
                  Store(RPS2, Index(\PSLM, 2))
                  Store(RTS2, Index(\TSLM, 2))
                  Store(RPS3, Index(\PSLM, 3))
                  Store(RTS3, Index(\TSLM, 3))
                  If (And(\PETE, 0x80))
                  {
                    DbgTrace("H2: Recalculate P-state")
                    Notify(\_SB.SCK0.CP00, 0x80)
                    Notify(\_SB.SCK0.CP01, 0x80)
                    Notify(\_SB.SCK0.CP02, 0x80)
                    Notify(\_SB.SCK0.CP03, 0x80)
                    Notify(\_SB.SCK0.CP04, 0x80)
                    Notify(\_SB.SCK0.CP05, 0x80)
                    Notify(\_SB.SCK0.CP06, 0x80)
                    Notify(\_SB.SCK0.CP07, 0x80)
                    Notify(\_SB.SCK0.CP08, 0x80)
                    Notify(\_SB.SCK0.CP09, 0x80)
                    Notify(\_SB.SCK0.CP0A, 0x80)
                    Notify(\_SB.SCK0.CP0B, 0x80)
                    Notify(\_SB.SCK0.CP0C, 0x80)
                    Notify(\_SB.SCK0.CP0D, 0x80)
                    Notify(\_SB.SCK0.CP0E, 0x80)
                    Notify(\_SB.SCK0.CP0F, 0x80)
                    Notify(\_SB.SCK0.CP10, 0x80)
                    Notify(\_SB.SCK0.CP11, 0x80)
                    Notify(\_SB.SCK0.CP12, 0x80)
                    Notify(\_SB.SCK0.CP13, 0x80)
                    Notify(\_SB.SCK0.CP14, 0x80)
                    Notify(\_SB.SCK0.CP15, 0x80)
                    Notify(\_SB.SCK0.CP16, 0x80)
                    Notify(\_SB.SCK0.CP17, 0x80)
                    Notify(\_SB.SCK0.CP18, 0x80)
                    Notify(\_SB.SCK0.CP19, 0x80)
                    Notify(\_SB.SCK0.CP1A, 0x80)
                    Notify(\_SB.SCK0.CP1B, 0x80)
                    Notify(\_SB.SCK0.CP1C, 0x80)
                    Notify(\_SB.SCK0.CP1D, 0x80)
                    Notify(\_SB.SCK0.CP1E, 0x80)
                    Notify(\_SB.SCK0.CP1F, 0x80)
                    Notify(\_SB.SCK0.CP20, 0x80)
                    Notify(\_SB.SCK0.CP21, 0x80)
                    Notify(\_SB.SCK0.CP22, 0x80)
                    Notify(\_SB.SCK0.CP23, 0x80)
                    Notify(\_SB.SCK0.CP24, 0x80)
                    Notify(\_SB.SCK0.CP25, 0x80)
                    Notify(\_SB.SCK0.CP26, 0x80)
                    Notify(\_SB.SCK0.CP27, 0x80)
                    Notify(\_SB.SCK0.CP28, 0x80)
                    Notify(\_SB.SCK0.CP29, 0x80)
                    Notify(\_SB.SCK0.CP2A, 0x80)
                    Notify(\_SB.SCK0.CP2B, 0x80)
                    Notify(\_SB.SCK0.CP2C, 0x80)
                    Notify(\_SB.SCK0.CP2D, 0x80)
                    Notify(\_SB.SCK0.CP2E, 0x80)
                    Notify(\_SB.SCK0.CP2F, 0x80)
                    
                    Notify(\_SB.SCK1.CP00, 0x80)
                    Notify(\_SB.SCK1.CP01, 0x80)
                    Notify(\_SB.SCK1.CP02, 0x80)
                    Notify(\_SB.SCK1.CP03, 0x80)
                    Notify(\_SB.SCK1.CP04, 0x80)
                    Notify(\_SB.SCK1.CP05, 0x80)
                    Notify(\_SB.SCK1.CP06, 0x80)
                    Notify(\_SB.SCK1.CP07, 0x80)
                    Notify(\_SB.SCK1.CP08, 0x80)
                    Notify(\_SB.SCK1.CP09, 0x80)
                    Notify(\_SB.SCK1.CP0A, 0x80)
                    Notify(\_SB.SCK1.CP0B, 0x80)
                    Notify(\_SB.SCK1.CP0C, 0x80)
                    Notify(\_SB.SCK1.CP0D, 0x80)
                    Notify(\_SB.SCK1.CP0E, 0x80)
                    Notify(\_SB.SCK1.CP0F, 0x80)
                    Notify(\_SB.SCK1.CP10, 0x80)
                    Notify(\_SB.SCK1.CP11, 0x80)
                    Notify(\_SB.SCK1.CP12, 0x80)
                    Notify(\_SB.SCK1.CP13, 0x80)
                    Notify(\_SB.SCK1.CP14, 0x80)
                    Notify(\_SB.SCK1.CP15, 0x80)
                    Notify(\_SB.SCK1.CP16, 0x80)
                    Notify(\_SB.SCK1.CP17, 0x80)
                    Notify(\_SB.SCK1.CP18, 0x80)
                    Notify(\_SB.SCK1.CP19, 0x80)
                    Notify(\_SB.SCK1.CP1A, 0x80)
                    Notify(\_SB.SCK1.CP1B, 0x80)
                    Notify(\_SB.SCK1.CP1C, 0x80)
                    Notify(\_SB.SCK1.CP1D, 0x80)
                    Notify(\_SB.SCK1.CP1E, 0x80)
                    Notify(\_SB.SCK1.CP1F, 0x80)
                    Notify(\_SB.SCK1.CP20, 0x80)
                    Notify(\_SB.SCK1.CP21, 0x80)
                    Notify(\_SB.SCK1.CP22, 0x80)
                    Notify(\_SB.SCK1.CP23, 0x80)
                    Notify(\_SB.SCK1.CP24, 0x80)
                    Notify(\_SB.SCK1.CP25, 0x80)
                    Notify(\_SB.SCK1.CP26, 0x80)
                    Notify(\_SB.SCK1.CP27, 0x80)
                    Notify(\_SB.SCK1.CP28, 0x80)
                    Notify(\_SB.SCK1.CP29, 0x80)
                    Notify(\_SB.SCK1.CP2A, 0x80)
                    Notify(\_SB.SCK1.CP2B, 0x80)
                    Notify(\_SB.SCK1.CP2C, 0x80)
                    Notify(\_SB.SCK1.CP2D, 0x80)
                    Notify(\_SB.SCK1.CP2E, 0x80)
                    Notify(\_SB.SCK1.CP2F, 0x80)
                    
                    Notify(\_SB.SCK2.CP00, 0x80)
                    Notify(\_SB.SCK2.CP01, 0x80)
                    Notify(\_SB.SCK2.CP02, 0x80)
                    Notify(\_SB.SCK2.CP03, 0x80)
                    Notify(\_SB.SCK2.CP04, 0x80)
                    Notify(\_SB.SCK2.CP05, 0x80)
                    Notify(\_SB.SCK2.CP06, 0x80)
                    Notify(\_SB.SCK2.CP07, 0x80)
                    Notify(\_SB.SCK2.CP08, 0x80)
                    Notify(\_SB.SCK2.CP09, 0x80)
                    Notify(\_SB.SCK2.CP0A, 0x80)
                    Notify(\_SB.SCK2.CP0B, 0x80)
                    Notify(\_SB.SCK2.CP0C, 0x80)
                    Notify(\_SB.SCK2.CP0D, 0x80)
                    Notify(\_SB.SCK2.CP0E, 0x80)
                    Notify(\_SB.SCK2.CP0F, 0x80)
                    Notify(\_SB.SCK2.CP10, 0x80)
                    Notify(\_SB.SCK2.CP11, 0x80)
                    Notify(\_SB.SCK2.CP12, 0x80)
                    Notify(\_SB.SCK2.CP13, 0x80)
                    Notify(\_SB.SCK2.CP14, 0x80)
                    Notify(\_SB.SCK2.CP15, 0x80)
                    Notify(\_SB.SCK2.CP16, 0x80)
                    Notify(\_SB.SCK2.CP17, 0x80)
                    Notify(\_SB.SCK2.CP18, 0x80)
                    Notify(\_SB.SCK2.CP19, 0x80)
                    Notify(\_SB.SCK2.CP1A, 0x80)
                    Notify(\_SB.SCK2.CP1B, 0x80)
                    Notify(\_SB.SCK2.CP1C, 0x80)
                    Notify(\_SB.SCK2.CP1D, 0x80)
                    Notify(\_SB.SCK2.CP1E, 0x80)
                    Notify(\_SB.SCK2.CP1F, 0x80)
                    Notify(\_SB.SCK2.CP20, 0x80)
                    Notify(\_SB.SCK2.CP21, 0x80)
                    Notify(\_SB.SCK2.CP22, 0x80)
                    Notify(\_SB.SCK2.CP23, 0x80)
                    Notify(\_SB.SCK2.CP24, 0x80)
                    Notify(\_SB.SCK2.CP25, 0x80)
                    Notify(\_SB.SCK2.CP26, 0x80)
                    Notify(\_SB.SCK2.CP27, 0x80)
                    Notify(\_SB.SCK2.CP28, 0x80)
                    Notify(\_SB.SCK2.CP29, 0x80)
                    Notify(\_SB.SCK2.CP2A, 0x80)
                    Notify(\_SB.SCK2.CP2B, 0x80)
                    Notify(\_SB.SCK2.CP2C, 0x80)
                    Notify(\_SB.SCK2.CP2D, 0x80)
                    Notify(\_SB.SCK2.CP2E, 0x80)
                    Notify(\_SB.SCK2.CP2F, 0x80)
                    
                    Notify(\_SB.SCK3.CP00, 0x80)
                    Notify(\_SB.SCK3.CP01, 0x80)
                    Notify(\_SB.SCK3.CP02, 0x80)
                    Notify(\_SB.SCK3.CP03, 0x80)
                    Notify(\_SB.SCK3.CP04, 0x80)
                    Notify(\_SB.SCK3.CP05, 0x80)
                    Notify(\_SB.SCK3.CP06, 0x80)
                    Notify(\_SB.SCK3.CP07, 0x80)
                    Notify(\_SB.SCK3.CP08, 0x80)
                    Notify(\_SB.SCK3.CP09, 0x80)
                    Notify(\_SB.SCK3.CP0A, 0x80)
                    Notify(\_SB.SCK3.CP0B, 0x80)
                    Notify(\_SB.SCK3.CP0C, 0x80)
                    Notify(\_SB.SCK3.CP0D, 0x80)
                    Notify(\_SB.SCK3.CP0E, 0x80)
                    Notify(\_SB.SCK3.CP0F, 0x80)
                    Notify(\_SB.SCK3.CP10, 0x80)
                    Notify(\_SB.SCK3.CP11, 0x80)
                    Notify(\_SB.SCK3.CP12, 0x80)
                    Notify(\_SB.SCK3.CP13, 0x80)
                    Notify(\_SB.SCK3.CP14, 0x80)
                    Notify(\_SB.SCK3.CP15, 0x80)
                    Notify(\_SB.SCK3.CP16, 0x80)
                    Notify(\_SB.SCK3.CP17, 0x80)
                    Notify(\_SB.SCK3.CP18, 0x80)
                    Notify(\_SB.SCK3.CP19, 0x80)
                    Notify(\_SB.SCK3.CP1A, 0x80)
                    Notify(\_SB.SCK3.CP1B, 0x80)
                    Notify(\_SB.SCK3.CP1C, 0x80)
                    Notify(\_SB.SCK3.CP1D, 0x80)
                    Notify(\_SB.SCK3.CP1E, 0x80)
                    Notify(\_SB.SCK3.CP1F, 0x80)
                    Notify(\_SB.SCK3.CP20, 0x80)
                    Notify(\_SB.SCK3.CP21, 0x80)
                    Notify(\_SB.SCK3.CP22, 0x80)
                    Notify(\_SB.SCK3.CP23, 0x80)
                    Notify(\_SB.SCK3.CP24, 0x80)
                    Notify(\_SB.SCK3.CP25, 0x80)
                    Notify(\_SB.SCK3.CP26, 0x80)
                    Notify(\_SB.SCK3.CP27, 0x80)
                    Notify(\_SB.SCK3.CP28, 0x80)
                    Notify(\_SB.SCK3.CP29, 0x80)
                    Notify(\_SB.SCK3.CP2A, 0x80)
                    Notify(\_SB.SCK3.CP2B, 0x80)
                    Notify(\_SB.SCK3.CP2C, 0x80)
                    Notify(\_SB.SCK3.CP2D, 0x80)
                    Notify(\_SB.SCK3.CP2E, 0x80)
                    Notify(\_SB.SCK3.CP2F, 0x80)
                  }
                  If (And(\PETE, 0x40))
                  {
                    DbgTrace("H2: Recalculate T-state")
                    Notify(\_SB.SCK0.CP00, 0x82)
                    Notify(\_SB.SCK0.CP01, 0x82)
                    Notify(\_SB.SCK0.CP02, 0x82)
                    Notify(\_SB.SCK0.CP03, 0x82)
                    Notify(\_SB.SCK0.CP04, 0x82)
                    Notify(\_SB.SCK0.CP05, 0x82)
                    Notify(\_SB.SCK0.CP06, 0x82)
                    Notify(\_SB.SCK0.CP07, 0x82)
                    Notify(\_SB.SCK0.CP08, 0x82)
                    Notify(\_SB.SCK0.CP09, 0x82)
                    Notify(\_SB.SCK0.CP0A, 0x82)
                    Notify(\_SB.SCK0.CP0B, 0x82)
                    Notify(\_SB.SCK0.CP0C, 0x82)
                    Notify(\_SB.SCK0.CP0D, 0x82)
                    Notify(\_SB.SCK0.CP0E, 0x82)
                    Notify(\_SB.SCK0.CP0F, 0x82)
                    Notify(\_SB.SCK0.CP10, 0x82)
                    Notify(\_SB.SCK0.CP11, 0x82)
                    Notify(\_SB.SCK0.CP12, 0x82)
                    Notify(\_SB.SCK0.CP13, 0x82)
                    Notify(\_SB.SCK0.CP14, 0x82)
                    Notify(\_SB.SCK0.CP15, 0x82)
                    Notify(\_SB.SCK0.CP16, 0x82)
                    Notify(\_SB.SCK0.CP17, 0x82)
                    Notify(\_SB.SCK0.CP18, 0x82)
                    Notify(\_SB.SCK0.CP19, 0x82)
                    Notify(\_SB.SCK0.CP1A, 0x82)
                    Notify(\_SB.SCK0.CP1B, 0x82)
                    Notify(\_SB.SCK0.CP1C, 0x82)
                    Notify(\_SB.SCK0.CP1D, 0x82)
                    Notify(\_SB.SCK0.CP1E, 0x82)
                    Notify(\_SB.SCK0.CP1F, 0x82)
                    Notify(\_SB.SCK0.CP20, 0x82)
                    Notify(\_SB.SCK0.CP21, 0x82)
                    Notify(\_SB.SCK0.CP22, 0x82)
                    Notify(\_SB.SCK0.CP23, 0x82)
                    Notify(\_SB.SCK0.CP24, 0x82)
                    Notify(\_SB.SCK0.CP25, 0x82)
                    Notify(\_SB.SCK0.CP26, 0x82)
                    Notify(\_SB.SCK0.CP27, 0x82)
                    Notify(\_SB.SCK0.CP28, 0x82)
                    Notify(\_SB.SCK0.CP29, 0x82)
                    Notify(\_SB.SCK0.CP2A, 0x82)
                    Notify(\_SB.SCK0.CP2B, 0x82)
                    Notify(\_SB.SCK0.CP2C, 0x82)
                    Notify(\_SB.SCK0.CP2D, 0x82)
                    Notify(\_SB.SCK0.CP2E, 0x82)
                    Notify(\_SB.SCK0.CP2F, 0x82)
                    
                    Notify(\_SB.SCK1.CP00, 0x82)
                    Notify(\_SB.SCK1.CP01, 0x82)
                    Notify(\_SB.SCK1.CP02, 0x82)
                    Notify(\_SB.SCK1.CP03, 0x82)
                    Notify(\_SB.SCK1.CP04, 0x82)
                    Notify(\_SB.SCK1.CP05, 0x82)
                    Notify(\_SB.SCK1.CP06, 0x82)
                    Notify(\_SB.SCK1.CP07, 0x82)
                    Notify(\_SB.SCK1.CP08, 0x82)
                    Notify(\_SB.SCK1.CP09, 0x82)
                    Notify(\_SB.SCK1.CP0A, 0x82)
                    Notify(\_SB.SCK1.CP0B, 0x82)
                    Notify(\_SB.SCK1.CP0C, 0x82)
                    Notify(\_SB.SCK1.CP0D, 0x82)
                    Notify(\_SB.SCK1.CP0E, 0x82)
                    Notify(\_SB.SCK1.CP0F, 0x82)
                    Notify(\_SB.SCK1.CP10, 0x82)
                    Notify(\_SB.SCK1.CP11, 0x82)
                    Notify(\_SB.SCK1.CP12, 0x82)
                    Notify(\_SB.SCK1.CP13, 0x82)
                    Notify(\_SB.SCK1.CP14, 0x82)
                    Notify(\_SB.SCK1.CP15, 0x82)
                    Notify(\_SB.SCK1.CP16, 0x82)
                    Notify(\_SB.SCK1.CP17, 0x82)
                    Notify(\_SB.SCK1.CP18, 0x82)
                    Notify(\_SB.SCK1.CP19, 0x82)
                    Notify(\_SB.SCK1.CP1A, 0x82)
                    Notify(\_SB.SCK1.CP1B, 0x82)
                    Notify(\_SB.SCK1.CP1C, 0x82)
                    Notify(\_SB.SCK1.CP1D, 0x82)
                    Notify(\_SB.SCK1.CP1E, 0x82)
                    Notify(\_SB.SCK1.CP1F, 0x82)
                    Notify(\_SB.SCK1.CP20, 0x82)
                    Notify(\_SB.SCK1.CP21, 0x82)
                    Notify(\_SB.SCK1.CP22, 0x82)
                    Notify(\_SB.SCK1.CP23, 0x82)
                    Notify(\_SB.SCK1.CP24, 0x82)
                    Notify(\_SB.SCK1.CP25, 0x82)
                    Notify(\_SB.SCK1.CP26, 0x82)
                    Notify(\_SB.SCK1.CP27, 0x82)
                    Notify(\_SB.SCK1.CP28, 0x82)
                    Notify(\_SB.SCK1.CP29, 0x82)
                    Notify(\_SB.SCK1.CP2A, 0x82)
                    Notify(\_SB.SCK1.CP2B, 0x82)
                    Notify(\_SB.SCK1.CP2C, 0x82)
                    Notify(\_SB.SCK1.CP2D, 0x82)
                    Notify(\_SB.SCK1.CP2E, 0x82)
                    Notify(\_SB.SCK1.CP2F, 0x82)
                    
                    Notify(\_SB.SCK2.CP00, 0x82)
                    Notify(\_SB.SCK2.CP01, 0x82)
                    Notify(\_SB.SCK2.CP02, 0x82)
                    Notify(\_SB.SCK2.CP03, 0x82)
                    Notify(\_SB.SCK2.CP04, 0x82)
                    Notify(\_SB.SCK2.CP05, 0x82)
                    Notify(\_SB.SCK2.CP06, 0x82)
                    Notify(\_SB.SCK2.CP07, 0x82)
                    Notify(\_SB.SCK2.CP08, 0x82)
                    Notify(\_SB.SCK2.CP09, 0x82)
                    Notify(\_SB.SCK2.CP0A, 0x82)
                    Notify(\_SB.SCK2.CP0B, 0x82)
                    Notify(\_SB.SCK2.CP0C, 0x82)
                    Notify(\_SB.SCK2.CP0D, 0x82)
                    Notify(\_SB.SCK2.CP0E, 0x82)
                    Notify(\_SB.SCK2.CP0F, 0x82)
                    Notify(\_SB.SCK2.CP10, 0x82)
                    Notify(\_SB.SCK2.CP11, 0x82)
                    Notify(\_SB.SCK2.CP12, 0x82)
                    Notify(\_SB.SCK2.CP13, 0x82)
                    Notify(\_SB.SCK2.CP14, 0x82)
                    Notify(\_SB.SCK2.CP15, 0x82)
                    Notify(\_SB.SCK2.CP16, 0x82)
                    Notify(\_SB.SCK2.CP17, 0x82)
                    Notify(\_SB.SCK2.CP18, 0x82)
                    Notify(\_SB.SCK2.CP19, 0x82)
                    Notify(\_SB.SCK2.CP1A, 0x82)
                    Notify(\_SB.SCK2.CP1B, 0x82)
                    Notify(\_SB.SCK2.CP1C, 0x82)
                    Notify(\_SB.SCK2.CP1D, 0x82)
                    Notify(\_SB.SCK2.CP1E, 0x82)
                    Notify(\_SB.SCK2.CP1F, 0x82)
                    Notify(\_SB.SCK2.CP20, 0x82)
                    Notify(\_SB.SCK2.CP21, 0x82)
                    Notify(\_SB.SCK2.CP22, 0x82)
                    Notify(\_SB.SCK2.CP23, 0x82)
                    Notify(\_SB.SCK2.CP24, 0x82)
                    Notify(\_SB.SCK2.CP25, 0x82)
                    Notify(\_SB.SCK2.CP26, 0x82)
                    Notify(\_SB.SCK2.CP27, 0x82)
                    Notify(\_SB.SCK2.CP28, 0x82)
                    Notify(\_SB.SCK2.CP29, 0x82)
                    Notify(\_SB.SCK2.CP2A, 0x82)
                    Notify(\_SB.SCK2.CP2B, 0x82)
                    Notify(\_SB.SCK2.CP2C, 0x82)
                    Notify(\_SB.SCK2.CP2D, 0x82)
                    Notify(\_SB.SCK2.CP2E, 0x82)
                    Notify(\_SB.SCK2.CP2F, 0x82)
                    
                    Notify(\_SB.SCK3.CP00, 0x82)
                    Notify(\_SB.SCK3.CP01, 0x82)
                    Notify(\_SB.SCK3.CP02, 0x82)
                    Notify(\_SB.SCK3.CP03, 0x82)
                    Notify(\_SB.SCK3.CP04, 0x82)
                    Notify(\_SB.SCK3.CP05, 0x82)
                    Notify(\_SB.SCK3.CP06, 0x82)
                    Notify(\_SB.SCK3.CP07, 0x82)
                    Notify(\_SB.SCK3.CP08, 0x82)
                    Notify(\_SB.SCK3.CP09, 0x82)
                    Notify(\_SB.SCK3.CP0A, 0x82)
                    Notify(\_SB.SCK3.CP0B, 0x82)
                    Notify(\_SB.SCK3.CP0C, 0x82)
                    Notify(\_SB.SCK3.CP0D, 0x82)
                    Notify(\_SB.SCK3.CP0E, 0x82)
                    Notify(\_SB.SCK3.CP0F, 0x82)
                    Notify(\_SB.SCK3.CP10, 0x82)
                    Notify(\_SB.SCK3.CP11, 0x82)
                    Notify(\_SB.SCK3.CP12, 0x82)
                    Notify(\_SB.SCK3.CP13, 0x82)
                    Notify(\_SB.SCK3.CP14, 0x82)
                    Notify(\_SB.SCK3.CP15, 0x82)
                    Notify(\_SB.SCK3.CP16, 0x82)
                    Notify(\_SB.SCK3.CP17, 0x82)
                    Notify(\_SB.SCK3.CP18, 0x82)
                    Notify(\_SB.SCK3.CP19, 0x82)
                    Notify(\_SB.SCK3.CP1A, 0x82)
                    Notify(\_SB.SCK3.CP1B, 0x82)
                    Notify(\_SB.SCK3.CP1C, 0x82)
                    Notify(\_SB.SCK3.CP1D, 0x82)
                    Notify(\_SB.SCK3.CP1E, 0x82)
                    Notify(\_SB.SCK3.CP1F, 0x82)
                    Notify(\_SB.SCK3.CP20, 0x82)
                    Notify(\_SB.SCK3.CP21, 0x82)
                    Notify(\_SB.SCK3.CP22, 0x82)
                    Notify(\_SB.SCK3.CP23, 0x82)
                    Notify(\_SB.SCK3.CP24, 0x82)
                    Notify(\_SB.SCK3.CP25, 0x82)
                    Notify(\_SB.SCK3.CP26, 0x82)
                    Notify(\_SB.SCK3.CP27, 0x82)
                    Notify(\_SB.SCK3.CP28, 0x82)
                    Notify(\_SB.SCK3.CP29, 0x82)
                    Notify(\_SB.SCK3.CP2A, 0x82)
                    Notify(\_SB.SCK3.CP2B, 0x82)
                    Notify(\_SB.SCK3.CP2C, 0x82)
                    Notify(\_SB.SCK3.CP2D, 0x82)
                    Notify(\_SB.SCK3.CP2E, 0x82)
                    Notify(\_SB.SCK3.CP2F, 0x82)
                    Store(DeRefOf(Index(\TSLM, 0)), CTS0)
                    Store(DeRefOf(Index(\TSLM, 1)), CTS1)
                    Store(DeRefOf(Index(\TSLM, 2)), CTS2)
                    Store(DeRefOf(Index(\TSLM, 3)), CTS3)
                  }
                  If (And(\PETE, 0x80))
                  {
                    Store(DeRefOf(Index(\PSLM, 0)), CPS0)
                    Store(DeRefOf(Index(\PSLM, 1)), CPS1)
                    Store(DeRefOf(Index(\PSLM, 2)), CPS2)
                    Store(DeRefOf(Index(\PSLM, 3)), CPS3)
                  }
                  //
                  // Send ACK for P/T-state message
                  //
                  Store(Local1, CBWW)
                  Or(Local0, And(\PETE, 0xff), CBWW)
                  Store(1, HIG)
                }
              } // If cmd is HECI_CMD_MAXPTSTATE
              ElseIf (LEqual(Local5, 0x03)) // If processor utilisation request
              {
                DbgTrace("H2: Got Processor Utilisation")
                If (\PURE)
                {
                  Store(Local0, \PURA)
                  Notify(\_SB.PRAD, 0x80)
                }
                else
                {
                  DCNF(Local0)
                }
              }
#if POWER_METER
              ElseIf (LEqual(Local5, 0x04)) // if Power Meter measurement req
              {
                Store(Multiply(And(ShiftRight(Local0, 0x10), 0xFFFF), 1000), PWR)
                DCNF(Or(Local0, 0x80))
              }
              ElseIf (LEqual(Local5, 0x05)) // if Power Meter get averaging interval req
              {
                Store(Multiply(And(ShiftRight(Local0, 0x10), 0xFFFF), 1000), PMAI)
                DCNF(Or(Local0, 0x80))
                Notify(\_SB.PMT1, 0x84)
              }
              ElseIf (LEqual(Local5, 0x06)) // if get hardware limit req
              {
                Store(Multiply(And(ShiftRight(Local0, 0x10), 0xFFFF), 1000), HWL)
                DCNF(Or(Local0, 0x80))
                Notify(\_SB.PMT1, 0x82)
              }
              ElseIf (LEqual(Local5, 0x0C)) // if power state change req
              {
                // ACK: Rsvd:Active:SeqNo:Command
                DCNF(Or(ShiftLeft(PMAV, 0x10), And(Or(Local0, 0x80), 0xFFFF)))
                If (LEqual(1, PMAV)) // If Power Meter is active
                {
                  // capabilities possible change
                  If (LEqual(And(ShiftRight(Local0, 0x10), 0xFF), 1))
                  {
                    \_SB.PCI0.HEC2.SSFW(0, 0x08, 0, 0)
                  }
                  Store(Divide(HWL, 1000), Local2)
                  \_SB.PCI0.HEC2.SSFW(0, 0x07, Local2, 0) // Set HWL
                  Store(Divide(PMAI, 1000), Local2)
                  \_SB.PCI0.HEC2.SSFW(0, 0x0A, Local2, 0) // Set PAI
                  Store(Divide(PTPU, 1000), Local2)
                  Store(Divide(PTPL, 1000), Local3)
                  \_SB.PCI0.HEC2.SSFW(1, 0x0B, Local2, Local3) // Set PAI
                }
              }
             // Get Current Capabilities ACK , Report Capabilities REQ
              ElseIf (Or(LEqual(Local5, 0x88),   // if get current capabilities ack,
                          LEqual(Local5, 0x0D))) //    or report capabilities req
              {
                Store(And(ShiftRight(Local0, 0x18), 0xFF), Index(PMCP, 0)) // supported capabilities
                Store(And(ShiftRight(Local0, 0x11), 0x01), Index(PMCP, 2)) // measurement type
                If (LEqual(And(ShiftRight(Local0, 0x10), 0x01), 0x01))    // Is hw limit configurable
                {
                  Store(0xFFFFFFFF, Index(PMCP, 8))
                }
                Else
                {
                  Store(0x00000000, Index(PMCP, 8))
                }
                Store(CBRW, Index(PMCP, 3))
                Store(CBRW, Index(PMCP, 4))
                Store(CBRW, Index(PMCP, 5))
                Store(CBRW, Index(PMCP, 6))
                Store(CBRW, Index(PMCP, 7))
                Store(CBRW, Index(PMCP, 9))
                Store(CBRW, Index(PMCP, 10))
                If (LEqual(Local5, 0x0D))
                {
                  DCNF(And(Or(Local0, 0x80), 0xFFFF))
                }
                Else
                {
                  Store(1, HIG)
                }
                Store(1, PMAV)
                Notify(\_SB.PMT1, 0x80)
              }
              ElseIf (LEqual(Local5, 0x09)) // Power Trip Points exceeded REQ
              {
                DCNF(Or(Local0, 0x80))
                Notify(\_SB.PMT1, 0x81)
              }
              ElseIf (LEqual(Local5, 0x87)) // Set Hardware Limit ACK
              {
                Store(And(ShiftRight(Local0, 0x08), 0xFF), HLST)
              }
              ElseIf (LEqual(Local5, 0x8A)) // Power Averaging Interval ACK
              {
                Store(And(ShiftRight(Local0, 0x08), 0xFF), AIST)
              }
              ElseIf (LEqual(Local5, 0x8B)) // Power Trip Points ACK
              {
                Store(And(ShiftRight(Local0, 0x08), 0xFF), TPST)
              }
#endif // POWER_METER
              Return(Local5) // Command is returned by H2RD
            } // If (buffer not empty)
          } // If (MRD)
        } // if (MRS )
        Return(0xFF) // There is no command to return
      } // Method(H2RD,...)
      
      //
      // SCI handler method - called from _L24() SCI handler
      // 
      Method(HSCI, 0, Serialized)
      {
        DbgTrace("H2: SCI Handler for HECI-2")
        If (HIS) // If Host Interrupt Status is Set (when ME_IG is set to 1)
        {
          Store(1, HIS)    // clear interrupt status before reading queue
          Store(0, Local0) // clear timeout counter
          While (And(LNotEqual(H2RD(), 0xFF), LNotEqual(Local0, 8)))
          {
            Add(Local0, 1, Local0)
          }
        } // If (HIS)
        Store(1, HIE)
      } // Method(HSCI,...)
#if POWER_METER
      //
      // Send request to ME and wait for answer.
      // It sends to ME message frame of 1 or 2 DWORDs,
      // Message header plus depending on input parameter.
      // Arg0  Message size: 0 - 1 DWORD (_SHL, _PAI), 1 - 2 DWORDS (_PTP).
      // Arg1  Command.
      // Arg2  Setting param. Depending on command, it represents
      //         Hardware limit [W], or averaging interval [s], or upper trip point [W]
      // Arg3  Setting param in 2 DWORDs message. Lower trip point [sec].
      // 
      Method(SSFW, 4, Serialized)
      {
        Store(HIE, Local1) // Save HECI-2 SCI interrupt enable state
        Store(0, HIE)      // Disable HECI-2 SCI interrupt
        If (LEqual (Arg0, 0)) 
        {
          Store(0x80040011, CBWW)
          Store(Or(ShiftLeft(Arg2, 0x10), And(Arg1, 0x00FF)), CBWW)
        }
        ElseIf (LEqual(Arg0, 1))
        {
          Store(0x80080011, CBWW)
          Store(And (Arg1, 0xFF), CBWW)
          Store(Or(ShiftLeft(Arg3, 0x10), And(Arg2, 0xFFFF)), CBWW)
        }
        Store(1, HIG)
        Store(0, Local0)  // Timeout counter
        While (LNotEqual(Local0, 10))
        {
          If (LEqual(1, HIS))
          {
            Store(1, HIS) // Clear interrupt status
            If (LEqual(H2RD(), Or(Arg1, 0x80)))
            {
              Break
            }
          }
          Add(Local0, 1, Local0)
          Sleep(100)
        }
        Store(Local1, HIE)  // Restore HECI-2 SCI interrupt enable state
      }
#endif // POWER_METER
#if 0
    } // Device(HEC2)
#endif
  } // Scope(SB.PCI0)
  
#if POWER_METER
  Scope (\_SB)
  {
    Name(PWR, 0xFFFFFFFF)  // Power reading
    Name(PMAI, 0xFFFFFFFF) // Power meter averaging interval
    Name(HWL, 0xFFFFFFFF)  // HW limit
    Name(PTPU, 0)          // Upper Trip Point
    Name(PTPL, 0)          // Lower Trip Point
    Name(PMAV, 0)          // Is Power Meter inferface active
    Name(HLST, 0)          // Set HW Limit Status, return code
    Name(AIST, 0)          // Set Averaging Interval Status, return code
    Name(TPST, 0)          // Set Trip Points Status, return code
    
    // Power Meter capabilities
    Name(PMCP, Package()
    {
      0x00000000, // Supported capabilities, (from LSB) -> measurement, trip points, HW enforced limit, notifications when HW limit is enforced, 3 x reserved, report only when discharging
      0x00000000, // Measurement unit, mW only 
      0x00000000, // Measurement input power
      0x00000000, // Accuracy of PMT measurements, 95000 = 95%
      0xFFFFFFFF, // Sampling time of PMT in ms
      0x00000000, // Min averaging interval in ms
      0x00000000, // Max averaging interval in ms
      0xFFFFFFFF, // Hysteresis margin (mW)
      0x00000000, // Is HW limit configurable, 0x00000000 = read only, 0xFFFFFFFF = writable
      0x00000000, // Min configurable HW limit (mW)
      0x00000000, // Max configurable HW limit (mW)
      "Intel(R) Node Manager",// PMT model number, empty string if not supported
      "",         // PMT serial number,  empty string if not supported
      "Meter measures total domain" // OEM Info
    })
    
    Device(PMT1) //Power Meter # 1
    {
      Name(_HID, "ACPI000D")
      Name(_UID,  0x00)      // Unique ID required if many power meters

      //
      // Device status handler method - called by OSPM before _INI.
      // Power Meter status follows HECI-2 status but is not hidden from UI.
      //
      Method(_STA, 0, NotSerialized)
      {
        Return(Or(\_SB.PCI0.HEC2._STA, 0x04))
      }
      //
      //Power Meter Capabilities
      //
      Method(_PMC, 0, NotSerialized)
      {
        If (LEqual(PMAV, 0))
        {
          \_SB.PCI0.HEC2.SSFW(0, 0x08, 0, 0)
        }
        Return(PMCP)
      }
      
      //
      //Power Meter Devices
      //
      Method(_PMD, 0, NotSerialized)
      {
        Return(Package(){\_SB})
      }
      
      //
      // Power Meter Measurement
      //
      Method(_PMM, 0, NotSerialized)
      {
        Return(PWR)
      }
      
      //
      // Power Meter Averaging Interval
      //
      Method(_PAI, 1, NotSerialized)
      {
        Store(Divide(Arg0, 1000), Local0)
        Store(0x00000002, AIST)
        \_SB.PCI0.HEC2.SSFW(0, 0x0A, Local0, 0) 
        If (LEqual(And (AIST, 0xFF), 0x00))
        {
          Store(Arg0, PMAI)
          Notify(\_SB.PMT1, 0x84)
        }
        Return(AIST)
      }
      
      //
      // Get Averaging Interval
      //
      Method(_GAI, 0, NotSerialized)
      {
        Return(PMAI)
      }
      
      //
      // Power Trip Points
      //
      Method(_PTP, 2, NotSerialized)
      {
        Store(Divide(Arg0, 1000), Local0)
        //
        // Round lower trip point to higher value
        //
        Store(Add(Arg1, 500), Local1)
        Store(Divide(Arg1, 1000), Local1)
        Store(0x00000002, TPST)
        \_SB.PCI0.HEC2.SSFW(1, 0x0B, Local0, Local1)
        If (LEqual(And(TPST, 0xFF), 0))
        {
          Store(Arg0, PTPU)
          Store(Arg1, PTPL)
        }
        Return (TPST)
      }
      
      //
      //Get Hardware Limit
      //
      Method(_GHL, 0, NotSerialized)
      {
        Return(HWL)
      }
      
      //
      // Set Hardware Limit
      //
      Method(_SHL, 1, NotSerialized)
      {
        Store(Divide(Arg0, 1000), Local0)
        Store(0x00000002, HLST)
        \_SB.PCI0.HEC2.SSFW (0, 0x07, Local0, 0)
        If (LEqual(And(HLST, 0xFF), 0x00))
        {
          Store(Arg0, HWL)
          Notify(\_SB.PMT1, 0x83)
        }
        Return (HLST)
      }
    } // Device(PMT1)
  } // Scope(SB)
#endif // POWER_METER

  Scope(\_GPE)
  {
    Method(_L24, 0x0, Serialized) 
    { 
      \_SB.PCI0.HEC2.HSCI()
    }
  }
}// Definition Block

