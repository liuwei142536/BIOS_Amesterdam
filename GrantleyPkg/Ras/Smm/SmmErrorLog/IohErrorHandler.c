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
// $Header: /Alaska/SOURCE/Chipset/Intel/NorthBridge/SandyBridgeEP/RtErrorLogging/RtIohElog.c 13    12/07/11 3:09p Bhimanadhunik $
//
// $Revision: 13 $
//
// $Date: 12/07/11 3:09p $
//*************************************************************************
// Revision History
// ----------------
// $Log: /Alaska/SOURCE/Chipset/Intel/NorthBridge/SandyBridgeEP/RtErrorLogging/RtIohElog.c $
// 
// 13    12/07/11 3:09p Bhimanadhunik
// fix in programming the vt-d mask register.
// 
// 10    10/11/11 2:45p Bhimanadhunik
// minor change
// 
// 9     10/07/11 6:09p Bhimanadhunik
// use debug message at runtime based on RUNTIME_ERROR_DEBUG_MESSAGE
// define.
// 
// 8     10/05/11 5:54p Bhimanadhunik
// Added debug messages on error detection.
// 
// 7     8/05/11 11:08a Bhimanadhunik
// Enabling IOH QPI errors by default.
// 
// 6     7/28/11 4:46p Bhimanadhunik
// Ioh vt-d error type issues are fixed.
// 
// 5     7/14/11 6:41p Bhimanadhunik
// Support for handle last boot errors.
// 
// 4     6/03/11 12:51p Bhimanadhunik
// Fix for multi loop issue.
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
//
//*************************************************************************
//<AMI_FHDR_START>
//
// Name:  IohErrorHandler.c
//
//  Description:
//  Initialization and handler code for IOH Errors.
//
//<AMI_FHDR_END>
//*************************************************************************

//---------------------------------------------------------------------------
// Includes

#include <Token.h>
#include "CommonErrorHandlerIncludes.h"

//PORTING////PORTING////PORTING////PORTING////PORTING////PORTING////PORTING////PORTING//

#pragma optimize( "", off )
UINT8						mResetRequired;
UINT8                     	mIohPoisonSupportEnableInSetup = 0;
BOOLEAN mLastbootMemErrFlag = FALSE;
UINT16	mHaMcBankAddress[MAX_HA] = { 0x41c, 0x420};
UINT16	mIMCbankStartAddress	 = 0x424; 
UINT16	mIMCbankEndAddress		 = 0x440;

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
// Procedure:   HandleIohCoreErrors
//
// Description:
//  Event handler for Ioh Core errors. 
// Input:
//
// Output: 
//  VOID
//
// Modified:
//  Nothing
//
// Referrals:
//
// Notes:
//  None
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID
HandleIohCoreErrors(
IN UINT8      BusNum,
IN UINT8      Ioh)
{ 
    UINT32  ErrMask;
    UINT32	Data32 = 0;
    UINT32  IohCoreErr;
    ERROR_DATA_TYPE1    ErrorData;

    IohCoreErr = PciExpressRead32 (PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IIOERRST));
    ErrorData.Header.ErrorType  = IohCoreSource;
    ErrorData.Socket = Ioh;
#if RUNTIME_ERROR_DEBUG_MESSAGE
	DEBUG((-1, "IOH Core Error detected.\n"));
    DEBUG((-1, "HandleIohCoreErrors: IohCoreErr reg 0x%08X, Ioh No 0x%02X \n", IohCoreErr,Ioh));
#endif
    if( IohCoreErr & IOH_CORE_C4){ 
      ErrorData.ErrorSubType = IOH_CORE_C4_ERR;
      ErrorData.Header.ErrorSeverity  = Correctable;
      CommonErrorHandling((VOID *)&ErrorData);
    }
    
    if( IohCoreErr & IOH_CORE_C5){ 
      ErrorData.ErrorSubType = IOH_CORE_C5_ERR;
      ErrorData.Header.ErrorSeverity  = Correctable;
      CommonErrorHandling((VOID *)&ErrorData);
    }

    if( IohCoreErr & IOH_CORE_C6){ 
      ErrorData.ErrorSubType = IOH_CORE_C6_ERR;
      ErrorData.Header.ErrorSeverity  = Correctable;
      CommonErrorHandling((VOID *)&ErrorData);
    }

    if( IohCoreErr & (IOH_CORE_C4 | IOH_CORE_C5 | IOH_CORE_C6)){
        // TO_DO : need to do Power good reset, handle mResetRequired variable
        mResetRequired = PowerGood;
    }

    ErrMask = BIT23;


    // Clear the Fatal and Non-Fatal FERR and NERR.
    Data32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GFFERRST));
    PciExpressWrite32(PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GFFERRST), Data32 | ErrMask);
    Data32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GFNERRST));
    PciExpressWrite32(PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GFNERRST), Data32 | ErrMask);
    Data32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GNFERRST));
    PciExpressWrite32(PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GNFERRST), Data32 | ErrMask);
    Data32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GNNERRST));
    PciExpressWrite32(PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GNNERRST), Data32 | ErrMask);

    return;
  }

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   ClearIohQpiErrors
//
// Description: Clear IOH QPI Errors
//
// Parameters:  
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID
ClearIohQpiErrors(
  UINT8 Bus)
{

  UINT32  Data32;

  Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IRPP0FFERRST));
  PciExpressWrite32 (PCI_PCIE_ADDR(Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IRPP0FFERRST), Data32);

  Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IRPP1FFERRST));
  PciExpressWrite32 (PCI_PCIE_ADDR(Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IRPP1FFERRST), Data32);

  Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IRPP0FNERRST));
  PciExpressWrite32 (PCI_PCIE_ADDR(Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IRPP0FNERRST), Data32);

  Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IRPP1FNERRST));
  PciExpressWrite32 (PCI_PCIE_ADDR(Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IRPP1FNERRST), Data32);
 
  Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IRPP0NFERRST));
  PciExpressWrite32 (PCI_PCIE_ADDR(Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IRPP0NFERRST), Data32);

  Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IRPP1NFERRST));
  PciExpressWrite32 (PCI_PCIE_ADDR(Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IRPP1NFERRST), Data32);

  Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IRPP0NNERRST));
  PciExpressWrite32 (PCI_PCIE_ADDR(Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IRPP0NNERRST), Data32);

  Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IRPP1NNERRST));
  PciExpressWrite32 (PCI_PCIE_ADDR(Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IRPP1NNERRST), Data32);

}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
// Procedure:   HandleIohQpiErrors
//
// Description:
//  Event handler for Ioh QPI errors. 
// Input:
//
// Output: 
//  VOID
//
// Modified:
//  Nothing
//
// Referrals:
//
// Notes:
//  None
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID
HandleIohQpiErrors(
IN UINT8      BusNum,
IN UINT8      Ioh)
{ 
  
  BOOLEAN FatalError = FALSE; 
  UINT32  ErrMask;
  UINT32	Data32 = 0;
  UINT16	Data16 = 0;
  UINT8	  Data8 = 0;
  UINT32  IohQpiPErr;
  UINT32  IohQpiP0Err;
  UINT32  IohQpiP1Err;
  ERROR_DATA_TYPE1    ErrorData;

#if RUNTIME_ERROR_DEBUG_MESSAGE
   DEBUG((-1, "IOH QPI Error detected.\n"));
#endif

    // First, read the global system status register.
    Data32 = PciExpressRead32 (PCI_PCIE_ADDR(CORE05_BUS_NUM, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GSYSST));
    
    // Severity 2 fatal error 
    if (Data32 & V_IIO_GSYSCTL_S2STS) {
      FatalError = TRUE;
    }
      
    IohQpiP0Err = PciExpressRead32 (PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IRPP0ERRST));
    IohQpiP1Err = PciExpressRead32 (PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IRPP1ERRST));

    ErrorData.Header.ErrorType  = IohQpiSource;
    ErrorData.Socket = Ioh;
        
    if( IohQpiP0Err & QPI_P_ERRMSK){
      ErrorData.Port = 0;
    }

    if( IohQpiP0Err & QPI_P_ERRMSK){
      ErrorData.Port = 1;
    }

    IohQpiPErr = (IohQpiP0Err | IohQpiP1Err);

