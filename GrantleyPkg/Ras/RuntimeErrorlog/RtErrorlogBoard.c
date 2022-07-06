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
//
// $Header: $
//
// $Revision: $
//
// $Date: $
//
//
//*****************************************************************************

//<AMI_FHDR_START>
//----------------------------------------------------------------------------
//
// Name:		RtErrorLogBoard.c
//
// Description:	
//
//----------------------------------------------------------------------------
//<AMI_FHDR_END>

#include "RtErrorlogBoard.h"


//EFI_SMM_GPI_DISPATCH_PROTOCOL   *mGpiDispatch;
//EFI_SMM_ICHN_DISPATCH2_PROTOCOL  *mIchnDispatch2;

//extern EFI_GUID gEfiSmmIchnDispatch2ProtocolGuid;

//EFI_SMM_BASE_PROTOCOL         	*mSmmBase;
//EFI_HANDLE  					mImageHandle;
UINT8  				ErrorSource;
UINT8                           mErrorSeverity;

//---------------------------------------------------------------------------
// External (eLink) handler functions
//---------------------------------------------------------------------------

typedef EFI_STATUS (MEM_ERROR_HANDLER_FUNCTION) (
		MEMORY_DEV_INFO		*MemInfo
);

extern MEM_ERROR_HANDLER_FUNCTION MEM_ERROR_HANDLER_FUNCTION_LIST EndOfHandlerList;

MEM_ERROR_HANDLER_FUNCTION* MemErrorHandlerList[] = {
		MEM_ERROR_HANDLER_FUNCTION_LIST
	NULL
};

typedef EFI_STATUS (CORE_ERROR_HANDLER_FUNCTION) (
		CORE_ERROR_RECORD         *CoreInfo
);

extern CORE_ERROR_HANDLER_FUNCTION CORE_ERROR_HANDLER_FUNCTION_LIST CoreEndOfHandlerList;

CORE_ERROR_HANDLER_FUNCTION* CoreErrorHandlerList[] = {
		CORE_ERROR_HANDLER_FUNCTION_LIST
	NULL
};

typedef EFI_STATUS (QPI_ERROR_HANDLER_FUNCTION) (
		QPI_ERROR_RECORD         *QpiInfo
);

extern QPI_ERROR_HANDLER_FUNCTION QPI_ERROR_HANDLER_FUNCTION_LIST QpiEndOfHandlerList;

QPI_ERROR_HANDLER_FUNCTION* QpiErrorHandlerList[] = {
		QPI_ERROR_HANDLER_FUNCTION_LIST
	NULL
};

typedef EFI_STATUS (CBO_ERROR_HANDLER_FUNCTION) (
		CBO_ERROR_RECORD         *QpiInfo
);

extern CBO_ERROR_HANDLER_FUNCTION CBO_ERROR_HANDLER_FUNCTION_LIST CboEndOfHandlerList;

CBO_ERROR_HANDLER_FUNCTION* CboErrorHandlerList[] = {
		CBO_ERROR_HANDLER_FUNCTION_LIST
	NULL
};

typedef EFI_STATUS (IOHQPI_ERROR_HANDLER_FUNCTION) (
    IN UINT16   ErrorType,
    IN UINT8    ErrorSubType,
    IN ERROR_SEVERITY ErrorSeverity,
    IN UINT8    Socket,
    IN UINT8    Port
);

extern IOHQPI_ERROR_HANDLER_FUNCTION IOHQPI_ERROR_HANDLER_FUNCTION_LIST IohQpiEndOfHandlerList;

IOHQPI_ERROR_HANDLER_FUNCTION* IohQpiErrorHandlerList[] = {
		IOHQPI_ERROR_HANDLER_FUNCTION_LIST
	NULL
};

typedef EFI_STATUS (IOHCORE_ERROR_HANDLER_FUNCTION) (
    IN UINT16   ErrorType,
    IN UINT8    ErrorSubType,
    IN ERROR_SEVERITY ErrorSeverity,
    IN UINT8    Socket    
);

extern IOHCORE_ERROR_HANDLER_FUNCTION IOHCORE_ERROR_HANDLER_FUNCTION_LIST IohCoreEndOfHandlerList;

IOHCORE_ERROR_HANDLER_FUNCTION* IohCoreErrorHandlerList[] = {
		IOHCORE_ERROR_HANDLER_FUNCTION_LIST
	NULL
};

typedef EFI_STATUS (IOHVTD_ERROR_HANDLER_FUNCTION) (
    IN UINT16   ErrorType,
    IN UINT8    ErrorSubType,
    IN ERROR_SEVERITY ErrorSeverity,
    IN UINT8    Socket
);

