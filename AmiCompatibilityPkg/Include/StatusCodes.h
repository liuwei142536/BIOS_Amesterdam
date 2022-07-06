//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
/** @file
  AMI-specific PI Status Code Operation Definitions
**/

#ifndef _EFI_STATUS_CODE_H_
#define _EFI_STATUS_CODE_H_
#include <Efi.h>
#include <UefiHii.h>
#include <DataHubSubClass.h>

// Include from MdePkg
#include <Guid/StatusCodeDataTypeId.h>
#include <Pi/PiStatusCode.h>
// IntelFrameworkModulePkg
#include <Guid/DataHubStatusCodeRecord.h>
// IntelFrameworkPkg
#include <Framework/StatusCode.h>

// Map legacy AMI-specific codes to standard PI specification definitions
//Chipset
#define AMI_CHIPSET_PC_PEI_CAR_SB_INIT EFI_CHIPSET_PC_PEI_CAR_SB_INIT
#define AMI_CHIPSET_PC_PEI_CAR_NB_INIT EFI_CHIPSET_PC_PEI_CAR_NB_INIT
#define AMI_CHIPSET_PC_PEI_MEM_SB_INIT EFI_CHIPSET_PC_PEI_MEM_SB_INIT
#define AMI_CHIPSET_PC_PEI_MEM_NB_INIT EFI_CHIPSET_PC_PEI_MEM_NB_INIT
#define AMI_CHIPSET_PC_DXE_HB_INIT EFI_CHIPSET_PC_DXE_HB_INIT
#define AMI_CHIPSET_PC_DXE_NB_INIT EFI_CHIPSET_PC_DXE_NB_INIT
#define AMI_CHIPSET_PC_DXE_NB_SMM_INIT EFI_CHIPSET_PC_DXE_NB_SMM_INIT
#define AMI_CHIPSET_PC_DXE_SB_RT_INIT EFI_CHIPSET_PC_DXE_SB_RT_INIT
#define AMI_CHIPSET_PC_DXE_SB_INIT EFI_CHIPSET_PC_DXE_SB_INIT
#define AMI_CHIPSET_PC_DXE_SB_SMM_INIT EFI_CHIPSET_PC_DXE_SB_SMM_INIT
#define AMI_CHIPSET_PC_DXE_SB_DEVICES_INIT EFI_CHIPSET_PC_DXE_SB_DEVICES_INIT
#define AMI_CHIPSET_EC_BAD_BATTERY EFI_CHIPSET_EC_BAD_BATTERY
#define AMI_CHIPSET_EC_DXE_NB_ERROR EFI_CHIPSET_EC_DXE_NB_ERROR
#define AMI_CHIPSET_EC_DXE_SB_ERROR EFI_CHIPSET_EC_DXE_SB_ERROR

//PEI Core
#define AMI_PEI_CORE_EC_DXEIPL_NOT_FOUND EFI_SW_PEI_CORE_EC_DXEIPL_NOT_FOUND
#define AMI_PS_EC_MEMORY_NOT_INSTALLED EFI_SW_PEI_CORE_EC_MEMORY_NOT_INSTALLED

//PEI Services
#define AMI_PS_PC_RESET_SYSTEM EFI_SW_PS_PC_RESET_SYSTEM
#define AMI_PS_EC_RESET_NOT_AVAILABLE EFI_SW_PS_EC_RESET_NOT_AVAILABLE
#define AMI_PS_EC_MEMORY_INSTALLED_TWICE EFI_SW_PS_EC_MEMORY_INSTALLED_TWICE

//PEIM
#define AMI_PEIM_PC_S3_BOOT_SCRIPT EFI_SW_PEI_PC_S3_BOOT_SCRIPT
#define AMI_PEIM_PC_OS_WAKE EFI_SW_PEI_PC_OS_WAKE
#define AMI_PEIM_EC_S3_RESUME_PPI_NOT_FOUND EFI_SW_PEI_EC_S3_RESUME_PPI_NOT_FOUND
#define AMI_PEIM_EC_S3_BOOT_SCRIPT_ERROR EFI_SW_PEI_EC_S3_BOOT_SCRIPT_ERROR
#define AMI_PEIM_EC_S3_OS_WAKE_ERROR EFI_SW_PEI_EC_S3_OS_WAKE_ERROR
#define AMI_PEIM_EC_RECOVERY_PPI_NOT_FOUND EFI_SW_PEI_EC_RECOVERY_PPI_NOT_FOUND
#define AMI_PEIM_EC_RECOVERY_FAILED EFI_SW_PEI_EC_RECOVERY_FAILED
#define AMI_PEIM_EC_S3_RESUME_FAILED EFI_SW_PEI_EC_S3_RESUME_FAILED

