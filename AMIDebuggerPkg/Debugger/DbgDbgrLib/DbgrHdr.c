//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2014, American Megatrends, Inc.        **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
// $Header: /AptioV/BIN/Debugger/DxeDebugger/DbgDbgrLib/DbgrHdr.c 1     11/02/12 10:18a Sudhirv $
//
// $Revision: 1 $
//
// $Date: 11/02/12 10:18a $
//*****************************************************************
//*****************************************************************
// Revision History
// ----------------
// $Log: /AptioV/BIN/Debugger/DxeDebugger/DbgDbgrLib/DbgrHdr.c $
// 
// 1     11/02/12 10:18a Sudhirv
// Debugger eModule for AMI Debug for UEFI 3.0
// 
// 1     8/13/12 3:36p Mohammadm
// 
// 1     8/08/12 3:46p Sudhirv
// Debugger 3.0 Binary Version
// 
// 2     7/16/12 9:39p Sudhirv
// Updated
// 
// 1     7/15/12 8:16p Sudhirv
// Debugger Module for Aptio 5
// 
//
//*****************************************************************

//**********************************************************************
//<AMI_FHDR_START>
//
// Name:	DbgrHdr.c
//
// Description:	File containing the globals for CAR base address & size.
//				Also the code which make use of this so that it can be
//				integrated as it is Debugger eModule of binary.
//
//<AMI_FHDR_END>
//**********************************************************************

#include "Pei.h"
//#include "misc.h"
#ifndef EFIx64
#include <Library/AMIpeidebug.h>
#else
#include <Library/AMIpeidebugX64.h>
#endif
#include "Token.h"
//**********************************************************************
#define	PEI_DEBUG_DATASECTION_BASEADDRESS	GetDbgDataBaseAddress()	//Starting address of PEI debugger data section
#define PEI_DEBUG_DATASECTION_SIZE		    0x2048

UINTN Code_Start_Addr = (0xffffffff - FLASH_SIZE);
UINTN Code_End_Addr	= 0xffffffff;

//Defining NVRAM_HOB_GUID for reading NVRAM Details. 
#ifndef NVRAM_HOB_GUID
#define NVRAM_HOB_GUID \
    { 0xc0ec00fd, 0xc2f8, 0x4e47, 0x90, 0xef, 0x9c, 0x81, 0x55, 0x28, 0x5b, 0xec }
#endif

#ifndef SW_SMI_IO_ADDRESS
#define SW_SMI_IO_ADDRESS 0
#endif
#ifndef SMM_BASE_SAVE_STATE_OFFSET
#define SMM_BASE_SAVE_STATE_OFFSET 0
#endif
#ifndef MAX_SMM_SAVE_STATE_SIZE
#define MAX_SMM_SAVE_STATE_SIZE 0
#endif
#ifndef SMM_BSP_BASE
#define SMM_BSP_BASE 0
#endif
#ifndef TSEG_SIZE
#define TSEG_SIZE 0
#endif
#ifndef DBG_WRITE_IO_SUPPORT
#define DBG_WRITE_IO_SUPPORT 0
#endif
#ifndef DBG_WRITE_IO_80_SUPPORT
#define DBG_WRITE_IO_80_SUPPORT 0
#endif

#if SHELL_DEBUGGER_SUPPORT   //Defining NVRAM_HOB structure if build is for Shell debugger
EFI_GUID NvramHobGuid = NVRAM_HOB_GUID;
extern VOID *
EFIAPI
GetFirstGuidHob (
  IN CONST EFI_GUID         *Guid
  );

typedef struct {
    EFI_HOB_GUID_TYPE Header;
    EFI_PHYSICAL_ADDRESS NvramAddress;
    EFI_PHYSICAL_ADDRESS BackupAddress;
    UINT32 NvramSize;
    UINT32 HeaderLength;
    UINT32 NvramMode;
} NVRAM_HOB;
#endif

//For SMM Outcontext Support
volatile UINT16 gSMIIOAddress = SW_SMI_IO_ADDRESS;
volatile UINTN gSMMBaseSaveState = SMM_BASE_SAVE_STATE_OFFSET;
volatile UINTN gSMMSaveStateSize = MAX_SMM_SAVE_STATE_SIZE;
volatile UINTN gSMMBspBase = SMM_BSP_BASE;
volatile UINT32 gTsegSize = TSEG_SIZE;
volatile UINT32 gSMMTseg = 0;

//**********************************************************************

EFI_SYSTEM_CONTEXT			gContext = { NULL };

volatile UINTN gDbgWriteIOSupport = DBG_WRITE_IO_SUPPORT;

#ifdef DbgPerformanceMode_Support
volatile UINTN gDbgPerformanceMode = DbgPerformanceMode_Support;
#else
volatile UINTN gDbgPerformanceMode = 0;
#endif

#if defined RECORD_VERSION && (RECORD_VERSION >= 2)
UINT8 pmversion = 1;
#else
UINT8 pmversion = 0;
#endif

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	UpdateCARLimits
//
// Description:	Updates the CAR Limits
//
// Input:		UINTN , UINTN , UINTN 
//
// Output:		VOID
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>
void UpdateCARLimits(UINTN DbgBaseAddr, UINTN Base, UINTN Size)
{
#ifndef EFIx64
	PEIDebugData_T *pData;
	DbgGblData_T *DbgData;
	pData	= (PEIDebugData_T *)((UINT32)DbgBaseAddr);
	DbgData = (DbgGblData_T *)(pData->DbgrLocalVar);
#else
	PEIDebugData_Tx64 *pData;
	DbgGblData_Tx64 *DbgData;
	pData	= (PEIDebugData_Tx64 *)((UINT32)DbgBaseAddr);
	DbgData = (DbgGblData_Tx64 *)(pData->DbgrLocalVar);
#endif
	
	DbgData->m_CarBaseAddress = (UINT32)Base;
	DbgData->m_CarEndAddress = (UINT32)(Base+Size);
	
	return;
}

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	getNvramAddress
//
// Description:	returns the NVRAM_ADDRESS
//
// Input:		VOID
//
// Output:		UINT64
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>
UINT64 getNvramAddrRuntime(VOID)
{
#if SHELL_DEBUGGER_SUPPORT
    VOID        *GuidHob;
    NVRAM_HOB          *NvramHob;
    //Reading NVRAM Address from NVRAM_HOB, NVRAM dependency can be avoided if for Shell debugger
    GuidHob = GetFirstGuidHob (&NvramHobGuid);  
    if (GuidHob != NULL) {                      
        NvramHob = (NVRAM_HOB *)GuidHob;
        return NvramHob->NvramAddress;
    }
#endif
    return PcdGet64 (PcdNvramBaseAddres);
}

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:   getNvramSizeRuntime
//
// Description: returns the NVRam header size
//
// Input:       VOID
//
// Output:      UINT64
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>

UINT32 getNvramSizeRuntime()
{
#if SHELL_DEBUGGER_SUPPORT
    VOID        *GuidHob;
    NVRAM_HOB          *NvramHob;
    
    //Reading Header length from NVRAM_HOB, NVRAM dependency can be avoided if for Shell debugger
    GuidHob = GetFirstGuidHob (&NvramHobGuid);
    if (GuidHob != NULL) {                   
        NvramHob = (NVRAM_HOB *)GuidHob;     
        return NvramHob->HeaderLength;
    }
    return 0;
#else
    return PcdGet32(PcdNvramHeaderSize);
#endif

}

//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2014, American Megatrends, Inc.        **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//

