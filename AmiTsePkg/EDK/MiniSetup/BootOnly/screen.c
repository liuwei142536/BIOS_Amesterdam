//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file screen.c
    This file contains code for screen operations.

**/

#include "minisetup.h"

VOID MouseStop(VOID);
VOID MouseRefresh(VOID);
BOOLEAN IsMouseSupported(VOID);
BOOLEAN IsSoftKbdSupported(VOID);
extern UINTN  gGraphicsVerticalResolution;
extern UINTN  gGraphicsHorizontalResolution;
/**
    function to clear screen without flushing.

    @param Attrib 

    @retval void

**/
VOID ClearScreenWithoutFlush( UINT8 Attrib )
{
	if ( NULL != gActiveBuffer )
	{
		gActiveBuffer->ScreenTag = ACTIVE_SCREEN_TAG;
		MemSet( gActiveBuffer->Attribs, sizeof(gActiveBuffer->Attribs), Attrib );

		MemFillUINT16Buffer( gActiveBuffer->Chars, MAX_DIMENSIONS, (UINT16)L' ' );
	}
}

/**
    function to clear lines without flushing.

    @param LineStart UINT8 NumLines, UINT8 Attrib

    @retval void

**/
VOID ClearLinesWithoutFlush( UINT16 LineStart, UINT16 NumLines, UINT8 Attrib )
{

	gActiveBuffer->ScreenTag = ACTIVE_SCREEN_TAG;
	MemSet(&( gActiveBuffer->Attribs[LineStart*gMaxCols]), NumLines*gMaxCols /*sizeof(gActiveBuffer->Attribs)*/, Attrib );

	MemFillUINT16Buffer( &(gActiveBuffer->Chars[LineStart*gMaxCols]), NumLines*gMaxCols/*MAX_ROWS * MAX_COLS*/, (UINT16)L' ' );
}

/**
    function to clear the screen.

    @param Attrib 

    @retval void

**/
VOID ClearScreen( UINT8 Attrib )
{
	ClearScreenWithoutFlush( Attrib );

	if(gST->ConOut!=NULL)
	{
	    if(!IsWriteCharLastRowColSupport())
	    {    
	        gST->ConOut->EnableCursor( gST->ConOut, FALSE );
	        gST->ConOut->SetAttribute( gST->ConOut, Attrib );
	        gST->ConOut->ClearScreen( gST->ConOut);
	    }
	}

	FlushLines( 0, gMaxRows - 1 );

}  

/**
    function to clear the FlushBuffer.

    @param Attrib 

    @retval void

**/
VOID ClearFlushBuffer(UINT8 Attrib)
{
	if(gFlushBuffer)
		MemSet(gFlushBuffer, sizeof(SCREEN_BUFFER), Attrib);
}

EFI_UGA_PIXEL *gPostscreenwithlogo;
/**
    function to get the backup of the screen.

    @param void

    @retval void

**/
VOID *SaveScreen( VOID )
{
	SCREEN_BUFFER *Buffer = (SCREEN_BUFFER*)NULL;

	Buffer = EfiLibAllocatePool( sizeof(SCREEN_BUFFER) );

	if ( Buffer != NULL )
	{
		MemCopy( Buffer, gActiveBuffer, sizeof(SCREEN_BUFFER) );
		Buffer->ScreenTag = SAVE_SCREEN_TAG;
	}

	return (VOID *)Buffer;
}

/**
    function to restore the screen from the saved screen buffer.

    @param ScreenBuffer 

    @retval void

**/
VOID RestoreScreen( VOID *ScreenBuffer )
{
	SCREEN_BUFFER *Buffer = (SCREEN_BUFFER *)ScreenBuffer;

	MouseStop();

	if (( Buffer == NULL )||( Buffer->ScreenTag != SAVE_SCREEN_TAG ))
	    return;
	
	if ((gActiveBuffer == NULL)||(gFlushBuffer == NULL))
    {
        MemFreePointer( (VOID **)&Buffer );
		return;
    }
	///Clear Original data in the buffers before restoring with saved screen buffer.
	MemSet(gActiveBuffer, sizeof(SCREEN_BUFFER), 0);
	MemSet(gFlushBuffer, sizeof(SCREEN_BUFFER), 0);

	MemCopy( gActiveBuffer, Buffer, sizeof(SCREEN_BUFFER) );
	gActiveBuffer->ScreenTag = ACTIVE_SCREEN_TAG;
	FlushLines( 0, gMaxRows - 1 );
	DoRealFlushLines();

	MemFreePointer( (VOID **)&Buffer );
	MouseRefresh();
}

