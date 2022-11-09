//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file page.h
    Header file for code to handle page level operations

**/

#ifndef _PAGE_H_
#define _PAGE_H_

#include "object.h"
#include "action.h"
#include "frame.h"
#include "hotkey.h"

#define	PAGE_MEMBER_VARIABLES	\
	PAGE_INFO	PageData;		\
	UINT32		CurrentFrame;	\
	UINT32		FrameCount;		\
	FRAME_DATA	**FrameList;	\
	UINT32		HotKeyCount;	\
	HOTKEY_DATA	**HotKeyList;

typedef struct _PAGE_METHODS PAGE_METHODS;

typedef struct _PAGE_DATA
{
	PAGE_METHODS	*Methods;

	OBJECT_MEMBER_VARIABLES
	PAGE_MEMBER_VARIABLES

}
PAGE_DATA;

#define	PAGE_METHOD_FUNCTIONS

struct _PAGE_METHODS
{
	OBJECT_METHOD_FUNCTIONS
	PAGE_METHOD_FUNCTIONS
};

extern PAGE_METHODS gPage;

// Object Methods
EFI_STATUS PageCreate( PAGE_DATA **object );
EFI_STATUS PageDestroy( PAGE_DATA *object, BOOLEAN freeMem );
EFI_STATUS PageInitialize( PAGE_DATA *object, PAGE_INFO *data );
EFI_STATUS PageDraw( PAGE_DATA *object );
EFI_STATUS PageHandleAction( PAGE_DATA *object, ACTION_DATA *data );
EFI_STATUS PageSetCallback( PAGE_DATA *object, OBJECT_DATA *container, OBJECT_CALLBACK callback, VOID *cookie );

VOID _PageUpdateMemo( PAGE_DATA *page, UINT32 frameType, /*EFI_HII_HANDLE*/VOID * handle, UINT16 token );

VOID _PageUpdateHelp( PAGE_DATA *page );
VOID _PageUpdateSubtitle( PAGE_DATA *page );
VOID _PageUpdateTitle( PAGE_DATA *page );
VOID _PageUpdateNavStrings( PAGE_DATA *page );
VOID _PageUpdateFrameStrings( PAGE_DATA *page );

#endif /* _PAGE_H_ */
