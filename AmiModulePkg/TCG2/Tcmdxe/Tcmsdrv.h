/*++
 Copyright (c) 2009 Nationz Technologies Inc. All rights reserved

Module Name:

  Tcmsdrv.h

Abstract:

  TIS (TCM Interface Specification) functions

   Product:    TCM DXE Driver for Nationz V1.04  
   Author:     Nosa

   $Revision:  01.04.11.09 
   $Date:      12/14/2009

--*/
#define _CRT_SECURE_NO_WARNINGS
#include <Uefi.h>
#include "TisPc.h"
#include <AmiTcg/Tcm.h>

#define EFI_TCMMP_PROTOCOL_GUID \
  { 0xA062CF1F, 0x8473, 0x4bf3, 0x87, 0x93, 0x60, 0x0B, 0xC4, 0xFF, 0xA9, 0xA9 }

EFI_GUID gEfiTcmMPProtocolGuid = EFI_TCMMP_PROTOCOL_GUID;

#define PFlagLen                   0x11
#define TCM_VID                    0x1B4E
#define TCM_MMIO_BASE              0xFED40000

#pragma pack (push)
#pragma pack (1)
//
// TCM_Startup
//
typedef struct _TCM_CMD_STARTUP {
  TCM_RQU_COMMAND_HDR                Header;
  TCM_STARTUP_TYPE                   StartupType;
} TCM_CMD_STARTUP;

//
// TCM_PhysicalSetDeactivated
//
typedef struct _TCM_CMD_PHYSICAL_SET_DEACTIVATED {
  TCM_RQU_COMMAND_HDR               Header;
  BOOLEAN                           Flag;
} TCM_CMD_PHYSICAL_SET_DEACTIVATED;

//
//---TCM_GetCapability funciton structure
//
typedef struct _TCM_CMD_GET_CAPABILITY_IN{
  TCM_RQU_COMMAND_HDR               Header;
  UINT32 	                        capArea;
  UINT32 	                        subCapSize;
  UINT32                            subCap; 
}TCM_GET_CAPABILITY_IN;

typedef struct _TCM_CMD_GET_CAPABILITY_OUT{
  TCM_RSP_COMMAND_HDR               Header;
  UINT32                            subSize;
  UINT8                             resp[20];
}TCM_CMD_GET_CAPABILITY_OUT;

#pragma pack (pop)

EFI_STATUS
EFIAPI
TcmCommCheckStatus (
    IN      EFI_STATUS              Status,
    IN      TCM_RSP_COMMAND_HDR     *RspHdr,
    IN      TCM_STRUCTURE_TAG       RspHdrTag
 );
