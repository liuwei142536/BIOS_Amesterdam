//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++
  
Copyright 1996-2016, Intel Corporation.

This source code and any documentation accompanying it ("Material") is furnished
under license and may only be used or copied in accordance with the terms of that
license.  No license, express or implied, by estoppel or otherwise, to any
intellectual property rights is granted to you by disclosure or delivery of these
Materials.  The Materials are subject to change without notice and should not be
construed as a commitment by Intel Corporation to market, license, sell or support
any product or technology.  Unless otherwise provided for in the license under which
this Material is provided, the Material is provided AS IS, with no warranties of
any kind, express or implied, including without limitation the implied warranties
of fitness, merchantability, or non-infringement.  Except as expressly permitted by
the license for the Material, neither Intel Corporation nor its suppliers assumes
any responsibility for any errors or inaccuracies that may appear herein.  Except
as expressly permitted by the license for the Material, no part of the Material
may be reproduced, stored in a retrieval system, transmitted in any form, or
distributed by any means without the express written consent of Intel Corporation.

  Module Name:
  
    CpuHwp.asl
  
  Abstract:
  
    Contains root level name space objects for Cpu 0..F with HWP (HW P-States) support
  
NOTES:-
 This Processor Power Management BIOS Source Code is furnished under license
 and may only be used or copied in accordance with the terms of the license.
 The information in this document is furnished for informational use only, is
 subject to change without notice, and should not be construed as a commitment
 by Intel Corporation. Intel Corporation assumes no responsibility or liability
 for any errors or inaccuracies that may appear in this document or any
 software that may be provided in association with this document.

 Except as permitted by such license, no part of this document may be
 reproduced, stored in a retrieval system, or transmitted in any form or by
 any means without the express written consent of Intel Corporation.

 WARNING: You are authorized and licensed to install and use this BIOS code
 ONLY on an IST PC. This utility may damage any system that does not
 meet these requirements.

  An IST PC is a computer which
  (1) Is capable of seamlessly and automatically transitioning among
      multiple performance states (potentially operating at different
      efficiency ratings) based upon power source changes, end user
      preference, processor performance demand, and thermal conditions; and
  (2) Includes an Intel Pentium II processors, Intel Pentium III
      processor, Mobile Intel Pentium III Processor-M, Mobile Intel Pentium 4
      Processor-M, Intel Pentium M Processor, or any other future Intel 
      processors that incorporates the capability to transition between 
      different performance states by altering some, or any combination of, 
      the following processor attributes: core voltage, core frequency, bus 
      frequency, number of processor cores available, or any other attribute 
      that changes the efficiency (instructions/unit time-power) at which the 
      processor operates.

--*/


DefinitionBlock (
  "CPUMFC.aml",     // Output Filename
  "NITR",           // Signature
  2,                // SSDT Revision
  "INTEL",          // OEMID
  "NITROMFC",       // Table ID
  0x01            // OEM Revision
  ) {
     scope(\_SB)
     {

				Device(INTS)
				{
					Name(_HID, EISAID("INT3510")) 
					Name(_STR, Unicode ("Intel Nitrous Device")) 
				}
				
	}
				
}