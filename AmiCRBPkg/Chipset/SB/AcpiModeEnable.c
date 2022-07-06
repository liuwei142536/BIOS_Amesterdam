//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2011, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
// $Header: /Alaska/BIN/Chipset/Intel/SouthBridge/PatsburgPCH/Chipset/AcpiModeEnable.c 1     12/09/11 1:19p Archanaj $
//
// $Revision: 1 $
//
// $Date: 12/09/11 1:19p $
//**********************************************************************
// Revision History
// ----------------
// $Log: /Alaska/BIN/Chipset/Intel/SouthBridge/PatsburgPCH/Chipset/AcpiModeEnable.c $
// 
// 1     12/09/11 1:19p Archanaj
// 
// 2     7/07/10 2:56p Bhimanadhunik
// Intial check-in for Patsburg.
// 
// 4     4/12/10 5:19a Wesleychen
// [CATEGORY]  Improvement
// [REASON]    Update for coding standard.
// [TAG]       None
// 
// 3     3/18/10 4:53a Tonywu
// Add "AcpiEnableCallbackList" and "AcpiDisableCallbackList" elink in the
// AcpiModeEnable driver.
// 
// 2     3/11/10 9:27a Tonywu
// Remove S3 resume elink from the ACPI Mode Enable driver.
// 
// 1     2/24/10 11:04p Tonywu
// Intel Cougar Point/SB eChipset initially releases.
//
//**********************************************************************

//<AMI_FHDR_START>
//---------------------------------------------------------------------------
//
// Name:  AcpiModeEnable.c
//
// Description: Provide functions to enable and disable ACPI mode
//
//---------------------------------------------------------------------------
//<AMI_FHDR_END>

//----------------------------------------------------------------------------
// Include(s)
//----------------------------------------------------------------------------

#include <AmiDxeLib.h>
#include <Token.h>
#include <AmiCspLib.h>
#include <Protocol\SmmSwDispatch2.h>
#include "AcpiModeEnable.h"
#include <AcpiModeELinks.h>


//----------------------------------------------------------------------------
// Variable and External Declaration(s)
//----------------------------------------------------------------------------
// Variable Declaration(s)

ACPI_DISPATCH_LINK  *gAcpiEnDispatchHead = 0, *gAcpiEnDispatchTail = 0;
ACPI_DISPATCH_LINK  *gAcpiDisDispatchHead = 0, *gAcpiDisDispatchTail = 0;

UINT16 wPM1_SaveState;
UINT32 dGPE_SaveState;

// GUID Definition(s)
EFI_GUID gEfiAcpiEnDispatchProtocolGuid = EFI_ACPI_EN_DISPATCH_PROTOCOL_GUID;
EFI_GUID gEfiAcpiDisDispatchProtocolGuid = EFI_ACPI_DIS_DISPATCH_PROTOCOL_GUID;

extern INIT_FUNCTION ACPIMODE_ENABLE_LIST EndOfAcpiModeEnList;
INIT_FUNCTION* AcpiModeEnList[]= {ACPIMODE_ENABLE_LIST NULL};

extern INIT_FUNCTION ACPIMODE_DISABLE_LIST EndOfAcpiModeDisList;
INIT_FUNCTION* AcpiModeDisList[]= {ACPIMODE_ENABLE_LIST NULL};

