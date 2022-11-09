//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file StyleHook.h
    Style Hook List.

**/
//**********************************************************************

#include "page.h"
#include "frame.h"

EFI_STATUS StyleGetWindowColor(UINT8 *Color, INTN Level, UINT8 WindowType) ;
EFI_STATUS StyleGetMessageboxColor( UINT8 MessageBoxType, UINT8 *Color);
VOID NewStyleHook1(VOID);
EFI_STATUS StyleControlColor(FRAME_DATA *frame, STYLECOLORS *Colors);
EFI_STATUS OverRideControlColor(FRAME_DATA *frame, STYLECOLORS *Colors);
VOID	StyleInit( VOID );
VOID	StyleExit( VOID );
VOID	StyleDrawPageBorder( UINT32 page );
UINT32	StyleGetPageFrames( UINT32 page );
UINT32		StyleFrameIndexOf( UINT32 frameType );
VOID		StyleBeforeFrame( FRAME_DATA *frame ) ;
EFI_STATUS	StyleFrameDrawable( FRAME_DATA *frame );
VOID	StyleUpdateFrameStrings(PAGE_DATA *page);
VOID *	StyleGetFrameInitData( UINT32 page, UINT32 frame );

EFI_STATUS StyleFrameHandleMouse( FRAME_DATA *frame, MOUSE_INFO MouseInfo);
EFI_STATUS StyleFrameHandleTimer( FRAME_DATA *frame, ACTION_DATA *action );
EFI_STATUS StyleFrameHandleKey( FRAME_DATA *frame, EFI_INPUT_KEY Key );
EFI_STATUS StyleHandleControlOverflow( FRAME_DATA *frame, CONTROL_DATA *control, UINT16 count );
EFI_STATUS StyleInitializeFrame( FRAME_DATA *frame, UINT32 frameType, PAGE_INFO *data );
UINT16 StyleGetNavToken( UINT32 page );
EFI_STATUS StyleAddAdditionalControls( FRAME_DATA *frame, UINT32 controlNumber, BOOLEAN focus );

//EFI_STATUS StyleMenuCallback( FRAME_DATA *frame, MENU_DATA *menu, CALLBACK_MENU *data );
VOID StyleLabelCallback( FRAME_DATA *frame, LABEL_DATA *label, VOID *cookie );

EFI_STATUS _StyleAddControl( FRAME_DATA *frame, UINT16 type, VOID *data, UINT32 *number, BOOLEAN *focus );
UINT16 StyleLanguagePage( SUBMENU_DATA *submenu );
UINT16 StyleBootManagerPage( SUBMENU_DATA *submenu );
BOOLEAN StyleHelpAreaScrollable(VOID);
VOID StylePageItemFocus(PAGE_DATA *page, FRAME_DATA *frame );
VOID StyleSubPageItemFocus(PAGE_DATA *page, FRAME_DATA *frame );
BOOLEAN StyleGetShadowSupport(VOID);
VOID StyleDrawScrollBar(FRAME_DATA *frame, UINT32 FirstLine, UINT32 LastLine, UINT32 modVal, 
				UINT32 sizeOfBar, UINT32 numOfBlocks, BOOLEAN bEraseScrollBar);

VOID StyleDrawHelpScrollBar( MEMO_DATA *memo, UINT16 height);
BOOLEAN   IsHelpAreaScrollBarSupport (void);


VOID StyleSelectFirstDisplayPage( VOID );
VOID StyleAddExtraPagesData( VOID );
EFI_STATUS StyleGetSpecialColor( UINT16 ControlType, UINT8 *Color);
UINTN StyleGetDateFormat(VOID);
BOOLEAN StyleShowDay(VOID);
BOOLEAN StyleGetScrollBehavior(VOID) ;

VOID StyleFrameSetControlPositions(FRAME_DATA *frame,  UINT32 *pOtherEnd);

// Hotkey template moved to StyleHoook.h
#define SUBMENU_COMPLETE_REDRAW		2
#define MENU_COMPLETE_REDRAW		1
#define DEACTIVATE_CONTROLS         3

EFI_STATUS StyleFrameAddTitle( FRAME_DATA *frame, UINT32 frameType,CONTROL_INFO * dummy );
