/*++

Copyright (c) 1996 - 2013, Intel Corporation.

This source code and any documentation accompanying it ("Material") is furnished
under license and may only be used or copied in accordance with the terms of that
license.  No license, express or implied, by estoppel or otherwise, to any
intellectual property rights is granted to you by disclosure or delivery of these
Materials.  The Materials are subject to change without notice and should not be
construed as a commitment by Intel Corporation to market, license, sell or support
any product or technology.  Unless otherwise provided for in the license under which
this Material is provided, the Material is provided AS IS, with no warranties of
any kind, express or implied, including without limitation the implied warranties
of fitness, merchantability, or non-infringement.  Except as expressly permitted by
the license for the Material, neither Intel Corporation nor its suppliers assumes
any responsibility for any errors or inaccuracies that may appear herein.  Except
as expressly permitted by the license for the Material, no part of the Material
may be reproduced, stored in a retrieval system, transmitted in any form, or
distributed by any means without the express written consent of Intel Corporation.


Module Name:

  DFWUpgrade.c

Abstract:

  This driver implements functions for Direct Firmware Upgrade.
  

--*/
#if 0
// Currently this code is not used.
// Enable this code if we decide to implement ME update in BIOS.

#include <Base.h>
#include <Uefi.h>
#include <PiDxe.h>


#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Library/DebugLib.h>
#include <Library/PrintLib.h>

#include <Guid/GlobalVariable.h>
#include <Guid/SetupVariable.h>

#include <Guid/HobList.h>
#include <Guid/EventLegacyBios.h>
#include <Guid/SpsInfoHobGuid.h>
#include <Guid/MeFwModesGuid.h>

#include <Protocol/MpService.h>

#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/CpuLib.h>

#include <Protocol/CpuIo.h>
#include <Protocol/HeciProtocol.h>
#include <Protocol/PciIo.h>
#include <Protocol/SpsSiEn.h>
#include <Protocol/SpsNm.h>

#include <Sps.h>
#include <SpsDxe.h>
#include <HeciRegs.h>
#define FILE_NAME "DFWUpgrade.c"


/* Direct Firmware Upgrade definitions */
/* Rsanch2 Dont want to missed this again!!! */

#pragma pack(push)
typedef union _CBM_COMMAND {
  UINT8 Data;
  struct {
    UINT8 Command : 7;
    UINT8 IsResponse : 1;
  } Fields;
} CBM_COMMAND;

typedef struct   {
    HECI_MESSAGE_HEADER    HeciHeader;
    UINT8           	    GroupId;
    CBM_COMMAND             Command;
    UINT16                  Result;
} ME_BIOS_END_OF_POST_REQUEST;

typedef struct  {
    HECI_MESSAGE_HEADER    HeciHeader;
    UINT8           	    GroupId;
    CBM_COMMAND             Command;
    UINT8                   Result;
    UINT8                   Reserved[4];
} ME_BIOS_END_OF_POST_RESPONSE;

typedef union {
    ME_BIOS_END_OF_POST_REQUEST     Request;
    ME_BIOS_END_OF_POST_RESPONSE    Response;
} ME_BIOS_END_OF_POST_BUFFER;

typedef struct   {
    HECI_MESSAGE_HEADER    HeciHeader;
    UINT8           	    GroupId;
    CBM_COMMAND             Command;
    UINT16                  Result;
} ME_BIOS_FW_VERSION_REQUEST;

typedef struct  {
    HECI_MESSAGE_HEADER    HeciHeader;
    UINT8           	    GroupId;
    CBM_COMMAND             Command;
    UINT16                   Result;
    UINT16                   FwVerMinor;
    UINT16                   FwVerMajor;
    UINT16                   FwVerBuild;
    UINT16                   FwVerPatch;
    UINT16                   etc[4];
    UINT8                   Reserved[4];
} ME_BIOS_FW_VERSION_RESPONSE;

typedef union {
    ME_BIOS_FW_VERSION_REQUEST     Request;
    ME_BIOS_FW_VERSION_RESPONSE    Response;
} ME_BIOS_FW_VERSION_BUFFER;
#pragma pack(pop)

typedef struct { 
	HECI_MESSAGE_HEADER	HeciHeader;
	MKHI_MESSAGE_HEADER	MkhiHeader;
	UINT8			Nonce;
} HMRFPO_ENABLE_REQUEST;

#ifndef CORE_BIOS_MSG_H
typedef struct { 
	MKHI_MESSAGE_HEADER	MkhiHeader;
	UINT32			FactoryBase;
	UINT32			FactoryLimit;
	UINT8			Status;	
	UINT8			Reserved;
} HMRFPO_ENABLE_RESPONSE;
#endif

typedef union {
	HMRFPO_ENABLE_REQUEST	Request;
	HMRFPO_ENABLE_RESPONSE	Response;
}HMRFPO_ENABLE_BUFFER;


typedef struct { 
	HECI_MESSAGE_HEADER	HeciHeader;
	MKHI_MESSAGE_HEADER	MkhiHeader;
} HMRFPO_LOCK_REQUEST;

