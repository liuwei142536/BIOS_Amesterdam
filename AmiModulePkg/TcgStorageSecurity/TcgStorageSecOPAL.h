//***********************************************************************
//***********************************************************************
//**                                                                   **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.          **
//**                                                                   **
//**                       All Rights Reserved.                        **
//**                                                                   **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093         **
//**                                                                   **
//**                       Phone: (770)-246-8600                       **
//**                                                                   **
//***********************************************************************
//***********************************************************************

/** @file TcgStorageSecOPAL.h
    This file contains the Includes, Definitions, typedefs,
    Variable and External Declarations, Structure and
    function prototypes needed for TcgStorageSecurity driver

**/

#ifndef _TCG_STORAGE_SEC_OPAL_H_
#define _TCG_STORAGE_SEC_OPAL_H_

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------
#include <Token.h>
#include <Library/PrintLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include "AmiTcgStorageSecurity.h"
//---------------------------------------------------------------------------

#ifndef NVME_COMMAND_TIMEOUT
#define NVME_COMMAND_TIMEOUT                    1   // 1 Sec
#endif
#define SATA_COMMAND_TIMEOUT                    16   // 16 Sec For SATA device use the std. timeout we use in AHCI driver
#define NUM_OF_RETRIES                          10   // in NVME_COMMAND_TIMEOUT Or SATA_COMMAND_TIMEOUT units,

// OPAL Commands
#define     BLOCK_SID_COMID                 0x0005
#define     COMID_LEVEL0_DISCOVERY          0x01
#define     TPER_RESET                      0x04

// Security Mask Info
// Security Locked               : BIT02
// Security Locking Enabled      : BIT01
// Security Locking Supported    : BIT00

#define OPAL_SECURITY_LOCKING_FEATURE_MASK      0x07

#define TPer_Feature                                    0x0001
#define Locking_Feature                                 0x0002
#define Geometry_Reporting_Feature                      0x0003 // Opal SSC V 2.01 Spec
#define OPAL_SSC_V200_Feature                           0x0203 // Opal SSC V 2.01 Spec
#define OPALITE_SSC_Feature                             0x0301 //
#define PYRITE_SSC_Feature                              0x0302 // Pyrite SSC V 1.00 Spec
#define PYRITE_SSC_Feature_v2_0                         0x0303 // Pyrite SSC V 2.00 Spec
#define RUBY_SSC_Feature                                0x0304 // RUBY SSC V 1.00 Spec
#define ENTERPRISE_SSC_Feature                          0x0100
#define Block_SID_Authentification_Feature              0x0402

#define UNSIGNED_DATA                                   0
#define SIGNED_DATA                                     1
#define BYTE_DATA                                       1
#define INTEGER_DATA                                    0

// Token Types
#define START_LIST                                      0xF0
#define END_LIST                                        0xF1
#define START_NAME                                      0xF2
#define END_NAME                                        0xF3
#define CALL_TOKEN                                      0xF8
#define EOD_TOKEN                                       0xF9
#define EOS_TOKEN                                       0xFA
#define START_TRANSMISSION                              0xFB
#define END_TRANSMISSION                                0xFC
#define EMPTY_ATOM                                      0xFF

#define KEEP_GLOBAL_RANGE_KEY_PARAMETER                 0x60000

// Method Status Codes
#define SUCCESS                                         0x00
#define NOT_AUTHORIZED                                  0x01
//#define   OBSOLETE                                        0x02
#define SP_BUSY                                         0x03
#define SP_FAILED                                       0x04
#define SP_DISABLED                                     0x05
#define SP_FROZEN                                       0x06
#define NO_SESSIONS_AVAILABLE                           0x07
#define UNIQUENESS_CONFLICT                             0x08
#define INSUFFICIENT_SPACE                              0x09
#define INSUFFICIENT_ROWS                               0x0A
#define INVALID_PARAMETER                               0x0C
//#define   OBSOLETE                                        0x0D
//#define   OBSOLETE                                        0x0E
#define TPER_MALFUNCTION                                0x0F
#define TRANSACTION_FAILURE                             0x10
#define RESPONSE_OVERFLOW                               0x11
#define AUTHORITY_LOCKED_OUT                            0x12
#define FAIL                                            0x3F

