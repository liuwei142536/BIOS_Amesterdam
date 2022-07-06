//*************************************************************************
// $Header: /Alaska/SOURCE/Modules/TCG/PPI/TcgService_PPI/TcgService/TcgTcmService.h 3     9/29/11 6:08p Davidd $
//
// $Revision: 3 $
//
// $Date: 9/29/11 6:08p $
//*************************************************************************
// Revision History
// ----------------
// $Log: /Alaska/SOURCE/Modules/TCG/PPI/TcgService_PPI/TcgService/TcgTcmService.h $
//
// 3     9/29/11 6:08p Davidd
// [TAG]           EIP71378
// [Category]      Improvement
// [Description]   Customer names in source files
// [Files]         TcgTcmService.h
//
// 2     3/29/11 3:00p Fredericko
//
// 1     3/28/11 3:15p Fredericko
// [TAG]        EIP 54642
// [Category] Improvement
// [Description] 1. Checkin Files related to TCG file override
// 2. Include TCM and TPM auto detection
// [Files] Affects all TCG files
//
// 4     5/19/10 6:49p Fredericko
// Included File Header
// Included File Revision History
// EIP 37653
//
//*************************************************************************
//*************************************************************************
//<AMI_FHDR_START>
//
// Name:  TCMService.h
//
// Description:
//  Header file for TCMService.c
//
//<AMI_FHDR_END>
//*************************************************************************
#ifndef _TCG_TCM_SERVICE_H_
#define _TCG_TCM_SERVICE_H_

#include <PiPei.h>
#include <AmiTcg/TCGMisc.h>
#include <Uefi.h>
#include <AmiTcg/Tcm.h>
#include <AmiTcg/TcmPc.h>


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

//carlPEIpcr-s
//
//TCM EXTEND PCR
//
typedef struct _TCM_CMD_EXTENDPCR {
  TCM_RQU_COMMAND_HDR   Header;
  UINT16                PCRIndex;
  TCM_DIGEST_HASH       TCM_INPUT_BUFFER;
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
  UINT32                            capArea;
  UINT32                            subCapSize;
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
  IN    TCM_PC_REGISTERS_PTR    TisReg,
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
EFIAPI
TcmCommCheckStatus (
    IN      EFI_STATUS              Status,
    IN      TCM_RSP_COMMAND_HDR     *RspHdr,
    IN      TCM_STRUCTURE_TAG       RspHdrTag
 );


struct _AMI_PEI_TCM_INTERFACE;
typedef struct _AMI_PEI_TCM_INTERFACE AMI_PEI_TCM_INTERFACE;

typedef
EFI_STATUS
(EFIAPI * PEI_TCM_LOG_EVENT)(
    IN AMI_PEI_TCM_INTERFACE      *This,
    IN EFI_PEI_SERVICES **PeiServices,
    IN TCM_PCR_EVENT    *Event,
    OUT UINT32          *EventNum );

typedef
EFI_STATUS
(EFIAPI * PEI_TCM_HASH_LOG_EXTEND_EVENT)(
    IN AMI_PEI_TCM_INTERFACE      *This,
    IN EFI_PEI_SERVICES **PeiServices,
    IN UINT8            *HashData,
    IN UINT32           HashDataLen,
    IN TCM_PCR_EVENT    *NewEvent,
    OUT UINT32          *EventNum
);

typedef
EFI_STATUS
(EFIAPI * PEI_TCM_PASS_THROUGH_TO_TCM)(
    IN AMI_PEI_TCM_INTERFACE      *This,
    IN EFI_PEI_SERVICES **PeiSerivces,
    IN UINT32           TcmInputParameterBlockSize,
    IN UINT8            *TcmInputParameterBlock,
    IN UINT32           TcmOutputParameterBlockSize,
    IN UINT8            *TcmOutputParameterBlock
);

typedef struct _AMI_PEI_TCM_INTERFACE
{
    PEI_TCM_HASH_LOG_EXTEND_EVENT TCMHashLogExtendEvent;
    PEI_TCM_LOG_EVENT             TCMLogEvent;
    PEI_TCM_PASS_THROUGH_TO_TCM   TCMPassThroughToTcm;
} AMI_PEI_TCM_INTERFACE;


#endif
