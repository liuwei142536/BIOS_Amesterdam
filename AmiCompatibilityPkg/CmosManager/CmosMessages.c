//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file CmosMessages.c
    Contains code to display CMOS-related boot messages.

**/

//---------------------------------------------------------------------------

#include <Efi.h>
#include <AmiDxeLib.h>
#include <Library/BaseMemoryLib.h>
#include <SspData.h>
#include <SspTokens.h>
#include <CmosAccess.h>
#include "CmosManager.h"
#include <Setup.h>
#include <Protocol/AMIPostMgr.h>
#include <Protocol/SimpleTextIn.h>
#include <Protocol/SimpleTextOut.h>
#include <Library/HiiLib.h>
#include "AmiVfr.h"
#include "Include/UefiHii.h"
#include "Protocol/HiiDatabase.h"
#include "Protocol/HiiString.h"

//---------------------------------------------------------------------------
extern EFI_BOOT_SERVICES        *gBS;
extern EFI_SYSTEM_TABLE         *gST;
extern EFI_RUNTIME_SERVICES     *gRT;

static EFI_GUID                  gAmiPostManagerProtocolGuid =
                                     AMI_POST_MANAGER_PROTOCOL_GUID;
static AMI_POST_MANAGER_PROTOCOL *gAmiPostMgr = NULL;
static EFI_HII_HANDLE            gHiiHandle = NULL;
static VOID                      *gCmosMgrNotifyReg = NULL;
static EFI_EVENT                 gCmosMgrEvent = NULL;

// Console messages are only supported if TSE sources are included

#ifdef EFI_DEBUG

#ifdef CMOS_TRACE_FULL
  #undef CMOS_TRACE_FULL
#endif

#define CMOS_TRACE_FULL(args) CmosTrace args;

CHAR8* CmosStrDup16to8(CHAR16 *String)
{
    CHAR8 *text;
    UINTN len = 0, j = 0;

    while(String[j++])
       len++;
    text = MallocZ( (1 + len) * sizeof(CHAR8));
    if(text != NULL)
    {
        j = 0;
        do
        {
            text[j] = (CHAR8)String[j];
        }while(j++ < len);
    }

    return text;
}

VOID CmosTrace(
    IN BOOLEAN    Char16,
    IN CHAR8      *Format,
    ... )
{
    va_list ArgList;
    CHAR16  tmpBuffer[512];
    CHAR8 *trace = NULL;
    CHAR16 *c16Format = (CHAR16 *)NULL;

    if (Char16){
//        VA_START (ArgList, Format);
        va_start(ArgList, Format);
        Swprintf_s_va_list  (tmpBuffer, 512, (CONST CHAR16 *)c16Format, ArgList);
//        VA_END (ArgList);
        va_end(ArgList);
        trace = CmosStrDup16to8(tmpBuffer);
        TRACE ((-1, trace));
        gBS->FreePool( (VOID **)&trace );
    }
    else {
//        ArgList = va_start(ArgList,Format);
        va_start(ArgList,Format);
        PrintDebugMessageVaList(-1, Format, ArgList);
        va_end(ArgList);
    }
}
#endif


VOID * CmosAllocateZeroPool (
  IN UINTN            AllocationSize)
{
  VOID        *Memory;
  EFI_STATUS  Status;

  Status = gBS->AllocatePool (EfiBootServicesData, AllocationSize, &Memory);
  if (EFI_ERROR (Status)) {
    Memory = NULL;
  }
  else {
    Memory = ZeroMem (Memory, AllocationSize);
  }
  return Memory;
}


