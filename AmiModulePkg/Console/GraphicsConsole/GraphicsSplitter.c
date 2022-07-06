//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
/** @file
    @brief This file contains the code that provides the Graphics Splitter functionality for the BIOS.
 
 */
#include <Protocol/GraphicsOutput.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/ComponentName2.h>
#include <Library/UefiLib.h>
#include <Uefi/UefiBaseType.h>                                                  // Build
#include <Library/BaseMemoryLib.h>
#include <AmiDxeLib.h>
#include <Library/AmiListLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Base.h>

//=======================================================================================
// new typedefs

typedef struct _GOP_DEVICE
{
    AMI_LINK                        Link; ///< Linked list pointer MUST BE THE FIRST FIELD
    EFI_GRAPHICS_OUTPUT_PROTOCOL    *Gop; ///< Protocol pointer
    EFI_HANDLE                      Handle; ///< Device handle
} GOP_DEVICE;


typedef struct __PIXEL_RES
{
    UINT32   ModeNumber;  // number of the GOP devices mode
    UINT32   HorizontalRes;  ///< Horizontal Resolution in Pixels
    UINT32   VerticalRes;    ///< Vertical Resolution in Pixels
} PIXEL_RES;

typedef struct __PIXEL_RES_LIST
{
    AMI_LINK                       Link; ///< Linked list pointer MUST BE THE FIRST FIELD
    PIXEL_RES                   Resolution; //
} PIXEL_RES_LIST;

typedef struct _SAVE_SCREEN_INFO
{
    UINT32 HorizontalResolution;
    UINT32 VerticalResolution;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL  *Buffer;
    UINTN XAxis;
    UINTN YAxis;
} SAVE_SCREEN_INFO;

//-----------------------------------------------------------------------------
// Function Prototypes

EFI_STATUS EFIAPI 
GraphicsSplitter_QueryMode (
        IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
        IN UINT32 ModeNumber,
        OUT UINTN *SizeOfInfo,
        OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info);

EFI_STATUS EFIAPI 
GraphicsSplitter_SetMode(
        IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
        IN UINT32 ModeNumber);

EFI_STATUS EFIAPI 
GraphicsSplitter_Blt (
        IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
        IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer, OPTIONAL
        IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION BltOperation,
        IN UINTN SourceX,
        IN UINTN SourceY,
        IN UINTN DestinationX,
        IN UINTN DestinationY,
        IN UINTN Width,
        IN UINTN Height,
        IN UINTN Delta OPTIONAL);

EFI_STATUS EFIAPI
GS_BltNoGopDevice(
        IN SAVE_SCREEN_INFO *This,
        IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer, OPTIONAL
        IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION BltOperation,
        IN UINTN SourceX,
        IN UINTN SourceY,
        IN UINTN DestinationX,
        IN UINTN DestinationY,
        IN UINTN Width,
        IN UINTN Height,
        IN UINTN Delta OPTIONAL);

EFI_STATUS  EFIAPI
GraphicsSplitterDriverBindingSupported ( 
    IN EFI_DRIVER_BINDING_PROTOCOL    *This,
    IN EFI_HANDLE                     ControllerHandle,
    IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath);

EFI_STATUS  EFIAPI
GraphicsSplitterDriverBindingStart ( 
    IN EFI_DRIVER_BINDING_PROTOCOL    *This,
    IN EFI_HANDLE                     ControllerHandle,
    IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath);

EFI_STATUS  EFIAPI
GraphicsSplitterDriverBindingStop ( 
    IN EFI_DRIVER_BINDING_PROTOCOL  *This,
    IN EFI_HANDLE                   ControllerHandle,
    IN  UINTN                       NumberOfChildren,
    IN  EFI_HANDLE                  *ChildHandleBuffer);


EFI_STATUS ConfigureGopModes( EFI_GRAPHICS_OUTPUT_PROTOCOL *PhysicalGopDevice, BOOLEAN *ModeChange);
EFI_STATUS AddGopDevice(EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput, EFI_HANDLE ControllerHandle);
EFI_STATUS VerifyAndSetMode( EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, UINT32 HorRes, UINT32 VertRes, OPTIONAL BOOLEAN *Match);

EFI_STATUS EFIAPI RedrawSaveScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput);
EFI_STATUS EFIAPI SaveScreen (EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput);


//------------------------------------------------------------------------------
//  Global Variables
EFI_GRAPHICS_OUTPUT_MODE_INFORMATION GraphicsSplitterInfo = {
        0, // UINT32     Version;
        0, // UINT32  HorizontalResolution;
        0, // UINT32  VerticalResolution;
        PixelBltOnly, // EFI_GRAPHICS_PIXEL_FORMAT  PixelFormat;
        {0, 0, 0, 0}, // EFI_PIXEL_BITMASK          PixelInformation;
        0, // UINT32                     PixelsPerScanLine;
};

EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE GraphicsSplitterMode = {
        0, // UINT32    MaxMode;
        0xFFFF, // UINT32    Mode;
        &GraphicsSplitterInfo, // EFI_GRAPHICS_OUTPUT_MODE_INFORMATION   *Info;
        0, // UINTN     SizeOfInfo;
        0, // EFI_PHYSICAL_ADDRESS                   FrameBufferBase;
        0, //UINTN                                  FrameBufferSize;
};

EFI_GRAPHICS_OUTPUT_PROTOCOL GraphicsSplitter = {
        GraphicsSplitter_QueryMode,
        GraphicsSplitter_SetMode,
        GraphicsSplitter_Blt,
        &GraphicsSplitterMode
};

//==============================================================================
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOLEAN gModeChanged = FALSE;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//==============================================================================

EFI_HANDLE GopSplitterHandle = NULL;

AMI_LIST GopList;  // List of GOP devices
AMI_LIST GopModes; // List of the compatiblt modes
AMI_LIST NewModeList; // List used when a new GOP is found to compare the list

SAVE_SCREEN_INFO SaveScreenInfo = {
        0,      // Horizontal Resolution
        0,      // Vertical Resolution
        NULL,   // Pointer to a buffer that will save the video screen only when no physical devices are connected.  Otherwise, NULL
        0,      // X Axis cursor position
        0       // Y Axis cursor position
};

// Refer to a comment block towards the end of ConfigureGopModes function
// for additional details about the dirty flag.
BOOLEAN DirtyMode = FALSE;

//-----------------------------------------------------------------------------
// Driver Binding Protocol
/// Driver Binding protocol for the Graphics Splitter Driver
EFI_DRIVER_BINDING_PROTOCOL gGraphicsSplitterDriverBindingProtocol = {
    GraphicsSplitterDriverBindingSupported,
    GraphicsSplitterDriverBindingStart,
    GraphicsSplitterDriverBindingStop,
    0x11,
    NULL,
    NULL
    };

//-----------------------------------------------------------------------------
// Function Definitions
/// Driver Name used to identify the driver
//==============================================================================
//==============================================================================

static UINT16 *gDriverName = L"AMI Graphic Splitter Driver";
static UINT16 *gControllerName = L"AMI Graphics Splitter Controller";
//==============================================================================
//==============================================================================

/**
    This function is supposed to return the Controller name of the Handle that is passed in

    @param[in] This - pointer to protocol instance
    @param[in] Controller - controller handle
    @param[in] ChildHandle - child handle
    @param[in] Language - pointer to language description
    @param[out] ControllerName - pointer to store pointer to controller name

    @retval EFI_SUCCESS - controller name returned
    @retval EFI_INVALID_PARAMETER - language undefined
    @retval EFI_UNSUPPORTED - given language not supported
*/
static EFI_STATUS EFIAPI
GraphicsSplitterGetControllerName (
        IN  EFI_COMPONENT_NAME2_PROTOCOL  *This,
        IN  EFI_HANDLE                   ControllerHandle,
        IN  EFI_HANDLE                   ChildHandle        OPTIONAL,
        IN  CHAR8                        *Language,
        OUT CHAR16                       **ControllerName
)
{
    //Supports only English
    if(!Language || !ControllerName || !ControllerHandle)
        return EFI_INVALID_PARAMETER;

    if(ChildHandle!=GopSplitterHandle || !LanguageCodesEqual( Language, LANGUAGE_CODE_ENGLISH))
        return EFI_UNSUPPORTED;

    *ControllerName=gControllerName;
    return EFI_SUCCESS;
}


