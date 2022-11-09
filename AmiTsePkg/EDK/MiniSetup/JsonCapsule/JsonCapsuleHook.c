//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file JsonCapsuleHook.c
    Triggers the ProcessJsonConfigCapsuleEvent.
**/

#include <AmiDxeLib.h>

EFI_STATUS EfiLibNamedEventSignal (IN EFI_GUID  *Name );

/**
    Triggers the ProcessJsonConfigCapsuleEvent

    @param Handle 

    @retval VOID
**/
VOID ProcessJsonConfigCapsuleHook(VOID)
{   
    EfiLibNamedEventSignal (&gAmiTseProcessCapsuleGuid);        
}
