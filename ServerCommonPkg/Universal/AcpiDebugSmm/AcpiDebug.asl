//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/** @file
  Acpi Debug ASL code.

Copyright (c) 2011, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/ 

/*------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
;
; Intel(R) Confidential
;
; Intel ACPI Reference Code
;
; Copyright (c) 2006-2009 Intel Corporation
;
; Filename: AcpiDebug.asl
;
; Revision: See Relnotes.txt
;
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
;
; This Intel Corporation BIOS Source Code is furnished under license
; and may only be used or copied in accordance with the terms of the license.
; The information in this manual is furnished for informational use only,
; is subject to change without notice, and should not be construed as a
; commitment by Intel Corporation. Intel Corporation assumes no
; responsibility or liability for any errors or inaccuracies that may
; appear in this document or any software that may be provided in
; association with this document.
;
; Except as permitted by such license, no part of this document may be
; reproduced, stored in a retrieval system, or transmitted in any form or by
; any means without the express written consent of Intel Corporation.
;
;-------------------------------------------------------------------------------
;-----------------------------------------------------------------------------*/

DefinitionBlock (
	"AcpiDebug.aml",
	"SSDT",
	1,
	"Intel_",
	"ADebTabl",
	0x1000
  )
{
  Scope(\)
  {
    //
    // These pointers are patched during POST.
    // 
    Name(DPTR, 0x80000000) // Address of Acpi debug memory buffer, fixed up during POST
    Name(EPTR, 0x80000000) // End of Acpi debug memory buffer, fixed up during POST
    Name(CPTR, 0x80000000) // Current pointer used as an index into the buffer(starts after the buffer signature), fixed up during POST

    //
    //Use a Mutex to prevent multiple calls from simutaneously writing to the same memory.
    //
    Mutex(MMUT, 0)
    //
    // Operational region for Smi port access
    //
    OperationRegion (ADBP, SystemIO, 0xB2, 2)
    Field (ADBP, ByteAcc, NoLock, Preserve)
    { 
        B2PT, 8,
        B3PT, 8,  
    }

    //
    // Write a string to a memory buffer
    //
    Method(MDBG,1,Serialized)
    {
        
        OperationRegion(ADHD, SystemMemory, DPTR, 32) // Operation region for Acpi Debug buffer first 0x20 bytes
        Field(ADHD, ByteAcc, NoLock, Preserve) 
        {
          Offset(0x0),
          ASIG, 128,   // 16 bytes is Signature
          Offset(0x10),
          ASIZ, 32,    // 4 bytes is buffer size
          ACHP, 32,    // 4 bytes is current head pointer, normally is DPTR + 0x20, 
                       //   if there's SMM handler to print, then it's then starting of the info hasn't been printed yet.
          ACTP, 32,    // 4 bytes is current tail pointer, is the same as CPTR
          SMIN, 8,     // 1 byte of SMI Number for trigger callback
          WRAP, 8,     // 1 byte of wrap status
          RSVD, 16     // 2 bytes reserved
        }
        
        Store (Acquire(MMUT, 1000),Local0)  // save Acquire result so we can check for Mutex acquired
        If (LEqual(Local0, Zero))  // check for Mutex acquired
        {
          
          OperationRegion(ABLK, SystemMemory, CPTR, 64) // Operation region to allow writes to ACPI debug buffer
          Field(ABLK, ByteAcc, NoLock, Preserve) 
          {
            Offset(0x0),
            AAAA,	512  // 64 byte is max length for string or data
          }
          Store(Arg0,AAAA)  // anything past string buffer size is ignored by ACPI
          Add(SizeOf(Arg0), 1, Local1) //string size + \0
          
          Add(CPTR, Local1, CPTR) // advance current pointer to next string location in memory buffer
          If (LGreaterEqual(CPTR,EPTR) ) // check for end of 64kb Acpi debug buffer
          {
            Add (DPTR, 32, CPTR) // wrap around to beginning of buffer if the end has been reached
            Store(1, WRAP)
          }
          Store (CPTR, ACTP) 

          //
          // Triggle the SMI to print
          //
          Store (SMIN, B2PT)
          Release(MMUT)
        }


        Return(Local0) // return error code indicating whether Mutex was acquired
    }

  } // End Scope
} // End SSDT
