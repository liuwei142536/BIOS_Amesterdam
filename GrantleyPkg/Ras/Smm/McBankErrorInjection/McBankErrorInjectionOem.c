/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Copyright (c) 2012 Intel Corporation.  All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license.  Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:
    McBankErrorInjectionOem.c

Abstract:
    This file is to be update by OEM.

-----------------------------------------------------------------------------*/


#include "McBankErrorInjection.h"

extern UINT8	mSetupMcejSupport;

VOID
SetupInterface(
	)
/*
Routine Description:

 Access setup variable and updates the mSetupMcejSupport variable from setup data.

Arguments:
  
	none 

Returns:

	none
*/
{
	SYSTEM_CONFIGURATION				setupData;
	UINTN								varSize;
	EFI_STATUS							Status;

	// This variable is updated by setup option value of McbankErrorInjection
	mSetupMcejSupport	= 00;

	varSize = sizeof(SYSTEM_CONFIGURATION);
	// AptioV Server Override: Changed variable name to IntelSetup
	Status = gRT->GetVariable( L"IntelSetup", &gEfiSetupVariableGuid, NULL, &varSize, &setupData );
	if (EFI_ERROR(Status)) return;
	
	if(setupData.McBankErrorInjectionSupport)
		mSetupMcejSupport	= 01;

}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Copyright (c) 2012 Intel Corporation.  All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license.  Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

-----------------------------------------------------------------------------*/
