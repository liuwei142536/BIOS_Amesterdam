//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**             5555 Oakbrook Pkwy, Norcross, GA 30093               **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
/** @file
  Header file for the DxeAmiNvmeLib. This header file defines TCG Opal/Pyrite spec defined Data

  Do not include this header in your source file. It is a private header of the DxeAmiNvmeLib.
**/

#ifndef _AMI_NVME_LIB_INTERNAL_H_
#define _AMI_NVME_LIB_INTERNAL_H_

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <IndustryStandard/Nvme.h>
#include <Protocol/NvmExpressPassthru.h>

#define  ToBigLittleEndianDword(EndianDword)    (((EndianDword & 0xFF) << 24) + ((EndianDword & 0xFF00) << 8) + \
        ((EndianDword & 0xFF0000) >>8) + ((EndianDword & 0xFF000000) >>24))

#define SECURITY_PROTOCOL_INFORMATION       0x00
#define SECURITY_PROTOCOL_1                 0x01
#define SECURITY_PROTOCOL_2                 0x02

#define LEVEL0_DISCOVERY_COMID              0x01
#define BLOCK_SID_COMID                     0x05
#define TPER_FEATURE                        0x0001
#define LOCKING_FEATURE                     0x0002
#define BLOCK_SID_AUTHENTIFICATION_FEATURE  0x0402

#define TCG_DEVICE_SECURITY_LOCK_MASK       BIT2
#define TCG_DEVICE_FREEZE_LOCK_MASK         BIT3

#pragma pack(1)

typedef struct {
    UINT8      Reserved[6];
    UINT8      ListLengthHigh;
    UINT8      ListLengthLow;
    UINT8      SupportedSPList[502];
    UINT8      PadBytesHigh;
    UINT8      PadBytesLow;
} TCG_OPAL_SP0_TRUSTED_RECEIVE_PARAMETER_DATA;

typedef struct {
    UINT32     Length;
    UINT16     MajorVersion;
    UINT16     MinorVersion;
    UINT8      Reserved[8];
    UINT8      VendorUnique[32];
} TCG_OPAL_LEVEL0_DISCOVERY_HEADER;

typedef struct {
    UINT16      FeatureCode;
    UINT8       Version;
    UINT8       Length;
    UINT8       Data[1];
} TCG_OPAL_FEATURE_DESCRIPTOR;

typedef struct {
    TCG_OPAL_LEVEL0_DISCOVERY_HEADER     Level0DiscoveryHeader;
    TCG_OPAL_FEATURE_DESCRIPTOR          FeatureDescriptor;
} TCG_OPAL_LEVEL0_DISCOVERY_DATA;

typedef struct {
    UINT16      FeatureCode;
    UINT8       Version;
    UINT8       Length;
    UINT8       SIDStateValue  :  1;  // Bit 0
    UINT8       SIDBlockedState:  1;  // Bit 1
    UINT8       Reserved       :  6;
    UINT8       HWReset        :  1;  // Bit 0
    UINT8       Reserved1      :  7;
}TCG_FEATURE_DESCRIPTOR_BLOCK_SID;

typedef struct {
    TCG_OPAL_FEATURE_DESCRIPTOR          LockingFeature;
    TCG_FEATURE_DESCRIPTOR_BLOCK_SID     BlockSid;
}TCG_OPAL_FEATURE_LIST;

#pragma pack()


#endif

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**             5555 Oakbrook Pkwy, Norcross, GA 30093               **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