//----------------------------------------------------------------------------

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   EnableAcpiMode
//
// Description: This function enable ACPI mode by clearing all SMI and
//              enabling SCI generation
//              This routine is also called on a S3 resume for special ACPI
//              programming.
//              Status should not be cleared on S3 resume. Enables are
//              already taken care of.
//
// Input:  DispatchHandle - Handle to the Dispatcher
//              DispatchContext - SW SMM dispatcher context
//
// Output:     None
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS EnableAcpiMode(
	IN EFI_HANDLE  DispatchHandle,
	IN CONST VOID  *Context         OPTIONAL,
	IN OUT VOID    *CommBuffer      OPTIONAL,
	IN OUT UINTN   *CommBufferSize  OPTIONAL)
{
    ACPI_DISPATCH_LINK      *Link;
    UINTN i = 0;
    
#if defined EMUL6064_SUPPORT && EMUL6064_SUPPORT == 1
  if (READ_PCI8_SB(ICH_REG_LPC_ULKMC))
    WRITE_PCI8_SB(ICH_REG_LPC_ULKMC, 0);
#endif

    //Disable SMI Sources : SW SMI Timer
     RESET_IO8_PM(ICH_IOREG_SMI_EN, BIT06);
     
    // And enable SMI on write to SLP_EN when SLP_TYP is written
     SET_IO8_PM(ICH_IOREG_SMI_EN, BIT04);     

    //Disable and Clear PM1 Sources except power button   
    wPM1_SaveState = READ_IO16_PM(ICH_IOREG_PM1_EN); //PM1_EN 
    SET_IO16_PM(ICH_IOREG_PM1_EN, BIT08);
    WRITE_IO16_PM(ICH_IOREG_PM1_STS, 0xffff);

    //Disable and Clear GPE0 Sources
    dGPE_SaveState = READ_IO16_PM(ICH_IOREG_GPE0_EN); //GPE0_EN
    WRITE_IO16_PM(ICH_IOREG_GPE0_EN, 0);
    WRITE_IO32_PM(ICH_IOREG_GPE0_STS + 0, 0);
    WRITE_IO32_PM(ICH_IOREG_GPE0_STS + 4, 0);

    //Set day of month alarm invalid - ACPI 1.0 section 4.7.2.4
    WRITE_IO8(CMOS_ADDR_PORT, 0x0d |0x80);         //RTC_REGD
    WRITE_IO8(CMOS_DATA_PORT, 0);

    //Enable Sci
    SET_IO8_PM(ICH_IOREG_PM1_CNT, BIT00);

    for (i = 0; AcpiModeEnList[i] ; i++)
    	 AcpiModeEnList[i](DispatchHandle, Context);

    for (Link = gAcpiEnDispatchHead; Link; Link = Link->Link)
    {
        Link->Function(Link);
    }

   WRITE_IO8(0x80, SW_SMI_ACPI_ENABLE);

    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   DisableAcpiMode
//
// Description: This function disables ACPI mode by enabling SMI generation
//
// Input:  DispatchHandle - Handle to the Dispatcher
//              DispatchContext - SW SMM dispatcher context
//
// Output:     None
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS DisableAcpiMode(
		IN EFI_HANDLE  DispatchHandle,
		IN CONST VOID  *Context         OPTIONAL,
		IN OUT VOID    *CommBuffer      OPTIONAL,
		IN OUT UINTN   *CommBufferSize  OPTIONAL)
{
    ACPI_DISPATCH_LINK *Link;
    UINTN i = 0;

    //TODO: What about SW SMI Timer?
    //Disable SW SMI Timer
//#### byteValue = IoRead8(PM_BASE_ADDRESS + 0x30);  //SMI_EN (SMI Control and Enable register.)
//#### byteValue |= 0x40;         //SWSMI_TMR_EN = 1;
//#### IoWrite8(PM_BASE_ADDRESS + 0x30, byteValue);

    //Clear PM Sources and set Enables
    WRITE_IO16_PM(ICH_IOREG_PM1_STS, 0xffff);
    WRITE_IO16_PM(ICH_IOREG_PM1_EN, wPM1_SaveState);

    //Clear GPE0 Sources and set Enables
    WRITE_IO32_PM(ICH_IOREG_GPE0_STS + 0, 0xffffffff);  //GPE0_STS
    WRITE_IO32_PM(ICH_IOREG_GPE0_STS + 4, 0xffffffff);  //GPE0_STS
    WRITE_IO32_PM(ICH_IOREG_GPE0_EN, dGPE_SaveState); //GPE0_EN


    //Disable SCI
    RESET_IO8_PM(ICH_IOREG_PM1_CNT, BIT00);

    for (i = 0; AcpiModeDisList[i] != NULL; i++)
    	 AcpiModeDisList[i](DispatchHandle, Context);

    for (Link = gAcpiDisDispatchHead; Link; Link = Link->Link)
    {
        Link->Function(Link);
    }

    WRITE_IO8(0x80, SW_SMI_ACPI_DISABLE);

    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   AddLink
//
// Description: Create and add link to specified list.
//
// Input:  Size -
//              Head -
//              Tail -
//
// Output:     VOID Pointer
//
// Modified:
//
// Referrals:   SmmAllocatePool
//
// Notes:       Here is the control flow of this function:
//              1. Allocate Link in Smm Pool.
//              2. Add Link to end.
//              3. Return Link address.
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID * AddLink(
    IN UINT32       Size,
    IN VOID         **Head,
    IN VOID         **Tail)
{
    VOID *Link;

    if (pSmst->SmmAllocatePool(EfiRuntimeServicesData, Size, &Link) != EFI_SUCCESS) return 0;

    ((GENERIC_LINK*)Link)->Link = 0;
    if (!*Head)
    {
        *Head = *Tail = Link;
    }
    else
    {
        ((GENERIC_LINK*)*Tail)->Link = Link;
        *Tail = Link;
    }

    return Link;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   RemoveLink
//
// Description: Remove link from specified list.
//
// Input:  Handle - EFI Handle
//              Head -
//              Tail -
//
// Output:     BOOLEAN
//                  TRUE if link was removed. FALSE if link not in the list.
//
// Modified:
//
// Referrals:   SmmFreePool
//
// Notes:       Here is the control flow of this function:
//              1. Search link list for Link.
//              2. Remove link from list.
//              3. Free link.
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

BOOLEAN RemoveLink(
    IN EFI_HANDLE   Handle,
    IN VOID         **Head,
    IN VOID         **Tail)
{
    GENERIC_LINK *PrevLink, *Link;

    PrevLink = *Head;

    // Is link first. Link address is the same as the Handle.
    if (((GENERIC_LINK*)*Head) == Handle)
    {
        if (PrevLink == *Tail) *Tail = 0; // If Tail = Head, then 0.
        *Head = PrevLink->Link;
        pSmst->SmmFreePool(PrevLink);
        return TRUE;
    }

    // Find Link.
    for (Link = PrevLink->Link; Link; PrevLink = Link, Link = Link->Link)
    {
        if (Link == Handle)     // Link address is the same as the Handle.
        {
            if (Link == *Tail) *Tail = PrevLink;
            PrevLink->Link = Link->Link;
            pSmst->SmmFreePool(Link);
            return TRUE;
        }
    }

    return FALSE;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   EfiAcpiEnRegister
//
// Description: Register a Link on ACPI enable SMI.
//
// Input:  This -
//              Function -
//              Context -
//
//
// Output:     Handle -
//              EFI_STATUS
//
// Modified:    gAcpiEnDispatchHead, gAcpiEnDispatchTail
//
// Referrals:   AddLink
//
// Notes:       Here is the control flow of this function:
//              1. Verify if Context if valid. If invalid,
//                 return EFI_INVALID_PARAMETER.
//              2. Allocate structure and add to link list.
//              3. Fill link.
//              4. Enable Smi Source.
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS EfiAcpiEnRegister(
    IN EFI_ACPI_DISPATCH_PROTOCOL   *This,
    IN EFI_ACPI_DISPATCH            Function,
    OUT EFI_HANDLE                  *Handle)
{
    ACPI_DISPATCH_LINK *NewLink;

    NewLink = AddLink(sizeof(ACPI_DISPATCH_LINK), \
                      &gAcpiEnDispatchHead, \
                      &gAcpiEnDispatchTail);
    if (!NewLink) return EFI_OUT_OF_RESOURCES;

    NewLink->Function   = Function;
    *Handle = NewLink;

    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   EfiAcpiEnUnregister
//
// Description: Unregister a Link on ACPI enable SMI.
//
// Input:  This -
//              Handle -
//
// Output:     EFI_STATUS
//
// Modified:    gAcpiEnDispatchHead, gAcpiEnDispatchTail
//
// Referrals:   RemoveLink
//
// Notes:       Here is the control flow of this function:
//              1. Remove link. If no link, return EFI_INVALID_PARAMETER.
//              2. Disable SMI Source if no other handlers using source.
//              3. Return EFI_SUCCESS.
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS EfiAcpiEnUnregister(
    IN EFI_ACPI_DISPATCH_PROTOCOL   *This,
    IN EFI_HANDLE                   Handle)
{
    if (!RemoveLink(Handle, &gAcpiEnDispatchHead, &gAcpiEnDispatchTail))
        return EFI_INVALID_PARAMETER;
    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   EfiAcpiDisRegister
//
// Description: Register a Link on ACPI disable SMI.
//
// Input:  This -
//              Function -
//              *Context -
//
//
// Output:     Handle - EFI Handle
//              EFI_STATUS
//
// Modified:    gAcpiDisDispatchHead, gAcpiDisDispatchTail
//
// Referrals:   AddLink
//
// Notes:       Here is the control flow of this function:
//              1. Verify if Context if valid. If invalid,
//                 return EFI_INVALID_PARAMETER.
//              2. Allocate structure and add to link list.
//              3. Fill link.
//              4. Enable Smi Source.
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS EfiAcpiDisRegister(
    IN EFI_ACPI_DISPATCH_PROTOCOL   *This,
    IN EFI_ACPI_DISPATCH            Function,
    OUT EFI_HANDLE                  *Handle)
{
    ACPI_DISPATCH_LINK *NewLink;

    NewLink = AddLink(sizeof(ACPI_DISPATCH_LINK), \
                      &gAcpiDisDispatchHead, \
                      &gAcpiDisDispatchTail);
    if (!NewLink) return EFI_OUT_OF_RESOURCES;

    NewLink->Function   = Function;
    *Handle = NewLink;

    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   EfiAcpiDisUnregister
//
// Description: Unregister a Link on ACPI Disable SMI.
//
// Input:  This -
//              Handle - EFI Handle
//
// Output:     EFI_STATUS
//
// Modified:    gAcpiDisDispatchHead, gAcpiDisDispatchTail
//
// Referrals:   RemoveLink
//
// Notes:       Here is the control flow of this function:
//              1. Remove link. If no link, return EFI_INVALID_PARAMETER.
//              2. Disable SMI Source if no other handlers using source.
//              3. Return EFI_SUCCESS.
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS EfiAcpiDisUnregister(
    IN EFI_ACPI_DISPATCH_PROTOCOL   *This,
    IN EFI_HANDLE                   Handle)
{
    if (!RemoveLink(Handle, &gAcpiDisDispatchHead, &gAcpiDisDispatchTail))
        return EFI_INVALID_PARAMETER;
    return EFI_SUCCESS;
}

EFI_ACPI_DISPATCH_PROTOCOL gEfiAcpiEnDispatchProtocol = \
        {EfiAcpiEnRegister, EfiAcpiEnUnregister};

EFI_ACPI_DISPATCH_PROTOCOL gEfiAcpiDisDispatchProtocol = \
        {EfiAcpiDisRegister, EfiAcpiDisUnregister};


EFI_STATUS InSmmFunction(
    IN EFI_HANDLE           ImageHandle,
    IN EFI_SYSTEM_TABLE     *SystemTable)
{
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   AcpiModeEnableInit
//
// Description: installs appropriate ACPI enable/disable Dispatch Protocol.
//
// Input:  ImageHandle - Image handle
//              *SystemTable - Pointer to the system table
//
// Output:     EFI_STATUS
//
// Modified:
//
// Referrals:   InitAmiLib, InitSmmHandler
//
// Notes:
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS AcpiModeEnableInit(
    IN EFI_HANDLE           ImageHandle,
    IN EFI_SYSTEM_TABLE     *SystemTable)
{
    EFI_SMM_SW_DISPATCH2_PROTOCOL *SwDispatch2;
    EFI_SMM_SW_REGISTER_CONTEXT AcpiEnableContext = {SW_SMI_ACPI_ENABLE};
    EFI_SMM_SW_REGISTER_CONTEXT AcpiDisableContext = {SW_SMI_ACPI_DISABLE};

    EFI_STATUS  Status;
    EFI_HANDLE  Handle;
    EFI_HANDLE  DummyHandle = NULL;

    InitAmiSmmLib(ImageHandle, SystemTable);

    Status = pSmst->SmmLocateProtocol(&gEfiSmmSwDispatch2ProtocolGuid, NULL, &SwDispatch2);
    if (EFI_ERROR(Status)) return Status;

    Status = SwDispatch2->Register(SwDispatch2,
                                  EnableAcpiMode, 
                                  &AcpiEnableContext, 
                                  &Handle);
    if (EFI_ERROR(Status)) return Status;

    Status = pSmst->SmmInstallProtocolInterface (
                      &DummyHandle,
                      &gEfiAcpiEnDispatchProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      &gEfiAcpiEnDispatchProtocol
                      );
    if (EFI_ERROR(Status)) return Status;

    Status = SwDispatch2->Register(SwDispatch2,
                                  DisableAcpiMode, 
                                  &AcpiDisableContext, 
                                  &Handle);

    if (EFI_ERROR(Status)) return Status;

    DummyHandle = NULL;
    Status = pSmst->SmmInstallProtocolInterface (
                      &DummyHandle,
                      &gEfiAcpiDisDispatchProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      &gEfiAcpiDisDispatchProtocol
                      );

    return Status;
}
//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2010, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
