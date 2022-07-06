/*++
 Copyright (c) 2009 Nationz Technologies Inc. All rights reserved

Module Name:

  Tcmsdrv.c

Abstract:

   Product:    TCM DXE Driver for Nationz V1.04  
   Author:     Nosa

   $Revision:  01.04.11.09 
   $Date:      12/14/2009

--*/

#include <Uefi.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DebugLib.h>
#include<Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Library/IoLib.h>
#include <Guid/MemoryOverwriteControl.h>
#include <Library/UefiLib.h>
#include <IndustryStandard/SmBios.h>
#include <AmiTcg/Tcm.h>
#include <AmiTcg/Tcmdxe.h>
#include "Tcmsdrv.h"

TCM_PC_REGISTERS_PTR      TCMHandle = (TCM_PC_REGISTERS_PTR)(UINTN)TCM_MMIO_BASE;

EFI_STATUS
EFIAPI
TcmLibPassThrough (
  IN    TCM_PC_REGISTERS_PTR    TisReg,
  IN    UINT8                   *TcmInputParameterBlock,
  IN    UINT32                  TcmInputParameterBlockSize,
  IN    UINT8                   *TcmOutputParameterBlock,
  IN    UINT32                  TcmOutputParameterBlockSize
);
 
EFI_STATUS
EFIAPI
Tcm_StartUp (
  IN      TCM_PC_REGISTERS_PTR           TcmHandle,
  IN      UINT16		                 StartupType
  )
{
    EFI_STATUS                   Status;
    TCM_CMD_STARTUP		         cmdStartup;   
    TCM_RSP_COMMAND_HDR          RspHdr;

    cmdStartup.Header.tag       = TCM_H2NS (TCM_TAG_RQU_COMMAND);
    cmdStartup.Header.paramSize = TCM_H2NL (sizeof (cmdStartup));
    cmdStartup.Header.ordinal   = TCM_H2NL (TCM_ORD_Startup);
    cmdStartup.StartupType      = TCM_H2NS (StartupType) ;

    Status = TcmLibPassThrough (
                TcmHandle,
                (UINT8*)&cmdStartup,
                sizeof(cmdStartup),
                (UINT8*)&RspHdr,
                sizeof(RspHdr)
                );

   return TcmCommCheckStatus(Status, &RspHdr, TCM_TAG_RSP_COMMAND);
}

EFI_STATUS
EFIAPI
Tcm_ContinueSelfTest (
  IN      TCM_PC_REGISTERS_PTR            TcmHandle
  )
{
    EFI_STATUS                   Status;   
    TCM_RQU_COMMAND_HDR          cmdSelfTest;
    TCM_RSP_COMMAND_HDR          RspHdr;

    cmdSelfTest.tag          = TCM_H2NS (TCM_TAG_RQU_COMMAND);
    cmdSelfTest.paramSize    = TCM_H2NL (sizeof (cmdSelfTest));
    cmdSelfTest.ordinal      = TCM_H2NL (TCM_ORD_ContinueSelfTest);

    Status = TcmLibPassThrough (
                TcmHandle,
                (UINT8*)&cmdSelfTest,
                sizeof(cmdSelfTest),
                (UINT8*)&RspHdr,
                sizeof(RspHdr)
                );

   return TcmCommCheckStatus(Status, &RspHdr, TCM_TAG_RSP_COMMAND);
}

EFI_STATUS
EFIAPI
Tcm_PhysicalEnable (
  IN      TCM_PC_REGISTERS_PTR            TcmHandle
  )
{
    EFI_STATUS                   Status;   
    TCM_RQU_COMMAND_HDR          cmdPhysicEn;
    TCM_RSP_COMMAND_HDR          RspHdr;

    cmdPhysicEn.tag          = TCM_H2NS (TCM_TAG_RQU_COMMAND);
    cmdPhysicEn.paramSize    = TCM_H2NL (sizeof (cmdPhysicEn));
    cmdPhysicEn.ordinal      = TCM_H2NL (TCM_ORD_PhysicalEnable);

    Status = TcmLibPassThrough (
                TcmHandle,
                (UINT8*)&cmdPhysicEn,
                sizeof(cmdPhysicEn),
                (UINT8*)&RspHdr,
                sizeof(RspHdr)
                );

   return TcmCommCheckStatus(Status, &RspHdr, TCM_TAG_RSP_COMMAND);
}

