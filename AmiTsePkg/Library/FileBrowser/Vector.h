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
  This file contains Vector interface definitions

**/
#ifndef __AMI_VECTOR__H__
#define __AMI_VECTOR__H__
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
	VOID *buf;
	UINT32 m_size;	//max buffer size in bytes
	UINT32 c_size;	//used buffer size in bytes
	UINT32 e_size;	//element size in bytes
	UINT32 e_inc;	//element increment
} AMI_VECTOR;

AMI_VECTOR * VectorInit (UINT32 init_size, UINT32 elem_size, UINT32 elem_inc);
VOID VectorDestroy (AMI_VECTOR *v);
VOID VectorGrow (AMI_VECTOR *v);
VOID VectorAdd (AMI_VECTOR *v, VOID *e);
UINTN VectorGetSize (AMI_VECTOR *v);
VOID * VectorGetElement (AMI_VECTOR *v, UINTN idx);

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif
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
