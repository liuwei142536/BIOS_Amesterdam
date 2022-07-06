/*++
 Copyright (c) 2009 Nationz Technologies Inc. All rights reserved

Module Name:

  TisPc.c

Abstract:

  TIS (TCM Interface Specification) functions

   Product:    TCM DXE Driver for Nationz V1.04  
   Author:     Nosa

   $Revision:  01.04.11.09 
   $Date:      12/14/2009

--*/

#include <Uefi.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DebugLib.h>
#include<Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Library/IoLib.h>
#include <Guid/MemoryOverwriteControl.h>
#include <Library/UefiLib.h>
#include "TisPc.h"
#include <AmiTcg/Tcm.h>
#include <Library/TimerLib.h>

#define TIS_TIMEOUT_NONE            ((UINT64) -1)
//#define TIS_TIMEOUT_A               gTisTimeout[0]
//#define TIS_TIMEOUT_B               gTisTimeout[1]
//#define TIS_TIMEOUT_C               gTisTimeout[2]
//#define TIS_TIMEOUT_D               gTisTimeout[3]

UINT64    gTisTimeout[4] = {
  1ull << 18, // = ~750 ms on CPU running @ ~12GHz
  1ull << 20, // = ~3 second on CPU running @ ~12GHz
  1ull << 18, // = ~750 ms on CPU running @ ~12GHz
  1ull << 18  // = ~750 ms on CPU running @ ~12GHz
};	


UINT32 AmiTcmSwapBytes32(IN UINT32 Data)
{
	UINT32 Data32;
	Data32 = Data << 24 | (Data & 0xff00) << 8 | (Data & 0xff0000) >> 8 | Data >>24;
	return Data32;
}

UINT8
EFIAPI
AmiTcmMmioRead8 (
  IN      UINTN                     Address
  )
{
  UINT8                             Value;

  Value = *(volatile UINT8*)Address;
  return Value;
}

UINT8
EFIAPI
tisRegRead (
  IN      UINTN                     Address
  )
{
  UINT8                             Value;

  Value = *(volatile UINT8*)Address;
  return Value;
}


void
EFIAPI
tisRegWrite (
  IN      UINTN                     Address,
  IN      UINT8                     Value
  )
{

  *(UINT8*)Address = Value;

}

VOID
NewIODelay(
    IN UINT16 DelayCount
)
{    		
	while(DelayCount--)
	{
        MicroSecondDelay(30);
	}
}




//el2015 >>>

/**
  Check whether TPM chip exist.

  @param[in] locality  Locality number to be checked.

  @retval    TRUE     TPM chip exists.
  @retval    FALSE    TPM chip is not found.
**/
static BOOLEAN TisPcPresenceCheck (void)
{
	UINT8	RegRead;

	RegRead = tisRegRead (TPM_ACCESS_ADDR(0));
	//printf("RegRead = %d\n",RegRead);
	//printf("(UINT8)-1 =%d\n",(UINT8)-1);
	//if( TRUE == (BOOL)(RegRead != (UINT8)-1)  )
	//printf("Return TRUE\n");
	//else
	//printf("Return FALSE\n");
	//printf("Return Result  = %d\n",(BOOL)(RegRead != (UINT8)-1));
	return (BOOLEAN)(RegRead != (UINT8)-1);
}

/**
  Check whether the value of a TPM chip register satisfies the input BIT setting.

  @param[in]  TisReg       Address port of register to be checked.
  @param[in]  BitSet       Check these data bits are set.
  @param[in]  BitClear     Check these data bits are clear.
  @param[in]  TimeOut      The max wait time (unit MicroSecond) when checking register.

  @retval     TIS_SUCCESS  The register satisfies the check bit.
  @retval     TIS_TIMEOUT  The register can't run into the expected status in time.
**/
static TIS_STATUS TisPcwaitRegisterBits (
	UINT32 TisReg,
	UINT8 BitSet,
	UINT8 BitClear,
	UINT32 TimeOut)
{
	UINT8 RegRead;
	UINT32	WaitTime;

	for (WaitTime = 0; WaitTime < TimeOut; WaitTime += 1) {
		RegRead = tisRegRead (TisReg);
		if ((RegRead & BitSet) == BitSet && (RegRead & BitClear) == 0)
			return TIS_SUCCESS;
		NewIODelay(1);
	}
	return TIS_TIMEOUT;
}

/**
  Get BurstCount by reading the burstCount field of a TIS regiger
  in the time of default TIS_TIMEOUT_D.

  @param[in]  locality		Locality number.
  @param[out] BurstCount	Pointer to a buffer to store the got BurstConut.

  @retval     TIS_SUCCESS           Get BurstCount.
  @retval     TIS_INVALID_PARAMETER TisReg is NULL or BurstCount is NULL.
  @retval     TIS_TIMEOUT           BurstCount can't be got in time.
**/
static TIS_STATUS TisPcReadBurstCount (
	UINT8 locality,
	UINT16 *BurstCount)
{
	UINT32	WaitTime;
	UINT8	DataByte0;
	UINT8	DataByte1;

	if (BurstCount == NULL) {
		return TIS_INVALID_PARAMETER;
	}

	WaitTime = 0;
	do {
		DataByte0 = tisRegRead(TPM_BURST_ADDR(locality));
		DataByte1 = tisRegRead(TPM_BURST_ADDR(locality) + 1);
		*BurstCount = (UINT16)((DataByte1 << 8) + DataByte0);
		if (*BurstCount != 0) {
			return TIS_SUCCESS;
		}
		NewIODelay(1);
		WaitTime += 1;
	} while (WaitTime < TIS_TIMEOUT_D);

	return TIS_TIMEOUT;
}

