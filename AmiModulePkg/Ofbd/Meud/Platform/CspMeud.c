//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file CspMeud.c
**/
//----------------------------------------------------------------------------
// Includes
// Statements that include other files
#include "Efi.h"
#include "Token.h"
#include <AmiLib.h>
#include <AmiDxeLib.h>
#include "CspMeud.h"
#include <Protocol/SmmSwDispatch.h>
//Aptio V Server Override Start : EFI_SMM_HECI_PROTOCOL structure change conflict resolution
//###DEBUG #include "Protocol/HeciSmm.h"
#include "Protocol\HeciProtocol.h"
#include "../../GrantleyPkg/Me/Include/Protocol/HeciSmm.h"
//Aptio V Server Override End : EFI_SMM_HECI_PROTOCOL structure change conflict resolution
#include <Library/PciLib.h>
#include <Meud.h>
#include <CspMeud.h>
#include <Sps.h>
#include <Flash.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/AmiBufferValidationLib.h> // Security Update changes
BOOLEAN	IsIgnition;
BOOLEAN	Is_GPIO33_JMPR;
BOOLEAN Is_SECOVR_MSG;
UINT64	Nounce;
UINT32	Factory_Base;
UINT32	Factory_Limit;

// Aptio V Server Override Start: EFI_SMM_HECI_PROTOCOL structure change conflict resolution
//###DEBUG SMM_HECI_PROTOCOL            *SmmHeci = NULL;
EFI_SMM_HECI_PROTOCOL            *SmmHeci = NULL;
// Aptio V Server Override End: EFI_SMM_HECI_PROTOCOL structure change conflict resolution

//Aptio V server override Start: Me update changes.

UINT8   gMeStateBeforeFlashUpdate = 0xFF;

#define SPS_INIT_TIMEOUT        2000000 // [us] Timeout when waiting for InitComplete
#define SPS_RESET_TIMEOUT       5000000
#define STALL_1US               1
#define STALL_1MS               1000
#define STALL_1S                1000000

UINTN
EFIAPI
MicroSecondDelay (
  IN      UINTN                     MicroSeconds
  );
//Aptio V server override End: Me update changes.
FLASH_REGIONS_DESCRIPTOR
FlashRegionsDescriptor[] =
{
    { FDT_BLK, "FDR" },
    { BIOS_BLK, "BIOS" },
    { GBE_BLK, "GBER" },
    { PDR_BLK, "PDR" },
    { DER_BLK, "DER" },
    { BIOS_2ND_BLK, "BIOS" },
    { REG7_BLK, "REG7" },
    { REG8_BLK, "BMC" },
    { REG9_BLK, "DER2" },
    { REGA_BLK, "IE" },
    { REGB_BLK, "GBEA" },
    { REGC_BLK, "REGC" },
    { REGD_BLK, "REGD" },
    { REGE_BLK, "REGE" },
    { REGF_BLK, "PTT" },
    { ME_BLK, "MER" },
    { MAX_BLK, "" }
};

BOOLEAN HECIError = FALSE;
UINT8   MacAddr[6];
// For checking if the content in OPRx is correct or not
UINT8   HMRFPO_STATUS;

