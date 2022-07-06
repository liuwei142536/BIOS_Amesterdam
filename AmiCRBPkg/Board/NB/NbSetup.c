//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2012, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

//*************************************************************************
// $Header: $
//
// $Revision:  $
//
// $Date:  $
//*************************************************************************

//<AMI_FHDR_START>
//-----------------------------------------------------------------------
//
// Name:        NbSetup.c
//
// Description: This C file contains code related to NB setup routines
//
//-----------------------------------------------------------------------
//<AMI_FHDR_END>

#include <Setup.h>
#include <AmiDxeLib.h>
#include <Guid/MemoryMapData.h>
#include <Library/HobLib.h>

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   InitNbStrings
//
// Description: Initializes North Bridge Setup String
//
// Input:       HiiHandle - Handle to HII database
//              Class - Indicates the setup class
//
// Output:      None
//
// Notes:       PORTING REQUIRED
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID
InitNbStrings(
    IN EFI_HII_HANDLE      HiiHandle,
    IN UINT16              Class
)
{
	EFI_HOB_GUID_TYPE             	*GuidHob;
	struct SystemMemoryMapHob     	*SystemMemoryMap;
	UINT32							MemorySize;

	if(Class == MAIN_FORM_SET_CLASS) {

 	    GuidHob = GetFirstGuidHob (&gEfiMemoryMapGuid);
 		if (GuidHob == NULL) {
 		    return;
 		}

 	    SystemMemoryMap = (struct SystemMemoryMapHob*) GET_GUID_HOB_DATA (GuidHob);
 	    MemorySize = SystemMemoryMap->memSize << 6;  //mamory size in 1MB units

            InitString(
        		    HiiHandle,
        		    STRING_TOKEN(STR_MEMORY_SIZE_VALUE),
        		    L"%4d MB",
        		    MemorySize
            );
        }
}

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2013, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
