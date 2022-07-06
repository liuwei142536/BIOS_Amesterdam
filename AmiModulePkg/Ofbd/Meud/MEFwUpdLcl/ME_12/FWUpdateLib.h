/*++

This file contains a 'Sample Driver' and is licensed as such
under the terms of your license agreement with Intel or your
vendor.  This file may be modified by the user, subject to  
the additional terms of the license agreement               

--*/

/*++

Copyright (c)  2009-13 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  FwUpdateLib.h

Abstract:

  FW Update Local Sample Code Header file
  
--*/

#define SUCCESS                     0
#define INVALID_DATA_FORMAT_VERSION 0
#define INVALID_PARTITION_START     0
#define INVALID_MANIFEST_DATA       1
#define NO_FPT_IMAGE                2
#define MANIFEST_BUFFER             0x1000
#define FPT_PARTITION_NAME_FPT      0x54504624

//=====
#define bool BOOLEAN
#define CHAR CHAR8
#define MAXIMUM_IPU_SUPPORTED       4
#define FPT_PARTITION_NAME_ISHC 0x43485349

typedef UINT32 STATUS;

typedef struct
{
   unsigned long  Data1;
   unsigned short Data2;
   unsigned short Data3;
   unsigned char  Data4[8];
} _UUID;

typedef enum _FWU_OPERATION
{
   FWU_FULL_UPDATE_OPERATION   =   0,
   FWU_IPU_UPDATE_OPERATION
}FWU_OPERATION;

typedef enum _FWU_ENVIRONMENT
{
   FWU_ENV_MANUFACTURING = 0,   // Manufacturing update
   FWU_ENV_IFU,                 // Independent Firmware update
}FWU_ENVIRONMENT;

#define FPT_PARTITION_NAME_RBEP         0x50454252 /**< "RBEP" Rom Boot Extension Partition Name */
#define FPT_PARTITION_NAME_FTPR         0x52505446 /**< "FTPR" Fault Tolerant Partition Name*/
#define FPT_PARTITION_NAME_NFTP         0x5054464E /**< "NFTP" Non-Fault Tolerant Partition Name */  
#define FPT_PARTITION_NAME_WCOD         0x444f4357 /**< "WCOD" Partition Name */
#define FPT_PARTITION_NAME_LOCL         0x4C434F4C /**< "LOCL" Partition Name */
#define FPT_PARTITION_NAME_ISHC         0x43485349 /**< "ISHC" Partition Name*/
#define FPT_PARTITION_NAME_PMCP         0x50434D50 /**< "PMCP" Partition Name*/
#define FPT_PARTITION_NAME_OEMP         0x504D454F /**< "OEMP" Partition Name*/
#define FPT_PARTITION_NAME_IUNP         0x504E5549 /**< "IUNP" Partition Name*/
#define FPT_PARTITION_NAME_IOMP         0x504D4F49 /**< "IOMP" Partition Name*/
#define FPT_PARTITION_NAME_MGPP         0x5050474D /**< "MGPP" Partition Name*/
#define FPT_PARTITION_NAME_TBTP         0x50544254 /**< "TBTP" Partition Name*/
#define FPT_PARTITION_NAME_DPHY         0x59485044 /**< "DPHY" Partition Name */

