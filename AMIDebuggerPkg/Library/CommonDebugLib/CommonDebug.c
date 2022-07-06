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
// $Header: /AptioV/BIN/AMIDebugRx/DbgHostStatusLib/CommonDebug.c 1     11/02/12 10:14a Sudhirv $
//
// $Revision: 1 $
//
// $Date: 11/02/12 10:14a $
//
//*********************************************************************
// Revision History
// ----------------
// $Log: /AptioV/SRC/AMIDebugRx/DbgHostStatusLib/CommonDebug.c $
// 
//
//*********************************************************************
//<AMI_FHDR_START>
//----------------------------------------------------------------------------
//
// Name:          CommonDebug.C
//
// Description:   Common Debug definitions
//
//----------------------------------------------------------------------------
//<AMI_FHDR_END>
#include "Efi.h"
#include "Pei.h"
#include "Include/Library/IoLib.h"
//#include "misc.h"
#ifndef	EFIx64
#include <Library/AMIpeidebug.h>
#else
#include <Library/AMIpeidebugX64.h>
#endif
#include <Library/AMIPeiGUIDs.h>

#include "Token.h"
#include <Library/PciExpressLib.h>

#ifdef REDIRECTION_ONLY_MODE
#if REDIRECTION_ONLY_MODE
volatile UINTN gRedirectionOnlyEnabled = 1;
#else
volatile UINTN gRedirectionOnlyEnabled = 0;
#endif
#endif

#ifdef DBG_PERFORMANCE_RECORDS
UINTN gDbgPerformanceRecords = DBG_PERFORMANCE_RECORDS;
#else
UINTN gDbgPerformanceRecords = 0;
#endif

#ifdef DBG_WRITE_IO_80_SUPPORT
volatile UINTN gDbgWriteIO80Support = DBG_WRITE_IO_80_SUPPORT;
#else
volatile UINTN gDbgWriteIO80Support = 0;
#endif

#ifdef SERIAL_SEC_DBG_SUPPORT
volatile UINT8 gSecDbgSupport = SERIAL_SEC_DBG_SUPPORT;
#else
volatile UINT8 gSecDbgSupport = 0;
#endif

#ifndef GENERIC_USB_CABLE_SUPPORT
#define GENERIC_USB_CABLE_SUPPORT 0
#endif

#ifdef USB_3_DEBUG_SUPPORT
UINT8 gDbgUSB3DebugSupport = USB_3_DEBUG_SUPPORT;
#else
UINT8 gDbgUSB3DebugSupport = 0;
#endif


#ifdef USB_3_DEBUG_DEBUGPORT_INIT_WAIT
UINT8 gDbgUSB3Wait = USB_3_DEBUG_DEBUGPORT_INIT_WAIT;
#else
UINT8 gDbgUSB3Wait = 0;
#endif

#define ATTR_MOVE_BYTE      1
#define ATTR_MOVE_WORD      2
#define ATTR_MOVE_DWORD     4

// Load Fv Support
UINTN gFvMainBase = FV_MAIN_BASE;
UINTN gFvMainBlocks = FV_MAIN_BLOCKS;
UINTN gFvBBBlocks = FV_BB_BLOCKS;
UINTN gBlockSize = FLASH_BLOCK_SIZE;

volatile UINTN gGenericUsbSupportEnabled = GENERIC_USB_CABLE_SUPPORT;

EFI_GUID  mPeiDbgBasePpiGuid = EFI_PEI_DBG_BASEADDRESS_PPI_GUID;

EFI_GUID  mDxeDbgDataGuid = DXE_DBG_DATA_GUID;
INT8 CompareGuid(EFI_GUID *G1, EFI_GUID *G2);

UINTN DebugDataBaseAddress = 0;
UINTN SMMDebugDataBaseAddress = 0;
UINTN DxeDataBaseAddress = 0;

UINT8
EFIAPI
DbgPciExpressRead8 (
  IN      UINTN                     Address
  );

UINT32
EFIAPI
DbgPciExpressRead32 (
  IN      UINTN                     Address
  );

UINT8
EFIAPI
DbgPciExpressWrite8 (
  IN      UINTN                     Address,
  IN      UINT8                     Value
  );

UINT16
EFIAPI
DbgPciExpressWrite16 (
  IN      UINTN                     Address,
  IN      UINT16                    Value
  );

UINT16
EFIAPI
DbgPciExpressWrite32 (
  IN      UINTN                     Address,
  IN      UINT32                    Value
  );

#include <Library/DebugLib.h>

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   PciExpressReadRegisterWrapper 
//
// Description: Wrapper function to read register from Pci configspace using 
//				PciExpressLib
//
// Input:       UINT8 Bus, UINT8 Dev, UINT8 Fun, UINT16 Reg
//
// Output:      UINT8 
//-------------------x---------------------------------------------------------
//<AMI_PHDR_END>
UINT8 PciExpressReadRegisterWrapper(UINT8 Bus, UINT8 Dev, UINT8 Fun, UINT16 Reg)
{
    return (DbgPciExpressRead8(PCI_EXPRESS_LIB_ADDRESS(Bus, Dev, Fun, Reg)));
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   PciExpressWriteRegisterWrapper 
//
// Description: Wrapper function to write register to Pci configspace using 
//				PciExpressLib
//
// Input:       UINT8 Bus, UINT8 Dev, UINT8 Fun, UINT16 Reg, VOID *Data, UINT8 DataSize
//
// Output:      UINT8 
//-------------------x---------------------------------------------------------
//<AMI_PHDR_END>
UINT32 PciExpressWriteRegisterWrapper(UINT8 Bus, UINT8 Dev, UINT8 Fun, UINT16 Reg, VOID *Data, UINT8 DataSize)
{
    if(Data == NULL)
        return 0;
    
    switch(DataSize)
    {
    case ATTR_MOVE_BYTE:
        return (DbgPciExpressWrite8(PCI_EXPRESS_LIB_ADDRESS(Bus, Dev, Fun, Reg), *((UINT8 *)Data)));
    case ATTR_MOVE_WORD:
        return (DbgPciExpressWrite16(PCI_EXPRESS_LIB_ADDRESS(Bus, Dev, Fun, Reg), *((UINT16 *)Data)));
    case ATTR_MOVE_DWORD:
        return (DbgPciExpressWrite32(PCI_EXPRESS_LIB_ADDRESS(Bus, Dev, Fun, Reg), *((UINT32 *)Data)));
    default:
        return 0;
    }
    
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   GetTsegBaseAddress 
//
// Description: Returns the base address of TSEG.
//
// Input:       None
//
// Output:      UINT32 - The Base Address of TSEG.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

UINT32 GetTsegBaseAddress (VOID)
{
    // The TSEG DRAM memory base address register is 0xB8 in the MC device(B:0D:0F:0).
    return (DbgPciExpressRead32(PCI_EXPRESS_LIB_ADDRESS(0, 0, 0, 0xB8)) & 0xFFF00000);    
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   GetTargetVersion 
//
// Description: Returns the Build version of the target eModule.
//
// Input:       None
//
// Output:      UINT8.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
UINT8 GetTargetVersion()
{
#ifdef AMI_DEBUG_BUILD
	return AMI_DEBUG_BUILD;
#else
	return 0;
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