VOID EFIAPI CmosCreatePopUp (
  IN  UINTN          Attribute,
  OUT EFI_INPUT_KEY  *Key,      OPTIONAL
  ...)
{
  VA_LIST                          Args;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *ConOut;
  EFI_SIMPLE_TEXT_OUTPUT_MODE      SavedConsoleMode;
  UINTN                            Columns;
  UINTN                            Rows;
  UINTN                            Column;
  UINTN                            Row;
  UINTN                            NumberOfLines;
  UINTN                            MaxLength;
  CHAR16                           *String;
  UINTN                            Length;
  CHAR16                           *Line;
  UINTN                            EventIndex;

  //
  // Determine the length of the longest line in the popup and the total
  // number of lines in the popup
  //
  VA_START (Args, Key);
  MaxLength = 0;
  NumberOfLines = 0;
  while ((String = VA_ARG (Args, CHAR16 *)) != NULL) {
    MaxLength = MAX (MaxLength, StrLen (String));
    NumberOfLines++;
  }
  VA_END (Args);

  //
  // If the total number of lines in the popup is zero, then ASSERT()
  //
  ASSERT (NumberOfLines != 0);

  //
  // If the maximum length of all the strings is zero, then ASSERT()
  //
  ASSERT (MaxLength != 0);

  //
  // Cache a pointer to the Simple Text Output Protocol in the EFI System Table
  //
  ConOut = gST->ConOut;

  //
  // Save the current console cursor position and attributes
  //
  CopyMem (&SavedConsoleMode, ConOut->Mode, sizeof (SavedConsoleMode));

  //
  // Retrieve the number of columns and rows in the current console mode
  //
  ConOut->QueryMode (ConOut, SavedConsoleMode.Mode, &Columns, &Rows);

  //
  // Disable cursor and set the foreground and background colors specified by Attribute
  //
  ConOut->EnableCursor (ConOut, FALSE);
  ConOut->SetAttribute (ConOut, Attribute);

  //
  // Limit NumberOfLines to height of the screen minus 3 rows for the box itself
  //
  NumberOfLines = MIN (NumberOfLines, Rows - 3);

  //
  // Limit MaxLength to width of the screen minus 2 columns for the box itself
  //
  MaxLength = MIN (MaxLength, Columns - 2);

  //
  // Compute the starting row and starting column for the popup
  //
  Row    = (Rows - (NumberOfLines + 3)) / 2;
  Column = (Columns - (MaxLength + 2)) / 2;

  //
  // Allocate a buffer for a single line of the popup with borders and a Null-terminator
  //
  Line = CmosAllocateZeroPool ((MaxLength + 3) * sizeof (CHAR16));
  ASSERT (Line != NULL);

  //
  // Draw top of popup box
  //
  SetMem16 (Line, (MaxLength + 2) * 2, BOXDRAW_HORIZONTAL);
  Line[0]             = BOXDRAW_DOWN_RIGHT;
  Line[MaxLength + 1] = BOXDRAW_DOWN_LEFT;
  Line[MaxLength + 2] = L'\0';
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line);

  //
  // Draw middle of the popup with strings
  //
  VA_START (Args, Key);
  while ((String = VA_ARG (Args, CHAR16 *)) != NULL && NumberOfLines > 0) {
    Length = StrLen (String);
    SetMem16 (Line, (MaxLength + 2) * 2, L' ');
    if (Length <= MaxLength) {
      //
      // Length <= MaxLength
      //
      CopyMem (Line + 1 + (MaxLength - Length) / 2, String , Length * sizeof (CHAR16));
    } else {
      //
      // Length > MaxLength
      //
      CopyMem (Line + 1, String + (Length - MaxLength) / 2 , MaxLength * sizeof (CHAR16));
    }
    Line[0]             = BOXDRAW_VERTICAL;
    Line[MaxLength + 1] = BOXDRAW_VERTICAL;
    Line[MaxLength + 2] = L'\0';
    ConOut->SetCursorPosition (ConOut, Column, Row++);
    ConOut->OutputString (ConOut, Line);
    NumberOfLines--;
  }
  VA_END (Args);

  //
  // Draw bottom of popup box
  //
  SetMem16 (Line, (MaxLength + 2) * 2, BOXDRAW_HORIZONTAL);
  Line[0]             = BOXDRAW_UP_RIGHT;
  Line[MaxLength + 1] = BOXDRAW_UP_LEFT;
  Line[MaxLength + 2] = L'\0';
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line);

  //
  // Free the allocated line buffer
  //
  gBS->FreePool (Line);

  //
  // Restore the cursor visibility, position, and attributes
  //
  ConOut->EnableCursor      (ConOut, SavedConsoleMode.CursorVisible);
  ConOut->SetCursorPosition (ConOut, SavedConsoleMode.CursorColumn, SavedConsoleMode.CursorRow);
  ConOut->SetAttribute      (ConOut, SavedConsoleMode.Attribute);

  //
  // Wait for a keystroke
  //
  if (Key != NULL) {
    gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &EventIndex);
    gST->ConIn->ReadKeyStroke (gST->ConIn, Key);
  }
}