#if RUNTIME_ERROR_DEBUG_MESSAGE
   DEBUG((-1, "HandleIohQpiErrors: IohQpiPErr reg 0x%08X, Ioh No 0x%02X \n", IohQpiPErr,Ioh));
#endif

    // Protocol errors 

      if( IohQpiPErr & B_IIO_IRPPCTL_WRI_CACHE_COR_EN){
      ErrorData.ErrorSubType = IOH_QPI_B4_ERR;
      ErrorData.Header.ErrorSeverity  = Correctable;
      CommonErrorHandling((VOID *)&ErrorData);
    }

    if( IohQpiPErr & B_IIO_IRPPCTL_LAY_EN){
      ErrorData.ErrorSubType = IOH_QPI_C1_ERR;
      ErrorData.Header.ErrorSeverity  = Recoverable;
      CommonErrorHandling((VOID *)&ErrorData);
    }

    if( IohQpiPErr & B_IIO_IPRERR_WRI_CAC_UNCOR_ECC_EN){
      ErrorData.ErrorSubType = IOH_QPI_C2_ERR;
      ErrorData.Header.ErrorSeverity  = Recoverable;
      CommonErrorHandling((VOID *)&ErrorData);
    }

    if( IohQpiPErr & B_IIO_IPRERR_CSR_CRO32BIT_BOUN_EN){
      ErrorData.ErrorSubType = IOH_QPI_C3_ERR;
      ErrorData.Header.ErrorSeverity  = Recoverable;
      CommonErrorHandling((VOID *)&ErrorData);
    }

    if( IohQpiPErr & B_IIO_IPRERR_PRO_REC_UNRES_EN){
      ErrorData.ErrorSubType = IOH_QPI_D7_ERR;
      ErrorData.Header.ErrorSeverity  = Fatal;
      CommonErrorHandling((VOID *)&ErrorData);
    }

    if( IohQpiPErr & B_IIO_IPRERR_PRO_QUETAB_FLOW_EN){
      ErrorData.ErrorSubType = IOH_QPI_DA_ERR;
      ErrorData.Header.ErrorSeverity  = Fatal;
      CommonErrorHandling((VOID *)&ErrorData);
    }

    if( IohQpiPErr & B_IIO_IRPERR_PRO_PAR_ERR_EN){
      ErrorData.ErrorSubType = IOH_QPI_DB_ERR;
      ErrorData.Header.ErrorSeverity  = Fatal;
      CommonErrorHandling((VOID *)&ErrorData);
    }

    // Clear IOH QPI Errors

      ClearIohQpiErrors(BusNum);

     // Set up the IOH QPI Error Mask.

      ErrMask = (BIT0 | BIT1);


      // Clear the Fatal and Non-Fatal FERR and NERR.
      Data32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GFFERRST));
      PciExpressWrite32(PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GFFERRST), Data32 | ErrMask);
      Data32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GFNERRST));
      PciExpressWrite32(PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GFNERRST), Data32 | ErrMask);
      Data32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GNFERRST));
      PciExpressWrite32(PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GNFERRST), Data32 | ErrMask);
      Data32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GNNERRST));
      PciExpressWrite32(PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GNNERRST), Data32 | ErrMask);


    if (FatalError) {
      // Do the System reset on fatal errors, need to check all kind of errors before reset.
	  //	SystemResetOnFe ();
    }
    return;
  }

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
// Procedure:   LogIohVtdError
//
// Description:
//  Detect type of VTd error
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID
LogIohVtdError(
	IN UINT32      IohVtdErr,
	IN UINT8      Ioh)
{ 

	ERROR_DATA_TYPE1    ErrorData;

    ErrorData.Header.ErrorType  = IohVtdSource;
    ErrorData.Socket = Ioh;


	if( IohVtdErr & BIT0){ 
      ErrorData.ErrorSubType = IOH_VTD_90_ERR;
      ErrorData.Header.ErrorSeverity  = Fatal;
      CommonErrorHandling((VOID *)&ErrorData);
    }

    if( IohVtdErr & BIT1){ 
      ErrorData.ErrorSubType = IOH_VTD_91_ERR;
      ErrorData.Header.ErrorSeverity  = Fatal;
      CommonErrorHandling((VOID *)&ErrorData);
    }

	if( IohVtdErr & BIT2){
      ErrorData.ErrorSubType = IOH_VTD_92_ERR;
      ErrorData.Header.ErrorSeverity  = Fatal;
      CommonErrorHandling((VOID *)&ErrorData);
    }

	if( IohVtdErr & BIT3){
      ErrorData.ErrorSubType = IOH_VTD_93_ERR;
      ErrorData.Header.ErrorSeverity  = Fatal;
      CommonErrorHandling((VOID *)&ErrorData);
    }

	if( IohVtdErr & BIT4){
      ErrorData.ErrorSubType = IOH_VTD_94_ERR;
      ErrorData.Header.ErrorSeverity  = Fatal;
      CommonErrorHandling((VOID *)&ErrorData);
    }

	if( IohVtdErr & BIT5){
      ErrorData.ErrorSubType = IOH_VTD_95_ERR;
      ErrorData.Header.ErrorSeverity  = Fatal;
      CommonErrorHandling((VOID *)&ErrorData);
    }

	if( IohVtdErr & BIT6){
      ErrorData.ErrorSubType = IOH_VTD_96_ERR;
      ErrorData.Header.ErrorSeverity  = Recoverable;
      CommonErrorHandling((VOID *)&ErrorData);
    }

	if( IohVtdErr & BIT7){
      ErrorData.ErrorSubType = IOH_VTD_97_ERR;
      ErrorData.Header.ErrorSeverity  = Fatal;
      CommonErrorHandling((VOID *)&ErrorData);
    }

	if( IohVtdErr & BIT8){
      ErrorData.ErrorSubType = IOH_VTD_98_ERR;
      ErrorData.Header.ErrorSeverity  = Fatal;
      CommonErrorHandling((VOID *)&ErrorData);
    }

	if( IohVtdErr & BIT31){
      ErrorData.ErrorSubType = IOH_VTD_A0_ERR;
      ErrorData.Header.ErrorSeverity  = Recoverable;
      CommonErrorHandling((VOID *)&ErrorData);
    }
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
// Procedure:   HandleIohVtdErrors
//
// Description:
//  Event handler for Ioh Vtd errors. 
// Input:
//
// Output: 
//  VOID
//
// Modified:
//  Nothing
//
// Referrals:
//
// Notes:
//  None
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID
HandleIohVtdErrors(
IN UINT8      BusNum,
IN UINT8      Ioh)
{ 
    UINT32  ErrMask;
	UINT32	Data32 = 0;
    UINT32  IohVtdErr;

    IohVtdErr = PciExpressRead32 (PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IIO_VTUNCERRSTS));

#if RUNTIME_ERROR_DEBUG_MESSAGE
    DEBUG((-1, "IOH Vtd Error detected.\n"));
    DEBUG((-1, "HandleIohVtdErrors: IohVtdErr reg 0x%08X, Ioh No 0x%02X \n", IohVtdErr,Ioh));
#endif

    LogIohVtdError (IohVtdErr,Ioh);


      ErrMask = BIT25;

      // Clear the Fatal and Non-Fatal FERR and NERR.
      Data32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GFFERRST));
      PciExpressWrite32(PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GFFERRST), Data32 | ErrMask);
      Data32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GFNERRST));
      PciExpressWrite32(PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GFNERRST), Data32 | ErrMask);
      Data32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GNFERRST));
      PciExpressWrite32(PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GNFERRST), Data32 | ErrMask);
      Data32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GNNERRST));
      PciExpressWrite32(PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GNNERRST), Data32 | ErrMask);


    return;
  }

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
// Procedure:   HandleLastBootIohQpiErrors
//
// Description:
//  Event handler for Ioh QPI errors. 
// Input:
//
// Output: 
//  VOID
//
// Modified:
//  Nothing
//
// Referrals:
//
// Notes:
//  None
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID
HandleLastBootIohQpiErrors(
        IN UINT8      BusNum,
        IN UINT8      Ioh)
{ 
  
  BOOLEAN FatalError = FALSE; 
  UINT32	Data32 = 0;
  UINT32  IohQpiPErr;
  UINT32  IohQpiP0Err;
  UINT32  IohQpiP1Err;
  ERROR_DATA_TYPE1    LastbootErrorData;

    DEBUG((-1, "IOH QPI Error detected.\n"));


    // First, read the global system status register.
    Data32 = PciExpressRead32 (PCI_PCIE_ADDR(CORE05_BUS_NUM, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GSYSST));

    // Severity 2 fatal error 
    if (Data32 & V_IIO_GSYSCTL_S2STS) {
      FatalError = TRUE;
    }

    IohQpiP0Err = PciExpressRead32 (PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IRPP0ERRST));
    IohQpiP1Err = PciExpressRead32 (PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IRPP1ERRST));

    LastbootErrorData.Header.ErrorType  = IohQpiSource;
    LastbootErrorData.Socket = Ioh;

    if( IohQpiP0Err & QPI_P_ERRMSK){
      LastbootErrorData.Port = 0;
    }

    if( IohQpiP1Err & QPI_P_ERRMSK){
      LastbootErrorData.Port = 1;
    }

    IohQpiPErr = (IohQpiP0Err | IohQpiP1Err);

    // Protocol errors 
    if( IohQpiPErr & B_IIO_IRPPCTL_WRI_CACHE_COR_EN){
      LastbootErrorData.ErrorSubType = IOH_QPI_B4_ERR;
      LastbootErrorData.Header.ErrorSeverity  = Correctable;
      CommonErrorHandling((VOID *)&LastbootErrorData);
    }

    if( IohQpiPErr & B_IIO_IRPPCTL_LAY_EN){
      LastbootErrorData.ErrorSubType = IOH_QPI_C1_ERR;
      LastbootErrorData.Header.ErrorSeverity  = Recoverable;
      CommonErrorHandling((VOID *)&LastbootErrorData);
    }

    if( IohQpiPErr & B_IIO_IPRERR_WRI_CAC_UNCOR_ECC_EN){
      LastbootErrorData.ErrorSubType = IOH_QPI_C2_ERR;
      LastbootErrorData.Header.ErrorSeverity  = Recoverable;
      CommonErrorHandling((VOID *)&LastbootErrorData);
    }

    if( IohQpiPErr & B_IIO_IPRERR_CSR_CRO32BIT_BOUN_EN){
      LastbootErrorData.ErrorSubType = IOH_QPI_C3_ERR;
      LastbootErrorData.Header.ErrorSeverity  = Recoverable;
      CommonErrorHandling((VOID *)&LastbootErrorData);
    }

    if( IohQpiPErr & B_IIO_IPRERR_PRO_REC_UNRES_EN){
      LastbootErrorData.ErrorSubType = IOH_QPI_D7_ERR;
      LastbootErrorData.Header.ErrorSeverity  = Fatal;
      CommonErrorHandling((VOID *)&LastbootErrorData);
    }

    if( IohQpiPErr & B_IIO_IPRERR_PRO_QUETAB_FLOW_EN){
      LastbootErrorData.ErrorSubType = IOH_QPI_DA_ERR;
      LastbootErrorData.Header.ErrorSeverity  = Fatal;
      CommonErrorHandling((VOID *)&LastbootErrorData);
    }

    if( IohQpiPErr & B_IIO_IRPERR_PRO_PAR_ERR_EN){
      LastbootErrorData.ErrorSubType = IOH_QPI_DB_ERR;
      LastbootErrorData.Header.ErrorSeverity  = Fatal;
      CommonErrorHandling((VOID *)&LastbootErrorData);
    }  
   if (FatalError) {
      // Do the System reset on fatal errors, need to check all kind of errors before reset.
	  //	SystemResetOnFe ();
    }
    return;
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
// Procedure:   HandleLastBootIohCoreErrors
//
// Description:
//  Event handler for Ioh Core errors. 
// Input:
//
// Output: 
//  VOID
//
// Modified:
//  Nothing
//
// Referrals:
//
// Notes:
//  None
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID
HandleLastBootIohCoreErrors(
    IN UINT8      BusNum,
    IN UINT8      Ioh

)
{ 
    UINT32  IohCoreErr;
    ERROR_DATA_TYPE1    LastbootErrorData;

    DEBUG((-1, "IOH Core Error detected.\n"));

    IohCoreErr = PciExpressRead32 (PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_IIOERRST));
    LastbootErrorData.Header.ErrorType  = IohCoreSource;
    LastbootErrorData.Socket = Ioh;

    if( IohCoreErr & IOH_CORE_C4){ 
      LastbootErrorData.ErrorSubType = IOH_CORE_C4_ERR;
      LastbootErrorData.Header.ErrorSeverity  = Correctable;
      CommonErrorHandling((VOID *)&LastbootErrorData);
    }

    if( IohCoreErr & IOH_CORE_C5){ 
      LastbootErrorData.ErrorSubType = IOH_CORE_C5_ERR;
      LastbootErrorData.Header.ErrorSeverity  = Correctable;
      CommonErrorHandling((VOID *)&LastbootErrorData);
    }

    if( IohCoreErr & IOH_CORE_C6){ 
      LastbootErrorData.ErrorSubType = IOH_CORE_C6_ERR;
      LastbootErrorData.Header.ErrorSeverity  = Correctable;
      CommonErrorHandling((VOID *)&LastbootErrorData);
    }
    return;
  }

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
// Procedure:   HandleLastBootIohVtdErrors
//
// Description:
//  Event handler for Ioh Vtd errors. 
// Input:
//
// Output: 
//  VOID
//
// Modified:
//  Nothing
//
// Referrals:
//
// Notes:
//  None
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID
HandleLastBootIohVtdErrors(
IN UINT8      BusNum,
IN UINT8      Ioh
)
{ 

    UINT32  IohVtdErr;

    DEBUG((-1, "IOH Vtd Error detected.\n"));

    IohVtdErr = PciExpressRead32 (PCI_PCIE_ADDR(BusNum, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IIO_VTUNCERRSTS));

    LogIohVtdError (IohVtdErr,Ioh);

    return;
  }

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
// Procedure:   LastBootPcieRootInformation
//
// Description:
//  Event handler for LastBoot Pcie Errors. 
// Input:
//
// Output: 
//  VOID
//
// Modified:
//  Nothing
//
// Referrals:
//
// Notes:
//  None
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID
LastBootPcieRootInformation(
IN  UINT8  Bus,		
IN	UINT16 ErrorPortNumber
)
{
	PCI_PCIE_ERROR_LOCAL_STRUC            LastBootPciErrStruc;
	LastBootPciErrStruc.ErrorSource  = PciSource;
	LastBootPciErrStruc.ErrorType    = 0x0A;   // To Represent the Last boot Error in SELVIEW
	LastBootPciErrStruc.Segment      = 0;
	switch (ErrorPortNumber)
    {
    	case 0 :   // DMI or Por 0
    		LastBootPciErrStruc.Bus          = Bus;
    		LastBootPciErrStruc.Device       = 0;
    		LastBootPciErrStruc.Function     = 0;
    		break;
    		
    	case 1 :  // PORT 1A
    		LastBootPciErrStruc.Bus          = Bus;
    		LastBootPciErrStruc.Device       = 1;
    		LastBootPciErrStruc.Function     = 0;
    		break;
    		
    	case 2 :  // PORT 1B
    		LastBootPciErrStruc.Bus          = Bus;
    		LastBootPciErrStruc.Device       = 1;
    		LastBootPciErrStruc.Function     = 1;
    		break;
    		    		
    	case 3 :  // PORT 2A
    		LastBootPciErrStruc.Bus          = Bus;
    		LastBootPciErrStruc.Device       = 2;
    		LastBootPciErrStruc.Function     = 0;
    		break;
    		    		
    	case 4 :  // PORT 2B
    		LastBootPciErrStruc.Bus          = Bus;
    		LastBootPciErrStruc.Device       = 2;
    		LastBootPciErrStruc.Function     = 1;
    		break;
    		    		
    	case 5 :  // PORT 2C
    		LastBootPciErrStruc.Bus          = Bus;
    		LastBootPciErrStruc.Device       = 2;
    		LastBootPciErrStruc.Function     = 2;
    		break;
    		    		
    	case 6 :  // PORT 2D
    		LastBootPciErrStruc.Bus          = Bus;
    		LastBootPciErrStruc.Device       = 2;
    		LastBootPciErrStruc.Function     = 3;
    		break;
    		    		
    	case 7 :  // PORT 3A
    		LastBootPciErrStruc.Bus          = Bus;
    		LastBootPciErrStruc.Device       = 3;
    		LastBootPciErrStruc.Function     = 0;
    		break;
    		    		
    	case 8 :  // PORT 3B
    		LastBootPciErrStruc.Bus          = Bus;
    		LastBootPciErrStruc.Device       = 3;
    		LastBootPciErrStruc.Function     = 1;
    		break;
    		    		
    	case 9 :  // PORT 3C
    		LastBootPciErrStruc.Bus          = Bus;
    		LastBootPciErrStruc.Device       = 3;
    		LastBootPciErrStruc.Function     = 2;
    		break;
    		    		
    	case 10 : // PORT 3D
    		LastBootPciErrStruc.Bus          = Bus;
    		LastBootPciErrStruc.Device       = 3;
    		LastBootPciErrStruc.Function     = 3;
    		break;
    		    		
      	default :
    		LastBootPciErrStruc.Bus          = Bus;
    		LastBootPciErrStruc.Device       = 0xFF;
    		LastBootPciErrStruc.Function     = 0xFF;
    		break;
    }
    CommonErrorHandling((VOID *)&LastBootPciErrStruc);          
    return ;
}
//<AMI_PHDR_START>
//---------------------------------------------------------------------------
// Procedure:   HandlelastbootPcieErrors
//
// Description:
//  Primary event handler for PCI errors. Commences search for PCI error and
//  clears SMI status bits when complete. // 
//
// Input:

//
// Output: 

//
// Modified:
//  Nothing
//
// Referrals:

//
// Notes:
//  None
//---------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID
HandlelastbootPcieErrors()
{
	
  UINT8       RootBridgeLoop;
  UINT8 	  SocketId;
  GFERRST_IIO_RAS_STRUCT iioGFERRSTReg;
  GNERRST_IIO_RAS_STRUCT iioGNERRSTReg;
  UINT8		  Bus;
  UINT16      ErrorPortNumber;
//  UINT8 	  pcieferr;
//  UINT8       pcienerr;
  
	
  for ( RootBridgeLoop =0; RootBridgeLoop < mNumIoh; RootBridgeLoop++) {
	if(mIioUds->PlatformData.IIO_resource[RootBridgeLoop].Valid != 1) continue;
    	
	SocketId = mIioUds->PlatformData.IIO_resource[RootBridgeLoop].SocketID;
	
	Bus = mIioUds->PlatformData.IioQpiInfo[RootBridgeLoop].BusNum;
	//
	// Global Fatal Error Status
	//
	iioGFERRSTReg.Data = mCpuCsrAccess->ReadCpuCsr(SocketId, 0, GFERRST_IIO_RAS_REG); 
	iioGNERRSTReg.Data = mCpuCsrAccess->ReadCpuCsr(SocketId, 0, GNERRST_IIO_RAS_REG); 
/*	
	pcieferr = iioGFERRSTReg.Bits.dmi    | 
	                 iioGFERRSTReg.Bits.pcie0  | 
	                 iioGFERRSTReg.Bits.pcie1  | 
	                 iioGFERRSTReg.Bits.pcie2  |
	                 iioGFERRSTReg.Bits.pcie3  |
	                 iioGFERRSTReg.Bits.pcie4  |
	                 iioGFERRSTReg.Bits.pcie5  |
	                 iioGFERRSTReg.Bits.pcie6  |
	                 iioGFERRSTReg.Bits.pcie7  |
	                 iioGFERRSTReg.Bits.pcie8  |
	                 iioGFERRSTReg.Bits.pcie9  |
	                 iioGFERRSTReg.Bits.pcie10 ;
	      
	pcienerr = iioGNERRSTReg.Bits.dmi    | 
	                 iioGNERRSTReg.Bits.pcie0  | 
	                 iioGNERRSTReg.Bits.pcie1  | 
	                 iioGNERRSTReg.Bits.pcie2  |
	                 iioGNERRSTReg.Bits.pcie3  |
	                 iioGNERRSTReg.Bits.pcie4  |
	                 iioGNERRSTReg.Bits.pcie5  |
	                 iioGNERRSTReg.Bits.pcie6  |
	                 iioGNERRSTReg.Bits.pcie7  |
	                 iioGNERRSTReg.Bits.pcie8  |
	                 iioGNERRSTReg.Bits.pcie9  |
	                 iioGNERRSTReg.Bits.pcie10 ;
   */
	
	if ((iioGFERRSTReg.Bits.dmi)  || (iioGNERRSTReg.Bits.dmi)) // DMI Port
	{
		ErrorPortNumber = 0 ;
		LastBootPcieRootInformation(Bus,ErrorPortNumber);
	}
	
	if ((iioGFERRSTReg.Bits.pcie0)  || (iioGNERRSTReg.Bits.pcie0))
	{
		ErrorPortNumber = 0 ;  // Port 0 
		LastBootPcieRootInformation(Bus,ErrorPortNumber);
	}
		
	if ((iioGFERRSTReg.Bits.pcie1)  || (iioGNERRSTReg.Bits.pcie1))
	{
		ErrorPortNumber = 1 ; //  Port 1A 
		LastBootPcieRootInformation(Bus,ErrorPortNumber);
	}
			
	if ((iioGFERRSTReg.Bits.pcie2)  || (iioGNERRSTReg.Bits.pcie2))
	{
		ErrorPortNumber = 2 ;  // Port 1B
		LastBootPcieRootInformation(Bus,ErrorPortNumber);
	}

	if ((iioGFERRSTReg.Bits.pcie3)  || (iioGNERRSTReg.Bits.pcie3))
	{
		ErrorPortNumber = 3 ;  // Port 2A
		LastBootPcieRootInformation(Bus,ErrorPortNumber);
	}
			
	if ((iioGFERRSTReg.Bits.pcie4)  || (iioGNERRSTReg.Bits.pcie4))
	{
		ErrorPortNumber = 4 ;  // Port 2B
		LastBootPcieRootInformation(Bus,ErrorPortNumber);
	}
				
	if ((iioGFERRSTReg.Bits.pcie5)  || (iioGNERRSTReg.Bits.pcie5))
	{
		ErrorPortNumber = 5 ;  // Port 2C
		LastBootPcieRootInformation(Bus,ErrorPortNumber);
	}

	if ((iioGFERRSTReg.Bits.pcie6)  || (iioGNERRSTReg.Bits.pcie6))
	{
		ErrorPortNumber = 6 ; // Port 2D
		LastBootPcieRootInformation(Bus,ErrorPortNumber);
	}
						
	if ((iioGFERRSTReg.Bits.pcie7)  || (iioGNERRSTReg.Bits.pcie7))
	{
		ErrorPortNumber = 7 ; // Port 3A
		LastBootPcieRootInformation(Bus,ErrorPortNumber);
	}
							
	if ((iioGFERRSTReg.Bits.pcie8)  || (iioGNERRSTReg.Bits.pcie8))
	{
		ErrorPortNumber = 8 ; // Port 3B
		LastBootPcieRootInformation(Bus,ErrorPortNumber);
	}
								
	if ((iioGFERRSTReg.Bits.pcie9)  || (iioGNERRSTReg.Bits.pcie9))
	{
		ErrorPortNumber = 9 ;  // Port 3C
		LastBootPcieRootInformation(Bus,ErrorPortNumber);
	}
		
	if ((iioGFERRSTReg.Bits.pcie10)  || (iioGNERRSTReg.Bits.pcie10))
	{
		ErrorPortNumber = 10 ;  // Port 3D
		LastBootPcieRootInformation(Bus,ErrorPortNumber);
	}

  }
  return ;
	
}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
// Procedure:   HandleLastBootIohErrors
//
// Description:
//  Handler for IOH Errors.
//
// Input:

// Output:
//  VOID
//
// Modified:
//  Nothing

// Notes:
//  None
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID
HandleLastBootIohErrors()
{
  UINT32  		GlobalFerr;
  UINT32  		GlobalNerr;
  UINT8       	Ioh;

  // Handle Last boot PCI-e errors
  HandlelastbootPcieErrors(); 
  for ( Ioh = 0; Ioh < MAX_SOCKET; Ioh++) {
    if (PciExpressRead16 (PCI_PCIE_ADDR(mIohInfo[Ioh].BusNum, CORE05_DEV_NUM, CORE05_FUNC_NUM, PCI_VENDOR_ID_OFFSET)) != 0xffff) {
      GlobalFerr = PciExpressRead32 (PCI_PCIE_ADDR(mIohInfo[Ioh].BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GFERRST));
      GlobalNerr = PciExpressRead32 (PCI_PCIE_ADDR(mIohInfo[Ioh].BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GNERRST));

      if((GlobalFerr | GlobalNerr) & ( GST_QPI0 |GST_QPI1 ) ) {
        HandleLastBootIohQpiErrors(mIohInfo[Ioh].BusNum, Ioh );
      }
      if((GlobalFerr | GlobalNerr) & ( GST_IOH_CORE_ERR) ) {
        HandleLastBootIohCoreErrors(mIohInfo[Ioh].BusNum, Ioh );
      }
      if((GlobalFerr | GlobalNerr) & ( GST_IOH_VTD_ERR) ) {
        HandleLastBootIohVtdErrors(mIohInfo[Ioh].BusNum, Ioh );
      }
    } //if
  } // for

}

