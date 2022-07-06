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

/** @file NvmeController.h
    Nvme Controller related definition

**/


#ifndef _NVME_CONTROLLER_H_
#define _NVME_CONTROLLER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <Protocol/StorageSecurityCommand.h>

#pragma pack(1)

#define NVME_BAR0_INDEX 0x00

#define NVME_SPEC_VS_1                0x10000    // Spec version value for 1.0 compliant controllers 
#define NVME_SPEC_VS_1_1              0x10100    // Spec version value for 1.1 compliant controllers 
#define NVME_SPEC_VS_1_2              0x10200    // Spec version value for 1.2 compliant controllers 
#define NVME_SPEC_VS_1_2_1            0x10201    // Spec version value for 1.2.1 compliant controllers 
#define NVME_SPEC_VS_1_3              0x10300    // Spec version value for 1.3 compliant controllers 

// Controller register map for 1.1 compliant controllers 
#define Offset_Cap      0x0
#define Offset_Version  0x8
#define Offset_Intms    0xC
#define Offset_Intmc    0x10
#define Offset_CC       0x14
#define Offset_CSTS     0x1C
#define CSTS_RDY        0x1
#define CSTS_CFS        0x2
#define Offset_Nssrs    0x20
#define Offset_Aqa      0x24
#define Offset_Asq      0x28
#define Offset_Acq      0x30


// Additional register map for 1.2 compliant controllers
#define Offset_CMBLOC   0x38
#define Offset_CMBSZ    0x3C
#define Offset_Admin_Submission_Queue_Tail_DoorBell 0x1000

// Admin Cmd Set Figure 38 NVM Express 1.2
#define DELETE_IO_SUBMISSION_QUEUE  0x00
#define CREATE_IO_SUBMISSION_QUEUE  0x01
#define GET_LOG_PAGE                0x02
#define DELETE_IO_COMPLETION_QUEUE  0x04
#define CREATE_IO_COMPLETION_QUEUE  0x05
#define IDENTIFY                    0x06
#define ABORT                       0x08 
#define SET_FEATURES                0x09
#define GET_FEATURES                0x0A
#define ASYNC_EVENT_REQUEST         0x0C
#define FIRMWARE_ACTIVATE           0x10
#define FIRMWARE_IMAGE_DOWNLOAD     0x11
#define FORMAT_NVM                  0x80
#define SECURITY_SEND               0x81
#define SECURITY_RECEIVE            0x82

// Additional Admin Cmd Set supported for 1.2 compliant controllers
#define NAMESPACE_ATTACHMENT        0x15
#define NAMESPACE_MANAGEMENT        0x0D                                  
                                     

// Additional Admin Cmd Set supported for 1.2.1 compliant controllers
#define KEEP_ALIVE                  0x18

// Additional Admin Cmd Set supported for 1.3 compliant controllers
#define NVMEMI_SEND                 0x1D
#define NVMEMI_RECEIVE              0x1E
#define DEVICE_SELF_TEST            0x14
#define DIRECTIVE_SEND              0x19
#define DIRECTIVE_RECEIVE           0x1A
#define VIRTUALIZATION_MANAGEMENT   0x1C
#define DOORBELL_BUFFER_CONFIG      0x7C
#define SANITIZE_CMD                0x84

//Figure 149: Opcodes for NVM Commands NVM Express 1.2
#define NVME_FLUSH                  0x00
#define NVME_WRITE                  0x01
#define NVME_READ                   0x02
#define NVME_WRITE_UNCORRECTABLE    0x04
#define NVME_COMPARE                0x05
#define NVME_WRITE_ZEROES           0x08
#define NVME_DATASET_MANAGEMENT     0x09
#define NVME_RESERVATION_REGISTER   0x0D
#define NVME_RESERVATION_REPORT     0x0E
#define NVME_RESERVATION_ACQUIRE    0x11
#define NVME_RESERVATION_RELEASE    0x15

#define  PRP2LIST_UNSUPPORTED_ADMIN_CMDS_1_1 GET_FEATURES,\
                                             GET_LOG_PAGE, \
                                             SECURITY_SEND,\
                                             SECURITY_RECEIVE

#define  PRP2LIST_UNSUPPORTED_NVM_CMDS_1_1   NVME_DATASET_MANAGEMENT,\
                                             NVME_RESERVATION_ACQUIRE,\
                                             NVME_RESERVATION_REGISTER,\
                                             NVME_RESERVATION_RELEASE                                                                                                                                              

