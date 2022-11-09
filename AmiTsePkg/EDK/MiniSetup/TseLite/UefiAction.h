//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2022, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file UefiAction.h
    Header file for UefiAction data

**/

#ifndef _UEFI_ACTION_H_
#define _UEFI_ACTION_H_


#include "control.h"
#include "action.h"

typedef struct _UEFI_ACTION_METHODS	UEFI_ACTION_METHODS;

#define	UEFI_ACTION_MEMBER_VARIABLES \
  LABEL_MEMBER_VARIABLES \
  UINT8	Interval;

typedef struct _UEFI_ACTION_DATA
{
	UEFI_ACTION_METHODS	*Methods;

	OBJECT_MEMBER_VARIABLES
	CONTROL_MEMBER_VARIABLES
	UEFI_ACTION_MEMBER_VARIABLES
}
UEFI_ACTION_DATA;

#define UEFI_ACTION_METHOD_FUNCTIONS

struct _UEFI_ACTION_METHODS
{
	OBJECT_METHOD_FUNCTIONS
	CONTROL_METHOD_FUNCTIONS
	LABEL_METHOD_FUNCTIONS
  UEFI_ACTION_METHOD_FUNCTIONS
};

extern UEFI_ACTION_METHODS gUefiAction;

// Object Methods
EFI_STATUS UefiActionCreate( VOID **object );
EFI_STATUS UefiActionInitialize( VOID *object, VOID *data );
EFI_STATUS UefiActionDraw( VOID *object );
EFI_STATUS UefiActionHandleAction(VOID *object, ACTION_DATA *Data);

#endif /*#ifndef _UEFI_ACTION_H_*/

