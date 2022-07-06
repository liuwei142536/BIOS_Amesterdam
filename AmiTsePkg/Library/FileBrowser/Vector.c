//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file
  This file implements supplemental Vector structure
  
**/

#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#include "Vector.h"

/**
  This function initializes new Vector structure
  
  @param  init_size Initial size of vector in elements
  @param  elem_size Vector element size
  @param  elem_inc	 Number of bytes to read
  
  @return Pointer to created Vector

**/
AMI_VECTOR * VectorInit (UINT32 init_size, UINT32 elem_size, UINT32 elem_inc)
{
	AMI_VECTOR *v;
	v = AllocatePool (sizeof (*v));
	v->m_size = init_size * elem_size;
	v->c_size = 0;
	v->e_size = elem_size;
	v->e_inc = elem_inc;
	v->buf = AllocatePool (v->m_size);
	return v;
}

/**
  This function releases Vector structure
  
  @param  v Pointer to vector to release

**/
VOID VectorDestroy (AMI_VECTOR *v)
{
	FreePool (v->buf);
	FreePool (v);
}

/**
  This function grows Vector structure
  
  @param  v Pointer to vector to grow

**/
VOID VectorGrow (AMI_VECTOR *v)
{
	VOID *p;
	v->m_size += v->e_size * v->e_inc;
	p = AllocatePool (v->m_size);
	CopyMem (p, v->buf, v->c_size);
	FreePool (v->buf);
	v->buf = p;
}

/**
  This function adds element to vector
  
  @param  v Pointer to vector
  @param  e Pointer to element

**/
VOID VectorAdd (AMI_VECTOR *v, VOID *e)
{
	UINT8 *p;
	if (v->c_size == v->m_size)
		VectorGrow (v);
    p = (UINT8 *)v->buf; 
	CopyMem (&p[v->c_size], e, v->e_size);
	v->c_size += v->e_size;
}

/**
  This function returns vector size
  
  @param  v Pointer to vector
  
  @return Number of elements in vector

**/
UINTN VectorGetSize (AMI_VECTOR *v)
{
	return v->c_size / v->e_size;
}

/**
  This function returns element of vector by index
  
  @param  v   Pointer to vector
  @param  idx Element index

  @return Pointer to element or NULL if index is out of range
**/
VOID * VectorGetElement (AMI_VECTOR *v, UINTN idx)
{
	UINT8 *p = (UINT8 *)v->buf;
	if (idx >= VectorGetSize (v))
		return NULL;
	return &p[idx * v->e_size];
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