#define TCGUID(a,b,c,d,e,f,g,h)     (UINT64)a + ((UINT64)b << 8) + ((UINT64)c << 16) + ((UINT64)d << 24) + ((UINT64)e << 32) + ((UINT64)f << 40) + ((UINT64)g << 48) + ((UINT64)h << 56)

// UIDs Table 239 Special Purpose UIDs
#define NULL_UID                    TCGUID(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)
#define THISSP_UID                  TCGUID(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01)
#define SM_UID                      TCGUID(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF)
#define CPIN_UID                    TCGUID(0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x01)

// Session Manager Method UIDs Table 241
#define PROPERTIES_UID              TCGUID(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x01)
#define START_SESSION_UID           TCGUID(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x02)
#define SYNC_SESSION_UID            TCGUID(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03)
#define START_TRUSTED_SESSION_UID   TCGUID(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x04)
#define SYNC_TRUSTED_SESSION_UID    TCGUID(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x05)
#define CLOSE_SESSION_UID           TCGUID(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x06)

//Single Row Table Row IDs Table 244
#define SPINFO_UID                  TCGUID(0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0xFF, 0x01)
#define TPERINFO_UID                TCGUID(0x00, 0x00, 0x02, 0x01, 0x00, 0x03, 0x00, 0x01)
#define LOCKINGINFO_UID             TCGUID(0x00, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x01)
#define MBRCONTROL_UID              TCGUID(0x00, 0x00, 0x08, 0x03, 0x00, 0x00, 0x00, 0x01)

//Table Default Rows Table 245
#define C_PIN_SID_UID                TCGUID(0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x01)
#define SP_ADMIN_UID                 TCGUID(0x00, 0x00, 0x02, 0x05, 0x00, 0x00, 0x00, 0x01)
#define CLOCK_TIME_CLOCK_UID         TCGUID(0x00, 0x00, 0x04, 0x01, 0x00, 0x00, 0x00, 0x01)
#define LOGLIST_LOG_UID              TCGUID(0x00, 0x00, 0x0A, 0x02, 0x00, 0x00, 0x00, 0x01)
#define LOCKING_GLOBAL_RANGE_UID     TCGUID(0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x00, 0x01)

//Method UIDs from CORE spec Table 242
#define GEN_KEY_UID                 TCGUID(0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x10)
#define GET_KEY_UID                 TCGUID(0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x16)
#define GET_KEY_UID_ENTERPRISE      TCGUID(0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06)
#define SET_KEY_UID                 TCGUID(0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x17)
#define SET_KEY_UID_ENTERPRISE      TCGUID(0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07)
#define AUTH_UID                    TCGUID(0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x1C)
#define AUTH_UID_ENTERPRISE         TCGUID(0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0C)
#define REVERT_UID                  TCGUID(0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x02, 0x02)
#define REVERTSP_UID                TCGUID(0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x11)
#define ACTIVATE_UID                TCGUID(0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x02, 0x03) // Opal SSC Spec


// Authority UIDs Table 243 Core Spec
#define ANYBODY_AUTH_UID            TCGUID(0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x01)
#define ADMINS_AUTH_UID             TCGUID(0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x02)
#define SID_AUTH_UID                TCGUID(0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x06)

#define PSID_AUTH_UID               TCGUID(0x00, 0x00, 0x00, 0x09, 0x00, 0x01, 0xFF, 0x01) // Defined in Opal SSC PSID doc

// From SSC Opal Spec
#define C_PIN_MSID_UID              TCGUID(0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x84, 0x02)
#define C_PIN_ADMIN1_UID            TCGUID(0x00, 0x00, 0x00, 0x0B, 0x00, 0x01, 0x00, 0x01)

