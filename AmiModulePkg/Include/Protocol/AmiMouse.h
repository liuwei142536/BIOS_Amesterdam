//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file AmiMouse.h
    Ami Mouse Driver protocol

    This Protocol can be used to get the both Simple Pointer and Absolute Pointer device
    current position. Using this Protocol can draw or hide the Mouse Pointer.
**/

#ifndef __AMI_MOUSE_PROTOCOL__H__
#define __AMI_MOUSE_PROTOCOL__H__

#include <Protocol/GraphicsOutput.h>

// Mouse protocol GUID
#define AMI_MOUSE_PROTOCOL_GUID \
        {0xc7a7030c, 0xc3d8, 0x45ee,{ 0xbe, 0xd9, 0x5d, 0x9e, 0x76, 0x76, 0x29, 0x53} }

// These are axillary protocols with NULL interface that are used to signal certain events.
#define AMI_SOFT_KEY_ACTIVATE_PROTOCOL_GUID \
        {0x1172381f, 0x7ae6, 0x4652,{ 0x8d, 0x85, 0xb6, 0x51, 0x69, 0x7b, 0xe3, 0xcf} }
#define AMI_SOFT_KEY_DEACTIVATE_PROTOCOL_GUID \
        {0xd3431c06, 0x2b4c, 0x4337,{ 0x93, 0x34, 0xff, 0xd9, 0xc0, 0x55, 0x15, 0x21} }
#define AMI_PROCESS_ESC_KEY_PROTOCOL_GUID \
        {0xd78ebe23, 0x7413, 0x43b9,{ 0x9d, 0x94, 0x44, 0x21, 0x6f, 0x4, 0xbb, 0x51} }
#define AMI_PROCESS_POST_KEY_PROTOCOL_GUID \
        {0x8a6e4fcf, 0x6e69, 0x4929,{ 0xa6,0x29, 0xa2, 0xca, 0xd6, 0xc5, 0xfc, 0x56} }

typedef struct _AMI_MOUSE_PROTOCOL AMI_MOUSE_PROTOCOL;

// For CLICK Event
#define     AMI_MOUSE_NULL_CLICK         0
#define     AMI_MOUSE_RIGHT_CLICK        1
#define     AMI_MOUSE_LEFT_CLICK         2
#define     AMI_MOUSE_MIDDLE_CLICK       3
#define     AMI_MOUSE_LEFT_DCLICK        4
#define     AMI_MOUSE_RIGHT_DOWN         5
#define     AMI_MOUSE_LEFT_DOWN          6
#define     AMI_MOUSE_MIDDLE_DOWN        7
#define     AMI_MOUSE_RIGHT_UP           8
#define     AMI_MOUSE_LEFT_UP            9
#define     AMI_MOUSE_MIDDLE_UP         10

typedef enum {
    ///
    /// Show Mouse pointer and report actions
    ///
    AMI_MOUSE_POINTER_PROPERTY_NORMAL = 0,
    ///
    /// freeze the Mouse pointer and don't update the movement and update the actions.
    ///
    AMI_MOUSE_POINTER_PROPERTY_FREEZE,
    ///
    /// Hide the Mouse pointer but report the movement and actions
    ///
    AMI_MOUSE_POINTER_PROPERTY_HIDE
} AMI_MOUSE_POINTER_PROPERTY;