/**
    Saves post graphics screen

    @param VOID

    @retval VOID

**/
VOID SaveGraphicsScreen(VOID)
{
   UINTN HorizontalResolution = 0;
   UINTN VerticalResolution = 0;
   GetScreenResolution(&HorizontalResolution, &VerticalResolution);
   SETUP_DEBUG_TSE("[TSE] SaveGraphicsScreen: [%dx%d]\n",HorizontalResolution, VerticalResolution);
		
		if(NULL != gPostscreenwithlogo)
		{
			MemFreePointer( (VOID **)&gPostscreenwithlogo );
		}
		
		gPostscreenwithlogo = EfiLibAllocateZeroPool( HorizontalResolution * VerticalResolution * sizeof(EFI_UGA_PIXEL)); 

		if (NULL == gPostscreenwithlogo)
		{
		 	SETUP_DEBUG_TSE("[TSE] Not able to save screen. Out of memory. \n");
			return;
		}
		 MouseStop();
		 if ( gGOP){
		     gGOP->Blt (
		             gGOP,
		             gPostscreenwithlogo,
		             EfiBltVideoToBltBuffer,
		             (UINTN)0, (UINTN)0,
		             (UINTN)0, (UINTN)0,
		             HorizontalResolution, VerticalResolution,
		             0
		     );
	}
    MouseRefresh();
}

/**
    Restores the saved post graphics screen

    @param VOID

    @retval VOID

**/
VOID RestoreGraphicsScreen (VOID)
{
   if( gPostscreenwithlogo != NULL )
   {
      UINTN HorizontalResolution = 0;
      UINTN VerticalResolution = 0;
      
      GetScreenResolution(&HorizontalResolution, &VerticalResolution);
      SETUP_DEBUG_TSE ("[TSE] RestoreGraphicsScreen [%dx%d] \n",HorizontalResolution, VerticalResolution);
      MouseStop();
      if ( gGOP){
         gGOP->Blt (
         gGOP,
         gPostscreenwithlogo,
         EfiBltBufferToVideo,
         (UINTN)0, (UINTN)0,
         (UINTN)0, (UINTN)0,
         HorizontalResolution, VerticalResolution,
         0
      );
	}
      MouseRefresh();
//  Memory will be freed during CleanUpLogo()
//      MemFreePointer( (VOID **)&gPostscreenwithlogo );
   }
   else
		SETUP_DEBUG_TSE ("[TSE] Not able to restore screen\n",gGOP);
}

/**
    function to update the TSE global variables and screen.

    @param void

    @retval void

**/
VOID UpdateScreen( VOID )
{
	EFI_STATUS Status;
	UINTN ConCols,ConRows;
	
	if(gGOP && ((TSE_BEST_HORIZONTAL_RESOLUTION > gGOP->Mode->Info->HorizontalResolution) || (TSE_BEST_VERTICAL_RESOLUTION > gGOP->Mode->Info->VerticalResolution)))
	{
		SetScreenResolution(TSE_BEST_HORIZONTAL_RESOLUTION,TSE_BEST_VERTICAL_RESOLUTION);
	}
	
	if(gGOP && gGraphicsHorizontalResolution && gGraphicsVerticalResolution && ((gGraphicsHorizontalResolution != gGOP->Mode->Info->HorizontalResolution) || (gGraphicsVerticalResolution != gGOP->Mode->Info->VerticalResolution)))
	{ //Revert the existing resolution. If existing resolution not supported in new GOP, then free the gPostscreenwithlogo whose memory allocated based on existing resolution. If not, screen will corrupt. 
		Status = SetScreenResolution(gGraphicsHorizontalResolution,gGraphicsVerticalResolution);
		if(EFI_ERROR(Status) && gPostscreenwithlogo )
		{
			MemFreePointer( (VOID **)&gPostscreenwithlogo );
		}
	}
	
#if !TSE_USE_EDK_LIBRARY	
	if(gConsoleControl != NULL)
	{
		if(TSE_POST_STATUS_IN_QUITE_BOOT_SCREEN == gPostStatus)
		{
			gConsoleControl->SetMode( gConsoleControl, EfiConsoleControlScreenGraphics );
		}
		else
		{
			gConsoleControl->SetMode( gConsoleControl, EfiConsoleControlScreenText );
		}
		
	}	
#endif	
	if(IsMouseSupported())
	{
		if(((gPostStatus < TSE_POST_STATUS_IN_TSE)&&IsSoftKbdSupported())||
				(gPostStatus >= TSE_POST_STATUS_IN_TSE))
		{
			MouseInit();
		}
	}
	
	if(gST && gST->ConOut)
	{
		Status = gST->ConOut->QueryMode( gST->ConOut, gST->ConOut->Mode->Mode, &ConCols, &ConRows);
		if (EFI_ERROR(Status))
			return ;

		if ((STYLE_STD_MAX_ROWS > ConRows )  || (STYLE_STD_MAX_COLS > ConCols))
		{
			gMaxRows = StyleGetStdMaxRows();
			gMaxCols = StyleGetStdMaxCols(); 
			SetDesiredTextMode ();
		}
	}
	
}

