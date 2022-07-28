//***********************************************************************
//*                                                                     *
//*                      Copyright (c) 1985-2021, AMI.                  *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file  EfiOsBootOptionNames.h
    Header file for EfiOsBootOptionNames module.
**/

#ifndef _EFI_OS_BOOT_OPTION_NAMES_H_
#define _EFI_OS_BOOT_OPTION_NAMES_H_
#ifdef __cplusplus
extern "C" {
#endif

#define EFI_BOOT_FILE_NAME EFI_REMOVABLE_MEDIA_FILE_NAME

#if   defined (MDE_CPU_IA32)
  #define MACHINE_TYPE L"ia32"
#elif defined (MDE_CPU_X64)
  #define MACHINE_TYPE L"x64"
#elif defined (MDE_CPU_EBC)
#elif defined (MDE_CPU_ARM)
  #define MACHINE_TYPE L"arm"
#elif defined (MDE_CPU_AARCH64)
  #define MACHINE_TYPE L"aa64"
#else
  #error Unknown Processor Type
#endif
#define LINUX_FIRMWARE_UPDATE_FILE (L"\\fwupd"  MACHINE_TYPE L".efi")
#define LINUX_FIRMWARE_UPDATE_BOOT_OPTION BIT0
#define IsAttribute(value, attribute) (value & attribute)


#define EFI_OS_BOOT_OPTION_NAMES_GUID \
    {0x69ECC1BE, 0xA981, 0x446D, 0x8E, 0xB6, 0xAF, 0x0E, 0x53, 0xD0, 0x6C, 0xE8}

#pragma pack(1)
typedef struct _PARTITION_ENTRY {
    UINT8 ActiveFlag;               	///< Bootable or not
    UINT8 StartingTrack;            	///< Not used
    UINT8 StartingCylinderLsb;      	///< Not used
    UINT8 StartingCylinderMsb;      	///< Not used
    UINT8 PartitionType;            	///< 12 bit FAT, 16 bit FAT etc.
    UINT8 EndingTrack;              	///< Not used
    UINT8 EndingCylinderLsb;        	///< Not used
    UINT8 EndingCylinderMsb;        	///< Not used
    UINT32 StartSector;                 ///< Relative sectors
    UINT32 PartitionLength;         	///< Sectors in this partition
} PARTITION_ENTRY;

typedef struct
{
	CHAR16 *FilePath;            ///< Path to open the OS image.
	CHAR16 *BootOptionName;      ///< Or, OS Name.
}NAME_MAP;

#pragma pack()

typedef struct {
    BOOLEAN                   Invalid;
    EFI_LOAD_OPTION           *NvramOption;
    UINTN                     Size;
    CHAR16                    *Desc;
	EFI_DEVICE_PATH_PROTOCOL  *Dp;
    UINT16                    Num;
    BOOLEAN                   Fw;
    UINT32                    Attr;
}SIMPLE_BOOT_OPTION;

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif
