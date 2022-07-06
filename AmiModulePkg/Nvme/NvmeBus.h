//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file NvmeBus.h
    Header file for the Nvme Bus Driver

**/

#ifndef _NVME_BUS_H_
#define _NVME_BUS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "NvmeIncludes.h"
#include "NvmeController.h"
#include <Protocol/DiskInfo.h>
#include <Protocol/AmiNvmeController.h>
#include <Protocol/AmiNvmePassThru.h>
#include <Protocol/NvmExpressPassthru.h>

#define IDENTIFY_NAMESPACE_CNS_VALUE     0x00
#define IDENTIFY_CONTROLLER_CNS_VALUE    0x01
#define NAMESPACE_LIST_CNS_VALUE         0x02

#define MAX_MPS_ALLOWED             SIZE_64KB  // Maximum memory Page size allowed by NVMe driver
#define ADMIN_QUEUE_SIZE            0x100
#define QUEUE1_SIZE                 0x100

//Get Feature Identifier for Software Progress Marker Feature
#define PROGRESS_MARKER_FID                         0x80
#define ACTIVE_NAME_SPACE_SIG       SIGNATURE_32('N','V','M','E') 
//
// TCG Storage Security Protocol
//
#define SECURITY_PROTOCOL_INFORMATION                   0x00
#define SECURITY_PROTOCOL_2                             0x02

#define TCPA_PPIOP_ENABLE_BLOCK_SID_FUNC                                96
#define ACPI_FUNCTION_GET_USER_CONFIRMATION_STATUS_FOR_REQUEST          8

#define AMI_TCG_CONFIRMATION_FLAGS_GUID \
    {0x7d3dceee, 0xcbce, 0x4ea7, 0x87, 0x09, 0x6e, 0x55, 0x2f, 0x1e, 0xdb, 0xde}

typedef struct
{
    UINT32 RQST;                 //Store PPI request
    UINT32 RCNT;                 //Store most recent PPI request
    UINT32 ERROR;                //Store BIOS ERROR information after a PPI request
    UINT32 Flag;                 //Flag used by PPI SMM interface to determine ACPI PPI function request
    UINT32 AmiMisc;              //Misc storage not used
    UINT32 OptionalRqstData;     //Store optional data that OS might provide during a PPI request
    UINT32 RequestFuncResponse;  //Store Response of ACPI function request. Returned to caller
} AMI_ASL_PPI_NV_VAR;
typedef struct {
    EFI_DEVICE_PATH_PROTOCOL    Header;
    UINT32                      Nsid;
    UINT64                      EUI64;
} NVME_DEVICE_PATH;

typedef struct {
    UINT32                          Signature;
    EFI_HANDLE                      NvmeDeviceHandle;
    BOOLEAN                         Configured;                       // Updated after installing BLOCK_IO_PROTOCOL
    UINT32                          ActiveNameSpaceID;
    AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController;
    UINT64                          *PRP2List;                        // if the data transfer size is more than twice the max. page size 
    EFI_PHYSICAL_ADDRESS            PRP2ListMappedAddr;             
    VOID                            *PRP2ListUnMap;                   // if the data transfer size is more than twice the max. page size 
                                                                      // then PRP List is used.
    IDENTIFY_NAMESPACE_DATA         IdentifyNamespaceData;
    EFI_BLOCK_IO_PROTOCOL           NvmeBlockIO;
    EFI_DISK_INFO_PROTOCOL          NvmeDiskInfo;
    EFI_DEVICE_PATH_PROTOCOL        *EfiDevicePath;
    EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   NvmeSecurityStorageCmd;

    UINT8                           Int13DeviceAddress;            // Used only for legacy Support
    UINT16                          wEmulationOption;              // For INT13 support
    UINT8                           bHiddenSectors;
    UINT8                           NumHeads;
    UINT8                           LBANumHeads;
    UINT16                          NumCylinders;
    UINT16                          LBANumCyls;
    UINT8                           NumSectors;
    UINT8                           LBANumSectors;
    UINT64                          dMaxLBA;
    UINT16                          wBlockSize;
    UINT8                           bStorageType;
    UINT8                           bEmuType;
    UINT8                           PNM[27];
    UINT8                           NvmeManufactureId[NVME_MANUFACTUREID_LENGTH];
    
    LIST_ENTRY                      Link;                           // Links to parent NVM Controller
    
    UINT64                         ExtendedLBA;                    // Used if MetaData part of extended LBA
    UINT64                         ExtendedLBAMappedAddr;          // Mapped address 
    UINT64                         *ExtendedLBAUnMap;        
    UINT64                         ExtendedLBABufferSize;          // Buffer size for LBA data and MetaData if interleaved
    UINT64                         MappedExtendedLBABufferSize;    
    
    UINT64                         MetaDataBlock;            	  // Used if MetaData is separate buffer
    UINT64                         MetaDataBlockMappedAddr;   	  // Mapped address 
    UINT64                         *MetaDataBlockUnMap;
    UINT64                         MetaDataBufferSize;            // More local buffer better the performance
    UINT64                         MappedMetaDataBufferSize;      
    
} ACTIVE_NAMESPACE_DATA;