//Locking SP
#define LOCKING_SP_UID              TCGUID(0x00, 0x00, 0x02, 0x05, 0x00, 0x00, 0x00, 0x02)
#define LOCKING_SP_UID_ENYTERPRISE  TCGUID(0x00, 0x00, 0x02, 0x05, 0x00, 0x01, 0x00, 0x01)
#define LOCKING_SP_ADMIN1_UID       TCGUID(0x00, 0x00, 0x00, 0x09, 0x00, 0x01, 0x00, 0x01)

#define LOCKING_GLOBAL_RANGE_UID    TCGUID(0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x00, 0x01)
#define LOCKING_RANGE1_UID          TCGUID(0x00, 0x00, 0x08, 0x02, 0x00, 0x03, 0x00, 0x01)

#define K_AES_256_RANGE1_KEY_UID    TCGUID(0x00, 0x00, 0x08, 0x06, 0x00, 0x03, 0x00, 0x01)
#define K_AES_128_RANGE1_KEY_UID    TCGUID(0x00, 0x00, 0x08, 0x05, 0x00, 0x03, 0x00, 0x01)

#define MBR_UID                     TCGUID(0x00, 0x00, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00)

// "MBR" Table UID in Locking SP. See Table 27 in SSC Opal spec
#define MBR_ROW_UID                 TCGUID(0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x08, 0x04)

// Authority
#define Authority                   TCGUID(0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00)
#define UserMMMM                    TCGUID(0x00, 0x00, 0x00, 0x09, 0x00, 0x03, 0x00, 0x00)

// C_PIN
#define C_PIN_USERMMMM              TCGUID(0x00, 0x00, 0x00, 0x0B, 0x00, 0x03, 0x00, 0x00)

#define ACE_Locking_GlobalRange_Set_RdLocked TCGUID(0x00, 0x00, 0x00, 0x08, 0x00, 0x03, 0xE0, 0x00)
#define ACE_Locking_GlobalRange_Set_WrLocked TCGUID(0x00, 0x00, 0x00, 0x08, 0x00, 0x03, 0xE8, 0x00)
#define ACE_Authority_Set_Enabled            TCGUID(0x00, 0x00, 0x00, 0x08, 0x00, 0x03, 0x90, 0x01)
#define ACE_Authority_Get_All                TCGUID(0x00, 0x00, 0x00, 0x08, 0x00, 0x03, 0x90, 0x00)

//Authority Object
#define BANDMASTER0_UID             TCGUID(0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x80, 0x01)
#define BANDMASTER1_UID             TCGUID(0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x80, 0x02)
#define ERASEMASTER_UID             TCGUID(0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x84, 0x01)

#define BANDMASTER0_PIN_UID         TCGUID(0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x80, 0x01)
#define BANDMASTER1_PIN_UID         TCGUID(0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x80, 0x02)
#define ERASEMASTER_PIN_UID         TCGUID(0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x84, 0x01)

#define LOCKING_BAND1_UID_ENTERPRISE TCGUID(0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x00, 0x02)

// Table 40 from SSC Opal spec. LifeCycle state
#define LIFE_CYCLE_STATE_ISSUED                     0
#define LIFE_CYCLE_STATE_ISSUED_DISABLED            1
#define LIFE_CYCLE_STATE_ISSUED_FROZEN              2
#define LIFE_CYCLE_STATE_ISSUED_DISABLED_FROZEN     3
#define LIFE_CYCLE_STATE_ISSUED_FAILED              4
#define LIFE_CYCLE_STATE_ISSUED_MANUF_INACTIVE      8
#define LIFE_CYCLE_STATE_ISSUED_MANUF               9
#define LIFE_CYCLE_STATE_ISSUED_MANUF_FROZEN_DIS   10
#define LIFE_CYCLE_STATE_ISSUED_MANUF_FROZEN        11
#define LIFE_CYCLE_STATE_ISSUED_MANUF_DIS_FROZEN    12
#define LIFE_CYCLE_STATE_ISSUED_MANUF_FAILED        13

