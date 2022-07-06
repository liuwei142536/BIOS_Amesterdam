//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2013, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

//**********************************************************************
//<AMI_FHDR_START>
//
// Name:  GPE_PME.ASL
//
// Description:	GPE PME handler implementation
//
//<AMI_FHDR_END>
//**********************************************************************

  OperationRegion(PXCS,PCI_Config,0x00,0xE0)
  Field(PXCS,AnyAcc, NoLock, Preserve)
  {      
    Offset(0),
    VDID, 32,
  //AptioV server override :support hot plug function for SB start
    Offset(0x54), // SLCAP - Slot Capabilities Register
    , 6,
    HPCE, 1,      // 6, Hot Plug Capable
    Offset(0x5A), // SLSTS[7:0] - Slot Status Register
    ABPX, 1,      // 0, Attention Button Pressed
    , 2,
    PDCX, 1,      // 3, Presence Detect Changed
    , 2,
    PDSX, 1,      // 6, Presence Detect State       
    , 1,
  //AptioV server override :support hot plug function for SB end
    Offset(0x60), // RSTS Root Status Register
    , 16,
    PMEX, 1,      // 16,  PME Status

    Offset(0xDC), // SMSCS - SMI/SCI Status Register
    , 31,
    PMCS, 1       // 31,  Power Management SCI Status 
  }

  Method(DEVS, 0x0, NotSerialized)
  {
    If(LEqual(VDID, 0xFFFFFFFF)){
      Return(0x00)
    } Else {
      Return(0x0F)
    }
  }

  //
  // PCI_EXP_STS Handler for PCIE Root Port
  //
  Method(HPME,0,Serialized)
  {
    //
    // 1. Read the PME Status bit (D28:F0~F7:60h[16]) of the root port. If the bit is clear, 
    // then go to step 5.
    //
    If(PMEX) {

      //
      // Clear the PME SCI status bit with timeout
      //
      Store(200,Local0)

      While(Local0) {

        //
        // 2. Clear the PME Status bit, D28:F0~F7:60h[16], by setting it.
        //
        Store(1, PMEX)

        //
        // 3. Check the PME Status bit to see if it is set again (in case there was another PME 
        // event pending). If it is set, then go back to step 2.
        //
        If(PMEX) {
          Decrement(Local0)
        } else {
          Store(0,Local0)
        } // If(PMEX)

      } // While(Local0)

      //
      // 4. Clear the PMCS status bit (D28:F0~F7:DCh[31]) by setting it. 
      //
      Store(1, PMCS)

    } // If(PMEX)

  } // Method(HPME)

  Method(_PRW, 0) { Return(GPRW(0x09, 4)) }	// can wakeup from S4 state
  
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2013, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