#define PRP2LIST_UNSUPPORTED_ADMIN_CMDS IDENTIFY,\
                                        SET_FEATURES,\
                                        NAMESPACE_ATTACHMENT,\
                                        NAMESPACE_MANAGEMENT

// CDW10 for NameSpace Management & Attachment as per NVM Express 1.2.1
#define CREATE_NAMESPACE            0x0
#define DELETE_NAMESPACE            0x01
#define CONTROLLER_ATTACH           0x0
#define CONTROLLER_DETACH           0x01

// Security Protocol Identifiers
#define NVME_SECURITY_PROTOCOL_INFORMATION           0x00
#define NVME_SECURITY_PROTOCOL_1                     0x01
#define NVME_SECURITY_PROTOCOL_2                     0x02
#define NVME_SECUIRTY_SECP                           0xEF

//#define NVME_SECURITY_LOCK_MASK                      0x400
//#define NVME_SECURITY_FREEZE_MASK                    0x800
#define NVME_SECURITY_LOCK_MASK                      0x004

#define NVME_SECURITY_RECEIVE_BUFFER_SIZE            0x10
#define NVME_SECURITY_SEND_BUFFER_SIZE               0x24

// Macros used to get Level 0 Discovery data
#define NVME_COMID_LEVEL0_DISCOVERY                  0x0001
#define NVME_Locking_Feature                         0x0002
#define NVME_Block_SID_Authentification_Feature      0x0402

// Security Protocol Specific Data for Freeze Lock command
#define NVME_BLOCK_SID_COMID                         0x05
#define NVME_SECURITY_FREEZE_LOCK                    0x05
// Sanitize Action
#define EXIT_FAILURE_MODE                            0x01
#define BLOCK_ERASE_SANITIZE                         0x02
#define OVERWRITE_SANITIZE                           0x03
#define CRYPTO_ERASE_SANITIZE                        0x04

// UINT16   SecurityStatus
//          Bit 0 : Security Supported
//          Bit 1 : Security Enabled
//          Bit 2 : Security Locked
//          Bit 3 : Security Frozen
typedef struct { 
    UINT16    SAT3DeviceStatus;
    UINT16    TcgDeviceStatus;
}NVME_DEVICE_SECURITY_STATUS;

typedef struct { 
    UINT8      Reserved[6]; 
    UINT8      ListLengthHigh; 
    UINT8      ListLengthLow; 
    UINT8      SupportedSPList[502]; 
    UINT8      PadBytesHigh; 
    UINT8      PadBytesLow; 
}NVME_SP0_TRUSTED_RECEIVE_PARAMETER_DATA;

typedef struct {
    UINT16      FeatureCode;
    UINT8       Version;
    UINT8       Length;
    UINT8       Reserved[8];
    UINT32      LogicalBlockSize;
    UINT64      AlignmentGranularity;
    UINT64      LowestAlignedLBA;
}NVME_FEATURE_DESCRIPTOR_GEOMETRY;

typedef struct {
    UINT16      FeatureCode;
    UINT8       Version;
    UINT8       Length;
    UINT8       SIDStateValue  :  1;  // Bit 0
    UINT8       SIDBlockedState:  1;  // Bit 1
    UINT8       Reserved       :  6;
    UINT8       HWReset        :  1;  // Bit 0
    UINT8       Reserved1      :  7; 
}NVME_FEATURE_DESCRIPTOR_BLOCK_SID;

typedef struct {
    UINT16      FeatureCode;
    UINT8       Version;
    UINT8       Length;
    UINT16      BaseComID;
    UINT16      NumberofComIDs;
    UINT8       Byte8;
    UINT16      SPAdminAuth;
    UINT16      SPUserAuth;
    UINT8       CPinSid;
    UINT8       CPinSidRevert;
    UINT8       Reserved[5];
}NVME_FEATURE_DESCRIPTOR_OPAL_SSC_V200;

