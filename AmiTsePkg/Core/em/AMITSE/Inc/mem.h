//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
//*****************************************************************//
/** @file mem.h
    memory utility functions

**/

#ifndef _MEM_H_
#define _MEM_H_

VOID MemFreePointer (VOID **ptr);
VOID StringZeroFreeMemory (VOID **ptr);
VOID MemCopy( VOID *dest, VOID *src, UINTN size );
VOID MemSet( VOID *buffer, UINTN size, UINT8 value );
INTN MemCmp( VOID *dest, VOID *src, UINTN size );
VOID *MemReallocateZeroPool( VOID *oldBuffer, UINTN oldSize, UINTN newSize );
VOID MemFillUINT16Buffer( VOID *buffer, UINTN bufferSize, UINT16 value );

#endif /* _MEM_H_ */
