//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file hotkey.h
    Header file for hotkey code to handling

**/

#ifndef _HOTKEY_H_
#define _HOTKEY_H_

#include "object.h"
#include "action.h"

/*
typedef struct _HOTKEY_TEMPLATE
{
	EFI_INPUT_KEY	Key;
	UINT32			KeySftSte;
	OBJECT_CALLBACK	Callback;
	UINT32			BasePage;
}
HOTKEY_TEMPLATE;
*/
typedef struct _HOTKEY_INFO
{
	EFI_INPUT_KEY	Key;
	UINT32			KeySftSte;
}
HOTKEY_INFO;

#define	HOTKEY_MEMBER_VARIABLES	\
	HOTKEY_INFO		HotKeyData;	\
	UINT32			BasePage;

typedef struct _HOTKEY_METHODS HOTKEY_METHODS;

typedef struct _HOTKEY_DATA
{
	HOTKEY_METHODS	*Methods;

	OBJECT_MEMBER_VARIABLES
	HOTKEY_MEMBER_VARIABLES

}
HOTKEY_DATA;

#define	HOTKEY_METHOD_FUNCTIONS

struct _HOTKEY_METHODS
{
	OBJECT_METHOD_FUNCTIONS
	HOTKEY_METHOD_FUNCTIONS
};

extern HOTKEY_METHODS gHotKey;

// Object Methods
EFI_STATUS HotKeyCreate( HOTKEY_DATA **object );
EFI_STATUS HotKeyDestroy( HOTKEY_DATA *object, BOOLEAN freeMem );
EFI_STATUS HotKeyInitialize( HOTKEY_DATA *object, VOID *data );
EFI_STATUS HotKeyEmptyMethod( HOTKEY_DATA *object );
EFI_STATUS HotKeyHandleAction( HOTKEY_DATA *object, ACTION_DATA *data );
EFI_STATUS HotKeySetCallback( HOTKEY_DATA *object, OBJECT_DATA *container, OBJECT_CALLBACK callback, VOID *cookie );

#endif /* _HOTKEY_H_ */
