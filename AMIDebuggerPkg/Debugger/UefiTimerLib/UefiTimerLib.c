//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2014, American Megatrends, Inc.        **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
// $Header: /AptioV/SRC/Debugger/UefiTimerLib/UefiTimerLib.c 1     1/12/17 8:16p Madhans $
//
// $Revision: 1 $
//
// $Date: 1/12/17 8:16p $
//*****************************************************************
//**********************************************************************
// Revision History
// ----------------
// $Log: /AptioV/SRC/Debugger/UefiTimerLib/UefiTimerLib.c $
// 
// 1     1/12/17 8:16p Madhans
// 
// 7     2/14/15 4:10p Sudhirv
// CPPCheck Changes
// 
// 6     2/13/15 4:35p Sudhirv
// ConsoleRedirection Exception packets Change
// 
// 5     2/11/15 1:08p Jekekumarg
// Fixed the Issue EIP-203977-SMM Entry / Exit not Booting to Shell. It
// Hangs in Post Screen
// 
// 4     10/14/14 3:06p Sudhirv
// Made Changes to clear the timer periodically
// 
// 3     10/13/14 8:06p Sudhirv
// Made Changes to update the AMI_PEI_TRANSPORT_PROTOCOL accurately
// 
// 2     7/23/14 5:48p Sudhirv
// [TAG]	EIP164829
// [Category]	Bug Fix
// [Severity:]	Normal
// [Symptom:]	'Target Reset' operation is sometimes not resetting the
// target when the target is in checkpoint 'AD'
// [Files]	AMISerialIO.c
// 
// 1     2/20/14 5:07p Sudhirv
// Added for AMI Debug for UEFI 3.01.0008 INT Label
// 
// 1     9/21/13 4:25a Sudhirv
// Added for AMI Debug for UEFI 3.01.0006 Label.
// 
// 1     11/02/12 10:06a Sudhirv
// USBRedirection eModule for AMI Debug for UEFI 3.0
// 
// 1     10/09/12 12:16p Sudhirv
// [TAG]     EIP99567
// [Category] New Feature
// [Severity] Normal
// [Symptom] UsbRedirectionPkg for Aptio V
// 
// 
//**********************************************************************
//*****************************************************************

//**********************************************************************
//<AMI_FHDR_START>
//
// Name:	AmiSerialIo.c
//
// Description:	Serial IO related functions
//
//<AMI_FHDR_END>
//**********************************************************************

#include "Efi.h"
#include <Protocol/Timestamp.h>
#include "Token.h"

EFI_BOOT_SERVICES 		*gBS;

EFI_TIMESTAMP_PROTOCOL *gTimeStampInterface = (EFI_TIMESTAMP_PROTOCOL*)NULL;
extern EFI_GUID gEfiTimestampProtocolGuid;


EFI_STATUS
EFIAPI
UefiTimerLibConstor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
	EFI_STATUS Status = EFI_SUCCESS;
	gBS = SystemTable->BootServices;
	if(EFI_SUCCESS != gBS->LocateProtocol(&gEfiTimestampProtocolGuid, NULL,(void**) &gTimeStampInterface))
		gTimeStampInterface = (EFI_TIMESTAMP_PROTOCOL*)NULL;
	
	return Status;
}



UINTN
EFIAPI
MicroSecondDelay (
  IN      UINTN                     MicroSeconds
  )
{
	return gBS->Stall(MicroSeconds);
  //return MicroSeconds;
}

UINT64
EFIAPI
GetPerformanceCounter (
  VOID
  )
{
	return gTimeStampInterface->GetTimestamp();
  //return 0;
}

UINT64
EFIAPI
GetPerformanceCounterProperties (
  OUT      UINT64                    *StartValue,  OPTIONAL
  OUT      UINT64                    *EndValue     OPTIONAL
  )
{
	EFI_TIMESTAMP_PROPERTIES TimeStampProperties;
	if(EFI_SUCCESS == gTimeStampInterface->GetProperties(&TimeStampProperties))
		return TimeStampProperties.Frequency;
	
  return (UINT64)(-1);
}

//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2014, American Megatrends, Inc.        **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