VOID DisplayCmosPopUp(
    CHAR16  *Title,
    CHAR16  *Message)
{
    EFI_INPUT_KEY               Key;
    CHAR16                      Blank[51] = L"";
    CHAR16                      Line1[51] = L"";
    CHAR16                      Line2[51] = L"";
    CHAR16                      Line3[51] = L"";
    CHAR16                      Line4[51] = L"";
    CHAR16                      Line5[51] = L"";
    CHAR16                      Prompt[51] = L"Press Enter (OK)";
    CHAR16                      *CurLine;
    UINTN                       LineCount = 1;
    UINTN                       CharCount = 1;
    CHAR16                      *SourceStr = Message;

    while (*SourceStr){
        if (CharCount == 1 && LineCount == 1){
            CurLine = Line1;
        }
        if (CharCount == 1 && LineCount == 2){
            CurLine = Line2;
        }
        if (CharCount == 1 && LineCount == 3){
            CurLine = Line3;
        }
        if (CharCount == 1 && LineCount == 4){
            CurLine = Line4;
        }
        if (CharCount == 1 && LineCount == 5){
            CurLine = Line5;
        }
        if (CharCount == 1 && LineCount == 6){
#ifdef CMOS_TRACE_FULL
            CMOS_TRACE_FULL(( FALSE,
                "DisplayCmosPopUp: Six lines is not supported.\n" ));
#endif
            ASSERT(FALSE);
        }
        *CurLine++ = *SourceStr++;
        if (++CharCount > 50){

            // if the next source char to be processed is not a space
            // or termination character, then go left until the first
            // space is found, clearing each character in the current
            // line until a space is found.

            while (*SourceStr != L' '){
            if (*SourceStr == L'\0')
                break;
                --SourceStr;
                --CurLine;
                *CurLine = L' ';
        }

            // ensure the first character on a new line is not a space

            while (*SourceStr == L' '){
            ++SourceStr;
        }
            ++LineCount;
            CharCount = 1;
        }
    }
    do {
        CmosCreatePopUp (EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
            &Key, Title, Blank, Line1, Line2, Line3, Line4,
            Line5, Blank, Prompt, NULL);
    } while ((Key.UnicodeChar != CHAR_CARRIAGE_RETURN));

    gST->ConOut->ClearScreen (gST->ConOut);
}

