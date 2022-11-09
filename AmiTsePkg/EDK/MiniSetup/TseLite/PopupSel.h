//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
//*****************************************************************//
/** @file PopupSel.h
    Header file for Popup selections

**/

#ifndef _POPUPSEL_H_
#define	_POPUPSEL_H_

#include "control.h"
#include "ListBox.h" 

#define POPUPSEL_VALUE_BASE		UINT64
#define	POPUPSEL_MAX_WIDTH	(gControlRightAreaWidth)
typedef struct _PTRTOKENS
{
	UINT16 Option;
	POPUPSEL_VALUE_BASE Value;
}PTRTOKENS;

#define	POPUPSEL_MEMBER_VARIABLES	\
	UINT16			Title;			\
	UINT8			LabelFGColor;	\
	UINT8	NSelLabelFGColor;		\
	UINT16 			LabelMargin;	\
	UINT8			SelFGColor;		\
	UINT8			SelBGColor;		\
	UINT8			NSelFGColor;	\
	LISTBOX_DATA	*ListBoxCtrl;	\
	BOOLEAN			ListBoxEnd;		\
	/*EFI_HII_HANDLE*/VOID *			PopupSelHandle;  \
	UINT16			Sel;            \
	UINT16			ItemCount;		\
    UINT16          bInteractive;   \
    UINT16          CallBackKey;    \
    PTRTOKENS		*PtrTokens;		\
	UINT8   Interval;


typedef struct _POPUPSEL_METHODS	POPUPSEL_METHODS;

typedef struct _POPUPSEL_DATA
{
	POPUPSEL_METHODS	*Methods;

	OBJECT_MEMBER_VARIABLES
	CONTROL_MEMBER_VARIABLES
	POPUPSEL_MEMBER_VARIABLES

}
POPUPSEL_DATA;


struct _POPUPSEL_METHODS
{
	OBJECT_METHOD_FUNCTIONS
	CONTROL_METHOD_FUNCTIONS	
};

extern POPUPSEL_METHODS gPopupSel;

// Object Methods
EFI_STATUS PopupSelCreate( POPUPSEL_DATA **object );
EFI_STATUS PopupSelDestroy( POPUPSEL_DATA *object, BOOLEAN freeMem );
EFI_STATUS PopupSelInitialize( POPUPSEL_DATA *object, VOID *data );
EFI_STATUS PopupSelDraw( POPUPSEL_DATA *object );
EFI_STATUS PopupSelHandleAction(POPUPSEL_DATA *object, ACTION_DATA *Data);
EFI_STATUS PopupSelSetCallback(  POPUPSEL_DATA *object, OBJECT_DATA *container, OBJECT_CALLBACK callback, VOID *cookie );

// Control Methods
EFI_STATUS PopupSelSetFocus(POPUPSEL_DATA *object, BOOLEAN focus);
EFI_STATUS PopupSelSetPosition(POPUPSEL_DATA *object, UINT16 Left, UINT16 Top);
EFI_STATUS PopupSelSetDimensions(POPUPSEL_DATA *object, UINT16 Width, UINT16 Height);
EFI_STATUS PopupSelSetAttributes(POPUPSEL_DATA *object, UINT8 FGColor, UINT8 BGColor);
EFI_STATUS PopupSelGetControlHeight( POPUPSEL_DATA *object,VOID *frame, UINT16 *height );

VOID _PopupSelCallback( POPUPSEL_DATA *container, CONTROL_DATA *popupSel, VOID *cookie );
VOID RearrangeBootDriverOrderVariable (POPUPSEL_DATA *popupSel, UINT8 bIncrease);		//EIP70421 & 70422  Support for driver order

#endif /* _POPUPSEL_H_ */
