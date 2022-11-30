//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2013, American Megatrends, Inc.        **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
// $Archive: /Alaska/SOURCE/Modules/AMITSE2_0/AMITSE/BootOnly/notify.c $
//
// $Author: Arunsb $
//
// $Revision: 32 $
//
// $Date: 9/26/12 7:21a $
//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//

/** @file notify.c
    file contains code to support the notification functions

**/

#include "minisetup.h"

EFI_GUID        gEfiDXEMouseDriverProtocolGuid  = 
                            EFI_MOUSE_DRIVER_PROTOCOL_GUID;
#include "TseDrvHealth.h"       //Has TSE related driver health structures

#define	NOTIFY_MASK_UGA				(0x00000001)
#define	NOTIFY_MASK_CONSOLE_CONTROL	(0x00000002)
#define	NOTIFY_MASK_CONSOLE_IN		(0x00000004)
#define	NOTIFY_MASK_CONSOLE_OUT		(0x00000008)
#define	NOTIFY_MASK_MOUSE_DRIVER	(0x00000010)
BOOLEAN  IsDriverHealthSupported (VOID);
////////////////////////////////////////////////////////////
/// VARIABLE DECLARATIONS
///////////////////////////////////////////////////////////
typedef struct _NOTIFICATION_INFO
{
	EFI_HANDLE	NotifyHandle;
	EFI_EVENT	NotifyEvent;
	UINT32		NotifyMask;
	EFI_GUID	*NotifyGuid;
	BOOLEAN		NotifyDevicePath;
	VOID		*NotifyRegistration;
	VOID		*NotifyProtocol;
	VOID		**pgProtocol;
}
NOTIFICATION_INFO;

#ifndef STANDALONE_APPLICATION
static UINT32	gGlobalNotify = 0;
BOOLEAN	gQuietBoot = FALSE;
extern UINTN gPSMaxRows,gPSMaxCols;
 

extern SCREEN_BUFFER	*gFlushBuffer;
extern BOOLEAN ActivateInputDone;
VOID ActivateInput(VOID);
extern BOOLEAN IsSetupUseGopSplitter(void);
//EFI_GUID gAmiGraphicsSplitterModeChangeProtocolGuid = { 0xc9a2603a, 0xd5d6, 0x4b64, { 0x83, 0x2b, 0x1a, 0xc, 0xb2, 0x41, 0x96, 0x69 } };
extern EFI_GUID gAmiGraphicsSplitterModeChangeProtocolGuid;

static NOTIFICATION_INFO _gUgaNotify = 
{
	NULL,                             //EFI_HANDLE  NotifyHandle;
	NULL,                             //EFI_EVENT   NotifyEvent;
	NOTIFY_MASK_UGA,                  //UINT32      NotifyMask;
	&gEfiGraphicsOutputProtocolGuid,  //EFI_GUID    *NotifyGuid;
	TRUE,                             //BOOLEAN     NotifyDevicePath;
	NULL,                             //VOID        *NotifyRegistration;
	NULL,                             //VOID        *NotifyProtocol;
	(VOID**)&gGOP                     //VOID        **pgProtocol;
};

#if !TSE_USE_EDK_LIBRARY
static NOTIFICATION_INFO _gConsoleControlNotify = 
{
	NULL,                             //EFI_HANDLE  NotifyHandle;
	NULL,                             //EFI_EVENT   NotifyEvent;
	NOTIFY_MASK_CONSOLE_CONTROL,      //UINT32      NotifyMask;
	&gEfiConsoleControlProtocolGuid,  //EFI_GUID    *NotifyGuid;
	FALSE,                            //BOOLEAN     NotifyDevicePath;
	NULL,                             //VOID        *NotifyRegistration;
	NULL,                             //VOID        *NotifyProtocol;
	NULL //(VOID**)&gConsoleControl   //VOID        **pgProtocol;
			//gConsoleControl is used by PostManagerHAndshake to know if ActivateApplication has been called
			//if we allow Notification function to set it, when PostManager handshake need to call ActivateApplication as in case of
			//headless system, the call may be missed.
};

