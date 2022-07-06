//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**             6145-F Northbelt Pkwy, Norcross, GA 30071            **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************


/** @file PciStaticSetup.c
    Pci Bus Setup Routines

    @note  Use this hook to dynamically change PCI BUS Driver Setup screen 
 Strings.

**/

#include <AmiLib.h>
#include <AmiDxeLib.h>
#include <Setup.h>
#include <Protocol/PciIo.h>
#include <Token.h>

// Function Definitions


/**
    This function initializes the board related setup option values

    @param HiiHandle	Handle to HII database
    @param Class		Indicates the setup class

    @retval VOID

**/

VOID 
EFIAPI InitPciBusStrings(
	EFI_HII_HANDLE		HiiHandle, 
	UINT16				Class
)
{
   	if(Class == ADVANCED_FORM_SET_CLASS){
        InitString(HiiHandle, STRING_TOKEN(STR_PCI_DRIVER_VER), L"%X.%02d.%02d",
        PCI_BUS_MAJOR_VER, PCI_BUS_MINOR_VER, PCI_BUS_REVISION);
    }
	return;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**             6145-F Northbelt Pkwy, Norcross, GA 30071            **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