enum errorValues {
    FWU_ERROR_SUCCESS,
    FWU_IME_NO_DEVICE = 8193,
    FWU_UPD_VER_MIS   = 8199,
    FWU_VER_GET_ERR = 8204,
    FWU_CERT_ERR = 8213,
    FWU_REBOOT_NEEDED = 8703,
    FWU_SKU_MISMATCH,
    FWU_VER_MISMATCH,
    FWU_SGN_MISMATCH,
    FWU_GENERAL,
    FWU_UPD_PROCESS,
    FWU_NO_MEMORY = 8710,
    FWU_AUTH = 8712,
    FWU_IMG_HEADER,
    FWU_FILE_OPEN,
    FWU_HTTP_ERROR,
    FWU_USAGE_ERROR,
    FWU_HOSTNAME,
    FWU_UPDATE_TIMEOUT,
    FWU_LOCAL_DIS,
    FWU_SECURE_DIS,
    FWU_IME_UN_SUP_MESS = 8722,
    FWU_NO_UPDATE,
    FWU_IME_NOT_READY,
    FWU_LAST_STATUS,
    FWU_GET_VER_ERR = 8727,
    FWU_IME_SMALL_BUFF,
    FWU_WSMAN_NO = 8734,
    FWU_UNSUPPRT_OS = 8740,
    FWU_ERROR_FW,
    FWU_HECI,
    FWU_UNSUPPRT_PLAT,
    FWU_VERIFY_OEM_ID_ERR,
    FWU_INVALID_OEM_ID = 8745,
    FWU_INVALID_IMG_LENGTH,
    FWU_GLBL_BUFF_UNAVAILABLE,
    FWU_INVALID_FW_PARAMS,
    FWU_AMT_STATUS_INTERNAL_ERROR,
    FWU_AMT_STATUS_NOT_READY = 8750,
    FWU_AMT_STATUS_INVALID_AMT_MODE,
    FWU_AMT_STATUS_INVALID_MESSAGE_LENGTH,
    FWU_SAVE_RESTORE_POINT_ERROR,
    FWU_FILE_WRITE,
    FWU_GET_BLIST_ERROR = 8755,
    FWU_CHECK_VERSION_ERROR,
    FWU_DISPLAY_FW_VERSION,
    FWU_IMAGE_UNDER_VCN,
    FWU_IMAGE_VER_HIST_CHK_FAIL,
    FWU_DOWNGRADE_VETOED = 8760,
    FWU_FW_WRITE_FILE_FAIL,
    FWU_FW_READ_FILE_FAIL,
    FWU_FW_DELETE_FILE_FAIL,
    FWU_PARTITION_LAYOUT_NOT_COMP,
    FWU_DOWNGRADE_NOT_ALLOWED_DATA_MISMATCH = 8765,
    FWU_UPDATE_PASSWORD_NOT_MATCHED,
    FWU_UPDATE_PASSWORD_EXCEED_MAXIMUM_RETRY,
    FWU_UPDATE_PASSWORD_NOT_PROVIDED,
    FWU_UPDATE_POLLING_FAILED,
    FWU_FILE_ALREADY_EXISTS = 8770,
    FWU_FILE_INVALID,
    FWU_USAGE_ERROR_B,
    FWU_AUDIT_POLICY_FAILURE,
    FWU_ERROR_CREATING_FT,
    FWU_SAL_NOTIFICATION_ERROR = 8775,
    FWU_UPD_IMG_LOADING,
    FWU_UPD_IMG_AUTHENTICATING,
    FWU_UPD_IMG_PROCESSING,
    FWU_UPD_CREATING_FT,
    FWU_UPD_UPDATING_CODE,
    FWU_UPD_UPDATING_NFT,
    FWU_FLASH_CODE_PARTITION_INVALID,
    FWU_FLASH_NFT_PARTITION_INVALID,
    FWU_ILLEGAL_IMAGE_LENGTH,
    FWU_HOST_RESET_REQUIRED,
    FWU_INVALID_GLUT,
    FWU_GET_PATTRIB_ERROR,
    FWU_GET_UPD_INFO_STATUS,
    FWU_PID_NOT_EXPECTED,
    FWU_UPDATE_INRECOVERY_MODE_RESTRICT_UPDATE_TO_ATTEMPTED_VERSION,
    FWU_BUFFER_COPY_FAILED,
    FWU_GET_ME_FWU_INFO,
    FWU_APP_REGISTER_OS_FAILURE,
    FWU_APP_UNREGISTER_OS_FAILURE,
    FWU_INVALID_PARTID,
    FWU_LIVE_PING_FAILURE,
    FWU_SERVICE_CONNECT_FAILURE,
    FWU_SERVICE_NOT_AVAILABLE,
    FWU_SERVICE_BUSY,
    FWU_USER_NOT_ADMIN,
    FWU_WMI_FAIL,
    FWU_CHK_BIT_LOCKER_FAIL,
    FWU_REG_CMD_FAIL,
    FWU_UPDATE_IMAGE_BLACKLISTED,
    FWU_DOWNGRADE_NOT_ALLOWED_SVN_RESTRICTION,
    FWU_INVALID_POINTER,
    FWU_UPV_VERSION_MISMATCHED,
    FWU_INSTID_IS_NOT_EXPECTED_ID,
    FWU_INFO_NOT_AVAILABLE,
    FWU_REJ_IPU_FULL_UPDATE_NEEDED,
    FWU_IPU_NAMEID_NOT_FOUND,
    FWU_RESTORE_POINT_INVALID,
    FWU_RESTORE_POINT_VALID_BUT_NOT_LATEST,
    FWU_RESTORE_POINT_OPERATION_NOT_ALLOWED,
    FWU_DOWNGRADE_NOT_ALLOWED_VCN_RESTRICTION,
    FWU_INVALID_SVN,
    FWU_OUT_OF_SVN_RESOURCES,
    FWU_REJECT_RESTORE_POINT_REQUEST_FLASH_IN_RECOVERY,
    FWU_REJECTED_BY_UPDATE_POLICY,
    FWU_REJECTED_INCOMPATIBLE_TOOL_USAGE,
    FWU_REJECTED_CROSSPOINT_UPDATE_NOT_ALLOWED,
    FWU_REJECTED_CROSSHOTFIX_UPDATE_NOT_ALLOWED,
    FWU_REJECTED_CURRENT_FW_NOT_ELIGIBLE_FOR_UPDATE,
    FWU_REJECTED_WRONG_UPDATE_OPERATION,
    FWU_REJECTED_WRONG_UPDATE_IMAGE_FOUND,
    FWU_REJECTED_IFR_UPDATE_NOT_ALLOWED,
    FWU_FAILURE_OCCURRED_DURING_ROLLBACK,
    FWU_RESTORE_POINT_OPERATION_FAILED,
    FWU_ERROR_DETECT_POWER_SOURCE,
    FWU_DISPLAY_PART_VERSION,
    FWU_PART_NOT_PRESENT,
    FWU_ERROR_REQUEST_REPLY_SIZE_MISMATCH,
    FWU_UNSUPPRT_SKU_SLIM,
    FWU_REJECT_RESTORE_POINT_REQUEST_RESTART_NEEDED,
    FWU_ERROR_PMC_INSTANCE,
    FWU_ERROR_H_LP_MISMATCH,  
    FWU_ERROR_INIT_PMX,
    FWU_ERROR_REJECT_RESTORE_POINT_REQUEST_RESTART_NEEDED,
    FWU_ERROR_PMC_INSTANCE_MISMATCH,
    FWU_ERROR_UPD_IMG_TOO_BIG,
    FWU_ERROR_INVALID_MANIFEST_SIZE,
    FWU_ERROR_OPEN_LDR_VER_MAN_FAILED,
    FWU_ERROR_OPEN_LDR_KEYS_FAILED,
    FWU_ERROR_INSTALL_KEYS_FAILED,
    FWU_ERROR_UNINSTALL_KEYS_FAILED,
    FWU_ERROR_OPEN_LDR_INSTALL_FAILED,
    FWU_ERROR_INSTALL_FAILED,
    FWU_ERROR_PREUPDATE_VFS,
    FWU_ERROR_SPAWN_FAILED, 
    FWU_ERROR_WAITPID_FAILED,
    FWU_ERROR_PREUPDATE_FAILED,
    FWU_ERROR_SKUMGR_FAILED,
    FWU_ERROR_CFGMGR_FAILED,
    FWU_ERROR_MAN_NOT_FOUND,
    FWU_ERROR_CRYPTO_FAILED,
    FWU_ERROR_VER_MAN_FAILED_FTPR,
    FWU_ERROR_VER_MAN_FAILED_NFTP,
    FWU_ERROR_VER_MAN_FAILED_DLMP,
    FWU_ERROR_VER_MAN_FAILED_RBEP,
    FWU_ERROR_VER_MAN_FAILED_PMCP,
    FWU_ERROR_VER_MAN_FAILED_OEMP,
    FWU_ERROR_VER_MAN_FAILED_WCOD,
    FWU_ERROR_VER_MAN_FAILED_LOCL,
    FWU_ERROR_VER_MAN_FAILED_PAVP,
    FWU_ERROR_VER_MAN_FAILED_IOMP,
    FWU_ERROR_VER_MAN_FAILED_MGPP,
    FWU_ERROR_VER_MAN_FAILED_TBTP,
    FWU_ERROR_VER_MAN_FAILED_ISHC,
    FWU_ERROR_VER_MAN_FAILED_IUNP,
    FWU_ERROR_VER_MAN_FAILED_UPDT,
    FWU_ERROR_GET_EXT_FAILED_FTPR,
    FWU_ERROR_GET_EXT_FAILED_NFTP,
    FWU_ERROR_GET_EXT_FAILED_DLMP,
    FWU_ERROR_GET_EXT_FAILED_RBEP,
    FWU_ERROR_GET_EXT_FAILED_PMCP,
    FWU_ERROR_GET_EXT_FAILED_OEMP,
    FWU_ERROR_GET_EXT_FAILED_WCOD,
    FWU_ERROR_GET_EXT_FAILED_LOCL,
    FWU_ERROR_GET_EXT_FAILED_PAVP,
    FWU_ERROR_GET_EXT_FAILED_IOMP,
    FWU_ERROR_GET_EXT_FAILED_MGPP,
    FWU_ERROR_GET_EXT_FAILED_TBTP,
    FWU_ERROR_GET_EXT_FAILED_ISHC,
    FWU_ERROR_GET_EXT_FAILED_IUNP,
    FWU_ERROR_GET_EXT_FAILED_UPDT,
    FWU_ERROR_INTEGRITY_FAILED_FTPR,
    FWU_ERROR_INTEGRITY_FAILED_NFTP,
    FWU_ERROR_INTEGRITY_FAILED_DLMP,
    FWU_ERROR_INTEGRITY_FAILED_RBEP,
    FWU_ERROR_INTEGRITY_FAILED_PMCP,
    FWU_ERROR_INTEGRITY_FAILED_OEMP,
    FWU_ERROR_INTEGRITY_FAILED_WCOD,
    FWU_ERROR_INTEGRITY_FAILED_LOCL,
    FWU_ERROR_INTEGRITY_FAILED_PAVP,
    FWU_ERROR_INTEGRITY_FAILED_IOMP,
    FWU_ERROR_INTEGRITY_FAILED_MGPP,
    FWU_ERROR_INTEGRITY_FAILED_TBTP,
    FWU_ERROR_INTEGRITY_FAILED_ISHC,
    FWU_ERROR_INTEGRITY_FAILED_IUNP,
    FWU_ERROR_INTEGRITY_FAILED_UPDT,
    FWU_ERROR_PART_SIZE,
    FWU_ERROR_BACKUP_OUTSIDE_NFTP,
    FWU_ERROR_MAX_IUPS,
    FWU_ERROR_NOT_IUP,
    FWU_ERROR_IUP_MISSING_UPDATE,
    FWU_ERROR_PMC_MISSING_UPDATE,
    FWU_ERROR_NOT_PARTIAL_IUP,
    FWU_ERROR_PARTIAL_TCSS,
    FWU_ERROR_FTPR_VER,
    FWU_ERROR_FTPR_SVN,
    FWU_ERROR_FTPR_VCN,
    FWU_ERROR_FTPR_VER_MAJOR,
    FWU_ERROR_IUP_SVN,
    FWU_ERROR_IUP_VCN,
    FWU_ERROR_IMAGE_LEN,
    FWU_ERROR_PV_BIT,
    FWU_ERROR_REVENUE,
    FWU_ERROR_SVN_UPGRADE,
    FWU_ERROR_SVN_HOTFIX,
    FWU_ERROR_API_VER_MAJOR_FTPR,
    FWU_ERROR_API_VER_MAJOR_NFTP,
    FWU_ERROR_API_VER_MAJOR_DLMP,
    FWU_ERROR_API_VER_MAJOR_RBEP,
    FWU_ERROR_API_VER_MAJOR_PMCP,
    FWU_ERROR_API_VER_MAJOR_OEMP,
    FWU_ERROR_API_VER_MAJOR_WCOD,
    FWU_ERROR_API_VER_MAJOR_LOCL,
    FWU_ERROR_API_VER_MAJOR_PAVP,
    FWU_ERROR_API_VER_MAJOR_IOMP,
    FWU_ERROR_API_VER_MAJOR_MGPP,
    FWU_ERROR_API_VER_MAJOR_TBTP,
    FWU_ERROR_API_VER_MAJOR_ISHC,
    FWU_ERROR_API_VER_MAJOR_IUNP,
    FWU_ERROR_API_VER_MAJOR_UPDT,
    FWU_ERROR_IUP_MISSING_FLASH,
    FWU_ERROR_PARTITION_NOT_FOUND,
    FWU_ERROR_ENGINEERING_MISMATCH,
    FWU_ERROR_OEMP_MISSING,
    FWU_ERROR_IUPS_NOT_COMPATIBLE,
    FWU_ERROR_OEMP_IN_UPDATE,
    FWU_ERROR_WRONG_IUP,
    FWU_ERROR_IMAGE_IUP_SIZE,
    FWU_ERROR_OPEN_IUP,
    FWU_ERROR_SPI_IUP,
    FWU_ERROR_RULE_FILE_INVALID,
    FWU_ERROR_PWDMGR_FAILED,
    FWU_ERROR_PWR_FAILED,
    FWU_ERROR_SPI_FAILED,
    FWU_ERROR_RESTORE_POINT_ALREADY_STARTED,
    FWU_ERROR_RESTORE_POINT_OFFSET_INVALID,
    FWU_ERROR_WRONG_HECI_MSG_LENGTH,
    FWU_ERROR_ENV_INVALID,
    FWU_ERROR_WRONG_DATA_OPERATION,
    FWU_ERROR_DATA_LENGTH_INVALID,
    FWU_ERROR_WRONG_END_OPERATION,
    FWU_ERROR_FLUSH_NVM_ERR,
    FWU_ERROR_IPK_FAILURE,
    FWU_ERROR_PG_FAILURE,
    FWU_ERROR_INVALID_FLASH_IMAGE,
    FWU_ERROR_INVALID_UPDATE_IMAGE,
    FWU_ERROR_NO_IUP_IN_BASE_IMAGE,
    FWU_FILE_INVALID_MISSING_IUNP,
    FWU_FILE_INVALID_MISSING_ISHC,
    FWU_FILE_INVALID_MISSING_IUNP_IN_FLASH,
    FWU_FILE_INVALID_MISSING_ISHC_IN_FLASH,
    FWU_FILE_INVALID_MISSING_OEMP,
    FWU_FILE_INVALID_MISSING_OEMP_IN_FLASH,
    FWU_ERROR_GET_PLATFORM_TYPE,
    FWU_DISPLAY_PART_VENDOR_ID,
    FWU_INVALID_PARTID_ALL,
    FWU_ERROR_SVN_TCB_ARB,
    FWU_LAST_STATUS_CODE    
};