typedef struct {
    UINT32  ActivateKeyStartX;    ///< Start X position of Activate SoftKeybord icon.
    UINT32  ActivateKeyEndX;      ///< End X position of Activate SoftKeybord icon.
    UINT32  ActivateKeyStartY;    ///< Start Y position of Activate SoftKeybord icon.
    UINT32  ActivateKeyEndY;      ///< End Y position for Activate SoftKeybord icon.
    UINT32  DeActivateKeyStartX;  ///< Start X position of DeActivate SoftKeybord icon.
    UINT32  DeActivateKeyEndX;    ///< End X position of DeActivate SoftKeybord icon.
    UINT32  DeActivateKeyStartY;  ///< Start Y position of DeActivate SoftKeybord icon.
    UINT32  DeActivateKeyEndY;    ///< End Y position of DeActivate SoftKeybord icon.
    UINT32  EscKeyStartX;         ///< Start X position of Esc icon.
    UINT32  EscKeyEndX;           ///< End X position of Esc icon.
    UINT32  EscKeyStartY;         ///< Start Y position of Esc icon.
    UINT32  EscKeyEndY;           ///< End Y position of Esc icon.
    UINT32  SetupKeyStartX;       ///< Start X position of Setup icon.
    UINT32  SetupKeyEndX;         ///< End X position of Setup icon.
    UINT32  SetupKeyStartY;       ///< Start Y position of Setup icon.
    UINT32  SetupKeyEndY;         ///< End Y position of Setup icon.
    UINT32  BbsKeyStartX;         ///< Start X position of Bbs icon.
    UINT32  BbsKeyEndX;           ///< End X position of Bbs icon.
    UINT32  BbsKeyStartY;         ///< Start Y position of Bbs icon.
    UINT32  BbsKeyEndY;           ///< End Y position of Bbs icon.
} AMI_SOFT_KEY_DATA;

typedef struct {
    UINT32  MinSpeed;                         ///< Minimum speed of the mouse pointer.
    UINT32  MaxSpeed;                         ///< Maximum speed of the mouse pointer.
    UINT32  TranslationMultiplier;            ///< Multiplier for changing the mouse speed.
} AMI_MOUSE_TRANSLATION_TABLE;


/**
    Polling function to get mouse movement and action.

    This function called to update the Mouse pointer pointer position
    in the screen according to current position

    @param  This        Mouse Protocol Pointer.

    @retval EFI_SUCCESS The operation completed successfully.

**/

typedef EFI_STATUS (EFIAPI *AMI_MOUSE_POLL)(
    IN  AMI_MOUSE_PROTOCOL  *This
);

/**
    Initializes mouse movement related events and restrict its movement to a
    defined area.

    Based on the Input Argument passed, the Mouse movement in the screen is initialized.

    @param  This          Mouse Protocol Pointer.
    @param  StartX        Min Row for the mouse movement.
    @param  StartY        Min Column for the mouse movement.
    @param  EndX          Maximum Row for the mouse movement.
    @param  EndY          Maximum Column for the mouse movement.

    @retval EFI_SUCCESS            Initialized mouse movement successfully.
    @retval EFI_NOT_FOUND          Resource allocation failed.
    @retval EFI_INVALID_PARAMETER  One of the parameters has an invalid value.

**/

typedef EFI_STATUS (EFIAPI *AMI_MOUSE_INITIALIZE)(
    IN  AMI_MOUSE_PROTOCOL  *This,
    IN  INT32               StartX,
    IN  INT32               StartY,
    IN  INT32               EndX,
    IN  INT32               EndY
);

/**
    Starts updating the mouse pointer movement.

    Update the Mouse cursor position in the screen based on the latest Mouse coordinates

    @param  This         Mouse Protocol Pointer.

    @retval EFI_SUCCESS  Mouse pointer movement updated
                         successfully.

**/

typedef EFI_STATUS (EFIAPI *AMI_MOUSE_START_UPDATING_MOUSE_CURSOR)(
    IN  AMI_MOUSE_PROTOCOL  *This
);

/**
    Stops updating the mouse pointer movement and removes pointer from the screen.

    Freeze the Mouse Cursor or it stops updating the Mouse Cursor position.

    @param  This         Mouse Protocol Pointer.

    @retval EFI_SUCCESS  Stops updating the mouse pointer.

**/

typedef EFI_STATUS (EFIAPI *AMI_MOUSE_STOP_UPDATING_MOUSE_CURSOR)(
    IN  AMI_MOUSE_PROTOCOL  *This
);

