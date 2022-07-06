//***********************************************************************
//***********************************************************************
//**                                                                   **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.          **
//**                                                                   **
//**                       All Rights Reserved.                        **
//**                                                                   **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093         **
//**                                                                   **
//**                       Phone: (770)-246-8600                       **
//**                                                                   **
//***********************************************************************
//***********************************************************************

/** @file TcgStorageSec.h
    This file contains the Includes, Definitions, typedefs,
    Variable and External Declarations, Structure and
    function prototypes needed for TcgStorageSecurity driver

**/

#ifndef _TCG_STORAGE_SEC_H_
#define _TCG_STORAGE_SEC_H_

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------

#include <Token.h>

#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/BlockIo.h>
#include <Protocol/ComponentName.h>

#include <Library/TimerLib.h>
#include "TcgStorageSecOPAL.h"
#include "TcgStorageSecurityCommon.h"
#include <Protocol/SmmControl2.h>

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) ) || ((AHCI_SUPPORT == 0) && (NVMe_SUPPORT == 0))
#include <Protocol/SmmCommunication.h>
#endif

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
#include <Protocol/AhciSmmProtocol.h>
#endif

#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
#include "Nvme/NvmeIncludes.h"
#include "Nvme/NvmeController.h"
#include "Nvme/NvmeBus.h"
#include <Protocol/AmiNvmeController.h>
#include <Nvme/NvmePassthru.h>
#endif

#define TCG_PWD_ENCRYPTION_KEY      "H?p1mA*k920_84o3d^!z@L.x4$kY64"

// Size of SMM communicate header, without including the Data.
#define SMM_COMMUNICATE_HEADER_SIZE  (OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data))

#define OPAL_PASSWORD_SIGNATURE SIGNATURE_32 ('$','O','P','L')
#define TCG_SMI_UNREGISTER_SIGNATURE  SIGNATURE_64 ('S', 'M', 'I', 'U', 'N', 'R','E','G')

// AMITSE related defines
#define TCG_STORAGE_SECURITY_PASSWORD_SIZE      sizeof(TCG_STORAGE_SECURITY_PASSWORD) // Size of OPAL Password DATA size

#pragma pack(1)

typedef struct _TCGSEC_NVRAM_DATA TCGSEC_NVRAM_DATA;

struct _TCGSEC_NVRAM_DATA {
    UINT16  UserMaster;
    UINT32  PasswordLength;
    UINT8   TcgUserPassword[OPAL_PASSWORD_LENGTH];
    UINT8   TcgMasterPassword[OPAL_PASSWORD_LENGTH];
} ;

#pragma pack()

typedef enum {
  SavePassword        = 1,
  RemovePassword,
  SaveDeviceInfo
} TCG_STORAGE_SEC_PASSWORD_ACTION;

// Protocol Function Prototypes

EFI_STATUS
EFIAPI
TcgStorageSecEntryPoint (
    IN  EFI_HANDLE        ImageHandle,
    IN  EFI_SYSTEM_TABLE  *SystemTable
);

EFI_STATUS
EFIAPI
TcgInstallSecurityInterface (
    IN  VOID                       *BusInterface,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
);

#if ENABLE_DEVICE_RESET_THRU_PSID
EFI_STATUS
EFIAPI
TcgStoragePsidRevert (
    IN AMI_TCG_STORAGE_DEVICE_RESET_PROTOCOL    *This,
    IN UINT8                                    *PsidBuffer
);
#endif

EFI_STATUS
EFIAPI
TcgStopSecurityModeSupport (
    IN  VOID                       *BusInterface,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
);

EFI_STATUS
EFIAPI
TcgReturnSecurityStatus (
    IN  AMI_HDD_SECURITY_PROTOCOL   *This,
    IN OUT UINT16                      *SecurityStatus
);

EFI_STATUS
EFIAPI
TcgSecuritySetPassword (
    IN  AMI_HDD_SECURITY_PROTOCOL   *This,
    IN  UINT16                      Control,
    IN  UINT8                       *Buffer,
    IN  UINT16                      RevisionCode
);

