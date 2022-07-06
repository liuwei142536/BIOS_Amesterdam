//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************


/** @file SmbiosElog.h
    SmbiosElog header file function declarations, structures, constants

**/


#ifndef _EFI_SMBIOS_ELOG_H_
#define _EFI_SMBIOS_ELOG_H_
#define VFRCOMPILE


// Adding all the header file's


//---------------------------------------------------------------------------

#include <Include/AmiLib.h>
#include <SmbiosElogFlashFuncs.h>
#include <SmbiosElogType15.h>
#include <Setup.h>

// Including all the library's

#include <Library/MemoryAllocationLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/PcdLib.h>

// Including all the protocol's

#include <Protocol/AmiSmbios.h>
#include <Protocol/SmbiosGetFlashDataProtocol.h>
#include <Protocol/SmbiosElogSupport.h>
#include <Protocol/GpnvRedirProtocol.h>
#include <Protocol/DataHub.h>
#include <Protocol/ReportStatusCodeHandler.h>
#include <Protocol/FlashProtocol.h>

#define MAX_EVENT_TYPES 0xFF


// To calculate buffer size divide total event log length with minimum event 
// size possible. This will give max no events can be stored.

#define MAX_NO_EVENTS_STORED \
       (LOCAL_EVENT_LOG_LENGTH+1)-sizeof(EFI_GPNV_ERROR_HEADER_TYPE1)/(sizeof(HANDLE_DATA) + sizeof(EFI_GPNV_ERROR_EVENT))

// Macros For accessing time from RTC

#ifndef RTC_ADDRESS_REGISTER
#define RTC_ADDRESS_REGISTER        0x70
#endif

#ifndef RTC_DATA_REGISTER
#define RTC_DATA_REGISTER           0x71
#endif

#ifndef RTC_SECONDS_OFFSET
#define RTC_SECONDS_OFFSET           0   // R/W  Range 0..59
#endif

#ifndef RTC_MINUTES_OFFSET
#define RTC_MINUTES_OFFSET           2   // R/W  Range 0..59
#endif

#ifndef RTC_HOURS_OFFSET
#define RTC_HOURS_OFFSET             4   // R/W  Range 1..12 or 0..23 Bit 7 is AM/PM
#endif

#ifndef RTC_DAY_OFFSET
#define RTC_DAY_OFFSET               7   // R/W  Range 1..31
#endif

#ifndef RTC_MONTH_OFFSET
#define RTC_MONTH_OFFSET             8   // R/W  Range 1..12
#endif

#ifndef RTC_YEAR_OFFSET
#define RTC_YEAR_OFFSET              9   // R/W  Range 0..99
#endif

#pragma pack(1)

//
// Dallas DS12C887 Real Time Clock
//
#define RTC_ADDRESS_SECONDS           0   // R/W  Range 0..59
#define RTC_ADDRESS_SECONDS_ALARM     1   // R/W  Range 0..59
#define RTC_ADDRESS_MINUTES           2   // R/W  Range 0..59
#define RTC_ADDRESS_MINUTES_ALARM     3   // R/W  Range 0..59
#define RTC_ADDRESS_HOURS             4   // R/W  Range 1..12 or 0..23 Bit 7 is AM/PM
#define RTC_ADDRESS_HOURS_ALARM       5   // R/W  Range 1..12 or 0..23 Bit 7 is AM/PM
#define RTC_ADDRESS_DAY_OF_THE_WEEK   6   // R/W  Range 1..7
#define RTC_ADDRESS_DAY_OF_THE_MONTH  7   // R/W  Range 1..31
#define RTC_ADDRESS_MONTH             8   // R/W  Range 1..12
#define RTC_ADDRESS_YEAR              9   // R/W  Range 0..99
#define RTC_ADDRESS_REGISTER_A        10  // R/W[0..6]  R0[7]
#define RTC_ADDRESS_REGISTER_B        11  // R/W
#define RTC_ADDRESS_REGISTER_C        12  // RO
#define RTC_ADDRESS_REGISTER_D        13  // RO
#define RTC_ADDRESS_CENTURY           50  // R/W  Range 19..20 Bit 8 is R/W

#pragma pack(1)

/** @internal
    Register A
*/

typedef struct {
    UINT8 Rs : 4;   // Rate Selection Bits
    UINT8 Dv : 3;   // Divisor
    UINT8 Uip : 1;  // Update in progress
} RTC_REGISTER_A_BITS;

typedef union {
    RTC_REGISTER_A_BITS Bits;
    UINT8               Data;
} RTC_REGISTER_A;

/** @internal
    Register B
*/

typedef struct {
    UINT8 Dse : 1;  // 0 - Daylight saving disabled  1 - Daylight savings enabled
    UINT8 Mil : 1;  // 0 - 12 hour mode              1 - 24 hour mode
    UINT8 Dm : 1;   // 0 - BCD Format                1 - Binary Format
    UINT8 Sqwe : 1; // 0 - Disable SQWE output       1 - Enable SQWE output
    UINT8 Uie : 1;  // 0 - Update INT disabled       1 - Update INT enabled
    UINT8 Aie : 1;  // 0 - Alarm INT disabled        1 - Alarm INT Enabled
    UINT8 Pie : 1;  // 0 - Periodic INT disabled     1 - Periodic INT Enabled
    UINT8 Set : 1;  // 0 - Normal operation.         1 - Updates inhibited
} RTC_REGISTER_B_BITS;

typedef union {
    RTC_REGISTER_B_BITS Bits;
    UINT8               Data;
} RTC_REGISTER_B;

/** @internal
    Register D
*/

typedef struct {
    UINT8 Reserved : 7; // Read as zero.  Can not be written.
    UINT8 Vrt : 1;      // Valid RAM and Time
} RTC_REGISTER_D_BITS;

typedef union {
    RTC_REGISTER_D_BITS Bits;
    UINT8               Data;
} RTC_REGISTER_D;

#pragma pack()

/**
    Get time from RTC 
 
    @param    Time  the time data read from RTC
                     
    @retval   EFI_DEVICE_ERROR   RTC device error.
    @retval   EFI_SUCCESS        RTC is updated time and ready to use. 
    @retval   EFI_INVALID_PARAMETER  Parameters passed in are invalid. 

**/

EFI_STATUS
GpnvRtcGetTime (
  OUT  EFI_TIME               *Time
);

/** @internal
    Structure definition of the Private data
*/
typedef struct {

// NOTE: These Two must not be changed in Position.For use in Locating the 
// beginning of this structure in memory in both Regular and SMM memory 

    EFI_SM_ELOG_REDIR_PROTOCOL        SmbiosElog;
    AMI_SMBIOS_ELOG_SUPPORT_PROTOCOL  SmbiosElogSupport;

    UINT8*                            StartAddress;
    UINT8*                            EndAddress;
    UINT8*                            NextAddress;
    UINT8                             MECI;
    UINT8                             METW;
    BOOLEAN                           EraseEventLogWhenFull;
    BOOLEAN                           SystemBootRecordLogging;
    BOOLEAN                           EfiStatusCodesEnabled;
    BOOLEAN                           ConvertEfiToLegacyFlag;
    EFI_ELOG_TYPE_DESCRIPTOR          ValidSmbiosTypes[NumOfSupportedSmbiosTypes];
    UINT8                             CopyOfFlashArea[LOCAL_EVENT_LOG_LENGTH+1];
    UINT32                            NumberOfMultipleEventRecords;
    UINT8                             BufferForErasing[BUFFER_SIZE_NEEDED_FOR_ERASING];
    UINT32                            MultiEventOffset[MAX_NO_EVENTS_STORED];
    UINT8                             InternalCounter[MAX_NO_EVENTS_STORED];
    UINT32                            RecordId;
    FLASH_PROTOCOL                    *FlashProtocolPointer;
    BOOLEAN                           IsGPNVAreaFlashed;
} EFI_SMBIOS_ELOG_INSTANCE_DATA;

#pragma pack()

extern  EFI_SMBIOS_ELOG_INSTANCE_DATA*  mRedirSmbiosPrivate;

/**
    Convert RTC Read value of TIME in BCD Format.

    @param   Time       On Input, the time data read from RTC to convert
                        On Output, the time converted to BCD format

**/

VOID
ConvertTimetoBCD (
  OUT  EFI_TIME               *Time
);

/**
    Calls OEM RTC Read functions 

    @param Time - Pointer to the Structure to Read Time from RTC

    @return EFI_STATUS

**/

EFI_STATUS
GpnvRtcRead (
  OUT  EFI_TIME               *Time
);

/**
    Entry point of Smbios Elog Driver

    @param ImageHandle - The Image handle of this driver.
    @param SystemTable - The pointer of EFI_SYSTEM_TABLE.

    @return EFI_STATUS
    @retval EFI_SUCCESS - The driver successfully initialized
    @retval EFI_NOT_FOUND - Needed protocols were not found
	@retval EFI_UNSUPPORTED - Driver doesn't log any event

**/

EFI_STATUS
EFIAPI
InitializeSmbiosElog (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
);

/**
    Sends the Event-Log data to the destination.

    @param This - Instance of EFI_SM_ELOG_REDIR_PROTOCOL.
    @param ElogData - Pointer to the Event-Log data to be recorded.
    @param DataType - Type of Elog Data that is being recorded.
    @param AlertEvent - This is an indication that the input data type is an 
                        Alert.
    @param Size - Size of the data to be logged.
    @param RecordId - Record ID sent by the target.

    @return EFI_STATUS
    @retval EFI_SUCCESS - To indicate successful completion or valid EFI 
                          error code otherwise.
	@retval EFI_NOT_FOUND - Incorrect Type for this Log.	
	@retval EFI_UNSUPPORTED - SmbiosElogging is Deactivated.
	@retval EFI_INVALID_PARAMETER - Invalid pointers.
				  
    @note  N/A

**/

EFI_STATUS
EFIAPI
EfiSetSmbiosElogData (
  IN  EFI_SM_ELOG_REDIR_PROTOCOL        *This,
  IN  UINT8                             *ElogData,
  IN  EFI_SM_ELOG_REDIR_TYPE            DataType,
  IN  BOOLEAN                           AlertEvent,
  IN  UINTN                             Size,
  OUT UINT64                            *RecordId
);

/**
    Gets the Event-Log data from the destination.

    @param This - Instance of EFI_SM_ELOG_REDIR_PROTOCOL.
    @param ElogData - Pointer to the Event-Log data buffer that will contain  
                      the data to be retrieved.
    @param DataType - Type of Elog Data that is being recorded.
    @param Size - Size of the data to be retrieved.
    @param RecordId - This is the RecordId of the next record. If ElogData is 
                      NULL, this gives the RecordId of the first record 
                      available in the database with the correct DataSize. A 
                      value of 0 on  return indicates the last record if the 
                      EFI_STATUS indicates a success.

    @return EFI_STATUS
    @retval EFI_SUCCESS - Event-Log was recorded successfully.
    @retval EFI_INVALID_PARAMETER - Invalid parameter
    @retval EFI_NOT_FOUND - Event-Log target not found.
    @retval EFI_BUFFER_TOO_SMALL - Target buffer is too small to retrieve the 
                                   data.

**/

EFI_STATUS
EFIAPI
EfiGetSmbiosElogData (
  IN EFI_SM_ELOG_REDIR_PROTOCOL         *This,
  IN UINT8                              *ElogData,
  IN EFI_SM_ELOG_REDIR_TYPE             DataType,
  IN OUT UINTN                          *Size,
  IN OUT UINT64                         *RecordId
);

/**
    Erases the Event-Log data from the Flash Part.  This Implementation does 
    not allow the erasing of individual events, only the whole log.  Therefore
    RecordId must always be NULL to return a success.

    @param This - Instance of EFI_SM_ELOG_REDIR_PROTOCOL. 
    @param DataType - Type of Elog Data that is being Erased.   
    @param RecordId - This is the RecordId of the data to be erased. If 
                      RecordId is NULL, all the records on the database are 
                      erased if permitted by the target. Contains the deleted 
                      RecordId on return

    @return EFI_STATUS
    @retval EFI_SUCCESS - Event-Log was Erased successfully.
    @retval EFI_NOT_FOUND - Incorrect Type for this Log.
    @retval EFI_INVALID_PARAMETER - Incorrect parameter. Can not erase 
                                    individual records.

    @note  N/A

**/

