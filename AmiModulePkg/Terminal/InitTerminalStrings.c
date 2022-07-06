//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file InitTerminalStrings.c
    Contains InitTerminalStrings(), which is linked to 
    SetupStringInit in the sdl page.  Before entering setup,
    we do any modification or updating needed for the serial
    port name strings (STR_SIO_SERIAL_PORT0, etc.).

**/

//----------------------------------------------------------------------------
#include <AmiLib.h>
#include <AmiDxeLib.h>
#include <Setup.h>
#include <Token.h>
#include "TerminalSetupVar.h"
#include <Protocol/HiiString.h>

//----------------------------------------------------------------------------

#ifndef HII_LIBRARY_FUNCTIONS_SUPPORT
EFI_STATUS HiiLibGetString(
	IN EFI_HII_HANDLE HiiHandle, 
    IN STRING_REF StringId, 
	IN OUT UINTN *StringSize, 
    OUT EFI_STRING String
);
EFI_STATUS HiiLibSetString(
	IN EFI_HII_HANDLE HiiHandle, 
    IN STRING_REF StringId, 
    IN EFI_STRING String
);
#endif

STRING_REF gSioSerialPortStrRef[MAX_SIO_SERIAL_PORTS] = {
    STRING_TOKEN(STR_SIO_SERIAL_PORT0),
    STRING_TOKEN(STR_SIO_SERIAL_PORT1),
    STRING_TOKEN(STR_SIO_SERIAL_PORT2),
    STRING_TOKEN(STR_SIO_SERIAL_PORT3),
    STRING_TOKEN(STR_SIO_SERIAL_PORT4),
    STRING_TOKEN(STR_SIO_SERIAL_PORT5),
    STRING_TOKEN(STR_SIO_SERIAL_PORT6),
    STRING_TOKEN(STR_SIO_SERIAL_PORT7),
    STRING_TOKEN(STR_SIO_SERIAL_PORT8),
    STRING_TOKEN(STR_SIO_SERIAL_PORT9)
};

STRING_REF gPciSerialPortStrRef[MAX_PCI_SERIAL_PORTS] = {
    STRING_TOKEN(STR_PCI_SERIAL_PORT0),
    STRING_TOKEN(STR_PCI_SERIAL_PORT1),
    STRING_TOKEN(STR_PCI_SERIAL_PORT2),
    STRING_TOKEN(STR_PCI_SERIAL_PORT3)
};

UINT32 gTotalSioSerialPorts = TOTAL_SIO_SERIAL_PORTS;
UINT32 gTotalPciSerialPorts = TOTAL_PCI_SERIAL_PORTS;

EFI_GUID gTerminalVarGuid = TERMINAL_VAR_GUID;
TERMINAL_SERIAL_SUPPORT 	gTerminalSerialSupport;

static EFI_HII_STRING_PROTOCOL *HiiString=NULL;

/**
   @internal
   
    This function will return the supported languages.

    @param  HiiHandle

    @retval CHAR8* - supported languages are returned

   @endinternal
**/
CHAR8* 
TerminalGetSupportedLanguages (
    IN EFI_HII_HANDLE   HiiHandle
)
{
    EFI_STATUS  Status;
    CHAR8 *SupportedLanguages = NULL;
    UINTN Size = 0;
    
    // Retrieve list of supported languages
    if (HiiHandle == NULL)
        return NULL;
    if (!HiiString && EFI_ERROR(pBS->LocateProtocol(&gEfiHiiStringProtocolGuid, NULL, (VOID**)&HiiString)))
        return NULL;
    
    Status = HiiString->GetLanguages(HiiString, HiiHandle, SupportedLanguages, &Size);
    if (Status != EFI_BUFFER_TOO_SMALL) return NULL;
    SupportedLanguages = Malloc(Size);
    Status = HiiString->GetLanguages(HiiString, HiiHandle, SupportedLanguages, &Size);
    if (EFI_ERROR(Status)){
        pBS->FreePool(SupportedLanguages);
        return NULL;
    }
    return SupportedLanguages;
}