EFI_STATUS
EFIAPI
TcgSecurityUnlockPassword (
    IN  AMI_HDD_SECURITY_PROTOCOL   *This,
    IN  UINT16                      Control,
    IN  UINT8                       *Buffer
);

EFI_STATUS
EFIAPI
TcgSecurityDisablePassword (
    IN  AMI_HDD_SECURITY_PROTOCOL   *This,
    IN  UINT16                      Control,
    UINT8                           *Buffer
);

EFI_STATUS
EFIAPI
TcgSecurityBlockSid (
    IN  AMI_HDD_SECURITY_PROTOCOL   *This
);

EFI_STATUS
EFIAPI
TcgSecurityEraseUnit (
    IN AMI_HDD_SECURITY_PROTOCOL    *This,
    IN UINT16                       Control,
    IN UINT8                        *Buffer
);

EFI_STATUS
EFIAPI
TcgReturnIdePasswordFlags (
    IN  AMI_HDD_SECURITY_PROTOCOL   *This,
    IN OUT UINT32                   *PasswordFlags
);

// Other Function Prototypes

EFI_STATUS
UpdateLevel0Data(
    IN  VOID                       *BusInterface,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
);

EFI_STATUS
InitializeOpalConfig(
    IN  VOID                       *BusInterface,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
);

EFI_STATUS
GetLevel0DiscoveryData(
    IN  OPAL_DEVICE         *OpalDevice
);

EFI_STATUS
DisableOpalAdminPassword (
    IN  OPAL_DEVICE     *OpalDevice,
    IN  UINT8           *Buffer
);

VOID
UpdatePrintLevel0DiscoveryData(
    IN  OPAL_DEVICE             *OpalDevice,
    IN  LEVEL0_DISCOVERY_DATA   *QueryBuffer
);

UINT16
GetOpalSecurityStatus(
    IN  VOID                       *BusInterface,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
);

EFI_STATUS
SetAdminPassword (
   IN   OPAL_DEVICE         *OpalDevice,
   IN   UINT8               *AdminPassword
);

EFI_STATUS
SetUserPassword (
   IN   OPAL_DEVICE         *OpalDevice,
   IN   UINT8               *PasswordBuffer
);

EFI_STATUS
InitAdminSPCPinSid (
   IN   OPAL_DEVICE         *OpalDevice,
   IN   UINT8               *OldPassword,
   IN   UINT8               *AdminPassword
);

EFI_STATUS
ActivateLockingSP (
   IN   OPAL_DEVICE         *OpalDevice,
   IN   UINT8               *AdminPassword
);

EFI_STATUS
EnableGlobalRange (
   IN   OPAL_DEVICE         *OpalDevice,
   IN   UINT8               *AdminPassword
);

EFI_STATUS
UnlockOpalDrive (
    IN  OPAL_DEVICE         *OpalDevice,
    IN  UINT8               *PasswordBuffer,
    IN  BOOLEAN             AdminUser
);

EFI_STATUS
CreateandActivateUser (
   IN   UINT8              User,
   IN   OPAL_DEVICE        *OpalDevice,
   IN   UINT8              *AdminPassword,
   IN   UINT8              *UserPassword
);

EFI_STATUS
IsUserEnabled (
    IN  UINT8              User,
    IN  OPAL_DEVICE        *OpalDevice,
    IN  UINT8              *AdminPassword,
    IN  BOOLEAN            *UserEnabled
);

EFI_STATUS
ChangeUserPassword (
   IN   UINT8              User,        // One based
   IN   OPAL_DEVICE        *OpalDevice,
   IN   UINT8              *AdminPassword,
   IN   UINT8              *UserPassword
);

EFI_STATUS
GetCpin (
    IN  OPAL_DEVICE     *OpalDevice,
    IN  SESSION_PARAMS  *SessionParams,
    IN  UINT64          TableObjUid,
    IN  UINT32          *CpinLength,
    IN  UINT8           **Cpin,
    IN  VOID            *Buffer,
    IN  UINTN           BufferLength
);

EFI_STATUS
GetSupportedProtocols (
    IN  OPAL_DEVICE *OpalDevice
);

EFI_STATUS
GetTperProperties (
    IN  OPAL_DEVICE     *OpalDevice,
    IN  SESSION_PARAMS  *SessionParams,
    IN  VOID            *Buffer,
    IN  UINTN           BufferLength
);