typedef struct {
	UINT16 Major;
	UINT16 Minor;
	UINT16 Hotfix;
	UINT16 Build;
} FWVersion, VERSION;;

typedef struct {
	FWVersion code;
	FWVersion rcvy;
} FlashVersion;

typedef struct _PT_ATTRB
{
   unsigned int   PtNameId;      // HW_COMP_HDR_STRUCTID_WCOD     0x244f4357 OR 
                           // HW_COMP_HDR_STRUCTID_LOCL     0x4C434F4C OR
                           // HW_COMP_HDR_STRUCTID_MDMV     0x564D444D 
   unsigned int   LoadAddress;         // Load Address of the IPU
   VERSION  FwVer;         // FW version from IUP Manifest
   unsigned int   CurrentInstId; // Current Inst ID from flash, 0 indicate invalid ID 
   unsigned int   CurrentUpvVer; // Upper sig 16 bits are Major Version.
   unsigned int   ExpectedInstId;// Expected Inst ID that need to be updated to
   unsigned int   ExpectedUpvVer;// Upper sig 16 bits are Major Version.
   unsigned int   Resv[4];
}PT_ATTRB;

typedef struct _FWU_GET_IPU_PT_ATTRB_MSG_REPLY
{
   unsigned int   MessageType;      // Internal FWU tool use only
   STATUS   Status;           // Internal FWU tool use only
   VERSION  FtpFwVer;         // FW version in Fault Tolerance Partition. 
                              // This might be used for diagnostic or debug.
   unsigned int   SizeoOfPtAttrib;  // Size in bytes. Simply is the sizeof (PT_ATTRB structure)
   unsigned int   NumOfPartition;   // Number of partition actually return in this reply message
   PT_ATTRB  PtAttribute[MAXIMUM_IPU_SUPPORTED];
   unsigned int   Resv[4];
}FWU_GET_IPU_PT_ATTRB_MSG_REPLY;
//=====