// Security Update changes - Start
EFI_PHYSICAL_ADDRESS            Phy_Address = 0;
EFI_PHYSICAL_ADDRESS            Post_Phy_Address = 0;
// Security Update changes - End
EFI_PHYSICAL_ADDRESS   OPRx_Buffer;
OFBD_TC_55_ME_PROCESS_STRUCT  *StructPtr;
UINT32                        gCspMeudBiosLimit;
//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   GetHFS
//
// Description: Get Host Firmware Status pass to MEUD
//
// Input:   NONE
//
// Output:  Host Firmware Status
//
// Returns:
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
UINT32 GetHFS(VOID)
{
	    UINT32  R_HFS = 0;
	    UINT8   HeciIsHide = 0;

	    if ((Mmio32(SB_RCBA, 0x3428) & BIT01) != 0)
	    {
	        MmioRW32((SB_RCBA+0x3428), 0, BIT01);
	        HeciIsHide = 1;
	    }

	    R_HFS = HeciPciRead32(0x40);
	    if(R_HFS == 0xFFFFFFFF)
	        R_HFS = 0;

	    if (HeciIsHide)
	       MmioRW32((SB_RCBA+0x3428), BIT01, 0);

	    return R_HFS;
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   SetPRx
//
// Description: Set PRx with the Factory_Base and Factory_Limit in SMM.
//
// Input:   NONE
//
// Output:  NONE
//
// Returns:
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
VOID SetPRx()
{
    *(UINT16*)(SPI_BASE + 0x74) = \
              (*(volatile UINT16*)(SPI_BASE + 0x5C) + (Factory_Base/0x1000));

    *(UINT16*)(SPI_BASE + 0x76) = \
          (*(volatile UINT16*)(SPI_BASE + 0x74) + (Factory_Limit/0x1000) - 1);

    *(UINT32*)(SPI_BASE + 0x74) |= 0x80008000;

    pRS->SetVariable( L"Factory_Limit", &gSmmHeciProtocolGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS |
                      EFI_VARIABLE_RUNTIME_ACCESS,
                      sizeof(UINT32), &Factory_Limit);
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   ClearPRx
//
// Description: Clear PRx.
//
// Input:   NONE
//
// Output:  NONE
//
// Returns:
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
VOID ClearPRx()
{

    *(UINT32*)(SPI_BASE + 0x74) &= 0x7FFF7FFF;

    pRS->SetVariable( L"Factory_Limit", &gSmmHeciProtocolGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS |
                      EFI_VARIABLE_RUNTIME_ACCESS,
                      sizeof(UINT32), &Factory_Limit);

}
union
{
  MKHI_MSG_HMRFPO_LOCK_REQ   HmrfpoLockReq;
  MKHI_MSG_HMRFPO_LOCK_RSP   HmrfpoLockRsp;
  MKHI_MSG_HMRFPO_ENABLE_REQ HmrfpoEnableReq;
  MKHI_MSG_HMRFPO_ENABLE_RSP HmrfpoEnableRsp;
  MKHI_MSG_END_OF_POST_REQ   EopReq;
  MKHI_MSG_END_OF_POST_RSP   EopRsp;
} HeciMsg;

//Aptio V Server Override Start: EFI_SMM_HECI_PROTOCOL structure change conflict resolution

HECI_DEVICE   *mHeciDev ;

static HECI_MSG_HEADER mHeciDxeMsgBuffer[HECI_MSG_MAXLEN / sizeof(HECI_MSG_HEADER)];

/**
 * Sends one message through the HECI and wait for the response message.
 *
 * This function sends one message to ME through HECI circular buffer and
 * waits for one message recived from ME. It does not guarantee that the
 * received message is response to the send message.
 * 
 * @param[in,out] Message     Pointer to the message buffer
 * @param[in]     SendLength  Length in bytes of the message to send
 * @param[in,out] RecvLength  Length of the buffer for received message
 * @param[in]     HostAddress Address of the sending entity
 * @param[in]     MeAddress   Address of the ME entity receiving the message
 *
 * @returns EFI_STATUS is returned.
 */
EFI_STATUS EFIAPI
SmmHeciSendwACK(
  IN OUT  UINT32 *Message,
  IN      UINT32  SendLength,
  IN OUT  UINT32 *RecvLength,
  IN      UINT8   HostAddress,
  IN      UINT8   MeAddress)
{
  EFI_STATUS  Status;
  UINT32      i, Retry, Timeout, FrgLen;
  UINT8       *pMsg = (UINT8*)Message;
  
  if (Message == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  mHeciDxeMsgBuffer[0].DWord = 0;
  mHeciDxeMsgBuffer[0].Bits.MeAddress = MeAddress;
  mHeciDxeMsgBuffer[0].Bits.HostAddress = HostAddress;
  //
  // Fragment the message into queue-sized packets
  //
  ASSERT(mHeciDev->HMtu > 0); // make sure HECI is initialized when sending

    if (SendLength <= mHeciDev->HMtu)
    {
      mHeciDxeMsgBuffer[0].Bits.Length = SendLength;
      mHeciDxeMsgBuffer[0].Bits.MsgComplete = 1; // Set message complete in last fragment
    }
    else
    {
// The Message of lenght more than "mHeciDev->HMtu" is not supported in "HeciSendwACK".
      ASSERT(SendLength <= mHeciDev->HMtu);
      return EFI_INVALID_PARAMETER;
    }
    for (i = 0; i < mHeciDxeMsgBuffer[0].Bits.Length; i++)
    {
      ((UINT8*)&mHeciDxeMsgBuffer[1])[i] = pMsg[i];
    }
    Retry = HECI_MAX_RETRY;
    do
    {
      Timeout = HECI_SEND_TIMEOUT / HECI_TIMEOUT_UNIT;
      FrgLen = sizeof(mHeciDxeMsgBuffer);
      Status = SmmHeci->HeciRequest(SmmHeci, &Timeout, &mHeciDxeMsgBuffer[0], &mHeciDxeMsgBuffer[0], &FrgLen);
    }
    while (EFI_ERROR(Status) && Retry-- > 0);
    if (EFI_ERROR(Status))
    {
      goto GetOut;
    }
    else if (NULL != RecvLength)
    {
      FrgLen -= sizeof(HECI_MSG_HEADER);
      if (FrgLen > *RecvLength)
      {
        FrgLen = *RecvLength;
      }
      for (i = 0; i < FrgLen; i++)
      {
        ((UINT8*)Message)[i] = ((UINT8*)&mHeciDxeMsgBuffer[1])[i];
      }
      *RecvLength = FrgLen;
    }
  
GetOut:
  return Status;

} // SmmHeciSendwACK()

EFI_STATUS EFIAPI
SmmHeciReInitialize(VOID)
{
  UINT32 Timeout = HECI_INIT_TIMEOUT / HECI_TIMEOUT_UNIT;
  
  return SmmHeci->HeciInit(SmmHeci, &Timeout);
}

//Aptio V Server Override End : EFI_SMM_HECI_PROTOCOL structure change conflict resolution

//Aptio V server override Start: Me update changes.

//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   HMRFPO_MERESET
//
// Description: Send MERESET HECI message to ME FW.
//
// Input:   NONE
//
// Output:  EFI_STATUS
//
// Returns:
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS HMRFPO_MERESET(VOID)
{

        EFI_STATUS  Status;
        UINT8       HeciIsHide = 0;
        SPS_MEFS1   MeFs1;
        UINT32      Timeout;
        MKHI_MSG_HMRFPO_MERESET_REQ	HmrfpoMeResetReq;

        if ((Mmio32(SB_RCBA, 0x3428) & BIT01) != 0)
        {
            MmioRW32((SB_RCBA+0x3428), 0, BIT01);
            HeciIsHide = 1;
        }

        HmrfpoMeResetReq.Mkhi.Data = 0x00000005;
        HmrfpoMeResetReq.Nonce = Nounce;

// Aptio V Server Override Start: EFI_SMM_HECI_PROTOCOL structure change conflict resolution
//###DEBUG        Status = SmmHeci->SendMsg((UINT32*)&HmrfpoMeResetReq, sizeof(HmrfpoMeResetReq),
        Status = SmmHeciSendwACK((UINT32*)&HmrfpoMeResetReq, sizeof(HmrfpoMeResetReq), NULL,
                                      SPS_CLIENTID_BIOS, SPS_CLIENTID_ME_MKHI);
// Aptio V Server Override End: EFI_SMM_HECI_PROTOCOL structure change conflict resolution

        //
        // If Intel ME was not running in operational mode before 
        // flash update, return success.
        //
        if ( gMeStateBeforeFlashUpdate != ME_CURSTATE_NORMAL ) {
            Status = EFI_SUCCESS;
            goto MERESET_END;
        }

        //
        // From Grantley ME-BIOS specification:
        // HMRFPO_MERESET message triggers Intel ME-only reset. The reset occurs immediately after the
        // HMRFPO_MERESET response is written to HECI, so if there was no error and the 
        // request has been accepted it is possible that BIOS will not have enough time to read 
        // the response. It is crucial to not repeat the request in such a case. MEFS1 register 
        // should be observed to confirm Intel ME is re-initializing.
        // So check for MEFS1.CurrentState to confirm that ME is re-initializing.
        // ME state transition: RECOVERY -> RESET -> INIT -> OPERATIONAL
        Timeout = SPS_RESET_TIMEOUT / STALL_1MS;
        MeFs1.DWord = GetHFS();
        while ( MeFs1.Bits.CurrentState != ME_CURSTATE_INIT ) {
            if (!Timeout) {
                Status = EFI_DEVICE_ERROR;
                goto MERESET_END;
            }
            MicroSecondDelay(STALL_1MS);
            MeFs1.DWord = GetHFS();
            Timeout--;
        }
        
        //
        // Wait for ME init complete. Timeout is 2 seconds.
        //
        Timeout = SPS_INIT_TIMEOUT / STALL_1MS;
        MeFs1.DWord = GetHFS();
        while ( !MeFs1.Bits.InitComplete ) {
            if (!Timeout) {
                Status = EFI_DEVICE_ERROR;
                goto MERESET_END;
            }
            MicroSecondDelay(STALL_1MS);
            MeFs1.DWord = GetHFS();
            Timeout--;
        }
        //
        // Now, ME should be in normal operational State.
        //
        if ( MeFs1.Bits.CurrentState != ME_CURSTATE_NORMAL ) {
            Status = EFI_DEVICE_ERROR;
            goto MERESET_END;
        }

MERESET_END:        
	    if (HeciIsHide)
	        MmioRW32((SB_RCBA+0x3428), BIT01, 0);

	    return Status;

}
//Aptio V server override End: Me update changes.
//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   HMRFPO_ENABLE_MSG
//
// Description: Send Enable HECI message to ME FW.
//
// Input:   NONE
//
// Output:  EFI_STATUS
//
// Returns:
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS HmrfpoEnableMsg(VOID)
{

	    EFI_STATUS  Status;
	    UINT32      len;
	    UINT8       HeciIsHide = 0;

	    if ((Mmio32(SB_RCBA, 0x3428) & BIT01) != 0)
	    {
	        MmioRW32((SB_RCBA+0x3428), 0, BIT01);
	        HeciIsHide = 1;
	    }

	#if DISABLE_ME_WORKAROUND 
    	{
    	    UINTN       VariableSize = 8;
    	    Status = pRS->GetVariable( L"A", &gEfiHeciProtocolGuid,
    	                               NULL,
    	                               &VariableSize, &Nounce);
            if (EFI_ERROR(Status)) return Status;
    	}
	#endif


	          HeciMsg.HmrfpoEnableReq.Mkhi.Data = 0x00000105;
	          HeciMsg.HmrfpoEnableReq.Nonce = Nounce;
	          len = sizeof(HeciMsg.HmrfpoEnableRsp);
// Aptio V Server Override Start: EFI_SMM_HECI_PROTOCOL structure change conflict resolution
//###DEBUG	          Status = SmmHeci->SendwACK((UINT32*)&HeciMsg.HmrfpoEnableReq,
	          Status = SmmHeciSendwACK((UINT32*)&HeciMsg.HmrfpoEnableReq,
	                                   sizeof(HeciMsg.HmrfpoEnableReq), &len,
	                                   SPS_CLIENTID_BIOS, SPS_CLIENTID_ME_MKHI);
// Aptio V Server Override End: EFI_SMM_HECI_PROTOCOL structure change conflict resolution




	    HMRFPO_STATUS = HeciMsg.HmrfpoEnableRsp.Status;

	    if ((Status == EFI_SUCCESS) && \
	             (HeciMsg.HmrfpoEnableRsp.Mkhi.Data == \
	             0x00008105) &&
	            (HMRFPO_STATUS == 0))
	    {
/*
	        MemCpy(&Factory_Base, \
	               &((HMRFPO_ENABLE_RESPONSE_HECI_MSG*)MsgBuf)->Factory_Base,4);
	        MemCpy(&Factory_Limit, \
	               &((HMRFPO_ENABLE_RESPONSE_HECI_MSG*)MsgBuf)->Factory_Limit,4);

	        SetPRx();
*/
	    }

	    if (HeciIsHide)
	        MmioRW32((SB_RCBA+0x3428), BIT01, 0);

	    return Status;

}
//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   HMRFPO_LOCK_MSG
//
// Description: Send LOCK HECI message and lock ME.
//
// Input:   NONE
//
// Output:  NONE
//
// Returns:
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS HmrfpoLockMsg(VOID)
{
	    EFI_STATUS  Status;
	    UINT32      len;
	    UINT8       HeciIsHide = 0;
	    static BOOLEAN  NonceObtained = FALSE; //Aptio V server override: Me update changes.

	    if(HECIError)
	        return EFI_DEVICE_ERROR;

	    if ((Mmio32(SB_RCBA, 0x3428) & BIT01) != 0)
	    {
	        MmioRW32((SB_RCBA+0x3428), 0, BIT01);
	        HeciIsHide = 1;
	    }
	    // Clear Status

	      HeciMsg.HmrfpoLockReq.Mkhi.Data = 0x00000205;
	      len = sizeof(HeciMsg.HmrfpoLockRsp);
// Aptio V Server Override Start: EFI_SMM_HECI_PROTOCOL structure change conflict resolution
//###DEBUG	      Status = SmmHeci->SendwACK((UINT32*)&HeciMsg.HmrfpoLockReq,
	      Status = SmmHeciSendwACK((UINT32*)&HeciMsg.HmrfpoLockReq,
	                               sizeof(HeciMsg.HmrfpoLockReq), &len,
	                               SPS_CLIENTID_BIOS, SPS_CLIENTID_ME_MKHI);
// Aptio V Server Override End: EFI_SMM_HECI_PROTOCOL structure change conflict resolution


	    HMRFPO_STATUS = HeciMsg.HmrfpoLockRsp.Status;

	    if ((Status == EFI_SUCCESS) &&
	            (HeciMsg.HmrfpoLockRsp.Mkhi.Data == \
	             0x00008205) && \
	            (HeciMsg.HmrfpoLockRsp.Status == 0))
	    {
            //Aptio V server override Start: Me update changes.
            if ( NonceObtained != TRUE ) {
	            Nounce = HeciMsg.HmrfpoLockRsp.Nonce;
	            NonceObtained = TRUE;
            }
	        //Nounce = HeciMsg.HmrfpoLockRsp.Nonce;
            //Aptio V server override End: Me update changes.

	        MemCpy(&Factory_Base, \
	               &HeciMsg.HmrfpoLockRsp.FactoryBase,4);
	        MemCpy(&Factory_Limit, \
	               &HeciMsg.HmrfpoLockRsp.FactoryLimit,4);

/*	        if(((HMRFPO_LOCK_RESPONSE_HECI_MSG*)MsgBuf)->FactoryBase != 0)
	        {
//	            SetPRx();
	        }
*/
	#if DISABLE_ME_WORKAROUND
	        if(Nounce != 0)
	            pRS->SetVariable( L"A", &gEfiHeciProtocolGuid,
	                              EFI_VARIABLE_BOOTSERVICE_ACCESS |
	                              EFI_VARIABLE_RUNTIME_ACCESS |
	                              EFI_VARIABLE_NON_VOLATILE,
	                              sizeof(UINT64), Nounce);
	#endif
	    }


	    if (HeciIsHide)
	        MmioRW32((SB_RCBA+0x3428), BIT01, 0);

	    return Status;
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   HMRFPO_DISABLE
//
// Description: Send Disable HECI message when boot in SECOVR_MEI_MSG.
//
// Input:   NONE
//
// Output:  EFI_STATUS
//
// Returns:
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS HMRFPO_DISABLE(VOID)
{
    return EFI_UNSUPPORTED;
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   SEND_MEI_GLOBAL_RESET_MSG
//
// Description: To Reset System.
//
// Input:   NONE
//
// Output:  EFI_STATUS
//
// Returns:
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS SEND_MEI_GLOBAL_RESET_MSG(VOID)
{
    return EFI_UNSUPPORTED;
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	GetRegionOffset
//
// Description:	Get GBE Region Offet of whole FlashPart
//
// Input:
//      VOID
// Output:
//      UINT32  The offset of GBE Region
//
// Returns:
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS
GetRegionOffset(
    UINT8    Region,
    UINT32*  Offset,
    UINT32*  Length
)
{
    volatile UINT32	*FDOC;
    volatile UINT32	*FDOD;
    UINT32	FlashDescriptorSig = 0x0FF0A55A;
    UINT32	Buffer32, RegionStart, RegionEnd;


    FDOC = (UINT32*)(SPI_BASE + 0xB0);
    FDOD = (UINT32*)(SPI_BASE + 0xB4);
    *FDOC = 0;

    if (*FDOD != FlashDescriptorSig)
        return EFI_UNSUPPORTED;
    switch(Region)
    {
        // Flash Descriptor
        case 0:
        *FDOC = 0x2000;
        break;

        // BIOS
        case 1:
        *FDOC = 0x2004;
        break;

        // ME
        case 2:
        *FDOC = 0x2008;
        break;

        // GBE
        case 3:
        *FDOC = 0x200C;
        break;

        // PDR
        case 4:
        *FDOC = 0x2010;
        break;
        
        // EIP140541 +
        // DER
        case 5:
        *FDOC = 0x2014;
        break;
        // EIP140541 -        

        default:
        return EFI_UNSUPPORTED;
        break;
    }
    Buffer32 = *FDOD;
    RegionEnd = Buffer32 >> 16;
    RegionStart = Buffer32 & 0xFFFF;

    *Offset = RegionStart << 12;
    *Length = (RegionEnd - RegionStart + 1) << 12;
    
    // If Flash Descriptor Region is 0x1000, It returns 0.
    // 4K missing +
    if((RegionEnd == 0) && (Region == 0))
    {
        *Length = 0x1000;
        return EFI_SUCCESS;
    }
    // 4K missing -
    if(RegionEnd == 0)
    {
        *Length = 0;
        return EFI_NOT_FOUND;
    }

    return EFI_SUCCESS;
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   GetFlashCapacity
//
// Description: Send a HECI message to lock ME.
//
// Input:   NONE
//
// Output:  FlashDensity - Real Flash Size
//
// Returns:
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
UINT32 GetFlashCapacity(VOID)
{

	    volatile UINT32    *FDOC;
	    volatile UINT32    *FDOD;
	    UINT32	FlashDescriptorSig = 0x0FF0A55A, FlashDensity = 0;
	    UINT16	Components;
	    UINT8	i,j;

	    FDOC = (UINT32*)(SPI_BASE + 0xB0);
	    FDOD = (UINT32*)(SPI_BASE + 0xB4);

	    *FDOC = 0;
	    // Invalid !! Not Support ME
	    if (*FDOD != FlashDescriptorSig)
	        return 0;

	    *FDOC = 0x04;
	    Components = (*FDOD >> 8) & 0x03;

	    *FDOC = 0x1000;
	    j = *FDOD;


	    for (i = 0 ; i < ( Components + 1 ) ; i++)
	    {
	        switch (j & 0x07)
	        {
	        case 0:
	            FlashDensity += 0x80000;
	            break;
	        case 1:
	            FlashDensity += 0x100000;
	            break;
	        case 2:
	            FlashDensity += 0x200000;
	            break;
	        case 3:
	            FlashDensity += 0x400000;
	            break;
	        case 4:
	            FlashDensity += 0x800000;
	            break;
	        case 5:
	            FlashDensity += 0x1000000;
	            break;
	        default:
	            break;
	        }
#if BITS_OF_SPI_DENSITY
	        j = j >> 4;
#else
	        j = j >> 3;
#endif	        
	    }

	    return FlashDensity;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   CspReportMeInfo
//
// Description: Report ME Base address and Length to AFU
//
// Input:   BASE_Address - address of ME region to be updated
//          Length - Length of ME region to be updated
//          Func_Num - 0 for Command /MEUF , 3 for Command /ME
//
// Output:  NONE
//
// Returns: NONE
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS CspReportMeInfo
(
    IN UINT8 Func_Num,
    IN OUT UINT32* BASE_Address,
    IN OUT UINT32* Length
)
{
    // Fill these 2 item for Afu can update BIOS with whole image input
    *BASE_Address = 0;
    *Length = GetFlashCapacity() - FLASH_SIZE;
    switch (Func_Num)
    {
    case 0:
	// Command 0 is for Ignition FW, only ME6 support this.
        return EFI_UNSUPPORTED;
        break;
    case 3:
        // Always rteturn SUCCESS here !!
        break;
    default:
        return EFI_UNSUPPORTED;
        break;
    }
    return EFI_SUCCESS;
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   MEUDSMIHandler
//
// Description: Send Enable and Global reset MSG to ME FW.
//
// Input:   NONE
//
// Output:  NONE
//
// Returns: NONE
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
VOID MEUDSMIHandler (
    IN    EFI_HANDLE    DispatchHandle,
    IN    EFI_SMM_SW_DISPATCH_CONTEXT    *DispatchContext
)
{
	// This is not needed anymore, SPS can be disabled in runtime.
/*
    HmrfpoEnableMsg();
    SEND_MEI_GLOBAL_RESET_MSG();
*/
}

EFI_STATUS
LocateSmmHeciProtocol (
    CONST EFI_GUID  *Protocol,
    VOID            *Interface,
    EFI_HANDLE      Handle
) {

	EFI_STATUS  Status = pSmst->SmmLocateProtocol(&gSmmHeciProtocolGuid, NULL, &SmmHeci);
    mHeciDev = &SmmHeci->HeciDev; // Aptio V Server Override: EFI_SMM_HECI_PROTOCOL structure change conflict resolution
    return Status;
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   CspMeudInSmm
//
// Description: Get Host Firmware Status.
//              If needed, Send LOCK if needed in SMM.
//
// Input:   NONE
//
// Output:  NONE
//
// Returns: NONE
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
VOID CspMeudInSmm(VOID)
{
    EFI_STATUS  Status;
    UINT32      Offset;
    UINT32      Length;
    UINT8       Timer = 0;
    VOID        *ProtocolNotifyRegistration;

    Status = GetRegionOffset( BIOS_BLK, &Offset, &Length );
    if( !EFI_ERROR(Status) )
        gCspMeudBiosLimit = Offset + Length;
    else
        gCspMeudBiosLimit = GetFlashCapacity();

    Status = pBS->AllocatePages(AllocateAnyPages, EfiRuntimeServicesData,
                                EFI_SIZE_TO_PAGES(RT_ME_BUFFER), &Phy_Address);
    if (EFI_ERROR(Status)) return;

   Status = pBS->AllocatePages(AllocateAnyPages, EfiRuntimeServicesData,
                               EFI_SIZE_TO_PAGES(RT_OPR_BUFFER), &OPRx_Buffer);
   if (EFI_ERROR(Status)) return;

   Status = pSmst->SmmLocateProtocol(&gSmmHeciProtocolGuid, NULL, &SmmHeci);
   if (EFI_ERROR(Status)) return;


    if(EFI_ERROR(Status))
    {
        Status = pSmst->SmmRegisterProtocolNotify (
                    &gSmmHeciProtocolGuid,
                    LocateSmmHeciProtocol,
                    &ProtocolNotifyRegistration
                    );
        return;
    }
    // Send Lock Message
    mHeciDev = &SmmHeci->HeciDev; // Aptio V Server Override: EFI_SMM_HECI_PROTOCOL structure change conflict resolution

    do{
        Status = HmrfpoLockMsg();
        if(!EFI_ERROR(Status))
            break;
        Timer ++;
    }while(Timer < 3);
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	MEProcessHandleResult
//
// Description:	Handle ME Process
//
// Input:
//      UpdateResult
//      Message
// Output:
//      VOID
//
// Returns:
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID
MEProcessHandleResult(
    IN UINT16   Result,
    IN CHAR8*   Message
)
{
    // Security Update changes - Start
    EFI_STATUS   Status; 
    StructPtr->UpdateResult = Result;
    Status = AmiValidateMemoryBuffer ( (VOID*)StructPtr->ddMessageBuffer, Strlen(Message));
    if (EFI_ERROR (Status)){
        DEBUG ((DEBUG_ERROR, "%a Status of AmiValidateMemoryBuffer : %r\n", Status));
        return;
    }
    // Security Update changes - End
    MemCpy((UINT8*)(StructPtr->ddMessageBuffer),
                    Message, Strlen(Message));

    *(CHAR8*)(StructPtr->ddMessageBuffer + Strlen(Message)) = 0;
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	SendEnableMessage
//
// Description:	Send Enable Message
//
// Input:
//    BOOLEAN   InSmm
//
// Output:
//    NONE
//
// Returns:
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS
SendEnableMessage(
    BOOLEAN    InSmm
)
{
    UINT8       Counter;
    EFI_STATUS  Status;

//Aptio V server override - Start: Save the ME state before flash update.
    if ( gMeStateBeforeFlashUpdate == 0xFF ) {
        gMeStateBeforeFlashUpdate = ( 0x0F & GetHFS() );
    }
//Aptio V server override - End: Save the ME state before flash update.

    if(InSmm)
    {
        Counter = 0;
        do{
            Status = HmrfpoEnableMsg();
            if(!EFI_ERROR(Status))
                break;
            Counter ++;
//###DEBUG            SmmHeci->ReInitHeci();
            SmmHeciReInitialize(); // Aptio V Server Override: EFI_SMM_HECI_PROTOCOL structure change conflict resolution
        }while(Counter < 3);
        GetHFS();
    }
    else
    {
        UINTN        VariableSize = 1;
        EFI_GUID     gMEUDErrorguid = MEUD_ERROR_GUID;

        IoWrite8(0xB2, Disable_ME_SW_SMI);

        // Get Status
        Status = pRS->GetVariable( L"ENABLESTATUS", &gMEUDErrorguid,
                          NULL,
                          &VariableSize, &HMRFPO_STATUS);

        // Error, if variable not exist
        if(Status == EFI_NOT_FOUND)
            return Status;

        return EFI_SUCCESS;
    }

    return Status;

}
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	HandleBuffer
//
// Description:	Init the Length and Offset need to be updated
//              If needed, send ENABLE MESSAGE
//
// Input:
//      UpdateResult
//      Message
// Output:
//      VOID
//
// Returns:
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS
HandleBuffer(
    IN OUT UINT32*   ProgramOffset,
    IN OUT UINT32*   ProgramLength,
    IN OUT UINT8*    Step,
    IN     BOOLEAN   InSmm
)
{
    EFI_STATUS    Status;
    UINT32        Offset, Length;
    UINT32        HFS = GetHFS();

    switch(StructPtr->bBlockType)
    {
        case FDT_BLK:
            Status = GetRegionOffset(0, &Offset, &Length);
            *Step = 0;
        break;

        case PDR_BLK:
            Status = GetRegionOffset(4, &Offset, &Length);
            if(Status == EFI_NOT_FOUND)
                return EFI_UNSUPPORTED;
            *Step = 0;
        break;

        case GBE_BLK:
            Status = GetRegionOffset(3, &Offset, &Length);
            if(Status == EFI_NOT_FOUND)
                return EFI_UNSUPPORTED;
            // Store Mac address
            if(Length)
            {
                UINT8* Address = (UINT8*)FLASH_BASE_ADDRESS(Offset);
                FlashRead(Address, MacAddr, 6);
            }
            *Step = 0;
        break;

        case ME_BLK:
            if(!(HFS & BIT09))
            {
                MEProcessHandleResult(BIT03,
                              "ME FW not Initial done");

                return EFI_NOT_READY;
            }
            Status = GetRegionOffset(2, &Offset, &Length);
            if((HFS & BIT05) || (HFS & BIT10))
                *Step = 2;
            else
                *Step = 1;

        break;

        case ME_OPR_BLK:
            if((HFS & 0xF) != 2)
            {
                Status = SendEnableMessage(InSmm);
                if(EFI_ERROR(Status) || (HMRFPO_STATUS != 0))
                {
                    MEProcessHandleResult(BIT03,
                              "Error when sending Enable Message to ME !!");
                    return Status;
                }
            }
            Offset = OPR1_START;
            if(OPR2_LENGTH)
            {
                // If dual image system
                // 3.8.2.3
                if(!InSmm)
                {
                    // Update Factory_Limit.
                    UINTN VariableSize = sizeof(UINT32);

                    Status = pRS->GetVariable( L"Factory_Limit",
                                               &gSmmHeciProtocolGuid,
                                               NULL,
                                               &VariableSize,
                                               &Factory_Limit);
                    if (EFI_ERROR(Status)) Factory_Limit = 0;
                }
                if(Factory_Limit == (FPT_LENGTH + MFSB_LENGTH + FTPR_LENGTH +
                                     MFS_LENGTH + OPR1_LENGTH))
                    Offset = OPR2_START;

            }
            Length = OPR1_LENGTH;
            *Step = 0;
        break;

        default:
            *Step = 0;
            return EFI_UNSUPPORTED;
        break;
    }
    *ProgramOffset = Offset;
    *ProgramLength = Length;

    return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	HnadleMEUpdateSteps
//
// Description:	Handle Program Length and Offset , Steps and Program Buffer
//
// Input:
//    UINT32*   ProgramOffset
//    UINT32*   ProgramLength
//    UINT8     Step
//    BOOLEAN   InSmm
//    UINT8**   ProgramBuffer
// Output:
//
//    UINT32*   ProgramOffset
//    UINT32*   ProgramLength
//    UINT8**   ProgramBuffer
//
// Returns:
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS
HnadleMEUpdateSteps(
    IN OUT UINT32*   ProgramOffset,
    IN OUT UINT32*   ProgramLength,
    IN     UINT8     Step,
    IN     BOOLEAN   InSmm,
    IN OUT UINT8**   ProgramBuffer
)
{
    static BOOLEAN         SmmPages = FALSE;
    EFI_PHYSICAL_ADDRESS   SmmBuffer;
    static UINTN           BufferPages;
    EFI_STATUS             Status;
    UINT32                 Offset, Length;

    if(Step == 1)
    {
        // Step 1 : Program FTPR to OPR1 and OPR2
        // Send Enable to ME
        // Retry 3 times for Enable message.
        Status = SendEnableMessage(InSmm);

        if(EFI_ERROR(Status) || (HMRFPO_STATUS != 0))
        {
            MEProcessHandleResult(BIT03 | BIT02,
                      "Error when sending Enable Message to ME !!");
            return Status;
        }

        // Prepare buffer, the length and offset is defferent
        if(OPR2_LENGTH)
            BufferPages = (OPR1_LENGTH + OPR2_LENGTH) >> 12;
        else
            BufferPages = (OPR1_LENGTH) >> 12;

        if(InSmm && !SmmPages)
        {
            Status = pSmst->SmmAllocatePages(AllocateAnyPages,
                     EfiRuntimeServicesData, BufferPages, &SmmBuffer);

            if(!EFI_ERROR(Status))
            {
                SmmPages = TRUE;
                OPRx_Buffer = SmmBuffer;
            }
        }

        MemSet((UINT8*)(OPRx_Buffer), BufferPages*0x1000, 0xFF);

        MemCpy((UINT8*)(OPRx_Buffer),
               (UINT8*)(Phy_Address + FTPR_START), FTPR_LENGTH);

        // If OPR2 exist
        if(OPR2_LENGTH)
            MemCpy((UINT8*)(OPRx_Buffer + OPR1_LENGTH),
                   (UINT8*)(Phy_Address + FTPR_START), FTPR_LENGTH);

        // Update Buffer ,Length and Offset
        *ProgramBuffer = (UINT8*)OPRx_Buffer;

        Offset = OPR1_START;

        if(OPR2_LENGTH)
            Length = OPR1_LENGTH + OPR2_LENGTH;
        else
            Length = OPR1_LENGTH;
    }

    // Step 2 : Update FPT, MFSB, FTPR and MFS
    if(Step == 2)
    {
        // Free the memory we allocate last time.
        if(SmmPages)
        {
            pSmst->SmmFreePages(OPRx_Buffer, BufferPages);
            SmmPages = FALSE;
        }

        // Enable again !!
        Status = SendEnableMessage(InSmm);

        if(EFI_ERROR(Status) || (HMRFPO_STATUS != 0))
        {
            MEProcessHandleResult(BIT03 | BIT02,
                      "Error when sending Enable Message to ME !!");
            if(!EFI_ERROR(Status))
                Status = EFI_WRITE_PROTECTED;
            return Status;
        }

        Offset = FPT_START;
        // Just take care of length
        Length = MFS_LENGTH + FTPR_LENGTH + MFSB_LENGTH + FPT_LENGTH;
        //Aptio V server override Start: Me update changes.
 #if SCA_LENGTH
            Length+= (UINT8)SCA_LENGTH;
 #endif    
        //Aptio V server override End: Me update changes.
        *ProgramBuffer = (UINT8*)(Phy_Address + Offset);
    }

    // Step 3 : Update OPR1 and OPR2
    if(Step == 3)
    {
        // Take care of length
        if(OPR2_LENGTH)
            Length = OPR1_LENGTH + OPR2_LENGTH;
        else
            Length = OPR1_LENGTH;

        Offset = OPR1_START;
        *ProgramBuffer = (UINT8*)(Phy_Address + Offset);
    }
    *ProgramOffset = Offset;
    *ProgramLength = Length;
    ClearPRx();

    return EFI_SUCCESS;

}
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	UpdateRegions
//
// Description:	UpdateRegions
//
// Input:
//      Buffer
//
// Output:
//      VOID
//
// Returns:
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS
UpdateRegions(
    IN UINT8*    Buffer,
    IN BOOLEAN   InSmm
)
{
    static UINT32          Offset, Length;
    UINT8*                 Address;
    EFI_STATUS             Status;
    BOOLEAN                NeedToVerify = FALSE;
    // STEP 1 : Update FPTR to OPR
    // STEP 2 : Update FPT, MFSB, FTPR and MFS
    // STEP 3 : Update OPRx
    static UINT8           Step = 0;
    static BOOLEAN         NewRegion;
    UINTN                  Counter = 0;
    static UINT8           RetryTimes = RETRY_OPR_COUNT;
    static UINT8*          ProgramBuffer;

    // Prepare Offset and Length to be updated
    // If BIT02 , update buffer
    if((StructPtr->bHandleRequest & BIT02))
    {
        if(!InSmm)
        {
            (UINT8*)Phy_Address = Buffer;
            Status = pBS->AllocatePages(AllocateAnyPages,
                                  EfiRuntimeServicesData,
                                  EFI_SIZE_TO_PAGES(RT_OPR_BUFFER),
                                  &OPRx_Buffer);
        }

        Status = HandleBuffer(&Offset, &Length, &Step, InSmm);

        if(EFI_ERROR(Status))
        {
            MEProcessHandleResult(BIT03,
                      "UN SUPPORT TYPE");
            return Status;
        }
        // Frist In
        NewRegion = TRUE;
        ProgramBuffer = (UINT8*)(Phy_Address + Offset);
    }
    // Set MAC address to buffer
    if(((StructPtr->bBlockType) == GBE_BLK) && NewRegion)
    {
        MemCpy((Buffer + Offset),MacAddr,6);
        return EFI_SUCCESS;
    }

    // Handle Step
    if((StructPtr->bBlockType == ME_BLK) && NewRegion)
    {
        Status = HnadleMEUpdateSteps(&Offset, &Length, Step, InSmm, &ProgramBuffer);
        if(EFI_ERROR(Status))
            return Status;
    }

    if(NewRegion)
    {
        NewRegion = FALSE;
    }

    Address = (UINT8*)FLASH_BASE_ADDRESS(Offset);

    FlashBlockWriteEnable(Address);
    FlashEraseBlock(Address);
    FlashProgram(Address, ProgramBuffer, FLASH_BLOCK_SIZE);
    FlashBlockWriteDisable(Address);

    ProgramBuffer = ProgramBuffer + FLASH_BLOCK_SIZE;
    Length -= FLASH_BLOCK_SIZE;
    Offset += FLASH_BLOCK_SIZE;

    // End of Region Update
    if(Length == 0)
    {
        NewRegion = TRUE;
        // Only when Updating MER, the Step is lnot 0.
#if CHECK_FPTR_IN_OPR
        if((Step == 1) || (Step == 3) || (StructPtr->bBlockType == ME_OPR_BLK))
#else
        if((Step == 3) || (StructPtr->bBlockType == ME_OPR_BLK))
#endif
            NeedToVerify = TRUE;

    }

    // TODO :
    // OEM can output message here in every block updated.
    // Remember to Set BIT02
    else
    {
        MEProcessHandleResult((BIT01),
                         " ");
        return EFI_SUCCESS;
    }

    if(NeedToVerify)
    {
        if(InSmm)
        {
            Counter = 0;
            do{
                    Status = HmrfpoLockMsg();
                if(!EFI_ERROR(Status))
                    break;
                Counter ++;
//###DEBUG                SmmHeci->ReInitHeci();
                SmmHeciReInitialize(); // Aptio V Server Override: EFI_SMM_HECI_PROTOCOL structure change conflict resolution
            }while(Counter < 3);

        }
        else
        {
            EFI_GUID   gMEUDErrorguid = MEUD_ERROR_GUID;
            UINTN      VarianleSize;
            pRS->SetVariable( L"LOCKMSG", &gMEUDErrorguid,
                              EFI_VARIABLE_BOOTSERVICE_ACCESS |
                              EFI_VARIABLE_RUNTIME_ACCESS,
                              sizeof(UINTN), &Counter);

            IoWrite8(SW_SMI_IO_ADDRESS, Disable_ME_SW_SMI);

            VarianleSize = sizeof(UINT8);

            Status = pRS->GetVariable( L"LOCKSTATUS", &gMEUDErrorguid,
                              NULL,
                              &VarianleSize, &HMRFPO_STATUS);

            // Delete Flag
            pRS->SetVariable( L"LOCKSTATUS", &gMEUDErrorguid,
                              EFI_VARIABLE_BOOTSERVICE_ACCESS |
                              EFI_VARIABLE_RUNTIME_ACCESS,
                              0, &HMRFPO_STATUS);
        }

        // If success.
        if(!EFI_ERROR(Status))
        {
            // Check Status
            if(HMRFPO_STATUS == 2)
            {
                MEProcessHandleResult((BIT01 | BIT02),
                          "Warming : HMRFPO_LOCK_VALID !!");
                // Retry
                if(RetryTimes > 0)
                {
                    Step = 1;
                    RetryTimes -= 1;
                    return EFI_SUCCESS;
                }
            //Aptio V server override Start: Me update changes.
            } else if ( Step == 3 ) {
                //
                // Send MERESET Message.
                Status = HMRFPO_MERESET();
            //Aptio V server override End: Me update changes.
            }
        }
    }
    else
        Status = EFI_SUCCESS;

    // Show Strings
    if(!EFI_ERROR(Status))
    {
        switch(Step)
        {
            case 1:
                MEProcessHandleResult((BIT01 | BIT02),
                              "Successful Update Recovery Loader to OPRx!!");

            break;
            case 2:
                MEProcessHandleResult((BIT01 | BIT02),
                              "Successful Update FPT, MFSB, FTPR and MFS!!");
            break;
            case 3:
                MEProcessHandleResult((BIT03 | BIT02),
                              "ME Entire Image update success !!");
            break;
            default:
            {
                UINT8   ResultString[25];
                UINT8   Index;

                for( Index = 0; Index < MAX_BLK; Index++ )
                {
                    if( FlashRegionsDescriptor[Index].FlashRegion == MAX_BLK )
                        break;

                    if( FlashRegionsDescriptor[Index].FlashRegion == StructPtr->bBlockType )
                    {
                        Sprintf( ResultString, "Update success for %s", FlashRegionsDescriptor[Index].Command);
                        break;
                    }
                }

                if( (Index == MAX_BLK) || (FlashRegionsDescriptor[Index].FlashRegion == MAX_BLK) )
                    Sprintf( ResultString, "Update success");

                MEProcessHandleResult( (BIT03 | BIT02), ResultString );
            }
        }
        if(Step != 0)
            Step += 1;
    }else
    {
        // check Flash verify result first.
        if (Status == EFI_DEVICE_ERROR) {
            UINT8   Messages[80];
            Sprintf( Messages, "Flash Update Error at address 0x%08X !!", (UINT32)Address );
            MEProcessHandleResult((BIT00 | BIT02), Messages);
            return EFI_SUCCESS;                  
        }                      
        switch(Step)
        {
            case 1:
                MEProcessHandleResult((BIT00 | BIT02),
                              "Error when Update Recovery Loader to OPRx!!");
            break;
            case 2:
                MEProcessHandleResult((BIT00 | BIT02),
                              "Error when Update FPT, MFSB, FTPR and MFS!!");
            break;
            case 3:
                MEProcessHandleResult((BIT00 | BIT02),
                              "Error on ME Entire Image update!!");
            break;
            default:
                StructPtr->UpdateResult = BIT03;
            break;
        }
        if(Step != 0)
            Step += 1;
    }
    return EFI_SUCCESS;
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	MEProcessHandler
//
// Description:	Handle ME Process
//
// Input:
//      VOID
// Output:
//      OFBD_TC_55_ME_PROCESS_STRUCT
//
// Returns:
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID
MEProcessHandler
(
    IN OUT OFBD_TC_55_ME_PROCESS_STRUCT  **MEProcessStructPtr
)
{
    EFI_STATUS                    Status;
    static UINTN                  NumberOfPages;
    static UINT32                 SizeCopied;
    static BOOLEAN                UseSmmMem = FALSE;
    static EFI_PHYSICAL_ADDRESS   SMM_Address;
    static UINT8                  HeciIsHide = 0;

    StructPtr = *MEProcessStructPtr;

    switch(StructPtr->bHandleRequest)
    {
        // Allocate Buffer
        case 1:
            if ((Mmio32(SB_RCBA, 0x3428) & BIT01) != 0)
            {
                MmioRW32((SB_RCBA+0x3428), 0, BIT01);
                HeciIsHide = 1;
            }

//AptioV Server override Start
// (debx+20140205)>
            // If Non-BIOS(ME/GbE/DER/PDR) region can't be identified 
            // by FDOC, the TotalBlocks could be less than Non-BIOS Region,
            // then, the SMM memroy will get destroyed when AFU upload
            // Non-BIOS region data to SMM. So, below change to use Non-BIOS 
            // region size instead of TotalBlocks size for allocating memory
            // to avoid such situation.
            // Security Update changes - Start
            if(!Phy_Address)
            {
            // Security Update changes - End
            NumberOfPages = (GetFlashCapacity() - FLASH_SIZE) >> 12;
//-            NumberOfPages = StructPtr->TotalBlocks;
// <(debx+20140205)
//AptioV Server override End
            Status = pSmst->SmmAllocatePages(AllocateAnyPages,
                     EfiRuntimeServicesData, NumberOfPages, &SMM_Address);

            if(!EFI_ERROR(Status))
            {
                UseSmmMem = TRUE;
                Phy_Address = SMM_Address;
            // Security Update changes - Start
            } else {
                Phy_Address = Post_Phy_Address;
            }
            // Security Update changes - End
            }
            // No memory allocated
            if(!Phy_Address)
                MEProcessHandleResult((BIT00 | BIT02),
                              "Error : No Memory Allocated!!");

            SizeCopied = 0;
        break;

        // Recieve Data from AFU
        case 2:
            // Security Update changes - Start
            Status = EFI_OUT_OF_RESOURCES;
            if( UseSmmMem == TRUE )
            {
                if( (SizeCopied + StructPtr->ddMeDataSize) <= (GetFlashCapacity() - FLASH_SIZE) ) //Ensure SizeCopied remains within the allocated buffer
                Status = AmiValidateSmramBuffer( (VOID*)(Phy_Address + SizeCopied), StructPtr->ddMeDataSize );
            }
            else
            {
                if( (SizeCopied + StructPtr->ddMeDataSize) <= RT_ME_BUFFER ) //Ensure SizeCopied remains within the allocated buffer
                Status = AmiValidateMemoryBuffer( (VOID*)(Phy_Address + SizeCopied), StructPtr->ddMeDataSize );
            }
            if( EFI_ERROR(Status) )
            {
                MEProcessHandleResult( (BIT00 | BIT02), "Invalid memory usage" );
            }

            Status = AmiValidateMemoryBuffer( (VOID*)(StructPtr->ddMeDataBuffer), StructPtr->ddMeDataSize );
            if( EFI_ERROR(Status) )
            {
                MEProcessHandleResult( (BIT00 | BIT02), "Invalid memory usage" );
            }
            // Security Update changes - End
            MemCpy((UINT8*)(Phy_Address + SizeCopied),
                    (UINT8*)StructPtr->ddMeDataBuffer,StructPtr->ddMeDataSize);
            SizeCopied += StructPtr->ddMeDataSize;

        break;

        // Update
        case 4:
            UpdateRegions((UINT8*)Phy_Address, TRUE);
        break;

        // Continue....
        case 8:
            UpdateRegions((UINT8*)Phy_Address, TRUE);
        break;

        // Free Buffer
        case 0x10:
            if(UseSmmMem){ // Security Update changes - Start
                pSmst->SmmFreePages(Phy_Address, NumberOfPages);
                Phy_Address = 0;
                SMM_Address = 0;
            }
            // Security Update changes - End

            if (HeciIsHide)
               MmioRW32((SB_RCBA+0x3428), BIT01, 0);
        break;
    }
}
/**
    CspMeudReportFirmwareType reports AFU on-board firmware version, and trigger
    AFU new process flow by setting ReturnStatus with 1.

    @param MeVersionStructPtr

    @retval VOID

**/
VOID
CspMeudReportFirmwareType(
    IN  OFBD_TC_55_ME_VERSION_STRUCT    *MeVersionStructPtr
)
{
    MeVersionStructPtr->FwType = MEUD_FIRMWARE_SPS;

    //Don't modify ReturnStatus to indicate AFU use old process
    //MeVersionStructPtr->ReturnStatus = 0x01;

    return;
}