#ifndef CORE_BIOS_MSG_H
typedef struct { 
	MKHI_MESSAGE_HEADER	MkhiHeader;
	UINT8			Nonce[8];
	UINT32			FactoryBase;
	UINT32			FactoryLimit;
	UINT8			Status;	
	UINT8			Reserved[3];
} HMRFPO_LOCK_RESPONSE;
#endif
typedef union {
	HMRFPO_LOCK_REQUEST	Request;
	HMRFPO_LOCK_RESPONSE	Response;
} HMRFPO_LOCK_BUFFER;


typedef struct { 
	HECI_MESSAGE_HEADER	HeciHeader;
	MKHI_MESSAGE_HEADER	MkhiHeader;
} HMRFPO_GETSTATUS_REQUEST;

typedef struct { 
	HECI_MESSAGE_HEADER	HeciHeader;
	MKHI_MESSAGE_HEADER	MkhiHeader;
	UINT8			Status;
	UINT8			Reserved[3];
} HMRFPO_GETSTATUS_RESPONSE;

typedef union {
	HMRFPO_GETSTATUS_REQUEST	Request;
	HMRFPO_GETSTATUS_RESPONSE	Response;
} HMRFPO_GETSTATUS_BUFFER;


#define SPI_ALL_MESSAGES	      1
#define SPI_ENABLE_MESSAGE	    2
#define SPI_LOCK_MESSAGE	      3
#define SPI_GET_STATUS_MESSAGE	4


EFI_STATUS
HMRFPO_Enable (
  UINT8               Multiplier
  )
{
  EFI_STATUS            Status;
  EFI_HECI_PROTOCOL     *DxeHeci;
  union
  {
    MKHI_MSG_HMRFPO_ENABLE_REQ Req;
    MKHI_MSG_HMRFPO_ENABLE_REQ Rsp;
  } HeciMsg;
  UINT32                Length;

  DEBUG (
    (EFI_D_ERROR | EFI_D_INFO,
    "[SPS] %a: %a \n",
    FILE_NAME,
    "HMRFPO_Enable.")
    );

  Status = gBS->LocateProtocol (
                &gEfiHeciProtocolGuid,
                NULL,
                &DxeHeci
                );

  if (EFI_ERROR (Status)) {
    DEBUG (
      (EFI_D_ERROR | EFI_D_INFO,
      "[SPS_INTERACTION_ERROR] %a \n",
      "Problems locating gEfiHeciProtocolGuid.")
      );
    return EFI_ERROR (50);

  }

  HeciMsg.Req.Heci.Data  = 0x800C0007;
  HeciMsg.Req.MkhiHeader.Data  = 0x00000105;
  HeciMsg.Req.Nonce            = 0;

  Length                       = 0x10;

  Status = DxeHeci->SendMsg(&HeciMsg.Req,
                      HeciMsg.Req.HeciHeader.Fields.Length,
                      SPS_CLIENTID_BIOS,
                      SPS_CLIENTID_ME_HMRFPO);

  if (EFI_ERROR (Status)) {
    DEBUG (
      (EFI_D_ERROR | EFI_D_INFO,
      "[SPS_INTERACTION_ERROR] %a: %a \n",
      FILE_NAME,
      "Problemss sending the HMRFPO_Enable Request Message ")
      );
  }

  if (Multiplier == 0) {
    Multiplier = 1;
  }

  Status = DxeHeci->MeResetWait (HECI_REINIT_TIMEOUT * Multiplier);

  if (EFI_ERROR (Status)) {
    DEBUG (
      (EFI_D_ERROR | EFI_D_INFO,
      "[SPS_INTERACTION_ERROR] %a %d\n",
      "ME never come back. Increase timer on 0.5 secs. Current Multiplier: ",
      Multiplier)
      );
  }

  Status = DxeHeci->ReadMsg (1, &Buffer.Response.Msg.MkhiHeader, &Length);

  if (EFI_ERROR (Status)) {
    DEBUG (
      (EFI_D_ERROR | EFI_D_INFO,
      "[SPS_INTERACTION_ERROR] %a \n",
      "Problemss Receiving the HMRFPO_Enable Response Message ")
      );
      return Status;
  }
//definition on CoreBiosMsg.h
  if ( Buffer.Response.Msg.Status == HMRFPO_ENABLE_FAILURE )  {
	return EFI_ERROR (HMRFPO_ENABLE_FAILURE);
  }

  /*
  if  ((Length != 0x10) || (Buffer.Response.MkhiHeader.Data !=  0x8105)) {
      DEBUG(( EFI_D_ERROR | EFI_D_INFO, "[SPS_INTERACTION_ERROR] %a: %a %X;\n", 
				    FILE_NAME, 
					    "Decoding Heci Msg has wrong value: ", Buffer.Response.MkhiHeader));
	return EFI_ERROR(51);  
  } 
   
  DEBUG(( EFI_D_ERROR | EFI_D_INFO, "[SPS_INTERACTION] %a: %a %X;\n", 
				    FILE_NAME, 
					    "Status : ", Buffer.Response.Status));

*/
  return EFI_SUCCESS;

}