// below structure is defiend by the fw team
// in fwucommon.h file
typedef struct _UPDATE_FLAGS_LIB
{
   unsigned int RestorePoint:1;           // If set indicate restore point
   unsigned int RestartOperation:1;       // If set indicate restart operation, like lost hard drive etc...
   unsigned int UserRollback:1;           // indicates user has initiated a rollback
   unsigned int Reserve:29;               //
}UPDATE_FLAGS_LIB;

// Used by the tool to perform FULL FW update
typedef enum _UPDATE_TYPE
{
   DOWNGRADE_SUCCESS = 0,
   DOWNGRADE_FAILURE,
   SAMEVERSION_SUCCESS,
   SAMEVERSION_FAILURE,
   UPGRADE_SUCCESS,
   UPGRADE_PROMPT
}UPDATE_TYPE;

//Image type to validate the binary sent to update
//For Full Update - only FULL image type is valid
//For Partial Update - only FULL and PARTIAL image type is valid
//FULL Image => Image with Flash Partition Table, FTPR, and NFTPR
//PARTIAL Image => Image with no Flash Partition Table or FTPR or NFTPR,
//						only WCOD or LOCL
typedef enum _IMAGE_TYPE
{
   FULL = 0,
   PARTIAL,
   RESTORE,
   INVALID
}IMAGE_TYPE;


