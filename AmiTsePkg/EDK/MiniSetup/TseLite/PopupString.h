//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file PopupString.h
    Header file for popupstring control

**/

#ifndef _POPUP_STRING_H_
#define	_POPUP_STRING_H_

#include "control.h"
#include "action.h"
#include "Label.h"
#include "PopupEdit.h" 

#define	POPUP_STRING_MEMBER_VARIABLES	\
	CHAR16	*TempText;		\
	UINT16	TextMargin;		\
    UINT16	MinSize;		\
	UINT16	MaxSize;		\
	UINT16	TextWidth;		\
	UINT16  TextAreaWidth;	\
    UINT8	LabelFGColor;	\
	UINT8	NSelLabelFGColor;		\
	UINT8   LabelMargin;	\
	UINT8   Interval;

typedef struct _POPUP_STRING_METHODS	POPUP_STRING_METHODS;

typedef struct _POPUP_STRING_DATA
{
	POPUP_STRING_METHODS	*Methods;

	OBJECT_MEMBER_VARIABLES
	CONTROL_MEMBER_VARIABLES
	LABEL_MEMBER_VARIABLES
	POPUP_STRING_MEMBER_VARIABLES

}
POPUP_STRING_DATA;

#define	POPUP_STRING_METHOD_FUNCTIONS


struct _POPUP_STRING_METHODS
{
	OBJECT_METHOD_FUNCTIONS
	CONTROL_METHOD_FUNCTIONS
	POPUP_STRING_METHOD_FUNCTIONS

};

extern POPUP_STRING_METHODS gPopupString;

// Object Methods
EFI_STATUS PopupStringCreate( POPUP_STRING_DATA **object );
EFI_STATUS PopupStringDestroy( POPUP_STRING_DATA *object, BOOLEAN freeMem );
EFI_STATUS PopupStringInitialize( POPUP_STRING_DATA *object, VOID *data );
EFI_STATUS PopupStringDraw( POPUP_STRING_DATA *object );
EFI_STATUS PopupStringHandleAction(POPUP_STRING_DATA *object, ACTION_DATA *Data);
EFI_STATUS PopupStringSetCallback( POPUP_STRING_DATA *object, OBJECT_DATA *container, OBJECT_CALLBACK callback, VOID *cookie );

// Control Methods
EFI_STATUS PopupStringSetFocus( POPUP_STRING_DATA *object, BOOLEAN focus);
EFI_STATUS PopupStringSetPosition(POPUP_STRING_DATA *object, UINT16 Left, UINT16 Top);
EFI_STATUS PopupStringSetDimensions(POPUP_STRING_DATA *object, UINT16 Width, UINT16 Height);
EFI_STATUS PopupStringSetAttributes(POPUP_STRING_DATA *object, UINT8 FGColor, UINT8 BGColor);
EFI_STATUS PopupStringGetControlHeight(POPUP_STRING_DATA *object, VOID *frame, UINT16 *height);

EFI_STATUS _DoPopupStringEdit( POPUP_STRING_DATA *PopupString, UINT16 Title, CHAR16 **Text);

#endif /* _POPUP_STRING_H_ */

