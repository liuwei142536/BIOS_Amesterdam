//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2022, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file
  64-bit Math Worker Function.
  The 32-bit versions of C compiler generate calls to library routines
  to handle 64-bit math. These functions use non-standard calling conventions.

Copyright (c) 2014, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

/*
 * Shifts a 64-bit unsigned value right by a certain number of bits.
 */
__declspec(naked) void __cdecl
_aullshr (
  void
  )
{
  _asm {
    ;
    ; Checking: Only handle 64bit shifting or more
    ;
    cmp     cl, 64
    jae     _Exit

    ;
    ; Handle shifting between 0 and 31 bits
    ;
    cmp     cl, 32
    jae     More32
    shrd    eax, edx, cl
    shr     edx, cl
    ret

    ;
    ; Handle shifting of 32-63 bits
    ;
More32:
    mov     eax, edx
    xor     edx, edx
    and     cl, 31
    shr     eax, cl
    ret

    ;
    ; Invalid number (less  then 32bits), return 0
    ;

_Exit:
    xor     eax, eax
    xor     edx, edx
    ret
  }
}
// APTIOV OVERRIDE starts - NT32 code build error with CryptoPkg
/*
 * Shifts a 64-bit signed value right by a particular number of bits.
 */
__declspec(naked) void __cdecl 
_allshr (
  void
  )
{
  _asm {
    ;
    ; Handle shifts of 64 bits or more (if shifting 64 bits or more, the result
    ; depends only on the high order bit of edx).
      ;
    cmp     cl,64
    jae     short SIGNRETURN

    ;
    ; Handle shifts of between 0 and 31 bits
    ;
    cmp     cl, 32
    jae     short MORE32
    shrd    eax,edx,cl
    sar     edx,cl
    ret

    ;
    ; Handle shifts of between 32 and 63 bits
    ;
MORE32:
    mov     eax,edx
    sar     edx,31
    and     cl,31
    sar     eax,cl
    ret

    ;
    ; Return double precision 0 or -1, depending on the sign of edx
    ;
SIGNRETURN:
    sar     edx,31
    mov     eax,edx
    ret
  }
}
// APTIOV OVERRIDE ends - NT32 code build error with CryptoPkg