typedef enum _SKU_TYPE
{
   SKU_1_5_MB = 0,
   SKU_5_MB,
   SKU_RESERVED,
   SKU_INVALID
} SKU_TYPE;
/*
typedef enum _PCH_SKU
{
   PCH_SKU_H = 0,
   PCH_SKU_LP,
   PCH_SKU_INVALID
} PCH_SKU;
*/
typedef enum _FWUPD_POWER_SOURCE
{
    FWUPD_POWER_SOURCE_AC  = 1,  ///< AC Power source
    FWUPD_POWER_SOURCE_DC  = 2,  ///< DC Power source
    FWUPD_POWER_SOURCE_UNKOWN    ///< Unable to determine power source
} FWUPD_POWER_SOURCE;

//Used by the tool to retrieve FW version information
typedef struct {
	unsigned short Major;
	unsigned short Minor;
	unsigned short Hotfix;
	unsigned short Build;
}VersionLib;
//Should be used by both tool and UNS to retrieve the Updated UPV version
typedef struct _IPU_UPDATED_INFO
{
	unsigned int UpdatedUpvVer;//Version from the update image file that is for updating IPU
	unsigned int Reserved[4];
}IPU_UPDATED_INFO;

// disable the "zero-sized array" warning
#pragma warning(disable:4200)
typedef enum _FWU_STATUS {
	FWU_STATUS_SUCCESS = 0,
	FWU_STATUS_NO_MEMORY = 1,
	FWU_STATUS_NOT_READY = 2,
	FWU_STATUS_ILLEGAL_LENGTH = 3,
	FWU_STATUS_AUTHENTICATION_FAILED = 4,
	FWU_STATUS_INTERNAL_ERROR = 5,
	FWU_STATUS_SKU_FAILURE = 6,
	FWU_STATUS_VERSION_FAILURE = 7
}FWU_STATUS;