//<AMI_PHDR_START>
//---------------------------------------------------------------------------
// Procedure:   LastBootIohErrorHandler
//
// Description:
//  LastBoot Ioh Error Handler.
//
// Input:

// Output:
//  VOID
//
// Modified:
//  Nothing

// Notes:
//  None
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID
LastBootIohErrorHandler()
{
 
     /* During System Reset R_IIO_GSYSST 0x1CC values are not retained but R_IIO_GFERRST, R_IIO_GNERRST register values are retained.
     * R_IIO_GFERRST,R_IIO_GNERRST register value is enough to identify the last boot error.
     */
    // First, read the global system status register.
    //Data32 = PciExpressRead32 (PCI_PCIE_ADDR(CORE05_BUS_NUM, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_GSYSST));
    // Then, if any of the possible PCIe status bits are set, call the handler.
    // This logic could be made more complicated, if need-be, but this should work.
//    if (Data32 & (V_IIO_GSYSCTL_S0STS | V_IIO_GSYSCTL_S1STS | V_IIO_GSYSCTL_S2STS)) {
        HandleLastBootIohErrors();
//    }
	return;
}

VOID ClearMSR (VOID *Buffer)
{
	UINT32  Ia32McgCap = (UINT32) AsmReadMsr64(EFI_IA32_MCG_CAP);
	UINT8   Count = (UINT8) Ia32McgCap;
	UINT8   i;
	
	// Clear all MC bank registers.
	if (1) { //Power-on reset detected
	        //Clear all errors
		for(i = 0; i < Count; ++i) AsmWriteMsr64(EFI_IA32_MC0_STATUS + 4 * i, 0); //Write 0 to IA32_MCi_STATUS
	} 
	
}
//<AMI_PHDR_START>
//---------------------------------------------------------------------------
// Procedure:   LastBootErrorHandler
//
// Description:
//  LastBoot Error Handler.
//
// Input:

// Output:
//  VOID
//
// Modified:
//  Nothing

// Notes:
//  None
//---------------------------------------------------------------------------
//<AMI_PHDR_END>

VOID
LastBootErrorHandler(EFI_SMM_SYSTEM_TABLE2* mSmst)
{
//EFI_STATUS  Status = EFI_SUCCESS;
	// ****** Porting  ************

	
	UINTN       thread;
    UINT8                           nodeId;
	UINT8                           socket;
	mLastbootMemErrFlag = TRUE;
	ElogProcessorErrorHandler ();
	mLastbootMemErrFlag = FALSE;
	 for (thread = 0; thread < mELogHost.cpuCount; thread++) {
		    mSmst->SmmStartupThisAp (ClearMSR,
		                             thread, 
		                             NULL);
	 }
	 ClearMSR(&thread);
	  for (socket = 0; socket < MAX_SOCKET; socket++) {
	    //
		// Socket is empty, skip it
		//
	    if (mIohInfo[socket].Valid != 1)
	        continue;
	    for (nodeId = 0; nodeId <= MAX_HA-1; nodeId++) {
	        //
	        // If this node is not present, move to next node
	        //
	        if (!mMemRas->SystemMemInfo->NodeInfo[SKTMC_TO_NODE(socket, nodeId)].Enabled) {
	                continue;
	        }
			//
			// Clear MC5_STATUS_SHADOW
			//
	        mCpuCsrAccess->WriteCpuCsr(socket,nodeId,MC5_STATUS_SHADOW_N1_HA_PMON_REG, 0);
	        mCpuCsrAccess->WriteCpuCsr(socket,nodeId,MC5_STATUS_SHADOW_N0_HA_PMON_REG, 0);
       }
	  }
	// Handle Last Boot IOH errors 
	LastBootIohErrorHandler();
	// ****** Porting  ************

	return ;
}

