//**********************************************************************//
//**********************************************************************//
//**                                                                  **//
//**        (C)Copyright 1985-2008, American Megatrends, Inc.         **//
//**                                                                  **//
//**                       All Rights Reserved.                       **//
//**                                                                  **//
//**       5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093       **//
//**                                                                  **//
//**                       Phone: (770)-246-8600                      **//
//**                                                                  **//
//**********************************************************************//
//**********************************************************************//

//<AMI_FHDR_START>
//----------------------------------------------------------------------------
//
// Name:		RtErrorLogWhea.h
//
// Description:	Runtime error log WHEA includes and declarations
//
//----------------------------------------------------------------------------
//<AMI_FHDR_END>
#ifndef _WHEA_LOG_H
#define _WHEA_LOG_H

#include "token.h"
#include "RtErrorlogBoard.h"


EFI_STATUS
LogPciErrorToWhea(
		IN	  UINT8       ErrorSource,
		IN	  UINT8       ErrorType,
		IN	  UINT8       Segment,
		IN	  UINT8       Bus,
		IN	  UINT8       Device,
		IN	  UINT8       Function,
		IN 	  BOOLEAN     IsBridge,
    	IN    UINT16      SecondaryStatus,
    	IN    UINT8       PcieCapOffset,
    	IN    UINT16      PcieDeviceStatus,
    	IN    UINT16      AerOffset,
    	IN    UINT32      UncorrErrStatus,
    	IN    UINT32      CorrErrStatus,
    	IN    UINT32      RootErrStatus
    );

EFI_STATUS
LogMemErrorToWhea(
		MEMORY_DEV_INFO		*MemInfo
);

#endif