/**
    This function is supposed to return the Driver name of this Driver

    @param[in] This - pointer to EFI_COMPONENT_NAME2_PROTOCOL protocol instance
    @param[in] Language - pointer to language description
    @param[out] DriverName - pointer that will point to the Driver Name if return
                        value is EFI_SUCCESS

    @retval EFI_SUCCESS - driver name returned
    @retval EFI_INVALID_PARAMETER - language undefined
    @retval EFI_UNSUPPORTED - given language not supported
*/
static EFI_STATUS EFIAPI
GraphicsSplitterGetDriverName(
    IN  EFI_COMPONENT_NAME2_PROTOCOL  *This,
    IN  CHAR8                        *Language,
    OUT CHAR16                       **DriverName
)
{
    //Supports only English
    if(!Language || !DriverName) 
        return EFI_INVALID_PARAMETER;

    if (!LanguageCodesEqual( Language, LANGUAGE_CODE_ENGLISH)) 
        return EFI_UNSUPPORTED;
    else 
        *DriverName = gDriverName;
    return EFI_SUCCESS;
}

//Component Name Protocol
static EFI_COMPONENT_NAME2_PROTOCOL gGraphicsSplitterComponentName2 = {
  GraphicsSplitterGetDriverName,
  GraphicsSplitterGetControllerName,
  LANGUAGE_CODE_ENGLISH
};

// End of Component Name protocols
//=======================================================================================


//=====================================================================================
// Support functions
/**
    This function deletes all elements in a AMI_LIST
  
    @param pList - linked list to remove all elements from 
    @return
 */
VOID AmiListDeleteList( AMI_LIST *List)
{
    AMI_LINK *ListPtr, *Temp;
    
    ListPtr = List->pHead;
    while ( ListPtr != NULL)
    {
        Temp = ListPtr;
        ListPtr = ListPtr->pNext;
        AmiListDelete(List, Temp);
//        gBS->FreePool(pTemp);
        FreePool(Temp);
    }   
}



/**
     This function verifies that we are not already supporting this handle and that it is not our own handle
 
  @param ControllerHandle - Handle passed into the Supported function 
  @return
  
 */
EFI_STATUS CheckGopHandle( EFI_HANDLE ControllerHandle)
{
    EFI_STATUS Status = EFI_SUCCESS;
    
    // check to see if this handle 
    if (ControllerHandle == GopSplitterHandle)
        return EFI_ALREADY_STARTED;

    return Status;
    
}



/**
    Driver entry point - initializes all linked lists and installs the driver binding protocol for the driver

    @param ImageHandle Image handle of the Graphics Splitter driver
    @param SystemTable Pointer to the EFI_SYSTEM_TABLE

    @retval EFI_SUCCESS driver installed successfully
    @retval EFI_ERROR error occured during execution
**/
EFI_STATUS GraphicsSplitterEntryPoint(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
)
{
    EFI_STATUS Status;
    
    InitAmiLib(ImageHandle, SystemTable);

    // Initialize the global lists here
    AmiListInit(&GopList);
    AmiListInit(&NewModeList);
    AmiListInit(&GopModes);
    
    // Install driver binding protocol here
    Status = EfiLibInstallDriverBindingComponentName2 (
        ImageHandle,
        SystemTable,
        &gGraphicsSplitterDriverBindingProtocol,
        gGraphicsSplitterDriverBindingProtocol.DriverBindingHandle,
        NULL,
        &gGraphicsSplitterComponentName2
    );

    return Status;
}


/**
    Driver Binding Protocol Supported function
    
    Checks to see if this driver supports the device represented by the Controller Handle 

    @param[in] This - pointer to protocol instance
    @param[in] ControllerHandle - handle of controller to install driver on
    @param[in] RemainingDevicePath - pointer to device path

    @retval EFI_SUCCESS - Driver supports the Device
    @retval EFI_UNSUPPORTED - Driver cannot support the Device 
*/
EFI_STATUS  EFIAPI
GraphicsSplitterDriverBindingSupported (
    IN EFI_DRIVER_BINDING_PROTOCOL    *This,
    IN EFI_HANDLE                     ControllerHandle,
    IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
)
{
    EFI_STATUS                   Status;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput = NULL;
    
    // Check if we have supported this handle already and make sure it is not us
    Status = CheckGopHandle(ControllerHandle);
    if (EFI_ERROR(Status))
        return Status;
    
    // open the GOP protocol
    Status = gBS->OpenProtocol(
                        ControllerHandle, 
                        &gEfiGraphicsOutputProtocolGuid,
                        (VOID**)&GraphicsOutput, 
                        This->DriverBindingHandle,
                        ControllerHandle, 
                        EFI_OPEN_PROTOCOL_BY_DRIVER);
    if (EFI_ERROR(Status))
        return Status;

    gBS->CloseProtocol( 
                    ControllerHandle, 
                    &gEfiGraphicsOutputProtocolGuid,
                    This->DriverBindingHandle, 
                    ControllerHandle);

    // verify that the handle has a device path associated with it
    Status = gBS->OpenProtocol(
                        ControllerHandle, 
                        &gEfiDevicePathProtocolGuid,
                        NULL, 
                        This->DriverBindingHandle,
                        ControllerHandle, 
                        EFI_OPEN_PROTOCOL_TEST_PROTOCOL);

    return Status;
}




/**
    Driver Binding Protocol Start function

    This function grabs needed protocols and chooses the system's graphics resolution
    based on the different GOP devices that have been 

    @param[in] This - pointer to protocol instance
    @param[in] ControllerHandle - handle of controller to install driver on
    @param[in] RemainingDevicePath - pointer to device path

    @retval EFI_SUCCESS - SimpleTextOut Protocol installed
    @retval EFI_UNSUPPORTED - SimpleTextOut Protocol not installed
*/  
EFI_STATUS  EFIAPI
GraphicsSplitterDriverBindingStart (
    IN EFI_DRIVER_BINDING_PROTOCOL *This,
    IN EFI_HANDLE                  ControllerHandle,
    IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
)
{
    EFI_STATUS          Status;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput = NULL;
    BOOLEAN ModeChange;
    
    // open the GOP protocol
    Status = gBS->OpenProtocol( ControllerHandle, 
                                &gEfiGraphicsOutputProtocolGuid,
                                (VOID**)&GraphicsOutput, 
                                This->DriverBindingHandle,
                                ControllerHandle, 
                                EFI_OPEN_PROTOCOL_BY_DRIVER);

    if (EFI_ERROR(Status))
        return Status;
    
    // Gather Information on the current GOP device and combine it with the others
    Status = ConfigureGopModes(GraphicsOutput, &ModeChange);
    if (!EFI_ERROR(Status))
    {
        // Add GOP protocol to list owned protocols
        Status = AddGopDevice(GraphicsOutput, ControllerHandle);
        // if this isn't the first device, get the current screen and draw it on this screen.
        if (GopList.Size > 1)
        {
            // Get screen from the current screen and draw it on the second screen
            
        }
    }

    // if we have an error, then close the protocol and exit
    if (EFI_ERROR(Status))
    {
        gBS->CloseProtocol(ControllerHandle, &gEfiGraphicsOutputProtocolGuid,
                           This->DriverBindingHandle, ControllerHandle);
        return EFI_UNSUPPORTED;
    }


    // only install the GOP Splitter Protocol once.
    if (GopSplitterHandle == NULL) 
    {
        DEBUG((DEBUG_INFO, "GraphicsSplitter_Start: Get GopSplitterHandle\n\r"));
       // Install a logical GOP protocol that the GraphicsConsole driver can bind to it
        Status = gBS->InstallMultipleProtocolInterfaces(
                                &GopSplitterHandle,
                                &gEfiGraphicsOutputProtocolGuid, &GraphicsSplitter,
                                &gAmiGraphicsSplitterModeChangeProtocolGuid, &gModeChanged,
                                NULL);
    }

    Status = gBS->OpenProtocol( ControllerHandle, 
                            &gEfiGraphicsOutputProtocolGuid,
                            (VOID**)&GraphicsOutput, 
                            This->DriverBindingHandle,
                            GopSplitterHandle, 
                            EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER);

    if (ModeChange == TRUE)
    {
        // this is used to inform the Graphics Console and Consplitter that the resolution has changed.
        gBS->ReinstallProtocolInterface( GopSplitterHandle,
                                         &gEfiGraphicsOutputProtocolGuid, 
                                         &GraphicsSplitter,
                                         &GraphicsSplitter);
 

        DEBUG((DEBUG_INFO, "GraphicsSplitter_Start: Mode changed, reinstall interfaces\n\r"));
        gBS->ReinstallProtocolInterface( GopSplitterHandle,
                                         &gAmiGraphicsSplitterModeChangeProtocolGuid, 
                                         &gModeChanged,
                                         &gModeChanged);
    }


    // if the video buffer exists, attempt to draw it on the screen for the Physical Device, then delete the buffer.
    if (SaveScreenInfo.Buffer != NULL)
    {
        RedrawSaveScreen(GraphicsOutput);
    }
    

    

    DEBUG((DEBUG_INFO, "GraphicsSplitter_Start: Exiting Start routine\n\r"));
    return Status;
}