// SyncSession Data. Look at the sample data for SyncSession response. Offsets are calculated from it
// This is the first data in the DataPayload. Subsequent offsets can't be hard coded.
// This is relative to Data Payload
#define SYNC_SESSION_SMUID_OFFSET   2
#define SYNC_SESSION_SYNC_OFFSET    11
#define SYNC_SESSION_STARTLIST      19

#define TCG_STORAGE_ERR_CHECK(Status)        if (EFI_ERROR(Status)) { return Status;}
#define TCG_STORAGE_DEVICE_ERR_CHECK(Status)        if (Status == EFI_DEVICE_ERROR) { return Status;}
#define TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status) if (EFI_ERROR(Status)) { goto CloseSession;}
#define TCG_STORAGE_ERR_CHECK_VOID(Status)        if (EFI_ERROR(Status)) { return;}
#define IS_ENTERPRISE_DEVICE(OpalDevice) ((OpalDevice != NULL) &&((OpalDevice)->OpalFeature.FeatureCode == ENTERPRISE_SSC_Feature))

// DO NOT REMOVE PRAGMA for structure definition
#pragma pack(1)

typedef
struct _COM_PACKET_FORMAT {

    UINT32      Reserved;
    UINT16      ComID;
    UINT16      ComIDExtension;
    UINT32      OutStandingData;
    UINT32      MinTransfer;
    UINT32      Length;
    UINT8       PayLoad[1];

}COM_PACKET_FORMAT;

typedef
struct _PACKET_FORMAT {

    UINT32      TSN;
    UINT32      HSN;
    UINT32      SeqNumber;
    UINT16      Reserved;
    UINT16      AckType;
    UINT32      Acknowledgement;
    UINT32      Length;
    UINT8       PayLoad[1];

}PACKET_FORMAT;

typedef
struct _DATA_SUBPACKET_FORMAT {

    UINT8       Reserved[6];
    UINT16      Kind;
    UINT32      Length;
    UINT8       PayLoad[1];

}DATA_SUBPACKET_FORMAT;


typedef
struct _LEVEL0_DISCOVERY_DATA {

    LEVEL0_DISCOVERY_HEADER     Level0DiscoveryHeader;
    FEATURE_DESCRIPTOR          FeatureDescriptor;

}LEVEL0_DISCOVERY_DATA;

typedef
struct  _SESSION_PARAMS {
    UINT64      InvokingID;
    UINT64      MethodID;
    UINT32      HostSessionID;      // Used when Session is opened.
    UINT32      Tsn;                // Once Session is opened, this value gets updated and used in Packet. Gets updated in SyncSession
    UINT32      Hsn;                // Once Session is opened, this value gets updated and used in Packet. Gets updated in SyncSession
    UINT64      SPID;               // UID of SP
    BOOLEAN     Write;
    UINTN       HostChallengeLength;
    UINT8       *HostChallenge;
    UINT64      HostExchangeAuth;
    UINTN       HostExchangeCertLength;
    UINT8       *HostExchangeCert;
    UINT64      HostSigningAuthority;
    UINTN       HostSigningCertLength;
    UINT8       *HostSigningCert;
    UINT64      *SessionTimeOut;
    UINT64      *TransTimeout;
    UINT64      *InitialCredit;
    UINTN       SignedHashLength;
    UINT8       *SignedHash;
}SESSION_PARAMS;

//Format Sec 5.1.4.2.3 Core Spec
//Table = uidref, startRow = typeOr {UID : Uidref, Row : Uinteger }, endrow = Uinteger,
// StartColumn = Uinteger, endColumn = uinteger ]

typedef
struct _CELL_BLOCK {

    UINT64      TableUID;
    BOOLEAN     RowisUid;
    UINT64      StartRow;
    UINT8       EndRow;
    UINT8       StartColumn;
    UINT8       EndColumn;
    UINT8       StartRowString[33];
    UINT8       EndRowString[33];
    UINT8       StartColumnString[33];
    UINT8       EndColumnString[33];
}CELL_BLOCK;

#pragma pack()