extern IOHVTD_ERROR_HANDLER_FUNCTION IOHVTD_ERROR_HANDLER_FUNCTION_LIST IohVtdEndOfHandlerList;

IOHVTD_ERROR_HANDLER_FUNCTION* IohVtdErrorHandlerList[] = {
		IOHVTD_ERROR_HANDLER_FUNCTION_LIST
	NULL
};

typedef EFI_STATUS (PCI_ERROR_HANDLER_FUNCTION) (
		IN	 UINT8       	ErrorSource,
		IN	 UINT8       	ErrorType,
		IN	 UINT8       	Segment,
		IN	 UINT8       	Bus,
		IN	 UINT8       	Device,
		IN    UINT8       	Function,
		IN    BOOLEAN   	IsBridge,
		IN    UINT16      	SecondaryStatus,
		IN    UINT8        	PcieCapOffset,
		IN    UINT16      	PcieDeviceStatus,
		IN    UINT16      	AerOffset,
		IN    UINT32      	UncorrErrStatus,
		IN    UINT32      	CorrErrStatus,
		IN    UINT32      	RootErrStatus
);

extern PCI_ERROR_HANDLER_FUNCTION PCI_ERROR_HANDLER_FUNCTION_LIST PciEndOfHandlerList;

PCI_ERROR_HANDLER_FUNCTION* PciErrorHandlerList[] = {
		PCI_ERROR_HANDLER_FUNCTION_LIST
    NULL
};

