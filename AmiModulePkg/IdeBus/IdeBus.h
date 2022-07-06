//***********************************************************************
//***********************************************************************
//**                                                                   **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.          **
//**                                                                   **
//**                       All Rights Reserved.                        **
//**                                                                   **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093         **
//**                                                                   **
//**                       Phone: (770)-246-8600                       **
//**                                                                   **
//***********************************************************************
//***********************************************************************

/** @file IdeBus.h
    This file contains the Includes, Definitions, typedefs,
    Variable and External Declarations, Structure and
    function prototypes needed for the IdeBus driver

**/


#ifndef _IdeBus_
#define _IdeBus_

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------

#include <Token.h>
#include <AmiDxeLib.h>
#include "Protocol/PciIo.h"
#include "Protocol/DevicePath.h"
#include "Protocol/DriverBinding.h"
#include "Protocol/BlockIo.h"
#include <IndustryStandard/AmiAtaAtapi.h>
#include <Protocol/AmiIdeBus.h>
#include <Protocol/AmiHddSecurityInit.h>
#include <Protocol/AmiHddSmartInit.h>
#include <Protocol/AmiHddOpalSecInit.h>
#include <Protocol/AmiAtaPassThruInit.h>
#include <Protocol/AmiScsiPassThruInit.h>
#include <Protocol/ComponentName.h>
#include <Protocol/BootScriptSave.h>
#include <Protocol/AtaPassThru.h>
#include <Protocol/AmiIdeBusBoard.h>

//---------------------------------------------------------------------------

#define     IDE_BUS_DRIVER_VERSION              0x01

#define     PRIMARY_COMMAND_BLOCK               0x1F0
#define     PRIMARY_CONTROL_BLOCK               0x3F6
#define     SECONDARY_COMMAND_BLOCK             0x170
#define     SECONDARY_CONTROL_BLOCK             0x376

#ifndef FLUSH_CACHE
#define     FLUSH_CACHE                     0xE7
#endif

#ifndef FLUSH_CACHE_EXT
#define     FLUSH_CACHE_EXT                 0xEA
#endif

#ifndef BUSY_CLEAR_TIMEOUT
#define     BUSY_CLEAR_TIMEOUT                  1000            // 1Sec
#endif

#ifndef DRDY_TIMEOUT
#define     DRDY_TIMEOUT                        1000            // 1Sec
#endif

#ifndef DRQ_TIMEOUT
#define     DRQ_TIMEOUT                         10              // 10msec
#endif

#ifndef DRQ_CLEAR_TIMEOUT
#define     DRQ_CLEAR_TIMEOUT                   1000            // 1sec
#endif

#ifndef DRQ_SET_TIMEOUT
#define     DRQ_SET_TIMEOUT                     10              // 10msec
#endif

#ifndef HP_COMMAND_COMPLETE_TIMEOUT
#define     HP_COMMAND_COMPLETE_TIMEOUT         2000            // 2Sec
#endif

#ifndef COMMAND_COMPLETE_TIMEOUT
#define     COMMAND_COMPLETE_TIMEOUT            5000            // 5Sec
#endif

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

//	PCI Configure Space equates
#define     PROGRAMMING_INTERFACE_OFFSET        0x09
#define     IDE_SUB_CLASS_CODE                  0x0A
#define     SCC_IDE_CONTROLLER                  0x01
#define     SCC_AHCI_CONTROLLER                 0x06
#define     SCC_RAID_CONTROLLER                 0x04
#define     IDE_BASE_CLASS_CODE                 0x0B
#define     BASE_CODE_IDE_CONTROLLER            0x01
#define     PRIMARY_COMMAND_BLOCK_OFFSET        0x10
#define     PRIMARY_CONTROL_BLOCK_OFFSET        0x14
#define     SECONDARY_COMMAND_BLOCK_OFFSET      0x18
#define     SECONDARY_CONTROL_BLOCK_OFFSET      0x1C
#define     LEGACY_BUS_MASTER_OFFSET            0x20
#define     PCI_SIDPBA                          0x24
#define     SSTS_DET_PCE                        0x0003