EFI_STATUS
EFIAPI
Tcm_PhysicalDisable (
  IN      TCM_PC_REGISTERS_PTR            TcmHandle
  )
{
    EFI_STATUS                   Status;   
    TCM_RQU_COMMAND_HDR          cmdPhysicDis;
    TCM_RSP_COMMAND_HDR          RspHdr;

    cmdPhysicDis.tag          = TCM_H2NS (TCM_TAG_RQU_COMMAND);
    cmdPhysicDis.paramSize    = TCM_H2NL (sizeof (cmdPhysicDis));
    cmdPhysicDis.ordinal      = TCM_H2NL (TCM_ORD_PhysicalDisable);

    Status = TcmLibPassThrough (
                TcmHandle,
                (UINT8*)&cmdPhysicDis,
                sizeof(cmdPhysicDis),
                (UINT8*)&RspHdr,
                sizeof(RspHdr)
                );

   return TcmCommCheckStatus(Status, &RspHdr, TCM_TAG_RSP_COMMAND);
}

EFI_STATUS
EFIAPI
Tcm_PhysicalSetDeactivated (
  IN      TCM_PC_REGISTERS_PTR            TcmHandle,
  IN      BOOLEAN                         TureFalse
  )
{
    EFI_STATUS                                Status;   
    TCM_CMD_PHYSICAL_SET_DEACTIVATED          cmdPhysicSetDea;
    TCM_RSP_COMMAND_HDR                       RspHdr;

    cmdPhysicSetDea.Header.tag          = TCM_H2NS (TCM_TAG_RQU_COMMAND);
    cmdPhysicSetDea.Header.paramSize    = TCM_H2NL (sizeof (cmdPhysicSetDea));
    cmdPhysicSetDea.Header.ordinal      = TCM_H2NL (TCM_ORD_PhysicalSetDeactivated);
    cmdPhysicSetDea.Flag                = TureFalse;

    Status = TcmLibPassThrough (
                TcmHandle,
                (UINT8*)&cmdPhysicSetDea,
                sizeof(cmdPhysicSetDea),
                (UINT8*)&RspHdr,
                sizeof(RspHdr)
                );

   return TcmCommCheckStatus(Status, &RspHdr, TCM_TAG_RSP_COMMAND);
}

EFI_STATUS
EFIAPI
Tcm_ForceClear (
  IN      TCM_PC_REGISTERS_PTR            TcmHandle
  )
{
    EFI_STATUS                   Status;   
    TCM_RQU_COMMAND_HDR          cmdForceClr;
    TCM_RSP_COMMAND_HDR          RspHdr;

    cmdForceClr.tag          = TCM_H2NS (TCM_TAG_RQU_COMMAND);
    cmdForceClr.paramSize    = TCM_H2NL (sizeof (cmdForceClr));
    cmdForceClr.ordinal      = TCM_H2NL (TCM_ORD_ForceClear);

    Status = TcmLibPassThrough (
                TcmHandle,
                (UINT8*)&cmdForceClr,
                sizeof(cmdForceClr),
                (UINT8*)&RspHdr,
                sizeof(RspHdr)
                );

   return TcmCommCheckStatus(Status, &RspHdr, TCM_TAG_RSP_COMMAND);
}