EFI_STATUS
EFIAPI
EfiEraseSmbiosElogData (
  IN EFI_SM_ELOG_REDIR_PROTOCOL         *This,
  IN EFI_SM_ELOG_REDIR_TYPE             DataType,
  IN OUT UINT64                         *RecordId
);

/**
    This API enables/Disables Event Log.

    @param This - Instance of EFI_SM_ELOG_REDIR_PROTOCOL.
    @param DataType - Type of Elog Data that is being Activated/Disabled.
    @param EnableElog - Enables (TRUE)/Disables(FALSE) Event Log. If NULL just
                        returns the Current ElogStatus. 
    @param ElogStatus - Current (New) Status of Event Log. Enabled (TRUE),
                        Disabled (FALSE).

    @return EFI_STATUS
    @retval EFI_SUCCESS to indicate successful completion or valid EFI error
                        code otherwise.
     @retval EFI_NOT_FOUND - Incorrect Type for this Log 
     @retval EFI_UNSUPPORTED - Pointer is NULL
    @note  N/A

**/

EFI_STATUS
EFIAPI
EfiActivateSmbiosElog (
  IN  EFI_SM_ELOG_REDIR_PROTOCOL         *This,
  IN  EFI_SM_ELOG_REDIR_TYPE             DataType,
  IN  BOOLEAN                            *EnableElog,
  OUT BOOLEAN                            *ElogStatus
);

/**
	Compares difference in times in the event records to show number of minutes difference.

    @param EventToUse1 - Event to inspect
    @param EventToUse2 - Event to inspect

    @return UINT8 - Number of Minutes difference in Decimal (max of 99)

**/
UINT8
TimeDifference (
  IN  EFI_GPNV_ERROR_EVENT            EventToUse1,
  IN  EFI_GPNV_ERROR_EVENT            EventToUse2
);

/**
	This function will search the local records from the multiple events and
    find whether this event should simply increment a counter already 
    started or start a new counter and allow the system to record the record
    into the flash part.

    @param SmbiosElogPrivateData - Gives access to Private Variables
    @param EventToUse - Header for new event
    @param ExtendedData - Extended data for new event, size is 
                          EventToUse->Length - sizeof(EFI_GPNV_ERROR_EVENT)
    @param RecordId - RecordId of current record location
    @param VariableDataType - The variable data type associated with 
                              the record type

    @return BOOLEAN 
    @retval TRUE - The function has taken care of all needed actions
                   and the record does not need to be recorded into 
                   the flash part.
    @retval FALSE - The function requests that this record be
                    recorded into the flash part.

**/

BOOLEAN
DuplicateRecordAndAlreadyReacted (
  IN  EFI_SMBIOS_ELOG_INSTANCE_DATA      *SmbiosElogPrivateData,
  IN  EFI_GPNV_ERROR_EVENT               *EventToUse,
  IN  UINT8                              *ExtendedData,
  OUT UINT64                             *RecordId,
  UINT8                                  VariableDataType
);

/**
    Fix-up internal Data pointers.

    @param Event - EFI_EVENT_NOTIFY
    @param Context - The Event relative context

    @retval VOID

**/

VOID
EFIAPI
SmbiosElogVirtualAddressChangeEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
);

/**
    Erases the Event-Log data from the Flash Part.

    @param SmbiosElogPrivateData - Gives access to Private Variables to allow 
                                   erasing

    @return EFI_STATUS 
    @retval EFI_SUCCESS - Event-Log data erase successful.

    @note  N/A

**/

EFI_STATUS
EfiEraseSmbiosElogDataLocal (
  EFI_SMBIOS_ELOG_INSTANCE_DATA *SmbiosElogPrivateData
);

/**
    Checks Record to see if anything in structure indicates an invalid or 
    corrupted event.

    @param EventToUse - Pointer to Event to inspect
    @param SmbiosElogPrivateData - Gives access to Private Variable

    @return BOOLEAN 
    @retval FALSE - Invalid Record
    @retval TRUE - Valid Record

**/

BOOLEAN
IsValidEvent (
  IN  EFI_GPNV_ERROR_EVENT            *EventToUse,
  IN  EFI_SMBIOS_ELOG_INSTANCE_DATA   *SmbiosElogPrivateData
);

/**
    Takes an event structure and fills it with the appropriate date and time.
    Fills Type with 0x80 and Length as 0x00 also as defaults.

    @param EventPtr - A pointer to the event structure created here

    @return EFI_STATUS
    @retval EFI_SUCCESS - Event structure filled succesfully.

**/

EFI_STATUS
FillEventWithTimeAndDefaults (
  IN OUT  EFI_GPNV_ERROR_EVENT    *EventPtr
);

/**
    Adds to the event log the record indicating the system has booted. This is
    always the first record on a boot (unless the log was corrupt, in which case
    the first record is the log erased record, followed by the boot record).

    @param VOID

    @return EFI_STATUS 
    @retval EFI_SUCCESS - Added to Memory Copy Properly
    @retval EFI_OUT_OF_RESOURCES - Out of space on flash part

**/

EFI_STATUS
InsertSystemBootRecord (VOID);

/**
    Adds the Type 15 Structure to the DataHub, which will later be retrieved 
    to add to  the SMBIOS Table.

    @param Event - EFI_EVENT_NOTIFY
    @param Context - The Event relative context

    @return EFI_STATUS
    @retval EFI_SUCCESS - Addition of Type 15 Structure to the DataHub is successful.

**/

EFI_STATUS
EFIAPI
AddSMBIOSType15Structure (
  IN EFI_EVENT        Event,
  IN VOID             *Context
);

/**
    Checks if log is full,if full Check Setup Question and erases or does 
    nothing accordingly

    @param SmbiosElogPrivateData - Gives access to Private Variables
    @param EventSize - Size of Event to be added to memory

    @return EFI_STATUS
    @retval EFI_SUCCESS - Successful completion of function
    @retval EFI_OUT_OF_RESOURCES - GPNV log area is full

**/

EFI_STATUS
CheckIfFull (
  IN EFI_SMBIOS_ELOG_INSTANCE_DATA    *SmbiosElogPrivateData,
  IN UINT8                             EventSize
);

/** 
    Hook for doing an erase from the designated area in flash for the Smbios 
    Event Log.

    @param StartAddress - Starting address of the flash 
    @param EndAddress - Ending address of the flash
    @param SmbiosElogPrivateData - Gives access to Private Variables 

    @return EFI_STATUS
    @retval EFI_SUCCESS - Successful completion of the function.

**/


EFI_STATUS
ErrorLoggingEraseCustomBlockRange (
  IN UINT8*                               StartAddress,
  IN UINT8*                               EndAddress,
  IN EFI_SMBIOS_ELOG_INSTANCE_DATA        *SmbiosElogPrivateData
  );

/**
    Function to read the area to place into the Memory Copy.

    @param VOID

    @return EFI_STATUS
    @retval EFI_SUCCESS - Successful completion of the function.

**/

EFI_STATUS
ErrorLoggingReadFlashIntoMemoryCopy (VOID);

/**
    Function checks to see if it is able to write the current section starting
    at StartOffset and ending at EndOffset, and if the section is not able to be
    written without erasing, the whole log is erased and fully rewritten with 
    the copy from memory.  If the section is able to be written without erasing,
    that section is written and we exit.

    @param SmbiosElogPrivateData - Pointer to local information so that local
                                   memory copy may also be altered.
    @param StartOffset - The starting offset in the memory copy of the first
                         byte to check
    @param EndOffset - The ending offset in the memory copy of the last byte
                       to check

    @return EFI_STATUS
    @retval EFI_SUCCESS

**/

EFI_STATUS
RecordMemoryCopyDifferencesToFlash (
  IN EFI_SMBIOS_ELOG_INSTANCE_DATA  *SmbiosElogPrivateData,
  IN UINT32                         StartOffset,
  IN UINT32                         EndOffset
);

/**
    Reset and initialize all data in Memory Copy

    @param SmbiosElogPrivateData - Gives access to Private Variables 

    @return EFI_STATUS
    @retval EFI_SUCCESS - Event-Log was erased successfully.

**/

EFI_STATUS
ResetAllDataInMemoryCopy (
  EFI_SMBIOS_ELOG_INSTANCE_DATA *SmbiosElogPrivateData
);

/**
    The function returns the size of an error log entry for the specified  
    event type.

    @param Event - Pointer to the event structure.
    @param VariableDataType - The format type for the variable data associated
                              with the event.

    @return UINT8 - The size in bytes of the record is returned.

    @note  N/A

**/

UINT8
GetEventRecordSize (
  IN  ALL_GPNV_ERROR   *Event,
  IN  UINT8            VariableDataType
);

/**
    This will be called when ReportStatusCode() is called and here we log 
    the events.

    @param CodeType  - Type of Status Code.
    @param Value     - Value to output for Status Code.
    @param Instance  - Instance Number of this status code.
    @param CallerId  - ID of the caller of this status code.
    @param Data      - Optional data associated with this status code.

    @return EFI_STATUS
    @retval EFI_SUCCESS  Status returned from the function.
    @retval Others       Status returned from SetEventLogData.

**/

EFI_STATUS
EFIAPI
GpnvStatusCodeReportWorker (
  IN EFI_STATUS_CODE_TYPE           CodeType,
  IN EFI_STATUS_CODE_VALUE          Value,
  IN UINT32                         Instance,
  IN CONST EFI_GUID                 *CallerId,
  IN CONST EFI_STATUS_CODE_DATA     *Data OPTIONAL
);

/**
    Get the VariableDataType of Event

    @param EventType - Type of the Event
    @param VaraiableDataType - VaraiableDataType of Event is returned.

    @return EFI_STATUS
    @retval EFI_SUCCESS - Successful completion of the function
    @retval EFI_INVALID_PARAMETER - Type is not known

**/

EFI_STATUS
GetEventVariableDataType (
  IN  UINT8    EventType,
  OUT UINT8    *VariableDataType
);

/**
    Converts counter with number of times multi - event
    occured.

    @param *ExtendedData - Pointer to the Extended elog data
    @param VariableDataType - Type of Elog data
    
    @return UINT8 
    @retval Number of times multi - event occured.

**/

UINT8
GetCountFromMultiEventCounter (
  IN  UINT8     *ExtendedData,
  IN  UINT8     VariableDataType
);
// FOR ERROR TRANSLATION


//----------------------------------------------------------------------------
//  Extended Data Descriptions
//----------------------------------------------------------------------------
//
//  if  System Management (Type 0x10)
//      bit  15     = bit 31 of dword (bits 30 to 15 of dword are ignored)
//      bits 14-0   = bits 14-0 of dword
//
//
//  if  Post Error (Type 0x08)
//      bits 15-0   = bit number in POST?s 2 DWORDS
//              1st DWORD Starts at bit 0, 2nd DWORD starts at bit 32
//
//  if  Other Error w/ Handle
//      bits 15-0	= Handle
//
//
//  else bits 15-0 = 0xFFFF
//----------------------------------------------------------------------------
#pragma pack(1)
typedef struct {
    UINT8  Type;
    UINT16 ExtendedData;
} LEGACY_CODE_STRUCTURE;
#pragma pack ()

/**
    Converts EFI status code value to legacy code

    @param EfiStatusodeValue - EFI Status code value
    @param LegacyCode - Legacy code

    @return EFI_STATUS
    @retval EFI_SUCCESS - Conversion successful

**/

EFI_STATUS
ConvertEfiToLegacy (
  IN     UINT32                       EfiStatusCodeValue,
  IN OUT LEGACY_CODE_STRUCTURE        *LegacyCode
);

/** @internal
    Status code to smbios table
*/

typedef struct {
    UINT32                EfiValue;
    LEGACY_CODE_STRUCTURE LegacyErrorCode; 
} STATUS_CODE_TO_SMBIOS_TYPE_TABLE;

typedef UINT32 EFI_STATUS_CODE_TYPE;
typedef UINT32 EFI_STATUS_CODE_VALUE;

EFI_GUID gEfiStatusCodeGuid;

// Transition table to convert EFI status code to legacy status code