EFI_STATUS CmosMgrProcessMessages(
    IN EFI_EVENT  Event,
    IN VOID       *Context)
{
    EFI_STATUS                  Status;
    EFI_TPL                     OldTpl = 0;  // zero initial value required
    EFI_CMOS_ACCESS_INTERFACE   *Cmos = NULL;
    BOOLEAN                     BadBattery = FALSE;
    BOOLEAN                     DefaultsLoaded = FALSE;
    BOOLEAN                     NotUsable = FALSE;
    BOOLEAN                     BadChecksum = FALSE;
    BOOLEAN                     FirstBoot = FALSE;
    CMOS_STATUS_BYTES           CmosInfo;
    CHAR16                      Title[512]=L"";
    CHAR16                      BatteryMsg[512]=L"";
    CHAR16                      ChecksumMsg[512]=L"";
    CHAR16                      DefaultsMsg[512]=L"";
    CHAR16                      UsableMsg[512]=L"";
    CHAR16                      FirstBootMsg[512]=L"";
    UINTN                       Size;

    if (gCmosMgrEvent != NULL){
        pBS->CloseEvent(gCmosMgrEvent);
    }

    ZeroMem(Title, sizeof (Title));

#ifdef CMOS_TRACE_FULL
    CMOS_TRACE_FULL(( FALSE,
        "Locate CMOS Manager interface...\n" ));
#endif
    Status = gBS->LocateProtocol( &gAmiCmosAccessDxeProtocolGuid, NULL, &Cmos);
    if (!EFI_ERROR (Status)) {
        Cmos->ReadCmosStatusBytes(Cmos, &CmosInfo);

        NotUsable = CmosInfo.ConfigurationStatus.NotUsable;
    }
    else {
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE,
            "Could not locate CMOS Manager\n" ));
#endif
    }

    if ( !NotUsable ) {
        Cmos->Read(Cmos, CMOS_MGR_CHECKSUM_BAD,        &BadChecksum);
        Cmos->Read(Cmos, CMOS_MGR_BATTERY_BAD,         &BadBattery);
        Cmos->Read(Cmos, CMOS_MGR_DEFAULTS_LOADED,     &DefaultsLoaded);
        Cmos->Read(Cmos, CMOS_MGR_FIRST_BOOT_DETECTED, &FirstBoot);
    }

    // Show messages if the battery is bad or defaults were loaded.

    if ( BadChecksum || BadBattery || DefaultsLoaded
         || NotUsable || FirstBoot )
    {
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE,
            "Locate POST Manager interface...\n" ));
#endif
        Status = gBS->LocateProtocol( &gAmiPostManagerProtocolGuid, NULL,
                                      &gAmiPostMgr );
        if (EFI_ERROR (Status)) {
#ifdef CMOS_TRACE_FULL
            CMOS_TRACE_FULL(( FALSE,
                              "Could not locate POST manager\n" ));
#endif
            
            return EFI_SUCCESS;
        }

        // get strings
        Size = 512;
        Status = HiiLibGetString(gHiiHandle, STRING_TOKEN(CMOS_MESSAGE_TITLE), &Size, Title);
        if (EFI_ERROR(Status)){
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE,
            "Could not locate Title\n" ));
#endif
        }
        else {
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE, "Title: %S", Title ));
#endif
        }
        Size = 512;
        HiiLibGetString(gHiiHandle, STRING_TOKEN(BAD_BATTERY_MESSAGE), &Size, BatteryMsg);
        if (EFI_ERROR(Status)){
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE,
            "Could not locate BatteryMsg\n" ));
#endif
        }
        else {
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE, "BatteryMsg: %S", BatteryMsg ));
#endif
        }
        Size = 512;
        HiiLibGetString(gHiiHandle, STRING_TOKEN(BAD_CHECKSUM_MESSAGE), &Size, ChecksumMsg);
        if (EFI_ERROR(Status)){
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE,
            "Could not locate ChecksumMsg\n" ));
#endif
        }
        else {
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE, "ChecksumMsg: %S", ChecksumMsg ));
#endif
        }
        Size = 512;
        HiiLibGetString(gHiiHandle, STRING_TOKEN(DEFAULTS_LOADED_MESSAGE), &Size, DefaultsMsg);
        if (EFI_ERROR(Status)){
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE,
            "Could not locate DefaultsMsg\n" ));
#endif
        }
        else {
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE, "DefaultsMsg: %S", DefaultsMsg ));
#endif
        }
        Size = 512;
        HiiLibGetString(gHiiHandle, STRING_TOKEN(NOT_USABLE_MESSAGE), &Size, UsableMsg);
        if (EFI_ERROR(Status)){
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE,
            "Could not locate UsableMsg\n" ));