//<AMI_PHDR_START>
//-----------------------------------------------------------------------
//
//
// Procedure:   ErrorSeverityAction
//
// Description: 
//
// Parameters:  
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
ErrorSeverityAction (
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  
  // Changes done for the LASTBOOT ERROR Support
  // Don't generate NMI when Poison is enabled
  if (mPoisonEnabled == TRUE )
  {
	  return Status;
  }

  if ((ErrorSource == PciSource) || (ErrorSource == IohQpiSource) || (ErrorSource == IohCoreSource) || (ErrorSource == IohVtdSource)) {
      if (!mLastbootMemErrFlag) {
          if(mErrorSeverity == FATAL_ERROR_TYPE) {
			#if FATAL_ERROR_ACTION == GENERATE_NMI_ON_FE
  			// Generate NMI on fatal errors
	  	  	  	  ElogGenerateNmiNow ();
			#endif
          }
      }
  }
  return Status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   LogMemErrorRecoed
//
// Description: 
//
// Parameters:  
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
LogMemErrorRecord (
		MEMORY_DEV_INFO                  *MemInfo
  )
{

    // Call each handler function.
    {
       	UINTN i;
	    for(i=0; MemErrorHandlerList[i]; i++) {
            MemErrorHandlerList[i] (
              MemInfo
            );
        }
    }

  return EFI_SUCCESS;
}

EFI_STATUS
LogPciErrorRecord (
	PCI_PCIE_ERROR_LOCAL_STRUC   *PciErrorData
    )
{
	BOOLEAN 		IsBridge;
	UINT8   			PcieCapOffset;
	UINT16  		AerOffset = 0;
	UINT16  		PcieDeviceStatus = 0;
	UINT16  		SecondaryStatus = 0;
	UINT32  		UncorrErrStatus = 0;
	UINT32  		CorrErrStatus = 0;
	UINT32  		RootErrStatus = 0;
	UINT16 			NextCurrentCapPtr;
	UINT16 			CurrentCapPtr;
	UINT16 			CapId;
	UINT8  			PcieDeviceType;

	IsBridge = PciErrLibIsPciBridgeDevice(PciErrorData ->Bus, PciErrorData ->Device, PciErrorData ->Function);
	
	if (IsBridge) {
	        SecondaryStatus = PciExpressRead16 (PCI_PCIE_ADDR(PciErrorData ->Bus, PciErrorData ->Device, PciErrorData ->Function, PCI_BRIDGE_STATUS_REGISTER_OFFSET));
	}

	PcieCapOffset = PciErrLibGetCapabilitiesOffset(PciErrorData ->Bus, PciErrorData ->Device, PciErrorData ->Function);
	
	if (PcieCapOffset) {
		NextCurrentCapPtr = 0x100;
		do {
		      CurrentCapPtr = NextCurrentCapPtr;
		      CapId = PciExpressRead16 (PCI_PCIE_ADDR(PciErrorData ->Bus, PciErrorData ->Device, PciErrorData ->Function, CurrentCapPtr));
		      NextCurrentCapPtr = PciExpressRead16 (PCI_PCIE_ADDR(PciErrorData ->Bus, PciErrorData ->Device, PciErrorData ->Function, CurrentCapPtr + 2)) >> 4;
		      if(CapId == V_PCIE_EXT_CAP_HEADER_AERC) {
		          AerOffset = CurrentCapPtr;
		          break; 
		      }
		} while (NextCurrentCapPtr != 0 && CapId != 0xffff && NextCurrentCapPtr > CurrentCapPtr);
		
	        PcieDeviceStatus = PciExpressRead16 (PCI_PCIE_ADDR(PciErrorData ->Bus, PciErrorData ->Device, PciErrorData ->Function, PcieCapOffset + PCIE_DEVICE_STATUS_OFFSET));
	        PcieDeviceStatus &= ( B_PCIE_DEVICE_STATUS_COR_ERR | B_PCIE_DEVICE_STATUS_NF_ERR | B_PCIE_DEVICE_STATUS_FAT_ERR );
	        
	        if (AerOffset && PcieDeviceStatus) {
	            // Populate AER information.
	            UncorrErrStatus = PciExpressRead32 (PCI_PCIE_ADDR(PciErrorData ->Bus, PciErrorData ->Device, PciErrorData ->Function, AerOffset + PCIE_AER_UNCERRSTS_OFFSET));
	            UncorrErrStatus &= ~( PciExpressRead32 (PCI_PCIE_ADDR(PciErrorData ->Bus, PciErrorData ->Device, PciErrorData ->Function, AerOffset + PCIE_AER_UNCERRMSK_OFFSET)));

	            CorrErrStatus = PciExpressRead32 (PCI_PCIE_ADDR(PciErrorData ->Bus, PciErrorData ->Device, PciErrorData ->Function, AerOffset + PCIE_AER_CORERRSTS_OFFSET));
	            CorrErrStatus &= ~( PciExpressRead32 (PCI_PCIE_ADDR(PciErrorData ->Bus, PciErrorData ->Device, PciErrorData ->Function, AerOffset + PCIE_AER_CORERRMSK_OFFSET)));

	            PcieDeviceType = PciExpressRead8(PCI_PCIE_ADDR(PciErrorData ->Bus, PciErrorData ->Device, PciErrorData ->Function, PcieCapOffset + PCIE_CAPABILITY_OFFSET));
	            PcieDeviceType = (PcieDeviceType >> 4) & 0x0F;
	            // Check device type.
	            switch( PcieDeviceType )
	            {
	                case PCIE_TYPE_ROOTPORT:  // Root Port of PCI Express Root Complex
	                case 9:  // Root Complex Integrated Endpoint
	                case 10: // Root Complex Event Collector
	                    RootErrStatus = PciExpressRead32 (PCI_PCIE_ADDR(PciErrorData ->Bus, PciErrorData ->Device, PciErrorData ->Function, AerOffset + PCIE_AER_RPERRSTS_OFFSET));
	                    break;
	                default: // PCIe endpoint
	                    RootErrStatus = 0;
	                    break;
	            }
	        }
	}

	if ((SERROR_TYPE == PciErrorData ->ErrorType) || (PERROR_TYPE == PciErrorData ->ErrorType) || (DPERROR_TYPE == PciErrorData ->ErrorType) ) {
	        mErrorSeverity = Fatal;
	}

       // Call each handler function.
        {//local loop
            UINTN i;
            
            for(i=0; PciErrorHandlerList[i]; i++) {
                PciErrorHandlerList[i] (
                    PciErrorData ->ErrorSource,
                    PciErrorData ->ErrorType,
                    PciErrorData ->Segment,
                    PciErrorData ->Bus,
                    PciErrorData ->Device,
                    PciErrorData ->Function,
                    IsBridge,
                    SecondaryStatus,
                    PcieCapOffset,
                    PcieDeviceStatus,
                    AerOffset,
                    UncorrErrStatus,
                    CorrErrStatus,
                    RootErrStatus
                 );
            }// for
        }//local loop

    return EFI_SUCCESS;

}

EFI_STATUS
LogIohQpiErrorRecord (
        IOH_QPI_COMBINED_ERROR_RECORD *IohQpiErrorData
        ){

        // Call each handler function.
        {//local loop

            UINTN i;
            
            for(i=0; IohQpiErrorHandlerList[i]; i++) {
                IohQpiErrorHandlerList[i] (
                    IohQpiErrorData ->IohQpiErrorRecord.Socket,
                    IohQpiErrorData ->IohQpiErrorRecord.Port,
                    IohQpiErrorData ->IohQpiErrorRecord.Header.ErrorType,
                    IohQpiErrorData ->IohQpiErrorRecord.ErrorSubType,
                    IohQpiErrorData ->IohQpiErrorRecord.Header.ErrorSeverity
                );
            }// for
        }//local loop

    return EFI_SUCCESS;

}

EFI_STATUS
LogIohCoreErrorRecord (
        IOH_CORE_COMBINED_ERROR_RECORD *IohCoreErrorData
        ){

        // Call each handler function.
        {//local loop
            UINTN i;
            
            for(i=0; IohCoreErrorHandlerList[i]; i++) {
                IohCoreErrorHandlerList[i] (
                    IohCoreErrorData ->IohCoreErrorRecord.Socket,
                    IohCoreErrorData ->IohCoreErrorRecord.Header.ErrorType,
                    IohCoreErrorData ->IohCoreErrorRecord.ErrorSubType,
                    IohCoreErrorData ->IohCoreErrorRecord.Header.ErrorSeverity
                );
            }// for
        }//local loop

    return EFI_SUCCESS;
}

EFI_STATUS
LogIohVtdErrorRecord (
        IOH_VTD_COMBINED_ERROR_RECORD *IohVtdErrorData
        ){

        // Call each handler function.
        {//local loop
            UINTN i;
            
            for(i=0; IohVtdErrorHandlerList[i]; i++) {
                IohVtdErrorHandlerList[i] (
                    IohVtdErrorData ->IohVtdErrorRecord.Socket,
                    IohVtdErrorData ->IohVtdErrorRecord.Header.ErrorType,
                    IohVtdErrorData ->IohVtdErrorRecord.ErrorSubType,
                    IohVtdErrorData ->IohVtdErrorRecord.Header.ErrorSeverity
                );
            }// for
        }//local loop

    return EFI_SUCCESS;
}

EFI_STATUS
LogCoreErrorRecord (
	CORE_ERROR_RECORD                  *CoreErrorData
)
{
	UINTN i;
	
	// Call each handler function.
	for(i=0; CoreErrorHandlerList[i]; i++) {
		CoreErrorHandlerList[i] (
			CoreErrorData
		        );
	}
	return EFI_SUCCESS;
}

EFI_STATUS
LogQpiErrorRecord (
	QPI_ERROR_RECORD                  *QpiErrorData
)
{
	UINTN i;
	
	// Call each handler function.
	for(i=0; QpiErrorHandlerList[i]; i++) {
		QpiErrorHandlerList[i] (
			QpiErrorData
		        );
	}
	return EFI_SUCCESS;
}

EFI_STATUS
LogCboErrorRecord (
	CBO_ERROR_RECORD                  *CboErrorData
)
{
	UINTN i;
	
	// Call each handler function.
	for(i=0; CboErrorHandlerList[i]; i++) {
		CboErrorHandlerList[i] (
			CboErrorData
		        );
	}
	return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   CommonErrorHandling
//
// Description: 
//
// Parameters:  
//
// Returns:     
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
CommonErrorHandling (
  VOID  *ErrorDataStructure
  )
{

  EFI_STATUS  	Status = EFI_SUCCESS;

  ErrorSource = *(UINT8 *)ErrorDataStructure;
  mErrorSeverity = *((UINT8 *)ErrorDataStructure + 1);
  
  DEBUG((-1, "In CommonErrorHandling()\n"));

  switch (ErrorSource)  {
  case MemSource:
    LogMemErrorRecord ((MEMORY_DEV_INFO *)ErrorDataStructure);
    break;

  case PciSource:
    LogPciErrorRecord ((PCI_PCIE_ERROR_LOCAL_STRUC *)ErrorDataStructure);
    break;

  case IohQpiSource:
    LogIohQpiErrorRecord ((IOH_QPI_COMBINED_ERROR_RECORD *)ErrorDataStructure);
    break;

  case IohCoreSource:
    LogIohCoreErrorRecord ((IOH_CORE_COMBINED_ERROR_RECORD *)ErrorDataStructure);
    break;

  case IohVtdSource:
    LogIohVtdErrorRecord ((IOH_VTD_COMBINED_ERROR_RECORD *)ErrorDataStructure);
    break;

  case CoreSource:
    LogCoreErrorRecord  ((CORE_ERROR_RECORD *)ErrorDataStructure);
    break;

  case QpiSource:
    LogQpiErrorRecord  ((QPI_ERROR_RECORD *)ErrorDataStructure);
    break;
    
  case CboSource:
    LogCboErrorRecord  ((CBO_ERROR_RECORD *)ErrorDataStructure);
    break;

  default:
    break;

  }
  return Status;
}

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
