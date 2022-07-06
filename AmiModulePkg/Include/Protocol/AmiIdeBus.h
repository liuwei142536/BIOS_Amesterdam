//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2014, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file AmiIdeBus.h
AMI defined Protocol header file for the PATA and SATA Controllers in IDE mode
 **/

#ifndef _AMI_IDEBUS_PROTOCOL_H
#define _AMI_IDEBUS_PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------

#include <Library/UefiLib.h>
#include <Protocol/IdeControllerInit.h>
#include <Protocol/AmiHddSecurity.h>
#include <Protocol/AmiHddSmart.h>
#include <Protocol/AmiHddPowerMgmt.h>
#include <Protocol/AmiHddHpa.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/StorageSecurityCommand.h>

//---------------------------------------------------------------------------

#define IDE_SETUP_PROTOCOL_GUID\
        { 0x5578ae16, 0xf1c9, 0x4e8f, { 0xb1, 0x29, 0xba, 0x07, 0xf8, 0xfc, 0xf8, 0x4a } }

#define     MASTER_DRIVE        0
#define     SLAVE_DRIVE         1
#define     PRIMARY_CHANNEL     0
#define     SECONDARY_CHANNEL   1

/// Set bit 1 in Flags only if HDD loses power in S3 state.
#define     S3_RESUME_UNLOCK_HDD_PASSWORD       0x02     

/*
 * IDE Bus Master status Register
 */
#define     BM_INTERRUPT    0x4
#define     BM_ERROR        0x2
#define     BM_ACTIVE       0x1

/**
 * IDE Bus Master Command Register
 */

#define     BM_START_STOP       0x1
#define     DMA_MEMORY_WRITE    0x08
#define     DMA_MEMORY_READ     0x00

typedef struct _AMI_HDD_POWER_MGMT_INTERFACE AMI_HDD_POWER_MGMT_INTERFACE;
typedef struct _AMI_HDD_HPA_INTERFACE AMI_HDD_HPA_INTERFACE;
typedef struct _AMI_IDE_BUS_PROTOCOL AMI_IDE_BUS_PROTOCOL;


/**
 * ATA & ATAPI Specification Defined Structures.
 */

#pragma pack(1)

typedef struct { 
    UINT16          BMCommandRegister;
    UINT16          Reserved;
    UINT16          BMStatusRegister;
    UINT16          BMDescriptorTablePointerReg;
} BUS_MASTER_IO;

typedef struct { 
    UINT32          BaseAddress;
    UINT16          ByteCount;
    UINT16          Flag;
} BUS_MASTER_DESCRIPTOR_TABLE;

typedef struct { 
    UINT16          DataReg;
    UINT16          FeatureReg;
    UINT16          ErrorReg;
    UINT16          SectorCountReg;
    UINT16          LBALowReg;
    UINT16          LBAMidReg;
    UINT16          LBAHighReg;
    UINT16          DeviceReg;
    UINT16          CommandReg;
    UINT16          StatusReg;
} COMMAND_BLOCK;

typedef struct { 
    UINT16          DeviceControlReg;
    UINT16          AlternateStatusReg;
} CONTROL_BLOCK;

typedef struct { 
    COMMAND_BLOCK   CommandBlock;
    CONTROL_BLOCK   ControlBlock;
    BUS_MASTER_IO   BusMasterBlock;
} IO_REGS;

#pragma pack()

/**
 * IDE Setup Protocol Definition
 */

typedef struct { 
    UINT8           AcousticPwrMgmt;
    UINT8           AcousticLevel;
    UINT8           Dipm;
} AMI_IDE_SETUP_PROTOCOL;

typedef struct _IDE_DISK_INFO { 
    EFI_DISK_INFO_PROTOCOL              DiskInfo;               
    AMI_IDE_BUS_PROTOCOL                *IdeBusInterface;
} IDE_DISK_INFO;

typedef struct _IDE_BLOCK_IO { 
    EFI_BLOCK_IO_PROTOCOL               BlkIo;                  
    AMI_IDE_BUS_PROTOCOL                *IdeBusInterface;
} IDE_BLOCK_IO;

/**
 * AMI Ide Bus Init Protocol definition
 */

