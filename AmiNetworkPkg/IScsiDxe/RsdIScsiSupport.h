//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

#ifndef _RSD_ISCSI_SUPPORT_H_
#define _RSD_ISCSI_SUPPORT_H_

#include <AmiDxeLib.h>
#include "IScsiDxe/IScsiImpl.h"

#include <Include/RsdNetFnCommandDefinitions.h>
#include <Library/RsdIpmiExtCommandLib.h>

#define	RSD_MAX_STRING_LENGTH			0x71

// 
// Even though RSD allows the STRING size to be a maximum of 113 bytes, 
// the CHAP and Mutual CHAP secrets should be [(12 to 16) + 1] bytes of size.
//
#define	RSD_SECRET_MAX_LENGTH			0x11

//
// ISCSI Region boot blocks.
//
typedef enum {
	VersionBlock = 1,
	InitiatorBlock,
	TargetBlock,
	IScsiAttemptBlock
}RSD_ISCSI_PARAMETER_ID;

//
// ISCSI CHAP Names and Secrets
//
typedef enum {
	ChapName = 1,
	ChapSecret,
	MutualChapName,
	MutualChapSecret
}RSD_ISCSI_CHAP;


#pragma pack(1)


#if 0	// Use if needed.
typedef struct {
	UINT8	ParameterId;
	UINT16	Length;
	UINT16	Handle;
}RSD_ISCSI_COMMON_PARAMS;
#endif

//
// ISCSI Version Structure.
//
typedef struct {
//	RSD_ISCSI_COMMON_PARAMS	CommonParams;
	UINT8	ParameterId;	// 0x00
	UINT16	Length;			// 0x01
	UINT16	Handle;			// 0x03
	UINT8	MajorVersion;	// 0x05
	UINT8	MinorVersion;	// 0x06
}RSD_ISCSI_VERSION;

//
// ISCSI Initiator Structure.
//
typedef struct {
	//RSD_ISCSI_COMMON_PARAMS	CommonParams;
	UINT8	ParameterId;							// 0x00
	UINT16	Length;									// 0x01
	UINT16	Handle;									// 0x03
	UINT8	InitiatorDhcpEnabled;					// 0x05
	CHAR8	InitiatorName[RSD_MAX_STRING_LENGTH];	// 0x06
	UINT8	IpAddressType;							// 0x07
	UINT8	InitiatorIpAddress[16];					// 0x08
	UINT8	InitiatorSubnetMask[16];				// 0x18
	UINT8	InitiatorGateway[16];					// 0x28
	UINT8	PrimaryDns[16];							// 0x38
	UINT8	SecondaryDns[16];						// 0x48
	UINT8	NicMacAddress[6];						// 0x58
	// STRINGS at the end of the structure.
}RSD_ISCSI_INITIATOR;

//
// ISCSI Target Structure.
//
typedef struct {
//	RSD_ISCSI_COMMON_PARAMS	CommonParams;
	UINT8	ParameterId;								// 0x00
	UINT16	Length;										// 0x01
	UINT16	Handle;										// 0x03
	UINT8	TargetDhcpEnabled;							// 0x05
	CHAR8	TargetName[RSD_MAX_STRING_LENGTH];			// 0x06
	UINT8	IpAddressType;								// 0x07
	UINT8	TargetIpAddress[16];						// 0x08
	UINT16	TargetPort;									// 0X18
	UINT16	TargetLun;									// 0x1A
	UINT8	VlanEnable;									// 0x1C
	UINT16	VlanId;										// 0x1D
	UINT8	RouterAdvertisement;						// 0x1F
	UINT8	AuthenticationMethod;						// 0x20
	CHAR8	ChapName[RSD_MAX_STRING_LENGTH];		    // 0x21
//	CHAR8	ChapSecret[RSD_MAX_STRING_LENGTH];			// 0x22
	CHAR8	ChapSecret[RSD_SECRET_MAX_LENGTH];			// 0x22
	CHAR8	MutualChapName[RSD_MAX_STRING_LENGTH];		// 0x23
//	CHAR8	MutualChapSecret[RSD_MAX_STRING_LENGTH];	// 0x24 
	CHAR8	MutualChapSecret[RSD_SECRET_MAX_LENGTH];	// 0x24 
	// STRINGS at the end of the structure.
}RSD_ISCSI_TARGET;