/**
    Redraws the mouse pointer on the screen if it is corrupted.

    @param  This                  Mouse Protocol Pointer.

    @retval EFI_SUCCESS           Mouse pointer is Redrawn.
    @retval EFI_INVALID_PARAMETER BltOperation is not valid.
    @retval EFI_DEVICE_ERROR      A hardware error occurred writing to the video buffer.

**/

typedef EFI_STATUS (EFIAPI *AMI_MOUSE_REFRESH_MOUSE_CURSOR)(
    IN  AMI_MOUSE_PROTOCOL  *This
);

/**
    Returns the current mouse position.

    The current location Mouse cursor on the screen is returned

    @param  This            Mouse Protocol Pointer.
    @param  PointerX        current X-axis position.
    @param  PointerY        current Y-axis position.
    @param  PointerZ        current Z-axis position.

    @retval EFI_SUCCESS     Updated current mouse position.

**/

typedef EFI_STATUS (EFIAPI *AMI_MOUSE_GETPOSITION)(
    IN  AMI_MOUSE_PROTOCOL   *This,
    OUT INT32                *PointerX,
    OUT INT32                *PointerY,
    OUT INT32                *PointerZ
);

/**
    Sets the mouse pointer position.

    Re-draw the mouse cursor on the input mouse cursor position

    @param  This                   Mouse Protocol Pointer.
    @param  PointerX               X-axis position.
    @param  PointerY               Y-axis position.

    @retval EFI_SUCCESS            Coordinate values updated successfully.
    @retval EFI_INVALID_PARAMETER  Invalid input values.

**/

typedef EFI_STATUS (EFIAPI *AMI_MOUSE_SETPOSITION)(
    IN  AMI_MOUSE_PROTOCOL  *This,
    IN  INT32                PointerX,
    IN  INT32                PointerY
);

/**
    Sets the Speed of mouse movement

    For the simple pointer device or relative movement device, the movement speed can be set.
    Based on that the the movement value is calculated and updated as mouse device current location

    @param  This             Mouse Protocol Pointer.
    @param  Value            Speed of the move movement.


    @retval EFI_SUCCESS      Mouse movement Speed set successfully.
    @retval EFI_UNSUPPORTED  Invalid input values.

**/

typedef EFI_STATUS (EFIAPI *AMI_MOUSE_SETSPEED)(
    IN  AMI_MOUSE_PROTOCOL  *This,
    IN  UINT32              Value
);

/**
    Returns the mouse button status.

    returns both the simple pointer device and absolute pointer click status.

    @param  This           Mouse Protocol Pointer.
    @param  ButtonStatus   Device Button status.

    @retval EFI_SUCCESS    Mouse button status returned successfully.

    @note
      If there is no mouse click then
      *ButtonStatus = AMI_MOUSE_NULL_CLICK

**/

typedef EFI_STATUS (EFIAPI *AMI_MOUSE_GETBUTTONSTATUS)(
    IN  AMI_MOUSE_PROTOCOL  *This,
    OUT INT32               *ButtonStatus
);

/**
    Sets and restricts the mouse movement viewpoint.

    This function called to restrict the mouse movement coordinates in the screen

    @param  This                    Mouse Protocol Pointer.
    @param  MouseViewStartRow       Start row position.
    @param  MouseViewStartColumn    Start column position.
    @param  MouseViewEndRow         End row position.
    @param  MouseViewEndColumn      End column position.

    @retval EFI_SUCCESS             Mouse movement coordinates set successfully.

**/

typedef EFI_STATUS (EFIAPI *AMI_MOUSE_SETVIEWPOINT)(
    IN  AMI_MOUSE_PROTOCOL  *This,
    IN  INT32               MouseViewStartRow,
    IN  INT32               MouseViewStartColumn,
    IN  INT32               MouseViewEndRow,
    IN  INT32               MouseViewEndColumn
);

