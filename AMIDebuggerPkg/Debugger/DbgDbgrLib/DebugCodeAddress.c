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

//**********************************************************************
// $Header: /Alaska/Tools/template.c 6     1/13/10 2:13p Felixp $
//
// $Revision: 6 $
//
// $Date: 1/13/10 2:13p $
//**********************************************************************
// Revision History
// ----------------
// $Log: /Alaska/Tools/template.c $
// 
// 6     1/13/10 2:13p Felixp
// 
//**********************************************************************
//<AMI_FHDR_START>
//
// Name:  <This File's Name>
//
// Description:	
//
//<AMI_FHDR_END>
//**********************************************************************

#include <AmiDxeLib.h>

#ifndef EFIx64

extern UINTN Code_Start_Addr;
extern UINTN Code_End_Addr;

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:   Get_Code_StartAddr
//
// Description: Internal Helper function.
//
// Input:       none
//
// Output:      Get_Code_StartAddr (UINTN)
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>

UINTN   Get_Code_StartAddr()
{
    return (Code_Start_Addr);
}

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:   Get_Code_EndAddr
//
// Description: Internal Helper function.
//
// Input:       none
//
// Output:      Code_End_Addr (UINTN)
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>
UINTN   Get_Code_EndAddr()
{
    return (Code_End_Addr);
}

#endif
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