static NOTIFICATION_INFO _gConInNotify =
{
	NULL,                           //EFI_HANDLE  NotifyHandle;
	NULL,                           //EFI_EVENT   NotifyEvent;
	NOTIFY_MASK_CONSOLE_IN,         //UINT32      NotifyMask;
	&gEfiSimpleTextInProtocolGuid,  //EFI_GUID    *NotifyGuid;
	FALSE,                          //BOOLEAN     NotifyDevicePath;
	NULL,                           //VOID        *NotifyRegistration;
	NULL,                           //VOID        *NotifyProtocol;
	NULL //(VOID**)&gConIn          //VOID        **pgProtocol;
};
#endif
static NOTIFICATION_INFO _gConOutNotify =
{
	NULL,                            //EFI_HANDLE  NotifyHandle;
	NULL,                            //EFI_EVENT   NotifyEvent;
	NOTIFY_MASK_CONSOLE_OUT,         //UINT32      NotifyMask;
	&gEfiSimpleTextOutProtocolGuid,  //EFI_GUID    *NotifyGuid;
	FALSE,                           //BOOLEAN     NotifyDevicePath;
	NULL,                            //VOID        *NotifyRegistration;
	NULL,                            //VOID        *NotifyProtocol;
	NULL //(VOID**)&gConOut          //VOID        **pgProtocol;
};

static NOTIFICATION_INFO _gMousedriverNotify =
{
	NULL,                            //EFI_HANDLE  NotifyHandle;
	NULL,                            //EFI_EVENT   NotifyEvent;
	NOTIFY_MASK_MOUSE_DRIVER,         //UINT32      NotifyMask;
	&gEfiDXEMouseDriverProtocolGuid,  //EFI_GUID    *NotifyGuid;
	FALSE,                           //BOOLEAN     NotifyDevicePath;
	NULL,                            //VOID        *NotifyRegistration;
	NULL,                            //VOID        *NotifyProtocol;
	NULL 				          //VOID        **pgProtocol;
};


static NOTIFICATION_INFO *_gNotifyList[] =
{
	&_gUgaNotify,
#if !TSE_USE_EDK_LIBRARY
	&_gConsoleControlNotify,
	&_gConInNotify,
#endif
	&_gConOutNotify,
	&_gMousedriverNotify,
	NULL
};

#if APTIO_4_00 
EFI_STATUS FvReadResources (
  IN EFI_GUID                       *NameGuid,
  IN EFI_GUID                       *pGuid, 
  IN OUT VOID                       **Buffer,
  IN OUT UINTN                      *BufferSize
  );

// Signon Section Guide
// {2EBE0275-6458-4af9-91ED-D3F4EDB100AA}
#define SGN_ON_SECTION_GUID    {0x2ebe0275, 0x6458, 0x4af9, {0x91, 0xed, 0xd3, 0xf4, 0xed, 0xb1, 0x0, 0xaa}}

// Signon file guid
// {A59A0056-3341-44b5-9C9C-6D76F7673817}
// {0xa59a0056, 0x3341, 0x44b5, 0x9c, 0x9c, 0x6d, 0x76, 0xf7, 0x67, 0x38, 0x17};
#define SGN_ON_FILE_GUID    {0xa59a0056, 0x3341, 0x44b5, {0x9c, 0x9c, 0x6d, 0x76, 0xf7, 0x67, 0x38, 0x17}}

typedef struct _SIGNON
{
    UINT8 Sig[5];
    UINT8 Ver;
    UINT16 Reserved;
    CHAR8  SgnMsg1[]; 
}AMI_SIGN_ON;

#endif
////////////////////////////////////////////////////////////
/// EXTERN VARIABLES
///////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// FUNCTIONS DECLARATIONS
///////////////////////////////////////////////////////////
VOID 		FindandSetHotKeys (VOID);
UINT8   	IsSetupHideBiosSignOnMsg (void);
VOID 		GetTseBuildVersion(UINTN *TseMajor, UINTN *TseMinor, UINTN *TseBuild);
VOID 		GetTseBuildYear(UINT16 *TseBuildYear);
VOID 		SetDriverHealthCount (VOID);
BOOLEAN 	IsMouseSupported(VOID);
BOOLEAN 	IsSoftKbdSupported(VOID);
BOOLEAN	TseIgnoreKeyForFastBoot(); 

VOID *SavePostScreen( UINTN *SizeOfX, UINTN *SizeOfY );
VOID RestorePostScreen( VOID *UgaBlt, UINTN SizeOfX, UINTN SizeOfY );
VOID gAppClearScreen();

/**
    Notification function for driver health protocol instal.

    @param EFI_EVENT , VOID *

    @retval VOID

**/
VOID EFIAPI _DrvHealthNotifyFunction (EFI_EVENT Event, VOID *Context)
{
	SetDriverHealthCount ();
}