/**
    Graphics Splitter driver binding stop function 

    @param This Pointer to the driver binding protocol
    @param ControllerHandle Handle of the Graphics Splitter controller 
    @param NumberOfChildren Number of child handles in the ChildHandleBuffer
    @param ChildHandleBuffer Pointer to array of ChildDevices of this controller

    @retval EFI_SUCCESS driver stopped successfully
    @retval EFI_INVALID_PARAMETER invalid values passed for NumberOfChildren or ChildHandleBuffer
    @retval EFI_UNSUPPORTED The controller could not be removed from the management of this driver
**/
EFI_STATUS EFIAPI
GraphicsSplitterDriverBindingStop (
    IN EFI_DRIVER_BINDING_PROTOCOL *This,
    IN EFI_HANDLE                  ControllerHandle,
    IN UINTN                       NumberOfChildren,
    IN EFI_HANDLE                  *ChildHandleBuffer OPTIONAL
)
{
    EFI_STATUS Status;
    AMI_LINK   *ListPtr;
    GOP_DEVICE *GopDevice;
    BOOLEAN DeviceRemoved = FALSE;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop = NULL;

    // This is the case of the graphics splitter stopping itself
    if (NumberOfChildren == 0)
    {
        // if there are no GOP devices in the list and we have a screen buffer.  
        // get rid of the screen buffer
        if ((GopList.Size == 0) && (SaveScreenInfo.Buffer != NULL))
        {
            
            gBS->FreePool(SaveScreenInfo.Buffer);
            SaveScreenInfo.Buffer = NULL;
            SaveScreenInfo.HorizontalResolution = 0;
            SaveScreenInfo.VerticalResolution = 0;
            SaveScreenInfo.XAxis = 0;
            SaveScreenInfo.YAxis = 0;
        }
        
        return EFI_SUCCESS;
    }

    // too many children, invalid case
    if ( NumberOfChildren != 1 ||
         ChildHandleBuffer == NULL ||
         *ChildHandleBuffer != GopSplitterHandle )
        return EFI_INVALID_PARAMETER;

    // One child to remove, remove it and continue
    // remove the GOP Device from the list
    ListPtr = GopList.pHead;
    while (ListPtr != NULL)
    {
        GopDevice = BASE_CR(ListPtr, GOP_DEVICE, Link);
        if (GopDevice->Handle == ControllerHandle)
        {
            DeviceRemoved = TRUE;
            Gop = GopDevice->Gop;
            gBS->CloseProtocol(ControllerHandle, &gEfiGraphicsOutputProtocolGuid,
                        This->DriverBindingHandle, ControllerHandle);

            gBS->CloseProtocol(ControllerHandle, &gEfiGraphicsOutputProtocolGuid,
                        This->DriverBindingHandle, GopSplitterHandle);
            AmiListDelete(&GopList, ListPtr);

            FreePool(GopDevice);
            
            break;
        }

        ListPtr = ListPtr->pNext;
    }
    if (!DeviceRemoved) return EFI_INVALID_PARAMETER;

    // delete mode list
    AmiListDeleteList(&GopModes);

    // Now recalculate the graphics Mode list and the current mode
    if (GopList.Size > 0)
    {
        // loop through all leftover modes and re-initialize the mode list
        ListPtr = GopList.pHead;
        while (ListPtr != NULL)
        {
            GopDevice = BASE_CR(ListPtr, GOP_DEVICE, Link);
            // The devices that are left should share a mode since they did previously
            //  status can be ignored
            ConfigureGopModes(GopDevice->Gop, NULL);
            ListPtr = ListPtr->pNext;
        }


// TODO:  Did the mode change here??  how do we know?  there is no reason to reinstall if the mode doesn't change        
        // this is used to inform the Graphics Console and Consplitter that the resolution has changed.
        Status = gBS->ReinstallProtocolInterface( GopSplitterHandle,
                                         &gEfiGraphicsOutputProtocolGuid, 
                                         &GraphicsSplitter,
                                         &GraphicsSplitter);
    DEBUG((DEBUG_INFO, "GraphicsSplitter_SetMode: Mode changed reinstall Mode changed protocol\n\r"));

        gBS->ReinstallProtocolInterface( GopSplitterHandle,
                                         &gAmiGraphicsSplitterModeChangeProtocolGuid, 
                                         &gModeChanged,
                                         &gModeChanged);

    }
    // all devices are gone.  
    else 
    {
        SaveScreen(Gop);

        // set the Mode and Mode->Info information of the Graphics splitter structure to
        //  dummy values so that anyone reading these values will know these are fake.
        GraphicsSplitter.Mode->MaxMode = 1;
        GraphicsSplitter.Mode->Mode = 0;
        GraphicsSplitter.Mode->Info->HorizontalResolution = SaveScreenInfo.HorizontalResolution;
        GraphicsSplitter.Mode->Info->VerticalResolution = SaveScreenInfo.VerticalResolution;
    }

    return EFI_SUCCESS;
}
// End Driver Binding Protocol definition
//=======================================================================================

/**
    This function checks to see if mode *pMode is in mode list *pList.  If it is and 
    **Mode is not NULL, than the list entry is returned in **Mode
  
  @param pList - List of modes currently supported by the driver
  @param pMode - pointer to a mode that pList will be searched for
  @param Mode - pointer to a optional structure to pass back the mode if it is found in pList
  @return BOOLEAN
  @retval TRUE - pMode was found in pList and Mode now points to the mode is pList if Mode was not NULL
  @retval FALSE - pMode was not found in pList
 */
BOOLEAN IsModeInList(AMI_LIST *pList, PIXEL_RES_LIST *pMode, OUT PIXEL_RES_LIST **Mode)
{
    AMI_LINK *ListPtr;
    PIXEL_RES_LIST *ListMode;
    
    ListPtr = pList->pHead;
    while (ListPtr != NULL)
    {
        ListMode = BASE_CR(ListPtr, PIXEL_RES_LIST, Link);  
        
        if ((ListMode->Resolution.HorizontalRes == pMode->Resolution.HorizontalRes )
            && (ListMode->Resolution.VerticalRes == pMode->Resolution.VerticalRes))
        {
            if (Mode != NULL)
            {
                *Mode = ListMode;
            }
            
            return TRUE;
        }
        ListPtr = ListPtr->pNext;
    }
    return FALSE;
}

/**
 Add the Gop device to the GOP Splitter's list
 
    @param GraphicsOutput - pointer to Graphics Output Protocol that needs to be added to he list
    @param ControllerHandle - handle passed in for Graphics Output Protocol 
    @return EFI_STATUS
    @retval EFI_SUCCESS - Graphics Output Protocol is added to the GopList
    @retval EFI_OUT_OF_RESOURCES - not enough memory to allcate the node to add the protocol to the list
 */
EFI_STATUS
AddGopDevice(EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput, EFI_HANDLE ControllerHandle)
{
    GOP_DEVICE *GopDevice; 
    
    // Create an entry in the Con Spliter's Simple Out device list
    GopDevice = (GOP_DEVICE *)AllocatePool ( sizeof(GOP_DEVICE) );

    if (GopDevice == NULL)
        return EFI_OUT_OF_RESOURCES;

    GopDevice->Gop = GraphicsOutput;
    GopDevice->Handle = ControllerHandle;

    // Add new GOP device to our tracking list
    AmiListAdd(&GopList, &(GopDevice->Link));
    
    return EFI_SUCCESS;
}


/**
    This function goes through PhysGop and creates a linked list of supported modes
    
    @param PhysGop - GOP protocol to use to create a mode list
    @param ModeList - pointer tp a temporary linked list that will hold the list of modes of this device
    @return EFI_STATUS
    @retval EFI_INVALID_PARAMETER - incorrect parameters passed in
    @retval EFI_OUT_OF_RESOURCES - not enough memory to finish creating the list of modes from the device
    @retval EFI_SUCCESS - List created and passed back through ModeList

 */