// Sec 5.1.2 Core spec. Type Encodings
typedef enum {
    SIMPLE_TYPES,
    ENUMERATION_TYPES,
    ALTERNATIVE_TYPES,
    LIST_TYPES,
    RESTRICTED_REFERNECE_TYPES,
    GENERAL_REFERNCE_TYPES,
    NAMED_VALUE_TYPES,
    STRUCT_VALUE_TYPES,
    SET_VALUE_TYPES,
    MAX_VALUE_TYPES
} COLUMN_ENCODING_TYPES;

typedef 
struct _UIDTOSTRING { 
    UINT8       *String;
    UINT64      Uid;
}UIDTOSTRING;
// Function Declaration

UINT16
ToBigLittleEndianWord (
    UINT16 EndianWord
);

UINT32
ToBigLittleEndianDword (
    UINT32 EndianWord
);

UINT64
ToBigLittleEndianQword (
    UINT64  EndianQword
);

EFI_STATUS
CheckCloseSession (
    VOID            *Buffer,
    UINTN           BufferLength
);

EFI_STATUS
GetStatusValue (
    VOID            *Buffer,
    UINTN           BufferLength,
    UINT8           *bData
);

EFI_STATUS 
GetColumnValue (
    OPAL_DEVICE     *OpalDevice, 
    VOID            *Buffer, 
    UINTN           BufferLength,
    CELL_BLOCK      *CellBlock,
    UINT32          *CpinLength,
    UINT8           *Cpin
);

EFI_STATUS
BuildSetBooleanExpressionCmdBlock (
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    VOID                *Buffer,
    UINTN               BufferLength,
    UINT64              TableObjUid,
    UINT64              Uid1,
    UINT64              Uid2,
    BOOLEAN             BooleanOperator
);

EFI_STATUS
BuildBasicCmdBlock (
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    VOID                *Buffer,
    UINTN               BufferLength,
    UINT64              InvokingID,
    UINT64              MethodUid
);

EFI_STATUS
BuildMbrUidSetCmdBlock(
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    UINT64              Offset,
    UINT8               *MbrBuffer,
    UINT64              MbrBufferLength,
    VOID                *Buffer,
    UINTN               BufferLength
);

EFI_STATUS
BuildStartSessionCmdBlock (
    OPAL_DEVICE     *OpalDevice,
    VOID            *Buffer,
    UINTN           BufferLength,
    SESSION_PARAMS   *SessionParams
);

EFI_STATUS
BuildEndOfSessionCmdBlock (
    OPAL_DEVICE     *OpalDevice,
    VOID            *Buffer,
    UINTN           BufferLength,
    SESSION_PARAMS  *SessionParams
);

EFI_STATUS
BuildGetCmdBlock (
    OPAL_DEVICE     *OpalDevice,
    VOID            *Buffer,
    UINTN           BufferLength,
    SESSION_PARAMS  *SessionParams,
    UINT64          TableObjUid,
    CELL_BLOCK      *CellBlock
);

EFI_STATUS
BuildSetCmdBlock (
    OPAL_DEVICE     *OpalDevice,
    SESSION_PARAMS  *SessionParams,
    VOID            *Buffer,
    UINTN           BufferLength,
    UINT64          TableObjUid,
    UINT8           Column,
    CHAR8           *ColumnName,
    UINT8           *Pin,
    UINTN           PinLength,
    BOOLEAN			ByteInteger,
    COLUMN_ENCODING_TYPES EncodingType,
    BOOLEAN         Header,
    BOOLEAN         Tail
);

EFI_STATUS
BuildSpObjectUidAuthenticate( 
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    VOID                *Buffer, 
    UINTN               BufferLength,
    UINT64              InvokingUid,
    UINT64              MethodUid,
    UINT64              AuthUid,
    UINT8               *Challenge,
    UINT8               ChallengeLength
);

EFI_STATUS
DecodeTokenHeader (
    UINT8   *DataSubPacket,
    UINT8   *DataBuffer,
    UINT32  *DataBufferLength,
    UINT32  *Length,
    UINT8   *HeaderSize
);