EFI_STATUS
SplitPasswordBuffer(
    IN UINT8 *PasswordBuffer,
    IN OUT  UINT8 **Password1,
    IN OUT  UINT8 **Password2
);

EFI_STATUS
StartLockingSPSession (
    IN  OPAL_DEVICE     *OpalDevice,
    IN  SESSION_PARAMS  *SessionParams,
    IN  UINT8           *Password,
    IN  BOOLEAN         AdminUser
);

EFI_STATUS
TableUidGet (
    IN  OPAL_DEVICE         *OpalDevice,
    IN  SESSION_PARAMS      *SessionParams,
    IN  VOID                *Buffer,
    IN  UINTN               BufferLength,
    IN  UINT64              TableObjUid,
    IN  CELL_BLOCK          *CellBlock
);

EFI_STATUS
DisableOpalUserPassword (
    IN  OPAL_DEVICE     *OpalDevice,
    IN  UINT8           *PwdBuffer
);

EFI_STATUS
DisableUserPwd (
   IN   UINT8               User,
   IN   OPAL_DEVICE         *OpalDevice,
   IN   SESSION_PARAMS      *SessionParams,
   IN   UINT8                *PwdBuffer
);

EFI_STATUS
RevertAdminSP (
    IN  OPAL_DEVICE         *OpalDevice,
    IN  SESSION_PARAMS      *SessionParams,
    IN  UINT8                *PwdBuffer
);

EFI_STATUS
RevertLockingSP (
    IN  OPAL_DEVICE         *OpalDevice,
    IN  SESSION_PARAMS      *SessionParams,
    IN  UINT8                *PwdBuffer
);

EFI_STATUS
ChangeSIDtoMSID (
   IN  OPAL_DEVICE         *OpalDevice,
   IN  VOID                *PwdBuffer
);

#if ENABLE_DEVICE_RESET_THRU_PSID
EFI_STATUS
RevertUsingPsid (
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    UINT8               *PsidBuffer
);
#endif

EFI_STATUS
ChangeAdminPassword(
    IN  OPAL_DEVICE         *OpalDevice,
    IN  UINT8               *AdminPassword
);


#if SmmTcgStorageSec_SUPPORT

EFI_STATUS
SmmTcgPasswordInterface (
    IN  AMI_HDD_SECURITY_PROTOCOL   *This,
    IN  UINT16                      Control,
    IN  UINT8                       *Buffer,
    IN  TCG_STORAGE_SEC_PASSWORD_ACTION     Action
);

EFI_STATUS
TcgTransferPwDataFromNvramToSmram (
);

EFI_STATUS
GatherTcgDeviceNvramInfo (
    AMI_HDD_SECURITY_PROTOCOL   *TcgStorageSecurityProtocol,
    OUT UINT32                   *TcgDeviceNvramId
);

EFI_STATUS
GetTcgSecDataFromNvram(
    AMI_HDD_SECURITY_PROTOCOL   *TcgStorageSecurityProtocol,
    UINT8                        *Buffer
);

EFI_STATUS
SetTcgDeviceDataInNvram (
    UINT32      *TcgDeviceNvramId,
    UINT16      Control,
    UINT8       *Buffer
);
EFI_STATUS
TransferDmaBufferToSmram (
    IN  EFI_EVENT   Event, 
    IN  VOID        *Context
);
#endif

EFI_STATUS
EnableTperReset (
    IN   OPAL_DEVICE         *OpalDevice,
    IN   UINT8               *AdminPassword
);

EFI_STATUS
IssueTperReset (
    IN  VOID                       *BusInterface,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
);

EFI_STATUS
EnterpriseLockedStatus (
    IN   OPAL_DEVICE         *OpalDevice
);

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif

#endif

//***********************************************************************
//***********************************************************************
//**                                                                   **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.          **
//**                                                                   **
//**                       All Rights Reserved.                        **
//**                                                                   **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093         **
//**                                                                   **
//**                       Phone: (770)-246-8600                       **
//**                                                                   **
//***********************************************************************
//***********************************************************************