//
// ISCSI Attempt Structure.
//
typedef struct {
//	RSD_ISCSI_COMMON_PARAMS	CommonParams;
	UINT8	ParameterId;			// 0x00
	UINT16	Length;					// 0x01
	UINT16	Handle;					// 0x03
	UINT16	InitiatorHandle;		// 0x05
	UINT16	TargetHandle;			// 0x07
	UINT16	ConnectionWaitTime;		// 0x09
	UINT8	ConnectionRetryCount;	// 0x0B
}RSD_ISCSI_ATTEMPT;

#pragma pack()

//
// Linked list to store multiple units of Initiator, Target and iSCSI Attempt data.
// This will not be used if there's only one unit of iSCSI Boot data.
//
typedef struct {
	UINT16	Handle;
	UINT8	Type;
	VOID*	Data;
	struct RSD_ISCSI_BLOCK_DATA    *NextHandle;
}RSD_ISCSI_BLOCK_DATA;

//
// MACRO definitions for RSD v2.2 support
//
#define	RSD_BLOCK_OFFSET_BEGIN			0x00
#define RSD_VALID_DATA_REGION			0x01
#define RSD_STAT_UNLOCKED				0x00
#define RSD_ISCS_BOOT_OPTIONS_REGION	0x04
#define RSD_INTEL_SPEC					0x04
#define RSD_NIC_SIZE					0x06
#define	RSD_ISCSI_MAX_ATTEMPTS			0x10
#define	RSD_MAX_IP_BYTES				0x10

#define RSD_MAX_ALLOWED_READ_LENGTH		0x100

//
// Ip Mode Type
//
#define RSD_IP_MODE_AUTOCONFIG          0
#define RSD_IP_MODE_IP4                 1
#define RSD_IP_MODE_IP6                 2

// ISCSI CHAP TYPES
#define	RSD_ISCSI_NO_CHAP				0x00
#define	RSD_ISCSI_UNI_CHAP				0x01
#define	RSD_ISCSI_MUTUAL_CHAP			0x02

#define	RSD_ISCSI_VERISON_MAX_SIZE		sizeof(RSD_ISCSI_VERSION)
#define RSD_ISCSI_INIT_MAX_SIZE			sizeof(RSD_ISCSI_INITIATOR)
#define RSD_ISCSI_TARGET_MAX_SIZE		sizeof(RSD_ISCSI_TARGET)
#define RSD_ISCSI_BOOT_MAX_SIZE			sizeof(RSD_ISCSI_ATTEMPT)


//
// Global variables declaration.
//
EFI_IPMI_TRANSPORT		*gIpmiTransport;
//IPMI_USB_TRANSPORT		*gIpmiTransport;
UINT16					gRsdIScsiCurOffset, gRsdAttemptHandles[RSD_ISCSI_MAX_ATTEMPTS];
RSD_MDR_READ_REQ		gRsdIScsiReadReq;
RSD_MDR_READ_RES		*gRsdIScsiReadResp;
RSD_ISCSI_BLOCK_DATA    *gRsdIScsiRootHandle;
UINT8			        gRsdIScsiAttemptCount;
BOOLEAN                 gRsdIScsiDataAvailable; 
BOOLEAN                 gRsdIScsiInitDone;
CHAR8                   gRsdIScsiInitiatorName[ISCSI_NAME_MAX_SIZE];

//
// MACRO to find the Length of the string from the current offset in the response.
//
#define	RSD_ISCSI_STR_LEN(Resp, Length)\
	for(;Resp[Length] != 0x00; ++Length);

#define	IS_EXPECTED_DATA(Resp, ParamId) (ParamId == Resp[0])

#define	IS_VALID_SIZE(Resp) (0 == Resp[1])

#define	INCREMENT_BY_SIZE(Param1, Param2, Size) (Param1 += Size, Param2 += Size)

