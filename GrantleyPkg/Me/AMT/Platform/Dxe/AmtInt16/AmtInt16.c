/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2011, Intel Corporation. All rights reserved.
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

--*/
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <CommonIncludes.h>
#include <Library/DebugLib.h>
#include "AmtInt16.h"

EFI_LEGACY_BIOS_PROTOCOL	*gLegacyBios	= NULL;
EFI_PHYSICAL_ADDRESS		Int16TSR;
UINTN						Int16TSR_Pages;

AMT_WRAPPER_PROTOCOL *pAmtWrapper = NULL;

#define INT16TSR_INIT		0
#define INT16TSR_HOOK		1
#define INT16TSR_RELEASE	2

BOOLEAN IsAmtConsoleLocked()
{
	BOOLEAN KeyboardLocked = FALSE;

	if (pAmtWrapper == NULL){
		gBS->LocateProtocol(&gEfiAmtWrapperProtocolGuid, NULL, &pAmtWrapper);
    }
    if (pAmtWrapper != NULL){
        if (pAmtWrapper->ActiveManagementConsoleLocked() == TRUE){
            KeyboardLocked = TRUE;
        }
    }

    return KeyboardLocked;
}

EFI_STATUS
LoadRealModeFile(
    EFI_GUID  				*Guid,
    VOID      				**Buffer,
    UINTN     				*BufferSize
)
{
    EFI_STATUS                    Status;
    EFI_FIRMWARE_VOLUME_PROTOCOL  *Fv;
    UINT32                        Authentication;
    UINTN                         NumHandles;
    EFI_HANDLE                    *HandleBuffer;
    UINTN                         i;

	*Buffer=0;
	*BufferSize=0;

    Status = gBS->LocateHandleBuffer(
						ByProtocol,
						&gEfiFirmwareVolumeProtocolGuid,
						NULL,
						&NumHandles,
						&HandleBuffer);
    if (EFI_ERROR(Status))
    	return Status;

    for (i = 0; i< NumHandles; ++i) {
        Status = gBS->HandleProtocol(HandleBuffer[i],&gEfiFirmwareVolumeProtocolGuid,&Fv);
        if (EFI_ERROR(Status)) continue;

        Status = Fv->ReadSection(Fv,
                                Guid,
                                EFI_SECTION_RAW,
                                0,        //Instance
                                Buffer,
                                BufferSize,
                                &Authentication);

        if (Status == EFI_SUCCESS)
        	break;
    }

    gBS->FreePool(HandleBuffer);

    return Status;
}

VOID CallINT16TSR(
	UINT16	Function,
	UINT16	P1,
	UINT16	P2,
	UINT16 	P3
)
{
	EFI_IA32_REGISTER_SET RegSet;
	UINT16 ip, cs;
	EFI_STATUS	Status;

	if (gLegacyBios == NULL) {
		Status = gBS->LocateProtocol (&gEfiLegacyBiosProtocolGuid, NULL, &gLegacyBios);
		ASSERT_EFI_ERROR(Status);
	}

	cs = (UINT16)(RShiftU64(Int16TSR,4));
	ip = (UINT16)(Int16TSR & 0xF);
	ip = ip + *(UINT16 *)(UINTN)Int16TSR;

	gBS->SetMem(&RegSet, sizeof (EFI_IA32_REGISTER_SET), 0);
	RegSet.X.AX = Function;
	RegSet.X.BX = P1;
	RegSet.X.CX = P2;
	RegSet.X.DX = P3;

	//DEBUG((EFI_D_ERROR, "<cs:0x%X>", cs));
	//DEBUG((EFI_D_ERROR, "<ip:0x%X>", ip));

	gLegacyBios->FarCall86 (
				gLegacyBios,
				cs,
				ip,
				&RegSet,
				NULL,
				0);

	//DEBUG((EFI_D_ERROR, "<ax=0x%X>", RegSet.X.AX));
	//DEBUG((EFI_D_ERROR, "<ds=0x%X>", RegSet.X.DS));
	//DEBUG((EFI_D_ERROR, "<es=0x%X>", RegSet.X.ES));
}

VOID
AmtINT16LegacyBoot(
    EFI_EVENT           Event,
    VOID                *ParentImageHandle
)
{

    DEBUG((EFI_D_ERROR, "<INT16TSR_RELEASE>"));
    CallINT16TSR(INT16TSR_RELEASE << 8,0,0,0);
    DEBUG((EFI_D_ERROR, "<INT16TSR_FREE>"));
    gBS->FreePages(Int16TSR, Int16TSR_Pages);
    gBS->CloseEvent(Event);
    return;
}

EFI_STATUS
EFIAPI
AmtInt16Entry (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
    EFI_STATUS 	Status;
	UINT8		*Buffer;
	UINTN		BufferSize;
	EFI_EVENT   LegacyBootEvent;

    //
    // Initialize EFI library
    //
    // EfiInitializeDriverLib (ImageHandle, SystemTable);

	DEBUG((EFI_D_ERROR, "<AmtInt16Entry>"));

	if (IsAmtConsoleLocked()) {
		Status = LoadRealModeFile(&gAmtInt16CsmGuid, &Buffer, &BufferSize);
		if (EFI_ERROR(Status)) {
			DEBUG((EFI_D_ERROR, "<FAILED>"));
		} else {
			DEBUG((EFI_D_ERROR, "<Buffer=0x%X,0x%X>", (UINTN)Buffer, BufferSize));

			Int16TSR = 0x98000;
			Int16TSR_Pages = BufferSize/0x1000+1;
			Status = gBS->AllocatePages(
						AllocateMaxAddress,
						EfiBootServicesData,
						Int16TSR_Pages,
						&Int16TSR);
			if (EFI_ERROR(Status))
				return Status;

			DEBUG((EFI_D_ERROR, "<Address=0x%X>", Int16TSR));

			gBS->CopyMem((VOID *)(UINTN)Int16TSR, Buffer, BufferSize);

		    DEBUG((EFI_D_ERROR, "<INT16TSR_INIT>"));
			CallINT16TSR(INT16TSR_INIT << 8,0,0,0);

			DEBUG((EFI_D_ERROR, "<INT16TSR_HOOK>"));
			CallINT16TSR(INT16TSR_HOOK << 8,0,0,0);

	  		Status = EfiCreateEventLegacyBootEx(
	                       TPL_CALLBACK,
	                       AmtINT16LegacyBoot,
	                       (VOID *)&ImageHandle,
	                       &LegacyBootEvent
	      				);
	  		ASSERT_EFI_ERROR (Status);

			gBS->FreePool(Buffer);
		}
	}

    return EFI_SUCCESS;
}

