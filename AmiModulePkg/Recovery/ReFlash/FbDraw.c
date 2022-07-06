//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
/**
 * @file FbDraw.c
 * @brief Direct framebuffer write routines
 */

#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#include "ReFlashSmm.h"

#define LOGO_WIDTH  160			//in pixels
#define LOGO_HEIGHT 120			//in pixels
#define PROGRESS_BAR_THICKNESS 6

extern unsigned char mono_logo[];
extern unsigned int mono_logo_len;

/**
 * @brief Progress bar drawing routine
 *
 * @param Info Pointer to framebuffer info
 * @param pc Progress percentage (0-100%)
 */
VOID DrawProgress (AMI_REFLASH_FRAMEBUFFER_INFO *Info, UINT16 pc)
{
    UINT32 *dst = Info->FrameBuf;
    
    UINT16 p_line_size;					//width of progress bar in pixels
    UINT16 px_per_pc;					//pixels per 1%
    UINT16 p_height = PROGRESS_BAR_THICKNESS;		//progress bar thickness in
    UINT32 p_y_offset;					//start row of bar in pixels
    UINT16 p_x_offset;					//start col of bar in pixels

    UINT32 *p_line, *s_p_line;
    int pls, plc, i;
    
    if (!dst)
        return;
    
    p_line_size = Info->HorRes * 8 / 10;		//make progress bar 70% of screen width
    p_line_size -= p_line_size % 100;			//round down to 100
    px_per_pc = p_line_size / 100;
    p_x_offset = (Info->HorRes - p_line_size) / 2;
    p_y_offset = (Info->VerRes - LOGO_HEIGHT - 3 * PROGRESS_BAR_THICKNESS) / 2;
    p_y_offset += LOGO_HEIGHT + PROGRESS_BAR_THICKNESS;
    p_y_offset *= Info->PxScanLine;			//convert to pixels (row_num * pixels_per_scanline)
    
    p_line = (UINT32 *)AllocatePool (p_line_size * 4);
    s_p_line = p_line;
    plc = pc * px_per_pc;				//part of line show progress
    pls = p_line_size - plc; 				//remainder
    while (plc--)
        *p_line++ = 0x00ffffff;				//fill line with white color
    while (pls--)
        *p_line++ = 0x00808080;				//fill line with gray color
    p_line = s_p_line;
    
    dst += p_x_offset + p_y_offset;
    for (i = 0; i < p_height; i++, dst += Info->PxScanLine)
        CopyMem (dst, p_line, p_line_size * 4);
    FreePool (p_line);
}

/**
 * @brief AMI Logo drawing routine
 *
 * @param Info Pointer to framebuffer info
 */
VOID DrawLogo (AMI_REFLASH_FRAMEBUFFER_INFO *Info)
{
    UINT32 *dst = Info->FrameBuf;
    
    UINT16 logo_width = LOGO_WIDTH;
    UINT16 logo_height = LOGO_HEIGHT;
    UINT16 logo_x_offset;
    UINT32 logo_y_offset;		//lowest row, since we draw from bottom to top
    
    UINT32 *p_line, *s_p_line;
    UINT32 sz = mono_logo_len;
    UINT8 *src = mono_logo;
    UINT8 c;
    int i;
    UINT32 red = (Info->PxMask == 0) ? 0x000000ff : 0x00ff0000;
    
    if (!dst)
        return;
    
    logo_x_offset = (Info->HorRes - logo_width) / 2;
    logo_y_offset = (Info->VerRes - logo_height - 3 * PROGRESS_BAR_THICKNESS) / 2;
    logo_y_offset += logo_height;
    logo_y_offset *= Info->PxScanLine;	//convert to pixels (row_num * pixels_per_scanline)
    
    p_line = (UINT32 *)AllocatePool (logo_width * logo_height * 4);
    s_p_line = p_line;
    
    while (sz--) {
        c = ~(*src++);
        *p_line++ = (c & 0x80) ? red : 0x00000000;
        *p_line++ = (c & 0x40) ? red : 0x00000000;
        *p_line++ = (c & 0x20) ? red : 0x00000000;
        *p_line++ = (c & 0x10) ? red : 0x00000000;
        *p_line++ = (c & 0x08) ? red : 0x00000000;
        *p_line++ = (c & 0x04) ? red : 0x00000000;
        *p_line++ = (c & 0x02) ? red : 0x00000000;
        *p_line++ = (c & 0x01) ? red : 0x00000000;
    }
    p_line = s_p_line;
    
    dst += logo_x_offset + logo_y_offset;
    for (i = 0; i < logo_height; i++, p_line += logo_width, dst -= Info->PxScanLine)
        CopyMem (dst, p_line, logo_width * 4);
    FreePool (s_p_line);
}




//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
