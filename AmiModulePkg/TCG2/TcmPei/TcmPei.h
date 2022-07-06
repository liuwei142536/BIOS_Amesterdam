/*++
 Copyright (c) 2009 Nationz Technologies Inc. All rights reserved

Module Name:

  Tcmsdrv.h

Abstract:

   Product:    TCM PEI Driver for Nationz V1.04  
   Author:     Nosa

   $Revision:  01.04.11.09 
   $Date:      12/14/2009

--*/

#include <Efi.h>
#include "TisPcPei.h"
#include <AmiTcg/Tcm.h>
#include <Ppi/TcgTcmService.h>

//#define EFI_TCMMA_PROTOCOL_GUID \
//  { 0xA062CF1F, 0x8473, 0x4bf3, 0x87, 0x93, 0x60, 0x0B, 0xC4, 0xFF, 0xAA, 0xAA }

//EFI_GUID gPeiTcmPpiGuid = EFI_TCMMA_PROTOCOL_GUID;

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
//TCM EXTEND PCR
//
typedef struct _TCM_CMD_EXTENDPCR {
  TCM_RQU_COMMAND_HDR   Header;
  UINT16 				PCRIndex;
  TCM_DIGEST_HASH		TCM_INPUT_BUFFER;
} TCM_CMD_EXTENDPCR;
//carlPEIpcr-e
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

//
// TCM Function Common Call. For commnad StartUp, SelfTest, Enable, Disable, ForceClear...
//
typedef EFI_STATUS (EFIAPI *MA_TCMCOMCALL) ( 
    IN VOID
    );

//
// TCM Get TCM Status Info. Procedure.
//
typedef EFI_STATUS (EFIAPI *MA_GETSTATUS) ( 
    IN OUT UINT8         *EnabledSts,
    IN OUT UINT8         *ActivedSts,
    IN OUT UINT8         *OwnerSts 
    );

//
// TCG Pass Through TO TCM.
//
typedef EFI_STATUS (EFIAPI *MA_PASSPEITHROUGHTOTCM) ( 
  IN    TCM_PC_REGISTERS_PTR    TisReg,
  IN    UINT8                   *TcmInputParameterBlock,
  IN    UINT32                  TcmInputParameterBlockSize,
  IN    UINT8                   *TcmOutputParameterBlock,
  IN    UINT32                  TcmOutputParameterBlockSize 
    );

typedef EFI_STATUS (EFIAPI *MA_MAHashAllGlobal) (
  IN    TCM_PC_REGISTERS_PTR    TcmHandle,//carlPEIpcr
  IN    UINT8                   *TcmInputParameterBlock,
  IN    UINT32                  TcmInputParameterBlockSize,
  IN    UINT8                   *TcmOutputParameterBlock,
  IN    UINT16                  PCRIndex
    );


typedef struct _PEI_TCM_PPI {
    MA_TCMCOMCALL       Tcm_StartUp_Clear;   // MATCMSTClr;
    MA_TCMCOMCALL       Tcm_StartUp_State;   // MATCMSTSts;
    MA_TCMCOMCALL       Tcm_Enable;          // MATCMEnable;
    MA_TCMCOMCALL       Tcm_Disable;         // MATCMDisable;
    MA_TCMCOMCALL       Tcm_ClearOwner;      // MATCMClrOwmer;
    MA_GETSTATUS        Tcm_GetStatus;       // MATCMGetStatus;
    MA_PASSPEITHROUGHTOTCM TcmLibPassThrough;
    MA_MAHashAllGlobal  MAHashAllGlobal; //carlPEI
} PEI_TCM_PPI;

EFI_STATUS
TCMINTERNALAPIV
TcmCommCheckStatus (
    IN      EFI_STATUS              Status,
    IN      TCM_RSP_COMMAND_HDR     *RspHdr,
    IN      TCM_STRUCTURE_TAG       RspHdrTag
 );