typedef struct _AMI_IDE_BUS_INIT_PROTOCOL { 
    UINT8                               IdeBusInitData[2][2];
    AMI_IDE_BUS_PROTOCOL                *pIdeBusProtocol[2][2];
    EFI_IDE_CONTROLLER_INIT_PROTOCOL    *EfiIdeControllerInterface;
    UINT8                               BusMasterSelection;
    UINT8                               HPMask;
    EFI_EVENT                           HPEvent;
    UINT8                               Flags;
    UINT8                               Acoustic_Management_Level;
    UINT8                               Dipm;
    UINT16                              PrevPortNum;
    UINT16                              PrevPortMultiplierPortNum;
} AMI_IDE_BUS_INIT_PROTOCOL;

typedef struct _IDE_DEVICE_INTERFACE {
    IDENTIFY_DATA                       IdentifyData;
    UINT8                               Channel;
    UINT8                               Device;
    DEVICE_TYPE                         DeviceType;
    UINT8                               PIOMode;
    UINT8                               SWDma;
    UINT8                               MWDma;
    UINT8                               UDma;
    UINT8                               IORdy;
    UINT8                               ReadCommand;
    UINT8                               WriteCommand;
    IO_REGS                             Regs;
    EFI_PCI_IO_PROTOCOL                 *PciIO;
    EFI_UNICODE_STRING_TABLE            *UDeviceName;
    ATAPI_DEVICE                        *AtapiDevice;
} IDE_DEVICE_INTERFACE;

/**
 * IDE BUS Protocol Definition.
 */
 
 /**
    Issue a Soft Reset
        
    @param IdeBusInterface 
    @retval 
        EFI_STATUS

**/
typedef 
EFI_STATUS 
(*AMI_IDE_SOFT_RESET) (
    IN AMI_IDE_BUS_PROTOCOL         *This
);

/**
    Issues Read/Write Command and Read/Write the data from/to the ATA device

    @param This     IdeBus Protocol Interface 
    @param Buffer   Data Buffer
    @param ByteCount Number of Bytes to read/write
    @param LBA      Lba Address
    @param ReadWriteCommand Command
    @param ReadWrite Read/Write = 0/1

    @retval *Buffer

    @note  
  1. Check if Multiple sectors can be read/written to the ATA device.
  2. Check for 48-bit LBA support.
  3. Issue the command based on step 1 and step 2 results.
  4. check for ERRORs.
  5. If success read/write data.
  6. Based on step 1 results, complete the read/write sequence
  7. If all sectors are not completed, goto step 3.
**/

typedef 
EFI_STATUS
(*AMI_IDE_ATA_RAED_WRITE_PIO) (
    IN  AMI_IDE_BUS_PROTOCOL    *This,
    IN  OUT VOID                *Buffer,
    IN  UINTN                   ByteCount,
    IN  UINT64                  LBA,
    IN  UINT8                   ReadWriteCommand,
    IN  BOOLEAN                 ReadWrite
);

/**
    Issues command which require data to be read

    @param This     IdeBus Protocol Interface 
    @param Buffer   Data Buffer
    @param ByteCount Number of bytes to read/write
    @param SectorCount Sector Count
    @param LBALow 
    @param LBAMid 
    @param LBAHigh 
    @param Device 
    @param Command 
    @param Multiple // to determine the block size

    @retval *Buffer

    @note  
      Used to get Identify command data etc.
  1. Issue the command
  2. Check for ERRORs.
  3. Check if Data is ready. If yes, read it else return ERROR.

**/
typedef 
EFI_STATUS
(*AMI_IDE_ATA_PIO_DATA_IN) (
    IN  AMI_IDE_BUS_PROTOCOL    *This,
    OUT VOID                    *Buffer,
    IN  UINT32                  ByteCount,
    IN  UINT8                   Features,
    IN  UINT8                   SectorCount,
    IN  UINT8                   LBALow,
    IN  UINT8                   LBAMid,
    IN  UINT8                   LBAHigh,
    IN  UINT8                   Device,
    IN  UINT8                   Command,
    IN  BOOLEAN                 Multiple
);

