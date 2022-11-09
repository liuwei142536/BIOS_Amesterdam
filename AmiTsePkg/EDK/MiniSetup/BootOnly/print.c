//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2022, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file print.c
    file contains code to support the print functionalities

**/

/*++
  VSPrint, Print, SPrint format specification has the follwoing form

  %[flags][width]type

  flags:
    '-' - Left justify
    '+' - Prefix a sign
    ' ' - Prefix a blank
    ',' - Place commas in numberss
    '0' - Prefix for width with zeros
    'l' - UINT64
    'L' - UINT64

  width:
    '*' - Get width from a UINTN argumnet from the argument list
    Decimal number that represents width of print

  type:
    'X' - argument is a UINTN hex number, prefix '0'
    'x' - argument is a hex number
    'd' - argument is a decimal number
    'a' - argument is an ascii string 
    'S','s' - argument is an Unicode string
    'g' - argument is a pointer to an EFI_GUID
    't' - argument is a pointer to an EFI_TIME structure
    'c' - argument is an ascii character
    'r' - argument is EFI_STATUS
    '%' - Print a %

--*/
#include "minisetup.h"
#include "setupdbg.h"
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>

#if TSE_USE_EDK_LIBRARY
#include "Library/PerformanceLib.h"
#endif

#if TSE_DEBUG_MESSAGES
extern BOOLEAN SetupDebugKeyHandler(UINT16 ItemIndex, UINT16 Unicode, CHAR16	*Value) ;
#endif
//typedef CHAR16  CHAR_W;

#define AMITSE_VSPrint  UnicodeVSPrint

/**
    SPrint function to process format and place the results in Buffer

    @param Buffer Wide char buffer to print the results of the parsing of Format into.
    @param BufferSize The size, in bytes, of the output buffer specified by Buffer.
    @param Format Format string see file header for more details.
        ...    - Vararg list consumed by processing Format.

    @retval Number of characters printed.

**/
UINTN
EFIAPI SPrint (
  OUT CHAR16        *Buffer,
  IN  UINTN         BufferSize,
  IN  CONST CHAR16  *Format,
  ...
  )
{
  UINTN   Return;
  VA_LIST Marker;

  VA_START (Marker, Format);
//  Return = AMITSE_VSPrint(Buffer, BufferSize, Format, Marker);
  Return = AMITSE_VSPrint(Buffer, BufferSize, (CHAR16*)Format, Marker);
  VA_END (Marker);

  return Return;
}

#define MAX_DEBUG_STRING_SIZE 1000

CHAR16  tmpBuffer[MAX_DEBUG_STRING_SIZE];

/**

    @param 

    @retval 

**/
VOID EFIAPI SetupDebugPrint(IN CONST CHAR8  *Format, ...)
{
    VA_LIST Marker;
    CHAR16 *c16Format = (CHAR16 *)NULL;
    CHAR8 *trace = NULL;

    c16Format = StrDup8to16((CHAR8  *)Format);

    VA_START (Marker, Format);
    AMITSE_VSPrint(tmpBuffer, (MAX_DEBUG_STRING_SIZE * sizeof(CHAR16)) , ( CHAR16 *)c16Format, Marker);
    VA_END (Marker);

#if SERIAL_DEBUG
    trace = StrDup16to8(tmpBuffer);
    DEBUG ((DEBUG_ERROR, trace));
    MemFreePointer( (VOID **)&trace );
#else //Else of SERIAL_DEBUG

#if SCREEN_PRINT
    Print(tmpBuffer);
#else //Else of SCREEN_PRINT
    if(gST->ConOut){
        gST->ConOut->OutputString( gST->ConOut, tmpBuffer );
    }
#endif //End of SCREEN_PRINT
#endif //End of SERIAL_DEBUG
    MemFreePointer( (VOID **)&c16Format );
}

