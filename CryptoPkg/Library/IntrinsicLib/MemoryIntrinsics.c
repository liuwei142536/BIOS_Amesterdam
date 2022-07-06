//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2022, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file
  Intrinsic Memory Routines Wrapper Implementation for OpenSSL-based
  Cryptographic Library.

Copyright (c) 2010 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Token.h> //APTIOV OVERRIDE - Controlling build of wrapper methods based on SDL token
#include <Base.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>

//APTIOV OVERRIDE - Controlling build of wrapper methods based on SDL token
#if defined DISABLE_STDLIB_WRAPPER_METHODS && DISABLE_STDLIB_WRAPPER_METHODS==0

typedef UINTN size_t;

#if defined (__GNUC__) || defined (__clang__)
#define GLOBAL_USED  __attribute__((used))
#else
#define GLOBAL_USED
#endif

/* OpenSSL will use floating point support, and C compiler produces the _fltused
   symbol by default. Simply define this symbol here to satisfy the linker. 
 APTIOV OVERRIDE - When AmiModulePkg >= 39 is used, IntrinsicsLib from AmiModulePkg will be consumed in
 IA32 instead of CryptoPkg's IntrinsicLib. So adding "_fltused" in CrtWrapper files to avoid build error.
 int  GLOBAL_USED _fltused = 1; */

/* Sets buffers to a specified character */
void * __memset (   //APTIOV OVERRIDE - The original memset function is conflict with AmiDxeLib
  void    *dest, 
  int     ch, 
  size_t  count
  ) 
{
  //
  // NOTE: Here we use one base implementation for memset, instead of the direct
  //       optimized SetMem() wrapper. Because the IntrinsicLib has to be built
  //       without whole program optimization option, and there will be some
  //       potential register usage errors when calling other optimized codes.
  //

  //
  // Declare the local variables that actually move the data elements as
  // volatile to prevent the optimizer from replacing this function with
  // the intrinsic memset()
  //
  volatile UINT8  *Pointer;

  Pointer = (UINT8 *)dest;
  while (count-- != 0) {
    *(Pointer++) = (UINT8)ch;
  }

  return dest;
}

/* Compare bytes in two buffers. */
int
memcmp (
  const void  *buf1,
  const void  *buf2,
  size_t      count
  )
{
  return (int)CompareMem (buf1, buf2, count);
}

int
strcmp (
  const char  *s1,
  const char  *s2
  )
{
  return (int)AsciiStrCmp (s1, s2);
}
#endif //APTIOV OVERRIDE - Controlling build of wrapper methods based on SDL token