/**
   @internal
   
    This function will updates a string value in the Hii database based on the input string and 
    setting the updated string to the input string for all supportedlanguages

    @param  HiiHandle
    @param  InputStringId
    @param  OutStringId
    @param  SupportedLangs

    @retval EFI_STATUS

   @endinternal
**/

EFI_STATUS
UpdateSetupString (
    EFI_HII_HANDLE HiiHandle, 
    IN STRING_REF InputStringId, 
    IN STRING_REF OutStringId, 
    CHAR8 *SupportedLangs
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN          StringSize = 0x80;
    CHAR16         String[0x80];
    CHAR16         String2[0x80];
    CHAR8          *Supported;
    CHAR8          *Language;
    CHAR8          *AllocatedLanguages;
    
    if (SupportedLangs == NULL)
        return EFI_INVALID_PARAMETER;
    //AllocatedLanguages = AllocateCopyPool (AsciiStrSize (SupportedLanguages), SupportedLanguages);
    Status = pBS->AllocatePool(EfiBootServicesData,
                                AsciiStrSize(SupportedLangs), 
                                (VOID**)&AllocatedLanguages);
    if (EFI_ERROR(Status))
        return Status;
    
    pBS->CopyMem(AllocatedLanguages, SupportedLangs, AsciiStrSize (SupportedLangs));
        
    for (Supported = AllocatedLanguages; *Supported != '\0'; ) {
        // Cache a pointer to the beginning of the current language in the list of languages
        Language = Supported;
        // Search for the next language seperator and replace it with a Null-terminator
        for (; *Supported != 0 && *Supported != ';'; Supported++);
            if (*Supported != 0) {
              *(Supported++) = '\0';
            }
        
        if (AsciiStrnCmp (Language, UEFI_CONFIG_LANG, AsciiStrLen (UEFI_CONFIG_LANG)) == 0) {
          // Skip string package used for keyword protocol.
          continue;
        }

        StringSize = 0x80;
        Status = HiiString->GetString(HiiString, Language, HiiHandle,InputStringId, String, &StringSize, NULL);
        if(!EFI_ERROR(Status)){
            // StringSize will be altered when GetString() is called, so initialize it for each time 
            StringSize = 0x80;
            Status = HiiString->GetString(HiiString, Language, HiiHandle,OutStringId, String2, &StringSize, NULL);
            if(EFI_ERROR(Status)){
                continue;
            }
            StrCat(String, String2);
            Status = HiiString->SetString(HiiString, HiiHandle, InputStringId, Language, String, NULL);
        }
    }
    
    pBS->FreePool(AllocatedLanguages);
    return Status;
}

/**
    Certain strings in the terminal setup page can only be
    updated at runtime, which is what this function does.
    The pci serial port names are changed to contain the
    location (device and function numbers) of the port.
    If any serial ports are disabled, "Disabled"
    is concatenated to their names.

    @param  HiiHandle
    @param  Class

    @retval VOID

**/

