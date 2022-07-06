//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file AmiTcgStorageSecurity.h
    @brief AMI TCG Storage Security Features Header File.

  The file contains definitions of the AMI Specific TCG Storage 
  Security related structures and constants.
**/

#ifndef __AMI_TCG_STORAGE_SECURITY__H__
#define __AMI_TCG_STORAGE_SECURITY__H__

#include <Protocol/StorageSecurityCommand.h>
#include <Include/AmiStorageFeatures.h>
//
// Storage Security Protocol
//
#define SECURITY_PROTOCOL_INFORMATION                   0x00
#define SECURITY_PROTOCOL_1                             0x01
#define SECURITY_PROTOCOL_2                             0x02
#define SECURITY_PROTOCOL_SDCARD_TRUSTED_FLASH          0xED
#define SECURITY_PROTOCOL_IEEE1667                      0xEE

#pragma pack(1)

typedef struct { 
    UINT8      Reserved[6];
    UINT16     ListLength;
    UINT8      SupportedSPList[1];
}SUPPORTED_SECURITY_PROTOCOLS;

typedef struct { 
    UINT8      Reserved[6]; 
    UINT8      ListLengthHigh; 
    UINT8      ListLengthLow; 
    UINT8      SupportedSPList[502]; 
    UINT8      PadBytesHigh; 
    UINT8      PadBytesLow; 
}SP0_TRUSTED_RECEIVE_PARAMETER_DATA;

typedef struct { 
    UINT32     Length;
    UINT16     MajorVersion;
    UINT16     MinorVersion;
    UINT8      Reserved[8];
    UINT8      VendorUnique[32];
}LEVEL0_DISCOVERY_HEADER;

typedef struct {
    UINT16      FeatureCode;
    UINT8       Version;
    UINT8       Length;
    UINT8       Data[1];
}FEATURE_DESCRIPTOR;

typedef struct {
	UINT16		FeatureCode;
	UINT8		Version;
	UINT8		Length;
	UINT8		Reserved[8];
	UINT32		LogicalBlockSize;
	UINT64		AlignmentGranularity;
	UINT64		LowestAlignedLBA;
}FEATURE_DESCRIPTOR_GEOMETRY;

typedef struct {
	UINT16		FeatureCode;
	UINT8		Version;
	UINT8		Length;
	UINT16		BaseComID;
	UINT16		NumberofComIDs;
	UINT8		Byte8;
	UINT16		SPAdminAuth;
	UINT16		SPUserAuth;
	UINT8		CPinSid;
	UINT8		CPinSidRevert;
	UINT8		Reserved[5];
}FEATURE_DESCRIPTOR_OPAL_SSC_V200;

typedef struct {
	UINT16		FeatureCode;
	UINT8		Version;
	UINT8		Length;
	UINT16		BaseComID;
	UINT16		NumberofComIDs;
	UINT8		Reserved[5];
	UINT8		CPinSid;
	UINT8		CPinSidRevert;
	UINT8		Reserved1[5];
}FEATURE_DESCRIPTOR_OPALITE_SSC;

typedef struct {
    UINT16		FeatureCode;
    UINT8		Version;
    UINT8		Length;
    UINT16      BaseComID;
    UINT16      NumberofComIDs;
    UINT8       RangeCrossing:1;
}FEATURE_DESCRIPTOR_ENTERPRISE_SSC;

typedef struct {
    UINT16      FeatureCode;
    UINT8       Version;
    UINT8       Length;
    UINT8       SIDStateValue  :  1;  // Bit 0
    UINT8       SIDBlockedState:  1;  // Bit 1
    UINT8       Reserved       :  6;
    UINT8       HWReset        :  1;  // Bit 0
    UINT8       Reserved1      :  7; 
}FEATURE_DESCRIPTOR_BLOCK_SID;


typedef enum {
   UserEnabledStatusUnkown = 0,
   UserEnabled,
   UserNotEnabled
}USER_PWD_STATUS;

typedef struct {
    EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *StorageSecurityProtocol;
    UINT8                                   SupportedProtocols[16];
    UINT8                                   TPerFeature;
    UINT8                                   LockingFeature;
    USER_PWD_STATUS                         UserEnabledStatus;
    FEATURE_DESCRIPTOR_GEOMETRY             GeometryFeature;
    FEATURE_DESCRIPTOR_OPAL_SSC_V200        OpalFeature;
    FEATURE_DESCRIPTOR_BLOCK_SID	        BlockSid;
    UINT16                                  ComIDExt;
    UINT8                                   HostSessionID;
    UINT32                                  CPinMsidLength;
    UINT8                                   *pCPinMsid;
    UINT64                                  MBRSize;
    UINT32                                  MaxComPacketSize;
    UINT32			                        MediaId;
    AMI_STORAGE_BUS_INTERFACE               BusInterfaceType;
    EFI_HANDLE                              DeviceHandle;
    UINT8                                   PortNumber;
    UINT8                                   PMPortNumber;
    USER_PWD_STATUS                         EnterpriseAdminPwdSts;
    BOOLEAN                                 RstControlled;              //Create for RST password  
}OPAL_DEVICE;

#pragma pack()

#endif
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