VOID
LogUncorrectedError ( 
  ERROR_SOURCE Source,
  UINT8 Type,
  UINT64 MsrMCStatus,
  UINT64 Addr,
  UINT64 Misc)
{
    MEMORY_DEV_INFO               MemInfo;  // Whea Error record uses MEMORY_DEV_INFO structure
    PCI_PCIE_ERROR_LOCAL_STRUC            PciErrStruc; 
	UINT8               core = 00;
	UINT64              physicalAddress = 00;
	UINT8				SocketId = 00;
	UINT8				MemoryControllerId = 00;
	UINT8				ChannelId = 00;
	UINT8				DimmSlot = 00;
	UINT8				DimmRank = 00;
	UINT32				Row = 00;
	UINT32				Col = 00;
	UINT8				Bank = 00;
	UINT8               BankGroup = 00;
	UINT16				McBankAddress = 00;
	IA32_MCI_STATUS_MSR_STRUCT MsrIa32MCiStatus;
	UINT8					Bus = 0x0FF;
	UINT8					Device = 0x0FF;
	UINT8					Function = 0x0FF;
	UINT8					Segment = 0;
    UINT8                   ChipSelect = 00;

  //
  // Clear the MemInfo structure so that unused elements
  // have a predictable value in the error record.
  //
  SetMem (&MemInfo, sizeof (MEMORY_DEV_INFO), 0xFF);
  MsrIa32MCiStatus.Data = MsrMCStatus;

// To handle Memory UCE Errors 	
  if (Source == MemSource) {
	MemInfo.Source = MemSource;
	MemInfo.Type = Type;
	MemInfo.ValidBits = 0;
	MemInfo.UefiErrorRecordRevision = mErrLogSetupData->WheaErrorRecordFormat;
    if (MsrIa32MCiStatus.Bits.addrv && MsrIa32MCiStatus.Bits.miscv) {
	//
	// Convert Addr, Misc to physical address
	//
	physicalAddress = MCAddress (Addr, Misc);
	//
	// Translate physical address to DIMM
	//
	mMemRas->SystemAddressToDimmAddress (physicalAddress,&SocketId,&MemoryControllerId,&ChannelId,&DimmSlot,&DimmRank,&Row,&Col,&Bank,&BankGroup,&ChipSelect);
    MemInfo.ValidBits = PLATFORM_MEM_PHY_ADDRESS_VALID | PLATFORM_MEM_PHY_ADDRESS_MASK_VALID | PLATFORM_MEM_NODE_VALID 
		  	  	  	  | PLATFORM_MEM_CARD_VALID | PLATFORM_MEM_ROW_VALID | PLATFORM_MEM_COLUMN_VALID
		  	  	  	  | PLATFORM_MEM_MODULE_VALID | PLATFORM_MEM_BANK_VALID;
    MemInfo.Node      = SKTMC_TO_NODE(SocketId, MemoryControllerId);
    MemInfo.Branch    = 00;
    MemInfo.PhyAddr   = physicalAddress;
    MemInfo.PhyAddrMask = 0x3fffffffffc0;
    MemInfo.Channel   = ChannelId;
    MemInfo.Dimm      = DimmSlot;
    MemInfo.Column    = Col;
    MemInfo.Bank      = Bank;
    MemInfo.Row       = Row;
    if( mErrLogSetupData->WheaErrorRecordFormat == GENERIC_ERROR_SECTION_REVISION_UEFI231) {
      MemInfo.Rank      = DimmRank;
      MemInfo.ValidBits |= PLATFORM_RANK_NUMBER_VALID;
      if(mSmbiosProtocol != NULL) {
        MemInfo.ValidBits |= PLATFORM_CARD_HANDLE_VALID | PLATFORM_Module_HANDLE_VALID;
        MemInfo.SmBiosCardHandle = mELogHost.ElogType16Hanlde; // Type 16 handle;
        MemInfo.SmBiosModuleHandle = mELogHost.ElogSmBiosType17[(SKTMC_TO_NODE(SocketId, MemoryControllerId))][ChannelId][DimmSlot].SmBiosType17Handle; // Type 17 handle
      }
    }
  }

  CommonErrorHandling((VOID *)&MemInfo);
  }

// To Handle PCIE UCE Errors  
  if (Source == PciSource) 
  {
  	if(MsrIa32MCiStatus.Bits.miscv) 
  	{
  		Segment = (Misc & 0x0FF0) >> 8; 
  		Bus     = (Misc & 0x0FF000000) >> 24;
  		Device  = (Misc & 0xF80000) >> 19;
  		Function = (Misc & 70000) >> 16 ;
  	    PciErrStruc.ErrorSource  = PciSource;
  	    PciErrStruc.ErrorType    = Type;
  	    PciErrStruc.Segment      = Segment;
  	    PciErrStruc.Bus          = Bus;
  	    PciErrStruc.Device       = Device;
  	    PciErrStruc.Function     = Function;
  	  
  	    CommonErrorHandling((VOID *)&PciErrStruc);
  	}

  	 
  }

  return;
}

VOID
CoreErrorHandler ( 
  ERROR_SOURCE Source,
  UINT8	skt,
  UINT8 McBank,
  UINT64 MsrMCStatus,
  UINT64 Addr,
  UINT64 Misc)
{
	CORE_ERROR_RECORD			CoreInfo;
	IA32_MCI_STATUS_MSR_STRUCT 	MsrIa32MCiStatus;

	ZeroMem (&CoreInfo, sizeof (CORE_ERROR_RECORD));
	
	if (McBank == MCA_BANK_IFU) {
		CoreInfo.CoreMcaBank = 0;
	}
	else if (McBank == MCA_BANK_DCU) {
		CoreInfo.CoreMcaBank = 1;
	}
	else if (McBank == MCA_BANK_DLTB) {
		CoreInfo.CoreMcaBank = 2;
	}
	else {
		CoreInfo.CoreMcaBank = 3;
	}
	CoreInfo.Socket = skt;
	CoreInfo.Valid = TRUE;
	CoreInfo.ErrorType = Source;
	MsrIa32MCiStatus.Data = MsrMCStatus;
	if (MsrIa32MCiStatus.Bits.uc) {
		CoreInfo.ErrorSeverity = FATAL_ERROR_TYPE;
	}
	else {
		CoreInfo.ErrorSeverity = COR_ERROR_TYPE;
	}
	CoreInfo.CorrectedErrorCount = MsrIa32MCiStatus.Bits.corrected_err_count;
	CoreInfo.ModelSpecificCode = MsrIa32MCiStatus.Bits.ms_code;
	CoreInfo.MachineCheckCode = MsrIa32MCiStatus.Bits.mca_code;

	CommonErrorHandling((VOID *)&CoreInfo);
}

VOID
QpiErrorHandler ( 
  ERROR_SOURCE Source,
  UINT8	skt,
  UINT8 McBank,
  UINT64 MsrMCStatus,
  UINT64 Addr,
  UINT64 Misc)
{
	QPI_ERROR_RECORD			QpiInfo;
	IA32_MCI_STATUS_MSR_STRUCT 	MsrIa32MCiStatus;

	ZeroMem (&QpiInfo, sizeof (QPI_ERROR_RECORD));
	
	if (McBank == MCA_BANK_QPI0) {
		QpiInfo.QpiPort = 0;
	}
	else if (McBank == MCA_BANK_QPI1) {
		QpiInfo.QpiPort = 1;
	}
	else {
		QpiInfo.QpiPort = 2;
	}
	QpiInfo.Socket = skt;
	QpiInfo.Valid = TRUE;
	QpiInfo.ErrorType = Source;
	MsrIa32MCiStatus.Data = MsrMCStatus;
	if (MsrIa32MCiStatus.Bits.uc) {
		QpiInfo.ErrorSeverity = FATAL_ERROR_TYPE;
	}
	else {
		QpiInfo.ErrorSeverity = COR_ERROR_TYPE;
	}
	QpiInfo.CorrectedErrorCount = MsrIa32MCiStatus.Bits.corrected_err_count;
	QpiInfo.ModelSpecificCode = MsrIa32MCiStatus.Bits.ms_code;
	QpiInfo.MachineCheckCode = MsrIa32MCiStatus.Bits.mca_code;
	if (MsrIa32MCiStatus.Bits.addrv) {
		QpiInfo.Address = (Addr & MCA_ADDR_BIT_MASK) >> 6;
	}
	CommonErrorHandling((VOID *)&QpiInfo);
}

