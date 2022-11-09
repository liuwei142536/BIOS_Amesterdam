//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file logo.c
    file contains code to handle the logo operations.
    Taken from graphics.c to eliminnate library dependancy

**/

#include "minisetup.h"
#include "Protocol/EdidActive.h"
#include "Protocol/EdidDiscovered.h"

// to retrieve the frame coordinates in case different frames in a image
UINT16 gGifSrcX=0;
UINT16 gGifSrcY=0;
UINT16 gGifDestX=0;
UINT16 gGifDestY=0;
BOOLEAN GifImageFlag = 0;
UINTN  gGraphicsVerticalResolution;
UINTN  gGraphicsHorizontalResolution;
extern BOOLEAN gDrawQuietBootMessage;

#define TSE_ABS(a,b)             (((a) < (b)) ? ((b) - (a)) : ((a) - (b)))

extern LOGO_TYPE GetBmpLogoType(UINT8 *ImageData);

extern EFI_STATUS ConvertBmpToUgaBltWrapper (IN VOID *BmpImage, IN UINTN BmpImageSize, IN OUT VOID **UgaBlt,
		IN OUT UINTN  *UgaBltSize, OUT UINTN *PixelHeight, OUT UINTN *PixelWidth );

VOID *ConvertBlt2Bmp ();
VOID AddImageDetailToACPI (UINT8 *, INTN, INTN, BOOLEAN);
UINT8 gAddBgrtResolutions = 0;
VOID SetGetBgrtCoordinates (
	UINTN	*DestX,
	UINTN	*DestY,
	UINTN   *Width,
	UINTN   *Height,
	BOOLEAN Command
	);
VOID InvalidateStatusInBgrtWrapper (VOID);
BOOLEAN IsBGRTSupported(VOID); 
extern void PrintGopAndConInDetails();
extern BOOLEAN IsTSEGopNotificationSupport(VOID);

LOGO_TYPE GetLogoType(UINT8 *ImageData)
{
	if ( BMP_Logo == GetBmpLogoType(ImageData))
		return BMP_Logo;
	
	return Unsupported_Logo;
}

/**
    function to Clear all the logo buffers.

    @param nil

    @retval void

**/
VOID CleanUpLogo()
{
	if(NULL != gPostscreenwithlogo)
	{
		MemFreePointer( (VOID **)&gPostscreenwithlogo );
	}
	CleanUpExtendedLogoWrapper();
	InvalidateStatusInBgrtWrapper ();			//Setup key detected so clearing the status field
}

/**
    function to set the screen resolutions.

    @param ResX UINTN ResY

    @retval status

**/
EFI_STATUS SetScreenResolution(UINTN ResX, UINTN ResY)
{
    EFI_STATUS Status;

    UINT32  i;
    UINTN   SizeOfInfo;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION    *pModeInfo;

    if( gGOP == NULL )
        return EFI_UNSUPPORTED;

    for(i=0;i<gGOP->Mode->MaxMode;i++)
    {
        Status = gGOP->QueryMode(gGOP,i,&SizeOfInfo,&pModeInfo);
        if((EFI_SUCCESS == Status) && SizeOfInfo)
        {
            if((ResX == pModeInfo->HorizontalResolution) &&
               (ResY == pModeInfo->VerticalResolution))
            {
                //Set to this mode
                Status = gGOP->SetMode(gGOP,i);
                return Status;
            }
        }
    }

    return EFI_UNSUPPORTED;
}

/**
    Sets the mode which approximately fits the current image resolution

    @param UINTN *, UINTN * -> Pointer to the width and height of the image to be drawn

    @retval EFI_STATUS -> Status for the operation

**/
EFI_STATUS GOPSetScreenResolution (UINTN *Width, UINTN *Height)
{
	UINT32  i=0, Index = 0;
	UINTN 	DiffXY = 0, ResXY = 0;
	UINTN   SizeOfInfo;
	BOOLEAN ResXYSet = FALSE;
	EFI_STATUS	Status;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION    *pModeInfo;

	if( NULL == gGOP )
	  return EFI_UNSUPPORTED;//Validate the Gop before usage in all the possible cases and also get instance of Gop through notification
	SETUP_DEBUG_TSE("[TSE] GOPSetScreenResolution: Set to [%dx%d] \n", *Width, *Height );
	PrintGopAndConInDetails();
	//Find proper mode to fit image
	for (i=0;i<gGOP->Mode->MaxMode;i++)
	{
		Status = gGOP->QueryMode (gGOP,i,&SizeOfInfo,&pModeInfo);
		if ((EFI_SUCCESS == Status) && SizeOfInfo)
		{
			DiffXY = TSE_ABS(*Width , pModeInfo->HorizontalResolution) + TSE_ABS(*Height , pModeInfo->VerticalResolution);
			if ((ResXY || ResXYSet) ? (DiffXY <= ResXY) : (ResXY <= DiffXY))
			{
				Index = i;
				ResXY = DiffXY;
				ResXYSet = TRUE;
			}
		}
	}	
	gGOP->SetMode (gGOP, Index);
	Status = GOPSetScreenResolutionHook (Width, Height, Index);
	PrintGopAndConInDetails();
	SETUP_DEBUG_TSE("[TSE] GOPSetScreenResolution: [%dx%d] Mode =%d \n",*Width, *Height, Index );
	return Status;
}