static STATUS_CODE_TO_SMBIOS_TYPE_TABLE TranslationTable1[] = {
//      { EFI_COMPUTING_UNIT_UNSPECIFIED | EFI_CU_EC_NON_SPECIFIC,         {0,0} },
//      { EFI_COMPUTING_UNIT_UNSPECIFIED | EFI_CU_EC_DISABLED,         {0,0} },
//      { EFI_COMPUTING_UNIT_UNSPECIFIED | EFI_CU_EC_NOT_SUPPORTED,         {0,0} },
//      { EFI_COMPUTING_UNIT_UNSPECIFIED | EFI_CU_EC_NOT_DETECTED,         {0,0} },
//      { EFI_COMPUTING_UNIT_UNSPECIFIED | EFI_CU_EC_NOT_CONFIGURED,         {0,0} },
    
//      { EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_EC_NON_SPECIFIC,     {0,0} },
//      { EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_EC_DISABLED,         {0,0} },
//      { EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_EC_NOT_SUPPORTED,     {0,0} },
//      { EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_EC_NOT_DETECTED,     {0,0} },
//      { EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_EC_NOT_CONFIGURED,     {0,0} },
//      { EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_EC_INVALID_TYPE,      {0,0} },
//      { EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_EC_INVALID_SPEED,     {0,0} },
//      { EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_EC_MISMATCH,      {0,0} },
//      { EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_EC_TIMER_EXPIRED,     {0,0} },
//      { EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_EC_SELF_TEST,     {0,0} },
//      { EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_EC_INTERNAL,         {0,0} },
      { EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_EC_THERMAL,         {EFI_EVENT_LOG_TYPE_SYSTEM_LIMIT_EXCEEDED,0x0011} },
      { EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_EC_LOW_VOLTAGE,     {EFI_EVENT_LOG_TYPE_SYSTEM_LIMIT_EXCEEDED,0x0000} }, // ???
      { EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_EC_HIGH_VOLTAGE,     {EFI_EVENT_LOG_TYPE_SYSTEM_LIMIT_EXCEEDED,0x0005} }, // ???
      { EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_EC_CACHE,         {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CACHE_MEM_ERROR_BIT} },
//      { EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_EC_MICROCODE_UPDATE,     {0,0} },
//      { EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_EC_CORRECTABLE,     {0,0} },
//      { EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_EC_UNCORRECTABLE,     {0,0} },
    
//      { EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_EC_NON_SPECIFIC,     {0,0} },
//      { EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_EC_DISABLED,     {0,0} },
//      { EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_EC_NOT_SUPPORTED,     {0,0} },
//      { EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_EC_NOT_DETECTED,     {0,0} },
//      { EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_EC_NOT_CONFIGURED,      {0,0} },
//      { EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_HARD_FAIL,     {0,0} },
//      { EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_SOFT_FAIL,     {0,0} },
//      { EFI_COMPUTING_UNIT_FIRMWARE_PROCESSOR | EFI_CU_FP_EC_COMM_ERROR,     {0,0} },
    
//      { EFI_COMPUTING_UNIT_IO_PROCESSOR | EFI_CU_EC_NON_SPECIFIC,         {0,0} },
//      { EFI_COMPUTING_UNIT_IO_PROCESSOR | EFI_CU_EC_DISABLED,          {0,0} },
//      { EFI_COMPUTING_UNIT_IO_PROCESSOR | EFI_CU_EC_NOT_SUPPORTED,         {0,0} },
//      { EFI_COMPUTING_UNIT_IO_PROCESSOR | EFI_CU_EC_NOT_DETECTED,         {0,0} },
//      { EFI_COMPUTING_UNIT_IO_PROCESSOR | EFI_CU_EC_NOT_CONFIGURED,     {0,0} },
    
      { EFI_COMPUTING_UNIT_CACHE | EFI_CU_EC_NON_SPECIFIC,            {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CACHE_MEM_ERROR_BIT} },
      { EFI_COMPUTING_UNIT_CACHE | EFI_CU_EC_DISABLED,            {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CACHE_MEM_ERROR_BIT} },
      { EFI_COMPUTING_UNIT_CACHE | EFI_CU_EC_NOT_SUPPORTED,            {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CACHE_MEM_ERROR_BIT} },
      { EFI_COMPUTING_UNIT_CACHE | EFI_CU_EC_NOT_DETECTED,            {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CACHE_MEM_ERROR_BIT} },
      { EFI_COMPUTING_UNIT_CACHE | EFI_CU_EC_NOT_CONFIGURED,        {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CACHE_MEM_ERROR_BIT} },
      { EFI_COMPUTING_UNIT_CACHE | EFI_CU_CACHE_EC_INVALID_TYPE,        {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CACHE_MEM_ERROR_BIT} },
      { EFI_COMPUTING_UNIT_CACHE | EFI_CU_CACHE_EC_INVALID_SPEED,        {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CACHE_MEM_ERROR_BIT} },
      { EFI_COMPUTING_UNIT_CACHE | EFI_CU_CACHE_EC_INVALID_SIZE,        {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CACHE_MEM_ERROR_BIT} },
      { EFI_COMPUTING_UNIT_CACHE | EFI_CU_CACHE_EC_MISMATCH,        {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CACHE_MEM_ERROR_BIT} },
    
//      { EFI_COMPUTING_UNIT_MEMORY | EFI_CU_EC_NON_SPECIFIC,          {0,0} },
//      { EFI_COMPUTING_UNIT_MEMORY | EFI_CU_EC_DISABLED,             {0,0} },
//      { EFI_COMPUTING_UNIT_MEMORY | EFI_CU_EC_NOT_SUPPORTED,         {0,0} },
//      { EFI_COMPUTING_UNIT_MEMORY | EFI_CU_EC_NOT_DETECTED,         {0,0} },
//      { EFI_COMPUTING_UNIT_MEMORY | EFI_CU_EC_NOT_CONFIGURED,         {0,0} },
//      { EFI_COMPUTING_UNIT_MEMORY | EFI_CU_MEMORY_EC_INVALID_TYPE,      {0,0} },
//      { EFI_COMPUTING_UNIT_MEMORY | EFI_CU_MEMORY_EC_INVALID_SPEED,     {0,0} },
//      { EFI_COMPUTING_UNIT_MEMORY | EFI_CU_MEMORY_EC_CORRECTABLE,         {0,0} },
//      { EFI_COMPUTING_UNIT_MEMORY | EFI_CU_MEMORY_EC_UNCORRECTABLE,     {0,0} },
//      { EFI_COMPUTING_UNIT_MEMORY | EFI_CU_MEMORY_EC_SPD_FAIL,         {0,0} },
//      { EFI_COMPUTING_UNIT_MEMORY | EFI_CU_MEMORY_EC_INVALID_SIZE,         {0,0} },
//      { EFI_COMPUTING_UNIT_MEMORY | EFI_CU_MEMORY_EC_MISMATCH,         {0,0} },
//      { EFI_COMPUTING_UNIT_MEMORY | EFI_CU_MEMORY_EC_S3_RESUME_FAIL,     {0,0} },
//      { EFI_COMPUTING_UNIT_MEMORY | EFI_CU_MEMORY_EC_UPDATE_FAIL,         {0,0} },
//      { EFI_COMPUTING_UNIT_MEMORY | EFI_CU_MEMORY_EC_NONE_DETECTED,     {0,0} },
//      { EFI_COMPUTING_UNIT_MEMORY | EFI_CU_MEMORY_EC_NONE_USEFUL,         {0,0} },
      
//      { EFI_COMPUTING_UNIT_CHIPSET | EFI_CU_EC_NON_SPECIFIC,         {0,0} },
//      { EFI_COMPUTING_UNIT_CHIPSET | EFI_CU_EC_DISABLED,             {0,0} },
//      { EFI_COMPUTING_UNIT_CHIPSET | EFI_CU_EC_NOT_SUPPORTED,          {0,0} },
//      { EFI_COMPUTING_UNIT_CHIPSET | EFI_CU_EC_NOT_DETECTED,         {0,0} },
//      { EFI_COMPUTING_UNIT_CHIPSET | EFI_CU_EC_NOT_CONFIGURED,         {0,0} },
    
//      { EFI_PERIPHERAL_UNSPECIFIED | EFI_P_EC_NON_SPECIFIC,         {0,0} },
//      { EFI_PERIPHERAL_UNSPECIFIED | EFI_P_EC_DISABLED,             {0,0} },
//      { EFI_PERIPHERAL_UNSPECIFIED | EFI_P_EC_NOT_SUPPORTED,          {0,0} },
//      { EFI_PERIPHERAL_UNSPECIFIED | EFI_P_EC_NOT_DETECTED,         {0,0} },
//      { EFI_PERIPHERAL_UNSPECIFIED | EFI_P_EC_NOT_CONFIGURED,         {0,0} },
//      { EFI_PERIPHERAL_UNSPECIFIED | EFI_P_EC_INTERFACE_ERROR,         {0,0} },
//      { EFI_PERIPHERAL_UNSPECIFIED | EFI_P_EC_CONTROLLER_ERROR,         {0,0} },
//      { EFI_PERIPHERAL_UNSPECIFIED | EFI_P_EC_INPUT_ERROR,             {0,0} },
//      { EFI_PERIPHERAL_UNSPECIFIED | EFI_P_EC_OUTPUT_ERROR,         {0,0} },
      { EFI_PERIPHERAL_UNSPECIFIED | EFI_P_EC_RESOURCE_CONFLICT,         {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW2_SYS_BOARD_DEV_CONFLICT_BIT} },
    
//      { EFI_PERIPHERAL_KEYBOARD | EFI_P_EC_NON_SPECIFIC,             {0,0} },
//      { EFI_PERIPHERAL_KEYBOARD | EFI_P_EC_DISABLED,             {0,0} },
//      { EFI_PERIPHERAL_KEYBOARD | EFI_P_EC_NOT_SUPPORTED,             {0,0} },
//      { EFI_PERIPHERAL_KEYBOARD | EFI_P_EC_NOT_DETECTED,             {0,0} },
//      { EFI_PERIPHERAL_KEYBOARD | EFI_P_EC_NOT_CONFIGURED,             {0,0} },
//      { EFI_PERIPHERAL_KEYBOARD | EFI_P_EC_INTERFACE_ERROR,         {0,0} },
      { EFI_PERIPHERAL_KEYBOARD | EFI_P_EC_CONTROLLER_ERROR,         {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_KBD_CONTR_NOT_FUNC_BIT} },
//      { EFI_PERIPHERAL_KEYBOARD | EFI_P_EC_INPUT_ERROR,             {0,0} },
//      { EFI_PERIPHERAL_KEYBOARD | EFI_P_EC_OUTPUT_ERROR,             {0,0} },
//      { EFI_PERIPHERAL_KEYBOARD | EFI_P_EC_RESOURCE_CONFLICT,          {0,0} },
      { EFI_PERIPHERAL_KEYBOARD | EFI_P_KEYBOARD_EC_LOCKED,         {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_KBD_LOCKED_BIT} },
      { EFI_PERIPHERAL_KEYBOARD | EFI_P_KEYBOARD_EC_STUCK_KEY,          {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_KBD_NOT_FUNC_BIT} },
    
      { EFI_PERIPHERAL_MOUSE | EFI_P_EC_NON_SPECIFIC,         {EFI_EVENT_LOG_TYPE_POST_ERROR,10} },  // FOR TESTING ONLY!!!!!!!!!
      { EFI_PERIPHERAL_MOUSE | EFI_P_EC_DISABLED,             {EFI_EVENT_LOG_TYPE_POST_ERROR,10} },  // FOR TESTING ONLY!!!!!!!!!
      { EFI_PERIPHERAL_MOUSE | EFI_P_EC_NOT_SUPPORTED,         {EFI_EVENT_LOG_TYPE_POST_ERROR,10} },  // FOR TESTING ONLY!!!!!!!!!
      { EFI_PERIPHERAL_MOUSE | EFI_P_EC_NOT_DETECTED,         {EFI_EVENT_LOG_TYPE_POST_ERROR,10} },  // FOR TESTING ONLY!!!!!!!!!
      { EFI_PERIPHERAL_MOUSE | EFI_P_EC_NOT_CONFIGURED,          {EFI_EVENT_LOG_TYPE_POST_ERROR,10} },  // FOR TESTING ONLY!!!!!!!!!
      { EFI_PERIPHERAL_MOUSE | EFI_P_EC_INTERFACE_ERROR,         {EFI_EVENT_LOG_TYPE_POST_ERROR,10} },  // FOR TESTING ONLY!!!!!!!!!
      { EFI_PERIPHERAL_MOUSE | EFI_P_EC_CONTROLLER_ERROR,        {EFI_EVENT_LOG_TYPE_POST_ERROR,10} },  // FOR TESTING ONLY!!!!!!!!!
      { EFI_PERIPHERAL_MOUSE | EFI_P_EC_INPUT_ERROR,         {EFI_EVENT_LOG_TYPE_POST_ERROR,10} },  // FOR TESTING ONLY!!!!!!!!!
      { EFI_PERIPHERAL_MOUSE | EFI_P_EC_OUTPUT_ERROR,         {EFI_EVENT_LOG_TYPE_POST_ERROR,10} },  // FOR TESTING ONLY!!!!!!!!!
      { EFI_PERIPHERAL_MOUSE | EFI_P_EC_RESOURCE_CONFLICT,         {EFI_EVENT_LOG_TYPE_POST_ERROR,10} },  // FOR TESTING ONLY!!!!!!!!!
      { EFI_PERIPHERAL_MOUSE | EFI_P_MOUSE_EC_LOCKED,         {EFI_EVENT_LOG_TYPE_POST_ERROR,10} },  // FOR TESTING ONLY!!!!!!!!!
    
//      { EFI_PERIPHERAL_LOCAL_CONSOLE | EFI_P_EC_NON_SPECIFIC,     {0,0} },
//      { EFI_PERIPHERAL_LOCAL_CONSOLE | EFI_P_EC_DISABLED,         {0,0} },
//      { EFI_PERIPHERAL_LOCAL_CONSOLE | EFI_P_EC_NOT_SUPPORTED,     {0,0} },
      { EFI_PERIPHERAL_LOCAL_CONSOLE | EFI_P_EC_NOT_DETECTED,      {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW2_PRIMARY_OUTPUT_DEV_NOT_FOUND_BIT} },
//      { EFI_PERIPHERAL_LOCAL_CONSOLE | EFI_P_EC_NOT_CONFIGURED,       {0,0} },
//      { EFI_PERIPHERAL_LOCAL_CONSOLE | EFI_P_EC_INTERFACE_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_LOCAL_CONSOLE | EFI_P_EC_CONTROLLER_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_LOCAL_CONSOLE | EFI_P_EC_INPUT_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_LOCAL_CONSOLE | EFI_P_EC_OUTPUT_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_LOCAL_CONSOLE | EFI_P_EC_RESOURCE_CONFLICT,  {0,0} },
    
//      { EFI_PERIPHERAL_REMOTE_CONSOLE  | EFI_P_EC_NON_SPECIFIC,      {0,0} },
//      { EFI_PERIPHERAL_REMOTE_CONSOLE  | EFI_P_EC_DISABLED,      {0,0} },
//      { EFI_PERIPHERAL_REMOTE_CONSOLE  | EFI_P_EC_NOT_SUPPORTED,      {0,0} },
//      { EFI_PERIPHERAL_REMOTE_CONSOLE  | EFI_P_EC_NOT_DETECTED,      {0,0} },
//      { EFI_PERIPHERAL_REMOTE_CONSOLE  | EFI_P_EC_NOT_CONFIGURED,   {0,0} },
//      { EFI_PERIPHERAL_REMOTE_CONSOLE  | EFI_P_EC_INTERFACE_ERROR,  {0,0} },
//      { EFI_PERIPHERAL_REMOTE_CONSOLE  | EFI_P_EC_CONTROLLER_ERROR, {0,0} },
//      { EFI_PERIPHERAL_REMOTE_CONSOLE  | EFI_P_EC_INPUT_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_REMOTE_CONSOLE  | EFI_P_EC_OUTPUT_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_REMOTE_CONSOLE  | EFI_P_EC_RESOURCE_CONFLICT,{0,0} },
    
//      { EFI_PERIPHERAL_SERIAL_PORT | EFI_P_EC_NON_SPECIFIC,      {0,0} },
//      { EFI_PERIPHERAL_SERIAL_PORT | EFI_P_EC_DISABLED,          {0,0} },
//      { EFI_PERIPHERAL_SERIAL_PORT | EFI_P_EC_NOT_SUPPORTED,       {0,0} },
//      { EFI_PERIPHERAL_SERIAL_PORT | EFI_P_EC_NOT_DETECTED,      {0,0} },
//      { EFI_PERIPHERAL_SERIAL_PORT | EFI_P_EC_NOT_CONFIGURED,       {0,0} },
//      { EFI_PERIPHERAL_SERIAL_PORT | EFI_P_EC_INTERFACE_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_SERIAL_PORT | EFI_P_EC_CONTROLLER_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_SERIAL_PORT | EFI_P_EC_INPUT_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_SERIAL_PORT | EFI_P_EC_OUTPUT_ERROR,      {0,0} },
      { EFI_PERIPHERAL_SERIAL_PORT | EFI_P_EC_RESOURCE_CONFLICT,      {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW2_SERIAL_PORT_1_RESOURCE_ERROR_BIT} },
     
//      { EFI_PERIPHERAL_PARALLEL_PORT | EFI_P_EC_NON_SPECIFIC,      {0,0} },
//      { EFI_PERIPHERAL_PARALLEL_PORT | EFI_P_EC_DISABLED,          {0,0} },
//      { EFI_PERIPHERAL_PARALLEL_PORT | EFI_P_EC_NOT_SUPPORTED,       {0,0} },
//      { EFI_PERIPHERAL_PARALLEL_PORT | EFI_P_EC_NOT_DETECTED,      {0,0} },
//      { EFI_PERIPHERAL_PARALLEL_PORT | EFI_P_EC_NOT_CONFIGURED,       {0,0} },
//      { EFI_PERIPHERAL_PARALLEL_PORT | EFI_P_EC_INTERFACE_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_PARALLEL_PORT | EFI_P_EC_CONTROLLER_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_PARALLEL_PORT | EFI_P_EC_INPUT_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_PARALLEL_PORT | EFI_P_EC_OUTPUT_ERROR,      {0,0} },
      { EFI_PERIPHERAL_PARALLEL_PORT | EFI_P_EC_RESOURCE_CONFLICT,  {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW2_PARALLEL_PORT_RESOURCE_ERROR_BIT} },
    
//      { EFI_PERIPHERAL_FIXED_MEDIA | EFI_P_EC_NON_SPECIFIC,      {0,0} },
//      { EFI_PERIPHERAL_FIXED_MEDIA | EFI_P_EC_DISABLED,          {0,0} },
//      { EFI_PERIPHERAL_FIXED_MEDIA | EFI_P_EC_NOT_SUPPORTED,       {0,0} },
//      { EFI_PERIPHERAL_FIXED_MEDIA | EFI_P_EC_NOT_DETECTED,      {0,0} },
//      { EFI_PERIPHERAL_FIXED_MEDIA | EFI_P_EC_NOT_CONFIGURED,       {0,0} },
//      { EFI_PERIPHERAL_FIXED_MEDIA | EFI_P_EC_INTERFACE_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_FIXED_MEDIA | EFI_P_EC_CONTROLLER_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_FIXED_MEDIA | EFI_P_EC_INPUT_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_FIXED_MEDIA | EFI_P_EC_OUTPUT_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_FIXED_MEDIA | EFI_P_EC_RESOURCE_CONFLICT,      {0,0} },
     
//      { EFI_PERIPHERAL_REMOVABLE_MEDIA | EFI_P_EC_NON_SPECIFIC,      {0,0} },
//      { EFI_PERIPHERAL_REMOVABLE_MEDIA | EFI_P_EC_DISABLED,      {0,0} },
//      { EFI_PERIPHERAL_REMOVABLE_MEDIA | EFI_P_EC_NOT_SUPPORTED,       {0,0} },
//      { EFI_PERIPHERAL_REMOVABLE_MEDIA | EFI_P_EC_NOT_DETECTED,      {0,0} },
//      { EFI_PERIPHERAL_REMOVABLE_MEDIA | EFI_P_EC_NOT_CONFIGURED,   {0,0} },
//      { EFI_PERIPHERAL_REMOVABLE_MEDIA | EFI_P_EC_INTERFACE_ERROR,  {0,0} },
      { EFI_PERIPHERAL_REMOVABLE_MEDIA | EFI_P_EC_CONTROLLER_ERROR, {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_FLOPPY_CONTR_FAILURE_BIT} },
//      { EFI_PERIPHERAL_REMOVABLE_MEDIA | EFI_P_EC_INPUT_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_REMOVABLE_MEDIA | EFI_P_EC_OUTPUT_ERROR,      {0,0} },
      { EFI_PERIPHERAL_REMOVABLE_MEDIA | EFI_P_EC_RESOURCE_CONFLICT,{EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW2_FDC_RESOURCE_CONFLICT_BIT} },
    
//      { EFI_PERIPHERAL_AUDIO_INPUT | EFI_P_EC_NON_SPECIFIC,      {0,0} },
//      { EFI_PERIPHERAL_AUDIO_INPUT | EFI_P_EC_DISABLED,          {0,0} },
//      { EFI_PERIPHERAL_AUDIO_INPUT | EFI_P_EC_NOT_SUPPORTED,       {0,0} },
//      { EFI_PERIPHERAL_AUDIO_INPUT | EFI_P_EC_NOT_DETECTED,      {0,0} },
//      { EFI_PERIPHERAL_AUDIO_INPUT | EFI_P_EC_NOT_CONFIGURED,       {0,0} },
//      { EFI_PERIPHERAL_AUDIO_INPUT | EFI_P_EC_INTERFACE_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_AUDIO_INPUT | EFI_P_EC_CONTROLLER_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_AUDIO_INPUT | EFI_P_EC_INPUT_ERROR,         {0,0} },
//      { EFI_PERIPHERAL_AUDIO_INPUT | EFI_P_EC_OUTPUT_ERROR,      {0,0} },
      { EFI_PERIPHERAL_AUDIO_INPUT | EFI_P_EC_RESOURCE_CONFLICT,     {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW2_AUDIO_RESOURCE_CONFLICT_BIT} },
    
//      { EFI_PERIPHERAL_AUDIO_OUTPUT | EFI_P_EC_NON_SPECIFIC,      {0,0} },
//      { EFI_PERIPHERAL_AUDIO_OUTPUT | EFI_P_EC_DISABLED,         {0,0} },
//      { EFI_PERIPHERAL_AUDIO_OUTPUT | EFI_P_EC_NOT_SUPPORTED,       {0,0} },
//      { EFI_PERIPHERAL_AUDIO_OUTPUT | EFI_P_EC_NOT_DETECTED,      {0,0} },
//      { EFI_PERIPHERAL_AUDIO_OUTPUT | EFI_P_EC_NOT_CONFIGURED,       {0,0} },
//      { EFI_PERIPHERAL_AUDIO_OUTPUT | EFI_P_EC_INTERFACE_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_AUDIO_OUTPUT | EFI_P_EC_CONTROLLER_ERROR,     {0,0} },
//      { EFI_PERIPHERAL_AUDIO_OUTPUT | EFI_P_EC_INPUT_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_AUDIO_OUTPUT | EFI_P_EC_OUTPUT_ERROR,      {0,0} },
      { EFI_PERIPHERAL_AUDIO_OUTPUT | EFI_P_EC_RESOURCE_CONFLICT,      {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW2_AUDIO_RESOURCE_CONFLICT_BIT} },
    
//      { EFI_PERIPHERAL_LCD_DEVICE | EFI_P_EC_NON_SPECIFIC,      {0,0} },
//      { EFI_PERIPHERAL_LCD_DEVICE | EFI_P_EC_DISABLED,          {0,0} },
//      { EFI_PERIPHERAL_LCD_DEVICE | EFI_P_EC_NOT_SUPPORTED,       {0,0} },
//      { EFI_PERIPHERAL_LCD_DEVICE | EFI_P_EC_NOT_DETECTED,         {0,0} },
//      { EFI_PERIPHERAL_LCD_DEVICE | EFI_P_EC_NOT_CONFIGURED,      {0,0} },
//      { EFI_PERIPHERAL_LCD_DEVICE | EFI_P_EC_INTERFACE_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_LCD_DEVICE | EFI_P_EC_CONTROLLER_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_LCD_DEVICE | EFI_P_EC_INPUT_ERROR,          {0,0} },
//      { EFI_PERIPHERAL_LCD_DEVICE | EFI_P_EC_OUTPUT_ERROR,      {0,0} },
//      { EFI_PERIPHERAL_LCD_DEVICE | EFI_P_EC_RESOURCE_CONFLICT,      {0,0} },
    
//      { EFI_IO_BUS_UNSPECIFIED  | EFI_IOB_EC_NON_SPECIFIC,      {0,0} },
//      { EFI_IO_BUS_UNSPECIFIED  | EFI_IOB_EC_DISABLED,          {0,0} },
//      { EFI_IO_BUS_UNSPECIFIED  | EFI_IOB_EC_NOT_SUPPORTED,       {0,0} },
//      { EFI_IO_BUS_UNSPECIFIED  | EFI_IOB_EC_NOT_DETECTED,      {0,0} },
//      { EFI_IO_BUS_UNSPECIFIED  | EFI_IOB_EC_NOT_CONFIGURED,      {0,0} },
//      { EFI_IO_BUS_UNSPECIFIED  | EFI_IOB_EC_INTERFACE_ERROR,    {0,0} },
//      { EFI_IO_BUS_UNSPECIFIED  | EFI_IOB_EC_CONTROLLER_ERROR,      {0,0} },
//      { EFI_IO_BUS_UNSPECIFIED  | EFI_IOB_EC_READ_ERROR,          {0,0} },
//      { EFI_IO_BUS_UNSPECIFIED  | EFI_IOB_EC_WRITE_ERROR,          {0,0} },
      { EFI_IO_BUS_UNSPECIFIED  | EFI_IOB_EC_RESOURCE_CONFLICT,      {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW2_SYS_BOARD_DEV_CONFLICT_BIT} },
    
//      { EFI_IO_BUS_PCI | EFI_IOB_EC_NON_SPECIFIC,       {0,0} },
//      { EFI_IO_BUS_PCI | EFI_IOB_EC_DISABLED,          {0,0} },
//      { EFI_IO_BUS_PCI | EFI_IOB_EC_NOT_SUPPORTED,       {0,0} },
//      { EFI_IO_BUS_PCI | EFI_IOB_EC_NOT_DETECTED,       {0,0} },
//      { EFI_IO_BUS_PCI | EFI_IOB_EC_NOT_CONFIGURED,      {0,0} },
//      { EFI_IO_BUS_PCI | EFI_IOB_EC_INTERFACE_ERROR,      {0,0} },
//      { EFI_IO_BUS_PCI | EFI_IOB_EC_CONTROLLER_ERROR,      {0,0} },
//      { EFI_IO_BUS_PCI | EFI_IOB_EC_READ_ERROR,          {0,0} },
//      { EFI_IO_BUS_PCI | EFI_IOB_EC_WRITE_ERROR,          {0,0} },
      { EFI_IO_BUS_PCI | EFI_IOB_EC_RESOURCE_CONFLICT,      {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW2_PCI_MEM_CONFLICT_BIT} },
//      { EFI_IO_BUS_PCI | EFI_IOB_PCI_EC_PERR,          {0,0} },
//      { EFI_IO_BUS_PCI | EFI_IOB_PCI_EC_SERR,          {0,0} },
    
//      { EFI_IO_BUS_USB | EFI_IOB_EC_NON_SPECIFIC,       {0,0} },
//      { EFI_IO_BUS_USB | EFI_IOB_EC_DISABLED,          {0,0} },
//      { EFI_IO_BUS_USB | EFI_IOB_EC_NOT_SUPPORTED,       {0,0} },
//      { EFI_IO_BUS_USB | EFI_IOB_EC_NOT_DETECTED,       {0,0} },
//      { EFI_IO_BUS_USB | EFI_IOB_EC_NOT_CONFIGURED,      {0,0} },
//      { EFI_IO_BUS_USB | EFI_IOB_EC_INTERFACE_ERROR,      {0,0} },
//      { EFI_IO_BUS_USB | EFI_IOB_EC_CONTROLLER_ERROR,      {0,0} },
//      { EFI_IO_BUS_USB | EFI_IOB_EC_READ_ERROR,          {0,0} },
//      { EFI_IO_BUS_USB | EFI_IOB_EC_WRITE_ERROR,          {0,0} },
      { EFI_IO_BUS_USB | EFI_IOB_EC_RESOURCE_CONFLICT,      {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW2_SYS_BOARD_DEV_CONFLICT_BIT} },
    
//      { EFI_IO_BUS_IBA | EFI_IOB_EC_NON_SPECIFIC,       {0,0} },
//      { EFI_IO_BUS_IBA | EFI_IOB_EC_DISABLED,          {0,0} },
//      { EFI_IO_BUS_IBA | EFI_IOB_EC_NOT_SUPPORTED,       {0,0} },
//      { EFI_IO_BUS_IBA | EFI_IOB_EC_NOT_DETECTED,          {0,0} },
//      { EFI_IO_BUS_IBA | EFI_IOB_EC_NOT_CONFIGURED,      {0,0} },
//      { EFI_IO_BUS_IBA | EFI_IOB_EC_INTERFACE_ERROR,      {0,0} },
//      { EFI_IO_BUS_IBA | EFI_IOB_EC_CONTROLLER_ERROR,      {0,0} },
//      { EFI_IO_BUS_IBA | EFI_IOB_EC_READ_ERROR,          {0,0} },
//      { EFI_IO_BUS_IBA | EFI_IOB_EC_WRITE_ERROR,          {0,0} },
      { EFI_IO_BUS_IBA | EFI_IOB_EC_RESOURCE_CONFLICT,     {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW2_SYS_BOARD_DEV_CONFLICT_BIT} },
    
//      { EFI_IO_BUS_AGP | EFI_IOB_EC_NON_SPECIFIC,           {0,0} },
//      { EFI_IO_BUS_AGP | EFI_IOB_EC_DISABLED,          {0,0} },
//      { EFI_IO_BUS_AGP | EFI_IOB_EC_NOT_SUPPORTED,       {0,0} },
//      { EFI_IO_BUS_AGP | EFI_IOB_EC_NOT_DETECTED,       {0,0} },
//      { EFI_IO_BUS_AGP | EFI_IOB_EC_NOT_CONFIGURED,      {0,0} },
//      { EFI_IO_BUS_AGP | EFI_IOB_EC_INTERFACE_ERROR,      {0,0} },
//      { EFI_IO_BUS_AGP | EFI_IOB_EC_CONTROLLER_ERROR,      {0,0} },
//      { EFI_IO_BUS_AGP | EFI_IOB_EC_READ_ERROR,          {0,0} },
//      { EFI_IO_BUS_AGP | EFI_IOB_EC_WRITE_ERROR,          {0,0} },
      { EFI_IO_BUS_AGP | EFI_IOB_EC_RESOURCE_CONFLICT,      {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW2_SYS_BOARD_DEV_CONFLICT_BIT} },
    
//      { EFI_IO_BUS_PC_CARD | EFI_IOB_EC_NON_SPECIFIC,       {0,0} },
//      { EFI_IO_BUS_PC_CARD | EFI_IOB_EC_DISABLED,          {0,0} },
//      { EFI_IO_BUS_PC_CARD | EFI_IOB_EC_NOT_SUPPORTED,      {0,0} },
//      { EFI_IO_BUS_PC_CARD | EFI_IOB_EC_NOT_DETECTED,      {0,0} },
//      { EFI_IO_BUS_PC_CARD | EFI_IOB_EC_NOT_CONFIGURED,      {0,0} },
//      { EFI_IO_BUS_PC_CARD | EFI_IOB_EC_INTERFACE_ERROR,      {0,0} },
//      { EFI_IO_BUS_PC_CARD | EFI_IOB_EC_CONTROLLER_ERROR,      {0,0} },
//      { EFI_IO_BUS_PC_CARD | EFI_IOB_EC_READ_ERROR,      {0,0} },
//      { EFI_IO_BUS_PC_CARD | EFI_IOB_EC_WRITE_ERROR,      {0,0} },
      { EFI_IO_BUS_PC_CARD | EFI_IOB_EC_RESOURCE_CONFLICT,  {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW2_SYS_BOARD_DEV_CONFLICT_BIT} },
    
//      { EFI_IO_BUS_LPC | EFI_IOB_EC_NON_SPECIFIC,       {0,0} },
//      { EFI_IO_BUS_LPC | EFI_IOB_EC_DISABLED,          {0,0} },
//      { EFI_IO_BUS_LPC | EFI_IOB_EC_NOT_SUPPORTED,      {0,0} },
//      { EFI_IO_BUS_LPC | EFI_IOB_EC_NOT_DETECTED,       {0,0} },
//      { EFI_IO_BUS_LPC | EFI_IOB_EC_NOT_CONFIGURED,      {0,0} },
//      { EFI_IO_BUS_LPC | EFI_IOB_EC_INTERFACE_ERROR,      {0,0} },
//      { EFI_IO_BUS_LPC | EFI_IOB_EC_CONTROLLER_ERROR,     {0,0} },
//      { EFI_IO_BUS_LPC | EFI_IOB_EC_READ_ERROR,          {0,0} },
//      { EFI_IO_BUS_LPC | EFI_IOB_EC_WRITE_ERROR,          {0,0} },
      { EFI_IO_BUS_LPC | EFI_IOB_EC_RESOURCE_CONFLICT,      {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW2_SYS_BOARD_DEV_CONFLICT_BIT} },
    
//      { EFI_IO_BUS_SCSI  | EFI_IOB_EC_NON_SPECIFIC,       {0,0} },
//      { EFI_IO_BUS_SCSI  | EFI_IOB_EC_DISABLED,          {0,0} },
//      { EFI_IO_BUS_SCSI  | EFI_IOB_EC_NOT_SUPPORTED,       {0,0} },
//      { EFI_IO_BUS_SCSI  | EFI_IOB_EC_NOT_DETECTED,       {0,0} },
//      { EFI_IO_BUS_SCSI  | EFI_IOB_EC_NOT_CONFIGURED,     {0,0} },
//      { EFI_IO_BUS_SCSI  | EFI_IOB_EC_INTERFACE_ERROR,      {0,0} },
//      { EFI_IO_BUS_SCSI  | EFI_IOB_EC_CONTROLLER_ERROR,      {0,0} },
//      { EFI_IO_BUS_SCSI  | EFI_IOB_EC_READ_ERROR,         {0,0} },
//      { EFI_IO_BUS_SCSI  | EFI_IOB_EC_WRITE_ERROR,      {0,0} },
      { EFI_IO_BUS_SCSI  | EFI_IOB_EC_RESOURCE_CONFLICT,     {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW2_PRI_ATA_CNTR_RESOURCE_ERROR_BIT} },
     
//      { EFI_IO_BUS_ATA_ATAPI | EFI_IOB_EC_NON_SPECIFIC,      {0,0} },
//      { EFI_IO_BUS_ATA_ATAPI | EFI_IOB_EC_DISABLED,      {0,0} },
//      { EFI_IO_BUS_ATA_ATAPI | EFI_IOB_EC_NOT_SUPPORTED,      {0,0} },
//      { EFI_IO_BUS_ATA_ATAPI | EFI_IOB_EC_NOT_DETECTED,      {0,0} },
//      { EFI_IO_BUS_ATA_ATAPI | EFI_IOB_EC_NOT_CONFIGURED,      {0,0} },
//      { EFI_IO_BUS_ATA_ATAPI | EFI_IOB_EC_INTERFACE_ERROR,     {0,0} },
//      { EFI_IO_BUS_ATA_ATAPI | EFI_IOB_EC_CONTROLLER_ERROR,    {0,0} },
//      { EFI_IO_BUS_ATA_ATAPI | EFI_IOB_EC_READ_ERROR,    {0,0} },
//      { EFI_IO_BUS_ATA_ATAPI | EFI_IOB_EC_WRITE_ERROR,      {0,0} },
      { EFI_IO_BUS_ATA_ATAPI | EFI_IOB_EC_RESOURCE_CONFLICT,{EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW2_PRI_ATA_CNTR_RESOURCE_ERROR_BIT} },
     
//      { EFI_IO_BUS_FC | EFI_IOB_EC_NON_SPECIFIC,          {0,0} },
//      { EFI_IO_BUS_FC | EFI_IOB_EC_DISABLED,         {0,0} },
//      { EFI_IO_BUS_FC | EFI_IOB_EC_NOT_SUPPORTED,       {0,0} },
//      { EFI_IO_BUS_FC | EFI_IOB_EC_NOT_DETECTED,           {0,0} },
//      { EFI_IO_BUS_FC | EFI_IOB_EC_NOT_CONFIGURED,        {0,0} },
//      { EFI_IO_BUS_FC | EFI_IOB_EC_INTERFACE_ERROR,      {0,0} },
//      { EFI_IO_BUS_FC | EFI_IOB_EC_CONTROLLER_ERROR,     {0,0} },
//      { EFI_IO_BUS_FC | EFI_IOB_EC_READ_ERROR,          {0,0} },
//      { EFI_IO_BUS_FC | EFI_IOB_EC_WRITE_ERROR,          {0,0} },
      { EFI_IO_BUS_FC | EFI_IOB_EC_RESOURCE_CONFLICT,      {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW2_SYS_BOARD_DEV_CONFLICT_BIT} },
    
//      { EFI_IO_BUS_IP_NETWORK | EFI_IOB_EC_NON_SPECIFIC,         {0,0} },
//      { EFI_IO_BUS_IP_NETWORK | EFI_IOB_EC_DISABLED,         {0,0} },
//      { EFI_IO_BUS_IP_NETWORK | EFI_IOB_EC_NOT_SUPPORTED,          {0,0} },
//      { EFI_IO_BUS_IP_NETWORK | EFI_IOB_EC_NOT_DETECTED,         {0,0} },
//      { EFI_IO_BUS_IP_NETWORK | EFI_IOB_EC_NOT_CONFIGURED,         {0,0} },
//      { EFI_IO_BUS_IP_NETWORK | EFI_IOB_EC_INTERFACE_ERROR,        {0,0} },
//      { EFI_IO_BUS_IP_NETWORK | EFI_IOB_EC_CONTROLLER_ERROR,    {0,0} },
//      { EFI_IO_BUS_IP_NETWORK | EFI_IOB_EC_READ_ERROR,         {0,0} },
//      { EFI_IO_BUS_IP_NETWORK | EFI_IOB_EC_WRITE_ERROR,          {0,0} },
      { EFI_IO_BUS_IP_NETWORK | EFI_IOB_EC_RESOURCE_CONFLICT,      {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW2_SYS_BOARD_DEV_CONFLICT_BIT} },
     
//      { EFI_IO_BUS_SMBUS | EFI_IOB_EC_NON_SPECIFIC,      {0,0} },
//      { EFI_IO_BUS_SMBUS | EFI_IOB_EC_DISABLED,          {0,0} },
//      { EFI_IO_BUS_SMBUS | EFI_IOB_EC_NOT_SUPPORTED,      {0,0} },
//      { EFI_IO_BUS_SMBUS | EFI_IOB_EC_NOT_DETECTED,       {0,0} },
//      { EFI_IO_BUS_SMBUS | EFI_IOB_EC_NOT_CONFIGURED,     {0,0} },
//      { EFI_IO_BUS_SMBUS | EFI_IOB_EC_INTERFACE_ERROR,      {0,0} },
//      { EFI_IO_BUS_SMBUS | EFI_IOB_EC_CONTROLLER_ERROR,      {0,0} },
//      { EFI_IO_BUS_SMBUS | EFI_IOB_EC_READ_ERROR,          {0,0} },
//      { EFI_IO_BUS_SMBUS | EFI_IOB_EC_WRITE_ERROR,      {0,0} },
      { EFI_IO_BUS_SMBUS | EFI_IOB_EC_RESOURCE_CONFLICT,      {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW2_SYS_BOARD_DEV_CONFLICT_BIT} },
    
//      { EFI_IO_BUS_I2C | EFI_IOB_EC_NON_SPECIFIC,          {0,0} },
//      { EFI_IO_BUS_I2C | EFI_IOB_EC_DISABLED,        {0,0} },
//      { EFI_IO_BUS_I2C | EFI_IOB_EC_NOT_SUPPORTED,      {0,0} },
//      { EFI_IO_BUS_I2C | EFI_IOB_EC_NOT_DETECTED,         {0,0} },
//      { EFI_IO_BUS_I2C | EFI_IOB_EC_NOT_CONFIGURED,     {0,0} },
//      { EFI_IO_BUS_I2C | EFI_IOB_EC_INTERFACE_ERROR,     {0,0} },
//      { EFI_IO_BUS_I2C | EFI_IOB_EC_CONTROLLER_ERROR,      {0,0} },
//      { EFI_IO_BUS_I2C | EFI_IOB_EC_READ_ERROR,          {0,0} },
//      { EFI_IO_BUS_I2C | EFI_IOB_EC_WRITE_ERROR,          {0,0} },
      { EFI_IO_BUS_I2C | EFI_IOB_EC_RESOURCE_CONFLICT,     {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW2_SYS_BOARD_DEV_CONFLICT_BIT} },
      
//      { EFI_SOFTWARE_UNSPECIFIED | EFI_SW_EC_NON_SPECIFIC,      {0,0} },
//      { EFI_SOFTWARE_UNSPECIFIED | EFI_SW_EC_LOAD_ERROR,          {0,0} },
//      { EFI_SOFTWARE_UNSPECIFIED | EFI_SW_EC_INVALID_PARAMETER,     {0,0} },
//      { EFI_SOFTWARE_UNSPECIFIED | EFI_SW_EC_UNSUPPORTED,       {0,0} },
//      { EFI_SOFTWARE_UNSPECIFIED | EFI_SW_EC_INVALID_BUFFER,      {0,0} },
//      { EFI_SOFTWARE_UNSPECIFIED | EFI_SW_EC_OUT_OF_RESOURCES,      {0,0} },
//      { EFI_SOFTWARE_UNSPECIFIED | EFI_SW_EC_ABORTED,          {0,0} },
//      { EFI_SOFTWARE_UNSPECIFIED | EFI_SW_EC_ILLEGAL_SOFTWARE_STATE,{0,0} },
//      { EFI_SOFTWARE_UNSPECIFIED | EFI_SW_EC_ILLEGAL_HARDWARE_STATE,{0,0} },
//      { EFI_SOFTWARE_UNSPECIFIED | EFI_SW_EC_START_ERROR,          {0,0} },
      { EFI_SOFTWARE_UNSPECIFIED | EFI_SW_EC_BAD_DATE_TIME,        {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CMOS_TIME_NOT_SET_BIT} },
//      { EFI_SOFTWARE_UNSPECIFIED | EFI_SW_EC_CFG_INVALID,          {0,0} },
//      { EFI_SOFTWARE_UNSPECIFIED | EFI_SW_EC_CFG_CLR_REQUEST,      {0,0} },
//      { EFI_SOFTWARE_UNSPECIFIED | EFI_SW_EC_CFG_DEFAULT,          {0,0} },
//      { EFI_SOFTWARE_UNSPECIFIED | EFI_SW_EC_PWD_INVALID,          {0,0} },
//      { EFI_SOFTWARE_UNSPECIFIED | EFI_SW_EC_PWD_CLR_REQUEST,     {0,0} },
//      { EFI_SOFTWARE_UNSPECIFIED | EFI_SW_EC_PWD_CLEARED,          {0,0} },
//      { EFI_SOFTWARE_UNSPECIFIED | EFI_SW_EC_EVENT_LOG_FULL,     {0,0} },

//      { EFI_SOFTWARE_SEC | EFI_SW_EC_NON_SPECIFIC,      {0,0} },
//      { EFI_SOFTWARE_SEC | EFI_SW_EC_LOAD_ERROR,          {0,0} },
//      { EFI_SOFTWARE_SEC | EFI_SW_EC_INVALID_PARAMETER,      {0,0} },
//      { EFI_SOFTWARE_SEC | EFI_SW_EC_UNSUPPORTED,         {0,0} },
//      { EFI_SOFTWARE_SEC | EFI_SW_EC_INVALID_BUFFER,      {0,0} },
//      { EFI_SOFTWARE_SEC | EFI_SW_EC_OUT_OF_RESOURCES,      {0,0} },
//      { EFI_SOFTWARE_SEC | EFI_SW_EC_ABORTED,         {0,0} },
//      { EFI_SOFTWARE_SEC | EFI_SW_EC_ILLEGAL_SOFTWARE_STATE,{0,0} },
//      { EFI_SOFTWARE_SEC | EFI_SW_EC_ILLEGAL_HARDWARE_STATE,{0,0} },
//      { EFI_SOFTWARE_SEC | EFI_SW_EC_START_ERROR,          {0,0} },
      { EFI_SOFTWARE_SEC | EFI_SW_EC_BAD_DATE_TIME,      {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CMOS_TIME_NOT_SET_BIT} },
//      { EFI_SOFTWARE_SEC | EFI_SW_EC_CFG_INVALID,         {0,0} },
//      { EFI_SOFTWARE_SEC | EFI_SW_EC_CFG_CLR_REQUEST,     {0,0} },
//      { EFI_SOFTWARE_SEC | EFI_SW_EC_CFG_DEFAULT,          {0,0} },
//      { EFI_SOFTWARE_SEC | EFI_SW_EC_PWD_INVALID,          {0,0} },
//      { EFI_SOFTWARE_SEC | EFI_SW_EC_PWD_CLR_REQUEST,      {0,0} },
//      { EFI_SOFTWARE_SEC | EFI_SW_EC_PWD_CLEARED,          {0,0} },
//      { EFI_SOFTWARE_SEC | EFI_SW_EC_EVENT_LOG_FULL,      {0,0} },
      
//      { EFI_SOFTWARE_PEI_CORE | EFI_SW_EC_NON_SPECIFIC,          {0,0} },
//      { EFI_SOFTWARE_PEI_CORE | EFI_SW_EC_LOAD_ERROR,         {0,0} },
//      { EFI_SOFTWARE_PEI_CORE | EFI_SW_EC_INVALID_PARAMETER,      {0,0} },
//      { EFI_SOFTWARE_PEI_CORE | EFI_SW_EC_UNSUPPORTED,           {0,0} },
//      { EFI_SOFTWARE_PEI_CORE | EFI_SW_EC_INVALID_BUFFER,          {0,0} },
//      { EFI_SOFTWARE_PEI_CORE | EFI_SW_EC_OUT_OF_RESOURCES,      {0,0} },
//      { EFI_SOFTWARE_PEI_CORE | EFI_SW_EC_ABORTED,             {0,0} },
//      { EFI_SOFTWARE_PEI_CORE | EFI_SW_EC_ILLEGAL_SOFTWARE_STATE,      {0,0} },
//      { EFI_SOFTWARE_PEI_CORE | EFI_SW_EC_ILLEGAL_HARDWARE_STATE,      {0,0} },
//      { EFI_SOFTWARE_PEI_CORE | EFI_SW_EC_START_ERROR,         {0,0} },
      { EFI_SOFTWARE_PEI_CORE | EFI_SW_EC_BAD_DATE_TIME,           {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CMOS_TIME_NOT_SET_BIT} },
//      { EFI_SOFTWARE_PEI_CORE | EFI_SW_EC_CFG_INVALID,          {0,0} },
//      { EFI_SOFTWARE_PEI_CORE | EFI_SW_EC_CFG_CLR_REQUEST,      {0,0} },
//      { EFI_SOFTWARE_PEI_CORE | EFI_SW_EC_CFG_DEFAULT,         {0,0} },
//      { EFI_SOFTWARE_PEI_CORE | EFI_SW_EC_PWD_INVALID,         {0,0} },
//      { EFI_SOFTWARE_PEI_CORE | EFI_SW_EC_PWD_CLR_REQUEST,      {0,0} },
//      { EFI_SOFTWARE_PEI_CORE | EFI_SW_EC_PWD_CLEARED,          {0,0} },
//      { EFI_SOFTWARE_PEI_CORE | EFI_SW_EC_EVENT_LOG_FULL,          {0,0} },
//      { EFI_SOFTWARE_PEI_CORE | EFI_SW_PEI_CORE_EC_DXE_CORRUPT,      {0,0} },
    
//      { EFI_SOFTWARE_PEI_MODULE | EFI_SW_EC_NON_SPECIFIC,         {0,0} },
//      { EFI_SOFTWARE_PEI_MODULE | EFI_SW_EC_LOAD_ERROR,          {0,0} },
//      { EFI_SOFTWARE_PEI_MODULE | EFI_SW_EC_INVALID_PARAMETER,      {0,0} },
//      { EFI_SOFTWARE_PEI_MODULE | EFI_SW_EC_UNSUPPORTED,           {0,0} },
//      { EFI_SOFTWARE_PEI_MODULE | EFI_SW_EC_INVALID_BUFFER,      {0,0} },
//      { EFI_SOFTWARE_PEI_MODULE | EFI_SW_EC_OUT_OF_RESOURCES,      {0,0} },
//      { EFI_SOFTWARE_PEI_MODULE | EFI_SW_EC_ABORTED,         {0,0} },
//      { EFI_SOFTWARE_PEI_MODULE | EFI_SW_EC_ILLEGAL_SOFTWARE_STATE, {0,0} },
//      { EFI_SOFTWARE_PEI_MODULE | EFI_SW_EC_ILLEGAL_HARDWARE_STATE, {0,0} },
//      { EFI_SOFTWARE_PEI_MODULE | EFI_SW_EC_START_ERROR,          {0,0} },
      { EFI_SOFTWARE_PEI_MODULE | EFI_SW_EC_BAD_DATE_TIME,       {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CMOS_TIME_NOT_SET_BIT} },
//      { EFI_SOFTWARE_PEI_MODULE | EFI_SW_EC_CFG_INVALID,          {0,0} },
//      { EFI_SOFTWARE_PEI_MODULE | EFI_SW_EC_CFG_CLR_REQUEST,     {0,0} },
//      { EFI_SOFTWARE_PEI_MODULE | EFI_SW_EC_CFG_DEFAULT,         {0,0} },
//      { EFI_SOFTWARE_PEI_MODULE | EFI_SW_EC_PWD_INVALID,          {0,0} },
//      { EFI_SOFTWARE_PEI_MODULE | EFI_SW_EC_PWD_CLR_REQUEST,      {0,0} },
//      { EFI_SOFTWARE_PEI_MODULE | EFI_SW_EC_PWD_CLEARED,          {0,0} },
//      { EFI_SOFTWARE_PEI_MODULE | EFI_SW_EC_EVENT_LOG_FULL,      {0,0} },
//      { EFI_SOFTWARE_PEI_MODULE | EFI_SW_PEI_EC_NO_RECOVERY_CAPSULE,{0,0} },
    
//      { EFI_SOFTWARE_DXE_CORE | EFI_SW_EC_NON_SPECIFIC,          {0,0} },
//      { EFI_SOFTWARE_DXE_CORE | EFI_SW_EC_LOAD_ERROR,          {0,0} },
//      { EFI_SOFTWARE_DXE_CORE | EFI_SW_EC_INVALID_PARAMETER,      {0,0} },
//      { EFI_SOFTWARE_DXE_CORE | EFI_SW_EC_UNSUPPORTED,           {0,0} },
//      { EFI_SOFTWARE_DXE_CORE | EFI_SW_EC_INVALID_BUFFER,          {0,0} },
//      { EFI_SOFTWARE_DXE_CORE | EFI_SW_EC_OUT_OF_RESOURCES,      {0,0} },
//      { EFI_SOFTWARE_DXE_CORE | EFI_SW_EC_ABORTED,          {0,0} },
//      { EFI_SOFTWARE_DXE_CORE | EFI_SW_EC_ILLEGAL_SOFTWARE_STATE,      {0,0} },
//      { EFI_SOFTWARE_DXE_CORE | EFI_SW_EC_ILLEGAL_HARDWARE_STATE,      {0,0} },
//      { EFI_SOFTWARE_DXE_CORE | EFI_SW_EC_START_ERROR,          {0,0} },
      { EFI_SOFTWARE_DXE_CORE | EFI_SW_EC_BAD_DATE_TIME,           {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CMOS_TIME_NOT_SET_BIT} },
//      { EFI_SOFTWARE_DXE_CORE | EFI_SW_EC_CFG_INVALID,          {0,0} },
//      { EFI_SOFTWARE_DXE_CORE | EFI_SW_EC_CFG_CLR_REQUEST,      {0,0} },
//      { EFI_SOFTWARE_DXE_CORE | EFI_SW_EC_CFG_DEFAULT,          {0,0} },
//      { EFI_SOFTWARE_DXE_CORE | EFI_SW_EC_PWD_INVALID,          {0,0} },
//      { EFI_SOFTWARE_DXE_CORE | EFI_SW_EC_PWD_CLR_REQUEST,      {0,0} },
//      { EFI_SOFTWARE_DXE_CORE | EFI_SW_EC_PWD_CLEARED,          {0,0} },
//      { EFI_SOFTWARE_DXE_CORE | EFI_SW_EC_EVENT_LOG_FULL,          {0,0} },  
      
//      { EFI_SOFTWARE_EFI_BOOT_SERVICE  | EFI_SW_EC_NON_SPECIFIC,          {0,0} },
//      { EFI_SOFTWARE_EFI_BOOT_SERVICE  | EFI_SW_EC_LOAD_ERROR,          {0,0} },
//      { EFI_SOFTWARE_EFI_BOOT_SERVICE  | EFI_SW_EC_INVALID_PARAMETER,      {0,0} },
//      { EFI_SOFTWARE_EFI_BOOT_SERVICE  | EFI_SW_EC_UNSUPPORTED,          {0,0} },
//      { EFI_SOFTWARE_EFI_BOOT_SERVICE  | EFI_SW_EC_INVALID_BUFFER,      {0,0} },
//      { EFI_SOFTWARE_EFI_BOOT_SERVICE  | EFI_SW_EC_OUT_OF_RESOURCES,      {0,0} },
//      { EFI_SOFTWARE_EFI_BOOT_SERVICE  | EFI_SW_EC_ABORTED,          {0,0} },
//      { EFI_SOFTWARE_EFI_BOOT_SERVICE  | EFI_SW_EC_ILLEGAL_SOFTWARE_STATE,  {0,0} },
//      { EFI_SOFTWARE_EFI_BOOT_SERVICE  | EFI_SW_EC_ILLEGAL_HARDWARE_STATE,  {0,0} },
//      { EFI_SOFTWARE_EFI_BOOT_SERVICE  | EFI_SW_EC_START_ERROR,          {0,0} },
      { EFI_SOFTWARE_EFI_BOOT_SERVICE  | EFI_SW_EC_BAD_DATE_TIME,       {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CMOS_TIME_NOT_SET_BIT} },
//      { EFI_SOFTWARE_EFI_BOOT_SERVICE  | EFI_SW_EC_CFG_INVALID,          {0,0} },
//      { EFI_SOFTWARE_EFI_BOOT_SERVICE  | EFI_SW_EC_CFG_CLR_REQUEST,      {0,0} },
//      { EFI_SOFTWARE_EFI_BOOT_SERVICE  | EFI_SW_EC_CFG_DEFAULT,          {0,0} },
//      { EFI_SOFTWARE_EFI_BOOT_SERVICE  | EFI_SW_EC_PWD_INVALID,         {0,0} },
//      { EFI_SOFTWARE_EFI_BOOT_SERVICE  | EFI_SW_EC_PWD_CLR_REQUEST,      {0,0} },
//      { EFI_SOFTWARE_EFI_BOOT_SERVICE  | EFI_SW_EC_PWD_CLEARED,          {0,0} },
//      { EFI_SOFTWARE_EFI_BOOT_SERVICE  | EFI_SW_EC_EVENT_LOG_FULL,      {0,0} },  
      
//      { EFI_SOFTWARE_EFI_RUNTIME_SERVICE | EFI_SW_EC_NON_SPECIFIC,      {0,0} },
//      { EFI_SOFTWARE_EFI_RUNTIME_SERVICE | EFI_SW_EC_LOAD_ERROR,          {0,0} },
//      { EFI_SOFTWARE_EFI_RUNTIME_SERVICE | EFI_SW_EC_INVALID_PARAMETER,      {0,0} },
//      { EFI_SOFTWARE_EFI_RUNTIME_SERVICE | EFI_SW_EC_UNSUPPORTED,          {0,0} },
//      { EFI_SOFTWARE_EFI_RUNTIME_SERVICE | EFI_SW_EC_INVALID_BUFFER,      {0,0} },
//      { EFI_SOFTWARE_EFI_RUNTIME_SERVICE | EFI_SW_EC_OUT_OF_RESOURCES,      {0,0} },
//      { EFI_SOFTWARE_EFI_RUNTIME_SERVICE | EFI_SW_EC_ABORTED,         {0,0} },
//      { EFI_SOFTWARE_EFI_RUNTIME_SERVICE | EFI_SW_EC_ILLEGAL_SOFTWARE_STATE,{0,0} },
//      { EFI_SOFTWARE_EFI_RUNTIME_SERVICE | EFI_SW_EC_ILLEGAL_HARDWARE_STATE,{0,0} },
//      { EFI_SOFTWARE_EFI_RUNTIME_SERVICE | EFI_SW_EC_START_ERROR,          {0,0} },
      { EFI_SOFTWARE_EFI_RUNTIME_SERVICE | EFI_SW_EC_BAD_DATE_TIME,       {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CMOS_TIME_NOT_SET_BIT} },
//      { EFI_SOFTWARE_EFI_RUNTIME_SERVICE | EFI_SW_EC_CFG_INVALID,          {0,0} },
//      { EFI_SOFTWARE_EFI_RUNTIME_SERVICE | EFI_SW_EC_CFG_CLR_REQUEST,     {0,0} },
//      { EFI_SOFTWARE_EFI_RUNTIME_SERVICE | EFI_SW_EC_CFG_DEFAULT,          {0,0} },
//      { EFI_SOFTWARE_EFI_RUNTIME_SERVICE | EFI_SW_EC_PWD_INVALID,          {0,0} },
//      { EFI_SOFTWARE_EFI_RUNTIME_SERVICE | EFI_SW_EC_PWD_CLR_REQUEST,     {0,0} },
//      { EFI_SOFTWARE_EFI_RUNTIME_SERVICE | EFI_SW_EC_PWD_CLEARED,          {0,0} },
//      { EFI_SOFTWARE_EFI_RUNTIME_SERVICE | EFI_SW_EC_EVENT_LOG_FULL,      {0,0} },
      
//      { EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_EC_NON_SPECIFIC,         {0,0} },
//      { EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_EC_LOAD_ERROR,             {0,0} },
//      { EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_EC_INVALID_PARAMETER,          {0,0} },
//      { EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_EC_UNSUPPORTED,           {0,0} },
//      { EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_EC_INVALID_BUFFER,          {0,0} },
//      { EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_EC_OUT_OF_RESOURCES,          {0,0} },
//      { EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_EC_ABORTED,              {0,0} },
//      { EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_EC_ILLEGAL_SOFTWARE_STATE,      {0,0} },
//      { EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_EC_ILLEGAL_HARDWARE_STATE,      {0,0} },
//      { EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_EC_START_ERROR,          {0,0} },
      { EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_EC_BAD_DATE_TIME,            {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CMOS_TIME_NOT_SET_BIT} },
//      { EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_EC_CFG_INVALID,          {0,0} },
//      { EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_EC_CFG_CLR_REQUEST,          {0,0} },
//      { EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_EC_CFG_DEFAULT,          {0,0} },
//      { EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_EC_PWD_INVALID,          {0,0} },
//      { EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_EC_PWD_CLR_REQUEST,         {0,0} },
//      { EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_EC_PWD_CLEARED,          {0,0} },
//      { EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_EC_EVENT_LOG_FULL,         {0,0} },
//      { EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_DXE_BS_EC_LEGACY_OPROM_NO_SPACE,{0,0} },
    
//      { EFI_SOFTWARE_SMM_DRIVER | EFI_SW_EC_NON_SPECIFIC,          {0,0} },
//      { EFI_SOFTWARE_SMM_DRIVER | EFI_SW_EC_LOAD_ERROR,          {0,0} },
//      { EFI_SOFTWARE_SMM_DRIVER | EFI_SW_EC_INVALID_PARAMETER,     {0,0} },
//      { EFI_SOFTWARE_SMM_DRIVER | EFI_SW_EC_UNSUPPORTED,           {0,0} },
//      { EFI_SOFTWARE_SMM_DRIVER | EFI_SW_EC_INVALID_BUFFER,      {0,0} },
//      { EFI_SOFTWARE_SMM_DRIVER | EFI_SW_EC_OUT_OF_RESOURCES,      {0,0} },
//      { EFI_SOFTWARE_SMM_DRIVER | EFI_SW_EC_ABORTED,          {0,0} },
//      { EFI_SOFTWARE_SMM_DRIVER | EFI_SW_EC_ILLEGAL_SOFTWARE_STATE,    {0,0} },
//      { EFI_SOFTWARE_SMM_DRIVER | EFI_SW_EC_ILLEGAL_HARDWARE_STATE,    {0,0} },
//      { EFI_SOFTWARE_SMM_DRIVER | EFI_SW_EC_START_ERROR,          {0,0} },
      { EFI_SOFTWARE_SMM_DRIVER | EFI_SW_EC_BAD_DATE_TIME,        {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CMOS_TIME_NOT_SET_BIT} },
//      { EFI_SOFTWARE_SMM_DRIVER | EFI_SW_EC_CFG_INVALID,          {0,0} },  
//      { EFI_SOFTWARE_SMM_DRIVER | EFI_SW_EC_CFG_CLR_REQUEST,      {0,0} },  
//      { EFI_SOFTWARE_SMM_DRIVER | EFI_SW_EC_CFG_DEFAULT,         {0,0} },  
//      { EFI_SOFTWARE_SMM_DRIVER | EFI_SW_EC_PWD_INVALID,          {0,0} },  
//      { EFI_SOFTWARE_SMM_DRIVER | EFI_SW_EC_PWD_CLR_REQUEST,      {0,0} },  
//      { EFI_SOFTWARE_SMM_DRIVER | EFI_SW_EC_PWD_CLEARED,          {0,0} },  
//      { EFI_SOFTWARE_SMM_DRIVER | EFI_SW_EC_EVENT_LOG_FULL,      {0,0} },  
    
//      { EFI_SOFTWARE_RT  | EFI_SW_EC_NON_SPECIFIC,             {0,0} },
//      { EFI_SOFTWARE_RT  | EFI_SW_EC_LOAD_ERROR,              {0,0} },
//      { EFI_SOFTWARE_RT  | EFI_SW_EC_INVALID_PARAMETER,          {0,0} },
//      { EFI_SOFTWARE_RT  | EFI_SW_EC_UNSUPPORTED,              {0,0} },
//      { EFI_SOFTWARE_RT  | EFI_SW_EC_INVALID_BUFFER,         {0,0} },
//      { EFI_SOFTWARE_RT  | EFI_SW_EC_OUT_OF_RESOURCES,         {0,0} },
//      { EFI_SOFTWARE_RT  | EFI_SW_EC_ABORTED,              {0,0} },
//      { EFI_SOFTWARE_RT  | EFI_SW_EC_ILLEGAL_SOFTWARE_STATE,    {0,0} },
//      { EFI_SOFTWARE_RT  | EFI_SW_EC_ILLEGAL_HARDWARE_STATE,      {0,0} },
//      { EFI_SOFTWARE_RT  | EFI_SW_EC_START_ERROR,             {0,0} },
      { EFI_SOFTWARE_RT  | EFI_SW_EC_BAD_DATE_TIME,           {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CMOS_TIME_NOT_SET_BIT} },
//      { EFI_SOFTWARE_RT  | EFI_SW_EC_CFG_INVALID,             {0,0} },  
//      { EFI_SOFTWARE_RT  | EFI_SW_EC_CFG_CLR_REQUEST,          {0,0} },  
//      { EFI_SOFTWARE_RT  | EFI_SW_EC_CFG_DEFAULT,             {0,0} },  
//      { EFI_SOFTWARE_RT  | EFI_SW_EC_PWD_INVALID,              {0,0} },  
//      { EFI_SOFTWARE_RT  | EFI_SW_EC_PWD_CLR_REQUEST,          {0,0} },  
//      { EFI_SOFTWARE_RT  | EFI_SW_EC_PWD_CLEARED,              {0,0} },  
//      { EFI_SOFTWARE_RT  | EFI_SW_EC_EVENT_LOG_FULL,          {0,0} },  
    
//      { EFI_SOFTWARE_AL  | EFI_SW_EC_NON_SPECIFIC,            {0,0} },
//      { EFI_SOFTWARE_AL  | EFI_SW_EC_LOAD_ERROR,              {0,0} },
//      { EFI_SOFTWARE_AL  | EFI_SW_EC_INVALID_PARAMETER,          {0,0} },
//      { EFI_SOFTWARE_AL  | EFI_SW_EC_UNSUPPORTED,           {0,0} },
//      { EFI_SOFTWARE_AL  | EFI_SW_EC_INVALID_BUFFER,         {0,0} },
//      { EFI_SOFTWARE_AL  | EFI_SW_EC_OUT_OF_RESOURCES,          {0,0} },
//      { EFI_SOFTWARE_AL  | EFI_SW_EC_ABORTED,              {0,0} },
//      { EFI_SOFTWARE_AL  | EFI_SW_EC_ILLEGAL_SOFTWARE_STATE,      {0,0} },
//      { EFI_SOFTWARE_AL  | EFI_SW_EC_ILLEGAL_HARDWARE_STATE,     {0,0} },
//      { EFI_SOFTWARE_AL  | EFI_SW_EC_START_ERROR,             {0,0} },
      { EFI_SOFTWARE_AL  | EFI_SW_EC_BAD_DATE_TIME,            {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CMOS_TIME_NOT_SET_BIT} },
//      { EFI_SOFTWARE_AL  | EFI_SW_EC_CFG_INVALID,              {0,0} },  
//      { EFI_SOFTWARE_AL  | EFI_SW_EC_CFG_CLR_REQUEST,          {0,0} },  
//      { EFI_SOFTWARE_AL  | EFI_SW_EC_CFG_DEFAULT,             {0,0} },  
//      { EFI_SOFTWARE_AL  | EFI_SW_EC_PWD_INVALID,             {0,0} },  
//      { EFI_SOFTWARE_AL  | EFI_SW_EC_PWD_CLR_REQUEST,          {0,0} },  
//      { EFI_SOFTWARE_AL  | EFI_SW_EC_PWD_CLEARED,              {0,0} },  
//      { EFI_SOFTWARE_AL  | EFI_SW_EC_EVENT_LOG_FULL,          {0,0} },  
    
//      { EFI_SOFTWARE_EFI_APPLICATION | EFI_SW_EC_NON_SPECIFIC,          {0,0} },
//      { EFI_SOFTWARE_EFI_APPLICATION | EFI_SW_EC_LOAD_ERROR,          {0,0} },
//      { EFI_SOFTWARE_EFI_APPLICATION | EFI_SW_EC_INVALID_PARAMETER,      {0,0} },
//      { EFI_SOFTWARE_EFI_APPLICATION | EFI_SW_EC_UNSUPPORTED,           {0,0} },
//      { EFI_SOFTWARE_EFI_APPLICATION | EFI_SW_EC_INVALID_BUFFER,          {0,0} },
//      { EFI_SOFTWARE_EFI_APPLICATION | EFI_SW_EC_OUT_OF_RESOURCES,         {0,0} },
//      { EFI_SOFTWARE_EFI_APPLICATION | EFI_SW_EC_ABORTED,              {0,0} },
//      { EFI_SOFTWARE_EFI_APPLICATION | EFI_SW_EC_ILLEGAL_SOFTWARE_STATE,      {0,0} },
//      { EFI_SOFTWARE_EFI_APPLICATION | EFI_SW_EC_ILLEGAL_HARDWARE_STATE,      {0,0} },
//      { EFI_SOFTWARE_EFI_APPLICATION | EFI_SW_EC_START_ERROR,          {0,0} },
      { EFI_SOFTWARE_EFI_APPLICATION | EFI_SW_EC_BAD_DATE_TIME,            {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CMOS_TIME_NOT_SET_BIT} },
//      { EFI_SOFTWARE_EFI_APPLICATION | EFI_SW_EC_CFG_INVALID,          {0,0} },  
//      { EFI_SOFTWARE_EFI_APPLICATION | EFI_SW_EC_CFG_CLR_REQUEST,          {0,0} },  
//      { EFI_SOFTWARE_EFI_APPLICATION | EFI_SW_EC_CFG_DEFAULT,          {0,0} },  
//      { EFI_SOFTWARE_EFI_APPLICATION | EFI_SW_EC_PWD_INVALID,          {0,0} },  
//      { EFI_SOFTWARE_EFI_APPLICATION | EFI_SW_EC_PWD_CLR_REQUEST,          {0,0} },  
//      { EFI_SOFTWARE_EFI_APPLICATION | EFI_SW_EC_PWD_CLEARED,          {0,0} },  
//      { EFI_SOFTWARE_EFI_APPLICATION | EFI_SW_EC_EVENT_LOG_FULL,          {0,0} },  
    
//      { EFI_SOFTWARE_EFI_OS_LOADER | EFI_SW_EC_NON_SPECIFIC,          {0,0} },
//      { EFI_SOFTWARE_EFI_OS_LOADER | EFI_SW_EC_LOAD_ERROR,          {0,0} },
//      { EFI_SOFTWARE_EFI_OS_LOADER | EFI_SW_EC_INVALID_PARAMETER,         {0,0} },
//      { EFI_SOFTWARE_EFI_OS_LOADER | EFI_SW_EC_UNSUPPORTED,           {0,0} },
//      { EFI_SOFTWARE_EFI_OS_LOADER | EFI_SW_EC_INVALID_BUFFER,          {0,0} },
//      { EFI_SOFTWARE_EFI_OS_LOADER | EFI_SW_EC_OUT_OF_RESOURCES,          {0,0} },
//      { EFI_SOFTWARE_EFI_OS_LOADER | EFI_SW_EC_ABORTED,              {0,0} },
//      { EFI_SOFTWARE_EFI_OS_LOADER | EFI_SW_EC_ILLEGAL_SOFTWARE_STATE,      {0,0} },
//      { EFI_SOFTWARE_EFI_OS_LOADER | EFI_SW_EC_ILLEGAL_HARDWARE_STATE,      {0,0} },
//      { EFI_SOFTWARE_EFI_OS_LOADER | EFI_SW_EC_START_ERROR,          {0,0} },
      { EFI_SOFTWARE_EFI_OS_LOADER | EFI_SW_EC_BAD_DATE_TIME,            {EFI_EVENT_LOG_TYPE_POST_ERROR, EFI_POST_BITMAP_DW1_CMOS_TIME_NOT_SET_BIT} },
//      { EFI_SOFTWARE_EFI_OS_LOADER | EFI_SW_EC_CFG_INVALID,          {0,0} },  
//      { EFI_SOFTWARE_EFI_OS_LOADER | EFI_SW_EC_CFG_CLR_REQUEST,          {0,0} },  
//      { EFI_SOFTWARE_EFI_OS_LOADER | EFI_SW_EC_CFG_DEFAULT,          {0,0} },  
//      { EFI_SOFTWARE_EFI_OS_LOADER | EFI_SW_EC_PWD_INVALID,          {0,0} },  
//      { EFI_SOFTWARE_EFI_OS_LOADER | EFI_SW_EC_PWD_CLR_REQUEST,          {0,0} },  
//      { EFI_SOFTWARE_EFI_OS_LOADER | EFI_SW_EC_PWD_CLEARED,          {0,0} },  
//      { EFI_SOFTWARE_EFI_OS_LOADER | EFI_SW_EC_EVENT_LOG_FULL,          {0,0} }
};

#if PLATFORM_RTC_READ
//
// Hook list
//
typedef EFI_STATUS (GPNV_RTC_READ_HOOK) (
  EFI_TIME           *Time
);
#endif

#endif
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