typedef struct {
    EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *StorageSecurityProtocol;
    UINT8                                   SupportedProtocols[16];
    UINT8                                   TPerFeature;
    UINT8                                   LockingFeature;
    NVME_FEATURE_DESCRIPTOR_GEOMETRY        GeometryFeature;
    NVME_FEATURE_DESCRIPTOR_OPAL_SSC_V200   OpalFeature;
    NVME_FEATURE_DESCRIPTOR_BLOCK_SID       BlockSid;
    UINT16                                  ComIDExt;
    UINT8                                   HostSessionID;
    UINT32                                  CPinMsidLength;
    UINT8                                   *pCPinMsid;
    UINT64                                  MBRSize;
    UINT32                                  MaxComPacketSize;
    UINT32                                  MediaId;
}NVME_OPAL_DEVICE;

typedef struct { 
    UINT32     Length;
    UINT16     MajorVersion;
    UINT16     MinorVersion;
    UINT8      Reserved[8];
    UINT8      VendorUnique[32];
}NVME_LEVEL0_DISCOVERY_HEADER;

typedef struct {
    UINT16      FeatureCode;
    UINT8       Version;
    UINT8       Length;
    UINT8       Data[1];
}NVME_FEATURE_DESCRIPTOR;

typedef struct { 
    NVME_LEVEL0_DISCOVERY_HEADER     Level0DiscoveryHeader;
    NVME_FEATURE_DESCRIPTOR          FeatureDescriptor;
}NVME_LEVEL0_DISCOVERY_DATA;

typedef struct {
    UINT64      ControllerCapabilities;
    UINT32      Version;
    UINT32      InterruptMaskSet;
    UINT32      InterruptMaskClear;
    UINT32      ControllerConfiguration;
    UINT32      Reserved1;
    UINT32      ControllerStatus;
    UINT32      NVMSubSystemReset;
    UINT32      AdminQueueAttributes;
    UINT64      AdminSubmissionQueueBaseAddress;
    UINT64      AdminCompletionQueueBaseAddress;
    // Variable offsets follow
} NVME_CONTROLLER_REGISTER;

//Figure 25  NVM Express 1.1 Spec
typedef struct {
    UINT32      DW0;
    UINT32      DW1;
    UINT16      SQHeadPointer;
    UINT16      SQIdentifier;
    UINT16      CommandIdentifier;
    UINT16      PhaseTag :  1;
    UINT16      StatusCode :  8;
    UINT16      StatusCodeType : 3;
    UINT16      Reservered1 : 2;
    UINT16      More : 1;
    UINT16      DoNotRetry  : 1;
} COMPLETION_QUEUE_ENTRY;



// Figure 93 NVM Express 1.2 Spec
typedef struct {
    
    UINT16      MS;                     // Bits 15:0
    UINT8       LBADS;                  // Bits 23:16
    UINT8       RP : 2;                 // Bits 25:24
    UINT8       Reserved1 :6;           // Bits 31:26
    
} LBA_FORMAT_DATA;


// Figure 92 NVM Express 1.2 Spec
typedef struct {
    
    UINT64             NSIZE;                  // Offset 7:0
    UINT64             NCAP;                   // Offset 15:8
    UINT64             NUSE;                   // Offset 23:16
    UINT8              NSFEAT;                 // Offset 24
    UINT8              NLBAF;                  // Offset 25
    UINT8              FLBAS;                  // Offset 26
    UINT8              MC;                     // Offset 27
    UINT8              DPC;                    // Offset 28
    UINT8              DPS;                    // Offset 29
    UINT8              NMIC;                   // Offset 30
    UINT8              RESCAP;                 // Offset 31
    UINT8              FPI;                    // Offset 32
    UINT8              Reserved1;              // Offset 33
    UINT16             NAWUN;                  // Offset 35:34
    UINT16             NAWUPF;                 // Offset 37:36
    UINT16             NACWU;                  // Offset 39:38
    UINT16             NABSN;                  // Offset 41:40
    UINT16             NABO;                   // Offset 43:42
    UINT16             NABSPF;                 // Offset 45:44
    UINT8              Reserved2[2];           // Offset 46:47
    UINT8              NVMCAP[16];             // Offset 63:48
    UINT8              Reserved3[40];          // Offset 103:64
    UINT8              NGUID[16];              // Offset 119:104
    UINT64             EUI64;                  // Offset 127:120
    LBA_FORMAT_DATA    LBAF[16];               // Offset 191:128
    UINT8              Reserved4[192];         // Offset 383:192
    UINT8              VS[3712];               // Offset 4095:384
    
} IDENTIFY_NAMESPACE_DATA;

typedef struct {
    UINT16         NumberOfIdentifiers;
    UINT16         IdentifierList[2047];
} NVME_CONTROLLER_LIST;