#define IS_VALID_PARAM_ID(Id) (Id >= 1 && Id <= 4)

#define COPY_ATTEMPT_HANDLE(Handle)\
	UINT8 Num = 0;\
	for(;gRsdAttemptHandles[Num] != 0; Num++);\
	gRsdAttemptHandles[Num] = Handle;\

//
// Function declaration Part
//
/**
  Get the Data based on the Handle

  @param[in]  Handle     The Handle of Initiator or Target or iSCSI Attempt data.

  @return       Pointer to the RSD_ISCSI_BLOCK_DATA data.
  @retval NULL  The Handle cannot be found.

**/
RSD_ISCSI_BLOCK_DATA*
EFIAPI
RsdIScsiGetDataByHandle (
	UINT16		Handle
);

/**
  Parses and stores the BMC Initiator data into a linked list. Also checks if the next block
  of data is also of initiator data or not. If yes, the previous block of initiator data 
  will be added in a linked list and the iSCSI MDR Read request will be issued again.

  @param[in]  MdrResp			If known the length to read from BMC with this READ request.
  @param[in]  BmcInitRespSize	If known the Offset of the data from where we need to read.                           

  @retval EFI_SUCCESS				Parsed and stored the initiator data successfully.
  @retval EFI_NO_MAPPING			MdrResp doesn't contain Initiator data.
  @retval EFI_INVALID_PARAMETER		Bmc Initiator response data size is zero.
  @retval EFI_OUT_OF_RESOURCES		Adding the data to linked list failed due to some reason.

**/
EFI_STATUS
EFIAPI
RsdIScsiParseInitiatorResponse (
	RSD_MDR_READ_RES	*MdrInitResp,
	UINT8				BmcInitRespSize
);

/**
  Parses and stores the BMC Target data into a linked list. Also checks if the next block
  of data is also of initiator data or not. If yes, the previous block of initiator data 
  will be added in a linked list and the iSCSI MDR Read request will be issued again.

  @param[in]  MdrResp			If known the length to read from BMC with this READ request.
  @param[in]  BmcInitRespSize	If known the Offset of the data from where we need to read.                           

  @retval EFI_SUCCESS				Parsed and stored the initiator data successfully.
  @retval EFI_NO_MAPPING			MdrResp doesn't contain Initiator data.
  @retval EFI_INVALID_PARAMETER		Bmc Initiator response data size is zero.
  @retval EFI_OUT_OF_RESOURCES		Adding the data to linked list failed due to some reason.

**/
EFI_STATUS
EFIAPI
RsdIScsiParseTargetResponse (
	RSD_MDR_READ_RES	*MdrTarResp,
	UINT8				BmcTarRespSize
);

/**
  Parses and stores the BMC iSCSI Attempt data into a linked list. Also checks if the next block
  of data is also of iSCSI Attempt data or not. If yes, the previous block of iSCSI Attempt data 
  will be added in a linked list and the iSCSI MDR Read request will be issued again.

  @param[in]  MdrAttemptResp		If known the length to read from BMC with this READ request.
  @param[in]  BmcAttemptRespSize	If known the Offset of the data from where we need to read.                           

  @retval EFI_SUCCESS				Parsed and stored the iSCSI Attempt data successfully.
  @retval EFI_NO_MAPPING			MdrResp doesn't contain iSCSI Attempt data.
  @retval EFI_INVALID_PARAMETER		BMC iSCSI Attempt response data size is zero.
  @retval EFI_OUT_OF_RESOURCES		Adding the data to linked list failed due to some reason.

**/
EFI_STATUS
EFIAPI
RsdIScsiParseIScsiAttemptData (
	RSD_MDR_READ_RES	*MdrAttemptResp,
	UINT8				BmcAttemptRespSize
);


/**
  Deletes either all the data in the linked list or the data related to the Handle passed.

  @param[in]  DelAllHandles     If TRUE, the list will be emptied.
  @param[in]  Handle            The Handle to be removed from the list.

  @retval EFI_SUCCESS               The operation has been completed successfully.
  @retval EFI_INVALID_PARAMETER     If the handle is NULL and DelAllHandles is FALSE.

**/
EFI_STATUS
EFIAPI
RsdIScsiDeleteByHandle (
    BOOLEAN     DelAllHandles,
    UINT16      Handle
);