typedef struct {
    UINT64      Signature;
    UINT16      ControllerNumber;
    UINT16      NameSpaceId;
    BOOLEAN     LastNameSpace;
    UINT8       Data[1]; 		// Depends on NameSpaceId, either controller or namespace data will be copied
} NVME_CONTROLLER_DATA_TO_SMM;

#define ACTIVE_NAMESPACE_DATA_FROM_THIS(a) \
  CR(a, ACTIVE_NAMESPACE_DATA, NvmeBlockIO, ACTIVE_NAME_SPACE_SIG)

#define ACTIVE_NAMESPACE_DATA_FROM_THIS_FOR_DISKINFO(a) \
  CR(a, ACTIVE_NAMESPACE_DATA, NvmeDiskInfo, ACTIVE_NAME_SPACE_SIG)

#define ACTIVE_NAMESPACE_DATA_FROM_SECURITY_PROTOCOL(a) \
  CR(a, ACTIVE_NAMESPACE_DATA, NvmeSecurityStorageCmd, ACTIVE_NAME_SPACE_SIG)

#define NVME_CONTROLLER_FROM_SECURITY_PROTOCOL(a) \
  CR(a, AMI_NVME_CONTROLLER_PROTOCOL, StorageSecurityInterface, NVME_CONTROLLER_SIG)

EFI_STATUS 
EFIAPI
NvmeBusSupported (
    IN  EFI_DRIVER_BINDING_PROTOCOL *This,
    IN  EFI_HANDLE                  Controller,
    IN  EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
);

EFI_STATUS 
EFIAPI
NvmeBusStart (
    IN  EFI_DRIVER_BINDING_PROTOCOL *This,
    IN  EFI_HANDLE                  Controller,
    IN  EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
);

EFI_STATUS 
EFIAPI
NvmeBusStop (
    IN  EFI_DRIVER_BINDING_PROTOCOL *This,
    IN  EFI_HANDLE                  Controller,
    IN  UINTN                       NumberOfChildren,
    IN  EFI_HANDLE                  *ChildHandleBuffer
);

EFI_STATUS
InitializeNvmeController (
    IN  EFI_HANDLE                      Controller,
    IN  EFI_HANDLE                      DriverBindingHandle,
    OUT AMI_NVME_CONTROLLER_PROTOCOL    **NvmeController
);

EFI_STATUS
InstallBlockIoDiskInfo (
    IN  EFI_DRIVER_BINDING_PROTOCOL     *This,
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController
);

EFI_STATUS
CreateAdditionalSubmissionCompletionQueue (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController,
    IN  NVME_COMMAND_WRAPPER            *NvmeCmdWrapper,
    IN  UINT16                          QueueNumber,
    IN  UINT32                          QueueSize
);

EFI_STATUS
EFIAPI
ExecuteNvmeCmd (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController,
    IN  NVME_COMMAND_WRAPPER            *NvmeCmdWrapper,
    OUT COMPLETION_QUEUE_ENTRY          *CmdCompletionData
);

EFI_STATUS  
GetIdentifyData (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController,
    OUT UINT8                           *IdentifyData,
    IN  UINT8                           ControllerNameSpaceStructure, 
    IN  UINT32                          NameSpaceID
);

VOID
PrintNvmeCapability (
    AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController
);

VOID
PrintNvmeMassDevInfo (
    NVME_MASS_DEV_INFO  *NvmeMassDevInfo
);

EFI_STATUS
AddToAdminSubmissionQueue (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController,
    IN  NVME_COMMAND_WRAPPER            *NvmeCmdWrapper
);

EFI_STATUS
AddToQueue1SubmissionQueue (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController,
    IN  NVME_COMMAND_WRAPPER            *NvmeCmdWrapper
);

EFI_STATUS
UpdateDoorBellRegister (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController,
    IN  UINT16                          QueueNumber,
    IN  UINT32                          Value
);

EFI_STATUS
WaitForCompletionQueueUpdate (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController,
    IN  NVME_COMMAND_WRAPPER            *NvmeCmdWrapper,
    OUT COMPLETION_QUEUE_ENTRY          *CmdCompletionData 
);

EFI_STATUS
SetNumberOfQueues (
    IN  AMI_NVME_CONTROLLER_PROTOCOL     *NvmeController,
    IN  NVME_COMMAND_WRAPPER             *NvmeCmdWrapper
);

EFI_STATUS
EnumerateActiveNameSpace (
    IN OUT AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController,
    IN     UINT32                          ActiveNameSpaceId
);

EFI_STATUS
DetectActiveNameSpace (
    IN  AMI_NVME_CONTROLLER_PROTOCOL   *NvmeController,
    OUT ACTIVE_NAMESPACE_DATA          *ActiveNameSpaceData
);

VOID
PrintIdentifyDataStructure (
    IN  UINT8       *IdentifyData, 
    IN  UINT8       ControllerNameSpaceStructure
);

VOID
PrintCommandCompletionData (
    IN  COMPLETION_QUEUE_ENTRY   *pCmdCompletionData
);

VOID
PrintNvmeCmdWrapper(
    IN  NVME_COMMAND_WRAPPER            *NvmeCmdWrapper
);

EFI_STATUS
CreateNvmeDevicePath (
    IN  EFI_DRIVER_BINDING_PROTOCOL     *This, 
    IN  ACTIVE_NAMESPACE_DATA           *ActiveNameSpaceData
);

EFI_STATUS
RecreateAllQueues (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController
);

EFI_STATUS
EFIAPI
NvmeReadBlocks (
    IN  EFI_BLOCK_IO_PROTOCOL   *This,
    IN  UINT32                  MediaId,
    IN  EFI_LBA                 LBA,
    IN  UINTN                   BufferSize,
    OUT VOID                    *Buffer
);

EFI_STATUS
EFIAPI
NvmeWriteBlocks (
    IN  EFI_BLOCK_IO_PROTOCOL   *This,
    IN  UINT32                  MediaId,
    IN  EFI_LBA                 LBA,
    IN  UINTN                   BufferSize,
    OUT VOID                    *Buffer
);

EFI_STATUS
EFIAPI
NvmeReset (
    IN  EFI_BLOCK_IO_PROTOCOL   *This,
    IN  BOOLEAN                 ExtendedVerification
);

EFI_STATUS
EFIAPI
NvmeFlushBlocks (
    IN  EFI_BLOCK_IO_PROTOCOL   *This
);

EFI_STATUS
ProgramPRP2List (
    IN  UINT64          *PRP2List, 
    IN  UINT32          PageSize, 
    IN  UINTN           BufferAddress,
    IN  UINTN           BufferSize,
    IN  UINTN           *PRP2TransferSize
);