// From FWU_if.h

#pragma pack(1)

/**
 * FWU_GET_INFO_MESSAGE - get version
 *
 * @MessageType: FWU_MESSAGE_TYPE_GET_INFO
 */
typedef struct {
    UINT32 MessageType;
} FWU_GET_INFO_MESSAGE;

/**
 * FWU_DATA_MESSAGE - data fragment of the image
 *
 * @MessageType: FWU_MESSAGE_TYPE_DATA
 * @Length: The length of the data field, in Little Endian
 * @Reserved: Reserved, must be 0
 * @Data: The data of the image fragment
 */
typedef struct {
    UINT32 MessageType;
    UINT32 Length;
    UINT8 Reserved[3];
#ifndef _DOS
    UINT8 Data[0];
#endif
} FWU_DATA_MESSAGE;

#pragma pack()

bool 
IsUpdateStatusPending (
  IN  UINT32    status
)
/*++

Routine Description:

  Checks if ME FW Update client is ready to accept an update

Arguments:


Returns:

  true if ready

--*/
;

UINT32 
CheckPolicyBuffer (
  IN  char*       buffer, 
  IN  int         bufferLength, 
  IN  int         AllowSV, 
  IN  UPDATE_TYPE *Upd_Type,
	IN  VersionLib  *ver
)
/*++

Routine Description:

  This routine determines if the update is allowed given the ME Upgrade/downgrade/same version policy

Arguments:

Returns:

  FWU_ERROR_SUCCESS if operation allowed
  
--*/
;

