//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
//*****************************************************************//
/** @file ezportplus.h
    Main header file for ezportplus style module.

**/
//**********************************************************************
#ifndef _EZPORTPLUS_H_
#define _EZPORTPLUS_H_

#if SETUP_STYLE_EZPORTPLUS

VOID 		OverRideStyleInit( VOID );
VOID		OverRideStyleDrawPageBorder( UINT32 page );
VOID 		*OverRideStyleGetFrameInitData( UINT32 page, UINT32 frame );
VOID        OverRideComponent(VOID);
EFI_STATUS  OverRideEzportPlusFrameDestroy( FRAME_DATA *frame, BOOLEAN freeMem );

#if SETUP_STYLE_FULL_SCREEN
	#define	STYLE_MAX_COLS	STYLE_FULL_MAX_COLS
	#define	STYLE_MAX_ROWS	STYLE_FULL_MAX_ROWS
#else
	#define	STYLE_MAX_COLS	STYLE_STD_MAX_COLS
	#define	STYLE_MAX_ROWS	STYLE_STD_MAX_ROWS
#endif

// Style Overrides...
#define STYLE_OVERRIDE_INIT
#define STYLE_OVERRIDE_PAGE_BORDER
#define STYLE_OVERRIDE_FRAME_INIT_DATA
#define STYLE_OVERRIDE_CONTROL_OVERFLOW
#define STYLE_OVERRIDE_INITIALIZE_FRAME
#define STYLE_OVERRIDE_INITIALIZE_OEM_FRAME
#define STYLE_OVERRIDE_NAV_TOKEN
#define STYLE_OVERRIDE_ADD_CONTROLS
#define STYLE_OVERRIDE_FIRST_PAGE
#define	STYLE_OVERRIDE_VERSION_STRING
#define STYLE_COMPONENT_OVERRIDE
#define STYLE_OVERRIDE_FRAME_DRAWABLE

#endif /* SETUP_STYLE_EZPORTPLUS */

#endif /* _EZPORTPLUS_H_ */
