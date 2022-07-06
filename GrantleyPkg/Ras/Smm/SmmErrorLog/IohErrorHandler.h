//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2015, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**           5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093      **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

//*************************************************************************
// $Header: /Alaska/SOURCE/Chipset/Intel/NorthBridge/SandyBridgeEP/RtErrorLogging/RtIohElog.h 8     12/13/11 2:14p Bhimanadhunik $
//
// $Revision: 8 $
//
// $Date: 12/13/11 2:14p $
//*************************************************************************
// Revision History
// ----------------
// $Log: /Alaska/SOURCE/Chipset/Intel/NorthBridge/SandyBridgeEP/RtErrorLogging/RtIohElog.h $
// 
// 8     12/13/11 2:14p Bhimanadhunik
// s per Intel recommendation disabling the 0xC3 error.
// 
// 7     12/07/11 3:10p Bhimanadhunik
// fix in programming the vt-d mask register.
// 
// 6     10/10/11 2:10p Bhimanadhunik
// As per intel recommendation disabling the 0xC2 and 0xb4 errors.
// 
// 5     8/05/11 11:07a Bhimanadhunik
// Disabling IOH QPI 0xDA,0Xc2,)xc3 errors as intel hardware is generating
// these errors continuosly.
// 
// 4     7/28/11 4:47p Bhimanadhunik
// Ioh vt-d error type issues are fixed.
// 
// 3     6/02/11 7:23p Bhimanadhunik
// Support for Data Poison/Error data Containment support.
// 
// 2     4/29/11 1:00p Bhimanadhunik
// Support for IOH QPI, Core, VT-d errors.
// 
// 1     12/23/10 11:15a Bhimanadhunik
// Runtime Error Logging support for Sandybridge EP Platforms
// 
// 2     2/03/10 2:37p Bhimanadhunik
// Added support for IOH QPI, Core, VT-d Errors.
// 
// 1     2/02/10 5:42p Bhimanadhunik
// 
//
//*************************************************************************
//<AMI_FHDR_START>
//
// Name:  IohErrorHandler.h
//
//  Description:
//  Header file for IOH Error Logging code.
//
//<AMI_FHDR_END>
//*************************************************************************

#ifndef _IOH_ERROR_HANDLER_H
#define _IOH_ERROR_HANDLER_H

extern UINT8	mResetRequired;
extern UINT8 	mIohPoisonSupportEnableInSetup;


#define DMASK 	BIT12

// As per Intel recommendation disabling the 0xC3 error.
// As per intel recommendation disabling the 0xC2 and 0xb4 errors.
#define QPI_P_ERRMSK 0x4404 // Bit 0 to 19 QPI Protocol errors

#define IOH_CORE_C4     BIT4 // C4 - Master abort address error
#define IOH_CORE_C5     BIT5 // C5 - Completor abort address error
#define IOH_CORE_C6     BIT6 // C6 - FIFO Overflow/Underflow error

#define IOH_CORE_ERRMSK 0x70 // Bit 4 to 6 IOH Core errors

#define R_CORE20_IIOMISCCTRL                      0x1C4
#define B_CORE20_POISFEN                          BIT5

#define GST_QPI0  BIT0
#define GST_QPI1  BIT1

#define GST_IOH_CORE_ERR BIT23
#define GST_IOH_VTD_ERR  BIT25

#define	HA_UNC_RD_ERR_MCACOD				0x09
#define	HA_UNC_RD_ERR_MSCOD					0x01
#define	HA_UNC_PARTIAL_WR_ERR_MCACOD		0x0A
#define	HA_UNC_PARTIAL_WR_ERR_MSCOD			0x04

#define	IMC_UNC_PATROL_SCRUB_ERR_MCACOD		0x0C
#define	IMC_UNC_PATROL_SCRUB_ERR_MSCOD		0x10
#define	IMC_UNC_SAPRING_ERR_MCACOD			0x0C
#define	IMC_UNC_SAPRING_ERR_MSCOD			0x40

#define	IMC_ADD_CMD_PARITY_ERR_MCACOD		0x0B
#define	IMC_ADD_CMD_PARITY_ERR_MSCOD		0x01

#define MCA_BANK_IFU                           0
#define MCA_BANK_DCU                           1
#define MCA_BANK_DLTB                          2
#define MCA_BANK_MLC                           3
#define MCA_BANK_QPI0                          5
#define MCA_BANK_IIO                           6
#define MCA_BANK_HA                            7
#define MCA_BANK_HA1_IMC3					   16	
#define MCA_BANK_CBO0                          17
#define MCA_BANK_CBO1                          18
#define MCA_BANK_CBO2                          19
#define MCA_BANK_QPI1                          20
#define MCA_BANK_QPI2                          21

typedef enum {
    IOH_QPI_B4_ERR=0xB4,
    IOH_QPI_C1_ERR=0xC1,
    IOH_QPI_C2_ERR=0xC2,
    IOH_QPI_C3_ERR=0xC3,
    IOH_QPI_D7_ERR=0xD7,
    IOH_QPI_DA_ERR=0xDA,
    IOH_QPI_DB_ERR=0xDB,
} IOH_QPI_ERROR_TYPE;

typedef enum {
    IOH_CORE_C4_ERR = 0xc4,
    IOH_CORE_C5_ERR,
    IOH_CORE_C6_ERR
} IOH_CORE_ERROR_TYPE;

typedef enum {
    IOH_VTD_90_ERR = 0x90,
    IOH_VTD_91_ERR = 0x91,
    IOH_VTD_92_ERR = 0x92,
    IOH_VTD_93_ERR = 0x93,
    IOH_VTD_94_ERR = 0x94,
    IOH_VTD_95_ERR = 0x95,
    IOH_VTD_96_ERR = 0x96,
    IOH_VTD_97_ERR = 0x97,
    IOH_VTD_98_ERR = 0x98,
    IOH_VTD_A0_ERR = 0xA0
} IOH_VTD_ERROR_TYPE;

EFI_STATUS
EnableIohErrors (
  );

VOID
HandleIohQpiErrors (
	IN UINT8   BusNum,
	IN UINT8      Ioh);

VOID
HandleIohCoreErrors (
	IN UINT8   BusNum,
	IN UINT8      Ioh);

VOID
HandleIohVtdErrors (
	IN UINT8   BusNum,
	IN UINT8      Ioh);

VOID
LastBootIohErrorHandler(); 

VOID
LastBootErrorHandler();


VOID
LogUncorrectedError ( 
  ERROR_SOURCE Source,
  UINT8 Type,
  UINT64 MsrMCStatus,
  UINT64 Addr,
  UINT64 Misc);

extern 
EFI_STATUS
CommonErrorHandling (
  VOID  *ErrStruc
);

VOID
CoreErrorHandler ( 
  ERROR_SOURCE Source,
  UINT8	skt,
  UINT8 McBank,
  UINT64 MsrMCStatus,
  UINT64 Addr,
  UINT64 Misc
);

VOID
QpiErrorHandler ( 
  ERROR_SOURCE Source,
  UINT8	skt,
  UINT8 McBank,
  UINT64 MsrMCStatus,
  UINT64 Addr,
  UINT64 Misc
);

VOID
CboErrorHandler ( 
  ERROR_SOURCE Source,
  UINT8	skt,
  UINT8 McBank,
  UINT64 MsrMCStatus,
  UINT64 Addr,
  UINT64 Misc
);

VOID
HandleCoreAndUncoreErrors (
	UINT8 	McBank,
	UINT8	skt,
	EMCA_MC_SIGNATURE_ERR_RECORD *McErrSig
);

#endif

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2015, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**           5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093      **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