EFI_STATUS
SpObjectUidAuthenticate (
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    VOID                *Buffer,
    UINTN               BufferLength,
    UINT64              AuthObj,
    UINT8               *Challenge,
    UINT32              ChallengeLength
);

EFI_STATUS
ObjectUidSetBooleanExpression (
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    VOID                *Buffer,
    UINTN               BufferLength,
    UINT64              TableObjUid,
    UINT64              Uid1,
    UINT64              Uid2,
    BOOLEAN             BooleanOperator
);

EFI_STATUS
ObjectUidSet (
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    VOID                *Buffer,
    UINTN               BufferLength,
    UINT64              TableObjUid,
    UINT8               Column,
    CHAR8               *ColumnName,
    UINT8               *Pin,
    UINTN               PinLength,
    BOOLEAN				ByteInteger,
    COLUMN_ENCODING_TYPES EncodingType
);

EFI_STATUS
SpObjectUidRevert (
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    VOID                *Buffer,
    UINTN               BufferLength,
    UINT64              SPUid,
    UINT64              RevertUid,
    UINT8               KeepGlobalRangeKey
);

EFI_STATUS
EnableBlockSID (
    OPAL_DEVICE         *OpalDevice,
    VOID                *Buffer,
    UINTN               BufferLength
);

EFI_STATUS
SMUidStartSession (
    OPAL_DEVICE     *OpalDevice,
    SESSION_PARAMS  *SessionParams,
    VOID            *Buffer,
    UINTN           BufferLength
);

EFI_STATUS
SMUidCloseSession (
    OPAL_DEVICE     *OpalDevice,
    SESSION_PARAMS  *SessionParams,
    VOID            *Buffer,
    UINTN           BufferLength
);

EFI_STATUS
DecodeSyncSessionData (
    VOID                *Buffer,
    UINTN               BufferLength,
    SESSION_PARAMS      *SessionParams
);

EFI_STATUS
SearchNameTokenInList (
    OPAL_DEVICE     *OpalDevice,        
    UINT8           *ListPtr, 
    UINTN           ListPtrSize, 
    CELL_BLOCK      *CellBlock,
    UINTN           *Offset
);

EFI_STATUS
SearchProperties (
    VOID                *Buffer,
    UINTN               BufferLength,
    UINT8               *String,
    UINT32              *Value
);

#if defined(TCG_STORAGE_SEC_VERBOSE_PRINT) && (TCG_STORAGE_SEC_VERBOSE_PRINT == 1)
VOID
PrintCmdBlock (
    VOID            *Buffer
);
#endif

EFI_STATUS
GetResponse (
    IN  OPAL_DEVICE         *OpalDevice,
    IN  VOID                *Buffer,
    IN  UINTN               BufferLength,
    IN  SESSION_PARAMS      *SessionParams
);

EFI_STATUS
ExecuteSendTcgCmd (
    IN  OPAL_DEVICE         *OpalDevice,
    IN  UINT8               SecurityProtocolID,
    IN  UINT16              SPSpecificID,
    IN  VOID                *Buffer,
    IN  UINTN               BufferLength
);


EFI_STATUS
TcgSecAllocatePool(
  IN  EFI_MEMORY_TYPE              PoolType,
  IN  UINTN                        Size,
  OUT VOID                         **Buffer
  );

EFI_STATUS
TcgSecFreePool(
  IN  VOID                         *Buffer
  );

EFI_STATUS
OpenSessionAndAuthenticate (
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    VOID                *Buffer,
    UINTN               BufferLength,
    UINT64              AuthObj1,
    UINT64              AuthObj2,
    UINT8               *Pwd,
    UINT32              PwdLength
);
/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif

#endif

//***********************************************************************
//***********************************************************************
//**                                                                   **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.          **
//**                                                                   **
//**                       All Rights Reserved.                        **
//**                                                                   **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093         **
//**                                                                   **
//**                       Phone: (770)-246-8600                       **
//**                                                                   **
//***********************************************************************
//***********************************************************************