EFI_STATUS CreateResolutionList(EFI_GRAPHICS_OUTPUT_PROTOCOL *PhysGop, AMI_LIST *ModeList)
{
    EFI_STATUS Status;
    PIXEL_RES_LIST *ResolutionMode;
    UINT32  Idx;
    UINTN InfoSize;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
    
    if ((ModeList == NULL) || (PhysGop == NULL))
    {
        return EFI_INVALID_PARAMETER;
    }
    AmiListInit(ModeList);
    // Query all modes for the device and generate a list of modes to use.
    for (Idx = 0; Idx < PhysGop->Mode->MaxMode; Idx++)
    {
        // get mode from current device, by using QueryMode
        Status = PhysGop->QueryMode(PhysGop, Idx, &InfoSize, &Info);
        if (EFI_ERROR(Status))
        {
            continue;
        }
         ResolutionMode = (PIXEL_RES_LIST *)AllocatePool(sizeof(PIXEL_RES_LIST));
        if (ResolutionMode == NULL)
        {
            // free the Info structure from QueryMode
            FreePool(Info);
            // delete mode list since we were unable to finish creating the list
            AmiListDeleteList(ModeList);
            return EFI_OUT_OF_RESOURCES;
        }
        
        // Fill out needed mode info
        ResolutionMode->Resolution.HorizontalRes = Info->HorizontalResolution;
        ResolutionMode->Resolution.VerticalRes = Info->VerticalResolution;
        ResolutionMode->Resolution.ModeNumber = Idx;
        // free memory allocated by QueryMode call
        FreePool(Info);

        // Add new GOP device to our tracking list
        AmiListAdd(ModeList, &(ResolutionMode->Link));

    }  
    return EFI_SUCCESS;
}

VOID PrintModeInfoFromGop(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop)
{
    DEBUG((DEBUG_ERROR, "Gop = %X\n\r", Gop));
    if (Gop != NULL)
    {
        DEBUG((DEBUG_ERROR, "Printing Mode Information\n\r"));
        DEBUG((DEBUG_ERROR, "MaxMode = %d\n\r", Gop->Mode->MaxMode));
        DEBUG((DEBUG_ERROR, "Mode = %d\n\r", Gop->Mode->Mode));
        DEBUG((DEBUG_ERROR, "Horizontal Resolution = %d\n\r", Gop->Mode->Info->HorizontalResolution));
        DEBUG((DEBUG_ERROR, "Vertical Resolution = %d\n\r\n\r", Gop->Mode->Info->VerticalResolution));
    }
}

/**
    This function tries to find common video resolutions between the new GOP device and 
    the current devices handled by the Graphics Splitter
    
  @param PhysicalGopDevice - new GOP device 
  @return EFI_STATUS
  @retval EFI_SUCCESS - common modes found between the new device and the current devices handled by the system
  @retval EFI_UNSUPPORTED - no matching modes found 
  
 */
EFI_STATUS ConfigureGopModes( EFI_GRAPHICS_OUTPUT_PROTOCOL *PhysicalGopDevice, BOOLEAN *ModeChange)
{
    AMI_LINK *ListPtr = NULL; 
    AMI_LINK *TempPtr = NULL;
    AMI_LINK *NewModeListPtr = NULL;
    PIXEL_RES_LIST *Mode = NULL;
    PIXEL_RES_LIST *NewMode = NULL;
    UINT32 Idx;
    BOOLEAN Deleted = FALSE;

    if (ModeChange != NULL) *ModeChange = FALSE;

    // Check if this is the first GOP device
    if (AmiListEmpty(&GopModes))
    {
        CreateResolutionList(PhysicalGopDevice, &GopModes);
        
        // We're connecting first physical device to the graphics splitter.
        // Since we don't know which mode will be used by the GOP consumers,
        // just use the current mode of the physical GOP to avoid unneeded mode changes.
        // Unless the saveScreen buffer exists.  Then use the resolution of that buffer.
        GraphicsSplitter.Mode->MaxMode = (UINT32)GopModes.Size;
        if ( SaveScreenInfo.Buffer == NULL)        
        {
            DEBUG((DEBUG_INFO, "ConfigureGopModes: SaveScreenInfo is NULL \n\r"));
            
            if ((PhysicalGopDevice->Mode->Info->HorizontalResolution == GraphicsSplitter.Mode->Info->HorizontalResolution )
                && (PhysicalGopDevice->Mode->Info->VerticalResolution == GraphicsSplitter.Mode->Info->VerticalResolution))
            {
                if (ModeChange != NULL) *ModeChange = FALSE;
            }
            else 
            {
                if (ModeChange != NULL) *ModeChange = TRUE;
            }

            GraphicsSplitter.Mode->Mode = PhysicalGopDevice->Mode->Mode;
            GraphicsSplitter.Mode->Info->HorizontalResolution = PhysicalGopDevice->Mode->Info->HorizontalResolution;
            GraphicsSplitter.Mode->Info->VerticalResolution = PhysicalGopDevice->Mode->Info->VerticalResolution;
        }
        else 
        {
            DEBUG((DEBUG_INFO, "ConfigureGopModes: SaveScreenInfo is valid\n\r"));
            // Find the mode in the list that matches the SaveScreenInfo
            ListPtr = GopModes.pHead;
            while ( ListPtr != NULL)
            {
                // go through the GopModes list and find the matching mode in the current mode list
                Mode = BASE_CR(ListPtr, PIXEL_RES_LIST, Link);
                ListPtr = ListPtr->pNext;
                if ((SaveScreenInfo.HorizontalResolution == Mode->Resolution.HorizontalRes )
                        && (SaveScreenInfo.VerticalResolution == Mode->Resolution.VerticalRes))
                {
                    // once we find a matching mode, check to see if the Physical GOP device is in the same mode.
                    if ((SaveScreenInfo.HorizontalResolution == GraphicsSplitter.Mode->Info->HorizontalResolution )
                        && (SaveScreenInfo.VerticalResolution == GraphicsSplitter.Mode->Info->VerticalResolution))
                    {
                        if (ModeChange != NULL) *ModeChange = FALSE;
                    }
                    else 
                    {
                        if (ModeChange != NULL) *ModeChange = TRUE;
                    }
// do we set the mode here or later when we draw the screen                        
                    GraphicsSplitter.Mode->Mode = Mode->Resolution.ModeNumber;
                    GraphicsSplitter.Mode->Info->HorizontalResolution = Mode->Resolution.HorizontalRes;
                    GraphicsSplitter.Mode->Info->VerticalResolution = Mode->Resolution.VerticalRes;
                    
                    DEBUG((DEBUG_VERBOSE, "ConfigureGopModes: Use Information from SaveScreenInfo:  HR = %d  VR = %d\n\r", 
                            Mode->Resolution.HorizontalRes, Mode->Resolution.VerticalRes));
                    break;
                }
            }
            DirtyMode = TRUE;
        }
    }
    // If not the first GOP device go here
    else
    {
        // Get all the modes of the current device 
        //TODO: Check Status
        CreateResolutionList(PhysicalGopDevice, &NewModeList);

        // Check if physical GOP supports at least one mode from the splitter mode list
        NewModeListPtr = NewModeList.pHead;
        while (NewModeListPtr != NULL)
        {
            NewMode = BASE_CR(NewModeListPtr, PIXEL_RES_LIST, Link);
            // check if the new mode resolution is in GopModes.  
            if (IsModeInList(&GopModes, NewMode, NULL))
                break; // If it is in the list exit the loop

            NewModeListPtr = NewModeListPtr->pNext;
        }
        
        // if no matching mode is found, delete the new mode list and exit with an error to say we don't support this device
        if (NewModeListPtr == NULL)
        {   // if no matches clear the list and return unsupported
            AmiListDeleteList(&NewModeList);
            return EFI_UNSUPPORTED;
        }

        // Matching mode(s) found 
        // Remove Modes that are not in both lists
        Deleted = FALSE;
        ListPtr = GopModes.pHead;
        while ( ListPtr != NULL)
        {
            // go through the GopModes list and find the matching mode in the current mode list
            // if it doesn't match remove it from the list
            Mode = BASE_CR(ListPtr, PIXEL_RES_LIST, Link);
            TempPtr = ListPtr;
            ListPtr = ListPtr->pNext;

            if (IsModeInList(&NewModeList, Mode, &NewMode))
            {
                // Check if this mode is the current mode of the system
                // If it is, set the mode on the new GOP device
                if ((GraphicsSplitter.Mode->Info->HorizontalResolution == NewMode->Resolution.HorizontalRes )
                        && (GraphicsSplitter.Mode->Info->VerticalResolution == NewMode->Resolution.VerticalRes))
                {
                    DEBUG((DEBUG_VERBOSE,"\n\r resetting secondary video device mode to H-Res = %d    V-Res = %d\n\r", NewMode->Resolution.HorizontalRes, NewMode->Resolution.VerticalRes));
                    PhysicalGopDevice->SetMode(PhysicalGopDevice, NewMode->Resolution.ModeNumber);
                }
            }
            else
            {
                // This mode does not match, delete it from the list
                // check if this is the current mode for the system
                if ((Mode->Resolution.HorizontalRes == GraphicsSplitter.Mode->Info->HorizontalResolution )
                        && (Mode->Resolution.VerticalRes == GraphicsSplitter.Mode->Info->VerticalResolution ))
                {
                    // this is the current mode 
                    // change the mode number to a value that not real so the mode will be fixed later
                    GraphicsSplitter.Mode->Mode = 0xFFFF;
                }
                DEBUG((DEBUG_VERBOSE, "ConfigureModeList: Deleted a mode, setting Deleted and dirty."));
                Deleted = TRUE; // trigger a renumbering of the modes since we are deleting one
                DirtyMode = TRUE;

                AmiListDelete(&GopModes, TempPtr);
                FreePool(TempPtr);
            }
        }
//==========================================================================
        // Update Mode Numbers
        Idx = 0;
        ListPtr = GopModes.pHead;
        while ( ListPtr != NULL)
        {
            Mode = BASE_CR(ListPtr, PIXEL_RES_LIST, Link);
            Mode->Resolution.ModeNumber = Idx++;

            // update the GraphicsSplitter current Mode number.  The previous mode number may not be valid anymore
            if ((Mode->Resolution.HorizontalRes == GraphicsSplitter.Mode->Info->HorizontalResolution) && 
                    (Mode->Resolution.VerticalRes == GraphicsSplitter.Mode->Info->VerticalResolution))
            {
                GraphicsSplitter.Mode->Mode = Mode->Resolution.ModeNumber;
            }
            ListPtr = ListPtr->pNext;
        }    
        // update the max mode due to changes in this function
        GraphicsSplitter.Mode->MaxMode = (UINT32)GopModes.Size;
//==========================================================================
 
        // clear the mode list that was created for the new GOP device we found 
        AmiListDeleteList(&NewModeList);
    }

    
    // A mode was deleted from the list and it needs to be renumbered
    if (Deleted == TRUE)
    {
        // if the current mode is deleted fix this
        if (GraphicsSplitter.Mode->Mode == 0xFFFF)
        {
            DEBUG((DEBUG_INFO, "ConfigureGopModes: Current mode was deleted.  Choose a new mode\n\r"));
            
            // Choose a new mode since the previous mode used was deleted and the new one should be reassigned.
            // Since we don't know which mode will be used by the GOP consumers,
            // just use mode 0.
            Mode = BASE_CR(GopModes.pHead, PIXEL_RES_LIST, Link);
            GraphicsSplitter.Mode->Mode = 0;
            GraphicsSplitter.Mode->Info->HorizontalResolution = Mode->Resolution.HorizontalRes;
            GraphicsSplitter.Mode->Info->VerticalResolution = Mode->Resolution.VerticalRes;
            // The physical devices are not in a mode corresponding to splitter mode zero.
            // To switch them we need a SetMode call, however, we would like to avoid unneeded mode changes
            // (they cause screen flickering and take time to execute).
            // Once this function returns control to a driving binding start or stop routine, splitter GOP will be reinstalled
            // and GOP consumer will likely call SetMode anyway. So switching physical GOP modes at this point would be a waste.
            // Having said that, we still have to make sure splitter GOP instance works properly before consumer calls SetMode
            // (or if SetMode call will not be called). We're doing it by performing a "lazy mode switch"
            //(set a dirty flag now and change the mode when (and if) it is actually necessary).
            DirtyMode = TRUE;
        }
        
        if (ModeChange != NULL) *ModeChange = TRUE;
    }


    DEBUG((DEBUG_INFO, "ConfigureGopModes: Graphics Splitter Mode List\n\r"));
    PrintModeInfoFromGop(&GraphicsSplitter);
    return EFI_SUCCESS;
}





