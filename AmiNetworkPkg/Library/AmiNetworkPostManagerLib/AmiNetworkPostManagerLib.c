//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


//*************************************************************************
/** @file AmiNetworkPostManagerLib.c
    AmiNetworkPostManagerLib Definitions

**/
//*************************************************************************
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Library/AmiNetworkPostManagerLib.h>
#include <Protocol/AMIPostMgr.h>

AMI_POST_MANAGER_PROTOCOL *pAmiPostMgr = NULL;

EFI_GUID AmiPostMgrProtocolGuid=AMI_POST_MANAGER_PROTOCOL_GUID;

EFI_GRAPHICS_OUTPUT_BLT_PIXEL ForeGround = {0xFF,0xFF,0xFF,0};
EFI_GRAPHICS_OUTPUT_BLT_PIXEL BackGround = {0,0,0,0};

//
// Macro definitions
//
#define IP_MODE_IP4               0
#define IP_MODE_IP6               1

INTN
EFIAPI
CompareMem (
  IN CONST VOID  *DestinationBuffer,
  IN CONST VOID  *SourceBuffer,
  IN UINTN       Length
  );

/*++

Routine Description:

  Call AMI Post Manager protocol to create Popup.

Arguments:

  String - String for display.

Returns:

  None.

--*/

VOID 
AMICreatePopUp(
  IN  CHAR16    *String  
  )
{
  UINT8      MsgBoxSel;
  EFI_STATUS Status = EFI_SUCCESS;
     
        if (pAmiPostMgr == NULL) {
            Status =  gBS->LocateProtocol(&AmiPostMgrProtocolGuid, NULL, (void **)&pAmiPostMgr);   // AMI PORTING : Added to resolve the build error with GCC compiler.
        }

        if (EFI_ERROR (Status)) return;
	  
        pAmiPostMgr->DisplayMsgBox(
                   L" ISCSI Message",
                   String,
                   MSGBOX_TYPE_OK,
                   &MsgBoxSel
                   );
   
    }

/*++

Routine Description:

  Call AMI Post Manager protocol to create Popup.

Arguments:

  String   - String for display.
  MsgTitle - String for PopUp Title

Returns:

  None.

--*/

VOID 
AMICreatePopUpEx (
  IN  CHAR16     *MsgTitle,
  IN  CHAR16     *String
  )
{
  UINT8      MsgBoxSel;
  EFI_STATUS Status = EFI_SUCCESS;
 
  if (pAmiPostMgr == NULL) {
      Status =  gBS->LocateProtocol(&AmiPostMgrProtocolGuid, NULL, (void **)&pAmiPostMgr);   
  }

  if (EFI_ERROR (Status)) return;
  
  pAmiPostMgr->DisplayMsgBox(
                 MsgTitle,
                 String,
                 MSGBOX_TYPE_OK,
                 &MsgBoxSel
                 );

}

/*++

Routine Description:

  Call AMI Post Manager protocol to create Popup.

Arguments:

  String - String for display.

Returns:

  None.

--*/

VOID 
AMIPrintText (
  IN  CHAR16     *String  )
{
  EFI_STATUS Status = EFI_SUCCESS;

        if (pAmiPostMgr == NULL) {
            Status =  gBS->LocateProtocol(&AmiPostMgrProtocolGuid, NULL, (void **)&pAmiPostMgr);   // AMI PORTING : Added to resolve the build error with GCC compiler.
        }

        if (EFI_ERROR (Status)) return;
				
  pAmiPostMgr->DisplayPostMessage(String);
  pAmiPostMgr->DisplayQuietBootMessage(
                 String, 
                 0, 
                 0, 
                 CA_AttributeLeftTop,
                 ForeGround, 
                 BackGround
                 );

    }


/*++

Routine Description:

  Call AMI Post Manager protocol for switch to post screen.

Arguments:

  None.

Returns:

  None.

--*/

