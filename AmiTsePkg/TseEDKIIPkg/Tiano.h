/*++

Copyright (c) 2004 - 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

Module Name:

  Tiano.h

Abstract:

  Tiano master include file.

  This is the main include file for Tiano components. 

  Don't add include files to the list for convenience, only add things
  that are architectural. Don't add Protocols or GUID include files here

--*/

#ifndef _TIANO_H_
#define _TIANO_H_


#include <Base.h>
#include <PiDxe.h>
#include <Uefi/UefiMultiPhase.h>
#include <Library/DebugLib.h>
#include "Protocol/DevicePath.h"
#include <Framework/FrameworkInternalFormRepresentation.h>

/**
Macros required from EDK side
**/

#define EFI_BOOTSERVICE

//
// Define macros for including Protocols and Guids.
//
#define EFI_STRINGIZE(a)            #a
#define EFI_PROTOCOL_DEFINITION(a)  EFI_STRINGIZE (Protocol/a.h)
#define EFI_GUID_DEFINITION(a)      EFI_STRINGIZE (Guid/a.h)

#define PI_SPECIFICATION_VERSION 0x10028

/**
Macros required from EDK side over
**/

#define FLASH_ERASE_POLARITY 0

/**
AmiCompatibilityPkg macros
**/

#include <Guid/GlobalVariable.h>
#include <Library/BaseLib.h>
#include <Protocol/FirmwareVolume2.h>
#include <Library/DevicePathLib.h>
#include <Protocol/ComponentName2.h>
#include <Protocol/HiiConfigAccess.h>

//////// Header files Required in EdkHelper.c /////
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/StatusCode.h>
#include "Library/PrintLib.h"
///////////////////////////////////////////////////

//////// Header files added to compatible with UnicodeCollation2  /////
#include <Protocol/UnicodeCollation.h>
///////////////////////////////////////////////////


#define EFI_FORWARD_DECLARATION(x) typedef struct _##x x

#define MEDIA_FV_FILEPATH_DP  0x06

#define EFI_EVENT_TIMER EVT_TIMER
#define EFI_EVENT_NOTIFY_WAIT EVT_NOTIFY_WAIT
#define EFI_EVENT_NOTIFY_SIGNAL EVT_NOTIFY_SIGNAL
#define EFI_EVENT_NOTIFY_SIGNAL_ALL 0x00000400

#ifdef EFI_DEBUG
#define TRACE(Arguments) DEBUG (Arguments)
#else // EFI_DEBUG==0
#define TRACE(Arguments)
#endif

#define TRACE_DXE_CORE 0x20

#define END_DEVICE_PATH                 END_DEVICE_PATH_TYPE
#define END_ENTIRE_SUBTYPE              END_ENTIRE_DEVICE_PATH_SUBTYPE
#define NODE_LENGTH(pPath) ((pPath)->Length[0]+((pPath)->Length[1]<<8))
#define NEXT_NODE(pPath) ((EFI_DEVICE_PATH_PROTOCOL*)((UINT8*)(pPath)+NODE_LENGTH(pPath)))
#define isEndNode(pDp) ((pDp)->Type==END_DEVICE_PATH)

#ifndef BIT01
#define 	BIT01	0x00000002
#endif

#define ___INTERNAL_CONVERT_TO_STRING___(a) #a
#define CONVERT_TO_STRING(a) ___INTERNAL_CONVERT_TO_STRING___(a)
#define ___INTERNAL_CONVERT_TO_WSTRING___(a) L###a
#define CONVERT_TO_WSTRING(a) ___INTERNAL_CONVERT_TO_WSTRING___(a)

#define EFI_SCAN_DEL		    0x08
#define EFI_SCAN_ESC		    0x17

#define SHIFT_STATE_VALID       0x80000000
#define MENU_KEY_PRESSED        0x00000100
#define SYS_REQ_PRESSED         0x00000200

#define guidcmp(g1,g2) MemCmp(g1,g2,sizeof(EFI_GUID))

// UnicodeCollation.h
#define EFI_UNICODE_COLLATION2_PROTOCOL_GUID EFI_UNICODE_COLLATION_PROTOCOL2_GUID 
typedef EFI_UNICODE_COLLATION_PROTOCOL EFI_UNICODE_COLLATION2_PROTOCOL;

typedef UINT16 EFI_STRING_ID;

extern EFI_SYSTEM_TABLE			*pST;
extern EFI_BOOT_SERVICES		*pBS;
extern EFI_RUNTIME_SERVICES		*pRS; 

extern EFI_GUID gEfiEventReadyToBootGuid;

VOID MemCpy(VOID* pDestination, VOID* pSource, UINTN Length);
INTN MemCmp(VOID* pDestination, VOID* pSource, UINTN Length);


EFI_STATUS IsValidDevicePath(
    IN EFI_DEVICE_PATH_PROTOCOL *pDp
); 


VOID InitAmiLib(
	IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable
);

UINTN
Swprintf_s_va_list (
  CHAR16        *StartOfBuffer,
  UINTN         BufferSize,
  CHAR16		*FormatString,
  VA_LIST       Marker
  );

////////////////////// Macros required in EdkHelpr.c /////////
#define LANGUAGE_CODE_ENGLISH    "en-US"
#define IsEndOfLanguage(Lang) (*(Lang) == 0 || *(Lang) == ';')
#define PREFIX_SIGN   0x02
#define PREFIX_BLANK  0x04
#define LONG_TYPE     0x10

#define STRING_W(_s)                              L##_s

#define MAX_DP_SIZE 0x10000

////////////////////////////////////////////////////////

/**
AmiCompatibilityPkg macros over
**/
#ifndef _AMI_EFI_KEY_DATA_
#define _AMI_EFI_KEY_DATA_
typedef struct {
  EFI_INPUT_KEY  Key;
} AMI_EFI_KEY_DATA;
#endif

#endif