/**
    This function goes through the mode list of the passed in GOP and finds the mode that matches the 
    requested Horizontal Resolution and Vertical Resolution.  The this mode is set for this device.
 */
EFI_STATUS 
VerifyAndSetMode( EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, UINT32 HorRes, UINT32 VertRes, OPTIONAL BOOLEAN *Match)
{
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *pInfo = NULL;
    UINT32 InfoHorRes, InfoVertRes;
    UINTN Size;
    UINT32 Idx;

    DEBUG((DEBUG_INFO, "VerifyAndSetMode: Enter  Gop = %X\n\r",Gop));

    if (Gop->Mode->Info->HorizontalResolution  == HorRes && Gop->Mode->Info->VerticalResolution == VertRes)
    {
        DEBUG((DEBUG_INFO, " The current mode of the Physical GOP is correct!  \n\r Set mode to clear screen and exit \n\r"));
        DEBUG((DEBUG_VERBOSE, " Phys:  HorRes = %d   VerRes = %d\n\r", Gop->Mode->Info->HorizontalResolution,  Gop->Mode->Info->VerticalResolution));
        DEBUG((DEBUG_VERBOSE, " Splitter:  HorRes = %d   VerRes = %d\n\r", HorRes, VertRes));

        if (Match != NULL)
            *Match = TRUE;
        
        DEBUG((DEBUG_INFO, " VerifyAndSetMode call to Gop->SetMode:  Mode # = %d\n\r", Gop->Mode->Mode));
        Gop->SetMode(Gop, Gop->Mode->Mode); // clear the screen
        return EFI_SUCCESS;
    }

    
    for (Idx = 0; Idx < Gop->Mode->MaxMode; Idx++)
    {
        // get info on the current mode
        Status = Gop->QueryMode(Gop, Idx, &Size, &pInfo );
        // if we have an error move on to the next mode
        if (EFI_ERROR(Status))
            continue;

        InfoHorRes = pInfo->HorizontalResolution;
        InfoVertRes = pInfo->VerticalResolution;
        // free memory allocated in QueryMode
        FreePool(pInfo);
        if ((InfoHorRes == HorRes) && (InfoVertRes == VertRes ))
        {
            DEBUG((DEBUG_VERBOSE, " VerifyAndSetMode 2:  Mode # = %d\n\r", Idx));
            DEBUG((DEBUG_INFO, " Found a matching mode.  set it and exit\n\r"));
            // found the mode.  Set it
            Status = Gop->SetMode(Gop, Idx);
            if ( Match != NULL)
                *Match = TRUE;
            
            return Status;
        }
    }

    DEBUG((DEBUG_INFO, " No Modes Match.  Exit\n\r"));

    if (Match != NULL) *Match = FALSE;
    return EFI_UNSUPPORTED;
}

//=========================================================================================
//=========================================================================================
// Graphics Splitter Protocol Functions

/**
    This function returns a structure that contains information about the requested mode number
    
    @param This - pointer to the GOP Splitters GOP protocol
    @param ModeNumber - the mode that information is requested about
    @param SizeOfInfo - size of the Info structure
    @param Info - Pointer that will hold the information on the requested mode
    
    @return EFI_STATUS
    @retval EFI_INVALID_PARAMETER - some paramter is incorrect
    @retval EFI_OUT_OF_RESOURCES - not enough memory to create the Info structure to return the mode information
    @retval EFI_SUCCESS - Mode info found and returned to the caller
 */