/**
    Sets Soft keyboard Activate, Deactivate Button and ESC Icon coordinates.
    So Mouse driver can inform others when there is click on Activate and
    Deactivate button.

    @param  This          Mouse Protocol Pointer.
    @param  SoftKeyData   Structure that contains SoftKbd related
                          button coordinates.

    @retval EFI_SUCCESS   Coordinates are updated.

**/

typedef EFI_STATUS (EFIAPI *AMI_MOUSE_SOFTKEY)(
    IN  AMI_MOUSE_PROTOCOL  *This,
    IN  AMI_SOFT_KEY_DATA       *SoftKeyData
);

/**
    Stops updating the mouse pointer movement and removes pointer from the screen.

    @param  This                  Mouse Protocol Pointer.

    @retval EFI_SUCCESS           Stops mouse movement related events successfully.
    @retval EFI_INVALID_PARAMETER Invalid event or type.

**/

typedef EFI_STATUS (EFIAPI *AMI_MOUSE_STOP)(
    IN  AMI_MOUSE_PROTOCOL  *This
);

/**
    This function is provided to allow overriding of the default mouse
    speed translation table.

    @param  This            Mouse Protocol pointer.
    @param  NewTable        Pointer to new speed translation table.
    @param  NewTableSize    New table size.

    @retval EFI_SUCCESS           Translation table is updated with new speed.
    @retval EFI_INVALID_PARAMETER Invalid input parameters.
**/

typedef EFI_STATUS (EFIAPI *AMI_MOUSE_SET_SPEED_TRANSLATION_TABLE)(
    IN  AMI_MOUSE_PROTOCOL           *This,
    IN  AMI_MOUSE_TRANSLATION_TABLE  *NewTable,
    IN  UINT32                       NewTableSize
);

/**
    This function returns the current speed of the mouse device.

    @param  This             pointer to the mouse protocol.
    @param  MouseVelocity    contains the current mouse speed upon return.

    @retval EFI_SUCCESS      Current mouse speed returned.

**/

typedef EFI_STATUS (EFIAPI *AMI_MOUSE_GET_VELOCITY)(
    IN  AMI_MOUSE_PROTOCOL  *This,
    OUT UINT32              *MouseVelocity
);

/**
    Updates the Mouse pointer shape with the input pointer value.

    This function called to overwrite the default Mouse Pointer display with OEM defined
    Mouse Pointer

    @param  This                Pointer to the mouse protocol.
    @param  MousePointerBlt     Blt buffer of new Mouse pointer.
    @param  Width               Width in pixels of new mouse pointer.
    @param  Height              Height in pixels of new mouse pointer.
    @param  TransparentPixel    Pixel color in the input mouse pointer buffer that is
                                transparent (screen background).

    @retval EFI_NOT_FOUND          Resource allocation failed.
    @retval EFI_INVALID_PARAMETER  Initialize Mouse Pointer to default.
    @retval EFI_SUCCESS            Updates the Mouse pointer shape with the input pointer value.

**/

typedef EFI_STATUS (EFIAPI *AMI_MOUSE_UPDATE_POINTER)(
    IN  AMI_MOUSE_PROTOCOL              *This,
    IN  EFI_GRAPHICS_OUTPUT_BLT_PIXEL   *MousePointerBlt,
    IN  UINT32                          Width,
    IN  UINT32                          Height,
    IN  EFI_GRAPHICS_OUTPUT_BLT_PIXEL   *TransparentPixel
);

/**
    Updates Mouse Pointer Property value

    Sets the Mouse Pointer property display in the screen

    Sets the Mouse Pointer Property value for the MouseDriver.

    @param  This           Pointer to the mouse protocol.
    @param  Property       MousePointer property.

    @retval  EFI_SUCCESS   MousePointer property is updated successfully.

**/

typedef EFI_STATUS (EFIAPI *AMI_MOUSE_UPDATE_MOUSE_POINTER_PROPERTY)(
    IN  AMI_MOUSE_PROTOCOL      *This,
    IN  AMI_MOUSE_POINTER_PROPERTY  Property
);


