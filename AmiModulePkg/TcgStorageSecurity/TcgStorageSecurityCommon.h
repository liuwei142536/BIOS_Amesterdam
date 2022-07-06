//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file TcgStorageSecurityCommon.h
    This file contains the Includes, Definitions, typedefs,
    Variable and External Declarations, Structure and
    function prototypes needed for the different module parts in 
    TcgStorageSecurity driver

**/

#ifndef _TCG_STORAGE_SECURITY_COMMON__H_
#define _TCG_STORAGE_SECURITY_COMMON__H_

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------

#include <Protocol/DiskInfo.h>
#include <IndustryStandard/AmiAtaAtapi.h>
#include <Include/AmiStorageFeatures.h>
#include <Protocol/AmiHddSecurityInit.h>
#include <Protocol/AmiHddSecurity.h>
#if ENABLE_DEVICE_RESET_THRU_PSID
#include "AmiTcgStorageDeviceReset.h"
#endif
#if ( defined(IdeBusSrc_SUPPORT) && (IdeBusSrc_SUPPORT != 0) )
#include <Protocol/AmiIdeBus.h>
#endif
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
// Include AmiDxeLib only for AhciBusDriver.Once Ahci driver
// removes dependency of AmiCompatabilityPkg this can be removed.
#include <Include/AmiDxeLib.h>
#include <Protocol/AmiAhciBus.h>
#endif
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
#include <Include/NvmeRstPwdIncludes.h>
#endif
// GUID Definitions
#define AMI_TCG_STORAGE_SEC_SMM_SAVE_PWD_GUID \
	  { 0x71762fb9, 0x3fbf, 0x4a87, 0x97, 0xe9, 0x6d, 0x29, 0xd0, 0x46, 0x34, 0x8e }

#define AMI_TCG_STORAGE_SEC_SMM_REMOVE_PWD_GUID \
	  { 0x46ccc76f, 0x749d, 0x4921, 0xbe, 0x4c, 0x95, 0x29, 0xce, 0x67, 0x3d, 0x4e }

#define AMI_TCG_GET_NON_SMRAM_DATA_TRANSFER_BUFFER_GUID \
	  { 0x321e5e7f, 0x1d5f, 0x486c, 0xb4, 0x41, 0xe3, 0xe1, 0xc1, 0xfb, 0x2a, 0xcc }

#define AMI_TCG_STORAGE_SEC_NVME_BOOTSCRIPT_SAVE_TRIGGER_GUID \
	  { 0x7c0fc2a2, 0xd0a3, 0x4196, 0x9e, 0xf, 0xe7, 0xda, 0x40, 0x43, 0x78, 0xab }

#define AMI_TCG_STORAGE_SEC_SMI_BOOTSCRIPT_SAVE_TRIGGER_GUID \
	  { 0xb8a9d34b, 0xc97, 0x4854, 0x93, 0xb6, 0x66, 0x71, 0x3d, 0xdb, 0x48, 0xf6 }

#define AMI_TCG_STORAGE_SEC_LOAD_SETUP_DEFAULTS_GUID \
	  { 0x7ffbe5b9, 0x311, 0x422e, 0xaa, 0x3c, 0xa7, 0xa7, 0xe, 0x61, 0x4c, 0xa5 }

#define AMI_TCG_STORAGE_SECURITY_PWNV_GUID \
      { 0x1af5a7a6, 0x719a, 0x4b50, 0xb4, 0x2f, 0x9d, 0xce, 0x2d, 0x28, 0x0a, 0x92 }
	
#define AMI_TSE_TCG_NOTIFY_GUID \
      { 0x81231c9d, 0xa817, 0x4e2d, 0xa4, 0x5f, 0xea, 0x8c, 0x7c, 0xb1, 0x3e, 0x63 }

#define AMI_TSE_TCG_FREEZE_STATE_NOTIFY_GUID \
      { 0x38dc1092, 0xff7a, 0x4336, 0xa2, 0x73, 0xbf, 0xb5, 0x19, 0x36, 0x8a, 0x5f }

EFI_GUID gTcgStorageSecSavePwdGuid = AMI_TCG_STORAGE_SEC_SMM_SAVE_PWD_GUID;
EFI_GUID gTcgStorageSecRemovePwdGuid = AMI_TCG_STORAGE_SEC_SMM_REMOVE_PWD_GUID;
EFI_GUID gTcgGetNonSmramDataTransferBufferGuid = AMI_TCG_GET_NON_SMRAM_DATA_TRANSFER_BUFFER_GUID;
EFI_GUID gTcgStorageSecSmiBootScriptSaveTriggerGuid = AMI_TCG_STORAGE_SEC_SMI_BOOTSCRIPT_SAVE_TRIGGER_GUID;
EFI_GUID gTcgStorageSecNvmeBootScriptSaveTriggerGuid = AMI_TCG_STORAGE_SEC_NVME_BOOTSCRIPT_SAVE_TRIGGER_GUID;
EFI_GUID gTcgStorageSecurityPwNvguid = AMI_TCG_STORAGE_SECURITY_PWNV_GUID;

#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )

#ifndef EFI_PCI_CFG_ADDRESS
#define EFI_PCI_CFG_ADDRESS(bus,dev,func,reg)   \
 ((UINT64) ( (((UINTN)bus) << 24)+(((UINTN)dev) << 16) + (((UINTN)func) << 8) + ((UINTN)reg)))& 0x00000000ffffffff
#endif

#define PCI_VID              0x00
#define PCI_RID              0x08
#define PCI_CL_BRIDGE        0x06

#define NVME_CONTROLLER_BAR0 0x10
#define NVME_CONTROLLER_BAR1 0x14
#define NVME_CONTROLLER_BAR2 0x18
#define NVME_CONTROLLER_BAR3 0x1C
#define NVME_CONTROLLER_BAR4 0x20
#define NVME_CONTROLLER_BAR5 0x24
#define NVME_CONTROLLER_CMD  0x04

#define PCI_ROOT_BRIDGE_BUS   0x18
#define PCI_ROOT_BRIDGE_IO    0x1C
#define PCI_ROOT_BRIDGE_MEM   0x20
#define PCI_ROOT_BRIDGE_PFMEM 0x24   // Prefetchable Memory
#define PCI_ROOT_BRIDGE_CMD   0x04

typedef struct {
    UINT32                              RegNum;
    EFI_BOOT_SCRIPT_WIDTH               Width;
    UINT32                              Mask;
} BOOT_SCRIPT_NVME_REG_SAVE;

#define MAX_NVME_DEVICES 32

#define NVME_SECURITY_SAVE_SIGNATURE  SIGNATURE_32('n','s','b','s')

typedef struct {
    UINTN                               PciAddress;
    EFI_BOOT_SCRIPT_WIDTH               Width;
    UINTN                               Value;
} BOOT_SCRIPT_NVME_SECURITY_SAVE_ENTRY;

#endif


#define 	OPAL_PASSWORD_LENGTH 			32
#define     PSID_PASSWORD_LENGTH            32

#define     SecuritySupportedMask       0x0001
#define     SecurityEnabledMask         0x0002
#define     SecurityLockedMask          0x0004
#define     SecurityFrozenMask          0x0008

typedef struct {
    UINT32                    Signature;
    // Changed UserPassword and MasterPassword array to pointer 
    // since separate Confidential pool will be allocated for password in SMRAM
    UINT8                     *UserPassword; 
    UINT8                     *MasterPassword;
    UINT16                    BaseAddress;
    UINT16                    DeviceControlReg;
    UINT8                     Device;             // Master/Slave
    UINT16                    Control;
    UINT16                    RevisionCode;
    UINT8                     PortNumber;
    UINT8                     PMPortNumber;
    EFI_HANDLE                DevHandle;
    UINT8                     BusNo;
    UINT8                     DeviceNo;
    UINT8                     FunctionNo;
    AMI_STORAGE_BUS_INTERFACE BusInterfaceType;   
    BOOLEAN                   RstControlled;
    LIST_ENTRY                LinkList;
} TCG_STORAGE_SECURITY_PASSWORD;

typedef struct
{
    AMI_HDD_SECURITY_PROTOCOL               *TcgStorageSecProtocol;
#if ENABLE_DEVICE_RESET_THRU_PSID
    AMI_TCG_STORAGE_DEVICE_RESET_PROTOCOL   *TcgDeviceResetProtocol;
#endif
    EFI_HANDLE                              PostHiiHandle;
    UINT16                                  NameToken;
    UINT16                                  PromptToken;
    BOOLEAN                                 Locked;
    BOOLEAN                                 LoggedInAsMaster;
    BOOLEAN                                 Validated;
    UINT8                                   PWD[OPAL_PASSWORD_LENGTH + 1];
    EFI_HANDLE                              DeviceHandle;
} TCG_STORAGE_SECURITY_INTERNAL_DATA;

typedef struct _AMI_TCG_STORAGE_SECURITY_INTERFACE { 

    AMI_HDD_RETURN_SECURITY_STATUS       ReturnSecurityStatus;
    AMI_HDD_SECURITY_SET_PASSWORD        SecuritySetPassword;
    AMI_HDD_SECURITY_UNLOCK_PASSWORD     SecurityUnlockPassword;
    AMI_HDD_SECURITY_DISABLE_PASSWORD    SecurityDisablePassword;
    AMI_HDD_SECURITY_FREEZE_LOCK         SecurityFreezeLock;
    AMI_HDD_SECURITY_ERASE_UNIT          SecurityEraseUnit;
    AMI_HDD_RETURN_PASSWORD_FLAGS        ReturnIdePasswordFlags;
    VOID                                 *BusInterface;
    AMI_STORAGE_BUS_INTERFACE            BusInterfaceType;
    VOID                                 *PostHddCount;
    VOID                                 **PostHddData;
} AMI_TCG_STORAGE_SECURITY_INTERFACE;

#if ENABLE_DEVICE_RESET_THRU_PSID
typedef struct _AMI_TCG_STORAGE_RESET_INTERFACE { 

	AMI_TCG_STORAGE_DEVICE_RESET_PSIDREVERT         PsidRevert;
	AMI_TCG_STORAGE_SECURITY_INTERFACE              *TcgStorageSecurityInterface;

} AMI_TCG_STORAGE_RESET_INTERFACE;
#endif
/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif

#endif