/**
    function to get the screen resolutions.

    @param ResX UINTN *ResY

    @retval status

**/
EFI_STATUS GetScreenResolution(UINTN *ResX, UINTN *ResY)
{
    if( gGOP == NULL )
        return EFI_UNSUPPORTED;
    *ResX = gGOP->Mode->Info->HorizontalResolution;
    *ResY = gGOP->Mode->Info->VerticalResolution;
    return EFI_SUCCESS;
}

/**
    function to draw a image

    @param ImageData UINTN ImageSize, CO_ORD_ATTRIBUTE Attribute,
    @param CoordinateX INTN CoordinateY, BOOLEAN AdjustScreenResolution,
    @param Width UINTN *Height

    @retval status

**/
EFI_STATUS
DrawImage( 
    IN UINT8 *ImageData,
    IN UINTN ImageSize,
    IN CO_ORD_ATTRIBUTE Attribute,
    IN INTN CoordinateX,
    IN INTN CoordinateY,
    IN BOOLEAN AdjustScreenResolution,
    OUT UINTN *Width,
    OUT UINTN *Height
)
{

    EFI_STATUS	Status;
    UINTN		SizeOfX, SizeOfY;
    UINTN		UgaBltSize=0;
    UINTN		unBufferWidth=0;
    UINTN		LogoType;
    BOOLEAN		Animate = FALSE;

    EFI_UGA_PIXEL	*UgaBlt = NULL;

    *Height = *Width = 0;
    
    SETUP_DEBUG_TSE("[TSE] DrawImage: CoordinateX = %d  CoordinateY = %d  AdjustScreenResolution = %d\n",CoordinateX,CoordinateY,AdjustScreenResolution);
    UpdateGoPUgaDraw();
    Status = GetScreenResolution(&SizeOfX, &SizeOfY);
    SETUP_DEBUG_TSE("[TSE] Current Screen Resolution [%dx%d]  Status = %r\n",SizeOfX,SizeOfY, Status);
    if ( EFI_ERROR(Status) )
        return Status;

    LogoType = GetLogoType(ImageData);
    SETUP_DEBUG_TSE("[TSE] LogoType = %d\n",LogoType);
    switch(LogoType)
    {

	case BMP_Logo:
		Status = ConvertBmpToUgaBltWrapper(
                                ImageData,
                                ImageSize,
                                (void**)&UgaBlt,
                                &UgaBltSize,
                                Height,
                                Width );
            	unBufferWidth = *Width;
        	break;

        case Unsupported_Logo:
        	default:

		Status = ConvertAdvancedImageToUgaBlt(
                                ImageData,
                                ImageSize,
                               (void**) &UgaBlt,
                                &UgaBltSize,
                                Height,
                                Width,
				&Animate);
		unBufferWidth = *Width;


            break;
    }

    if( EFI_ERROR(Status) )
    {
		SETUP_DEBUG_TSE("[TSE] DrawImage: Conversion of image to Blt fails, exiting\n");
//        CleanUpLogo();    //If Jpeg off bmp on, jpeg image unsuccess clearing the drawn bmp image so commented.
        return Status;
    }
    SETUP_DEBUG_TSE("[TSE] DrawImage: [%dx%d] \n",*Width,*Height);
    if(AdjustScreenResolution)
    {
        //If the image is bigger than the current resolution
        //then try to set resolution to fit the image
        if((*Width > SizeOfX) || (*Height > SizeOfY))
        {
			Status = GOPSetScreenResolution (Width, Height);			//Always check to suite the best resolution
			GetScreenResolution (&SizeOfX,&SizeOfY);
		}
    }
    SETUP_DEBUG_TSE("[TSE] Current Screen Resolution: [%dx%d]\n",SizeOfX,SizeOfY);
     
    if(IsTSEGopNotificationSupport() && gPostStatus == TSE_POST_STATUS_IN_QUITE_BOOT_SCREEN)
    {// Save the buffer only in Quietboot screen.
    	SaveBltBuffer (UgaBlt, Attribute, *Width, *Height, CoordinateX, CoordinateY, unBufferWidth); // Save the logo into backup graphics blt buffer
    }
    DrawBltBuffer (UgaBlt, Attribute, *Width, *Height, CoordinateX, CoordinateY, unBufferWidth);
	if(Animate)
	{
		DoLogoAnimate(Attribute,CoordinateX,CoordinateY);
	}
	else
	{
		//MemFreePointer( (VOID **)&ImageData ); //image data will be freed in the calling function.
		MemFreePointer( (VOID **)&UgaBlt );
    }
	SETUP_DEBUG_TSE("[TSE] DrawImage: Done\n");
	return EFI_SUCCESS;
}

