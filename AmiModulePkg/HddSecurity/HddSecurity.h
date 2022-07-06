//***********************************************************************
//***********************************************************************
//**                                                                   **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.          **
//**                                                                   **
//**                       All Rights Reserved.                        **
//**                                                                   **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093         **
//**                                                                   **
//**                       Phone: (770)-246-8600                       **
//**                                                                   **
//***********************************************************************
//***********************************************************************

/** @file HddSecurity.h
    This file contains the Includes, Definitions, typedefs,
    Variable and External Declarations, Structure and
    function prototypes needed for the IdeSecurity driver

**/

#ifndef _HDD_SECURITY_H_
#define _HDD_SECURITY_H_

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
#if ( defined(IdeBusSrc_SUPPORT) && (IdeBusSrc_SUPPORT != 0) )
#include <Protocol/AmiIdeBus.h>
#endif
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
#include <Protocol/AmiAhciBus.h>
#endif
#include <Protocol/AmiHddSecurity.h>
#include <Protocol/StorageSecurityCommand.h>
#include <Protocol/ComponentName.h>
#include <Protocol/BootScriptSave.h>
#include <Protocol/SmmBase.h>

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
#include <Protocol/AhciSmmProtocol.h>
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
#include "Nvme/NvmeController.h"
#include <Protocol/AmiNvmeController.h>
#include <Protocol/NvmExpressPassthru.h>
#include <Nvme/NvmePassthru.h>
#endif

#include <Protocol/SmmCommunication.h>
#include "HddSecurityCommon.h"

//---------------------------------------------------------------------------
#define HDD_SEC_SIGNATURE  SIGNATURE_32 ('$','H','D','D')
#define HDD_SEC_SMI_UNREGISTER_SIGNATURE  SIGNATURE_64 ('S', 'M', 'I', 'U', 'N', 'R','E','G')

#define IDE_SECURITY_PWNV_GUID \
    { 0x69967a8c, 0x1159, 0x4522, 0xaa, 0x89, 0x74, 0xcd, 0xc6, 0xe5, 0x99, 0xa0}

#define EFI_SMM_SAVE_HDD_PASSWORD_GUID \
    { 0xeedcf975, 0x4dd3, 0x4d94, 0x96, 0xff, 0xaa, 0xca, 0x83, 0x53, 0xb8, 0x7b }

#define EFI_SMM_REMOVE_HDD_PASSWORD_GUID \
    { 0xc2b1e795, 0xf9c5, 0x4829, 0x8a, 0x42, 0xc0, 0xb3, 0xfe, 0x57, 0x15, 0x17 }

#define AMI_LOCATE_IDE_AHCI_PROTOCOL_GUID \
    { 0xb3f096e9, 0x2d46, 0x4e8e, 0xa2, 0x2c, 0x7d, 0xe8, 0xb1, 0x6b, 0x3a, 0x5b }

#define AMI_HDD_SECURITY_BOOTSCRIPT_SAVE_TRIGGER_GUID \
    { 0x60b0760c, 0x7d1b, 0x43f3, 0x95, 0x25, 0x60, 0x77, 0xbe, 0x41, 0x37, 0xe2 }

#define AMI_NVME_SECURITY_BOOTSCRIPT_SAVE_TRIGGER_GUID \
    { 0x3fb7e17f, 0x1172, 0x4e2a, 0x9a, 0x25, 0xba, 0x5f, 0xe6, 0x2c, 0xc7, 0xc8 }

#ifndef AMITSE_EVENT_BEFORE_BOOT_GUID
#define AMITSE_EVENT_BEFORE_BOOT_GUID \
  { 0x3677770f, 0xefb2, 0x43b2, 0xb8, 0xae, 0xb3, 0x2, 0xe9, 0x60, 0x48, 0x82 }
#endif


// Size of SMM communicate header, without including the Data.
#define SMM_COMMUNICATE_HEADER_SIZE  (OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data))

// Size of HDD Password DATA size
#define HDD_PASSWORD_SIZE  sizeof(HDD_PASSWORD)
#ifndef     SATA_COMMAND_TIMEOUT
#define SATA_COMMAND_TIMEOUT                    5   // 5 Sec
#endif
#ifndef     NVME_COMMAND_TIMEOUT
#define     NVME_COMMAND_TIMEOUT            0x01
#endif
EFI_GUID gSaveHddPasswordGuid=EFI_SMM_SAVE_HDD_PASSWORD_GUID;
EFI_GUID gRemoveHddPasswordGuid=EFI_SMM_REMOVE_HDD_PASSWORD_GUID;
EFI_GUID gLocateIdeAhciSmmProtocolGuid=AMI_LOCATE_IDE_AHCI_PROTOCOL_GUID;
EFI_GUID gHddSecurityBootScriptSaveTriggerGuid=AMI_HDD_SECURITY_BOOTSCRIPT_SAVE_TRIGGER_GUID;
EFI_GUID gNvmeSecurityBootScriptSaveTriggerGuid=AMI_NVME_SECURITY_BOOTSCRIPT_SAVE_TRIGGER_GUID;

#define HDD_PWD_ENCRYPTION_KEY      "H?p1mA*k920_84o3d^!z@L.x4$kY64"

#define EFI_SEGMENT( _Adr )     (UINT16) ((UINT16) (((UINTN) (_Adr))\
                                                    >> 4) & 0xf000)
#define EFI_OFFSET( _Adr )      (UINT16) (((UINT16) ((UINTN) (_Adr))) & 0xffff)

#define     ZeroMemory( Buffer, Size ) pBS->SetMem( Buffer, Size, 0 )

#define  NVME_ATA_SECURITY_SET_PASSWORD         0x1
#define  NVME_ATA_SECURITY_UNLOCK               0x2
#define  NVME_ATA_SECURITY_ERASE_PREPARE        0x3
#define  NVME_ATA_SECURITY_ERASE_UNIT           0x4
#define  NVME_ATA_SECURITY_FREEZE_LOCK          0x5
#define  NVME_ATA_SECURITY_DISABLE_PASSWORD     0x6

//
// Storage Security Protocol
//
#define SECURITY_PROTOCOL_INFORMATION           0x00
#define SECURITY_PROTOCOL_1                     0x01
#define SECURITY_PROTOCOL_2                     0x02
#define SECURITY_PROTOCOL_SDCARD_TRUSTED_FLASH  0xED
#define SECURITY_PROTOCOL_IEEE1667              0xEE
#define NVME_SECUIRTY_SECP                      0xEF
#define NVME_SECURITY_RECEIVE_BUFFER_SIZE       0x10
#define NVME_SECURITY_SEND_BUFFER_SIZE          0x24
#pragma pack(1)

typedef enum 
{
SavePassword,
RemovePassword
} HDD_SECURITY_PASSWORD_ACTION;

typedef 
struct _SP0_TRUSTED_RECEIVE_PARAMETER_DATA { 
    UINT8      Reserved[6]; 
    UINT8      ListLengthHigh; 
    UINT8      ListLengthLow; 
    UINT8      SupportedSPList[502]; 
    UINT8      PadBytesHigh; 
    UINT8      PadBytesLow; 
}SP0_TRUSTED_RECEIVE_PARAMETER_DATA;

typedef struct _HDDSECDATA
{
    UINT16 UserMaster;           // Bit 0 - MasterPwd Presence
    						     // Bit 1 - UserPwd Presence
    UINT16 UserPasswordLength;
    UINT16 MasterPasswordLength;
    UINT8  HddUserPassword[IDE_PASSWORD_LENGTH];
    UINT8  HddMasterPassword[IDE_PASSWORD_LENGTH];
} HDDSECDATA;

#pragma pack()

UINT16
ToBigLittleEndianWord (
    UINT16 EndianWord
);

EFI_STATUS
EFIAPI
InstallSecurityInterface (
    IN  VOID                       *BusInterface,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
);

EFI_STATUS
EFIAPI
StopSecurityModeSupport (
    IN  VOID                       *BusInterface,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
);

EFI_STATUS
EFIAPI
ReturnSecurityStatus (
    AMI_HDD_SECURITY_PROTOCOL   *This,
    UINT16                      *SecurityStatus
);

EFI_STATUS
EFIAPI
SecuritySetPassword (
    AMI_HDD_SECURITY_PROTOCOL   *This,
    UINT16                      Control,
    UINT8                       *Buffer,
    UINT16                      RevisionCode
);

EFI_STATUS
EFIAPI
SecurityUnlockPassword (
    AMI_HDD_SECURITY_PROTOCOL   *This,
    UINT16                      Control,
    UINT8                       *Buffer
);

EFI_STATUS
EFIAPI
SecurityDisablePassword (
    IN  AMI_HDD_SECURITY_PROTOCOL *This,
    UINT16                        Control,
    UINT8                         *Buffer
);

EFI_STATUS
SetDefaultMasterPassword (
    IN  AMI_HDD_SECURITY_PROTOCOL *This
);

EFI_STATUS
SecuritySetDefaultMasterPassword (
    IN  AMI_HDD_SECURITY_PROTOCOL *This,
    UINT16                        Control,
    UINT8                         *Buffer,
    UINT16                        RevisionCode
);

EFI_STATUS
EFIAPI
SecurityFreezeLock (
    IN  AMI_HDD_SECURITY_PROTOCOL *This
);

EFI_STATUS
EFIAPI
SecurityEraseUnit (
    IN AMI_HDD_SECURITY_PROTOCOL *This,
    UINT16                       Control,
    UINT8                        *Buffer
);

EFI_STATUS
EFIAPI
ReturnIdePasswordFlags (
    IN  AMI_HDD_SECURITY_PROTOCOL   *This,
    UINT32                          *IdePasswordFlags
);

EFI_STATUS
SecurityCommonRoutine (
    IN  VOID                   *IdeBusInterface,
    UINT16                     Control,
    UINT8                      *Buffer,
    UINT8                      SecurityCommand,
    UINT16                     RevisionCode,
    AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
);