/**
    Notification function for driver health protocol instal.

    @param EFI_EVENT , VOID *

    @retval VOID

**/
VOID EFIAPI _UpdateGoPNotifyFunction (EFI_EVENT Event, VOID *Context)
{
	UpdateGoPUgaDraw();// Validate the Gop before usage in all the possible cases and also get instance of Gop through notification
	if (TSE_POST_STATUS_IN_BBS_POPUP == gPostStatus) 
	{
		MemSet(gFlushBuffer, sizeof(SCREEN_BUFFER), 0);
		ClearScreen(EFI_BACKGROUND_BLACK | EFI_WHITE);    
	}
}
/**
    It will create a notify event and register a notification.

    @param VOID

    @retval Status

**/
EFI_STATUS RegisterNotification( VOID )
{
	EFI_STATUS 	Status = EFI_UNSUPPORTED;
	UINTN 		i;
	EFI_EVENT 	Event, GopEvent;
	EFI_GUID 	EfiDriverHealthProtocolGuid = EFI_DRIVER_HEALTH_PROTOCOL_GUID;
	VOID 		*Registration = NULL, *GopRegistration = NULL;

	NOTIFICATION_INFO **notify = _gNotifyList;

	for ( i = 0; *notify != NULL; i++, notify++ )
	{
		Status = gBS->CreateEvent(
				EFI_EVENT_NOTIFY_SIGNAL,
				EFI_TPL_CALLBACK,
				NotificationFunction,
				*notify,
				&((*notify)->NotifyEvent)
				);

		if ( EFI_ERROR(Status) )
			continue;	

		Status = gBS->RegisterProtocolNotify(
				(*notify)->NotifyGuid,
				(*notify)->NotifyEvent,
				&((*notify)->NotifyRegistration)
				);
			// get any of these events that have occured in the past
		if (!EFI_ERROR (Status))
		{
		    gBS->SignalEvent( (*notify)->NotifyEvent );
		}
	}
	if (IsDriverHealthSupported ())			//Notifying the driver health protocol installation to update the drv health variable in cache
	{
		Status = gBS->CreateEvent(
				EFI_EVENT_NOTIFY_SIGNAL,
				EFI_TPL_CALLBACK,
				_DrvHealthNotifyFunction,
				NULL,
				&Event
				);
		if (!EFI_ERROR (Status))
		{
			Status = gBS->RegisterProtocolNotify(
				&EfiDriverHealthProtocolGuid,
				Event,
				&Registration
				);
			if (!EFI_ERROR(Status))
			{
				gBS->SignalEvent (Event);
			}
		}
	}

//Validate the Gop before usage in all the possible cases and also get instance of Gop through notification
	Status = gBS->CreateEvent(
				EFI_EVENT_NOTIFY_SIGNAL,
				EFI_TPL_CALLBACK,
				_UpdateGoPNotifyFunction,
				NULL,
				&GopEvent
				);
		if (!EFI_ERROR (Status))
		{
			Status = gBS->RegisterProtocolNotify(
				&gEfiGraphicsOutputProtocolGuid,
				GopEvent,
				&GopRegistration
				);
			if (!EFI_ERROR(Status))
			{
				gBS->SignalEvent (GopEvent);
			}
		}
	return Status;
}

#if 0
static _gConOutCount = 0;
#endif

#if !APTIO_4_00
extern EFI_GUID  gEfiConsoleOutDeviceGuid;

#endif

