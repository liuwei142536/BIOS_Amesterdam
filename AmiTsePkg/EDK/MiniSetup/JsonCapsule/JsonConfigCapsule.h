//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#ifndef __JSON_CONFIG_CAPSULE_H__
#define __JSON_CONFIG_CAPSULE_H__

#include <AmiDxeLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseLib/BaseLibInternals.h>
#include <bootflow.h>
#include "Uefi.h"
#include "Library/MemoryAllocationLib.h"
#include "Library/BaseMemoryLib.h"
#include "Library/BaseLib.h"
#include "setupdata.h"
#include "NVRAM/NVRAM.h"
#include "variable.h"
#include "mem.h"
#include <Library/PcdLib.h>
/*#include "minisetup.h"
//
//Macro definitions
//
//#define EFI_PROCESS_CAPSULE_GUID \
//    {0x7a05e20a, 0x90f3, 0x47b5, {0xb3, 0xd1, 0xd4, 0x89, 0xd3, 0x11, 0x7b, 0x87}}
*/
#ifndef EFI_JSON_CAPSULE_ID_GUID
#define EFI_JSON_CAPSULE_ID_GUID \
    {0x67d6f4cd, 0xd6b8, 0x4573,{0xbf, 0x4a, 0xde, 0x5e, 0x25, 0x2d, 0x61, 0xae }}
#endif

#define JSON_CONFIG_FFS_GUID \
    {0x110dc5d3, 0xed94, 0x49c1, {0x9f, 0x2d, 0x13, 0xe1, 0x29, 0xba, 0x22, 0xf4}} 

#define EFI_CAPSULE_REPORT_GUID \
    {0x39b68c46, 0xf7fb, 0x441b,{0xb6, 0xec, 0x16, 0xb0, 0xf6, 0x98, 0x21, 0xf3 }}

#define MAX_BUFFER_SIZE  1024
#define MAX_ERROR_BUFFER_LENGTH 30

#ifndef EFI_JSON_CONFIG_DATA_TABLE_GUID
#define EFI_JSON_CONFIG_DATA_TABLE_GUID \
        {0x87367f87, 0x1119, 0x41ce,{0xaa, 0xec, 0x8b, 0xe0, 0x11, 0x1f, 0x55, 0x8a }}
#endif

#ifndef EFI_OS_INDICATIONS_JSON_CONFIG_DATA_REFRESH
#define EFI_OS_INDICATIONS_JSON_CONFIG_DATA_REFRESH  0x0000000000000080
#endif

//
//Structure definitions
//
typedef struct {
  UINT32 ConfigDataLength;
  UINT8  ConfigData[1];
} EFI_JSON_CONFIG_DATA_ITEM;


typedef struct {
  UINT32      Version;
  UINT32      TotalLength;
//EFI_JSON_CONFIG_DATA_ITEM ConfigDataList[];
} EFI_JSON_CAPSULE_CONFIG_DATA;

#pragma pack(1)
typedef struct {
UINT32 Version;
UINT32 CapsuleId;
UINT32 PayloadLength;
// UINT8 Payload[];
} EFI_JSON_CAPSULE_HEADER;
#pragma pack ()


typedef struct {
UINT32 VariableTotalSize;
UINT32 Reserved; //for alignment
EFI_GUID CapsuleGuid;
EFI_TIME CapsuleProcessed;
EFI_STATUS CapsuleStatus;
} EFI_CAPSULE_RESULT_VARIABLE_HEADER;


typedef struct {
UINT32 Version;
UINT32 CapsuleId;
UINT32 RespLength;
// UINT8 Resp[];
} EFI_CAPSULE_RESULT_VARIABLE_JSON;

typedef struct _AMI_PROCESS_CAPSULE_PROTOCOL AMI_PROCESS_CAPSULE_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *AMI_PROCESS_CAPSULE_PROTOCOL_PROCESS_CAPSULE) (
    IN AMI_PROCESS_CAPSULE_PROTOCOL *This,             //! pointer to AMI_PROCESS_CAPSULE_PROTOCOL instance
    IN VOID                         *Image,            //! pointer to CapsuleImage, including Capsule header
    IN UINTN                        ImageSize          //! Size of capsule including header
 );


