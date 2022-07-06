/*++

  Copyright (c)  2009 Intel Corporation.  All rights reserved.

  This software and associated documentation (if any) is furnished under
  a license and may only be used or copied in accordance with the terms
  of the license.  Except as permitted by such license, no part of this
  software or documentation may be reproduced, stored in a retrieval
  system, or transmitted in any form or by any means without the express
  written consent of Intel Corporation.

  Module Name:

    nVidiaSLI.asl

  Abstract:

    nVidia SLI Support

--*/


#define NVIDIA_SLI_NO_KEY  " No SLI key has been loaded because this board does not support SLI.  Intel Coporation (R)"                                              
#define NVIDIA_SLI_KEY "791369584825Genuine NVIDIA Certified SLI Ready Motherboard for INTEL CRB           ef7c-Copyright 2014 NVIDIA Corporation All Rights Reserved-394180768352(R)"
Scope(\_SB.PCI0)
{
        // The OEM SLI Approval Cookie
        Name(SLI1, Buffer()
        {
                NVIDIA_SLI_KEY
        })
        Name(SLI0, Buffer()
        {
                NVIDIA_SLI_NO_KEY
        })
}

Device(\_SB.PCI0.WMI1)
{
        Name(_HID, "PNP0C14")
        Name(_UID, "MXM2")
        // Description of data and events supported
        Name(_WDG, Buffer() {
                // Methods GUID {F6CB5C3C-9CAE-4ebd-B577-931EA32A2CC0}
                0x3C, 0x5C, 0xCB, 0xF6, 0xAE, 0x9C, 0xbd, 0x4e, 0xB5, 0x77, 0x93, 0x1E,
                0xA3, 0x2A, 0x2C, 0xC0,
                0x4D, 0x58,
                1, // Instance Count
                0x02, // Flags (WMIACPI_REGFLAG_METHOD)
        })

        // WMI tunnel
        Method(WMMX, 3)
        {
                CreateDWordField(Arg2, 0, FUNC)
                If (LEqual(FUNC, 0x41494c53)) { // "SLIA"
                   Return(\_SB.PCI0.SLI1)
                   }
              
           

                // Other WMMX functions here
                Return(0)
        } // End Method
} // end device