/**
    function to perform the notification operations.

    @param Event NOTIFICATION_INFO *Context

    @retval void

**/
VOID EFIAPI NotificationFunction( EFI_EVENT Event, VOID *NotificationContext )
{
	EFI_STATUS Status;
	UINTN Count = 0;
	EFI_HANDLE *HandleBuffer = NULL;
	BOOLEAN found = FALSE;
	UINT32 ModSomething = 0;
	NOTIFICATION_INFO *Context  =  NotificationContext;
	void *MouseDriverProtocol = NULL;
	UINT32 PasswordInstalled = AMI_PASSWORD_NONE;
	
	
	if ( Context == NULL )
		return;

	// Do nothing on Notification when Enters to setup/BBSPopup/Booting.
	if(gPostStatus >= TSE_POST_STATUS_ENTERING_TSE)
		return;

    //According to UEFI spec 2.0.: When LocateHandleBuffer() is called
    //with ByRegisterNotify it returns the next handle that is new
    //for the registration. Only one handle is returned at a time and
    //the caller must loop until no more handles.
    do
    {
        Status = gBS->LocateHandleBuffer(
                        ByRegisterNotify,
                        NULL,
                        Context->NotifyRegistration,
                        &Count,
                        &HandleBuffer
                        );

        if (EFI_ERROR( Status ))
            break;

        if(0 != Count)
        {
            EFI_DEVICE_PATH_PROTOCOL *DevicePath;

            Status = gBS->HandleProtocol( HandleBuffer[0], &gEfiDevicePathProtocolGuid, (void**)&DevicePath );

 			if (IsSetupUseGopSplitter() && Context == &_gUgaNotify)
            {  
                VOID* GraphicsSplitterModeChangeProtocol = NULL;

                Status = gBS->HandleProtocol( HandleBuffer[0], &gAmiGraphicsSplitterModeChangeProtocolGuid, (void**)&GraphicsSplitterModeChangeProtocol );
                if (EFI_SUCCESS == Status){
                    SETUP_DEBUG_UEFI( "[TSE] Found GraphicsSplitterModeChangeProtocol = 0x%X; Handle = 0x%X\n",
                            GraphicsSplitterModeChangeProtocol , HandleBuffer[0]);
                    found = TRUE;
                    break;
                }
                else
                    SETUP_DEBUG_UEFI( "[TSE] GraphicsSplitterModeChangeProtocol Status %r\n", Status);

            }
			else
            {
           //Check if notification for this protocol has to have a
                //device path
				if(Context->NotifyDevicePath)
				{
                	if ( !EFI_ERROR( Status ))
	        		{
	       			    found = TRUE;
	                    break;
	                }
				}
				else
        		{
       			    found = TRUE;
                    break;
                }
			}

            MemFreePointer( (VOID **)&HandleBuffer );
        }

    }while(Count);

#if 0
	if ( Context == &_gConOutNotify )
	{
		if ( _gConOutCount == 0 )
        {
            if(found)
            {
                MemFreePointer( (VOID **)&HandleBuffer );
    			found = FALSE;
            }
        }

		_gConOutCount++;
	}
#endif

	if ( ! found )
		return;

    Context->NotifyHandle = HandleBuffer[0];

	MemFreePointer( (VOID **)&HandleBuffer );

	RUNTIME_DEBUG( L"notify" );

	Status = gBS->HandleProtocol( Context->NotifyHandle, Context->NotifyGuid, (VOID **)&Context->NotifyProtocol );
    if( Context->pgProtocol )
        *(Context->pgProtocol) = Context->NotifyProtocol;

	if ( gGlobalNotify & Context->NotifyMask )
        return;

	if(Context->NotifyMask != NOTIFY_MASK_CONSOLE_IN)
		gGlobalNotify |= Context->NotifyMask;
	else
	{ 	//ConIn Notification is only valid with gST->ConIn != NULL. Otherwise not all the ConIn Connected.
		if(gST->ConIn != NULL )
		{
			gGlobalNotify |= Context->NotifyMask;
			gST->ConsoleInHandle = gST->ConsoleOutHandle;//Context->NotifyHandle;  To assign the "gST->ConsoleInHandle" to locate the Keycode protocol.
		}
	}
	
	ModSomething = GetNotifyMaskValue();	
	/*if(IsSoftKbdSupported())
	{//Checking for the presence of Softkbd
		ModSomething = SOMETHING;
	}
	else
	{//If softkbd is not present the unmasking the mousedriver notify from something
		ModSomething = SOMETHING & (~NOTIFY_MASK_MOUSE_DRIVER);
	}*/
 
//Commented because ActivateInput getting called twice
	/*if (NOTIFY_MASK_CONSOLE_IN == Context->NotifyMask)					//if softkbd presented then ActivateApplication invoked in postmanager only
	{																					//So changed the logic from switch to if
		//Check if Input has been activated and if yes Activate input
		//ConIn Notification is only valid with gST->ConIn != NULL
		if ( !IsDelayLogoTillInputSupported() ) 
		{
			if( (( gGlobalNotify & ModSomething ) == ModSomething) && ( gST->ConIn != NULL ) )
			{
				ActivateInput();
			}
		}
	}*/
	if (NOTIFY_MASK_MOUSE_DRIVER == Context->NotifyMask)
	{
		if(IsMouseSupported()&& IsSoftKbdSupported())// Initializing the mouse at post when mouse and softkbd present
		{
			MouseInit();					//If mouse is notified then Initializing the mouse
			InstallClickHandlers (); 	//install click handlers in notification itself. In other places installing click handler is removed.
		}
	}
	if (((gGlobalNotify & ModSomething) == ModSomething))			//if gConsoleControl is not validated then ActivateApplication will be invoked twice
	{
		/*//All necessary protocols available activate output
		if (NULL == gConsoleControl)	//If kept outside then activateinput wont get call succesully
		{
			ActivateApplication();
		}*/
	
		//If Simple Text Input already available but input has not been activated yet activate now
		// ConIn Notification is only valid with gST->ConIn != NULL
		if (IsDelayLogoTillInputSupported ()) 
		{
			if( (( gGlobalNotify & ModSomething ) == ModSomething) && ( gST->ConIn != NULL ) && (FALSE == ActivateInputDone))
			{
				if (IsMouseSupported()&& IsSoftKbdSupported())		//if softkbd & password presents wait until mouse notify
				{
					PasswordInstalled = PasswordCheckInstalled();
					if(CheckSystemPasswordPolicy(PasswordInstalled))
					{	
						Status = gBS->LocateProtocol(&gEfiDXEMouseDriverProtocolGuid, NULL, &MouseDriverProtocol);
						if (EFI_ERROR (Status))
						{
							return;
						}
					}
				}
				ActivateInputDone = TRUE;
				ActivateApplication();
				ActivateInput();
			}
		}
		else
		{
#if !TSE_USE_EDK_LIBRARY		
			if (NULL == gConsoleControl)
			{
				ActivateApplication ();
			}
#endif			
			if( (( gGlobalNotify & ModSomething ) == ModSomething) && ( gST->ConIn != NULL ) && (FALSE == ActivateInputDone)) 
			{
				if (IsMouseSupported()&& IsSoftKbdSupported())		//if softkbd & password presents wait until mouse notify	
				{
					PasswordInstalled = PasswordCheckInstalled();
					if(CheckSystemPasswordPolicy(PasswordInstalled))
					{	
						Status = gBS->LocateProtocol(&gEfiDXEMouseDriverProtocolGuid, NULL, &MouseDriverProtocol);
						if (EFI_ERROR (Status))
						{
							return;
						}
					}
				}
				ActivateInputDone = TRUE;
				ActivateInput();
			}
		}
   }
}

