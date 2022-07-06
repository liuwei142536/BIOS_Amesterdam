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
#ifndef TCMDXE_H
#define TCMDXE_H

#define _CRT_SECURE_NO_WARNINGS
#include <Uefi.h>
#include <AmiTcg/Tcm.h>

//
// TCM Function Common Call. For commnad StartUp, SelfTest, Enable, Disable, ForceClear...
//
typedef EFI_STATUS (EFIAPI *MP_TCMCOMCALL) ( 
    IN VOID
    );

//
// TCM Get TCM Status Info. Procedure.
//
typedef EFI_STATUS (EFIAPI *MP_GETSTATUS) ( 
    IN OUT UINT8         *EnabledSts,
    IN OUT UINT8         *ActivedSts,
    IN OUT UINT8         *OwnerSts 
    );

//
// TCG Pass Through TO TCM.
//
typedef EFI_STATUS (EFIAPI *MP_PASSTHROUGHTOTCM) ( 
  IN    TCM_PC_REGISTERS_PTR    TisReg,
  IN    UINT8                   *TcmInputParameterBlock,
  IN    UINT32                  TcmInputParameterBlockSize,
  IN    UINT8                   *TcmOutputParameterBlock,
  IN    UINT32                  TcmOutputParameterBlockSize 
    );

typedef struct _EFI_TCMSDRV_PROTOCOL {
    MP_TCMCOMCALL       Tcm_StartUp_Clear;   // mpTCMSTClr;
    MP_TCMCOMCALL       Tcm_StartUp_State;   // mpTCMSTSts;
    MP_TCMCOMCALL       Tcm_Enable;          // mpTCMEnable;
    MP_TCMCOMCALL       Tcm_Disable;         // mpTCMDisable;
    MP_TCMCOMCALL       Tcm_ClearOwner;      // mpTCMClrOwmer;
    MP_GETSTATUS        Tcm_GetStatus;       // mpTCMGetStatus;
    MP_PASSTHROUGHTOTCM TcmLibPassThrough;
} EFI_TCMSDRV_PROTOCOL;

#endif