UINT32
FwUpdateFullBuffer (
  IN  char                *buffer,
  IN  UINT32        bufferLength, 
  IN  char                *_pwd,
  IN  int                 _forceResetLib,
  IN  UINT32       UpdateEnvironment,	
  IN  _UUID               OemID,
  IN  UPDATE_FLAGS_LIB    update_flags,
  IN  void(*func)(UINT32, UINT32)
)
/*++

Routine Description:

  This routine sends the buffer to the ME FW Update Client

Arguments:

Returns:

  FWU_ERROR_SUCCESS if operation allowed
  
--*/
;

UINT32 
    FwUpdatePartialBuffer (
    IN  char             *buffer,
    IN  UINT32            bufferLength, 
    IN  UINT32            PartitionID,
    IN UINT32             Flags,
    IN IPU_UPDATED_INFO   *IpuUpdatedInfo,
    IN  char              *_pwd,
    //IN  int             _forceResetLib,
    IN  UINT32            UpdateEnvironment,    
    IN  _UUID             OemID,
    IN  UPDATE_FLAGS_LIB  update_flags,
    IN  void(*func)(UINT32, UINT32)
    );

UINT32
FWUpdate_QueryStatus_Get_Response (
  IN  UINT32  *UpdateStatus,
  IN  UINT32   *TotalStages,
  IN  UINT32   *PercentWritten,
  IN  UINT32   *LastUpdateStatus,
  IN  UINT32   *LastResetType
)
/*++

Routine Description:

  This routine queries the ME Kernel for the update status

Arguments:

Returns:

  FWU_ERROR_SUCCESS if operation allowed
  
--*/
;

bool
VerifyOemId(
  IN _UUID      id
)
/*++

Routine Description:

  The ME FW when created has an OEM ID embedded in it.  For security
  reasons, we need to verify that the OEM ID of the application (mOemID)
  matches the OEM ID of the ME FW.

  NOTE: A connection to the client is required before this routine is 
  executed.

Arguments:

  UUID to check
  

Returns:

  true or false indicating if the match was succesfull
  
--*/
;

EFI_STATUS
GetLastStatus(
  OUT UINT32          *LastFwUpdateStatus
)
/*++

Routine Description:
  
  Get the last status of the FW Update Client - this is used to determine if a previous update
  requires a reboot

Arguments:

  UINT32 pointer for results of previous update. 
  

Returns:

  EFI Error code
  
--*/
;

EFI_STATUS
GetLastUpdateResetType (
  OUT UINT32            *LastResetType
)/*++

Routine Description:
  
  Get the last status of the FW Update Client - this is used to determine if a previous update
  requires a reboot

Arguments:

  UINT32 pointer for results of previous update. 
  

Returns:

  EFI Error code
  
--*/
;

EFI_STATUS
GetInterfaces (
  OUT UINT16          *interfaces
)
;