/**
    function for the Active application operations.

    @param void

    @retval void

**/
VOID ActivateApplication( VOID )
{
	CHAR16 *text=NULL,*newString=NULL, *text2=NULL;
	UINTN length, TseMajor,TseMinor, TseBuild;
	UINT16 TseBuildYear=0;
#if APTIO_4_00
	UINTN ConCols,ConRows;
#endif
#if !TSE_USE_EDK_LIBRARY
	gConsoleControl = (EFI_CONSOLE_CONTROL_PROTOCOL *)_gConsoleControlNotify.NotifyProtocol;
#endif
	
	CheckEnableQuietBoot ();
	if(gST->ConOut != NULL)
	{
#if !APTIO_4_00
	SetDesiredTextMode();
#else
	if(EFI_ERROR(gST->ConOut->QueryMode( gST->ConOut, gST->ConOut->Mode->Mode, &ConCols, &ConRows )))
	{
		gMaxRows = StyleGetStdMaxRows();
		gMaxCols = StyleGetStdMaxCols();
	}
	else
	{
	  if((gMaxRows > ConRows) && (gMaxCols > ConCols))
	  {
	 	//  In Aptio 4.x use the Text mode as is it in post screen (Set by the Core)
		// Old Post messages already provided may be affected.
		// Preserve Post messages
		PreservePostMessages(ConCols, gMaxCols);
		gMaxRows = ConRows;
		gMaxCols = ConCols;
	  }
	  else if ((gMaxRows < ConRows) && (gMaxCols < ConCols) && (!gQuietBoot))		//Desired Text mode will be set at InitPostScreen, if quiet boot dont set mode for seamless boot
	  {
		// Current screen mode is bigger then the TSE supports.
		// TSE sets DesiredTextMode
      SetDesiredTextMode();
	  }
	}
#endif
	gPSMaxRows = gMaxRows; //Taking backup of post screen resolution to use it further 
	gPSMaxCols = gMaxCols;

	}
#if APTIO_4_00 
    {
        EFI_GUID SgnFile = SGN_ON_FILE_GUID;
        EFI_GUID SgnSection = SGN_ON_SECTION_GUID;
        AMI_SIGN_ON *SignOn;        
    	VOID  *pSection;
	    UINTN SectionSize;
        EFI_STATUS Status;
        CHAR8 *SgnMsg2;

        Status = FvReadResources (&SgnFile, &SgnSection,(VOID**)&pSection,&SectionSize);
    	if (! EFI_ERROR( Status ) ) {
            SignOn = (AMI_SIGN_ON*)((UINT8 *)pSection + sizeof(EFI_GUID));
            if( (MemCmp( (UINT8*)&SignOn->Sig, (UINT8*)"$SGN$",5) == 0) && (SignOn->Ver >= 2))
            {
                text = StrDup8to16( SignOn->SgnMsg1 );
				if (!IsSetupHideBiosSignOnMsg ())
				{
				    SgnMsg2 = (CHAR8*) ((UINT8 *)&SignOn->SgnMsg1 + StrLen8(SignOn->SgnMsg1)+1); // Cast for GCC Build 
					text2 = StrDup8to16( SgnMsg2 );
				}
            }
        }
        else
			text = HiiGetString( gHiiHandle, STRING_TOKEN(STR_MAIN_COPYRIGHT) );
    }
#else
	// Version number at the Setup/POST Screen corrupted if other language is chosen
	text = HiiGetString( gHiiHandle, STRING_TOKEN(STR_MAIN_COPYRIGHT) );
#endif

	if ( text != NULL )
	{
		length = EfiStrLen(text) + 10;
		newString = EfiLibAllocateZeroPool( length * sizeof(CHAR16) );
		if ( newString != NULL )
		{
			// Source modules in TSE should get the version details from binary in run time not at build time
			GetTseBuildVersion(&TseMajor,&TseMinor,&TseBuild);
			//Use BIOS build year for copyright message
			GetTseBuildYear(&TseBuildYear);
			SPrint( newString, length * sizeof(CHAR16), text, TseMajor, TseMinor, TseBuild,TseBuildYear );
			PostManagerDisplayPostMessage(newString);
			MemFreePointer( (VOID **)&newString );
		}
		else
			PostManagerDisplayPostMessage(text);
		MemFreePointer( (VOID **)&text );
	}
    // Sign on OEM String
	if(text2 != NULL)
	{
		PostManagerDisplayPostMessage(text2);
		MemFreePointer( (VOID **)&text2 );
	}

	if (IsDelayLogoTillInputSupported())	 
		ProcessConInAvailabilityHook();

#ifdef EFI_NT_EMULATOR
    gGlobalNotify = gGlobalNotify | NOTIFY_MASK_CONSOLE_OUT;
#endif

	if ( gGlobalNotify & NOTIFY_MASK_UGA )
	{
		if ( gQuietBoot )
		{
			gPostStatus = TSE_POST_STATUS_IN_QUITE_BOOT_SCREEN;
#if !TSE_USE_EDK_LIBRARY			
			if(gConsoleControl != NULL)
				gConsoleControl->SetMode( gConsoleControl, EfiConsoleControlScreenGraphics );
#endif
			DrawQuietBootLogoHook();
			if(IsMouseSupported()&& IsSoftKbdSupported())//Initializing the mouse at post when mouse and softkbd present
				MouseInit();
			gProgress->quiteBootActive = TRUE ;
			if(gProgress->active)
				PostManagerSetProgressBarPosition() ;
			
		}
	}

//	if ( gGlobalNotify & NOTIFY_MASK_CONSOLE_OUT )
	if ( gST->ConOut != NULL ) // If ConOut is available in System Table the display the post messages.
	{
		//Show post screen if it is not quiet boot or if it is a headless system
		if ( ! gQuietBoot || !gGOP)
		{
			CleanUpLogo();
			InitPostScreen();
		}
	
	ProcessConOutAvailableHook();
	}

	if (IsDelayLogoTillInputSupported())	
	{
		if ( !TseIgnoreKeyForFastBoot() ) 
			InstallKeyHandlers();
/*		
  		//Lets install mouse click event in NotificationFunction itself
		if( IsMouseSupported() && IsSoftKbdSupported())//: Checking for mouse and softkbd presence
			InstallClickHandlers();
*/			
	}
}



