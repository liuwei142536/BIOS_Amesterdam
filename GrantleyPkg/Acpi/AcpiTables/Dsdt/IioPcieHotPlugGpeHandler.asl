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

  // IIO1 Port 0 PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI1.QRP0.PMEP,1) ) {
    Store(\_SB.PCI1.QRP0.PMEH(9), Local0)
  } else {
    Store (\_SB.PCI1.QRP0.HPEH(9), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x10, Local1)
    Notify(\_SB.PCI1.QRP0.H000, Local0)
    Notify(\_SB.PCI1.QRP0.H001, Local0)
    Notify(\_SB.PCI1.QRP0.H002, Local0)
    Notify(\_SB.PCI1.QRP0.H003, Local0)
    Notify(\_SB.PCI1.QRP0.H004, Local0)
    Notify(\_SB.PCI1.QRP0.H005, Local0)
    Notify(\_SB.PCI1.QRP0.H006, Local0)
    Notify(\_SB.PCI1.QRP0.H007, Local0)
  }

  // IIO1 Port 2A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI1.QR2A.PMEP,1) ) {
    Store(\_SB.PCI1.QR2A.PMEH(1), Local0)
  } else {
    Store (\_SB.PCI1.QR2A.HPEH(1), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x11, Local1)
    Notify(\_SB.PCI1.QR2A.H000, Local0)
    Notify(\_SB.PCI1.QR2A.H001, Local0)
    Notify(\_SB.PCI1.QR2A.H002, Local0)
    Notify(\_SB.PCI1.QR2A.H003, Local0)
    Notify(\_SB.PCI1.QR2A.H004, Local0)
    Notify(\_SB.PCI1.QR2A.H005, Local0)
    Notify(\_SB.PCI1.QR2A.H006, Local0)
    Notify(\_SB.PCI1.QR2A.H007, Local0)
  }

  // IIO1 Port 2B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI1.QR2B.PMEP,1) ) {
    Store(\_SB.PCI1.QR2B.PMEH(2), Local0)
  } else {
    Store (\_SB.PCI1.QR2B.HPEH(2), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x12, Local1)
    Notify(\_SB.PCI1.QR2B.H000, Local0)
    Notify(\_SB.PCI1.QR2B.H001, Local0)
    Notify(\_SB.PCI1.QR2B.H002, Local0)
    Notify(\_SB.PCI1.QR2B.H003, Local0)
    Notify(\_SB.PCI1.QR2B.H004, Local0)
    Notify(\_SB.PCI1.QR2B.H005, Local0)
    Notify(\_SB.PCI1.QR2B.H006, Local0)
    Notify(\_SB.PCI1.QR2B.H007, Local0)
  }

  // IIO1 Port 2C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI1.QR2C.PMEP,1) ) {
    Store(\_SB.PCI1.QR2C.PMEH(3), Local0)
  } else {
    Store (\_SB.PCI1.QR2C.HPEH(3), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x13, Local1)
    Notify(\_SB.PCI1.QR2C.H000, Local0)
    Notify(\_SB.PCI1.QR2C.H001, Local0)
    Notify(\_SB.PCI1.QR2C.H002, Local0)
    Notify(\_SB.PCI1.QR2C.H003, Local0)
    Notify(\_SB.PCI1.QR2C.H004, Local0)
    Notify(\_SB.PCI1.QR2C.H005, Local0)
    Notify(\_SB.PCI1.QR2C.H006, Local0)
    Notify(\_SB.PCI1.QR2C.H007, Local0)
  }

  // IIO1 Port 2D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI1.QR2D.PMEP,1) ) {
    Store(\_SB.PCI1.QR2D.PMEH(4), Local0)
  } else {
    Store (\_SB.PCI1.QR2D.HPEH(4), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x14, Local1)
    Notify(\_SB.PCI1.QR2D.H000, Local0)
    Notify(\_SB.PCI1.QR2D.H001, Local0)
    Notify(\_SB.PCI1.QR2D.H002, Local0)
    Notify(\_SB.PCI1.QR2D.H003, Local0)
    Notify(\_SB.PCI1.QR2D.H004, Local0)
    Notify(\_SB.PCI1.QR2D.H005, Local0)
    Notify(\_SB.PCI1.QR2D.H006, Local0)
    Notify(\_SB.PCI1.QR2D.H007, Local0)
  }

  // IIO1 Port 3A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI1.QR3A.PMEP,1) ) {
    Store(\_SB.PCI1.QR3A.PMEH(5), Local0)
  } else {
    Store (\_SB.PCI1.QR3A.HPEH(5), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x15, Local1)
    Notify(\_SB.PCI1.QR3A.H000, Local0)
    Notify(\_SB.PCI1.QR3A.H001, Local0)
    Notify(\_SB.PCI1.QR3A.H002, Local0)
    Notify(\_SB.PCI1.QR3A.H003, Local0)
    Notify(\_SB.PCI1.QR3A.H004, Local0)
    Notify(\_SB.PCI1.QR3A.H005, Local0)
    Notify(\_SB.PCI1.QR3A.H006, Local0)
    Notify(\_SB.PCI1.QR3A.H007, Local0)
  }

  // IIO1 Port 3B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI1.QR3B.PMEP,1) ) {
    Store(\_SB.PCI1.QR3B.PMEH(6), Local0)
  } else {
    Store (\_SB.PCI1.QR3B.HPEH(6), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x16, Local1)
    Notify(\_SB.PCI1.QR3B.H000, Local0)
    Notify(\_SB.PCI1.QR3B.H001, Local0)
    Notify(\_SB.PCI1.QR3B.H002, Local0)
    Notify(\_SB.PCI1.QR3B.H003, Local0)
    Notify(\_SB.PCI1.QR3B.H004, Local0)
    Notify(\_SB.PCI1.QR3B.H005, Local0)
    Notify(\_SB.PCI1.QR3B.H006, Local0)
    Notify(\_SB.PCI1.QR3B.H007, Local0)
  }

  // IIO1 Port 3C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI1.QR3C.PMEP,1) ) {
    Store(\_SB.PCI1.QR3C.PMEH(7), Local0)
  } else {
    Store (\_SB.PCI1.QR3C.HPEH(7), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x17, Local1)
    Notify(\_SB.PCI1.QR3C.H000, Local0)
    Notify(\_SB.PCI1.QR3C.H001, Local0)
    Notify(\_SB.PCI1.QR3C.H002, Local0)
    Notify(\_SB.PCI1.QR3C.H003, Local0)
    Notify(\_SB.PCI1.QR3C.H004, Local0)
    Notify(\_SB.PCI1.QR3C.H005, Local0)
    Notify(\_SB.PCI1.QR3C.H006, Local0)
    Notify(\_SB.PCI1.QR3C.H007, Local0)
  }

  // IIO1 Port 3D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI1.QR3D.PMEP,1) ) {
    Store(\_SB.PCI1.QR3D.PMEH(8), Local0)
  } else {
    Store (\_SB.PCI1.QR3D.HPEH(8), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x18, Local1)
    Notify(\_SB.PCI1.QR3D.H000, Local0)
    Notify(\_SB.PCI1.QR3D.H001, Local0)
    Notify(\_SB.PCI1.QR3D.H002, Local0)
    Notify(\_SB.PCI1.QR3D.H003, Local0)
    Notify(\_SB.PCI1.QR3D.H004, Local0)
    Notify(\_SB.PCI1.QR3D.H005, Local0)
    Notify(\_SB.PCI1.QR3D.H006, Local0)
    Notify(\_SB.PCI1.QR3D.H007, Local0)
  }

  // IIO2 Port 0 PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI2.RRP0.PMEP,1) ) {
    Store(\_SB.PCI2.RRP0.PMEH(9), Local0)
  } else {
    Store (\_SB.PCI2.RRP0.HPEH(9), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x20, Local1)
    Notify(\_SB.PCI2.RRP0.H000, Local0)
    Notify(\_SB.PCI2.RRP0.H001, Local0)
    Notify(\_SB.PCI2.RRP0.H002, Local0)
    Notify(\_SB.PCI2.RRP0.H003, Local0)
    Notify(\_SB.PCI2.RRP0.H004, Local0)
    Notify(\_SB.PCI2.RRP0.H005, Local0)
    Notify(\_SB.PCI2.RRP0.H006, Local0)
    Notify(\_SB.PCI2.RRP0.H007, Local0)
  }

  // IIO2 Port 2A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI2.RR2A.PMEP,1) ) {
    Store(\_SB.PCI2.RR2A.PMEH(1), Local0)
  } else {
    Store (\_SB.PCI2.RR2A.HPEH(1), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x21, Local1)
    Notify(\_SB.PCI2.RR2A.H000, Local0)
    Notify(\_SB.PCI2.RR2A.H001, Local0)
    Notify(\_SB.PCI2.RR2A.H002, Local0)
    Notify(\_SB.PCI2.RR2A.H003, Local0)
    Notify(\_SB.PCI2.RR2A.H004, Local0)
    Notify(\_SB.PCI2.RR2A.H005, Local0)
    Notify(\_SB.PCI2.RR2A.H006, Local0)
    Notify(\_SB.PCI2.RR2A.H007, Local0)
  }

  // IIO2 Port 2B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI2.RR2B.PMEP,1) ) {
    Store(\_SB.PCI2.RR2B.PMEH(2), Local0)
  } else {
    Store (\_SB.PCI2.RR2B.HPEH(2), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x22, Local1)
    Notify(\_SB.PCI2.RR2B.H000, Local0)
    Notify(\_SB.PCI2.RR2B.H001, Local0)
    Notify(\_SB.PCI2.RR2B.H002, Local0)
    Notify(\_SB.PCI2.RR2B.H003, Local0)
    Notify(\_SB.PCI2.RR2B.H004, Local0)
    Notify(\_SB.PCI2.RR2B.H005, Local0)
    Notify(\_SB.PCI2.RR2B.H006, Local0)
    Notify(\_SB.PCI2.RR2B.H007, Local0)
  }

  // IIO2 Port 2C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI2.RR2C.PMEP,1) ) {
    Store(\_SB.PCI2.RR2C.PMEH(3), Local0)
  } else {
    Store (\_SB.PCI2.RR2C.HPEH(3), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x23, Local1)
    Notify(\_SB.PCI2.RR2C.H000, Local0)
    Notify(\_SB.PCI2.RR2C.H001, Local0)
    Notify(\_SB.PCI2.RR2C.H002, Local0)
    Notify(\_SB.PCI2.RR2C.H003, Local0)
    Notify(\_SB.PCI2.RR2C.H004, Local0)
    Notify(\_SB.PCI2.RR2C.H005, Local0)
    Notify(\_SB.PCI2.RR2C.H006, Local0)
    Notify(\_SB.PCI2.RR2C.H007, Local0)
  }

  // IIO2 Port 2D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI2.RR2D.PMEP,1) ) {
    Store(\_SB.PCI2.RR2D.PMEH(4), Local0)
  } else {
    Store (\_SB.PCI2.RR2D.HPEH(4), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x24, Local1)
    Notify(\_SB.PCI2.RR2D.H000, Local0)
    Notify(\_SB.PCI2.RR2D.H001, Local0)
    Notify(\_SB.PCI2.RR2D.H002, Local0)
    Notify(\_SB.PCI2.RR2D.H003, Local0)
    Notify(\_SB.PCI2.RR2D.H004, Local0)
    Notify(\_SB.PCI2.RR2D.H005, Local0)
    Notify(\_SB.PCI2.RR2D.H006, Local0)
    Notify(\_SB.PCI2.RR2D.H007, Local0)
  }

  // IIO2 Port 3A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI2.RR3A.PMEP,1) ) {
    Store(\_SB.PCI2.RR3A.PMEH(5), Local0)
  } else {
    Store (\_SB.PCI2.RR3A.HPEH(5), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x25, Local1)
    Notify(\_SB.PCI2.RR3A.H000, Local0)
    Notify(\_SB.PCI2.RR3A.H001, Local0)
    Notify(\_SB.PCI2.RR3A.H002, Local0)
    Notify(\_SB.PCI2.RR3A.H003, Local0)
    Notify(\_SB.PCI2.RR3A.H004, Local0)
    Notify(\_SB.PCI2.RR3A.H005, Local0)
    Notify(\_SB.PCI2.RR3A.H006, Local0)
    Notify(\_SB.PCI2.RR3A.H007, Local0)
  }

  // IIO2 Port 3B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI2.RR3B.PMEP,1) ) {
    Store(\_SB.PCI2.RR3B.PMEH(6), Local0)
  } else {
    Store (\_SB.PCI2.RR3B.HPEH(6), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x26, Local1)
    Notify(\_SB.PCI2.RR3B.H000, Local0)
    Notify(\_SB.PCI2.RR3B.H001, Local0)
    Notify(\_SB.PCI2.RR3B.H002, Local0)
    Notify(\_SB.PCI2.RR3B.H003, Local0)
    Notify(\_SB.PCI2.RR3B.H004, Local0)
    Notify(\_SB.PCI2.RR3B.H005, Local0)
    Notify(\_SB.PCI2.RR3B.H006, Local0)
    Notify(\_SB.PCI2.RR3B.H007, Local0)
  }

  // IIO2 Port 3C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI2.RR3C.PMEP,1) ) {
    Store(\_SB.PCI2.RR3C.PMEH(7), Local0)
  } else {
    Store (\_SB.PCI2.RR3C.HPEH(7), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x27, Local1)
    Notify(\_SB.PCI2.RR3C.H000, Local0)
    Notify(\_SB.PCI2.RR3C.H001, Local0)
    Notify(\_SB.PCI2.RR3C.H002, Local0)
    Notify(\_SB.PCI2.RR3C.H003, Local0)
    Notify(\_SB.PCI2.RR3C.H004, Local0)
    Notify(\_SB.PCI2.RR3C.H005, Local0)
    Notify(\_SB.PCI2.RR3C.H006, Local0)
    Notify(\_SB.PCI2.RR3C.H007, Local0)
  }

  // IIO2 Port 3D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI2.RR3D.PMEP,1) ) {
    Store(\_SB.PCI2.RR3D.PMEH(8), Local0)
  } else {
    Store (\_SB.PCI2.RR3D.HPEH(8), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x28, Local1)
    Notify(\_SB.PCI2.RR3D.H000, Local0)
    Notify(\_SB.PCI2.RR3D.H001, Local0)
    Notify(\_SB.PCI2.RR3D.H002, Local0)
    Notify(\_SB.PCI2.RR3D.H003, Local0)
    Notify(\_SB.PCI2.RR3D.H004, Local0)
    Notify(\_SB.PCI2.RR3D.H005, Local0)
    Notify(\_SB.PCI2.RR3D.H006, Local0)
    Notify(\_SB.PCI2.RR3D.H007, Local0)
  }

  // IIO3 Port 0 PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI3.SRP0.PMEP,1) ) {
    Store(\_SB.PCI3.SRP0.PMEH(0), Local0)
  } else {
    Store (\_SB.PCI3.SRP0.HPEH(0), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x30, Local1)
    Notify(\_SB.PCI3.SRP0.H000, Local0)
    Notify(\_SB.PCI3.SRP0.H001, Local0)
    Notify(\_SB.PCI3.SRP0.H002, Local0)
    Notify(\_SB.PCI3.SRP0.H003, Local0)
    Notify(\_SB.PCI3.SRP0.H004, Local0)
    Notify(\_SB.PCI3.SRP0.H005, Local0)
    Notify(\_SB.PCI3.SRP0.H006, Local0)
    Notify(\_SB.PCI3.SRP0.H007, Local0)
  }

  // IIO3 Port 2A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI3.SR2A.PMEP,1) ) {
    Store(\_SB.PCI3.SR2A.PMEH(1), Local0)
  } else {
    Store (\_SB.PCI3.SR2A.HPEH(1), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x31, Local1)
    Notify(\_SB.PCI3.SR2A.H000, Local0)
    Notify(\_SB.PCI3.SR2A.H001, Local0)
    Notify(\_SB.PCI3.SR2A.H002, Local0)
    Notify(\_SB.PCI3.SR2A.H003, Local0)
    Notify(\_SB.PCI3.SR2A.H004, Local0)
    Notify(\_SB.PCI3.SR2A.H005, Local0)
    Notify(\_SB.PCI3.SR2A.H006, Local0)
    Notify(\_SB.PCI3.SR2A.H007, Local0)
  }

  // IIO3 Port 2B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI3.SR2B.PMEP,1) ) {
    Store(\_SB.PCI3.SR2B.PMEH(2), Local0)
  } else {
    Store (\_SB.PCI3.SR2B.HPEH(2), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x32, Local1)
    Notify(\_SB.PCI3.SR2B.H000, Local0)
    Notify(\_SB.PCI3.SR2B.H001, Local0)
    Notify(\_SB.PCI3.SR2B.H002, Local0)
    Notify(\_SB.PCI3.SR2B.H003, Local0)
    Notify(\_SB.PCI3.SR2B.H004, Local0)
    Notify(\_SB.PCI3.SR2B.H005, Local0)
    Notify(\_SB.PCI3.SR2B.H006, Local0)
    Notify(\_SB.PCI3.SR2B.H007, Local0)
  }

  // IIO3 Port 2C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI3.SR2C.PMEP,1) ) {
    Store(\_SB.PCI3.SR2C.PMEH(3), Local0)
  } else {
    Store (\_SB.PCI3.SR2C.HPEH(3), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x33, Local1)
    Notify(\_SB.PCI3.SR2C.H000, Local0)
    Notify(\_SB.PCI3.SR2C.H001, Local0)
    Notify(\_SB.PCI3.SR2C.H002, Local0)
    Notify(\_SB.PCI3.SR2C.H003, Local0)
    Notify(\_SB.PCI3.SR2C.H004, Local0)
    Notify(\_SB.PCI3.SR2C.H005, Local0)
    Notify(\_SB.PCI3.SR2C.H006, Local0)
    Notify(\_SB.PCI3.SR2C.H007, Local0)
  }

  // IIO3 Port 2D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI3.SR2D.PMEP,1) ) {
    Store(\_SB.PCI3.SR2D.PMEH(4), Local0)
  } else {
    Store (\_SB.PCI3.SR2D.HPEH(4), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x34, Local1)
    Notify(\_SB.PCI3.SR2D.H000, Local0)
    Notify(\_SB.PCI3.SR2D.H001, Local0)
    Notify(\_SB.PCI3.SR2D.H002, Local0)
    Notify(\_SB.PCI3.SR2D.H003, Local0)
    Notify(\_SB.PCI3.SR2D.H004, Local0)
    Notify(\_SB.PCI3.SR2D.H005, Local0)
    Notify(\_SB.PCI3.SR2D.H006, Local0)
    Notify(\_SB.PCI3.SR2D.H007, Local0)
  }

  // IIO3 Port 3A PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI3.SR3A.PMEP,1) ) {
    Store(\_SB.PCI3.SR3A.PMEH(5), Local0)
  } else {
    Store (\_SB.PCI3.SR3A.HPEH(5), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x35, Local1)
    Notify(\_SB.PCI3.SR3A.H000, Local0)
    Notify(\_SB.PCI3.SR3A.H001, Local0)
    Notify(\_SB.PCI3.SR3A.H002, Local0)
    Notify(\_SB.PCI3.SR3A.H003, Local0)
    Notify(\_SB.PCI3.SR3A.H004, Local0)
    Notify(\_SB.PCI3.SR3A.H005, Local0)
    Notify(\_SB.PCI3.SR3A.H006, Local0)
    Notify(\_SB.PCI3.SR3A.H007, Local0)
  }

  // IIO3 Port 3B PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI3.SR3B.PMEP,1) ) {
    Store(\_SB.PCI3.SR3B.PMEH(6), Local0)
  } else {
    Store (\_SB.PCI3.SR3B.HPEH(6), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x36, Local1)
    Notify(\_SB.PCI3.SR3B.H000, Local0)
    Notify(\_SB.PCI3.SR3B.H001, Local0)
    Notify(\_SB.PCI3.SR3B.H002, Local0)
    Notify(\_SB.PCI3.SR3B.H003, Local0)
    Notify(\_SB.PCI3.SR3B.H004, Local0)
    Notify(\_SB.PCI3.SR3B.H005, Local0)
    Notify(\_SB.PCI3.SR3B.H006, Local0)
    Notify(\_SB.PCI3.SR3B.H007, Local0)
  }

  // IIO3 Port 3C PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI3.SR3C.PMEP,1) ) {
    Store(\_SB.PCI3.SR3C.PMEH(7), Local0)
  } else {
    Store (\_SB.PCI3.SR3C.HPEH(7), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x37, Local1)
    Notify(\_SB.PCI3.SR3C.H000, Local0)
    Notify(\_SB.PCI3.SR3C.H001, Local0)
    Notify(\_SB.PCI3.SR3C.H002, Local0)
    Notify(\_SB.PCI3.SR3C.H003, Local0)
    Notify(\_SB.PCI3.SR3C.H004, Local0)
    Notify(\_SB.PCI3.SR3C.H005, Local0)
    Notify(\_SB.PCI3.SR3C.H006, Local0)
    Notify(\_SB.PCI3.SR3C.H007, Local0)
  }

  // IIO3 Port 3D PCI-Ex Hot Plug
  // If PM_PME event clear INTs and AttnBtnPressed
  If( LEqual(\_SB.PCI3.SR3D.PMEP,1) ) {
    Store(\_SB.PCI3.SR3D.PMEH(8), Local0)
  } else {
    Store (\_SB.PCI3.SR3D.HPEH(8), Local0)
  }
  If(Lnot(LEqual(Local0,0xFF))) {
    Store(0x38, Local1)
    Notify(\_SB.PCI3.SR3D.H000, Local0)
    Notify(\_SB.PCI3.SR3D.H001, Local0)
    Notify(\_SB.PCI3.SR3D.H002, Local0)
    Notify(\_SB.PCI3.SR3D.H003, Local0)
    Notify(\_SB.PCI3.SR3D.H004, Local0)
    Notify(\_SB.PCI3.SR3D.H005, Local0)
    Notify(\_SB.PCI3.SR3D.H006, Local0)
    Notify(\_SB.PCI3.SR3D.H007, Local0)
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

    If( LEqual(Local1, 0x10)) {
      Store(1, \_SB.PCI1.QRP0.PMES)
      Store(1, \_SB.PCI1.QRP0.PMEP)
    }
    If( LEqual(Local1, 0x11)) {
      Store(1, \_SB.PCI1.QR2A.PMES)
      Store(1, \_SB.PCI1.QR2A.PMEP)
    }
    If( LEqual(Local1, 0x12)) {
      Store(1, \_SB.PCI1.QR2B.PMES)
      Store(1, \_SB.PCI1.QR2B.PMEP)
    }
    If( LEqual(Local1, 0x13)) {
      Store(1, \_SB.PCI1.QR2C.PMES)
      Store(1, \_SB.PCI1.QR2C.PMEP)
    }
    If( LEqual(Local1, 0x14)) {
      Store(1, \_SB.PCI1.QR2D.PMES)
      Store(1, \_SB.PCI1.QR2D.PMEP)
    }
    If( LEqual(Local1, 0x15)) {
      Store(1, \_SB.PCI1.QR3A.PMES)
      Store(1, \_SB.PCI1.QR3A.PMEP)
    }
    If( LEqual(Local1, 0x16)) {
      Store(1, \_SB.PCI1.QR3B.PMES)
      Store(1, \_SB.PCI1.QR3B.PMEP)
    }
    If( LEqual(Local1, 0x17)) {
      Store(1, \_SB.PCI1.QR3C.PMES)
      Store(1, \_SB.PCI1.QR3C.PMEP)
    }
    If( LEqual(Local1, 0x18)) {
      Store(1, \_SB.PCI1.QR3D.PMES)
      Store(1, \_SB.PCI1.QR3D.PMEP)
    }

    If( LEqual(Local1, 0x20)) {
      Store(1, \_SB.PCI2.RRP0.PMES)
      Store(1, \_SB.PCI2.RRP0.PMEP)
    }
    If( LEqual(Local1, 0x21)) {
      Store(1, \_SB.PCI2.RR2A.PMES)
      Store(1, \_SB.PCI2.RR2A.PMEP)
    }
    If( LEqual(Local1, 0x22)) {
      Store(1, \_SB.PCI2.RR2B.PMES)
      Store(1, \_SB.PCI2.RR2B.PMEP)
    }
    If( LEqual(Local1, 0x23)) {
      Store(1, \_SB.PCI2.RR2C.PMES)
      Store(1, \_SB.PCI2.RR2C.PMEP)
    }
    If( LEqual(Local1, 0x24)) {
      Store(1, \_SB.PCI2.RR2D.PMES)
      Store(1, \_SB.PCI2.RR2D.PMEP)
    }
    If( LEqual(Local1, 0x25)) {
      Store(1, \_SB.PCI2.RR3A.PMES)
      Store(1, \_SB.PCI2.RR3A.PMEP)
    }
    If( LEqual(Local1, 0x26)) {
      Store(1, \_SB.PCI2.RR3B.PMES)
      Store(1, \_SB.PCI2.RR3B.PMEP)
    }
    If( LEqual(Local1, 0x27)) {
      Store(1, \_SB.PCI2.RR3C.PMES)
      Store(1, \_SB.PCI2.RR3C.PMEP)
    }
    If( LEqual(Local1, 0x28)) {
      Store(1, \_SB.PCI2.RR3D.PMES)
      Store(1, \_SB.PCI2.RR3D.PMEP)
    }

    If( LEqual(Local1, 0x30)) {
      Store(1, \_SB.PCI3.SRP0.PMES)
      Store(1, \_SB.PCI3.SRP0.PMEP)
    }
    If( LEqual(Local1, 0x31)) {
      Store(1, \_SB.PCI3.SR2A.PMES)
      Store(1, \_SB.PCI3.SR2A.PMEP)
    }
    If( LEqual(Local1, 0x32)) {
      Store(1, \_SB.PCI3.SR2B.PMES)
      Store(1, \_SB.PCI3.SR2B.PMEP)
    }
    If( LEqual(Local1, 0x33)) {
      Store(1, \_SB.PCI3.SR2C.PMES)
      Store(1, \_SB.PCI3.SR2C.PMEP)
    }
    If( LEqual(Local1, 0x34)) {
      Store(1, \_SB.PCI3.SR2D.PMES)
      Store(1, \_SB.PCI3.SR2D.PMEP)
    }
    If( LEqual(Local1, 0x35)) {
      Store(1, \_SB.PCI3.SR3A.PMES)
      Store(1, \_SB.PCI3.SR3A.PMEP)
    }
    If( LEqual(Local1, 0x36)) {
      Store(1, \_SB.PCI3.SR3B.PMES)
      Store(1, \_SB.PCI3.SR3B.PMEP)
    }
    If( LEqual(Local1, 0x37)) {
      Store(1, \_SB.PCI3.SR3C.PMES)
      Store(1, \_SB.PCI3.SR3C.PMEP)
    }
    If( LEqual(Local1, 0x38)) {
      Store(1, \_SB.PCI3.SR3D.PMES)
      Store(1, \_SB.PCI3.SR3D.PMEP)
    }

    Store(0x01,PEES)               //Clear bit 9 of Status
    Store(0x00,PMEE)               //Clear bit 9 of GPE0_EN
  }