EFI_STATUS
CheckVersion(
  IN  UINT8           *FileBuffer,
  IN  BOOLEAN         AllowSV,
  OUT FlashVersion    *FlashVersion,
  OUT UINT32          *LastFwUpdateStatus
)
/*++

Routine Description:

  Policy routine to verify if the current FW version is older
  than the version in the image. 

  The routine will return the result of a previous ME FW Update.  If a reset is required
  before an update can occur, the return value will indicate it

  NOTE: A connection to the client is required before this routine is 
  executed.

Arguments:

  UINT8 pointer to update image
  BOOLEAN indicating TRUE for same version update
  FlashVersion pointer for return version numbers of FW Update image
  UINT32 pointer for results of previous update. 
  

Returns:

  EFI_UNSUPPORTED if version is of update image is not supported
  
--*/
;

EFI_STATUS 
StartUpdate(
  IN  UINT8   *FileBuffer,
  IN  UINT32  FileLength,
  IN  CHAR   *Password,
  IN  VOID    (*DataProgressProc)(),
  IN  UINT32  DataProgessSteps,
  OUT STATUS  *FWU_Status
)
/*++

Routine Description:

  Start the update process by asking the ME FW Update client
  to allocate a buffer for the image.  Note that the ME does not update the flash until
  the FWU_END message is received. 

  If the current ME Flash image is password protected, then that password is required to unlock
  the update process

  Because the process of downloading the image is time consuming, a callback is provided for each
  chunk of data sent to the ME FW Update client.  If desired, the caller can specify the number of steps
  that the total data send will have (e.g. 100 means a total of 100 callbacks for the entire duration). The
  number of steps supplied will be divided by the total result of (FileLength / Chunksize) + 1. 

Arguments:

  UINT8 pointer to FW Update Image
  UINT32 FW Update image length
  CHAR16 pointer to password (if no password then NULL)
  VOID pointer to callback function (if no callback then NULL)
  UINT32 callback steps (1 means each packet sent will trigger a callback)
  UINT32 pointer to ME FW Update Client return status

Returns:

  EFI_STATUS
  
--*/
;

EFI_STATUS
EndUpdate (
  IN  VOID    (*EndProgressProc)(),
  IN  UINT32  EndProgressSteps,
  OUT STATUS  *FWU_Status
)
/*++

Routine Description:

  Send the FWU_END message to the ME after a successful data download.
  This operation will request the ME to validate the image and update the
  flash.  Depending on flash speed, the operation may take serveral minutes
  to finish.  Be patient...

  Because the process of updating the image is time consuming, a callback is provided each delay (5 seconds)
  that occurs waiting for the ME FW Update client to respond.  If desired, the caller can specify a number of timeouts before
  a callback (e.g. 1 means send wait 5 seconds then invoke the callback routine).

Arguments:

  VOID pointer to callback function (if no callback then NULL)
  UINT32 callback ratio (1 means wait 5 seconds and then trigger a callback)
  UINT32 pointer to ME FW Update Client return status

Returns:

  EFI_STATUS
  
--*/
;

EFI_STATUS
GetFwFlashVersion(
  IN  FWVersion  *fwVersion
)
/*++

Routine Description:

  Get the FW version of the currently running ME FW

  NOTE: A connection to the client is required before this routine is 
  executed.

Arguments:

  FWVersion pointer to FW Flash version

Returns:

  EFI_STATUS

--*/
;
unsigned int FwUpdateCheckPowerSource(
    FWUPD_POWER_SOURCE* PowerState);

UINT32 GetExtendedIpuPartitionAttributes(
    FWU_GET_IPU_PT_ATTRB_MSG_REPLY* FwuGetIpuAttrbMsgInfo,
    UINT32 updateOp);

UINT32 GetIpuPartitionAttributes(FWU_GET_IPU_PT_ATTRB_MSG_REPLY* FwuGetIpuAttrbMsgInfo);

EFI_STATUS
HeciPdt (
    IN  char                *buffer,
    IN  UINT32        bufferLength
  );


int GetPartVersion(UINT32 partID, UINT16 *major, UINT16 *minor, UINT16 *hotfix, UINT16 *build);
int GetPartVendorID(UINT32 partID, UINT32 *vendor_id);
int strncmp(const char *s1, const char *s2, size_t n);