EFI_STATUS
EFIAPI
Tcm_GetCapability (
  IN      TCM_PC_REGISTERS_PTR      TcmHandle,
  IN      UINT32 	                capArea,
  IN      UINT32 	                subCap,
  IN      UINT32                    respSize,  
  OUT     UINT8                     *resp
  )
{
    EFI_STATUS                   Status;  
    TCM_GET_CAPABILITY_IN        cmdGetCap;
    TCM_CMD_GET_CAPABILITY_OUT   RspHdr;

    cmdGetCap.Header.tag        = TCM_H2NS (TCM_TAG_RQU_COMMAND);
    cmdGetCap.Header.paramSize  = TCM_H2NL (sizeof (cmdGetCap));
    cmdGetCap.Header.ordinal    = TCM_H2NL (TCM_ORD_GetCapability);
    cmdGetCap.capArea           = TCM_H2NL (capArea); 
    cmdGetCap.subCapSize        = TCM_H2NL (TCM_CAP_FLAG);
    cmdGetCap.subCap            = TCM_H2NL (subCap);

    Status = TcmLibPassThrough (
                TcmHandle,
                (UINT8*)&cmdGetCap,
                sizeof(cmdGetCap),
                (UINT8*)&RspHdr,
                sizeof(RspHdr.Header) + sizeof(RspHdr.subSize) + respSize
                );

    gBS->CopyMem(resp, RspHdr.resp, respSize);

    return TcmCommCheckStatus(Status, &RspHdr.Header, TCM_TAG_RSP_COMMAND);
}

EFI_STATUS
EFIAPI
Tcm_StartUp_Clear (
  IN VOID
  )
{
    EFI_STATUS         Status;
 
    Status = Tcm_StartUp (TCMHandle, TCM_ST_CLEAR);
    if(Status)
    {
 //       Print(L"Tcm StartUp Clear error!\n");
        DEBUG ((EFI_D_ERROR, "Tcm StartUp Clear error, returned %r\n", Status));
        return Status;
    }    

    Status = Tcm_ContinueSelfTest (TCMHandle);
    if(Status)
    {
 //       Print(L"Tcm ContinueSelfTest error!\n");
        DEBUG ((EFI_D_ERROR, "Tcm ContinueSelfTest error, returned %r\n", Status));
    }

    return Status;
}

EFI_STATUS
EFIAPI
Tcm_StartUp_State (
  IN VOID
  )
{
    EFI_STATUS         Status;
 
    Status = Tcm_StartUp (TCMHandle, TCM_ST_STATE);
    if(Status)
    {
 //       Print(L"Tcm StartUp State error!\n");
        DEBUG ((EFI_D_ERROR, "Tcm Startup state error, returned %r\n", Status));
        return Status;
    }    

    Status = Tcm_ContinueSelfTest (TCMHandle);
    if(Status)
    {
 //       Print(L"Tcm ContinueSelfTest error!\n");
        DEBUG ((EFI_D_ERROR, "Tcm ContinueSelfTest error, returned %r\n", Status));
    }

    return Status;
}

EFI_STATUS
EFIAPI
Tcm_Enable (
  IN VOID
  )
{
    EFI_STATUS         Status;
 
    Status = Tcm_PhysicalEnable (TCMHandle);
    if(Status)
    {
 //       Print(L"Tcm Physical Enable error!\n");
        DEBUG ((EFI_D_ERROR, "Tcm Physical enable error, returned %r\n", Status));
        return Status;
    }    

    Status = Tcm_PhysicalSetDeactivated (TCMHandle, FALSE);
    if(Status)
    {
 //       Print(L"Tcm Set Activate error!\n");
        DEBUG ((EFI_D_ERROR, "Tcm Set activate error, returned %r\n", Status));
    }    
 
    return Status;
}

EFI_STATUS
EFIAPI
Tcm_Disable (
  IN VOID
  )
{
    EFI_STATUS         Status;
 
    Status = Tcm_PhysicalSetDeactivated (TCMHandle, TRUE);  
    if(Status)
    {
 //       Print(L"Tcm Set Deactivate error!\n");
        DEBUG ((EFI_D_ERROR, "Tcm Set deactivate error, returned %r\n", Status));
        return Status;
    }    

    Status = Tcm_PhysicalDisable (TCMHandle);
    if(Status)
    {
 //       Print(L"Tcm Physical Disable error!\n");
        DEBUG ((EFI_D_ERROR, "Tcm Physical disable error, returned %r\n", Status));
    }    

    return Status;
}

