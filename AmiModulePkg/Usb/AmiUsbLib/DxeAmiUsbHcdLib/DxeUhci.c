//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file DxeUhci.c
   AMI USB UHCI DXE driver
**/


#include <AmiDef.h>
#include <UsbDef.h>

UINT32  EFIAPI HcReadPciReg(HC_STRUC*, UINT32);
VOID    EFIAPI HcWordWritePciReg(HC_STRUC*, UINT32, UINT16);


/**
    This routine is call to enable legacy register
    
    @param HcStruc   Ptr to the host controller structure
    
    @retval EFI_SUCCESS    Success to enable legacy register
        
**/

EFI_STATUS
EFIAPI
UhciEnableLegacyReg(
    HC_STRUC    *HcStruc
)
{
    return EFI_SUCCESS;
}

/**
    This routine is call to check legacy register
    
    @param HcStruc   Ptr to the host controller structure
    
    @retval TRUE     Legacy regiser is enable
    @retval FALSE    Legacy regiser is disable
        
**/
UINT8
EFIAPI
UhciCheckLegacyReg(
    HC_STRUC    *HcStruc
)
{
    return TRUE;
}

/**
    This routine is call to disable legacy register
    
    @param HcStruc   Ptr to the host controller structure
    
    @retval EFI_SUCCESS    Success to disable legacy register
        
**/
EFI_STATUS
EFIAPI
UhciDisableLegacyReg(
    HC_STRUC    *HcStruc
)
{
    return EFI_SUCCESS;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