/**
  Set TPM chip to ready state by sending ready command TPM_STATUS_READY
  to Status Register in time.

  @param[in] locality	Locality number.

  @retval    TIS_SUCCESS           TPM chip enters into ready state.
  @retval    TIS_INVALID_PARAMETER TisReg is NULL.
  @retval    TIS_TIMEOUT           TPM chip can't be set to ready state in time.
**/

static TIS_STATUS TisPcPrepareCommand (UINT8 locality)
{
	TIS_STATUS Status;

	tisRegWrite(TPM_STATUS_ADDR(locality), TPM_STATUS_READY);
	Status = TisPcwaitRegisterBits (
				TPM_STATUS_ADDR(locality),
				TPM_STATUS_READY,
				0,
				TIS_TIMEOUT_B
				);
	return Status;
}


/**
  Get the control of TPM chip by sending requestUse command TPM_ACCESS_REQUEST
  to ACCESS Register in the time of default TIS_TIMEOUT_A.

  @param[in] locality	Locality number.

  @retval    TIS_SUCCESS				Get the control of TPM chip.
  @retval    TIS_INVALID_PARAMETER		TisReg is NULL.
  @retval    TIS_LOCALITY_NOT_ENABLE	Locality is not to be enabled.
  @retval    TIS_TIMEOUT				Can't get the TPM control in time.
**/
static TIS_STATUS TisPcRequestUseTpm (UINT8 locality)
{
	TIS_STATUS Status;
	UINT8 RegRead;

	/*
	if (!TisPcPresenceCheck (locality)) {
		return FALSE;
	}
	*/
	RegRead = tisRegRead(TPM_ACCESS_ADDR(locality));
	
	if (RegRead == 0xff) {
		return TIS_LOCALITY_NOT_ENABLE;
	}
	
	tisRegWrite (TPM_ACCESS_ADDR(locality), TPM_ACCESS_REQUEST);
	Status = TisPcwaitRegisterBits (
				TPM_ACCESS_ADDR(locality),
				(UINT8)(TPM_ACCESS_ACTIVE | TPM_ACCESS_VALID),
				0,
				TIS_TIMEOUT_A
				);
	return Status;
}