/**
    function for the active Input.

    @param void

    @retval void

**/
VOID ActivateInput( VOID )
{
//    EFI_STATUS Status; Set not used 
    VOID *UgaBlt = NULL;
    UINTN SizeOfX=0, SizeOfY=0;
    BOOLEAN bRedraw = FALSE;		//If not initialized then on delay till logo input case we always restoring post due to garabage initialization

    FindandSetHotKeys ();

    if( !IsDelayLogoTillInputSupported())
    {              
        if ( gGlobalNotify & NOTIFY_MASK_UGA )
          UgaBlt = SavePostScreen(&SizeOfX, &SizeOfY);
            bRedraw = ProcessConInAvailabilityHook();
        if( gGlobalNotify & NOTIFY_MASK_UGA )
        {
            if(bRedraw)
                RestorePostScreen (UgaBlt, SizeOfX, SizeOfY);
            else 
                MemFreePointer((VOID **) &UgaBlt);       
        }
         if ( !TseIgnoreKeyForFastBoot() ) 
             InstallKeyHandlers();
    }
}

//Function to update gGop 
/**
    function to update the Gop when controls comes to TSE.

    @param void

    @retval void

**/
VOID UpdateGoPUgaDraw( VOID )
{
    EFI_STATUS Status;
	UINTN Count = 0;
	UINTN Index=0;
	EFI_HANDLE *HandleBuffer = NULL;	
    EFI_DEVICE_PATH_PROTOCOL *DevicePath;
	VOID* GraphicsSplitterModeChangeProtocol = NULL;
	gGOP = NULL;

    SETUP_DEBUG_UEFI( "[TSE]UpdateGop: Initial gGOP = 0x%x\n ",gGOP);
    Status = gBS->LocateHandleBuffer(
                    ByProtocol,
                    &gEfiGraphicsOutputProtocolGuid,
                	NULL,
                    &Count,
                    &HandleBuffer
                    );
     SETUP_DEBUG_UEFI( "[TSE] GOP handles found = %d\n ",Count);
	 for(Index=0;Index < Count;Index++)
     {
        Status = gBS->HandleProtocol( HandleBuffer[Index], &gEfiDevicePathProtocolGuid, (void**)&DevicePath );
        if(IsSetupUseGopSplitter())
        {  // Use GraphicsSplitterModeChangeProtocol to get the splitter handle
           Status = gBS->HandleProtocol( HandleBuffer[Index], &gAmiGraphicsSplitterModeChangeProtocolGuid, (void**)&GraphicsSplitterModeChangeProtocol );
           if (EFI_SUCCESS == Status){
               Status = gBS->HandleProtocol( HandleBuffer[Index], &gEfiGraphicsOutputProtocolGuid, (void**)&gGOP);
               SETUP_DEBUG_UEFI( "[TSE] GraphicsSplitterModeChangeProtocol: Handle[%d] = 0x%X, gGOP = 0x%X\n ", Index, HandleBuffer[Index], gGOP);
               break;
           }
           else
           {
			   SETUP_DEBUG_UEFI( "[TSE] GraphicsSplitterModeChangeProtocol not found: Handle[%d] = 0x%X, Status %r\n", Index, HandleBuffer[Index], Status);
               continue;
           }
        }
        else if (EFI_ERROR( Status ))
		{   // if splitter is not supported, check if there's a device path
	   		SETUP_DEBUG_UEFI( "[TSE] No GOP Splitter: GOP doesn't have a device path, continue\n ");
            continue;
		}
        else
        {
            Status = gBS->HandleProtocol( HandleBuffer[Index], &gEfiGraphicsOutputProtocolGuid, (void**)&gGOP);
            SETUP_DEBUG_UEFI( "[TSE] GOP with device path: Handle[%d] = 0x%X, gGOP = 0x%X\n ", Index, HandleBuffer[Index], gGOP);
            break;
        }             
        
	}	
	 if (!gGOP)
	 {
	     Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL,(void**) &gGOP);
	     if(EFI_ERROR(Status))
	            gGOP = NULL;
	     SETUP_DEBUG_UEFI( "[TSE] Locate any GOP = 0x%X\n",gGOP);
	 }
	 SETUP_DEBUG_UEFI( "[TSE] Update GOP = 0x%X\n ",gGOP);
	MemFreePointer( (VOID **)&HandleBuffer );	

}
#endif // STANDALONE_APPLICATION

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2013, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**     5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