#define QUEUE_DOORBELL_OFFSET(QUEUE_NUM, TAIL_HEAD, DoorBellStride) \
        (0x1000 + (((QUEUE_NUM * 2)  + TAIL_HEAD) *  (4 << DoorBellStride)))

//MMIO Access

#define NvmeMmAddress( BaseAddr, Register ) \
        ((UINT64)(BaseAddr) + \
        (UINTN)(Register) \
         )
#define NvmeMm32Ptr( BaseAddr, Register ) \
        ((volatile UINT32 *)NvmeMmAddress (BaseAddr, Register ))

#define NvmeMm16Ptr( BaseAddr, Register ) \
        ((volatile UINT16 *)NvmeMmAddress (BaseAddr, Register ))

#define NvmeMm8Ptr( BaseAddr, Register ) \
        ((volatile UINT8 *)NvmeMmAddress (BaseAddr, Register ))

//Controller Generic Registers

#define     CONTROLLER_REG32( pNvmeController, Register ) \
            (ReadDataDword ((pNvmeController), (Register)))

#define     CONTROLLER_WRITE_REG32( pNvmeController, Register, Data ) \
            (WriteDataDword( pNvmeController, Register, Data ))

#define     CONTROLLER_REG16( pNvmeController, Register ) \
            (ReadDataWord( pNvmeController, Register ))

#define     CONTROLLER_WRITE_REG16( pNvmeController, Register, Data ) \
            (WriteDataWord( pNvmeController, Register, Data ))

#define     CONTROLLER_REG8( pNvmeController, Register ) \
            (ReadDataByte ((pNvmeController), (Register)))

#define     CONTROLLER_WRITE_REG8( pNvmeController, Register, Data ) \
            (WriteDataByte( pNvmeController, Register, Data ))

#define     CONTROLLER_REG8_OR( pNvmeController, Register, OrData) \
            CONTROLLER_WRITE_REG8(pNvmeController, Register, ((CONTROLLER_REG8 ((pNvmeController), (Register))) | ((UINT8) (OrData))))

#define     CONTROLLER_REG16_OR( pNvmeController, Register, OrData) \
            CONTROLLER_WRITE_REG16(pNvmeController, Register, ((CONTROLLER_REG16 ((pNvmeController), (Register))) | ((UINT16) (OrData))))

#define     CONTROLLER_REG32_OR( pNvmeController, Register, OrData) \
            CONTROLLER_WRITE_REG32(pNvmeController, Register, ((CONTROLLER_REG32 ((pNvmeController), (Register))) | ((UINT32) (OrData))))

#define     CONTROLLER_REG8_AND( pNvmeController, Register, AndData) \
            CONTROLLER_WRITE_REG8(pNvmeController, Register, ((CONTROLLER_REG8 ((pNvmeController), (Register))) & ((UINT8) (AndData))))

#define     CONTROLLER_REG16_AND( pNvmeController, Register, AndData) \
            CONTROLLER_WRITE_REG16(pNvmeController, Register, ((CONTROLLER_REG16 ((pNvmeController), (Register))) & ((UINT16) (AndData))))
 
#define     CONTROLLER_REG32_AND( pNvmeController, Register, AndData) \
            CONTROLLER_WRITE_REG32(pNvmeController, Register, ((CONTROLLER_REG32 ((pNvmeController), (Register))) & ((UINT32) (AndData))))

#define     CONTROLLER_REG8_AND_OR( pNvmeController, Register, AndData, OrData) \
            CONTROLLER_WRITE_REG8(pNvmeController, Register, ((CONTROLLER_REG8 ((pNvmeController), (Register))) & ((UINT8) (AndData)) | ((UINT8) (OrData))))

#define     CONTROLLER_REG16_AND_OR( pNvmeController, Register, AndData, OrData) \
            CONTROLLER_WRITE_REG16(pNvmeController, Register, ((CONTROLLER_REG16 ((pNvmeController), (Register))) & ((UINT16) (AndData)) | ((UINT16) (OrData))))

#define     CONTROLLER_REG32_AND_OR( pNvmeController, Register,AndData,  OrData) \
            CONTROLLER_WRITE_REG32(pNvmeController, Register, ((CONTROLLER_REG32 ((pNvmeController), (Register))) & ((UINT32) (AndData)) | ((UINT32) (OrData))))

#pragma pack()

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
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