#define     EFI_SUCCESS_ACTIVE_SET              0x80000000
#define     HBA_PORTS_START                     0x0100
#define     HBA_PORTS_SCTL                      0x002C
#define ZeroMemory( Buffer, Size ) pBS->SetMem( Buffer, Size, 0 )


// Forward reference for pure ANSI compatibility
typedef struct _AMI_IDE_BUS_PROTOCOL AMI_IDE_BUS_PROTOCOL;

typedef struct {
    UINT32 Lowdword;
    UINT32 Upperdword;
} STRUCT_U64_U32;

typedef struct {
    UINT8   PrimaryChannel;
    UINT8   PrimaryMasterDevice;
    UINT8   PrimarySlaveDevice;
    UINT8   SecondaryChannel;
    UINT8   SecondaryMasterDevice;
    UINT8   SecondarySlaveDevice;
    UINT8   BusMasterEnable;
    UINT8   HPMask;
    UINT8   Flags;
    UINT8   Acoustic_Management_Level;
} CONTROLLER_INFO;  

EFI_STATUS
CreateIdeDevicePath (
    IN  EFI_DRIVER_BINDING_PROTOCOL     *This,
    IN   EFI_HANDLE                     Controller,
    AMI_IDE_BUS_INIT_PROTOCOL           *IdeBusInitInterface,
    AMI_IDE_BUS_PROTOCOL                *IdeBusInterface,
    IN  EFI_DEVICE_PATH_PROTOCOL        *RemainingDevicePath,
    UINT8                               Current_Channel,
    UINT8                               Current_Device
);

EFI_STATUS
InitIdeBlockIO (
    AMI_IDE_BUS_PROTOCOL        *IdeBusInterface
);

EFI_STATUS
InitIdeDiskInfo (
    AMI_IDE_BUS_PROTOCOL        *IdeBusInterface
);

EFI_STATUS
InstallBusInitProtocol (
    IN                          EFI_HANDLE Controller,
    AMI_IDE_BUS_INIT_PROTOCOL   * IdeBusInitInterface,
    VOID                        * IdeControllerInterface
);

EFI_STATUS
ConfigureIdeDeviceAndController (
    AMI_IDE_BUS_PROTOCOL                * IdeBusInterface,
    VOID                                * IdeControllerInterface,
    AMI_IDE_BUS_INIT_PROTOCOL           * IdeBusInitInterface
);

UINT8
ReturnMSBset (
    UINT32       Data
);

EFI_STATUS
IdeReadByte (
    IN  EFI_PCI_IO_PROTOCOL     *PciIO,
    IN  UINT16                  Register,
    OUT UINT8                   *Data8
);

EFI_STATUS
IdeReadMultipleByte (
    IN  EFI_PCI_IO_PROTOCOL     *PciIO,
    IN  UINT16                  Register,
    IN  UINT32                  Count,
    OUT UINT8                   *Data8
);

EFI_STATUS
IdeReadWord (
    IN  EFI_PCI_IO_PROTOCOL     *PciIO,
    IN  UINT16                  Register,
    OUT UINT16                  *Data16
);

EFI_STATUS
IdeReadMultipleWord (
    IN  EFI_PCI_IO_PROTOCOL     *PciIO,
    IN  UINT16                  Register,
    IN  UINT32                  Count,
    OUT UINT16                  *Data16
);

EFI_STATUS
IdeWriteByte (
    IN  EFI_PCI_IO_PROTOCOL     *PciIO,
    IN  UINT16                  Register,
    IN  UINT8                   Data8
);

EFI_STATUS
IdeWriteMultipleByte (
    IN   EFI_PCI_IO_PROTOCOL    *PciIO,
    IN  UINT16                  Register,
    IN  UINT32                  Count,
    IN  UINT8                   *Data8
);

EFI_STATUS
IdeWriteWord (
    IN  EFI_PCI_IO_PROTOCOL     *PciIO,
    IN  UINT16                  Register,
    IN  UINT16                  Data16
);

EFI_STATUS
IdeWriteMultipleWord (
    IN  EFI_PCI_IO_PROTOCOL     *PciIO,
    IN  UINT16                  Register,
    IN  UINT32                  Count,
    IN  UINT16                  *Data16
);

EFI_STATUS
IdeWriteDword (
    IN  EFI_PCI_IO_PROTOCOL     *PciIO,
    IN  UINT16                  Register,
    IN  UINT32                  Data32
);

EFI_STATUS
AtaReadWritePio (
    IN  AMI_IDE_BUS_PROTOCOL *IdeBusInterface,
    VOID                *Buffer,
    UINTN               ByteCount,
    UINT64              LBA,
    UINT8               readWriteCommand,
    BOOLEAN             ReadWrite
);

EFI_STATUS
IssueAtaReadWriteCommand (
    IN  AMI_IDE_BUS_PROTOCOL *IdeBusInterface,
    UINT64              LBA,
    INT32               SectorCount,
    UINT8               Command, 
    UINT8               Features
);

EFI_STATUS
AtaPioDataIn (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    VOID                *Buffer,
    UINT32              ByteCount,
    UINT8               Features,
    UINT8               SectorCount,
    UINT8               LBALow,
    UINT8               LBAMid,
    UINT8               LBAHigh,
    UINT8               DeviceReg,
    UINT8               Command,
    BOOLEAN             Multiple
);

EFI_STATUS
AtaPioDataOut (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    VOID                *Buffer,
    UINTN               ByteCount,
    UINT8               Features,
    UINT32              SectorCountIn,
    UINT8               LBALow,
    UINT8               LBALowExp,
    UINT8               LBAMid,
    UINT8               LBAMidExp,
    UINT8               LBAHigh,
    UINT8               LBAHighExp,
    UINT8               Device,
    UINT8               Command,
    BOOLEAN             ReadWrite,
    BOOLEAN             Multiple
);

EFI_STATUS
IdeNonDataCommand (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    UINT8               Features,
    UINT8               SectorCount,
    UINT8               LBALow,
    UINT8               LBAMid,
    UINT8               LBAHigh,
    UINT8               Device,
    UINT8               Command
);

EFI_STATUS
IdeNonDataCommandExp (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    UINT8               Features,
    UINT8               SectorCount,
    UINT8               SectorCountExp,
    UINT8               LBALow,
    UINT8               LBALowExp,
    UINT8               LBAMid,
    UINT8               LBAMidExp,
    UINT8               LBAHigh,
    UINT8               LBAHighExp,
    UINT8               Device,
    UINT8               Command
);

EFI_STATUS
AtaIdentifyCommand (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  OUT VOID                *Buffer
);

EFI_STATUS
AtapiIdentifyCommand (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  OUT VOID                *Buffer
);

EFI_STATUS
GetIdentifyData (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  OUT VOID                *Buffer
);

EFI_STATUS
IdeSetFeatureCommand (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    UINT8                       SubCommand,
    UINT8                       Mode
);

EFI_STATUS
IdeSoftReset (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
);

EFI_STATUS
WaitForCmdCompletion (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
);

EFI_STATUS
WaitForCmdCompletionWithTimeOutValue (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  UINT32                  TimeOutvalue
);

EFI_STATUS
CheckDriveReady (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
);

EFI_STATUS
WaitforBitSet (
    IN  EFI_PCI_IO_PROTOCOL *PciIO,
    UINT16                  AlternateStatusReg,
    UINT8                   BitSet,
    UINT32                  TimeOut
);

EFI_STATUS
WaitforBitClear (
    IN  EFI_PCI_IO_PROTOCOL *PciIO,
    UINT16                  AlternateStatus,
    UINT8                   BitSet,
    UINT32                  Timeout
);

EFI_STATUS
WaitforBitClearWithoutTimeout (
    IN  EFI_PCI_IO_PROTOCOL *PciIO,
    UINT16                  AlternateStatus,
    UINT8                   BitClear
);

EFI_STATUS
ControllerPresence (
    AMI_IDE_BUS_PROTOCOL    *IdeBusInterface 
);

void
DisableIdeInterrupt (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
);

EFI_STATUS
DetectIdeDevice (
    AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
);

EFI_STATUS
ConfigureIdeDevice (
    AMI_IDE_BUS_PROTOCOL        *IdeBusInterface,
    AMI_IDE_BUS_INIT_PROTOCOL   *IdeBusInitInterface
);

EFI_STATUS
InitAcousticSupport (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
);

EFI_STATUS
UpdateBaseAddress (
    AMI_IDE_BUS_PROTOCOL        *IdeBusInterface
);

EFI_STATUS
IdeBusSupported (
    IN  EFI_DRIVER_BINDING_PROTOCOL *This,
    IN  EFI_HANDLE                  Controller,
    IN  EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
);

EFI_STATUS
IdeBusStart (
    IN  EFI_DRIVER_BINDING_PROTOCOL *This,
    IN  EFI_HANDLE                  Controller,
    IN  EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
);

EFI_STATUS
IdeBusStop (
    IN   EFI_DRIVER_BINDING_PROTOCOL    *This,
    IN  EFI_HANDLE                      Controller,
    IN  UINTN                           NumberOfChildren,
    IN  EFI_HANDLE                      *ChildHandleBuffer
);

EFI_STATUS
AtaReset (
    IN  EFI_BLOCK_IO_PROTOCOL   *This,
    IN  BOOLEAN                 ExtendedVerification
);

EFI_STATUS
AtaBlkRead (
    IN  EFI_BLOCK_IO_PROTOCOL   *This,
    IN  UINT32                  MediaId,
    IN  EFI_LBA                 LBA,
    IN  UINTN                   BufferSize,
    OUT VOID                    *Buffer
);

EFI_STATUS
AtaBlkWrite (
    IN  EFI_BLOCK_IO_PROTOCOL   *This,
    IN  UINT32                  MediaId,
    IN  EFI_LBA                 LBA,
    IN  UINTN                   BufferSize,
    IN  VOID                    *Buffer
);

EFI_STATUS
AtaBlkFlush (
    IN  EFI_BLOCK_IO_PROTOCOL   *This
);

EFI_STATUS
AtapiReset (
    IN  EFI_BLOCK_IO_PROTOCOL   *This,
    IN  BOOLEAN                 ExtendedVerification
);

EFI_STATUS
AtapiBlkRead (
    IN  EFI_BLOCK_IO_PROTOCOL   *This,
    IN  UINT32                  MediaId,
    IN  EFI_LBA                 LBA,
    IN  UINTN                   BufferSize,
    OUT VOID                    *Buffer
);

EFI_STATUS
AtapiBlkWrite (
    IN  EFI_BLOCK_IO_PROTOCOL   *This,
    IN  UINT32                  MediaId,
    IN  EFI_LBA                 LBA,
    IN  UINTN                   BufferSize,
    IN  VOID                    *Buffer
);

EFI_STATUS
AtapiBlkFlush (
    IN  EFI_BLOCK_IO_PROTOCOL   *This
);

EFI_STATUS
DetectAtapiMedia (
    AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
);

EFI_STATUS
AtapiInquiryData (
    AMI_IDE_BUS_PROTOCOL *IdeBusInterface,
    UINT8            *InquiryData,
    IN OUT UINT16    *InquiryDataSize
);

EFI_STATUS
AtapiReadWritePio (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    VOID                        *Buffer,
    UINTN                       ByteCount,
    UINT64                      LBA,
    UINT8                       ReadWriteCommand,
    BOOLEAN                     READWRITE
);

EFI_STATUS
AtapiWritePio (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    VOID                        *Buffer,
    UINTN                       ByteCount,
    UINT64                      LBA,
    UINT8                       WriteCommand
);

EFI_STATUS 
AtaAtapiDmaDataCommand (
    IN AMI_IDE_BUS_PROTOCOL *IdeBusInterface,
    IN OUT VOID         *Buffer,
    IN UINTN            ByteCount,
    IN UINT8            Features,
    IN UINT32           SectorCountIn,
    IN UINT8            LbaLow,
    IN UINT8            LbaLowExp,
    IN UINT8            LbaMid,
    IN UINT8            LbaMidExp,
    IN UINT8            LbaHigh,
    IN UINT8            LbaHighExp,
    IN UINT8            Device,
    IN UINT8            Command,
    IN BOOLEAN          ReadWrite
);

EFI_STATUS
IssueAtapiPacketCommand (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    UINT16                      *PacketBuffer,
    UINT8                       Features,
    UINT16                      ByteCount
);

EFI_STATUS
IssueAtapiReset (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  BOOLEAN                 TESTUNITREADY
);

EFI_STATUS
HandleAtapiError (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
);

EFI_STATUS
DiskInfoInquiry (
    IN  EFI_DISK_INFO_PROTOCOL  *This,
    IN  OUT VOID                *InquiryData,
    IN  OUT UINT32              *InquiryDataSize
);

EFI_STATUS
DiskInfoIdentify (
    EFI_DISK_INFO_PROTOCOL  *This,
    IN  OUT VOID            *IdentifyData,
    IN  OUT UINT32          *IdentifyDataSize
);

EFI_STATUS
DiskInfoSenseData (
    EFI_DISK_INFO_PROTOCOL *This,
    VOID                   *SenseData,
    UINT32                 *SenseDataSize,
    UINT8                  *SenseDataNumber
);

EFI_STATUS 
DiskInfoWhichIDE (
    IN  EFI_DISK_INFO_PROTOCOL  *This,
    OUT UINT32                  *IdeChannel,
    OUT UINT32                  *IdeDevice
);

EFI_STATUS
GetOddType (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  OUT UINT16              *OddType
);

EFI_STATUS
GetOddLoadingType (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  OUT UINT8               *OddLoadingType
);

ODD_TYPE 
GetEnumOddType (
    IN  UINT16  OddType
);

BOOLEAN
Check48BitCommand (
    UINT8   Command
);

EFI_STATUS
StartStopUnitCommand (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  UINT8                   LoEjStart
);

EFI_STATUS
TestUnitReady (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
);

EFI_STATUS
CheckCHKonEntry (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
);

EFI_STATUS
ReadAtapiData (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    void                        *Data,
    UINT16                      *BytesRead
);

EFI_STATUS
WriteAtapiData (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    OUT void                    *Data,
    OUT UINT16                  *BytesRead
);

EFI_STATUS
GeneralAtapiCommandAndData (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    UINT8                       *PacketBuffer,
    UINT8                       *Buffer,
    UINT16                      *ByteCount
);

EFI_STATUS
AtaReadWriteBusMaster (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  OUT VOID                *Buffer,
    IN  UINTN                   ByteCount,
    IN  UINT64                  LBA,
    IN  UINT8                   ReadWriteCommand,
    IN  BOOLEAN                 ReadWrite
);

EFI_STATUS
AtapiReadWriteBusMaster (
    IN   AMI_IDE_BUS_PROTOCOL   *IdeBusInterface,
    IN  OUT VOID                *Buffer,
    IN  UINTN                   ByteCount,
    IN  UINT64                  LBA,
    IN  UINT8                   ReadWriteCommand,
    IN  BOOLEAN                 READWRITE 
);

EFI_STATUS
CreateDescriptorTable (
    IN  OUT UINTN               *DescriptorBuffer,
    IN  UINT8                   *StartAddress,
    IN  UINTN                   ByteCount,
    OUT UINTN                   *RemainingByteCount
);

EFI_STATUS
InitBusMasterRegisters (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  OUT UINTN               DescriptorBuffer,
    IN  BOOLEAN                 ReadWrite
);

EFI_STATUS
StartStopBusMastering (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  BOOLEAN                 StartStop
);

EFI_STATUS
WaitforDMAtoCompletion (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,
    IN  UINTN                   TimeDelay
);

BOOLEAN
DMACapable (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
);

EFI_STATUS
HandleATABMErrors (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
);

EFI_STATUS
MaskandSaveInterrupt (
    AMI_IDE_BUS_PROTOCOL        *IdeBusInterface
);

EFI_STATUS
RestoreInterrupt (
    AMI_IDE_BUS_PROTOCOL        *IdeBusInterface
);

void
PrintIdeDeviceInfo (
    AMI_IDE_BUS_PROTOCOL        *IdeBusInterface
);

//---------------------------------------------------------------------------
// IDE POWER MANAGEMENT SUPPORT START
//---------------------------------------------------------------------------


EFI_STATUS
InitIDEPowerManagement (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
);

UINT8
ConvertStanbyTimeoutValue (
    IN  UINT16                   StandbyTimeout
);