EFI_STATUS
EFIAPI
Tcm_ClearOwner (
  IN VOID
  )
{
    EFI_STATUS         Status;
 
    Status = Tcm_ForceClear (TCMHandle);
    if(Status)
    {
 //       Print(L"Tcm Force Clear error!\n");
        DEBUG ((EFI_D_ERROR, "Tcm Force clear error, returned %r\n", Status));
    }    

    return Status;
}

EFI_STATUS
EFIAPI
Tcm_GetStatus (
  IN OUT UINT8         *EnabledSts,
  IN OUT UINT8         *ActivedSts,
  IN OUT UINT8         *OwnerSts 
  )
{
    EFI_STATUS              Status;
    UINT8                   PFlag[PFlagLen];
    UINT8                   ownerSet;
 
    Status = Tcm_GetCapability(TCMHandle, TCM_CAP_FLAG, TCM_CAP_FLAG_PERMANENT, sizeof(PFlag), PFlag);
    if(Status)
    {
//        Print(L"Tcm Get PFlag error! %r\n",Status);
        DEBUG ((EFI_D_ERROR, "Tcm Get Pflag error, returned %r\n", Status));
        return Status;
    }

     if(PFlag[2] == 0)
	        *EnabledSts = 1;
	 else
	        *EnabledSts = 0;
    
     if(PFlag[4] == 0)
	        *ActivedSts = 1;
	 else
	        *ActivedSts = 0;

    Status = Tcm_GetCapability(TCMHandle, TCM_CAP_PROPERTY, TCM_CAP_PROP_OWNER, sizeof(ownerSet), &ownerSet);        	
	if(Status)
    {
 //       Print(L"Tcm Get VFlag error!\n");
        DEBUG ((EFI_D_ERROR, "Tcm Get Vflag error, returned %r\n", Status));
        return Status;
    }
	 
	 if( ownerSet == 0)
	        *OwnerSts = 0;
	 else
	        *OwnerSts = 1;

	 return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
TcmCommCheckStatus (
    IN      EFI_STATUS              Status,
    IN      TCM_RSP_COMMAND_HDR     *RspHdr,
    IN      TCM_STRUCTURE_TAG       RspHdrTag
 )
{  
      if(Status != EFI_SUCCESS)
        return Status;

      if( RspHdr->tag != TCM_H2NS(RspHdrTag) )
      {
//        Print(L"TCM no response.\n");
        DEBUG ((EFI_D_ERROR, "Tcm no response.\n"));
        Status = EFI_NO_RESPONSE;
      }

      return Status;
}

//
// TCM Smart Driver PROTOCOL 
//
EFI_TCMSDRV_PROTOCOL g_TcmFunc = {
    Tcm_StartUp_Clear,
    Tcm_StartUp_State,
    Tcm_Enable,
    Tcm_Disable,
    Tcm_ClearOwner,
    Tcm_GetStatus,
    TcmLibPassThrough
};


/*++

Routine Description:

  Tcm smart driver entry point function. 

Arguments:

  ImageHandle   - image handle for this driver image
  SystemTable   - pointer to the EFI System Table

Returns:

  EFI_SUCCESS   - driver initialization completed successfully
  OTHER- driver is not loaded

--*/
EFI_STATUS TcmMP_EntryPoint(
    IN EFI_HANDLE           ImageHandle, 
    IN EFI_SYSTEM_TABLE     *SystemTable
)
{
    EFI_STATUS 			 Status = EFI_SUCCESS;
    EFI_HANDLE           NewHandle;
    UINT16               VID;

    //Check if TCM
    VID = TCMHandle->vid;
    if( VID != TCM_VID )
        return EFI_UNSUPPORTED;        

    NewHandle = NULL;
    Status = gBS->InstallProtocolInterface (
                  &NewHandle,
                  &gEfiTcmMPProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &g_TcmFunc
                  );

    return Status;
}
