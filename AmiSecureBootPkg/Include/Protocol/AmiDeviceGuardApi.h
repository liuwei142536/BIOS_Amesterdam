//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file
	Ami SecureBoot DeviceGuard  API: Common Protocol definitions
**/

#ifndef _AMI_DEVICEGUARD_API_H_
#define _AMI_DEVICEGUARD_API_H_

// DAEEAFC8-D2A8-4d9f-B093-3438984E5FDD
#define AMI_DEVICEGUARD_SECBOOT_PROTOCOL_GUID \
    {0xdaeeafc8, 0xd2a8, 0x4d9f, 0xb0, 0x93, 0x34, 0x38, 0x98, 0x4e, 0x5f, 0xdd}

typedef struct _AMI_DEVICEGUARD_SECBOOT_PROTOCOL AMI_DEVICEGUARD_SECBOOT_PROTOCOL;

typedef 
EFI_STATUS
(EFIAPI *AMI_DEVICEGUARD_SECBOOT_RESET_DB_TO_DEFAULT) (
  IN CONST AMI_DEVICEGUARD_SECBOOT_PROTOCOL *This
);
typedef 
EFI_STATUS
(EFIAPI *AMI_DEVICEGUARD_SECBOOT_IS_UEFI_CA_IN_DB) (
  IN CONST AMI_DEVICEGUARD_SECBOOT_PROTOCOL *This
);
typedef 
EFI_STATUS
(EFIAPI *AMI_DEVICEGUARD_SECBOOT_DEL_UEFI_CA_FROM_DB) (
  IN CONST AMI_DEVICEGUARD_SECBOOT_PROTOCOL *This
);

struct _AMI_DEVICEGUARD_SECBOOT_PROTOCOL {
  UINT32                                         Version;
  AMI_DEVICEGUARD_SECBOOT_RESET_DB_TO_DEFAULT    ResetDbToDefault;
  AMI_DEVICEGUARD_SECBOOT_IS_UEFI_CA_IN_DB       IsUefiCaInDb;
  AMI_DEVICEGUARD_SECBOOT_DEL_UEFI_CA_FROM_DB    DeleteUefiCaFromDb;
};

#endif //_AMI_DEVICEGUARD_API_H_