EFI_STATUS
NvmeReadWriteBlocks (
    IN  ACTIVE_NAMESPACE_DATA   *ActiveNameSpace,
    IN  UINT32                  MediaId,
    IN  EFI_LBA                 LBA,
    IN  UINTN                   BufferSize,
    OUT VOID                    *Buffer,
    OUT COMPLETION_QUEUE_ENTRY  *NvmeCompletionData,
    IN  UINT8                   ReadWriteOpCode
);

EFI_STATUS
ExperimentWithAsyncEvents (
    AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController
);

VOID
InstallNvmeLegacyDevice (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController,
    IN  ACTIVE_NAMESPACE_DATA           *ActiveNameSpace
);

EFI_STATUS 
NvmeSetDefaultGeometry ( 
    IN  OUT ACTIVE_NAMESPACE_DATA       *ActiveNameSpace
);

VOID
NvmeMassUpdateCylinderInfo (
    IN  OUT ACTIVE_NAMESPACE_DATA       *ActiveNameSpace
);

EFI_STATUS
NvmeValidatePartitionTable (
    IN  UINT8   *Buffer,
    IN  UINT64  dMaxLBA,
    OUT UINT8   **ActPartAddr
);

EFI_STATUS
NvmeUpdateCHSFromBootRecord (
    IN  OUT ACTIVE_NAMESPACE_DATA   *ActiveNameSpace,
    IN  UINT8*                      BootRecord
);

EFI_STATUS
GetNvmeGeometryData (
    IN  OUT ACTIVE_NAMESPACE_DATA   *ActiveNameSpace,
    IN  OUT NVME_MASS_DEV_INFO      *NvmeMassDevInfo
);

VOID
TransferNvmeDataToSmram (
    IN  UINTN    CommunicateBufferSize
);

EFI_STATUS
EFIAPI
NvmeDiskInfoWhichIde (
    IN EFI_DISK_INFO_PROTOCOL   *This,
    OUT UINT32                  *IdeChannel,
    OUT UINT32                  *IdeDevice
);

EFI_STATUS
EFIAPI
NvmeDiskInfoSenseData (
    IN EFI_DISK_INFO_PROTOCOL   *This,
    OUT VOID                    *SenseData,
    OUT UINT32                  *SenseDataSize,
    OUT UINT8                   *SenseDataNumber
);

EFI_STATUS
EFIAPI
NvmeDiskInfoIdentify (
    EFI_DISK_INFO_PROTOCOL      *This,
    IN OUT VOID                 *IdentifyData,
    IN OUT UINT32               *IdentifyDataSize
);

EFI_STATUS
EFIAPI
NvmeDiskInfoInquiry (
    IN EFI_DISK_INFO_PROTOCOL   *This,
    IN OUT VOID                 *InquiryData,
    IN OUT UINT32               *InquiryDataSize
);

UINT32
ReadDataDword (
    IN AMI_NVME_CONTROLLER_PROTOCOL   *pNvmeController,
    IN  UINTN   Offset
);

UINT16
ReadDataWord (
    IN AMI_NVME_CONTROLLER_PROTOCOL   *pNvmeController,
    IN  UINTN   Offset
);

UINT8
ReadDataByte (
    IN AMI_NVME_CONTROLLER_PROTOCOL   *pNvmeController,
    IN  UINTN   Offset
);

VOID
WriteDataDword (
    IN AMI_NVME_CONTROLLER_PROTOCOL   *pNvmeController,
    IN  UINTN   Offset, 
    IN  UINTN   Data
);

VOID
WriteDataWord (
    IN AMI_NVME_CONTROLLER_PROTOCOL   *pNvmeController,
    IN  UINTN   Offset, 
    IN  UINTN   Data
);

VOID
WriteDataByte (
    IN AMI_NVME_CONTROLLER_PROTOCOL   *pNvmeController,
    IN  UINTN   Offset,
    IN  UINTN   Data
);

EFI_STATUS
GetNvmeUnicodeStringName (
    AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController
);

VOID
NvmeSendSecurityFreezeLock (
    IN AMI_NVME_CONTROLLER_PROTOCOL  *NvmeController
);