/**
  Reads the Initiator Structure from the iSCSI Boot options region.

  @param[in]  LengthToRead      If known, the length to read from BMC with this READ request.
                                Otherwise reads the maximum possible length.
  @param[in]  OffsetToRead      If known, the Offset of the data from where we need to read.
                                Otherwise reads from the offset in gRsdIScsiCurOffset.

  @retval EFI_INVALID_PARAMETER Any input or configured parameter is invalid.
  @retval EFI_SUCCESS           The operation is completed successfully.

**/
EFI_STATUS
EFIAPI
RsdIScsiReadInitiatorBlock (
    UINT8   LengthToRead,
    UINT16  OffsetToRead
);

/**
  Reads the Target Structure from the iSCSI Boot options region.

  @param[in]  LengthToRead      If known, the length to read from BMC with this READ request.
                                Otherwise reads the maximum possible length.
  @param[in]  OffsetToRead      If known, the Offset of the data from where we need to read.
                                Otherwise reads from the offset in gRsdIScsiCurOffset.

  @retval EFI_INVALID_PARAMETER Any input or configured parameter is invalid.
  @retval EFI_SUCCESS           The operation is completed successfully.

**/
EFI_STATUS
EFIAPI
RsdIScsiReadTargetBlock (
    UINT8   LengthToRead,
    UINT16  OffsetToRead
);


/**
  Reads the iSCSI Attempt Data from the iSCSI Boot options region.

  @param[in]  LengthToRead      If known, the length to read from BMC with this READ request.
                                Otherwise reads the maximum possible length.
  @param[in]  OffsetToRead      If known, the Offset of the data from where we need to read.
                                Otherwise reads from the offset in gRsdIScsiCurOffset.

  @retval EFI_INVALID_PARAMETER Any input or configured parameter is invalid.
  @retval EFI_SUCCESS           The operation is completed successfully.

**/
EFI_STATUS
EFIAPI
RsdIScsiReadIScsiAttemptBlock (
    UINT8   LengthToRead,
    UINT16  OffsetToRead
);

/**
  This function determines whether there is any attempt data available in BMC.

  @param[in]  VOID

  @param[out]  BOOLEAN      Value of gRsdIScsiDataAvailable.

**/
BOOLEAN
EFIAPI
RsdIScsiIsBmcDataAvailable();

/**
  Function to update the BMC cached attempt data for intermediate data save
  
  @param[in]  PortString                PortString for Attempt.
  @param[in]  RsaAttemptConfigData      Attempt data to update.
  
  @param[out] EFI_SUCCESS               If update went success.
  @param[out] EFI_INVALID_PARAMETER     If RsaAttemptConfigData is NULL.
  @param[out] EFI_NOT_READY             If BMC data is not initialized.

**/
EFI_STATUS
EFIAPI
RSAUpdateBMCIScsiAttemptDataCache (
  IN CHAR16 *PortString,
  IN OUT ISCSI_ATTEMPT_CONFIG_NVDATA *RsaAttemptConfigData
  );

/**
  This is the parent function that is responsible for 
  i) Initializing the common parameters,
  ii) Locates EFI_IPMI_TRANSPORT protocol,
  iii) Issuing MDR read requests and
  iv) Framing the ISCSI_ATTEMPT_CONFIG_NVDATA from the BMC data.

  @param[in]  VOID

  @param[out] EFI_SUCCESS               If data is read from BMC successfully and 
                                        gRsdIScsiAttemptConfigData is initialized.
  @param[out] EFI_NOT_FOUND             The data needed is not present in the list or
                                        EFI_IPMI_TRANSPORT is not found.
  @param[out] EFI_INAVLID_PARAMETER     BMC command execution failed.
**/
EFI_STATUS
EFIAPI
RsdIScsiInitializeBmcInformation();

extern EFI_GUID   gEfiRsdIScsiAttemptConfigGuid;

#endif //_RSD_ISCSI_SUPPORT_H_

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