/**
    function to draw a progress bar

    @param nil

    @retval void

**/
VOID DrawBltProgressBar()
{
#ifndef STANDALONE_APPLICATION

	UINTN x = gProgress->x ;
    UINTN y = gProgress->y ;
	UINTN w = gProgress->w ;
    UINTN h = gProgress->h ;
	UINTN delta = gProgress->delta ;
	gProgress->completed+=delta ;

	if(gProgress->quiteBootActive && (gProgress->completed <= w))
	{
		DrawBlock(x, y, w, h, gProgress->backgroundColor) ;					//draw background
		DrawBlock(x, y, gProgress->completed, h, gProgress->fillColor) ;	//draw progression
		DrawProgressBarBorder(x, y, w, h, gProgress->borderColor, 2) ; 		//draw border
	}
#endif
}

/**
    function to draw the border of a progress bar

    @param DestX UINTN DestY, UINTN Width, UINTN Height,
    @param BltBuffer UINTN	bw

    @retval void

**/
VOID DrawProgressBarBorder(
    UINTN	x,
    UINTN	y,
	UINTN	w,
    UINTN	h,
	EFI_UGA_PIXEL *BltBuffer,
	UINTN	bw
    )
{
#ifndef STANDALONE_APPLICATION
 	//Top
	DrawBlock(x, y, w, bw, BltBuffer) ;
	//Bottom
	DrawBlock(x, y+h-bw, w, bw, BltBuffer) ;
	//Left
	DrawBlock(x, y, bw, h, BltBuffer) ;
	//Right
	DrawBlock(x+w-bw, y, bw, h, BltBuffer) ;
#endif
}

/**
    function to draw draw a block

    @param DestX UINTN DestY, UINTN	Width, UINTN Height, EFI_UGA_PIXEL *BltBuffer

    @retval void

**/
VOID DrawBlock(
    UINTN	x,
    UINTN	y,
	UINTN	w,
    UINTN	h,
	EFI_UGA_PIXEL *BltBuffer
    )
{
#ifndef STANDALONE_APPLICATION
    if ( gQuietBoot && gGOP)
	{
    gGOP->Blt (
        gGOP,
        BltBuffer,
        EfiBltVideoFill,
        0, 0,
        x, y,
        w, h,
        0 /*BufferWidth * sizeof (EFI_UGA_PIXEL)*/
        );
	}
#endif
}

/**
    function to draw draw a block

    @param UgaBlt CO_ORD_ATTRIBUTE Attribute,
    @param Width UINTN Height, INTN DestX, INTN DestY,
    @param BufferWidth 

    @retval void

**/
VOID DrawBltBuffer(
    EFI_UGA_PIXEL *UgaBlt,
    CO_ORD_ATTRIBUTE Attribute,
    UINTN   Width,
    UINTN   Height,
    INTN    DestX,
    INTN    DestY,
    UINTN   BufferWidth
    )
{
    UINTN SizeOfX=0, SizeOfY=0;
    UINTN SourceX=0, SourceY=0;

    GetScreenResolution(&SizeOfX,&SizeOfY);
    // In case of gif set the frame position
    if( (GifImageFlag)&& (FALSE == gDrawQuietBootMessage) && (Attribute == CA_AttributeCenter)){
    	SourceX = gGifSrcX; 
    	SourceY = gGifSrcY;
    }
    switch(Attribute)
    {
        case CA_AttributeLeftTop:
            //Need to alter DestX and DestY based on the image descriptor.
            if( (GifImageFlag)&& (FALSE == gDrawQuietBootMessage)){
            	DestX = SourceX;
            	DestY = SourceY;
            }
            //No need to alter DestX and DestY
        break;

        case CA_AttributeCenterTop:
            DestX = ((INTN)(SizeOfX - Width)) / 2;
            //DestY need not be altered
        break;

        case CA_AttributeRightTop:
            DestX = (INTN)(SizeOfX - Width - DestX);
            //DestY need not be altered
        break;

        case CA_AttributeRightCenter:
            DestX = (INTN)(SizeOfX - Width - DestX);
            DestY = ((INTN)(SizeOfY - Height))/2;
        break;

        case CA_AttributeRightBottom:
            DestX = (INTN)(SizeOfX - Width - DestX);
            DestY = (INTN)(SizeOfY - Height - DestY);
        break;

        case CA_AttributeCenterBottom:
            DestX = ((INTN)(SizeOfX - Width))/2;
            DestY = (INTN)(SizeOfY - Height - DestY);
        break;

        case CA_AttributeLeftBottom:
            //DestX need not be altered
            DestY = (INTN)(SizeOfY - Height - DestY);
        break;

        case CA_AttributeLeftCenter:
            //DestX need not be altered
            DestY = ((INTN)(SizeOfY - Height))/2;
        break;

        case CA_AttributeCenter:
            // in case of gif, set the video display positions based on the image frame position.
            if( (GifImageFlag)&& (FALSE == gDrawQuietBootMessage)){
	           DestX = ((INTN)(SizeOfX - Width))/2 + SourceX;
               DestY = ((INTN)(SizeOfY - Height))/2 + SourceY;
            } 
				// Logo should be 40% of sreen dimension
				else if (IsBGRTSupported() && ( Width <= (((UINTN)(40 *SizeOfX))/100) ) && ( Height <= (((UINTN)(40 *SizeOfY))/100) ))
				{
					DestX = ((INTN)(SizeOfX - Width))/2; //Exact center of x-axis
               DestY = (((INTN)(SizeOfY*382))/1000) - ((INTN)Height/2);//Center of logo is 38.2% from the top of screen
				}
				else 
				{
	         	DestX = ((INTN)(SizeOfX - Width))/2;
               DestY = ((INTN)(SizeOfY - Height))/2;
            }

        break;

        default:
            //DestX and DestY remains as it is
        break;
	}

    //Fill the video based on image frame descriptor values
    
    if( DestX < 0 )
    {
        if(Width > SizeOfX)
           SourceX = (Width - SizeOfX)/2;
        DestX = 0;
    }

    if( DestY < 0 )
    {
        if(Height > SizeOfY)
            SourceY = (Height - SizeOfY)/2;
        DestY = 0;
    }
    if( (GifImageFlag)&& (FALSE == gDrawQuietBootMessage)&& (Attribute == CA_AttributeCenter)){
            Width = gGifDestX;
            Height = gGifDestY;
        }

    // Fix to show Logo images correctly when it is bigger then screen resolution.
    if((DestX + Width ) > SizeOfX)
    {
        if(DestX > (INTN)SizeOfX)
            Width = 0;
        else
        Width = (SizeOfX - DestX);
    }
	
    if((DestY + Height) > SizeOfY)
    {
        if(DestY > (INTN)SizeOfY)
            Height = 0;
        else
        Height = (SizeOfY - DestY);
    }   
	    SETUP_DEBUG_TSE ("[TSE] BLT: From: (%d, %d) To: (%d, %d) [%d x %d] gGifSrcX :%d gGifSrcY :%d gGifDestX: %d gGifDestX :%d\n",SourceX, SourceY, DestX, DestY, Width, Height);

    if ( gGOP && Width && Height)
	    gGOP->Blt (
	        gGOP,
	        UgaBlt,
	        EfiBltBufferToVideo,
	        SourceX, SourceY,
	        (UINTN)DestX, (UINTN)DestY,
	        Width, Height,
	        BufferWidth * sizeof (EFI_UGA_PIXEL)
	        );
	if (gAddBgrtResolutions)
	{
		SetGetBgrtCoordinates ((UINTN*)&DestX, (UINTN*)&DestY, &Width, &Height, TRUE);	//1 for set the value, Cast for GCC Build 
	}
}

