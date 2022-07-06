/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++
  
  Copyright (c)  2001-2013 Intel Corporation.  All rights reserved.

  This software and associated documentation (if any) is furnished under 
  a license and may only be used or copied in accordance with the terms 
  of the license.  Except as permitted by such license, no part of this 
  software or documentation may be reproduced, stored in a retrieval system, 
  or transmitted in any form or by any means without the express written 
  consent of Intel Corporation.
  
  Module Name:
  
    PRAD.asl
  
  Abstract:

    This file contains Processor Aggregator support for Node Manager (NM)
    in SPS ME firmware.
  
--*/

DefinitionBlock("SpsProcAggr.aml", "PRAD", 2, "INTEL", "SpsPrAgg", 2)
{
  External(\PURA, IntObj)
  External(\PURB, IntObj)
  External(\THNU, FieldUnitObj)
  External(\_SB.PCI0, DeviceObj)
  External(\_SB.PCI0.HEC2.DCNF, MethodObj)

  Scope(\_SB)
  {
    //
    // Processor Aggregator Device
    //
    Device(PRAD)
    {
      Name(_HID, "ACPI000C")
      
      Method(_STA, 0, NotSerialized)
      {
        Return(0x0F)
      }
      
      //
      // Processor Utilization Request method returns the
      // number of processors that platform wants to idle.
      //
      Method(_PUR, 0, NotSerialized)
      {
        Store("PRAD: Entering _PUR", Debug)
        Name(IDL, Package() {1, 0})
        And(Or(\PURA, 0x80), 0xFFFF, \PURB)
        And(ShiftRight(\PURA, 16), 0xffff, Index(IDL, 1)) 
        Return(IDL)
      }
      
      //
      // OSPM Status Indication method
      //
      Method(_OST, 3, Serialized)
      {
        Store("PRAD: Entering _OST", Debug)
        //
        // If _OST is called after _PUR send ACK to ME.
        // Arg2 conveys number of threads currently idled.
        // Calculate number of active threads and send it to ME.
        //
        If (LEqual(And(\PURB, 0xff), 0x83))
        {
          Subtract(\THNU, ToInteger(Arg2), Local0)
          Or(\PURB, ShiftLeft(Local0, 16), \PURB)
          \_SB.PCI0.HEC2.DCNF(\PURB) // Send ACK for core idling
        }
      } // _OST
    } // Devide PRAD
  } // Scope SB
} // Definition Block