/**
    Issues Read/Write Command and Read/Write the data from/to the ATA device
    with SubCommand Support.
        
    @param This     IdeBus Protocol Interface 
    @param Buffer   Data Buffer
    @param ByteCount Number of bytes to read/write
    @param Features 
    @param SectorCountIn 
    @param LBALow 
    @param LBALowExp 
    @param LBAMid 
    @param LBAMidExp 
    @param LBAHigh 
    @param LBAHighExp 
    @param Device 
    @param Command 
    @param ReadWrite // Read/Write = 0/1
    @param Multiple // to determine the block size

    @retval *Buffer

    @note  
  1. Check if Multiple sectors can be read/written to the ATA device.
  2. Check for 48-bit LBA support.
  3. Issue the command based on step 1 and step 2 results.
  4. check for ERRORs.
  5. If success read/write data.
  6. Based on step 1 results, complete the read/write sequence
  7. If all sectors are not completed, goto step 3.

**/
typedef 
EFI_STATUS 
(*AMI_IDE_ATA_PIO_DATA_OUT) (
    IN  AMI_IDE_BUS_PROTOCOL    *This,
    IN  OUT VOID                *Buffer,
    IN  UINTN                   ByteCount,
    IN  UINT8                   Features,
    IN  UINT32                  SectorCountIn,
    IN  UINT8                   LBALow,
    IN  UINT8                   LBALowExp,
    IN  UINT8                   LBAMid,
    IN  UINT8                   LBAMidExp,
    IN  UINT8                   LBAHigh,
    IN  UINT8                   LBAHighExp,
    IN  UINT8                   Device,
    IN  UINT8                   Command,
    IN  BOOLEAN                 ReadWrite,
    IN  BOOLEAN                 Multiple
);


/**
    Issues ATA/ATAPI Command using Bus master

    @param IdeBusInterface 
    @param VOID     *Buffer,
    @param UINT32   ByteCount,
    @param UINT8    Features,
    @param UINT32   SectorCountIn,
    @param UINT8    LbaLow,
    @param UINT8    LbaLowExp,
    @param UINT8    LbaMid,
    @param UINT8    LbaMidExp,
    @param UINT8    LbaHigh,
    @param UINT8    LbaHighExp,
    @param UINT8    Device,
    @param UINT8    Command,
    @param BOOLEAN  ReadWrite

    @retval *Buffer

**/
typedef 
EFI_STATUS 
(*AMI_IDE_ATA_ATAPI_DMA_DATA) (
    IN AMI_IDE_BUS_PROTOCOL     *This,
    IN OUT VOID                 *Buffer,
    IN UINTN                    ByteCount,
    IN UINT8                    Features,
    IN UINT32                   SectorCountIn,
    IN UINT8                    LBALow,
    IN UINT8                    LBALowExp,
    IN UINT8                    LBAMid,
    IN UINT8                    LBAMidExp,
    IN UINT8                    LBAHigh,
    IN UINT8                    LBAHighExp,
    IN UINT8                    Device,
    IN UINT8                    Command,
    IN BOOLEAN                  ReadWrite
);

/**
    Issues command where no data transfer takes place

    @param This     IdeBus Protocol Interface 
    @param Buffer   Data Buffer
    @param ByteCount Number of bytes to read/write
    @param SectorCount 
    @param LBALow 
    @param LBAMid 
    @param LBAHigh 
    @param Device 
    @param Command 

    @retval *Buffer

**/
typedef 
EFI_STATUS 
(*AMI_IDE_NON_DATA_CMD) (
    IN  AMI_IDE_BUS_PROTOCOL    *This,
    IN  UINT8                   Features,
    IN  UINT8                   SectorCount,
    IN  UINT8                   SectorCountExp,
    IN  UINT8                   LBALow,
    IN  UINT8                   LBALowExp,
    IN  UINT8                   LBAMid,
    IN  UINT8                   LBAMidExp,
    IN  UINT8                   LBAHigh,
    IN  UINT8                   LBAHighExp,
    IN  UINT8                   Device,
    IN  UINT8                   Command
    
);

/**
    Waits for Ide Busy bit to get clear
        
    @param IdeBusInterface 

    @retval EFI_STATUS

    @note  Wait for IDE_BSY bit to get clear. Check for any ERRORs.

**/
typedef 
EFI_STATUS 
(*AMI_WAIT_FOR_CMD_COMPLETE) (
    IN  AMI_IDE_BUS_PROTOCOL        *This
);

/**
    Issues the ATAPI command and reads the data

    @param IdeBusInterface 
    @param PacketBuffer 
    @param Buffer 
    @param ByteCount 

    @retval EFI_STATUS

**/
typedef 
EFI_STATUS 
(*AMI_GENERAL_ATAPI_COMMAND_DATA) (
    IN   AMI_IDE_BUS_PROTOCOL   *IdeBusInterface,
    UINT8                       *PacketBuffer,
    UINT8                       *Buffer,
    IN  OUT UINT16              *ByteCount 
);

struct _AMI_IDE_BUS_PROTOCOL{ 
    IDE_BLOCK_IO                          *IdeBlkIo;
    IDE_DISK_INFO                         *IdeDiskInfo;
    EFI_DEVICE_PATH_PROTOCOL              *DevicePathProtocol;
    EFI_IDE_CONTROLLER_INIT_PROTOCOL      *EfiIdeControllerInterface;
    EFI_HANDLE                            IdeControllerInterfaceHandle;
    AMI_HDD_SECURITY_PROTOCOL             *IdeSecurityInterface;
    AMI_HDD_SMART_PROTOCOL                *SmartInterface;
    AMI_HDD_POWER_MGMT_INTERFACE          *PowerMgmtInterface;
    AMI_HDD_HPA_INTERFACE                 *HpaInterface;
    EFI_STORAGE_SECURITY_COMMAND_PROTOCOL *StorageSecurityInterface;
    AMI_IDE_BUS_INIT_PROTOCOL             *IdeBusInitInterface;
    EFI_PCI_IO_PROTOCOL                   *PciIO;
    EFI_HANDLE                            IdeDeviceHandle;
    IDE_DEVICE_INTERFACE                  IdeDevice;
    BOOLEAN                               IsDeviceFeatureDone;
    EFI_UNICODE_STRING_TABLE              *ControllerNameTable;
    UINT8                                 AtapiSenseData[256];
    UINT8                                 AtapiSenseDataLength;
    AMI_IDE_SOFT_RESET                    IdeSoftReset;
    AMI_IDE_ATA_RAED_WRITE_PIO            AtaReadWritePio;
    AMI_IDE_ATA_PIO_DATA_IN               AtaPioDataIn;
    AMI_IDE_ATA_PIO_DATA_OUT              AtaPioDataOut;
    AMI_IDE_ATA_ATAPI_DMA_DATA            AtaAtapiDmaDataCommand;
    AMI_IDE_NON_DATA_CMD                  IdeNonDataCommand;
    AMI_WAIT_FOR_CMD_COMPLETE             WaitForCmdCompletion;
    AMI_GENERAL_ATAPI_COMMAND_DATA        GeneralAtapiCommandAndData;
};

struct _AMI_HDD_POWER_MGMT_INTERFACE {
    AMI_HDD_POWER_MGMT_CHECK_POWER_MODE         CheckPowerMode;
    AMI_HDD_POWER_MGMT_IDLE_MODE                IdleMode;
    AMI_HDD_POWER_MGMT_STANDBY_MODE             StandbyMode;
    AMI_HDD_POWER_MGMT_ADVANCE_POWER_MGMT_MODE  AdvancePowerMgmtMode;
    AMI_IDE_BUS_PROTOCOL                        *IdeBusInterface;
};

struct  _AMI_HDD_HPA_INTERFACE {
    AMI_HDD_HPA_GET_NATIVE_MAX_ADDRESS      GetNativeMaxAddress;
    AMI_HDD_HPA_SET_MAX_ADDRESS             SetMaxAddress;
    AMI_HDD_HPA_DISABLED_LAST_LBA           HpaDisabledLastLBA;
    AMI_HDD_HPA_SET_MAX_PASSWORD            SetMaxPassword;
    AMI_HDD_HPA_SET_MAX_LOCK                SetMaxLock;
    AMI_HDD_HPA_SET_MAX_UNLOCK              SetMaxUnLock;
    AMI_HDD_HPA_SET_MAX_FREEZE_LOCK         SetMaxFreezeLock;
    AMI_IDE_BUS_PROTOCOL                    *IdeBusInterface;
};

extern  EFI_GUID    gAmiIdeBusInitProtocolGuid;

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif

#endif

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2014, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