/**
  Send a command to TPM for execution and return response data.

  @param[in]      locality      Locality number.
  @param[in]      BufferIn      Buffer for command data.
  @param[in]      SizeIn        Size of command data.
  @param[in, out] BufferOut     Buffer for response data.
  @param[in, out] SizeOut       Size of response data.

  @retval TIS_SUCCESS           		Operation completed successfully.
  @retval TIS_TIMEOUT           		The register can't run into the expected status in time.
  @retval TIS_SEND_BUFFER_TOO_SMALL		Response data buffer is too small.
  @retval TIS_NOT_FOUND					TPM chip is not found.
  @retval TIS_RECEIVE_BUFFER_TOO_SMALL	Receive data buffer is too small.
**/
TIS_STATUS TisTpmCommand (
	UINT8 locality,
	UINT8 *BufferIn,
	UINT32 SizeIn,
	UINT8 *BufferOut,
	UINT32 *SizeOut)
{
	TIS_STATUS	Status;
	UINT16	BurstCount;
	UINT32	Index;
	UINT32	TpmOutSize;
	//UINT16	Data16;
	UINT32	Data32;
	//tisMemEnable();
	//*SizeOut = 4096;
	if(!TisPcPresenceCheck ()) {
		//DBG(("tdd_Info - TPM chip is not found!\n"));
		return TIS_NOT_FOUND;
	}

	Status = TisPcRequestUseTpm (locality);
	if (Status != TIS_SUCCESS) {
		//DBG(("tdd_Info -  Locality [%u] not ready for control!\n", locality));
		return Status;
	}
	/*
	DBG(("tdd_send - "));
	for (Index = 0; Index < SizeIn; Index++) {
		DBG(("%02x ", BufferIn[Index]));
	}
	DBG(("\n"));
	*/
	TpmOutSize = 0;

	Status = TisPcPrepareCommand (locality);
	if (Status != TIS_SUCCESS) {
		//DBG(("tdd_Info - TPM is not ready for command!\n"));
		return Status;
	}

	/* Send the command to TPM */
	Index = 0;
	while (Index < SizeIn) {
		Status = TisPcReadBurstCount (locality, &BurstCount);
		if (Status != TIS_SUCCESS) {
			Status =TIS_TIMEOUT;
			goto Exit;
		}
		for (; BurstCount > 0 && Index < SizeIn; BurstCount--) {
			tisRegWrite(TPM_DATA_ADDR(locality), *(BufferIn + Index));
			Index++;
		}
	}

	/* Check the TPM status STS_EXPECT change from 1 to 0 */
	Status = TisPcwaitRegisterBits (
				TPM_STATUS_ADDR(locality),
				(UINT8)TPM_ACCESS_VALID,
				TPM_STATUS_EXPECT,
				TIS_TIMEOUT_C);
	if (Status != TIS_SUCCESS) {
		//DBG(("tdd_Info - The send buffer too small!\n"));
		Status = TIS_SEND_BUFFER_TOO_SMALL;
		goto Exit;
	}

	/* Executed the TPM command and waiting for the response data reday */
	tisRegWrite(TPM_STATUS_ADDR(locality), TPM_STATUS_GO);
	Status = TisPcwaitRegisterBits (
				TPM_STATUS_ADDR(locality),
				(UINT8)(TPM_ACCESS_VALID | TPM_STATUS_AVAIL),
				0,
				TIS_TIMEOUT_B);
	if (Status != TIS_SUCCESS) {
		//DBG(("tdd_Info - Wait for TPM response data time out!\n"));
		Status = TIS_TIMEOUT;
		goto Exit;
	}

	/* Get Response data header */
	Index = 0;
	BurstCount = 0;
	while (Index < TPM_RESPONSE_HEADER_SIZE) {
		Status = TisPcReadBurstCount (locality, &BurstCount);
		if (Status != TIS_SUCCESS) {
			Status = TIS_TIMEOUT;
			goto Exit;
		}
		for (; BurstCount > 0; BurstCount--) {
			*(BufferOut + Index) = tisRegRead(TPM_DATA_ADDR(locality));
			Index++;
			if (Index == TPM_RESPONSE_HEADER_SIZE) break;
		}
	}

	/*
	printf("tdd_header: ");
	for (Index = 0; Index < TPM_RESPONSE_HEADER_SIZE; Index++) {
		printf ("%02x ", BufferOut[Index]);
	}
	printf ("\n");
	*/

	/* Check the response data header (Tag, Parasize and Returncode) */
	//memcpy(&Data32, (BufferOut + 2), sizeof (UINT32));
	gBS->CopyMem(&Data32, (BufferOut + 2), sizeof (UINT32));
	TpmOutSize = AmiTcmSwapBytes32(Data32);

	/*
	printf("tdd_debug - TpmOutSize = %u\n", TpmOutSize);
	printf("tdd_debug - SizeOut = %u\n", *SizeOut);
	*/

	if (*SizeOut < TpmOutSize) {
		Status = TIS_RECEIVE_BUFFER_TOO_SMALL;
		//DBG(("tdd_Info - The receive buffer too small!\n"));
		goto Exit;
	}
	*SizeOut = TpmOutSize;

	/* Continue reading the remaining data */
	while ( Index < TpmOutSize ) {
		for (; BurstCount > 0; BurstCount--) {
			*(BufferOut + Index) = tisRegRead(TPM_DATA_ADDR(locality));
			Index++;
			if (Index == TpmOutSize) {
				Status = TIS_SUCCESS;
				goto Exit;
			}
		}
		Status = TisPcReadBurstCount (locality, &BurstCount);
		if (Status != TIS_SUCCESS) {
			Status = TIS_TIMEOUT;
			goto Exit;
		}
	}
Exit:
	/*
	if (Status == TIS_SUCCESS) {
		DBG(("tdd_receive - "));
		for (Index = 0; Index < TpmOutSize; Index++) {
			DBG (("%02x ", BufferOut[Index]));
		}
		DBG (("\n"));
		DBG (("\n"));
	}*/
	TisPcPrepareCommand (locality);
	tisRegWrite (TPM_ACCESS_ADDR (locality), TPM_ACCESS_ACTIVE);

	return Status;
}


EFI_STATUS
EFIAPI
TcmLibPassThrough (
  IN    TCM_PC_REGISTERS_PTR    TisReg,
  IN    UINT8                   *TcmInputParameterBlock,
  IN    UINT32                  TcmInputParameterBlockSize,
  IN    UINT8                   *TcmOutputParameterBlock,
  IN    UINT32                  TcmOutputParameterBlockSize
)
{
  EFI_STATUS                        Status = 0;
  UINT8 locality;
  UINT32 *SizeOut;
  UINT32 TcmOutputSize;
  
	//Status = TisPcRequestUseTpm (TisReg);
	//if (Status != EFI_SUCCESS) {
		//DBG(("tdd_Info -  Locality [%u] not ready for control!\n", locality));
	//	return Status;
	//}
  TcmOutputSize = TcmOutputParameterBlockSize;
  SizeOut = &TcmOutputSize;
  locality = (UINT8)(((UINTN)TisReg - TPM_BASE)/TPM_LOCALITY_SIZE);
  Status = TisTpmCommand (locality, TcmInputParameterBlock, TcmInputParameterBlockSize, TcmOutputParameterBlock, SizeOut);
  return Status;

}
//el2015 <<<