VOID
CboErrorHandler ( 
  ERROR_SOURCE Source,
  UINT8	skt,
  UINT8 McBank,
  UINT64 MsrMCStatus,
  UINT64 Addr,
  UINT64 Misc)
{
	CBO_ERROR_RECORD			CboInfo;
	IA32_MCI_STATUS_MSR_STRUCT 	MsrIa32MCiStatus;

	ZeroMem (&CboInfo, sizeof (CBO_ERROR_RECORD));
	
	if (McBank == MCA_BANK_CBO0) {
		CboInfo.CboMcaBank = 0;
	}
	else if (McBank == MCA_BANK_CBO1) {
		CboInfo.CboMcaBank = 1;
	}
	else {
		CboInfo.CboMcaBank = 2;
	}
	CboInfo.Socket = skt;
	CboInfo.Valid = TRUE;
	CboInfo.ErrorType = Source;
	MsrIa32MCiStatus.Data = MsrMCStatus;
	if (MsrIa32MCiStatus.Bits.uc) {
		CboInfo.ErrorSeverity = FATAL_ERROR_TYPE;
	}
	else {
		CboInfo.ErrorSeverity = COR_ERROR_TYPE;
	}
	CboInfo.CorrectedErrorCount = MsrIa32MCiStatus.Bits.corrected_err_count;
	CboInfo.ModelSpecificCode = MsrIa32MCiStatus.Bits.ms_code;
	CboInfo.MachineCheckCode = MsrIa32MCiStatus.Bits.mca_code;
	if (MsrIa32MCiStatus.Bits.addrv) {
		CboInfo.Address = (Addr & MCA_ADDR_BIT_MASK) >> 6;
	}
	if (MsrIa32MCiStatus.Bits.miscv) {
		CboInfo.CoreId = (Misc & (BIT24 + BIT25 +BIT26 + BIT27 + BIT28)) >> 24;
		CboInfo.ThreadId = (Misc & BIT23) >> 23;
	}
	CommonErrorHandling((VOID *)&CboInfo);
}

VOID
HandleCoreAndUncoreErrors (
	UINT8 	McBank,
	UINT8	skt,
	EMCA_MC_SIGNATURE_ERR_RECORD *McErrSig)
{
	  IA32_MCI_STATUS_MSR_STRUCT MciStsMsr;

	  MciStsMsr.Data = McErrSig->Signature.McSts;

      if ((mLastbootMemErrFlag == TRUE) && (MciStsMsr.Bits.uc != 1))
      {
        return;
      }
	  // To Handle Core Errors
      if (McBank >= MCA_BANK_IFU && McBank <= MCA_BANK_MLC)  
      {
      	//
      	// Log the error
      	//
      	AcquireSpinLock (&mSmmErrorMpSyncData.LoggingLock);
      	CoreErrorHandler (CoreSource, skt, McBank, MciStsMsr.Data, McErrSig->Signature.McAddr, McErrSig->Signature.McMisc);
      	ReleaseSpinLock(&mSmmErrorMpSyncData.LoggingLock);
      }

      
	  // To Handle Qpi Errors
      if (McBank == MCA_BANK_QPI0 || McBank == MCA_BANK_QPI1 || McBank == MCA_BANK_QPI2)
      {
      	//
      	// Log the error
      	//
      	AcquireSpinLock (&mSmmErrorMpSyncData.LoggingLock);
      	QpiErrorHandler (QpiSource, skt, McBank, MciStsMsr.Data, McErrSig->Signature.McAddr, McErrSig->Signature.McMisc);
      	ReleaseSpinLock(&mSmmErrorMpSyncData.LoggingLock);
      }

	  // To Handle Cbo Errors
      if (McBank >= MCA_BANK_CBO0 && McBank <= MCA_BANK_CBO2)  
      {
	      //
	      // Log the error
	      //
	      AcquireSpinLock (&mSmmErrorMpSyncData.LoggingLock);
	      CboErrorHandler (CboSource, skt, McBank, MciStsMsr.Data, McErrSig->Signature.McAddr, McErrSig->Signature.McMisc);
	      ReleaseSpinLock(&mSmmErrorMpSyncData.LoggingLock);
      }
    
      // Aptio V Server Override Start: Changes done to support LastBoot Error Handling and Logging Uncorrectable Errors (Memory/PCIE ...)       
      if (MciStsMsr.Bits.uc != 1 /*|| MciStsMsr.Bits.en != 1*/)
      {
      	  return;
      }

      // To Handle Memory Errors
      if (McBank >= MCA_BANK_HA && McBank <= MCA_BANK_HA1_IMC3 )  
      {
          //
          // Log the error
          //
          AcquireSpinLock (&mSmmErrorMpSyncData.LoggingLock);
          LogUncorrectedError (MemSource, FATAL_ERROR_TYPE, MciStsMsr.Data, McErrSig->Signature.McAddr, McErrSig->Signature.McMisc);
          ReleaseSpinLock(&mSmmErrorMpSyncData.LoggingLock); 
      }

      // To Handle PCIE Errors
      if (McBank == MCA_BANK_IIO )  
      {
          //
          // Log the error
          //
          AcquireSpinLock (&mSmmErrorMpSyncData.LoggingLock);
            if (mLastbootMemErrFlag)   // To Represent the Last boot Error in SELVIEW (0x0F is passed as OEM ED1 Data)
      	      LogUncorrectedError (PciSource, LASTBOOT_PCIE_ERROR, MciStsMsr.Data, McErrSig->Signature.McAddr, McErrSig->Signature.McMisc);
            else
      	      LogUncorrectedError (PciSource, SERROR_TYPE, MciStsMsr.Data, McErrSig->Signature.McAddr, McErrSig->Signature.McMisc);
          ReleaseSpinLock(&mSmmErrorMpSyncData.LoggingLock);
      }

}

#pragma optimize( "", on )

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
