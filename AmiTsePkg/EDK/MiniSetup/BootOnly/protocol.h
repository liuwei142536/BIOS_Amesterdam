//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2015, American Megatrends, Inc.        **//
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
// $Archive: /Alaska/SOURCE/Modules/AMITSE2_0/AMITSE/BootOnly/protocol.h $
//
// $Author: Blaines $
//
// $Revision: 7 $
//
// $Date: 11/10/11 7:21p $
//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//

#ifndef _PROTOCOL_H_
#define	_PROTOCOL_H_

EFI_STATUS InstallProtocol( VOID );
VOID UninstallProtocol( VOID );
EFI_STATUS EFIAPI PostManagerHandshake( VOID );
EFI_STATUS EFIAPI PostManagerSetCursorPosition(UINTN X, UINTN Y);
EFI_STATUS EFIAPI PostManagerGetCursorPosition(UINTN *pX, UINTN *pY);
EFI_STATUS EFIAPI PostManagerSetProgressBarPosition();
EFI_STATUS EFIAPI PostManagerInitProgressBar(
	UINTN x,
	UINTN y,
	UINTN w,
	UINTN h,
	UINTN delta);

TSE_POST_STATUS	EFIAPI PostManagerGetPostStatus(VOID);

EFI_STATUS EFIAPI PostManagerDisplayInfoBox(	CHAR16		*InfoBoxTitle,
										CHAR16		*InfoString,
										UINTN		Timeout,
										EFI_EVENT	*Event
						 			);


EFI_STATUS EFIAPI PostManagerSetAttribute(UINT8 ATTRIB);
EFI_STATUS EFIAPI PostManagerGetAttribute(UINT8 *ATTRIB);

EFI_STATUS
EFIAPI PostManagerDisplayMenu(
    IN VOID	*HiiHandle, 	
    IN UINT16 	TitleToken, 
    IN UINT16 	LegendToken,	  
    IN POSTMENU_TEMPLATE *MenuData,
    IN UINT16 	MenuCount,
    OUT UINT16  *pSelection
);

typedef 
BOOLEAN
(EFIAPI *DISPLAY_TEXT_KEY_VALIDATE) (
	UINT16 ItemIndex,
    UINT16 Unicode,
    CHAR16	*Value
);


EFI_STATUS
EFIAPI PostManagerDisplayTextBox(
    IN VOID	    *HiiHandle, 	
    IN UINT16 	TitleToken, 
    IN TEXT_INPUT_TEMPLATE *InputData,
    IN UINT16 	ItemCount,
    IN DISPLAY_TEXT_KEY_VALIDATE DisplayTextKeyValidate
);

EFI_STATUS EFIAPI PostManagerDisplayProgress (
IN UINT8		ProgressBoxState, 
IN CHAR16		*Title,
IN CHAR16		*Message,
IN CHAR16		*Legend,
IN UINTN 		Percent,	// 0 - 100
IN OUT VOID		**Handle,	//Out HANDLE. Valid Handle for update and close
OUT AMI_POST_MGR_KEY	*OutKey		//Out Key	   
);

EFI_STATUS InstallScreenMgmtProtocol( EFI_HANDLE Handle);
VOID UninstallScreenMgmtProtocol( EFI_HANDLE Handle);

EFI_STATUS EFIAPI ScreenMgmtSaveTseScreen ();

EFI_STATUS EFIAPI ScreenMgmtRestoreTseScreen ();

EFI_STATUS EFIAPI ScreenMgmtClearScreen (
		IN UINT8 Attribute
		);
EFI_STATUS InstallHiiPopupProtocol( EFI_HANDLE Handle);
VOID UninstallHiiProtocolProtocol(EFI_HANDLE Handle);

#ifndef EFI_HII_POPUP_PROTOCOL_GUID
#define EFI_HII_POPUP_PROTOCOL_GUID \
        { 0x4311edc0, 0x6054, 0x46d4, { 0x9e, 0x40, 0x89, 0x3e, 0xa9, 0x52, 0xfc, 0xcc } }

#define EFI_HII_POPUP_PROTOCOL_REVISION 1
typedef struct _EFI_HII_POPUP_PROTOCOL EFI_HII_POPUP_PROTOCOL;

typedef enum {
    EfiHiiPopupStyleInfo,
    EfiHiiPopupStyleWarning,
    EfiHiiPopupStyleError
} EFI_HII_POPUP_STYLE;

typedef enum {
    EfiHiiPopupTypeOk,
    EfiHiiPopupTypeOkCancel,
    EfiHiiPopupTypeYesNo,
    EfiHiiPopupTypeYesNoCancel
} EFI_HII_POPUP_TYPE;

typedef enum {
    EfiHiiPopupSelectionOk,
    EfiHiiPopupSelectionCancel,
    EfiHiiPopupSelectionYes,
    EfiHiiPopupSelectionNo
} EFI_HII_POPUP_SELECTION;

typedef 
EFI_STATUS
(EFIAPI * EFI_HII_CREATE_POPUP) (
  IN EFI_HII_POPUP_PROTOCOL     *This,
  IN EFI_HII_POPUP_STYLE        PopupStyle,
  IN EFI_HII_POPUP_TYPE         PopupType,
  EFI_HII_HANDLE                HiiHandle,
  IN EFI_STRING_ID              Message,
  OUT EFI_HII_POPUP_SELECTION   *UserSelectionOPTIONAL
);

typedef struct _EFI_HII_POPUP_PROTOCOL{
    UINT64                  Revision;
    EFI_HII_CREATE_POPUP    CreatePopup;
}EFI_HII_POPUP_PROTOCOL;
#endif
#endif /* _PROTOCOL_H_ */


//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2015, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**     5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
