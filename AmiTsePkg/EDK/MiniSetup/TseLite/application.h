//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
//*****************************************************************//
/** @file application.h
    Header file for code to handle Application.

**/
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "object.h"
#include "action.h"
#include "page.h"
#include "hotkey.h"
#include "hotclick.h"

#define	APPLICATION_MEMBER_VARIABLES	\
	ACTION_DATA			*Action;		\
	UINT32				CurrentPage;	\
	UINT32				PageCount;		\
	PAGE_DATA			**PageList;		\
	UINTN				HotKeyCount;	\
	HOTKEY_DATA			**HotKeyList;	\
	UINTN				HotClickCount;	\
	HOTCLICK_DATA			**HotClickList;\
	BOOLEAN				CompleteRedraw;	\
	BOOLEAN				ClearScreen;	\
	BOOLEAN				Quit;			\
	UINT32				OnRedraw;\
    BOOLEAN             Fixed;

typedef struct _APPLICATION_METHODS APPLICATION_METHODS;

typedef struct _APPLICATION_INFO
{
	UINT32	PageCount;
}
APPLICATION_INFO;

typedef struct _APPLICATION_DATA
{
	APPLICATION_METHODS	*Methods;

	OBJECT_MEMBER_VARIABLES
	APPLICATION_MEMBER_VARIABLES

}
APPLICATION_DATA;

typedef EFI_STATUS		(*APPLICATION_METHOD_ADD_PAGE)	( APPLICATION_DATA *app, VOID *data );

#define APPLICATION_METHOD_FUNCTIONS		\
	APPLICATION_METHOD_ADD_PAGE	AddPage;

struct _APPLICATION_METHODS
{
	OBJECT_METHOD_FUNCTIONS
	APPLICATION_METHOD_FUNCTIONS
};

extern APPLICATION_METHODS gApplication;
extern APPLICATION_DATA *gApp;

// Object Methods
EFI_STATUS ApplicationCreate( APPLICATION_DATA **object );
EFI_STATUS ApplicationDestroy( APPLICATION_DATA *object, BOOLEAN freeMem );
EFI_STATUS ApplicationInitialize( APPLICATION_DATA *object, VOID *data );
EFI_STATUS ApplicationEmptyMethod( APPLICATION_DATA *object );
EFI_STATUS ApplicationHandleAction( APPLICATION_DATA *object, ACTION_DATA *Data );
EFI_STATUS ApplicationSetCallback( APPLICATION_DATA *object, OBJECT_DATA *container, OBJECT_CALLBACK callback, VOID *cookie );

// Application Methods
EFI_STATUS ApplicationAddPage( APPLICATION_DATA *app, VOID *data );

#endif /* _APPLICATION_H_ */