#endif
        }
        else {
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE,"UsableMsg: %S", UsableMsg ));
#endif
        }
        Size = 512;
        HiiLibGetString(gHiiHandle, STRING_TOKEN(FIRST_BOOT_MESSAGE), &Size, FirstBootMsg);
        if (EFI_ERROR(Status)){
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE,
            "Could not locate FirstBootMsg\n" ));
#endif
        }
        else {
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE,"FirstBootMsg: %S", FirstBootMsg ));
#endif
        }

        // save current TPL and then lower TPL to EFI_TPL_APPLICATION

        OldTpl = gBS->RaiseTPL( TPL_HIGH_LEVEL );
        gBS->RestoreTPL( TPL_APPLICATION );
    }


    if ( BadBattery  ){
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE,
            "Displaying bad battery POST message box...\n" ));
#endif
        DisplayCmosPopUp(Title, BatteryMsg);
    }

    if ( FirstBoot  ){
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE,
            "Displaying first boot POST message box...\n" ));
#endif
        DisplayCmosPopUp(Title, FirstBootMsg);
    }

    // The bad checksum message is expected on the first boot

    if ( BadChecksum && !FirstBoot  ){
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE,
            "Displaying bad checksum POST message box...\n" ));
#endif
        DisplayCmosPopUp(Title, ChecksumMsg);
    }

    if ( NotUsable ){
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE,
            "Displaying bad CMOS POST message box...\n" ));
#endif
        DisplayCmosPopUp(Title, UsableMsg);
    }

    if ( DefaultsLoaded ){
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE,
            "Displaying defaults loaded POST message box...\n" ));
#endif
        DisplayCmosPopUp(Title, DefaultsMsg);
    }

    if ( OldTpl != 0 )
        gBS->RaiseTPL(OldTpl);

    return EFI_SUCCESS;
}

VOID InitCmosStrings(EFI_HII_HANDLE HiiHandle, UINT16 Class)
{
    EFI_STATUS  Status;

#ifdef CMOS_TRACE_FULL
    CMOS_TRACE_FULL(( FALSE,
        "InitCmosStrings Entry >>>>>>>>>>>>>>>>>>\n" ));
#endif

    // Do it once
    if (Class != ADVANCED_FORM_SET_CLASS || gHiiHandle != NULL){
        return;
    }
    gHiiHandle = HiiHandle;


    // If the AMI_POST_MANAGER_PROTOCOL interface is available, call
    // CmosMgrProcessMessages. Otherwise, register a callback to
    // CmosMgrProcessMessages.

#ifdef CMOS_TRACE_FULL
    CMOS_TRACE_FULL(( FALSE,
        "Locate POST Manager interface...\n" ));
#endif
    Status = gBS->LocateProtocol( &gAmiPostManagerProtocolGuid, NULL,
                                  &gAmiPostMgr );
    if (EFI_ERROR(Status)){
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE,
            "Register callback to CmosMgrProcessMessages\n" ));
#endif
        Status = pBS->CreateEvent(EVT_NOTIFY_SIGNAL, TPL_NOTIFY,
            CmosMgrProcessMessages, NULL, &gCmosMgrEvent);
       if (!EFI_ERROR(Status)){
            Status = pBS->RegisterProtocolNotify(&gAmiPostManagerProtocolGuid,
                gCmosMgrEvent, &gCmosMgrNotifyReg);
            ASSERT_EFI_ERROR(Status);
        }
    }
    else {
#ifdef CMOS_TRACE_FULL
        CMOS_TRACE_FULL(( FALSE,
            "Call CmosMgrProcessMessages\n" ));
#endif
        CmosMgrProcessMessages(NULL, NULL);
    }

#ifdef CMOS_TRACE_FULL
    CMOS_TRACE_FULL(( FALSE,
        "InitCmosStrings Exit <<<<<<<<<<<<<<<<<<\n" ));
#endif
}