VOID  
AMISwitchToPostScreen ()
{
        EFI_STATUS Status = EFI_SUCCESS;
	      if (pAmiPostMgr == NULL) {
            Status =  gBS->LocateProtocol(&AmiPostMgrProtocolGuid, NULL, (void **)&pAmiPostMgr);   // AMI PORTING : Added to resolve the build error with GCC compiler.
        }

        if (EFI_ERROR (Status))
        {
            return;
        }
        pAmiPostMgr->SwitchToPostScreen( );
		
}

/*++

Routine Description:

  Clears graphics screen using GOP protocol.

Arguments:

  None.

Returns:

  None.

--*/

VOID 
ClearGraphicsScreen (
 VOID ) 
{
  EFI_STATUS      Status = EFI_SUCCESS;
  EFI_UGA_PIXEL   UgaBlt={0,0,0,0};
  UINT32          HorizontalResolution = 0;
  UINT32          VerticalResolution = 0;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *pGOP = NULL;
     
        
            Status =  gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (void **)&pGOP);   // AMI PORTING : Added to resolve the build error with GCC compiler.
            if ( EFI_ERROR (Status) ) {
                pGOP = NULL;
                return;
            }

  HorizontalResolution = pGOP->Mode->Info->HorizontalResolution;
  VerticalResolution = pGOP->Mode->Info->VerticalResolution;
  pGOP->Blt(
          pGOP,
          &UgaBlt,
          EfiBltVideoFill,
          0, 
          0,
          0, 
          0,
          HorizontalResolution, 
          VerticalResolution,
          0 
          );
}

/**
  Check for invalid character in IP address.

  @param  Source        A pointer to a Null-terminated Unicode string.

  @return 
  EFI_SUCCESS - If invalid characters are not present
  EFI_INVALID_PARAMETER - If in valid characters are present

**/
EFI_STATUS
CheckInvalidCharinIpAddress (
  IN CONST CHAR16       *IpSource,
  IN UINT8              IpMode
  )
{
    //
    // For IP4 address format: Below are the valid characters
    // 1) "0-9" 2) "."(DOT)
    //
    if ( IpMode == IP_MODE_IP4 ) {
        while (*IpSource != '\0') {
            if ( ( ((*IpSource >= '0') && (*IpSource <= '9')) ||\
                   (*IpSource =='.') ) != TRUE ) {
                return EFI_INVALID_PARAMETER;
            }
            IpSource++;
        }
    } else if ( IpMode == IP_MODE_IP6 ) {
        //
        // For IP6 address format: Below are the valid characters
        // 1) "0-9" 2) ":"(COLON) 3) "a-f" 4) "A-F"
        //
        while (*IpSource != '\0') {
            if ( ( ((*IpSource >= '0') && (*IpSource <= '9')) ||\
                   ((*IpSource >= 'a') && (*IpSource <= 'f')) ||\
                   ((*IpSource >= 'A') && (*IpSource <= 'F')) ||\
                   (*IpSource ==':') ) != TRUE ) {
                return EFI_INVALID_PARAMETER;
            }
            IpSource++;
        }
    } else {
        return EFI_INVALID_PARAMETER;
    }
    return EFI_SUCCESS;
}

/**
  Gets the string form Supported language. If it fails, gets the string from Default language.

  @param EFI_HII_HANDLE HiiHandle
  		 EFI_STRING_ID StringId
		 CHAR8 *Language

  @return CHAR16* 
 
**/


CHAR16* NetLibHiiGetString( EFI_HII_HANDLE HiiHandle, EFI_STRING_ID StringId, CHAR8 *Language)
{
	EFI_STRING String = NULL;
	CHAR8 *Lang = "en-US";

	String = HiiGetString(HiiHandle,StringId,Language);

	if(String == NULL && (Language == NULL || CompareMem(Language,Lang,5)))
	{
		String = HiiGetString(HiiHandle,StringId,Lang);
	}
	
	return String;
}