EFI_STATUS
InstallIDEPowerMgmtInterface (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
);

EFI_STATUS
StopIDEPowerMgmtInterface (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
);


// IDE POWER MANAGEMENT INTERFACE SUPPORT START
//---------------------------------------------------------------------------
  
EFI_STATUS
CheckPowerMode (
    IN  AMI_HDD_POWER_MGMT_PROTOCOL *IdePowerMgmtInterface,
    IN  OUT UINT8                   *PowerMode 
);

EFI_STATUS
IdleMode (
    IN  AMI_HDD_POWER_MGMT_PROTOCOL *IdePowerMgmtInterface,
    IN  UINT8                       StandbyTimeout
);

EFI_STATUS
StandbyMode (
    IN  AMI_HDD_POWER_MGMT_PROTOCOL *IdePowerMgmtInterface,
    IN  UINT8                       StandbyTimeout
);

EFI_STATUS
AdvancePowerMgmtMode (
    IN  AMI_HDD_POWER_MGMT_PROTOCOL *IdePowerMgmtInterface,
    IN  UINT8                       AdvPowerMgmtLevel
);

// IDE POWER MANAGEMENT INTERFACE SUPPORT END
//---------------------------------------------------------------------------
// IDE POWER MANAGEMENT SUPPORT END
//---------------------------------------------------------------------------

// HOST PROTECTED AREA SUPPORT START
//---------------------------------------------------------------------------


EFI_STATUS
InstallHpaInterface (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
);

EFI_STATUS
StopHpaInterface (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
);

EFI_STATUS
GetNativeMaxAddress (
    IN  AMI_HDD_HPA_PROTOCOL    *This,
    OUT UINT64                  *LBA
);

EFI_STATUS
SetMaxAddress (
    IN  AMI_HDD_HPA_PROTOCOL    *This,
    OUT UINT64                  LBA,
    IN  BOOLEAN                 Volatile
);

EFI_STATUS
HpaDisabledLastLBA (
    IN  AMI_HDD_HPA_PROTOCOL    *This,
    OUT UINT64                  *LBA
);

EFI_STATUS
SetMaxPassword (
    IN  AMI_HDD_HPA_PROTOCOL    *This,
    IN  UINT8                   *PasswordBuffer
);

EFI_STATUS
SetMaxLock (
    IN  AMI_HDD_HPA_PROTOCOL    *This
);

EFI_STATUS
SetMaxUnLock (
    IN  AMI_HDD_HPA_PROTOCOL    *This,
    IN  UINT8                   *PasswordBuffer
);

EFI_STATUS
SetMaxFreezeLock (
    IN  AMI_HDD_HPA_PROTOCOL    *This
);

EFI_STATUS
IssueSetMaxPasswordCmd (
    AMI_IDE_BUS_PROTOCOL        *IdeBusInterface,
    UINT8                       *PasswordBuffer,
    UINT8                       Cmd
);

// HOST PROTECTED AREA SUPPORT END
//---------------------------------------------------------------------------

// IDE HP SUPPORT START
//---------------------------------------------------------------------------

EFI_STATUS
CheckHPControllerPresence (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
);

EFI_STATUS
FlagDeviceRemoval (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
);

//  IDE HP SUPPORT END
//---------------------------------------------------------------------------
EFI_STATUS
IssueFreezeLockCommand (
    IN  AMI_IDE_BUS_PROTOCOL        *IdeBusInterface
);

EFI_STATUS
ReturnSecurityStatusWrapper (
    IN  AMI_HDD_SECURITY_PROTOCOL   *This,
    UINT16                          *SecurityStatus
);

EFI_STATUS
InstallIdeFeatures (
    IN  AMI_IDE_BUS_PROTOCOL        *IdeBusInterface
);

VOID
UnInstallIdeFeatures (
    IN  AMI_IDE_BUS_PROTOCOL        *IdeBusInterface
);


/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif

#endif

//***********************************************************************
//***********************************************************************
//**                                                                   **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.          **
//**                                                                   **
//**                       All Rights Reserved.                        **
//**                                                                   **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093         **
//**                                                                   **
//**                       Phone: (770)-246-8600                       **
//**                                                                   **
//***********************************************************************
//***********************************************************************