struct _AMI_PROCESS_CAPSULE_PROTOCOL
{
    AMI_PROCESS_CAPSULE_PROTOCOL_PROCESS_CAPSULE   ProcessCapsule;
};

typedef struct {
    BOOLEAN IsSigned;
    UINT64 Min;
    UINT64 Max;
} VALIDATE_MIN_MAX; 

typedef struct {
    UINT8  OptionWidth; 
    UINT64 OptionCount;
    UINT64 OptionList[1]; // Size will be allocated based on options count.
} VALIDATE_OPTIONS; 

typedef union {
    VALIDATE_MIN_MAX MinMax;
    VALIDATE_OPTIONS Options;
} VALIDATE_DATA;

typedef struct {
    CHAR16 Varname[100];
    EFI_GUID VarGuid;
    UINTN VarID;
    UINT32 Attributes;
}VAR_LIST;

typedef struct{
    CHAR16 *MappingID;
    UINTN VarId;
    UINTN Offset;
    UINTN Width;
    UINT8 ControlType;
    UINT8 AccessLevel;
    VALIDATE_DATA *ValidateData;
}CONFIG_LOOK_UP_TABLE;

typedef enum {
    CurrentValue,
    StdDefaultValue,
    MfgDefaultValue
} VAL_TYPE;

//
// Function declarations
//

EFI_STATUS 
InstallJsonProcessCapsuleProtocol(
    VOID
);

EFI_STATUS 
InitCapsuleData(
    VOID
);

EFI_STATUS 
GetConfigFileBufferFromFfs(
    UINT8 **FileBuffer,
    UINTN *BufferSize
);

EFI_STATUS 
InitConfigLookUpTable(
    UINT8* FileBuffer
);

EFI_STATUS 
GetVairableIndex(
    UINTN VarID,
    UINTN *VarIndex
);

VOID 
CleanUpLookUpTable(
    VOID
);

VOID
CleanUpVariableTable( 
    VOID 
);

EFI_STATUS
ProcessJsonConfigCapsule (
    IN AMI_PROCESS_CAPSULE_PROTOCOL *This,             //! pointer to AMI_PROCESS_CAPSULE_PROTOCOL instance
    IN VOID                         *Image,            //! pointer to CapsuleImage, including Capsule header
    IN UINTN                        ImageSize          //! Size of capsule including header
);

EFI_STATUS
UpdateJsonDataWithValues(
    VOID
);

EFI_STATUS
GetControlsCurrentValue(
    UINTN TableIndex,
    VOID *Buffer
);

CHAR8* 
ConvVfrNumToString(
    UINTN ControlDataWidth,
    VOID *Buffer
);

UINT8 
IsNegative(
    VOID *Buffer,
    UINTN Size
);

EFI_STATUS
ProcessCapsuleFileBuffer(
      void *Image,
      UINTN ImageSize
);

VOID EFIAPI
PublishFFSJsonPayLoadToOS(
    IN EFI_EVENT    Event, 
    IN VOID         *Context        
);

EFI_STATUS 
GetLookUpTableIndex(
    CHAR16* MappingID,
    UINTN *TableIndex
);

EFI_STATUS 
SetVariableBufferInNvram(
    VOID
);

EFI_STATUS
SetControlCurrentValue(
    UINTN TableIndex,
    VOID *Data
);

EFI_STATUS
GetVariableBufferFromNvram(
    VOID           
);

BOOLEAN
IsOsIndicationSupported(
    VOID      
);

EFI_STATUS
WriteNewCapsuleResultVariable (
    IN VOID    *CapsuleResult,
    IN UINTN   CapsuleResultSize
);

INTN
GetNewCapsuleResultIndex (
    VOID
);

INTN
GetCurrentCapsuleLastIndex (
    VOID
);

EFI_STATUS
UpadteResultVariable(
    INTN Index,
    char* MappingId,
    EFI_STATUS FailStatus
);

CHAR8*
GetStatusInString(
    EFI_STATUS Status
);

EFI_STATUS
WriteJsonCapsuleResult(
    EFI_STATUS Status,
    UINT32 CapsuleId,
    UINT32 Flags
);

VOID 
InitGlobalPointers(
    VOID 
);

/****** DO NOT WRITE BELOW THIS LINE *******/
#endif