/**

    @param 

    @retval 
**/
//extern VOID HandleDebugDumpKey( VOID *app, VOID *hotkey, VOID *cookie );
VOID SetupShowDebugScreenWrapper (VOID *app, VOID *hotkey, VOID *cookie )
{

#if TSE_DEBUG_MESSAGES    
    UINT8 i = 0;
    TEXT_INPUT_TEMPLATE SetupDebugItems[7] = 
    {
        {STRING_TOKEN(STR_DBG_PRINT_HIIPARSING), 1, TSE_FILTER_KEY_CUSTOM, FALSE, NULL},
        {STRING_TOKEN(STR_DBG_PRINT_HIIFUNCTION), 1, TSE_FILTER_KEY_CUSTOM, FALSE, NULL},
        {STRING_TOKEN(STR_DBG_PRINT_HIICALLBACK), 1, TSE_FILTER_KEY_CUSTOM, FALSE, NULL},
        {STRING_TOKEN(STR_DBG_PRINT_HIINOTIFICATION) , 1, TSE_FILTER_KEY_CUSTOM, FALSE, NULL},
        {STRING_TOKEN(STR_DBG_PRINT_VARIABLE) , 1, TSE_FILTER_KEY_CUSTOM, FALSE, NULL},
        {STRING_TOKEN(STR_DBG_PRINT_IFRFORM), 13, TSE_FILTER_KEY_CUSTOM, FALSE, NULL},
        {STRING_TOKEN(STR_DBG_PRINT_VARIABLE_CACHE), 13, TSE_FILTER_KEY_CUSTOM, FALSE, NULL}
    };
    
    UINT16 Title = HiiAddString( gHiiHandle, L"TSE Debug Print") ;

    for (i =0 ; i < 5; i ++)
    {
        SetupDebugItems[i].Value = EfiLibAllocateZeroPool(2*sizeof(CHAR16));
    }
    SetupDebugItems[i].Value = EfiLibAllocateZeroPool(14*sizeof(CHAR16));
    StrCpyS( SetupDebugItems[i].Value, 14, L"press any key");
    
    SetupDebugItems[i+1].Value = EfiLibAllocateZeroPool(14*sizeof(CHAR16));
    StrCpyS( SetupDebugItems[i+1].Value, 14, L"press any key");
    
    if((gDbgPrint & PRINT_UEFI_PARSE)!= PRINT_UEFI_PARSE)
       StrCpyS(  SetupDebugItems[0].Value, 2, L" ") ;      
    else
       StrCpyS(  SetupDebugItems[0].Value, 2, L"X") ;
    
    if((gDbgPrint & PRINT_UEFI)!= PRINT_UEFI)
        StrCpyS(  SetupDebugItems[1].Value, 2, L" ") ;
    else
       StrCpyS(  SetupDebugItems[1].Value, 2, L"X") ;
    
    if((gDbgPrint & PRINT_UEFI_NOTIFICATION)!= PRINT_UEFI_NOTIFICATION)
       StrCpyS(  SetupDebugItems[2].Value, 2, L" ") ;
    else
       StrCpyS(  SetupDebugItems[2].Value, 2, L"X") ;
    
    if((gDbgPrint & PRINT_UEFI_CALLBACK)!= PRINT_UEFI_CALLBACK)
       StrCpyS(  SetupDebugItems[3].Value, 2, L" ") ;
    else
       StrCpyS(  SetupDebugItems[3].Value, 2, L"X") ;   
    
    if((gDbgPrint & PRINT_UEFI_VAR) != PRINT_UEFI_VAR)
        StrCpyS(  SetupDebugItems[4].Value, 2, L" ") ;  
    else
       StrCpyS(  SetupDebugItems[4].Value, 2, L"X") ;

    PostManagerDisplayTextBox(
            gHiiHandle,
            Title,
            SetupDebugItems, 
            7,
            SetupDebugKeyHandler
            ) ;
    
    for(i=0; i<7; i++)
        MemFreePointer((VOID **)&SetupDebugItems[i].Value);
#endif    
}

#if TSE_DEBUG_MESSAGES

/**

    @param 

    @retval 
**/
VOID SetupToggleDebugFeature(UINT16 FeatureBit, CHAR16 *Value)
{
    
    //EFI_STATUS Status = EFI_UNSUPPORTED;
    EFI_GUID    guidDbgPrint = TSE_DEBUG_MESSAGES_GUID;  
    
    if((gDbgPrint & FeatureBit)== FeatureBit)
    {
        gDbgPrint &= ~((UINT16)FeatureBit) ;    
        StrCpyS( Value, 2, L" ") ;
   
    }
    else
    {
        gDbgPrint |= (UINT16)FeatureBit ;
        StrCpyS( Value, 2, L"x");
    }

    pRS->SetVariable(L"TseDebugPrint", 
        &guidDbgPrint,
        EFI_VARIABLE_NON_VOLATILE |
        EFI_VARIABLE_BOOTSERVICE_ACCESS,
        sizeof(gDbgPrint),
        &gDbgPrint);  

  
}
#endif //End of TSE_DEBUG_MESSAGES