EFI_STATUS EFIAPI 
GraphicsSplitter_QueryMode (
        IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
        IN UINT32 ModeNumber,
        OUT UINTN *SizeOfInfo,
        OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info)
{
    AMI_LINK *ListPtr;
    UINT32 Count;
    PIXEL_RES_LIST *Mode;
    EFI_TPL OldTpl;


    // Check parameters
    if ((This == NULL) || (Info == NULL) || (This->Mode == NULL) || (ModeNumber >= This->Mode->MaxMode) || (SizeOfInfo == NULL))
        return EFI_INVALID_PARAMETER;

    // allocate the memory for the Info structure to be passed back to the caller
    *Info = (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *) AllocatePool(sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
    if (*Info == NULL)
    {
        return EFI_OUT_OF_RESOURCES;
    }

    ListPtr = GopModes.pHead;
    Count = 0;
    while ( Count < ModeNumber && ListPtr != NULL)
    {
        ListPtr = ListPtr->pNext;
        Count++;
    }
    if (ListPtr == NULL) return EFI_INVALID_PARAMETER;
    
    Mode = BASE_CR(ListPtr, PIXEL_RES_LIST, Link);

    // This code is not reentrant and we don't want it to be interrupted
    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);
    
    // fill in Mode Info data structure
    (*Info)->Version = GraphicsSplitterInfo.Version;
    (*Info)->HorizontalResolution = Mode->Resolution.HorizontalRes;
    (*Info)->VerticalResolution = Mode->Resolution.VerticalRes;
    (*Info)->PixelFormat = PixelBltOnly;
    (*Info)->PixelsPerScanLine = (*Info)->HorizontalResolution;
    (*Info)->PixelInformation.BlueMask = 0;
    (*Info)->PixelInformation.RedMask = 0;
    (*Info)->PixelInformation.GreenMask = 0;
    (*Info)->PixelInformation.ReservedMask = 0;

    
    // define the size of the structure
    *SizeOfInfo = sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
    
    // Reset TPL back to the original value
    gBS->RestoreTPL(OldTpl);
    
    return EFI_SUCCESS;
}




/**
    This function set a new mode for the system
    
    @param This - pointer to the GOP protocol 
    @param ModeNumber - mode number to set the GOP device to
    
    @return EFI_STATUS 
    @retval EFI_INVALID_PARAMETER - invalid paramater passed in
    @retval EFI_SUCCESS - all GOP devices set to new mode
 */
EFI_STATUS EFIAPI 
GraphicsSplitter_SetMode(
        IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
        IN UINT32 ModeNumber)
{
    AMI_LINK *ListPtr;
    UINT32 HorRes, VerRes;
    UINT32 Count;
    PIXEL_RES_LIST *Mode;
    EFI_TPL OldTpl;
    BOOLEAN ModeChange = FALSE;

    DEBUG((DEBUG_INFO, "GraphicsSplitter_SetMode: Enter\n\r"));
    // Check parameters
    if (ModeNumber >= GraphicsSplitter.Mode->MaxMode)
    {
        return EFI_UNSUPPORTED;
    }

    DEBUG((DEBUG_INFO, "GS_SetMode:  new mode = %d\n\r", ModeNumber));
    
    // Go through Graphics Splitter's Mode list to find the requested mode number.
    ListPtr = GopModes.pHead;
    Count = 0;
    while ( Count < ModeNumber && ListPtr != NULL)
    {
        ListPtr = ListPtr->pNext;
        Count++;
    }
    if (ListPtr == NULL) return EFI_INVALID_PARAMETER;

    // get the Horizontal and Vertical resolutions of this mode
    Mode = BASE_CR(ListPtr, PIXEL_RES_LIST, Link);
    HorRes = Mode->Resolution.HorizontalRes;
    VerRes = Mode->Resolution.VerticalRes;

    if (( This->Mode->Mode == ModeNumber) && (This->Mode->Info->HorizontalResolution == HorRes) && (This->Mode->Info->VerticalResolution == VerRes))
    {        //this is the current mode.  
        ModeChange = FALSE;
    }
    else
    {
        ModeChange = TRUE;
    }
            



    DEBUG((DEBUG_VERBOSE, "GS_SetMode:  HR = %d   VR = %d\n\r", HorRes, VerRes));
    // This code is not reentrant and we don't want it to be interrupted
    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);

    // Set the Mode information for the Graphics Splitter based on the new mode
    This->Mode->Mode = ModeNumber;
    This->Mode->Info->HorizontalResolution = HorRes;
    This->Mode->Info->VerticalResolution = VerRes;
    This->Mode->Info->PixelInformation.BlueMask = 0;
    This->Mode->Info->PixelInformation.GreenMask = 0;
    This->Mode->Info->PixelInformation.RedMask = 0;
    This->Mode->Info->PixelInformation.ReservedMask = 0;
    This->Mode->Info->PixelsPerScanLine = HorRes;

    // Go through the list of GOP devices and for each one
    // find the mode that best corresponds to the requested Mode 
    ListPtr = GopList.pHead;
    while ( ListPtr != NULL)
    {
        BOOLEAN Match;
        GOP_DEVICE *GopDev = NULL;
        
        Match = FALSE;
        GopDev = BASE_CR(ListPtr, GOP_DEVICE, Link);

        // This uses the mode number passed in to find the common mode number in the 
        //  Graphics Splitter's mode list. Then use that mode information to find 
        //  the actual mode number of the GOP device that matches the common mode 
        //  and set the device to that mode
        DEBUG((DEBUG_INFO, "GraphicsSplitter_SetMode: call VerifyAndSetMode for current GOP device\n\r"));
        VerifyAndSetMode(GopDev->Gop, HorRes, VerRes, &Match);

        if (Match == FALSE)
        {
            DEBUG((DEBUG_WARN, "No modes of this device were found to match the requested mode.\n\r  This should not happen!!\n\r"));
            ASSERT(Match == TRUE);
        }
        // Get the next GOP device in the list
        ListPtr = ListPtr->pNext;
    }

    // We have just set the mode. Reset the dirty flag.
    // Refer to a comment block towards the end of ConfigureGopModes function
    // for additional details about the dirty flag.
    DirtyMode = FALSE;
    // Reset TPL back to the original value
    gBS->RestoreTPL(OldTpl);

    // provide notification that SetMode has been called with a valid display mode 
    if (ModeChange == TRUE)
    {
        DEBUG((DEBUG_INFO, "GraphicsSplitter_SetMode: mode changed reinstall Mode change notification\n\r"));
        gBS->ReinstallProtocolInterface( GopSplitterHandle,
                                     &gAmiGraphicsSplitterModeChangeProtocolGuid, 
                                     &gModeChanged,
                                     &gModeChanged);
    }
    return EFI_SUCCESS;
}



/**
    This function passes the BLT command on to each GOP device handled by the Graphics Splitter
    
  @param  This         Protocol instance pointer.
  @param  BltBuffer    The data to transfer to the graphics screen.
                       Size is at least Width*Height*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL).
  @param  BltOperation The operation to perform when copying BltBuffer on to the graphics screen.
  @param  SourceX      The X coordinate of source for the BltOperation.
  @param  SourceY      The Y coordinate of source for the BltOperation.
  @param  DestinationX The X coordinate of destination for the BltOperation.
  @param  DestinationY The Y coordinate of destination for the BltOperation.
  @param  Width        The width of a rectangle in the blt rectangle in pixels.
  @param  Height       The height of a rectangle in the blt rectangle in pixels.
  @param  Delta        Not used for EfiBltVideoFill or the EfiBltVideoToVideo operation.
                       If a Delta of zero is used, the entire BltBuffer is being operated on.
                       If a subrectangle of the BltBuffer is being used then Delta
                       represents the number of bytes in a row of the BltBuffer.

  @retval EFI_SUCCESS           BltBuffer was drawn to the graphics screen.
  @retval EFI_INVALID_PARAMETER BltOperation is not valid.
  @retval EFI_DEVICE_ERROR      The device had an error and could not complete the request.
 */