EFI_STATUS
HMRFPO_GetStatus (
  EFI_BOOT_SERVICES *BS
  )
{
  EFI_STATUS              Status;
  EFI_HECI_PROTOCOL       *DxeHeci;
  HMRFPO_GETSTATUS_BUFFER Buffer;
  UINT32                  Length;

  DEBUG (
    (EFI_D_ERROR | EFI_D_INFO,
    "[SPS_INTERACTION] %a: %a ;\n",
    FILE_NAME,
    "HMRFPO_Status.")
    );

  Status = BS->LocateProtocol (
                &gEfiHeciProtocolGuid,
                NULL,
                &DxeHeci
                );

  if (EFI_ERROR (Status)) {
    DEBUG (
      (EFI_D_ERROR | EFI_D_INFO,
      "[SPS_INTERACTION_ERROR] %a: %a ;\n",
      FILE_NAME,
      "Problems locating gEfiHeciProtocolGuid")
      );
    return EFI_ERROR (50);

  }

  HeciMsg.Req.HeciHeader.Data  = 0x80040007;
  HeciMsg.Req.MkhiHeader.Data  = 0x00000305;

  Length                          = 0x08;

  Status = DxeHeci->SendwACK (
                      &HeciMsg.Req.MkhiHeader,
                      HeciMsg.Req.HeciHeader.Fields.Length,
                      &Length,
                      (UINT8) HeciMsg.Req.HeciHeader.Fields.HostAddress,
                      (UINT8) HeciMsg.Req.HeciHeader.Fields.MeAddress
                      );

  if (EFI_ERROR (Status)) {
    DEBUG (
      (EFI_D_ERROR | EFI_D_INFO,
      "[SPS_INTERACTION_ERROR] %a: %a ;\n",
      FILE_NAME,
      "Problemss sending the HMRFPO_Enable Message ")
      );
  }

  if ((Length != 0x08) || (Buffer.Response.MkhiHeader.Data != 0x8305)) {
    DEBUG (
      (EFI_D_ERROR | EFI_D_INFO,
      "[SPS_INTERACTION_ERROR] %a: %a %X;\n",
      FILE_NAME,
      "Decoding Heci Msg has wrong value: ",
      Buffer.Response.MkhiHeader.Data)
      );
    return EFI_ERROR (51);
  }

  DEBUG (
    (EFI_D_ERROR | EFI_D_INFO,
    "[SPS_INTERACTION] %a: %a %X;\n",
    FILE_NAME,
    "Status : ",
    Buffer.Response.Status)
    );

  return EFI_SUCCESS;

}

EFI_STATUS
HMRFPO_Lock (
  )
{
  EFI_STATUS          Status;
  EFI_HECI_PROTOCOL   *DxeHeci;
  HMRFPO_LOCK_BUFFER  Buffer;
  UINT32              Length;

  DEBUG (
    (EFI_D_ERROR | EFI_D_INFO,
    "[SPS_INTERACTION] %a: %a ;\n",
    FILE_NAME,
    "HMRFPO_Lock")
    );

  Status = gBS->LocateProtocol (
                &gEfiHeciProtocolGuid,
                NULL,
                &DxeHeci
                );

  if (EFI_ERROR (Status)) {
    DEBUG (
      (EFI_D_ERROR | EFI_D_INFO,
      "[SPS_INTERACTION_ERROR] %a: %a ;\n",
      FILE_NAME,
      "Problems locating gEfiHeciProtocolGuid")
      );
    return EFI_ERROR (50);

  }

  HeciMsg.Req.HeciHeader.Data  = 0x80040007;
  HeciMsg.Req.MkhiHeader.Data  = 0x00000205;

  Length                          = 0x18;
  Status = DxeHeci->SendwACK (
                      &HeciMsg.Req.MkhiHeader.Data,
                      HeciMsg.Req.HeciHeader.Fields.Length,
                      &Length,
                      (UINT8) HeciMsg.Req.HeciHeader.Fields.HostAddress,
                      (UINT8) HeciMsg.Req.HeciHeader.Fields.MeAddress
                      );

  if (EFI_ERROR (Status)) {
    DEBUG (
      (EFI_D_ERROR | EFI_D_INFO,
      "[SPS_INTERACTION_ERROR] %a: %a ;\n",
      FILE_NAME,
      "Problemss sending the HMRFPO_Lock Message ")
      );
  }

  if ((Length != 0x18) || (Buffer.Response.Data.MkhiHeader.Data != 0x8205)) {
    DEBUG (
      (EFI_D_ERROR | EFI_D_INFO,
      "[SPS_INTERACTION_ERROR] %a: %a %X;\n",
      FILE_NAME,
      "Decoding Heci Msg has wrong value: ",
      Buffer.Response.Data.MkhiHeader.Data)
      );
  }
  //
  // TODO: Save the Nonce...
  //
  DEBUG (
    (EFI_D_ERROR | EFI_D_INFO,
    "[SPS_INTERACTION] %a: %a %X;\n",
    FILE_NAME,
    "Status : ",
    Buffer.Response.Data.Status)
    );

  return EFI_SUCCESS;

}
#endif