EFI_STATUS 
IsSecurityProtocolSupported (
    IN     VOID     *BusInterface,
    IN     AMI_STORAGE_BUS_INTERFACE  BusInterfaceType,
    OUT    BOOLEAN  *SAT3Supported,
    OUT    BOOLEAN  *TcgSupported
);

EFI_STATUS 
IsNvmeSecurityProtocolSupported (
        IN     VOID     *BusInterface,
        OUT    BOOLEAN  *SAT3Supported,
        OUT    BOOLEAN  *TcgSupported
);

EFI_STATUS 
GetNvmeSecurityProtocolInformation (
    IN     VOID     *BusInterface,
    IN OUT VOID     *Buffer,
    IN OUT UINT32    BufferLength 
);

EFI_STATUS
GetNvmeDeviceSecurityData (
     VOID        *BusInterface,
     UINT8       *Buffer,
     UINT32      Bufferlength
);

EFI_STATUS
NvmeSecurityRecieveRoutine(
    VOID        *BusInterface,
    UINT8       *Buffer,
    UINT32       Bufferlength,
    UINT32      CDW10_Value
);

EFI_STATUS
NvmeSecuritySendCommandCommonRoutine (
    IN  VOID                   *BusInterface,
    UINT16                     Control,
    UINT8                      *Buffer,
    UINT8                      SecurityCommand,
    UINT16                     RevisionCode,
    AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
);

EFI_STATUS
ConnectController (
    IN  EFI_HANDLE                                   ControllerHandle,
    IN  EFI_HANDLE*DriverImageHandle                 OPTIONAL,
    IN  EFI_DEVICE_PATH_PROTOCOL*RemainingDevicePath OPTIONAL,
    IN  BOOLEAN                                      Recursive
);

EFI_STATUS
GatherIdeInfo (
    IN  VOID                       *BusInterface,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType,
    OUT UINT32                     *HddDataId
);

EFI_STATUS
UpdateIdentifyData (
    IN  VOID                       *BusInterface,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
);

EFI_STATUS
SetHddSecDataInNvram (
    IN  VOID    *BusInterface,
    UINT32      *HddDataId,
    UINT16      Control,
    UINT8       *Buffer
);

EFI_STATUS 
GetHddSecDataFromNvram(
    IN  VOID                        *BusInterface,
    IN  AMI_STORAGE_BUS_INTERFACE   BusInterfaceType,
    IN  UINT16                      Control,
    OUT UINT8                       *Buffer 
);

EFI_STATUS
TransferPwDataFromNvramToSmram (
    IN  AMI_HDD_SECURITY_PROTOCOL   *This
);

VOID
ConvertHddDataIdToString (
    IN  UINT32  DataId,
    OUT CHAR16  *String
);

VOID
EncodeDecodePassword (
    IN  UINT8   *InputString,
    OUT UINT8   *OutputString,
    IN  UINT32  StringLength
);

EFI_STATUS
OEMSetMasterPassword (
    IN  AMI_HDD_SECURITY_PROTOCOL   *This
);

EFI_STATUS
SmmHDDPasswordInterface (
    IN  AMI_HDD_SECURITY_PROTOCOL   *This,
    UINT16                          Control,
    UINT8                           *Buffer,
    UINT8                           Action
);

VOID
EFIAPI
StoreUnlockSmiContextInBootScript (
    IN  EFI_EVENT Event,
    IN  VOID      *Context
);

VOID
IdeBusMiscSmmFeatureCallback (
    IN  EFI_EVENT   Event,
    IN   VOID       *Context
);

UINTN
EfiValueToString (
    IN  OUT CHAR16 *Buffer,
    IN  INT64      Value,
    IN  UINTN      Flags,
    IN  UINTN      Width
);

EFI_STATUS
CommonNonDataHook (
    IN  VOID                       *BusInterface,
    IN  COMMAND_STRUCTURE          CommandStructure,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
);

EFI_STATUS
CommonPioDataHook (
    IN  VOID                       *BusInterface,
    IN  COMMAND_STRUCTURE          CommandStructure,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
);

EFI_STATUS
CommonReadWritePioHook (
    IN  VOID                       *BusInterface,
    IN  OUT VOID                   *Buffer,
    IN  UINTN                      ByteCount,
    IN  UINT64                     LBA,
    IN  UINT8                      ReadWriteCommand,
    IN  BOOLEAN                    ReadWrite,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
);

EFI_STATUS
CommonWfccHook (
    IN  VOID                       *BusInterface,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
);

VOID
EFIAPI
LocateAhciSmmServiceEvent (
    EFI_EVENT   Event,
    VOID        *Context
);

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif

#endif

//***********************************************************************
//***********************************************************************
//**                                                                   **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.          **
//**                                                                   **
//**                       All Rights Reserved.                        **
//**                                                                   **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093         **
//**                                                                   **
//**                       Phone: (770)-246-8600                       **
//**                                                                   **
//***********************************************************************
//***********************************************************************