EFI_STATUS
GetNvmeDeviceLockStatus (
    IN OUT AMI_NVME_CONTROLLER_PROTOCOL         *NvmeController,
    OUT NVME_DEVICE_SECURITY_STATUS             *NvmeSecurityStatus
);

VOID 
EFIAPI
NvmeShutdown (
    IN EFI_EVENT    Event,
    IN VOID         *Context 
);

VOID
EFIAPI
NvmeFreezeLockDevice (
    IN EFI_EVENT    Event,
    IN VOID         *Context 
);

EFI_STATUS 
EFIAPI
NvmeSendData (
    IN EFI_STORAGE_SECURITY_COMMAND_PROTOCOL    *This,
    IN UINT32   MediaId,
    IN UINT64   Timeout,
    IN UINT8    SecurityProtocolId,
    IN UINT16   SecurityProtocolSpecificData,
    IN UINTN    PayloadBufferSize,
    IN VOID     *PayloadBuffer 
);

EFI_STATUS
EFIAPI
NvmeReceiveData (
    IN EFI_STORAGE_SECURITY_COMMAND_PROTOCOL    *This,
    IN UINT32    MediaId,
    IN UINT64   Timeout,
    IN UINT8     SecurityProtocolId,
    IN UINT16   SecurityProtocolSpecificData,
    IN UINTN    PayloadBufferSize,
    OUT VOID    *PayloadBuffer,
    OUT UINTN   *PayloadTransferSize
);

VOID
EFIAPI
InstallSwSmiHandlerForLegacy (
    EFI_EVENT    Event,
    VOID         *Context
);

VOID
EFIAPI
NvmeOpromPolicy (
    EFI_EVENT  Event,
    VOID       *Context
);

VOID
EFIAPI
NvmeAllDriverConnectedCallback (
    EFI_EVENT    Event,
    VOID         *Context
);

EFI_STATUS 
IssueLockOnResetCommand (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController
);

VOID
UninstallNvmeControllerProtocols (
    IN  EFI_DRIVER_BINDING_PROTOCOL   *This,
    IN  EFI_HANDLE                    Controller,
    IN  AMI_NVME_CONTROLLER_PROTOCOL  *NvmeController
);

EFI_STATUS
UninstallNvmeDeviceProtocols (
    IN  EFI_DRIVER_BINDING_PROTOCOL  *This,
    IN  EFI_HANDLE                   NvmeDeviceHandle,
    IN  AMI_NVME_CONTROLLER_PROTOCOL *NvmeController,
    IN  ACTIVE_NAMESPACE_DATA        *ActiveNameSpace,
    IN  LIST_ENTRY                   **LinkData
);

VOID
EFIAPI
NvmeReadyToBootEventCallback (
    EFI_EVENT    Event,
    VOID         *Context
);

EFI_STATUS 
ProgramPRPEntry (
    IN  AMI_NVME_CONTROLLER_PROTOCOL         *NvmeController,
    IN  NVME_COMMAND_WRAPPER                 *NvmeCmdWrapper,
    IN  UINTN                                TransferLength,
    IN  UINTN                                *TransferBuffer,
    IN  ACTIVE_NAMESPACE_DATA                *ActiveNameSpace,
    OUT UINT64                               **PRP2List,
    OUT VOID                                 **PRP2ListUnMap,
    OUT UINTN                                *BufferSize
);

ACTIVE_NAMESPACE_DATA*
GetActiveNameSpace (
    AMI_NVME_CONTROLLER_PROTOCOL   *NvmeController,
    UINT32                         NamespaceId
);

BOOLEAN
IsSecurityProtocolInstalled (
    IN  EFI_HANDLE ControllerHandle,
    IN  EFI_GUID  *SecurityProtocolGuid     
);

extern EFI_GUID gEfiSmmCommunicationProtocolGuid;
extern EFI_GUID gAmiSmmNvmeCommunicationGuid;
extern EFI_GUID gBdsAllDriversConnectedProtocolGuid;

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif

#endif

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
