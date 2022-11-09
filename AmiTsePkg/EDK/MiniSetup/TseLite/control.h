//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#ifndef _CONTROL_H_
#define	_CONTROL_H_

#include "object.h"
#include "setupdata.h"
#include "action.h"
#if UEFI_SOURCES_SUPPORT
#include "CtrlCond.h"
#endif

#define CONTROL_GRAYOUT_COLOR STYLE_COLOR_GRAYOUT

#define	CONTROL_MEMBER_VARIABLES		\
	CONTROL_INFO	ControlData;		\
	UINT16			Left;				\
	UINT16			Top;				\
	UINT16			Width;				\
	UINT16			Height;				\
	UINT8			FGColor;			\
    UINT8			BGColor;			\
	BOOLEAN			ControlFocus;		\
	BOOLEAN			ControlActive;		\
	UINT32			ParentFrameType;

typedef struct _CONTROL_METHODS	CONTROL_METHODS;

typedef struct _CONTROL_DATA
{
	CONTROL_METHODS	*Methods;

	OBJECT_MEMBER_VARIABLES
	CONTROL_MEMBER_VARIABLES

}
CONTROL_DATA;

typedef	EFI_STATUS	(*CONTROL_METHOD_SET_FOCUS)		( CONTROL_DATA *object, BOOLEAN focus );
typedef	EFI_STATUS	(*CONTROL_METHOD_SET_POSITION)	( CONTROL_DATA *object, UINT16 Left, UINT16 Top );
typedef	EFI_STATUS	(*CONTROL_METHOD_SET_DIMENSIONS)	( CONTROL_DATA *object, UINT16 Width, UINT16 Height );
typedef	EFI_STATUS	(*CONTROL_METHOD_SET_ATTRIBUTES)( CONTROL_DATA *object, UINT8 FGColor, UINT8 BGColor );
typedef	EFI_STATUS	(*CONTROL_METHOD_GET_CONTROL_HIGHT)( CONTROL_DATA *object, VOID *frame, UINT16 *height  );

#define	CONTROL_METHOD_FUNCTIONS					\
	CONTROL_METHOD_SET_FOCUS		SetFocus;		\
	CONTROL_METHOD_SET_POSITION		SetPosition;	\
	CONTROL_METHOD_SET_DIMENSIONS    SetDimensions;	\
	CONTROL_METHOD_SET_ATTRIBUTES	SetAttributes; \
	CONTROL_METHOD_GET_CONTROL_HIGHT GetControlHeight;

struct _CONTROL_METHODS
{
	OBJECT_METHOD_FUNCTIONS
	CONTROL_METHOD_FUNCTIONS
};

extern CONTROL_METHODS gControl;

// Object Methods
EFI_STATUS ControlCreate( CONTROL_DATA **object );
EFI_STATUS ControlDestroy( CONTROL_DATA *object, BOOLEAN freeMem );
EFI_STATUS ControlInitialize( CONTROL_DATA *object, VOID *data );
EFI_STATUS ControlEmptyMethod( CONTROL_DATA *object );
EFI_STATUS ControlDraw( CONTROL_DATA *object );
EFI_STATUS ControlHandleAction( CONTROL_DATA *object, ACTION_DATA *Data );
EFI_STATUS ControlSetCallback( CONTROL_DATA *object, OBJECT_DATA *container, OBJECT_CALLBACK callback, VOID *cookie );

// Control Methods
EFI_STATUS ControlSetFocus(CONTROL_DATA *object, BOOLEAN focus);
EFI_STATUS ControlSetPosition(CONTROL_DATA *object, UINT16 Left, UINT16 Top);
EFI_STATUS ControlSetDimensions(CONTROL_DATA *object, UINT16 Width, UINT16 Height);
EFI_STATUS ControlSetAttributes(CONTROL_DATA *object, UINT8 FGColor, UINT8 BGColor);
CONTROL_DATA * GetUpdatedControlData(CONTROL_DATA * ControlData,UINT16 ControlType,VOID *Handle,UINT16 Key);


#endif /* _CONTROL_H_ */
