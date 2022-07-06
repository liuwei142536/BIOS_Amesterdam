//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file AhciBus.h
    Header file for the Ahci Bus Driver
**/

#ifndef _AhciBus_
#define _AhciBus_

#ifdef __cplusplus
extern "C" {
#endif

#include <Token.h>
#include <AmiDxeLib.h>
#include "Protocol/PciIo.h"
#include "Protocol/DriverBinding.h"
#include "Protocol/BlockIo.h"
#include "Protocol/DiskInfo.h"
#include <Protocol/AmiAhciBus.h>
#include <Protocol/AmiHddSecurityInit.h>
#include <Protocol/AmiHddSmartInit.h>
#include <Protocol/AmiHddOpalSecInit.h>
#include <Protocol/AmiAtaPassThruInit.h>
#include <Protocol/AmiScsiPassThruInit.h>
#include <Protocol/AtaPassThru.h>
#include <Protocol/StorageSecurityCommand.h>
#include <Protocol/SmmControl2.h>
#include <Protocol/TrEEProtocol.h>
#include "AhciController.h"
#include <Library/DebugLib.h>
#if MDE_PKG_VERSION > 12
#include <IndustryStandard/TcgStorageOpal.h>
#endif

#define     TRACE_AHCI_LEVEL2 TRACE
#define      TRACE_AHCI             TRACE

#ifdef Debug_Level_1
VOID EfiDebugPrint (IN  UINTN ErrorLevel,IN  CHAR8 *Format,...);
#endif

#define     AHCI_BUS_DRIVER_VERSION     0x01
#define     ONE_MILLISECOND             1000

#define     COMMAND_LIST_SIZE_PORT      0x800

#ifndef FLUSH_CACHE
#define     FLUSH_CACHE                     0xE7
#endif

#ifndef FLUSH_CACHE_EXT
#define     FLUSH_CACHE_EXT                 0xEA
#endif

#ifndef ATAPI_BUSY_CLEAR_TIMEOUT
#define     ATAPI_BUSY_CLEAR_TIMEOUT       30000                // 30sec
#endif


#ifndef BUSY_CLEAR_TIMEOUT
#define     BUSY_CLEAR_TIMEOUT          1000                    // 1Sec
#endif

// As per ATA ATAPI -8 Spec. some Non Data commands(like Flush Cache) may take Upto 30 seconds
// to complete the command.


#ifndef NON_DATA_COMMAND_COMPLETE_TIMEOUT
#define     NON_DATA_COMMAND_COMPLETE_TIMEOUT   30000            // 30sec
#endif


// As per ATA Spec.the device may take Upto 30 seconds to respond to commands
// in StandBy / Idle mode.
#ifndef DMA_ATA_COMMAND_COMPLETE_TIMEOUT
#define     DMA_ATA_COMMAND_COMPLETE_TIMEOUT    30000            // 30 Sec.
#endif

#ifndef DMA_ATAPI_COMMAND_COMPLETE_TIMEOUT
#define     DMA_ATAPI_COMMAND_COMPLETE_TIMEOUT  30000           // 30 Sec.

#endif

#define     TIMEOUT_1SEC                1000                    // 1sec Serial ATA 1.0 Sec 5.2

#define     BLKIO_REVISION                      1

#define     DEVICE_DISABLED                     0
#define     DEVICE_IN_RESET_STATE               1
#define     DEVICE_DETECTION_FAILED             2
#define     DEVICE_DETECTED_SUCCESSFULLY        3
#define     DEVICE_CONFIGURED_SUCCESSFULLY      4
#define     DEVICE_REMOVED                      5
#define     CONTROLLER_NOT_PRESENT              0xff

#define     ENUMERATE_ALL                       0xff
#define     ENUMERATE_PRIMARY_MASTER            0x0
#define     ENUMERATE_PRIMARY_SLAVE             0x1
#define     ENUMERATE_SECONDARY_MASTER          0x2
#define     ENUMERATE_SECONDARY_SLAVE           0x3
#define     INQUIRY_DATA_LENGTH                 0x96
#define     READ_CAPACITY_DATA_LENGTH           0x08

//  PCI Config Space equates
#define     PROGRAMMING_INTERFACE_OFFSET        0x09
#define     IDE_SUB_CLASS_CODE                  0x0A
    #define SCC_IDE_CONTROLLER                  0x01
    #define SCC_AHCI_CONTROLLER                 0x06
    #define SCC_RAID_CONTROLLER                 0x04
#define     IDE_BASE_CLASS_CODE                 0x0B
    #define BASE_CODE_IDE_CONTROLLER            0x01
#define     PRIMARY_COMMAND_BLOCK_OFFSET        0x10
#define     PRIMARY_CONTROL_BLOCK_OFFSET        0x14
#define     SECONDARY_COMMAND_BLOCK_OFFSET      0x18
#define     SECONDARY_CONTROL_BLOCK_OFFSET      0x1C
#define     LEGACY_BUS_MASTER_OFFSET            0x20

#define     EFI_SUCCESS_ACTIVE_SET              0x80000000

#ifndef TCPA_PPIOP_ENABLE_BLOCK_SID_FUNC
#define TCPA_PPIOP_ENABLE_BLOCK_SID_FUNC                                96
#endif

#ifndef ACPI_FUNCTION_GET_USER_CONFIRMATION_STATUS_FOR_REQUEST
#define ACPI_FUNCTION_GET_USER_CONFIRMATION_STATUS_FOR_REQUEST          8
#endif

#ifndef AMI_TCG_CONFIRMATION_FLAGS_GUID
#define AMI_TCG_CONFIRMATION_FLAGS_GUID \
    {0x7d3dceee, 0xcbce, 0x4ea7, 0x87, 0x09, 0x6e, 0x55, 0x2f, 0x1e, 0xdb, 0xde}
#endif

typedef struct {
    UINT32 RQST;                 //Store PPI request
    UINT32 RCNT;                 //Store most recent PPI request
    UINT32 ERROR;                //Store BIOS ERROR information after a PPI request
    UINT32 Flag;                 //Flag used by PPI SMM interface to determine ACPI PPI function request
    UINT32 AmiMisc;              //Misc storage not used
    UINT32 OptionalRqstData;     //Store optional data that OS might provide during a PPI request
    UINT32 RequestFuncResponse;  //Store Response of ACPI function request. Returned to caller
} AMI_ASL_PPI_NV_VAR;

// TCG related structures available from MdePkg_13.
// Defining structures locally if MdePkg is less than label 13
#if MDE_PKG_VERSION < 13
#define TCG_OPAL_SECURITY_PROTOCOL_1                 0x01
#define TCG_OPAL_SECURITY_PROTOCOL_2                 0x02

#define TCG_SP_SPECIFIC_PROTOCOL_LIST               0x0000
#define TCG_SP_SPECIFIC_PROTOCOL_LEVEL0_DISCOVERY   0x0001

// Defined in TCG Storage Feature Set:Block SID Authentication spec,
// ComId used for BlockSid command is hardcode 0x0005.
#define TCG_BLOCKSID_COMID 0x0005

#define TCG_FEATURE_LOCKING             (UINT16)0x0002
#define TCG_FEATURE_BLOCK_SID           (UINT16)0x0402

#pragma pack(1)
typedef struct {
    UINT8   Reserved[6];
    UINT16  ListLength_BE;  // 6 - 7
    UINT8   List[504];      // 8...
} TCG_SUPPORTED_SECURITY_PROTOCOLS;

// Level 0 Discovery
typedef struct {
    UINT32 LengthBE;    // number of valid bytes in discovery response, not including length field
    UINT16 VerMajorBE;
    UINT16 VerMinorBE;
    UINT8  Reserved[8];
    UINT8  VendorUnique[32];
} TCG_LEVEL0_DISCOVERY_HEADER;

typedef struct _TCG_LEVEL0_FEATURE_DESCRIPTOR_HEADER {
    UINT16 FeatureCode_BE;
    UINT8  Reserved : 4;
    UINT8  Version : 4;
    UINT8  Length;     // length of feature dependent data in bytes
} TCG_LEVEL0_FEATURE_DESCRIPTOR_HEADER;

typedef struct {
    TCG_LEVEL0_FEATURE_DESCRIPTOR_HEADER Header;
    UINT8                                LockingSupported : 1;
    UINT8                                LockingEnabled : 1;   // means the locking security provider (SP) is enabled
    UINT8                                Locked : 1;   // means at least 1 locking range is enabled
    UINT8                                MediaEncryption : 1;
    UINT8                                MbrEnabled : 1;
    UINT8                                MbrDone : 1;
    UINT8                                Reserved : 2;
    UINT8                                Reserved515[11];
} TCG_LOCKING_FEATURE_DESCRIPTOR;

typedef struct {
    TCG_LEVEL0_FEATURE_DESCRIPTOR_HEADER Header;
    UINT8                                SIDValueState : 1;
    UINT8                                SIDBlockedState : 1;
    UINT8                                Reserved4 : 6;
    UINT8                                HardwareReset : 1;
    UINT8                                Reserved5 : 7;
    UINT8                                Reserved615[10];
} TCG_BLOCK_SID_FEATURE_DESCRIPTOR;
#pragma pack()
#endif // #if MDE_PKG_VERSION < 13

EFI_STATUS
InstallAhciBusProtocol (
    IN EFI_HANDLE                    Controller,
    AMI_AHCI_BUS_PROTOCOL            *IdeBusInitInterface,
    EFI_IDE_CONTROLLER_INIT_PROTOCOL *IdeControllerInterface,
    EFI_PCI_IO_PROTOCOL              *PciIO
);

EFI_STATUS
AhciInitController (
    AMI_AHCI_BUS_PROTOCOL               *AhciBusInterface
);

EFI_STATUS
CheckPortImplemented (
    AMI_AHCI_BUS_PROTOCOL               *AhciBusInterface, 
    UINT8                               Port
);

EFI_STATUS
AhciDetectDevice (
    AMI_AHCI_BUS_PROTOCOL               *AhciBusInterface, 
    EFI_IDE_CONTROLLER_INIT_PROTOCOL    *IdeControllerInterface, 
    UINT8                               Port, 
    UINT8                               PMPortNumber
);

EFI_STATUS
CheckDevicePresence (
    SATA_DEVICE_INTERFACE               *SataDevInterface,
    EFI_IDE_CONTROLLER_INIT_PROTOCOL    *IdeControllerInterface, 
    UINT8                               Port, 
    UINT8                               PMPort
);

EFI_STATUS
CheckPMDevicePresence (
    SATA_DEVICE_INTERFACE   			*SataDevInterface,
    EFI_IDE_CONTROLLER_INIT_PROTOCOL    *IdeControllerInterface, 
    UINT8                               Port, 
    UINT8                               PMPort
);

EFI_STATUS
ConfigureDevice (
    SATA_DEVICE_INTERFACE         *SataDevInterface,
    EFI_ATA_COLLECTIVE_MODE       **SupportedModes
);

EFI_STATUS
ConfigureController (
    SATA_DEVICE_INTERFACE         *SataDevInterface,
    EFI_ATA_COLLECTIVE_MODE       *SupportedModes
);

VOID 
InitializeDipm(
    SATA_DEVICE_INTERFACE         *SataDevInterface
);

VOID
InitializeDeviceSleep (
    SATA_DEVICE_INTERFACE         *SataDevInterface
);

EFI_STATUS
HostReset (
    AMI_AHCI_BUS_PROTOCOL           *AhciBusInterface 
);

EFI_STATUS
EFIAPI 
GeneratePortReset (
    AMI_AHCI_BUS_PROTOCOL          *AhciBusInterface, 
    SATA_DEVICE_INTERFACE          *SataDevInterface,  
    UINT8                          Port,
    UINT8                          PMPort,
    UINT8                          Speed,
    UINT8                          PowerManagement
);

EFI_STATUS
EFIAPI 
GenerateSoftReset (
    SATA_DEVICE_INTERFACE               *SataDevInterface,
    UINT8                               PMPort
);

EFI_STATUS
GetIdentifyData (
    SATA_DEVICE_INTERFACE          *SataDevInterface 
);

EFI_STATUS
HandlePortComReset (
    AMI_AHCI_BUS_PROTOCOL               *AhciBusInterface, 
    SATA_DEVICE_INTERFACE               *SataDevInterface,
    UINT8                               Port,
    UINT8                               PMPort
);

EFI_STATUS 
CheckValidDevice (
    SATA_DEVICE_INTERFACE               *SataDevInterface,
    UINT8                               Port,
    UINT8                               PMPort
);

SATA_DEVICE_INTERFACE*
GetSataDevInterface (
    AMI_AHCI_BUS_PROTOCOL          *AhciBusInterface, 
    UINT8                          Port,
    UINT8                          PMPort
);

EFI_STATUS
EFIAPI 
ExecuteNonDataCommand (
    SATA_DEVICE_INTERFACE               *SataDevInterface, 
    COMMAND_STRUCTURE                   CommandStructure
);

EFI_STATUS
EFIAPI 
ExecutePioDataCommand (
    SATA_DEVICE_INTERFACE               *SataDevInterface, 
    COMMAND_STRUCTURE                   *CommandStructure,
    BOOLEAN                             READWRITE
);

EFI_STATUS
EFIAPI 
ExecuteDmaDataCommand (
    SATA_DEVICE_INTERFACE               *SataDevInterface, 
    COMMAND_STRUCTURE                   *CommandStructure,
    BOOLEAN                             READWRITE
);

EFI_STATUS
EFIAPI 
SataReadWritePio (
    IN SATA_DEVICE_INTERFACE        *SataDevInterface,
    IN OUT VOID                     *Buffer,
    IN UINTN                        ByteCount,
    IN UINT64                       LBA,
    IN UINT8                        ReadWriteCommand,
    IN BOOLEAN                      READWRITE
) ;

EFI_STATUS
EFIAPI 
SataPioDataOut (
    IN SATA_DEVICE_INTERFACE        *SataDevInterface,
    COMMAND_STRUCTURE               CommandStructure,
    IN BOOLEAN                      READWRITE
) ;

EFI_STATUS
EFIAPI 
WaitforCommandComplete (
    SATA_DEVICE_INTERFACE               *SataDevInterface,
    COMMAND_TYPE                        CommandType,
    UINTN                               TimeOut    
);

UINT8
ReturnMSBset (
 UINT32     Data
);

EFI_STATUS
StartController (
    AMI_AHCI_BUS_PROTOCOL               *AhciBusInterface, 
    SATA_DEVICE_INTERFACE               *SataDevInterface,
    UINT32                              CIBitMask
);

EFI_STATUS
ReadytoAcceptCmd (
    SATA_DEVICE_INTERFACE               *SataDevInterface
);

EFI_STATUS
StopController (
    AMI_AHCI_BUS_PROTOCOL               *AhciBusInterface, 
    SATA_DEVICE_INTERFACE               *SataDevInterface,
    BOOLEAN                             StartOrStop
);

EFI_STATUS
DetectAndConfigureDevice (
    IN EFI_DRIVER_BINDING_PROTOCOL    *This,
    IN EFI_HANDLE                     Controller,
    IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath,
	AMI_AHCI_BUS_PROTOCOL             *AhciBusInterface,
    EFI_IDE_CONTROLLER_INIT_PROTOCOL  *IdeControllerInterface,
    UINT8                             Port,
    UINT8                             PMPort
);

EFI_STATUS
InstallOtherOptionalFeatures(
    IN AMI_AHCI_BUS_PROTOCOL          *AhciBusInterface
);

BOOLEAN
CheckForLockedDrives(
    IN  AMI_AHCI_BUS_PROTOCOL         *AhciBusInterface
);

EFI_STATUS
ConfigurePMPort (
    SATA_DEVICE_INTERFACE   *SataDevInterface
);

EFI_STATUS
ReadWritePMPort (
    SATA_DEVICE_INTERFACE   *SataDevInterface,
    UINT8                   Port,
    UINT8                   RegNum,
    UINT32                  *Data,
    BOOLEAN                 READWRITE	
);

UINT32
ReadSCRRegister (
	AMI_AHCI_BUS_PROTOCOL   *AhciBusInterface, 
    SATA_DEVICE_INTERFACE   *SataDevInterface, 
    UINT8                   Port, 
    UINT8                   PMPort, 
    UINT8                   Register
);

EFI_STATUS
WriteSCRRegister (
    AMI_AHCI_BUS_PROTOCOL   *AhciBusInterface, 
    SATA_DEVICE_INTERFACE   *SataDevInterface,
    UINT8                   Port, 
    UINT8                   PMPort, 
    UINT8                   Register,
    UINT32                  Data32
);

EFI_STATUS
WritePMPort (
    SATA_DEVICE_INTERFACE   *SataDevInterface,
    UINT8                   Port,
    UINT8                   RegNum,
    UINT32                  Data	
);

EFI_STATUS
BuildCommandList (
    SATA_DEVICE_INTERFACE               *SataDevInterface, 
    AHCI_COMMAND_LIST                   *CommandList,
    UINT64                              CommandTableBaseAddr
);

EFI_STATUS
BuildCommandFIS (
    SATA_DEVICE_INTERFACE               *SataDevInterface, 
    COMMAND_STRUCTURE                   CommandStructure,
    AHCI_COMMAND_LIST                   *CommandList,
    AHCI_COMMAND_TABLE                  *Commandtable
);

EFI_STATUS
BuildAtapiCMD (
    SATA_DEVICE_INTERFACE               *SataDevInterface, 
    COMMAND_STRUCTURE                   CommandStructure,
    AHCI_COMMAND_LIST                   *CommandList,
    AHCI_COMMAND_TABLE                  *Commandtable
);

EFI_STATUS
BuildPRDT (
    SATA_DEVICE_INTERFACE               *SataDevInterface, 
    COMMAND_STRUCTURE                   CommandStructure,
    AHCI_COMMAND_LIST                   *CommandList,
    AHCI_COMMAND_TABLE                  *Commandtable
);

EFI_STATUS 
WaitForMemSet (
    IN AMI_AHCI_BUS_PROTOCOL   *AhciBusInterface,
    IN UINT8  Port,
    IN UINT8  Register,
    IN UINT32 AndMask,
    IN UINT32 TestValue,
    IN UINT32 WaitTimeInMs
);

EFI_STATUS 
WaitforPMMemSet (
    IN SATA_DEVICE_INTERFACE   *SataDevInterface,
    IN UINT8                   PMPort,
    IN UINT8                   Register,
    IN UINT32                  AndMask,
    IN UINT32                  TestValue,
    IN UINT32                  WaitTimeInMs
);

EFI_STATUS 
WaitForMemClear (
    IN AMI_AHCI_BUS_PROTOCOL   *AhciBusInterface,
    IN UINT8  Port,
    IN UINT8  Register,
    IN UINT32 AndMask,
    IN UINT32 WaitTimeInMs
);

EFI_STATUS
CreateSataDevicePath (
    IN EFI_DRIVER_BINDING_PROTOCOL      *This,
    IN EFI_HANDLE                       Controller,
    IN SATA_DEVICE_INTERFACE            *SataDevInterface,	
    IN OUT EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
);

EFI_STATUS
InitSataBlockIO (
    IN SATA_DEVICE_INTERFACE            *SataDevInterface
);

EFI_STATUS
InitSataDiskInfo (
    IN SATA_DEVICE_INTERFACE            *SataDevInterface
);

EFI_STATUS
InitAcousticSupport (
    IN SATA_DEVICE_INTERFACE            *SataDevInterface
);	

EFI_STATUS
InitSMARTSupport (
    IN SATA_DEVICE_INTERFACE            *SataDevInterface
);	

EFI_STATUS
SMARTReturnStatusWrapper (
    IN SATA_DEVICE_INTERFACE            *SataDevInterface
);

EFI_STATUS
ConfigurePowerUpInStandby (
    IN SATA_DEVICE_INTERFACE            *SataDevInterface
);

EFI_STATUS 
EFIAPI 
AhciBusSupported (
    IN EFI_DRIVER_BINDING_PROTOCOL    *This,
    IN EFI_HANDLE                     Controller,
    IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
);

EFI_STATUS 
EFIAPI 
AhciBusStart (
    IN EFI_DRIVER_BINDING_PROTOCOL    *This,
    IN EFI_HANDLE                     Controller,
    IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath 
);

EFI_STATUS 
EFIAPI 
AhciBusStop (
    IN EFI_DRIVER_BINDING_PROTOCOL    *This,
    IN EFI_HANDLE                     Controller,
    IN UINTN                          NumberOfChildren,
    IN EFI_HANDLE                     *ChildHandleBuffer
);


EFI_STATUS
EFIAPI 
DiskInfoInquiry (
    IN EFI_DISK_INFO_PROTOCOL   *This,
    IN OUT VOID                 *InquiryData,
    IN OUT UINT32               *InquiryDataSize
);

EFI_STATUS
EFIAPI 
DiskInfoIdentify (
    EFI_DISK_INFO_PROTOCOL      *This,
    IN OUT VOID                 *IdentifyData,
    IN OUT UINT32               *IdentifyDataSize
);

EFI_STATUS
EFIAPI 
DiskInfoSenseData (
    EFI_DISK_INFO_PROTOCOL      *This,
    VOID                        *SenseData,
    UINT32                      *SenseDataSize,
    UINT8                       *SenseDataNumber
);

EFI_STATUS
EFIAPI 
DiskInfoWhichIDE (
    IN EFI_DISK_INFO_PROTOCOL   *This,
    OUT UINT32                  *IdeChannel,
    OUT UINT32                  *IdeDevice
);

#define ZeroMemory(Buffer,Size) pBS->SetMem(Buffer,Size,0)

BOOLEAN
DMACapable (
    SATA_DEVICE_INTERFACE       *SataDevInterface
); 

EFI_STATUS
EFIAPI 
SataBlkRead (
    IN EFI_BLOCK_IO_PROTOCOL    *This,
    IN UINT32                   MediaId,
    IN EFI_LBA                  LBA,
    IN UINTN                    BufferSize,
    OUT VOID                    *Buffer
);

EFI_STATUS
EFIAPI 
SataBlkWrite (
    IN EFI_BLOCK_IO_PROTOCOL    *This,
    IN UINT32                   MediaId,
    IN EFI_LBA                  LBA,
    IN UINTN                    BufferSize,
    OUT VOID                    *Buffer
);

EFI_STATUS
SataAtaBlkReadWrite (
    IN EFI_BLOCK_IO_PROTOCOL    *This,
    IN UINT32                   MediaId,
    IN EFI_LBA                  LBA,
    IN UINTN                    BufferSize,
    OUT VOID                    *Buffer,
    BOOLEAN                     READWRITE
);

EFI_STATUS
EFIAPI 
SataAtapiBlkRead (
    IN EFI_BLOCK_IO_PROTOCOL    *This,
    IN UINT32                   MediaId,
    IN EFI_LBA                  LBA,
    IN UINTN                    BufferSize,
    OUT VOID                    *Buffer
);

EFI_STATUS
EFIAPI 
SataAtapiBlkWrite (
    IN EFI_BLOCK_IO_PROTOCOL    *This,
    IN UINT32                   MediaId,
    IN EFI_LBA                  LBA,
    IN UINTN                    BufferSize,
    OUT VOID                    *Buffer
);

EFI_STATUS
EFIAPI 
SataReset (
    IN EFI_BLOCK_IO_PROTOCOL    *This,
    IN BOOLEAN                  ExtendedVerification
);

EFI_STATUS
EFIAPI 
SataBlkFlush (
    IN EFI_BLOCK_IO_PROTOCOL    *This
);

EFI_STATUS
SataReadWriteBusMaster (
    SATA_DEVICE_INTERFACE       *SataDevInterface,
    IN OUT VOID                 *Buffer,
    IN UINTN                    ByteCount,
    IN UINT64                   LBA,
    IN UINT8                    ReadWriteCommand,
    IN BOOLEAN                  READWRITE
);

EFI_STATUS
SataAtapiInquiryData (
    SATA_DEVICE_INTERFACE       *SataDevInterface,
    UINT8                       *InquiryData,
    UINT16                      *InquiryDataSize
);

EFI_STATUS
DetectAtapiMedia (
    SATA_DEVICE_INTERFACE           *SataDevInterface
);

EFI_STATUS
TestUnitReady (
    SATA_DEVICE_INTERFACE           *SataDevInterface
);

EFI_STATUS 
EFIAPI 
ExecutePacketCommand (
    SATA_DEVICE_INTERFACE           *SataDevInterface, 
    COMMAND_STRUCTURE               *CommandStructure,
    BOOLEAN                         READWRITE
);

EFI_STATUS
SataAtapiBlkReadWrite (
    IN EFI_BLOCK_IO_PROTOCOL        *This,
    IN UINT32                       MediaId,
    IN EFI_LBA                      LBA,
    IN UINTN                        BufferSize,
    OUT VOID                        *Buffer,
    BOOLEAN                         READWRITE
);

EFI_STATUS
HandleAtapiError (
    SATA_DEVICE_INTERFACE           *SataDevInterface
);

BOOLEAN
Check48BitCommand (
    IN UINT8                        Command
);

EFI_STATUS
InitSMARTSupport (
    IN SATA_DEVICE_INTERFACE        *SataDevInterface 
);

EFI_STATUS
SMARTReturnStatusWrapper (
    IN SATA_DEVICE_INTERFACE        *SataDevInterface 
);

EFI_STATUS
SataGetOddType (
    IN SATA_DEVICE_INTERFACE        *SataDevInterface,
    IN OUT UINT16                   *OddType
);

EFI_STATUS
SataGetOddLoadingType (
    IN SATA_DEVICE_INTERFACE        *SataDevInterface,
    IN OUT UINT8                    *OddLoadingType
);

ODD_TYPE 
GetEnumOddType (
    IN  UINT16                      OddType
);

EFI_STATUS
ConfigureSataPort (
    IN SATA_DEVICE_INTERFACE        *SataDevInterface
);

UINT64 
Shr64 (
    IN UINT64 Value,
    IN UINT8 Shift 
);

UINT64
Shl64 (
    IN UINT64 Value,
    IN UINT8 Shift
);

UINT32
ReadDataDword (
    IN AMI_AHCI_BUS_PROTOCOL   *AhciBusInterface,
    IN  UINTN   Index
);

UINT16
ReadDataWord (
    IN AMI_AHCI_BUS_PROTOCOL   *AhciBusInterface,
    IN  UINTN   Index
);

UINT8
ReadDataByte (
    IN AMI_AHCI_BUS_PROTOCOL   *AhciBusInterface,
    IN  UINTN   Index
);

VOID
WriteDataDword (
    IN AMI_AHCI_BUS_PROTOCOL   *AhciBusInterface,
    IN  UINTN   Index, 
    IN  UINTN   Data
);

VOID
WriteDataWord (
    IN AMI_AHCI_BUS_PROTOCOL   *AhciBusInterface,
    IN  UINTN   Index, 
    IN  UINTN   Data
);

VOID
WriteDataByte (
    IN AMI_AHCI_BUS_PROTOCOL   *AhciBusInterface,
    IN  UINTN   Index,
    IN  UINTN   Data
);

VOID
PrintCommandFIS (
  IN COMMAND_STRUCTURE        CommandStructure
);

VOID
PrintAhciCapability  (
  IN AMI_AHCI_BUS_PROTOCOL     *AhciBusInterface
);

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif

#endif

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
