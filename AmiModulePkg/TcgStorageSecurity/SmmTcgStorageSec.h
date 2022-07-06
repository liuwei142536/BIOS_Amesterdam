//***********************************************************************
//***********************************************************************
//**                                                                   **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.          **
//**                                                                   **
//**                       All Rights Reserved.                        **
//**                                                                   **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093         **
//**                                                                   **
//**                       Phone: (770)-246-8600                       **
//**                                                                   **
//***********************************************************************
//***********************************************************************

/** @file SmmTcgStorageSec.h
    This file contains the Includes, Definitions, typedefs,
    Variable and External Declarations, Structure and
    function prototypes needed for SmmTcgStorageSec Component

**/

#ifndef _SMM_TCG_STORAGE_SEC_H_
#define _SMM_TCG_STORAGE_SEC_H_

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------

#include <Token.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmSwDispatch2.h>
#include "TcgStorageSec.h"
#include <Library/AmiBufferValidationLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/BaseMemoryLib.h>
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
#include <Protocol/AmiAhciBus.h>
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
#include "Nvme/NvmeController.h"
#include <Protocol/AmiNvmeController.h>
#include <Protocol/AmiNvmePassThru.h>
#endif

//---------------------------------------------------------------------------

extern EFI_BOOT_SERVICES             *pBS;
extern EFI_GUID                      gAmiSmmNvmePassThruProtocolGuid;
extern EFI_GUID                      gAmiSmmNvmeControllerProtocolGuid;
extern EFI_GUID                      gIdeSecurityInterfaceGuid;
extern EFI_GUID                      gEfiSmmSwDispatch2ProtocolGuid;
extern EFI_GUID                      gEfiSmmBase2ProtocolGuid;
extern EFI_SMM_BASE2_PROTOCOL        *pSmmBase;
extern EFI_SMM_SYSTEM_TABLE2         *pSmst;
LIST_ENTRY                           gOpalPasswordList;

#ifndef HBA_CAP
#define     HBA_CAP                             0x0000
#endif

#ifndef HBA_CAP_NP_MASK
#define     HBA_CAP_NP_MASK                     0x1F
#endif

#ifndef HBA_PORTS_START
#define     HBA_PORTS_START                     0x0100
#endif

#ifndef HBA_PORTS_REG_WIDTH
#define     HBA_PORTS_REG_WIDTH                 0x0080
#endif

EFI_STATUS
EFIAPI
TcgStorageSecSMMInit (
    IN  EFI_HANDLE       ImageHandle,
    IN  EFI_SYSTEM_TABLE *SystemTable
 );

EFI_STATUS
EFIAPI
InSmmFunction (
    IN  EFI_HANDLE          ImageHandle,
    IN  EFI_SYSTEM_TABLE    *SystemTable
);

EFI_STATUS
InstallSmiHandler (
    IN  EFI_HANDLE          ImageHandle,
    IN  EFI_SYSTEM_TABLE    *SystemTable
);

#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
EFI_STATUS
TcgSMMUnlockNvmeDevice (
	IN	TCG_STORAGE_SECURITY_PASSWORD    *pOpalPassword
);

EFI_STATUS
SmmNvmeSendReceiveData(
    IN  AMI_NVME_CONTROLLER_PROTOCOL	 *gSmmNvmeController,
    IN	BOOLEAN 		              SendReceive,
    IN	UINT32    			          MediaId,
    IN	UINT64                     	  Timeout,
    IN	UINT8                      	  SecurityProtocolId,
    IN	UINT16                     	  SecurityProtocolSpecificData,
    IN	UINTN                      	  PayloadBufferSize,
    OUT VOID                      	  *PayloadBuffer
);
#endif

EFI_STATUS
TcgSmmBlockSID (
	IN	OPAL_DEVICE         *OpalDevice
);

EFI_STATUS
GetOpalDeviceStatus(
    IN  OPAL_DEVICE                    *OpalDeviceConfig 
	
);

EFI_STATUS
SendSecurityUnlockCommand (
    IN	TCG_STORAGE_SECURITY_PASSWORD   *pOpalPassword,
    IN   OPAL_DEVICE                    *OpalDeviceConfig 
);

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )

EFI_STATUS
TcgSMMUnlockAhciDevice (
    TCG_STORAGE_SECURITY_PASSWORD       *pOpalPassword
);

EFI_STATUS
SmmAhciSendReceiveData(
    IN  AMI_AHCI_BUS_SMM_PROTOCOL	*gAhciSmmProtocol,
    IN  BOOLEAN       SendReceive,
    IN  UINT32        MediaId,
    IN  UINT8         PortNumber,
    IN  UINT8         PMPortNumber,
    IN  UINT64        Timeout,
    IN  UINT8         SecurityProtocolId,
    IN  UINT16        SecurityProtocolSpecificData,
    IN  UINTN         PayloadBufferSize,
    OUT VOID          *PayloadBuffer
);

#endif

// Event Handlers

EFI_STATUS 
EFIAPI
SaveOPALPassword (
    IN  EFI_HANDLE      DispatchHandle,
    IN  CONST   VOID    *DispatchContext OPTIONAL,
    IN  OUT VOID        *CommBuffer      OPTIONAL,
    IN  OUT UINTN       *CommBufferSize  OPTIONAL
);

EFI_STATUS
EFIAPI
UnlockOPALPassword (
    IN  EFI_HANDLE       DispatchHandle,
    IN  CONST VOID       *DispatchContext OPTIONAL,
    IN  OUT VOID         *CommBuffer      OPTIONAL,
    IN  OUT UINTN        *CommBufferSize OPTIONAL
);

EFI_STATUS
EFIAPI
RemoveOPALPassword (
    IN  EFI_HANDLE      DispatchHandle,
    IN  CONST VOID      *DispatchContext OPTIONAL,
    IN  OUT VOID        *CommBuffer      OPTIONAL,
    IN  OUT UINTN       *CommBufferSize  OPTIONAL
);

EFI_STATUS
EFIAPI
BootScriptSaveTcgSecSmmEventForS3 (
    IN  EFI_HANDLE       DispatchHandle,
    IN  CONST VOID       *DispatchContext OPTIONAL,
    IN  OUT VOID         *CommBuffer      OPTIONAL,
    IN  OUT UINTN        *CommBufferSize  OPTIONAL
);

EFI_STATUS
EFIAPI
TcgBootScriptSaveNvmeEventForS3 (
    IN EFI_HANDLE       DispatchHandle,
    IN CONST VOID       *DispatchContext OPTIONAL,
    IN OUT VOID         *CommBuffer      OPTIONAL,
    IN OUT UINTN        *CommBufferSize  OPTIONAL
);

EFI_STATUS
EFIAPI
TcgLocateSMMProtocols (
    IN EFI_HANDLE       DispatchHandle,
    IN CONST VOID       *DispatchContext OPTIONAL,
    IN OUT VOID         *CommBuffer      OPTIONAL,
    IN OUT UINTN        *CommBufferSize  OPTIONAL
);


/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif

#endif  

//***********************************************************************
//***********************************************************************
//**                                                                   **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.          **
//**                                                                   **
//**                       All Rights Reserved.                        **
//**                                                                   **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093         **
//**                                                                   **
//**                       Phone: (770)-246-8600                       **
//**                                                                   **
//***********************************************************************
//***********************************************************************