/**
    Converts the Blt in the screen to Bmp buffer

    @param VOID

    @retval VOID *

**/
VOID *ConvertBlt2Bmp (VOID)
{
    BMP_IMAGE_HEADER BmpHeaderStruct = {
		'B',	//  CHAR8         CharB;
		'M',	//  CHAR8         CharM;
		0,		//  UINT32        Size; // Dynamic
		{0,0},	//  UINT16        Reserved[2];
		0x36,	//  UINT32        ImageOffset;
		0x28,	//  UINT32        HeaderSize;
		0,		//  UINT32        PixelWidth;	// Dynamic
		0,		//  UINT32        PixelHeight;	// Dynamic
		1,		//  UINT16        Planes;       // Must be 1
		0x18,	//  UINT16        BitPerPixel;  // 1, 4, 8, or 24
		0,		//  UINT32        CompressionType;
		0,		//  UINT32        ImageSize;    // // Dynamic
		0,		//  UINT32        XPixelsPerMeter;
		0,		//  UINT32        YPixelsPerMeter;
		0,		//  UINT32        NumberOfColors;
		0,		//  UINT32        ImportantColors;

    };
    BMP_IMAGE_HEADER  *BmpBuffer=NULL;
	UINT32 	i=0,j=0;
    UINT8   *Image;
    UINTN   ImageIndex;
    UINTN   AllignWidth = 0;
	UINTN 	SourceX = 0, SourceY = 0;
	UINTN 	Width = 0, Height = 0;
	EFI_STATUS 		Status = EFI_SUCCESS;
	EFI_UGA_PIXEL 	*BltBuffer;

	SetGetBgrtCoordinates (&SourceX, &SourceY, &Width, &Height, FALSE);		//Get the BGRT logo resolutions
	if(gGOP)
	{
		BltBuffer = EfiLibAllocateZeroPool (Width * Height * sizeof (EFI_UGA_PIXEL));
		if (NULL == BltBuffer)
		{
			return NULL;
		}
	    Status = gGOP->Blt (
	    	gGOP,
	     	BltBuffer,
	      	EfiBltVideoToBltBuffer,
	       	SourceX, SourceY,
	        0, 0,
	        Width, Height,
	        0);
		if (EFI_SUCCESS != Status)
		{
			MemFreePointer ((VOID **)&BltBuffer);
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
	AllignWidth = Width;
    if (Width%4 != 0)
    {
        AllignWidth += Width%4;					//Width should be 4 bytes alligned
    }
	BmpHeaderStruct.ImageSize = (UINT32)(3*AllignWidth*Height);
	BmpHeaderStruct.Size = sizeof (BmpHeaderStruct)+ BmpHeaderStruct.ImageSize;
	BmpBuffer = EfiLibAllocateZeroPool (BmpHeaderStruct.Size);
	if (BmpBuffer)
	{
		BmpHeaderStruct.PixelWidth = (UINT32)Width;
		BmpHeaderStruct.PixelHeight = (UINT32)Height;

		gBS->CopyMem(BmpBuffer,&BmpHeaderStruct,sizeof(BmpHeaderStruct));
		Image = (UINT8*)BmpBuffer;
		Image += BmpBuffer->ImageOffset;

		for(i=(UINT32)(Width*Height);i;i-=BmpHeaderStruct.PixelWidth)
		{
			for(j=BmpHeaderStruct.PixelWidth;j;j--)
			{
				*Image++ = BltBuffer[i-j].Blue;
				*Image++ = BltBuffer[i-j].Green;
				*Image++ = BltBuffer[i-j].Red;
			}
            ImageIndex = (UINTN) (Image - BmpBuffer->ImageOffset);
            if ((ImageIndex % 4) != 0) // Bmp Image starts each row on a 32-bit boundary!
                Image = Image + (4 - (ImageIndex % 4));
		}
		MemFreePointer ((VOID **)&BltBuffer);
        return (VOID*)BmpBuffer;
	}
	MemFreePointer ((VOID **)&BltBuffer);
	return NULL;
}

/**
    Converts Blt to Bmp buffer and form BGRT table and add to ACPI.

    @param BOOLEAN  = Indicates whether to set the BGRT status field or not.

    @retval VOID

**/
VOID ConvertBmpandAddBGRT (
	BOOLEAN GifImagePresence
    )
{
    VOID *BmpData = NULL;
    BmpData = ConvertBlt2Bmp ();
    if (BmpData)
    {
		UINTN	DestX = 0, DestY = 0, Width = 0, Height = 0;
		SetGetBgrtCoordinates (&DestX, &DestY, &Width, &Height, FALSE); //Get the Bgrt logo resolution
   	    AddImageDetailToACPI (BmpData, DestX, DestY, GifImagePresence);
	}
}

/**
    Sets the BGRT logo coordinates

    @param DestX = X coordinate
    @param DestY = Y coordinate
    @param Width = Width of the logo
    @param Height = Heigth of the logo
    @param Command = To set or get the Bgrt logo resolutions

    @retval VOID

**/
VOID SetGetBgrtCoordinates (
	UINTN	*DestX,
	UINTN	*DestY,
	UINTN   *Width,
	UINTN   *Height,
	BOOLEAN Command
	)
{
	static UINTN gBgrtStartResX = 0;
	static UINTN gBgrtStartResY = 0;
	static UINTN gBgrtEndResX = 0;
	static UINTN gBgrtEndResY = 0;

	if (FALSE == Command)			//To get the Bgrt logo resolution
	{
		*DestX = gBgrtStartResX;
		*DestY = gBgrtStartResY;
		*Width = gBgrtEndResX - gBgrtStartResX;
		*Height = gBgrtEndResY - gBgrtStartResY;
	}
	else
	{
		if (1 == gAddBgrtResolutions)			//At first time initialize the values
		{
			gBgrtStartResX = *DestX;
			gBgrtStartResY = *DestY;
			gBgrtEndResX = *DestX + *Width;
			gBgrtEndResY = *DestY + *Height;
		}
		else
		{
			if (*DestX < gBgrtStartResX)
			{
				gBgrtStartResX = *DestX;
			}
			if (*DestY < gBgrtStartResY)
			{
				gBgrtStartResY = *DestY;
			}
			if (gBgrtEndResX < (*DestX + *Width))
			{
				gBgrtEndResX = (*DestX + *Width);
			}
			if (gBgrtEndResY < (*DestY + *Height))
			{
				gBgrtEndResY = (*DestY + *Height);
			}
		}
		gAddBgrtResolutions ++;
	}
}

/**
    Sets the graphical resolution corresponding to the input parameter

    @param UINT32 - Horizontal resolution of the mode to set
        UINT32 - Vertical resolution of the mode to set

    @retval EFI_STATUS EFI_SUCCESS - If input resolutions are set successfully
        !EFI_SUCCESS - If input resolutions are not set successfully

**/
EFI_STATUS SetGraphicsResolution (UINT32 HorResolution, UINT32 VerResolution)
{
	EFI_STATUS	Status = EFI_SUCCESS;
	UINT32  		iIndex = 0;
	UINTN   		SizeOfInfo = 0;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *pModeInfo = NULL;

    SETUP_DEBUG_TSE ("[TSE] SetGraphicsResolution: Set to [%dx%d]\n", HorResolution, VerResolution);

	for (iIndex = 0; iIndex < gGOP->Mode->MaxMode; iIndex ++)
	{
		Status = gGOP->QueryMode (gGOP, iIndex, &SizeOfInfo, &pModeInfo);
		if (!EFI_ERROR (Status))
		{
		    SETUP_DEBUG_TSE ("[TSE] Mode =%d:[%dx%d]\n", iIndex, pModeInfo->HorizontalResolution,pModeInfo->VerticalResolution);
		    if ((HorResolution == pModeInfo->HorizontalResolution) && (VerResolution == pModeInfo->VerticalResolution))
			{
				if (gGOP->Mode->Mode != iIndex)			//If Current mode not equals the found mode then call SetMode otherwise return success
				{
					SETUP_DEBUG_TSE ("[TSE] Setting Mode %d\n", iIndex);
					Status = gGOP->SetMode (gGOP, iIndex);
					if (!EFI_ERROR (Status))				//Loop for all the cases. Not success also loop for another without returning from here
					{
						return EFI_SUCCESS;
					}
				}
				else
				{
					SETUP_DEBUG_TSE ("[TSE] Mode %d is already set\n", iIndex);
					return EFI_SUCCESS;
				}
			}
		}
	}
    SETUP_DEBUG_TSE ("[TSE] SetGraphicsResolution: Mode not found\n");
	return EFI_NOT_FOUND;
}

EFI_STATUS SetMaxGopResolution()
{
    EFI_STATUS  Status = EFI_SUCCESS;
    UINT32      iIndex = 0, Index = 0;
    UINTN       SizeOfInfo = 0;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *pModeInfo = NULL;
    UINTN MaxHorRes = 0;
    UINTN MaxVertRes = 0;
    
    for (iIndex = 0; iIndex < gGOP->Mode->MaxMode; iIndex++)
    {
        Status = gGOP->QueryMode (gGOP, iIndex, &SizeOfInfo, &pModeInfo);
        SETUP_DEBUG_TSE ("[TSE] SetMaxGopResolution Mode =%d:[%dx%d]\n", iIndex, pModeInfo->HorizontalResolution,pModeInfo->VerticalResolution);
        if (!EFI_ERROR (Status) && pModeInfo)
        {
            if(pModeInfo->HorizontalResolution > MaxHorRes ||(pModeInfo->HorizontalResolution == MaxHorRes && pModeInfo->VerticalResolution > MaxVertRes))
            {
                Index = iIndex;
                MaxHorRes = pModeInfo->HorizontalResolution;
                MaxVertRes = pModeInfo->VerticalResolution;
            }
        }
    }
    if (gGOP->Mode->Mode != Index)
    {
        SETUP_DEBUG_TSE ("[TSE] SetMaxMode =%d:[%dx%d]\n", Index, MaxHorRes, MaxVertRes);
        Status = gGOP->SetMode (gGOP, Index);
    }
    return Status;
}

/**
    Finds and sets the resolution from EDID

    @param VOID

    @retval VOID

**/
EFI_STATUS SetNativeResFromEdid (VOID)
{
	EFI_STATUS 	Status;
	UINT32 			HorRes = 0xFFFFFFFF;
	UINT32 			VerRes = 0xFFFFFFFF;
	EFI_EDID_ACTIVE_PROTOCOL *EdidActive;
	EFI_GUID EdidActiveProtocolGuid = EFI_EDID_ACTIVE_PROTOCOL_GUID;
    EFI_GUID EfiEdidDiscoveredProtocolGuid = EFI_EDID_DISCOVERED_PROTOCOL_GUID;
    EFI_HANDLE  *HandleBuffer;
    UINTN       HandleCount;
    UINTN       Index;
    EFI_EDID_DISCOVERED_PROTOCOL    *EdidDiscovered;
    
	SETUP_DEBUG_TSE ("[TSE] SetNativeResFromEdid:\n");
	Status = gBS->LocateProtocol (&EdidActiveProtocolGuid, NULL, (void**)&EdidActive);
	if(EFI_ERROR(Status) || EdidActive->SizeOfEdid == 0)
	{
	    Status = gBS->LocateHandleBuffer(
	                    ByProtocol,
	                    &EfiEdidDiscoveredProtocolGuid,
	                    NULL,
	                    &HandleCount,
	                    &HandleBuffer
	                    );
	    if (!(EFI_ERROR(Status))) {
	        SETUP_DEBUG_TSE ("[TSE] Found EfiEdidDiscoveredProtocol Count %x\n", HandleCount);
	        for (Index = 0; Index < HandleCount; Index++) {
	            Status = gBS->HandleProtocol(
	                    HandleBuffer[Index],
	                    &EfiEdidDiscoveredProtocolGuid,
	                    (VOID **)&EdidDiscovered);
	            if (EFI_ERROR(Status)) {
	                continue;
	            }
	            if (EdidDiscovered->SizeOfEdid == 0) {
	                continue;
	            }
	            if (!((EdidDiscovered->Edid)[24] & BIT01)) {
	                /* native resolution not supported */
	                continue;
	            }
                SETUP_DEBUG_TSE ("[TSE] EdidDiscovered[%d] HorRes %d VerRes %d\n",
                        Index,
                        (((((EdidDiscovered->Edid)[0x36 + 4]) & 0xF0) << 4) + (EdidDiscovered->Edid)[0x36 + 2]),
                        (((((EdidDiscovered->Edid)[0x36 + 7]) & 0xF0) << 4) + (EdidDiscovered->Edid)[0x36 + 5]));
	            if (HorRes > (UINT32)(((((EdidDiscovered->Edid)[0x36 + 4]) & 0xF0) << 4) + (EdidDiscovered->Edid)[0x36 + 2])) {
	                HorRes = ((((EdidDiscovered->Edid)[0x36 + 4]) & 0xF0) << 4) + (EdidDiscovered->Edid)[0x36 + 2];
	                VerRes = ((((EdidDiscovered->Edid)[0x36 + 7]) & 0xF0) << 4) + (EdidDiscovered->Edid)[0x36 + 5];
	            }
	        }
	        if (HandleBuffer != NULL) {
	            gBS->FreePool(HandleBuffer);
	        }
	    }
		if ((HorRes == 0xFFFFFFFF) || (VerRes == 0xFFFFFFFF)) {
		    SETUP_DEBUG_TSE ("[TSE] Edid active and discovered protocol fails, fall back to [1024x768] \n");
		    Status = SetGraphicsResolution (1024, 768);			//If EDID not found set 1024*768 mode
		    return Status;
		}
	} else {
	    if(!((EdidActive->Edid)[24] & BIT01))
	    {
	    /* native resolution not supported */
	        SETUP_DEBUG_TSE ("[TSE] Native resolution not supported, exiting\n");
	        return EFI_UNSUPPORTED;
	    }
	    HorRes = ((((EdidActive->Edid)[0x36 + 4]) & 0xF0) << 4) + (EdidActive->Edid)[0x36 + 2];
	    VerRes = ((((EdidActive->Edid)[0x36 + 7]) & 0xF0) << 4) + (EdidActive->Edid)[0x36 + 5];
	}
/*
	SETUP_DEBUG_TSE ("\n[TSE] %02d, ", EdidActive->Edid[0x36 + 4]);
	SETUP_DEBUG_TSE ("\n[TSE] %02d, ", EdidActive->Edid[0x36 + 2]);
	SETUP_DEBUG_TSE ("\n[TSE] %02d, ", EdidActive->Edid[0x36 + 7]);
	SETUP_DEBUG_TSE ("\n[TSE] %02d, ", EdidActive->Edid[0x36 + 5]);
*/	SETUP_DEBUG_TSE ("[TSE] EDID HorRes=%d VerRes=%d\n", HorRes, VerRes);

	Status = SetGraphicsResolution (HorRes, VerRes);
	if (EFI_ERROR(Status))
	{
		SETUP_DEBUG_TSE ("[TSE] Setting [1024x768] Mode \n");
		Status = SetGraphicsResolution (1024, 768);			//If EDID not found set 1024*768 mode
#if OVERRIDE_MainSetupLoopHook
		if (EFI_ERROR(Status)) {
			SETUP_DEBUG_TSE ("[TSE] Setting [800x600] Mode \n");
			Status = SetGraphicsResolution (800, 600);		//If setting to 1024x768 fails
		}
#endif
	}
	SETUP_DEBUG_TSE ("[TSE] SetNativeResFromEdid: Exiting\n");
	return Status;
}

/**
    Save the given blt buffer into gPostscreenwithlogo which will be used later in RestoreGraphicsScreen()
**/

EFI_STATUS 
UpdateTseGraphicsBltBuffer (  
  IN  EFI_UGA_PIXEL           	*BltBuffer,
  IN  UINTN                     SourceX,
  IN  UINTN                     SourceY,
  IN  UINTN                     DestinationX,
  IN  UINTN                     DestinationY,
  IN  UINTN                     Width,
  IN  UINTN                     Height,
  IN  UINTN                     Delta  
  )
{
    EFI_UGA_PIXEL 		*CurrSourceBuffer = NULL;
    EFI_UGA_PIXEL		*CurrDestinBuffer = NULL;
    UINTN               DstY;
    UINTN               SrcY;
    UINTN 				Index;
    EFI_STATUS Status = EFI_UNSUPPORTED;
    
   	Status = GetScreenResolution(&gGraphicsHorizontalResolution, &gGraphicsVerticalResolution);
   	if(EFI_ERROR(Status))
   		return Status;
    SETUP_DEBUG_TSE("[TSE] UpdateTseGraphicsBltBuffer : Current Screen Resolution - Horizontal Resolution : %d  Vertical Resolution : %d\n",gGraphicsHorizontalResolution,gGraphicsVerticalResolution);

    if (DestinationY + Height > gGraphicsVerticalResolution) {
    	SETUP_DEBUG_TSE("UpdateTseGraphicsBltBuffer : Destination height + Image height > Graphics VeriticalResolution. So not saving the image into blt buffer\n");
      return EFI_INVALID_PARAMETER;
    }

    if (DestinationX + Width > gGraphicsHorizontalResolution) {
    	SETUP_DEBUG_TSE("UpdateTseGraphicsBltBuffer : Destination width + Image width > Graphics HorizontalResolution. So not saving the image into blt buffer\n");
      return EFI_INVALID_PARAMETER;
    }
    
    if(gPostscreenwithlogo == NULL)
	{
    	gPostscreenwithlogo = (EFI_UGA_PIXEL *) EfiLibAllocateZeroPool(gGraphicsVerticalResolution * gGraphicsHorizontalResolution * sizeof (EFI_UGA_PIXEL));
	}

    //
    // If Delta is zero, then the entire BltBuffer is being used, so Delta
    // is the number of bytes in each row of BltBuffer.  Since BltBuffer is Width pixels size,
    // the number of bytes in each row can be computed.
    //
    if (Delta == 0) {
      Delta = Width * sizeof (EFI_UGA_PIXEL);
    }
	SETUP_DEBUG_TSE("UpdateTseGraphicsBltBuffer : Destination Height = %d  Destination Width = %d  Image Height = %d Image Width = %d\n",DestinationY,DestinationX,Height,Width);

	//Update the buffer
    for (Index = 0; Index < Height; Index++) 
   	{    
        SrcY  = SourceY + Index;
        DstY  = DestinationY +Index;
        
        CurrDestinBuffer = (EFI_UGA_PIXEL*)((UINT8*)gPostscreenwithlogo + ((DstY * gGraphicsHorizontalResolution + DestinationX) * sizeof (EFI_UGA_PIXEL)));
        
        CurrSourceBuffer = (EFI_UGA_PIXEL *) ((UINT8 *) BltBuffer + (SrcY * Delta) + SourceX * sizeof (EFI_UGA_PIXEL));
        gBS->CopyMem (CurrDestinBuffer, CurrSourceBuffer, Width * sizeof (EFI_UGA_PIXEL));
   	}
    SETUP_DEBUG_TSE("UpdateTseGraphicsBltBuffer : Image saved into gPostscreenwithlogo successfully\n");
    return EFI_SUCCESS;
}

/**
    Save the given blt buffer into gPostscreenwithlogo which will be used later in RestoreGraphicsScreen()
**/
EFI_STATUS SaveBltBuffer(
    EFI_UGA_PIXEL *UgaBlt,
    CO_ORD_ATTRIBUTE Attribute,
    UINTN   Width,
    UINTN   Height,
    INTN    DestX,
    INTN    DestY,
    UINTN   BufferWidth
    )
{
    UINTN SizeOfX=0, SizeOfY=0;
    UINTN SourceX=0, SourceY=0;
    EFI_STATUS Status = EFI_UNSUPPORTED;

    Status = GetScreenResolution(&SizeOfX,&SizeOfY);
    if(EFI_ERROR(Status))
    	return Status;
   
    // In case of gif set the frame position
    if(GifImageFlag) {
    	SourceX = gGifSrcX; 
    	SourceY = gGifSrcY;
    }
    switch(Attribute)
    {
        case CA_AttributeLeftTop:
            //Need to alter DestX and DestY based on the image descriptor.
            if(GifImageFlag) {
            	DestX = SourceX;
            	DestY = SourceY;
            }
            //No need to alter DestX and DestY
        break;

        case CA_AttributeCenterTop:
            DestX = ((INTN)(SizeOfX - Width)) / 2;
            //DestY need not be altered
        break;

        case CA_AttributeRightTop:
            DestX = (INTN)(SizeOfX - Width - DestX);
            //DestY need not be altered
        break;

        case CA_AttributeRightCenter:
            DestX = (INTN)(SizeOfX - Width - DestX);
            DestY = ((INTN)(SizeOfY - Height))/2;
        break;

        case CA_AttributeRightBottom:
            DestX = (INTN)(SizeOfX - Width - DestX);
            DestY = (INTN)(SizeOfY - Height - DestY);
        break;

        case CA_AttributeCenterBottom:
            DestX = ((INTN)(SizeOfX - Width))/2;
            DestY = (INTN)(SizeOfY - Height - DestY);
        break;

        case CA_AttributeLeftBottom:
            //DestX need not be altered
            DestY = (INTN)(SizeOfY - Height - DestY);
        break;

        case CA_AttributeLeftCenter:
            //DestX need not be altered
            DestY = ((INTN)(SizeOfY - Height))/2;
        break;

        case CA_AttributeCenter:
            // in case of gif, set the video display positions based on the image frame position.
            if(GifImageFlag) {
	           DestX = ((INTN)(SizeOfX - Width))/2 + SourceX;
               DestY = ((INTN)(SizeOfY - Height))/2 + SourceY;
            } 
				// Logo should be 40% of sreen dimension
				else if (IsBGRTSupported() && ( Width <= (((UINTN)(40 *SizeOfX))/100) ) && ( Height <= (((UINTN)(40 *SizeOfY))/100) ))
				{
					DestX = ((INTN)(SizeOfX - Width))/2; //Exact center of x-axis
               DestY = (((INTN)(SizeOfY*382))/1000) - ((INTN)Height/2);//Center of logo is 38.2% from the top of screen
				}
				else 
				{
	         	DestX = ((INTN)(SizeOfX - Width))/2;
               DestY = ((INTN)(SizeOfY - Height))/2;
            }

        break;

        default:
            //DestX and DestY remains as it is
        break;
	}

    //Fill the video based on image frame descriptor values
    if( (GifImageFlag)&& (FALSE == gDrawQuietBootMessage)){
        Width = gGifDestX;
        Height = gGifDestY;
    }

    if( DestX < 0 )
    {
        SourceX = (-(DestX));
        DestX = 0;
    }

    if( DestY < 0 )
    {
        SourceY = (-(DestY));
        DestY = 0;
    }

    // Fix to show Logo images correctly when it is bigger then screen resolution.
    if((DestX + (Width - SourceX)) > SizeOfX)
        Width = (SizeOfX - DestX);

    if((DestY + (Height - SourceY)) > SizeOfY)
        Height = (SizeOfY - DestY);

    UpdateTseGraphicsBltBuffer(UgaBlt,SourceX,SourceY,DestX,DestY,Width,Height, BufferWidth * sizeof (EFI_UGA_PIXEL));
   
    return EFI_SUCCESS;
}
