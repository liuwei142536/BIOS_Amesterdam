//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2022, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file ResetButton.h
    Header file for ResetButton data

**/

#ifndef _RESET_BUTTON_H_
#define _RESET_BUTTON_H_


#include "control.h"
#include "action.h"

typedef struct _RESET_BUTTON_METHODS	RESET_BUTTON_METHODS;

#define	RESET_BUTTON_MEMBER_VARIABLES \
  LABEL_MEMBER_VARIABLES

typedef struct _RESET_BUTTON_DATA
{
	RESET_BUTTON_METHODS	*Methods;

	OBJECT_MEMBER_VARIABLES
	CONTROL_MEMBER_VARIABLES
	RESET_BUTTON_MEMBER_VARIABLES
}
RESET_BUTTON_DATA;

#define RESET_BUTTON_METHOD_FUNCTIONS

struct _RESET_BUTTON_METHODS
{
	OBJECT_METHOD_FUNCTIONS
	CONTROL_METHOD_FUNCTIONS
  RESET_BUTTON_METHOD_FUNCTIONS
};

extern RESET_BUTTON_METHODS gResetButton;

// Object Methods
EFI_STATUS ResetButtonCreate( VOID **object );
EFI_STATUS ResetButtonInitialize( VOID *object, VOID *data );
EFI_STATUS ResetButtonDraw( VOID *object );
EFI_STATUS ResetButtonHandleAction(VOID *object, ACTION_DATA *Data);

#endif