VOID 
EFIAPI
InitTerminalStrings(
    IN EFI_HII_HANDLE HiiHandle, 
    IN UINT16 Class
)
{
#if (TOTAL_SERIAL_PORTS > 0)

    EFI_STATUS Status = EFI_SUCCESS;
    UINT32      gTerminalSerialSupportVarAttributes=0;
    UINTN       gTerminalSerialSupportVarSize = sizeof(TERMINAL_SERIAL_SUPPORT);
    CHAR8       *SupportedLanguages = NULL;
    UINT32 		i = 0;
    
#if (TOTAL_PCI_SERIAL_PORTS > 0)
    CHAR16 		Str2[512]=L"";
    CHAR16      Str[512]=L"";
    UINTN       Size=512;
#endif
    
    if ( EFI_ERROR(pBS->LocateProtocol(&gEfiHiiStringProtocolGuid, NULL, (VOID**)&HiiString)) )  {
        return;
	}
    
    //Get the supported languages
    SupportedLanguages = TerminalGetSupportedLanguages(HiiHandle);
    if (Class==ADVANCED_FORM_SET_CLASS) {
        
        Status = pRS->GetVariable(TERMINAL_SERIAL_VAR_C_NAME,
                                  &gTerminalVarGuid,
                                  &gTerminalSerialSupportVarAttributes,
                                  &gTerminalSerialSupportVarSize,
                                  &gTerminalSerialSupport);
        
        if (EFI_ERROR(Status)) {
            return;
        }
        for (i = 0; i < gTotalSioSerialPorts; i++) {
            if (!gTerminalSerialSupport.SerialPortsEnabledVar.PortsEnabled[i]) {
                
                //Update the strings in Hii database for all SupportedLanguages
                Status = UpdateSetupString(HiiHandle,
                                            gSioSerialPortStrRef[i],
                                            STRING_TOKEN(STR_COM_DISABLED),
                                            SupportedLanguages
                                            );
                if(EFI_ERROR(Status)) {
                    continue;
                }
            }
        }

#if (TOTAL_PCI_SERIAL_PORTS > 0)
        for (i = 0; i < gTotalPciSerialPorts; i++) {
            Size = 512;
            Status = HiiLibGetString(HiiHandle,
                                        gPciSerialPortStrRef[i],
                                        &Size, Str);
            if (!EFI_ERROR(Status)) {
                if(gTerminalSerialSupport.PciSerialPortsLocationVar.AmiPciSerialPresent[i] == TRUE) {
                    //
                    // If AMI Serial Protcocol was installed for this PCI Serial device,
                    // then display the device like COM3(Pci Bus2,Dev10,Func1,Port0)
                    // with Port information.
                    //
                    Swprintf(Str2, L"COM%X(Pci Bus%d,Dev%d,Func%d,Port%d)",
                                        gTotalSioSerialPorts+i,
                                        gTerminalSerialSupport.PciSerialPortsLocationVar.Bus[i],
                                        gTerminalSerialSupport.PciSerialPortsLocationVar.Device[i],
                                        gTerminalSerialSupport.PciSerialPortsLocationVar.Function[i],
                                        gTerminalSerialSupport.PciSerialPortsLocationVar.Port[i]);
                } else {
                    Swprintf(Str2, L"COM%X(Pci Bus%d,Dev%d,Func%d)",
                                        gTotalSioSerialPorts+i,
                                        gTerminalSerialSupport.PciSerialPortsLocationVar.Bus[i],
                                        gTerminalSerialSupport.PciSerialPortsLocationVar.Device[i],
                                        gTerminalSerialSupport.PciSerialPortsLocationVar.Function[i]);
                }
                
                StrCat(Str, Str2);
                InitString(HiiHandle, gPciSerialPortStrRef[i], Str);
                if (!gTerminalSerialSupport.SerialPortsEnabledVar.PortsEnabled[gTotalSioSerialPorts+i]) {
                    Status = UpdateSetupString(HiiHandle,
                                                gPciSerialPortStrRef[i],
                                                STRING_TOKEN(STR_COM_DISABLED),
                                                SupportedLanguages
                                                );
                    if(EFI_ERROR(Status)) {
                        continue;
                    }
                }
            }
        }
#endif
    }
#endif
}

#ifndef HII_LIBRARY_FUNCTIONS_SUPPORT
#include <Protocol/Hii.h>

static EFI_HII_PROTOCOL *Hii = NULL;

static EFI_STATUS HiiLibGetString(
	IN EFI_HII_HANDLE HiiHandle,
    IN STRING_REF StringId,
	IN OUT UINTN *StringSize,
    OUT EFI_STRING String
)
{
    if (Hii == NULL && EFI_ERROR(pBS->LocateProtocol(&gEfiHiiProtocolGuid, NULL, (VOID**)&Hii)))
        return EFI_NOT_FOUND;

    return Hii->GetString(Hii, HiiHandle, StringId, TRUE, NULL, StringSize, String);
}

static EFI_STATUS HiiLibSetString(
	IN EFI_HII_HANDLE HiiHandle, 
    IN STRING_REF StringId,
    IN EFI_STRING String
)
{
    if (Hii == NULL && EFI_ERROR(pBS->LocateProtocol(&gEfiHiiProtocolGuid, NULL, (VOID**)&Hii)))
        return EFI_NOT_FOUND;

    return Hii->NewString(Hii, L"   ", HiiHandle, &StringId, String);
}
#endif