//DXE Core
#define AMI_DXE_CORE_PC_ARCH_READY EFI_SW_DXE_CORE_PC_ARCH_READY
#define AMI_DXE_CORE_EC_NO_ARCH EFI_SW_DXE_CORE_EC_NO_ARCH

//DXE Driver
#define AMI_DXE_BS_EC_INVALID_PASSWORD EFI_SW_DXE_BS_EC_INVALID_PASSWORD
#define AMI_DXE_BS_EC_BOOT_OPTION_LOAD_ERROR EFI_SW_DXE_BS_EC_BOOT_OPTION_LOAD_ERROR
#define AMI_DXE_BS_EC_BOOT_OPTION_FAILED EFI_SW_DXE_BS_EC_BOOT_OPTION_FAILED
#define AMI_DXE_BS_EC_INVALID_IDE_PASSWORD EFI_SW_DXE_BS_EC_INVALID_IDE_PASSWORD


// Software Class PEI Module Subclass Error Code definitions.
#ifndef EFI_SW_PEIM_EC_NO_RECOVERY_CAPSULE
#define EFI_SW_PEIM_EC_NO_RECOVERY_CAPSULE EFI_SW_PEI_EC_NO_RECOVERY_CAPSULE
#endif
#ifndef EFI_SW_PEIM_EC_INVALID_CAPSULE_DESCRIPTOR
#define EFI_SW_PEIM_EC_INVALID_CAPSULE_DESCRIPTOR EFI_SW_PEI_EC_INVALID_CAPSULE_DESCRIPTOR
#endif
///////////////////////////////////////////////////////////////////////////////
// AMI-specific operations
///////////////////////////////////////////////////////////////////////////////
//AMI Status Code definitions
#define AMI_STATUS_CODE_CLASS EFI_OEM_SPECIFIC //0x8000
#define AMI_STATUS_CODE_BEEP_CLASS 0x8100

// Host Processor AMI-specific codes
#define AMI_CU_HP_PC_DXE_CPU_INIT (AMI_STATUS_CODE_CLASS | 0x00000000)
#define AMI_CU_HP_EC_DXE_CPU_ERROR (AMI_STATUS_CODE_CLASS | 0x00000001)

//Chipset AMI-specific codes
#define AMI_CHIPSET_EC_INTRUDER_DETECT (AMI_STATUS_CODE_CLASS | 0x0000000C)
#define AMI_CHIPSET_PC_PEI_CAR_NB2_INIT (AMI_STATUS_CODE_CLASS | 0x0000000F)
#define AMI_CHIPSET_PC_PEI_MEM_NB2_INIT (AMI_STATUS_CODE_CLASS | 0x00000010)
#define AMI_CHIPSET_PC_DXE_NB2_INIT (AMI_STATUS_CODE_CLASS | 0x00000011)

// Keyboard AMI-specific codes
#define AMI_P_KEYBOARD_EC_BUFFER_FULL (AMI_STATUS_CODE_CLASS | 0x00000000)

//PEIM AMI-specific codes
#define AMI_PEIM_PC_S3_STARTED (AMI_STATUS_CODE_CLASS | 0x00000000)
#define AMI_PEIM_PC_VIDEO_REPOST (AMI_STATUS_CODE_CLASS | 0x00000003)
#define AMI_PEIM_EC_S3_RESUME_ERROR (AMI_STATUS_CODE_CLASS | 0x00000008)

//Boot Services AMI-specific codes
#define AMI_BS_PC_NVRAM_INIT (AMI_STATUS_CODE_CLASS | 0x00000000)
#define AMI_BS_PC_NVRAM_GC (AMI_STATUS_CODE_CLASS | 0x00000001)
#define AMI_BS_PC_CONFIG_RESET (AMI_STATUS_CODE_CLASS | 0x00000002)

//DXE Driver AMI-specific codes
#define AMI_DXE_BS_PC_ACPI_INIT (AMI_STATUS_CODE_CLASS | 0x00000000)
#define AMI_DXE_BS_PC_CSM_INIT (AMI_STATUS_CODE_CLASS | 0x00000001)
#define AMI_DXE_BS_EC_MBR_GPT_WRITE_PROTECTED (AMI_STATUS_CODE_CLASS | 0x00000006)

#endif
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