EFI_STATUS EFIAPI 
GraphicsSplitter_Blt (
        IN EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
        IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer, OPTIONAL
        IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION BltOperation,
        IN UINTN SourceX,
        IN UINTN SourceY,
        IN UINTN DestinationX,
        IN UINTN DestinationY,
        IN UINTN Width,
        IN UINTN Height,
        IN UINTN Delta OPTIONAL)
{
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_STATUS Return = EFI_SUCCESS;
    AMI_LINK *ListPtr = NULL;
    GOP_DEVICE *Gop = NULL;
    EFI_TPL OldTpl;

    DEBUG((DEBUG_INFO, "GraphicsSplitter : enter Blt function\n\r"));
    
    if (BltOperation >= EfiGraphicsOutputBltOperationMax)
        return EFI_INVALID_PARAMETER;
    
    // The dirty flag is set, physical GOP devices have to be switch to a proper mode. 
    // Perform a SetMode call (dirty flag is reset by the SetMode).
    // Refer to a comment block towards the end of ConfigureGopModes function
    // for additional details about the dirty flag.
    if (DirtyMode)
    {
        Status = GraphicsSplitter_SetMode(This, GraphicsSplitter.Mode->Mode);
        if (EFI_ERROR(Status)) return Status;
    }
    
    // This code is not reentrant and we don't want it to be interrupted
    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);

    ListPtr = GopList.pHead;
    if ((ListPtr == NULL) && (SaveScreenInfo.Buffer != NULL))
    {
        DEBUG((DEBUG_ERROR, " calling GS_BltNoGopDevice\n\r"));
        Return = GS_BltNoGopDevice(&SaveScreenInfo, 
                            BltBuffer, 
                            BltOperation,
                            SourceX,
                            SourceY,
                            DestinationX,
                            DestinationY,
                            Width,
                            Height,
                            Delta);
    }
    else
    {
        while ( ListPtr != NULL)
        {
            Gop = BASE_CR(ListPtr, GOP_DEVICE, Link);
    
            // Call the Blt function for each GOP device
            Status = Gop->Gop->Blt(Gop->Gop, 
                            BltBuffer, 
                            BltOperation,
                            SourceX,
                            SourceY,
                            DestinationX,
                            DestinationY,
                            Width,
                            Height,
                            Delta);
            
            if (EFI_ERROR(Status))
            {
                Return = Status;
            }
            else if (BltOperation == EfiBltVideoToBltBuffer)
            {
                // Reset TPL back to the original value
                gBS->RestoreTPL(OldTpl);
                return EFI_SUCCESS;
            }
            
            ListPtr = ListPtr->pNext;
        }
    }

    // Reset TPL back to the original value
    gBS->RestoreTPL(OldTpl);

    return Return;
}

//=========================================================================================
//=========================================================================================
// Supporting functions that provide the virtual buffer that is used when no physical GOP 
//  devices are installed

/**
    Draw a buffer to the GOP device that is passed in.
*/
VOID EFIAPI
DrawScreen( EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput, 
            EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Buffer, 
            UINT32 HorRes, 
            UINT32 VertRes)
{
    EFI_STATUS Status;
    
    // Check for invalid parameters and return if any are bad
    if ((GraphicsOutput == NULL) || (Buffer == NULL) || (HorRes == 0) || (VertRes == 0))
        return ;


    DEBUG((DEBUG_INFO, "Draw the screen now\n\r"));
    Status = GraphicsOutput->Blt(
                    GraphicsOutput,
                    Buffer,
                    EfiBltBufferToVideo,
                    0, // beginning of the buffer
                    0, // beginning of the buffer
                    0,
                    0,
                    HorRes,
                    VertRes,
                    0  // filling entire screen
                    );
    if (EFI_ERROR(Status))
    {
        DEBUG((DEBUG_ERROR, "BLT function returned ERROR = %r\n\r", Status));
    }
    else
    {
        DEBUG((DEBUG_ERROR, "BLT function completed\n\r"));
    }
}



/**
    Used the saved screen buffer to redraw the screen when the first GOP device is reattached
*/
EFI_STATUS EFIAPI
RedrawSaveScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput)
{
    BOOLEAN FoundTrue = FALSE;
    UINT32 HorRes, VertRes;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Buffer = NULL;

    if ((GraphicsOutput == NULL) || (SaveScreenInfo.Buffer == NULL))
    {
        return EFI_INVALID_PARAMETER;
    }

    DEBUG((DEBUG_VERBOSE, "Redraw Screen: Push SaveScreenInfo out to the screen \n\r"));
    // Find the mode of the physical device that matches the saved resolution.
    // This function is called from the driver binding start routine when first physical GOP is added.
    // The first GOP is not automatically switched to splitter's mode (ConfigureGopModes returns no mode change).
    // That is why we need to switch physical GOP to a correct mode here.
    //TODO: check the status
    Buffer = SaveScreenInfo.Buffer;
    HorRes = SaveScreenInfo.HorizontalResolution;
    VertRes = SaveScreenInfo.VerticalResolution;
    
    DEBUG((DEBUG_VERBOSE, "Redraw Screen: SaveScreenInfo Verify and Set Mode\n\r"));
    DEBUG((DEBUG_VERBOSE, "Redraw Screen: Splitter Before HR = %d   VR = %d\n\r", HorRes, VertRes));
    DEBUG((DEBUG_VERBOSE, "RedrawScreen: SetMode Call 2\n\r"));
    VerifyAndSetMode(GraphicsOutput, HorRes, VertRes, &FoundTrue);
    DEBUG((DEBUG_VERBOSE, "Redraw Screen: Phys GOP After  HR = %d   VR = %d\n\r", GraphicsOutput->Mode->Info->HorizontalResolution, GraphicsOutput->Mode->Info->VerticalResolution));

    if (FoundTrue)
    {
        
        DEBUG((DEBUG_ERROR, "Redraw Screen: Calling DrawScreen\n\r"));
        DrawScreen( GraphicsOutput, Buffer, HorRes, VertRes);
    }

    //Free buffer, set pointer to NULL and clear resolution info
    if (Buffer != NULL)
    {
        FreePool(Buffer);

        // Put cursor back
        gST->ConOut->SetCursorPosition(gST->ConOut, SaveScreenInfo.XAxis, SaveScreenInfo.YAxis);
        SaveScreenInfo.Buffer = NULL;
        SaveScreenInfo.HorizontalResolution = 0;
        SaveScreenInfo.VerticalResolution = 0;
        SaveScreenInfo.XAxis = 0;
        SaveScreenInfo.YAxis = 0;
    }
    
    return EFI_SUCCESS;
}




/**
    This function is called when the last physical GOP devic is disconnected.  
    It saves the screen to a buffer and sets the system up to maintain the video
    buffer.

    @param GraphicsOutput - A pointer to the last physical GOP device.

 */
EFI_STATUS EFIAPI
SaveScreen (EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput)
{
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info = NULL;
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN BufferSize;
    
   
    // get the information about the current mode
    Info = GraphicsOutput->Mode->Info;
    DEBUG((DEBUG_ERROR, "Saving Buffer from BLT\n\r"));
    DEBUG((DEBUG_VERBOSE, "Buffer Resolution : HorRes = %d   VertRes = %d\n\r", Info->HorizontalResolution, Info->VerticalResolution));

    // if successful, allocate a buffer big enough for the screen
    BufferSize = (UINTN)MultU64x32(MultU64x32(Info->HorizontalResolution, Info->VerticalResolution), sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
    SaveScreenInfo.Buffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) AllocatePool (BufferSize);
                
    if (SaveScreenInfo.Buffer != NULL)
    {
        // if memory is correctly allocated, save the screen data and copy the screen
        SaveScreenInfo.HorizontalResolution = Info->HorizontalResolution;
        SaveScreenInfo.VerticalResolution = Info->VerticalResolution;
        SaveScreenInfo.YAxis = gST->ConOut->Mode->CursorRow;
        SaveScreenInfo.XAxis = gST->ConOut->Mode->CursorColumn;
        // save the screen into the buffer
        Status = GraphicsOutput->Blt(
                        GraphicsOutput,
                        SaveScreenInfo.Buffer,
                        EfiBltVideoToBltBuffer,
                        0, // beginning of the buffer
                        0, // beginning of the buffer
                        0, // Destination X
                        0, // Destination Y
                        SaveScreenInfo.HorizontalResolution,
                        SaveScreenInfo.VerticalResolution,
                        0  // filling entire screen
                        );
        if (EFI_ERROR(Status))
        {
            DEBUG((DEBUG_ERROR, "BLT function returned ERROR = %r\n\r", Status));
        }
        else
            DEBUG((DEBUG_ERROR, "BLT function completed\n\r"));
    }
    
    return Status;
}

/**
    This function processes calls to the BLT function when no Physical GOP devices are attached
    The function is designed to maintain a video buffer for reposting once a GOP device is 
    reattached
    
    @param This - pointer to the structure that contains needed info about the BLT buffer when no physical GOP is available
    @param BltBuffer - Buffer used to pass information in or out based on the Blt Operation 
    @param BltOperation - defines the operation for the function to perform 
    @param SourceX - the X coordinate of the source for the BLT operation (X, Y)
    @param SourceY - the Y coordinate of the source for the BLT operation (X, Y)
    @param DestinationX - the X coordinate of the destination for the BLT operation (X, Y)
    @param DestinationY - the Y coordinate of the destination for the BLT operation (X, Y)
    @param Width - the width of the rectangle being operated on
    @param Height - the height of the rectangle being operated on
    @param Delta - optional - the number of bytes in a row of the BLT buffer 
        
    
 */
