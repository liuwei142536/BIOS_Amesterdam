//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/*++

  Copyright (c)  2001 - 2015 Intel Corporation.  All rights reserved.

  This software and associated documentation (if any) is furnished under 
  a license and may only be used or copied in accordance with the terms 
  of the license.  Except as permitted by such license, no part of this 
  software or documentation may be reproduced, stored in a retrieval 
  system, or transmitted in any form or by any means without the express 
  written consent of Intel Corporation.

  Module Name:

    IioPcieHotPlugGpeHandler.asl

  Abstract:

    Contains root level name space objects for the platform

--*/

  //
  // Delay introduced as initial delay after entering ACPI hotplug method 
  //
  Sleep (200)
  Store (0x01, IO80)
  Sleep (10)
  Store (0,Local1)

  // PCI0 Port 1A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI0.BR1A.PMEP,1) ) {
    Store(\_SB.PCI0.BR1A.PMEH(9), Local0)
  } else {
    Store (\_SB.PCI0.BR1A.HPEH(9), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x01, Local1)
    Notify(\_SB.PCI0.BR1A.H000, Local0)
    Notify(\_SB.PCI0.BR1A.H001, Local0)
    Notify(\_SB.PCI0.BR1A.H002, Local0)
    Notify(\_SB.PCI0.BR1A.H003, Local0)
    Notify(\_SB.PCI0.BR1A.H004, Local0)
    Notify(\_SB.PCI0.BR1A.H005, Local0)
    Notify(\_SB.PCI0.BR1A.H006, Local0)
    Notify(\_SB.PCI0.BR1A.H007, Local0)
  }

  // PCI0 Port 1B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI0.BR1B.PMEP,1) ) {
    Store(\_SB.PCI0.BR1B.PMEH(9), Local0)
  } else {
    Store (\_SB.PCI0.BR1B.HPEH(9), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x02, Local1)
    Notify(\_SB.PCI0.BR1B.H000, Local0)
    Notify(\_SB.PCI0.BR1B.H001, Local0)
    Notify(\_SB.PCI0.BR1B.H002, Local0)
    Notify(\_SB.PCI0.BR1B.H003, Local0)
    Notify(\_SB.PCI0.BR1B.H004, Local0)
    Notify(\_SB.PCI0.BR1B.H005, Local0)
    Notify(\_SB.PCI0.BR1B.H006, Local0)
    Notify(\_SB.PCI0.BR1B.H007, Local0)
  }

  // PCI0 Port 2A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI0.BR2A.PMEP,1) ) {
    Store(\_SB.PCI0.BR2A.PMEH(1), Local0)
  } else {
    Store (\_SB.PCI0.BR2A.HPEH(1), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x03, Local1)
    Notify(\_SB.PCI0.BR2A.H000, Local0)
    Notify(\_SB.PCI0.BR2A.H001, Local0)
    Notify(\_SB.PCI0.BR2A.H002, Local0)
    Notify(\_SB.PCI0.BR2A.H003, Local0)
    Notify(\_SB.PCI0.BR2A.H004, Local0)
    Notify(\_SB.PCI0.BR2A.H005, Local0)
    Notify(\_SB.PCI0.BR2A.H006, Local0)
    Notify(\_SB.PCI0.BR2A.H007, Local0)
  }

  // PCI0 Port 2B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI0.BR2B.PMEP,1) ) {
    Store(\_SB.PCI0.BR2B.PMEH(2), Local0)
  } else {
    Store (\_SB.PCI0.BR2B.HPEH(2), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x04, Local1)
    Notify(\_SB.PCI0.BR2B.H000, Local0)
    Notify(\_SB.PCI0.BR2B.H001, Local0)
    Notify(\_SB.PCI0.BR2B.H002, Local0)
    Notify(\_SB.PCI0.BR2B.H003, Local0)
    Notify(\_SB.PCI0.BR2B.H004, Local0)
    Notify(\_SB.PCI0.BR2B.H005, Local0)
    Notify(\_SB.PCI0.BR2B.H006, Local0)
    Notify(\_SB.PCI0.BR2B.H007, Local0)
  }

  // PCI0 Port 2C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI0.BR2C.PMEP,1) ) {
    Store(\_SB.PCI0.BR2C.PMEH(3), Local0)
  } else {
    Store (\_SB.PCI0.BR2C.HPEH(3), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x05, Local1)
    Notify(\_SB.PCI0.BR2C.H000, Local0)
    Notify(\_SB.PCI0.BR2C.H001, Local0)
    Notify(\_SB.PCI0.BR2C.H002, Local0)
    Notify(\_SB.PCI0.BR2C.H003, Local0)
    Notify(\_SB.PCI0.BR2C.H004, Local0)
    Notify(\_SB.PCI0.BR2C.H005, Local0)
    Notify(\_SB.PCI0.BR2C.H006, Local0)
    Notify(\_SB.PCI0.BR2C.H007, Local0)
  }

  // PCI0 Port 2D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI0.BR2D.PMEP,1) ) {
    Store(\_SB.PCI0.BR2D.PMEH(4), Local0)
  } else {
    Store (\_SB.PCI0.BR2D.HPEH(4), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x06, Local1)
    Notify(\_SB.PCI0.BR2D.H000, Local0)
    Notify(\_SB.PCI0.BR2D.H001, Local0)
    Notify(\_SB.PCI0.BR2D.H002, Local0)
    Notify(\_SB.PCI0.BR2D.H003, Local0)
    Notify(\_SB.PCI0.BR2D.H004, Local0)
    Notify(\_SB.PCI0.BR2D.H005, Local0)
    Notify(\_SB.PCI0.BR2D.H006, Local0)
    Notify(\_SB.PCI0.BR2D.H007, Local0)
  }

  // PCI0 Port 3A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI0.BR3A.PMEP,1) ) {
    Store(\_SB.PCI0.BR3A.PMEH(5), Local0)
  } else {
    Store (\_SB.PCI0.BR3A.HPEH(5), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x07, Local1)
    Notify(\_SB.PCI0.BR3A.H000, Local0)
    Notify(\_SB.PCI0.BR3A.H001, Local0)
    Notify(\_SB.PCI0.BR3A.H002, Local0)
    Notify(\_SB.PCI0.BR3A.H003, Local0)
    Notify(\_SB.PCI0.BR3A.H004, Local0)
    Notify(\_SB.PCI0.BR3A.H005, Local0)
    Notify(\_SB.PCI0.BR3A.H006, Local0)
    Notify(\_SB.PCI0.BR3A.H007, Local0)
  }

  // PCI0 Port 3B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI0.BR3B.PMEP,1) ) {
    Store(\_SB.PCI0.BR3B.PMEH(6), Local0)
  } else {
    Store (\_SB.PCI0.BR3B.HPEH(6), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x08, Local1)
    Notify(\_SB.PCI0.BR3B.H000, Local0)
    Notify(\_SB.PCI0.BR3B.H001, Local0)
    Notify(\_SB.PCI0.BR3B.H002, Local0)
    Notify(\_SB.PCI0.BR3B.H003, Local0)
    Notify(\_SB.PCI0.BR3B.H004, Local0)
    Notify(\_SB.PCI0.BR3B.H005, Local0)
    Notify(\_SB.PCI0.BR3B.H006, Local0)
    Notify(\_SB.PCI0.BR3B.H007, Local0)
  }

  // PCI0 Port 3C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI0.BR3C.PMEP,1) ) {
    Store(\_SB.PCI0.BR3C.PMEH(7), Local0)
  } else {
    Store (\_SB.PCI0.BR3C.HPEH(7), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x09, Local1)
    Notify(\_SB.PCI0.BR3C.H000, Local0)
    Notify(\_SB.PCI0.BR3C.H001, Local0)
    Notify(\_SB.PCI0.BR3C.H002, Local0)
    Notify(\_SB.PCI0.BR3C.H003, Local0)
    Notify(\_SB.PCI0.BR3C.H004, Local0)
    Notify(\_SB.PCI0.BR3C.H005, Local0)
    Notify(\_SB.PCI0.BR3C.H006, Local0)
    Notify(\_SB.PCI0.BR3C.H007, Local0)
  }

  // PCI0 Port 3D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI0.BR3D.PMEP,1) ) {
    Store(\_SB.PCI0.BR3D.PMEH(8), Local0)
  } else {
    Store (\_SB.PCI0.BR3D.HPEH(8), Local0) 
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0xA, Local1)
    Notify(\_SB.PCI0.BR3D.H000, Local0)
    Notify(\_SB.PCI0.BR3D.H001, Local0)
    Notify(\_SB.PCI0.BR3D.H002, Local0)
    Notify(\_SB.PCI0.BR3D.H003, Local0)
    Notify(\_SB.PCI0.BR3D.H004, Local0)
    Notify(\_SB.PCI0.BR3D.H005, Local0)
    Notify(\_SB.PCI0.BR3D.H006, Local0)
    Notify(\_SB.PCI0.BR3D.H007, Local0)
  }

  //If a hotplug event was serviced check if this was generated by PM_PME
  If (Lnot (LEqual(Local0, 0))) {
    //Clear the status bit 16 of PMEStatus
    //Clear the PME Pending bit 17 of PMEStatus
    If( LEqual(Local1, 0x01)) {
      Store(1, \_SB.PCI0.BR1A.PMES)
      Store(1, \_SB.PCI0.BR1A.PMEP)
    }
    If( LEqual(Local1, 0x02)) {
      Store(1, \_SB.PCI0.BR1B.PMES)
      Store(1, \_SB.PCI0.BR1B.PMEP)
    }
    If( LEqual(Local1, 0x03)) {
      Store(1, \_SB.PCI0.BR2A.PMES)
      Store(1, \_SB.PCI0.BR2A.PMEP)
    }
    If( LEqual(Local1, 0x04)) {
      Store(1, \_SB.PCI0.BR2B.PMES)
      Store(1, \_SB.PCI0.BR2B.PMEP)
    }
    If( LEqual(Local1, 0x05)) {
      Store(1, \_SB.PCI0.BR2C.PMES)
      Store(1, \_SB.PCI0.BR2C.PMEP)
    }
    If( LEqual(Local1, 0x06)) {
      Store(1, \_SB.PCI0.BR2D.PMES)
      Store(1, \_SB.PCI0.BR2D.PMEP)
    }
    If( LEqual(Local1, 0x07)) {
      Store(1, \_SB.PCI0.BR3A.PMES)
      Store(1, \_SB.PCI0.BR3A.PMEP)
    }
    If( LEqual(Local1, 0x08)) {
      Store(1, \_SB.PCI0.BR3B.PMES)
      Store(1, \_SB.PCI0.BR3B.PMEP)
    }
    If( LEqual(Local1, 0x09)) {
      Store(1, \_SB.PCI0.BR3C.PMES)
      Store(1, \_SB.PCI0.BR3C.PMEP)
    }
    If( LEqual(Local1, 0x0A)) {
      Store(1, \_SB.PCI0.BR3D.PMES)
      Store(1, \_SB.PCI0.BR3D.PMEP)
    }

    Store(0x01,PEES)               //Clear bit 9 of Status
    Store(0x00,PMEE)               //Clear bit 9 of GPE0_EN
  }