/**
    Updates GOP instance from other application

    @param  This                 Pointer to the mouse protocol.
    @param  Gop                  Pointer to Graphics output protocol.
    @param  RestoreOptimization  set when application overrides default GOP instance.

    @retval EFI_SUCCESS          GOP instance updated successfully.

**/

typedef EFI_STATUS (EFIAPI *AMI_MOUSE_SET_MOUSE_GOP)(
    IN  AMI_MOUSE_PROTOCOL               *This,
    IN  EFI_GRAPHICS_OUTPUT_PROTOCOL     *Gop,
    IN  BOOLEAN                          RestoreOptimization
);

/**
    Returns the mouse image over the passed background buffer

    This function called to get the BLT buffer of the Mouse cursor.

    @param  This                   AMI_MOUSE_PROTOCOL instance.
    @param  BkgBuffer              Buffer for background of mouse image.
    @param  MouseBuffer            Mouse cursor image on top of background buffer.

    @retval EFI_SUCCESS            Mouse buffer is updated.
    @retval EFI_INVALID_PARAMETER  Invalid input parameter.
    @retval EFI_OUT_OF_RESOURCES   Resource allocation failed.

**/

typedef EFI_STATUS (EFIAPI *AMI_MOUSE_GET_MOUSE_CURSOR)(
    IN  AMI_MOUSE_PROTOCOL               *This,
    IN  EFI_GRAPHICS_OUTPUT_BLT_PIXEL    *BkgBuffer,
    OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL   **MouseBuffer
);

/**
    The AMI_MOUSE_PROTOCOL provides a set of services for a pointer device.

 */

struct _AMI_MOUSE_PROTOCOL {
    UINT8                                               Revision;   ///< Mouse protocol revision.
    AMI_MOUSE_POLL                                      MousePoll;
    AMI_MOUSE_INITIALIZE                                InitializePointingDevice;
    AMI_MOUSE_START_UPDATING_MOUSE_CURSOR               StartUpdatingMouseCursor;
    AMI_MOUSE_STOP_UPDATING_MOUSE_CURSOR                StopUpdatingMouseCursor;
    AMI_MOUSE_REFRESH_MOUSE_CURSOR                      RefreshMouseCursor;
    AMI_MOUSE_GETPOSITION                               GetCoordinates;
    AMI_MOUSE_SETPOSITION                               SetCoordinates;
    AMI_MOUSE_SETSPEED                                  SetMouseSpeed;
    AMI_MOUSE_GETBUTTONSTATUS                           GetButtonStatus;
    AMI_MOUSE_SETVIEWPOINT                              SetMouseCoordinates;
    AMI_MOUSE_SOFTKEY                                   SetSoftKeyData;
    AMI_MOUSE_STOP                                      StopPointingDevice;
    AMI_MOUSE_SET_SPEED_TRANSLATION_TABLE               SetSpeedTranslationTable;
    AMI_MOUSE_GET_VELOCITY                              GetMouseVelocity;
    AMI_MOUSE_UPDATE_POINTER                            UpdateMousePointer;
    AMI_MOUSE_UPDATE_MOUSE_POINTER_PROPERTY             UpdateMousePointerProperty;
    AMI_MOUSE_SET_MOUSE_GOP                             SetMouseGOP;
    AMI_MOUSE_GET_MOUSE_CURSOR                          GetMouseCursorImage;
};

// Mouse protocol GUID
extern EFI_GUID gAmiMouseProtocolGuid;

// These are axillary protocols with NULL interface that are used to signal certain events.
extern EFI_GUID gAmiSoftkeyActivateProtocolGuid;
extern EFI_GUID gAmiSoftKeyDeactivateProtocolGuid;
extern EFI_GUID gAmiProcessEscKeyProtocolGuid;
extern EFI_GUID gAmiProcessPostKeyProtocolGuid;

#endif

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