EFI_STATUS EFIAPI
GS_BltNoGopDevice(
        IN  SAVE_SCREEN_INFO *This,
        IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer, OPTIONAL
        IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION BltOperation,
        IN UINTN SourceX,
        IN UINTN SourceY,
        IN UINTN DestinationX,
        IN UINTN DestinationY,
        IN UINTN Width,
        IN UINTN Height,
        IN UINTN Delta OPTIONAL)
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINT32 Data;
    UINTN Size;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *ScreenBuffer;
    EFI_TPL OldTpl;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *TempBuffer;
    UINTN PixelSize = sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL);
    UINTN LineLength;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Source, *Destination, *End;
    UINT32 HorizontalResolution;
    
    // This is actually a pointer to the buffer we created to capture the things drawn to screen 
    //  since the last Physical GOP device was uninstalled
    if (This == NULL || This->Buffer == NULL || BltBuffer == NULL)
    {
        return EFI_INVALID_PARAMETER;
    }
    
    // UEFI Spec parameter check first.  The operation should not be greater than or equal to EfiGraphicsOutputBltOperationMax
    if (BltOperation >= EfiGraphicsOutputBltOperationMax)
        return EFI_INVALID_PARAMETER;
    
    // Now check that the BLT buffer fits within the screen
    if (Width == 0 || Height == 0) 
    {
        return EFI_INVALID_PARAMETER;
    }

     // Since this is for a virtual buffer, both Source and destination should be checked

    // prep for actions on the buffer
    ScreenBuffer = This->Buffer;
    LineLength = Width * PixelSize;
    HorizontalResolution = This->HorizontalResolution;
    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);
    
        
    // Do the work for each BLT Operation
    switch (BltOperation)
    {
        case EfiBltVideoFill:
            DEBUG((DEBUG_INFO, "EfiBltVideoFill\n\r"));
            // Write data from the BltBuffer pixel (0,0) directly to every pixel of the video display 
            // rectangle (DestinationX, DestinationY) (DestinationX + Width, DestinationY + Height). 
            // Only one pixel will be used from the BltBuffer. Delta is NOT used.

            //TODO: Validate input parameters
            
            
            
            // Step 1: read data to write to the entire buffer
            Data = *( (UINT32*)(BltBuffer));
            
            Destination = ScreenBuffer + HorizontalResolution * DestinationY + DestinationX;
            End = Destination + HorizontalResolution * Height;
            
            // Step 2: loop through the specified buffer area and write the data that was read in Step 1 to the buffer
            // One line at a time 
            for (; Destination < End; Destination += HorizontalResolution)
            {
                // This should index into the video Buffer 
                // DEstination - the video buffer we are using to temporaily hold what the screen looks like
                //                  this uses the following parameters to determine where to write the info
                //      DestinationY - the number of lines down to start 
                //      Idy - the Index for loop that determines the number of pixel lines to copy
                //      This->HorizontalResolution - The number of pixels in a row of BufferPtr
                //      DestinationX - the number of pixels into a row where to start copying
                // LineLength - is the number of pixels to copy for each row
                // Data - this is the Pixel info copy into the Buffer
                SetMem32(Destination, LineLength, Data);
            }
            ASSERT(End <= ScreenBuffer + This->HorizontalResolution * This->VerticalResolution);
            break;  // End case EfiBltVideoFill

        case EfiBltVideoToBltBuffer:
            DEBUG((DEBUG_INFO, "EfiBltVideoToBltBuffer\n\r"));

            // Read data from the video display rectangle (SourceX, SourceY) (SourceX + Width, SourceY + Height) 
            // and place it in the 
            // BltBuffer rectangle (DestinationX, DestinationY ) (DestinationX + Width, DestinationY + Height). 
            // If DestinationX or DestinationY is not zero then Delta must be set to the length in bytes of a 
            // row in the BltBuffer.
            
            //TODO: Validate input parameters
            if (Delta==0)
            {
                Delta = LineLength;
            } 
            else if (Delta < LineLength)
            {
                DEBUG((DEBUG_ERROR, "ERROR: The Delta (width of the BLT Buffer) is smaller than the width of the rectangle that is to be copied\n\r "));
                return EFI_INVALID_PARAMETER;
            }

            Source = ScreenBuffer + HorizontalResolution * SourceY + SourceX;
            End = Source + HorizontalResolution * Height;
            Destination = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)((UINT8*)BltBuffer + Delta * DestinationY);
            Destination += DestinationX;
            // Step 2: Copy from the Video Buffer(BufferPtr) to the BltBuffer
            for (; Source < End
                 ; Source += HorizontalResolution, Destination = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)((UINT8*)BltBuffer + Delta))
            {
                CopyMem(Destination,Source,LineLength);
            }
            ASSERT(Source <= ScreenBuffer + This->HorizontalResolution * This->VerticalResolution);
            ASSERT(End <= ScreenBuffer + This->HorizontalResolution * This->VerticalResolution);
            break; // end case EfiBltVideoToBltBuffer
            
        case EfiBltBufferToVideo:
            DEBUG((DEBUG_INFO, "EfiBltBufferToVideo\n\r"));

            // Write data from the BltBuffer rectangle (SourceX, SourceY) (SourceX + Width, SourceY + Height) 
            // directly to the video display rectangle (DestinationX, DestinationY) (DestinationX + Width, DestinationY + Height). 
            // If SourceX or SourceY is not zero then Delta must be set to the length in bytes of a row in the BltBuffer.

            // Step 1: Check BLT Buffer and return if NULL 
            if (Delta==0)
            {
                Delta = LineLength;
            } 
            else if (Delta < LineLength)
            {
                DEBUG((DEBUG_ERROR, "ERROR: The Delta (width of the BLT Buffer) is smaller than the width of the rectangle that is to be copied\n\r"));
                return EFI_INVALID_PARAMETER;
            }
            Source = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)((UINT8*)BltBuffer + Delta * SourceY);
            Source += SourceX;
            Destination = ScreenBuffer + HorizontalResolution * DestinationY + DestinationX;
            End = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)((UINT8*)Source + Delta * Height);
            // Step 2: Copy from the Blt Buffer to the Video Buffer (BufferPtr)
            for (; Source < End
                 ; Source = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)((UINT8*)Source + Delta), Destination += HorizontalResolution)
            {
                CopyMem(Destination,Source,LineLength);
            }
            ASSERT(Destination <= ScreenBuffer + This->HorizontalResolution * This->VerticalResolution);
            break; // end case EfiBltBufferToVideo
            
        case EfiBltVideoToVideo:
            DEBUG((DEBUG_INFO, "EfiBltVideoToVideo\n\r"));

            // Copy from the video display rectangle (SourceX, SourceY) (SourceX + Width, SourceY + Height) 
            // to the video display rectangle(DestinationX, DestinationY) (DestinationX + Width, DestinationY + Height). 
            // The BltBuffer and Delta are not used in this mode. There is no limitation on the overlapping 
            // of the source and destination rectangles.
            
            //TODO: Validate input parameters
            
            // Step 1:  Allocate temporary buffer
            Size = Height * Width * PixelSize;
//            Status = gBS->AllocatePool (EfiBootServicesData, Size, (VOID**)&TempBuffer);
            TempBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)AllocatePool(Size);
            if (TempBuffer == NULL)
            {
                DEBUG((DEBUG_ERROR, "ERROR: Unable to allocate temporary buffer for copy\n\r"));
                return Status;
            }

            Source = ScreenBuffer + HorizontalResolution * SourceY + SourceX;
            Destination = ScreenBuffer + HorizontalResolution * DestinationY + DestinationX;
            
            // Step 2: copy data to TempBuffer
            Status = GS_BltNoGopDevice(
                This, TempBuffer, EfiBltVideoToBltBuffer,
                SourceX, SourceY,
                0, 0,
                Width,
                Height,
                0
            );
            
            //Step 3: copy data out to the new area of the Buffer
            if (!EFI_ERROR(Status)){
                Status = GS_BltNoGopDevice(
                    This, TempBuffer, EfiBltBufferToVideo,
                    0, 0,
                    DestinationX, DestinationY,
                    Width,
                    Height,
                    0
                );
            }
            
            // Step 4: Free allocated Temp buffer
//            gBS->FreePool(TempBuffer);
            FreePool(TempBuffer);
            break;  // end case EfiBltVideoToVideo
        default:
            break;
    }
    // restore original TPL level after screen operations are done
    gBS->RestoreTPL(OldTpl);
    return Status;
}


//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
