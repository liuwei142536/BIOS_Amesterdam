//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file HddSecurityCommon.h
    This file contains the Includes, Definitions, typedefs,
    Variable and External Declarations, Structure and
    function prototypes needed for the different module parts in the 
    HddSecurity driver

**/

#ifndef _HDD_SECURITY_COMMON_
#define _HDD_SECURITY_COMMON_

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------
#include <Protocol/DiskInfo.h>
#include <IndustryStandard/AmiAtaAtapi.h>
#include <Include/AmiStorageFeatures.h>
#include "Protocol/AmiHddSecurityInit.h"
#include <Protocol/AmiHddSecurity.h>
#include <Protocol/StorageSecurityCommand.h>
#include <Library/DebugLib.h>
#if ( defined(IdeBusSrc_SUPPORT) && (IdeBusSrc_SUPPORT != 0) )
#include <Protocol/AmiIdeBus.h>
#endif
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
#include <Protocol/AmiAhciBus.h>
#else
typedef struct _AHCI_ATAPI_COMMAND{
    UINT8           Ahci_Atapi_Command[0x10];
} AHCI_ATAPI_COMMAND;

typedef struct{
    VOID                        *Buffer;
    UINT32                      ByteCount;
    UINT8                       Features;
    UINT8                       FeaturesExp;
    UINT16                      SectorCount;
    UINT8                       LBALow;
    UINT8                       LBALowExp;
    UINT8                       LBAMid;
    UINT8                       LBAMidExp;
    UINT8                       LBAHigh;
    UINT8                       LBAHighExp;
    UINT8                       Device;
    UINT8                       Command;
    UINT8                       Control;
    AHCI_ATAPI_COMMAND          AtapiCmd;
}COMMAND_STRUCTURE;
#endif

#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )

#ifndef EFI_PCI_CFG_ADDRESS
#define EFI_PCI_CFG_ADDRESS(bus,dev,func,reg)   \
 ((UINT64) ( (((UINTN)bus) << 24)+(((UINTN)dev) << 16) + (((UINTN)func) << 8) + ((UINTN)reg)))& 0x00000000ffffffff
#endif

#ifndef PCI_VID
#define PCI_VID              0x0000
#endif

#ifndef PCI_RID
#define PCI_RID              0x0008
#endif

#ifndef PCI_CL_BRIDGE
#define PCI_CL_BRIDGE        0x06
#endif

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

//---------------------------------------------------------------------------

#define     SecuritySupportedMask       0x0001
#define     SecurityEnabledMask         0x0002
#define     SecurityLockedMask          0x0004
#define     SecurityFrozenMask          0x0008

#define AMI_HDD_SECURITY_LOAD_SETUP_DEFAULTS_GUID \
        { 0x2ff61c92, 0xdb91, 0x431d, { 0x94, 0xf9, 0x89, 0x92, 0xc3, 0x61, 0x5f, 0x48 } }

// Remove below code when gTseEscOnPwdPromptGuid 
// definition is added in AmiTsePkg.dec
#ifndef AMITSE_ESC_ON_PWD_PROMPT_GUID 
#define AMITSE_ESC_ON_PWD_PROMPT_GUID \
    {0x5b5d1d38, 0x1fa9, 0x433b, {0x97, 0xd6, 0xf8, 0x26, 0x77, 0xab, 0x3f, 0xe4}}
#endif

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
    DLINK                     LinkList;
} HDD_PASSWORD;

typedef struct
{
    AMI_HDD_SECURITY_PROTOCOL *HddSecurityProtocol;
    EFI_HANDLE                PostHiiHandle;
    UINT16                    NameToken;
    UINT16                    PromptToken;
    BOOLEAN                   Locked;
    BOOLEAN                   LoggedInAsMaster;
    BOOLEAN                   Validated;
    UINT8                     PWD[IDE_PASSWORD_LENGTH + 1];
    EFI_HANDLE                DeviceHandle;
} HDD_SECURITY_INTERNAL_DATA;

typedef struct _AMI_HDD_SECURITY_INTERFACE { 

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

} AMI_HDD_SECURITY_INTERFACE;

#define AMI_TSE_HDD_NOTIFY_GUID \
        { 0x6de538e4, 0xe391, 0x4881, { 0x80, 0x6c, 0xc8, 0xf9, 0x66, 0xfb, 0xee, 0x35 } }

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif

#endif

