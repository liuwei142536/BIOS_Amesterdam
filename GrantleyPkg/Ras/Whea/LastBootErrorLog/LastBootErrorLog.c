#include "LastBootErrorlog.h"
#include <Guid/LastBootErrorlogHob.h>

EFI_WHEA_BOOT_PROTOCOL  *mWheaBootProtocol = NULL;
EFI_MEM_RAS_PROTOCOL	  *mMemRas = NULL;
EFI_SMBIOS_PROTOCOL		  *mSmbiosProtocol = NULL;
IIO_UDS                 *mIioUds = NULL;
EFI_CPU_CSR_ACCESS_PROTOCOL *mCpuCsrAccess;
	PREV_BOOT_ERR_SRC_HOB	  *mPrevBootErrSrcHob = NULL;
LASTBOOT_SMBIOS_HANDLE		mLastBootSmbios; // AptioV Server Override: Added to log Error details for Last Boot Whea Memory Errors.

// IOH_DATA structure is used in runtime when IOH UDS protocol is not available.
IOH_INFO  IohInfo[MAX_SOCKET] = {{(UINT8) -1, (UINT8) -1, (UINT8) -1, (UINT8) -1 },
                               {(UINT8) -1, (UINT8) -1, (UINT8) -1, (UINT8) -1  },
                               {(UINT8) -1, (UINT8) -1, (UINT8) -1, (UINT8) -1  }};

PREV_BOOT_ERR_GLOBAL_DATA mPrevBootErrData;

EFI_STATUS
UpdateCboError(
	MCBANK_ERR_INFO			*McBankErrInfo
	)
/*
Routine Description: 

  Checks and updates the previous boot fatal CBO errors. 

Arguments:  
	I/P : McBankErrInfo contains the Prev boot fatal QPI/PCU error type.
	
Returns:	
	Status  : Success or  failure

*/
{
	EFI_STATUS  Status = EFI_SUCCESS;
	CPU_DEV_INFO    CpuInfo; 
	EFI_CPUID_REGISTER Register;
  
	AsmCpuid (EFI_CPUID_VERSION_INFO, &Register.RegEax, &Register.RegEbx, &Register.RegEcx, &Register.RegEdx);
	// Need to port other parameters of CPU_DEV_INFO if required
	CpuInfo.ProcessorType = GEN_ERR_PROC_TYPE_IA32_X64;
	CpuInfo.ProcessorISA = GEN_ERR_PROC_ISA_X64;
	CpuInfo.ProcessorErrorType = UEFI_PROC_CACHE_ERR_TYPE;	// Cache error.
	CpuInfo.Operation = 0;		// Generic Error
	CpuInfo.ProcessorApicId = McBankErrInfo->ApicId;		// Processor ID // AptioV Server Override: Filled the ProcessorApicId Information
	CpuInfo.VersionInfo	= Register.RegEax;
  // if Mcbanks status is cleared and whea is enabled . Report the error using whea.
	if((mPrevBootErrData.SetupMcBankStsClear == 1) && (mWheaBootProtocol != NULL))
		Status = mWheaBootProtocol->WheaBootElogProcessor(mWheaBootProtocol, GEN_ERR_SEV_CPU_GENERIC_FATAL, &CpuInfo); 

	return Status;
}

EFI_STATUS
UpdateQpiPcuError(
	MCBANK_ERR_INFO			*McBankErrInfo
	)
/*
Routine Description: 

  Checks and updates the previous boot fatal QPI/PCU errors. 

Arguments:  
	I/P : McBankErrInfo contains the Prev boot fatal QPI/PCU error type.
	
Returns:	
	Status  : Success or  failure

*/
{
	EFI_STATUS  Status = EFI_SUCCESS;
	CPU_DEV_INFO    CpuInfo; 
	EFI_CPUID_REGISTER Register;

	AsmCpuid (EFI_CPUID_VERSION_INFO, &Register.RegEax, &Register.RegEbx, &Register.RegEcx, &Register.RegEdx);
	// Need to port other parameters of CPU_DEV_INFO if required
	CpuInfo.ProcessorType = GEN_ERR_PROC_TYPE_IA32_X64;
	CpuInfo.ProcessorISA = GEN_ERR_PROC_ISA_X64;
	CpuInfo.ProcessorErrorType = 0;	// Unknown error.
	CpuInfo.Operation = 0;		// Generic Error
	CpuInfo.ProcessorApicId = McBankErrInfo->ApicId;		// Processor ID // AptioV Server Override: Filled the ProcessorApicId Information
	CpuInfo.VersionInfo	= Register.RegEax;
  // if Mcbanks status is cleared and whea is enabled . Report the error using whea.
	if((mPrevBootErrData.SetupMcBankStsClear == 1) && (mWheaBootProtocol != NULL))
		Status = mWheaBootProtocol->WheaBootElogProcessor(mWheaBootProtocol, GEN_ERR_SEV_CPU_GENERIC_FATAL, &CpuInfo); 

	return Status;
}

EFI_STATUS
UpdateCoreError(
	MCBANK_ERR_INFO			*McBankErrInfo
	)
/*
Routine Description: 

  Checks and updates the previous boot fatal Core errors. 

Arguments:  
	I/P : McBankErrInfo contains the Prev boot fatal Core error type.
	
Returns:	
	Status  : Success or  failure

*/
{
	EFI_STATUS  Status = EFI_SUCCESS;
	CPU_DEV_INFO    CpuInfo; 
	EFI_CPUID_REGISTER Register;
  
	AsmCpuid (EFI_CPUID_VERSION_INFO, &Register.RegEax, &Register.RegEbx, &Register.RegEcx, &Register.RegEdx);
	// Need to port other parameters of CPU_DEV_INFO if required
	CpuInfo.ProcessorType = GEN_ERR_PROC_TYPE_IA32_X64;
	CpuInfo.ProcessorISA = GEN_ERR_PROC_ISA_X64;
	CpuInfo.ProcessorErrorType = UEFI_PROC_MICRO_ARCH_ERR_TYPE;	// MicroArchitecture error.
	CpuInfo.Operation = 0;		// Generic Error
	CpuInfo.ProcessorApicId = McBankErrInfo->ApicId;		// Processor ID
	CpuInfo.VersionInfo	= Register.RegEax;
  // if Mcbanks status is cleared and whea is enabled . Report the error using whea.
	if((mPrevBootErrData.SetupMcBankStsClear == 1) && (mWheaBootProtocol != NULL))
		Status = mWheaBootProtocol->WheaBootElogProcessor(mWheaBootProtocol, GEN_ERR_SEV_CPU_GENERIC_FATAL, &CpuInfo); 

	return Status;
}

EFI_STATUS
UpdateMemoryError(
	MCBANK_ERR_INFO			*McBankErrInfo
	)
/*
Routine Description: 

  Checks and updates the previous boot fatal memory errors. 

Arguments:  
	I/P : McBankErrInfo contains the Prev boot fatal memory error type.
	
Returns:	
	Status  : Success or  failure

*/
{

	EFI_STATUS  Status = EFI_SUCCESS;
	MEMORY_DEV_INFO     MemInfo;
	UINT8				SocketId = 00;
	UINT8				MemoryControllerId = 00;
	UINT8				ChannelId = 00;
	UINT8				DimmSlot = 00;
	UINT8				DimmRank = 00;
	UINT32				Row = 00;
	UINT32				Col = 00;
	UINT8				Bank = 00;
  UINT8       BankGroup = 00;
  UINT8       ChipSelect = 00;
	UINT64			McBankAddValue;
    IA32_MCI_STATUS_MSR_STRUCT MsrIa32MCiStatusReg; // AptioV Server Override: Added to log Error details for Last Boot Whea Memory Errors.

// AptioV Server Override Start: To fill Error Details for Memory Boot Errors.
  SetMem (&MemInfo, sizeof (MEMORY_DEV_INFO), 0xFF);

  MsrIa32MCiStatusReg.Data = McBankErrInfo->McBankStatus;
  if ( (MsrIa32MCiStatusReg.Bits.addrv) && (MsrIa32MCiStatusReg.Bits.miscv) ) {
	MemInfo.Node = McBankErrInfo->Socket;
	MemInfo.ValidBits = 0; //HSD 4032453: Klocwork ID 699
// AptioV Server Override Start: Added to log Error details for Last Boot Whea Memory Errors.
//	MemInfo.ValidBits |= PLATFORM_MEM_NODE_VALID;
	McBankAddValue = McBankErrInfo->McbankAddr;
	MsrIa32MCiStatusReg.Data = McBankErrInfo->McBankStatus;
// AptioV Server Override Start: Fix for Last Boot Errors not getting logged if the MCbank Address comes as zero.
//	if( (McBankAddValue != 0) && (mMemRas != NULL) ) {
	if(mMemRas != NULL ) {
// AptioV Server Override End: Fix for Last Boot Errors not getting logged if the MCbank Address comes as zero.		
		mMemRas->SystemAddressToDimmAddress (McBankAddValue,&SocketId,&MemoryControllerId,&ChannelId,&DimmSlot,&DimmRank,&Row,&Col,&Bank,&BankGroup, &ChipSelect);
		MemInfo.ValidBits |= PLATFORM_MEM_PHY_ADDRESS_VALID | PLATFORM_MEM_PHY_ADDRESS_MASK_VALID; 
        MemInfo.Node      = SKTMC_TO_NODE(SocketId, MemoryControllerId);
        MemInfo.Channel   = ChannelId;
	    MemInfo.PhyAddr   = McBankAddValue;
	    MemInfo.PhyAddrMask = 0x3fffffffffc0;
		MemInfo.Dimm      = DimmSlot;
		MemInfo.Column    = Col;
		MemInfo.Bank      = Bank;
		MemInfo.Row       = Row;
        MemInfo.ValidBits |=  PLATFORM_MEM_NODE_VALID | PLATFORM_MEM_CARD_VALID | PLATFORM_MEM_MODULE_VALID | PLATFORM_MEM_ROW_VALID | PLATFORM_MEM_COLUMN_VALID | PLATFORM_MEM_BANK_VALID;
        if ((MsrIa32MCiStatusReg.Bits.val == 1) && (MsrIa32MCiStatusReg.Bits.ms_code == 0x0001)) {
            MemInfo.ErrorType = PLATFORM_MEM_ERROR_MLTIBIT_ECC;
            MemInfo.ValidBits |= PLATFORM_MEM_ERROR_TYPE_VALID;
        }
		if( mPrevBootErrData.WheaErrorRecordFormat == GENERIC_ERROR_SECTION_REVISION_UEFI231)	{
			MemInfo.Rank      = DimmRank;
			MemInfo.ValidBits |= PLATFORM_RANK_NUMBER_VALID;
			if(mSmbiosProtocol != NULL)	{
				MemInfo.ValidBits |= PLATFORM_CARD_HANDLE_VALID | PLATFORM_Module_HANDLE_VALID;
				MemInfo.SmBiosCardHandle	= mLastBootSmbios.LastBootType16Hanlde;	// Type 16 handle;
				MemInfo.SmBiosModuleHandle	= mLastBootSmbios.LastBootType17Handle[(SKTMC_TO_NODE(SocketId, MemoryControllerId))][ChannelId][DimmSlot];	// Type 17 handle
			}
		}
	}
  }
// AptioV Server Override End: Added to log Error details for Last Boot Whea Memory Errors.
  // if Mcbanks status is cleared and whea is enabled . Report the error using whea.
	if((mPrevBootErrData.SetupMcBankStsClear == 1) && (mWheaBootProtocol != NULL))
		Status = mWheaBootProtocol->WheaBootElogMemory(mWheaBootProtocol, GEN_ERR_SEV_PLATFORM_MEMORY_FATAL, &MemInfo);

	return Status;
}

EFI_STATUS
UpdatePciExError(
  PCI_EX_ERR_INFO   *PcieDev
	)
/*
Routine Description: 

  Checks and updates the previous boot fatal pci-ex errors. 
  This function is currently supported for Root ports only.

Arguments:  
	I/P : McBankErrInfo contains the Prev boot fatal memory error type.
	
Returns:	
	Status  : Success or  failure

*/
{
  EFI_STATUS        Status = EFI_SUCCESS;
  PCIE_PCI_DEV_INFO ErrPcieDev;
  UINT16            Data8 = 0;
  
  ErrPcieDev.Segment  = PcieDev->Segment;
  ErrPcieDev.Bus      = PcieDev->Bus;
  ErrPcieDev.Device   = PcieDev->Device;  
  ErrPcieDev.Function = PcieDev->Function;

  Data8 = PciExpressRead8( PCI_ADDRESS(ErrPcieDev.Segment,ErrPcieDev.Bus,ErrPcieDev.Device,ErrPcieDev.Function,PCI_HEADER_TYPE_OFFSET));
  // Check for type 0 header.
  if( (Data8 & 0x03f) == 0)   {
 	  if( mWheaBootProtocol != NULL )  {
      Status = mWheaBootProtocol->WheaBootElogPcieRootDevBridge(mWheaBootProtocol, GEN_ERR_SEV_PCIE_FATAL, &ErrPcieDev);       
    }
  }
  return  Status;

}

VOID
CheckAndUpdatePciExError()
{
  UINT8 RootBridgeLoop;
  UINT8 Device = 0;
  UINT8 Function = 0;
  UINT8 Port = 0;
  UINT16 Data16 = 0;
  UINT32 Data32 = 0;
  PCIE_PCI_DEV_INFO ErrPcieDev;
  EFI_STATUS  Status = EFI_SUCCESS;
// Aptio V Server Override Start : Changes done to log the PCIE Errors in BERT Table  
  GFERRST_IIO_RAS_STRUCT iioGFERRSTReg;
  GNERRST_IIO_RAS_STRUCT iioGNERRSTReg;

// Aptio V Server Override End : Changes done to log the PCIE Errors in BERT Table
  if(mIioUds == NULL) return;

  for ( RootBridgeLoop =0; RootBridgeLoop < MAX_SOCKET; RootBridgeLoop++) {

    if (mIioUds->PlatformData.IIO_resource[RootBridgeLoop].Valid == 1) {
// Aptio V Server Override Start : Changes done to log the PCIE Errors in BERT Table	
	    /*
      IohInfo[RootBridgeLoop].Valid = 1;
      IohInfo[RootBridgeLoop].SocketId = mIioUds->PlatformData.IIO_resource[RootBridgeLoop].SocketID;
      IohInfo[RootBridgeLoop].BusNum  = mIioUds->PlatformData.IIO_resource[RootBridgeLoop].BusBase;
      IohInfo[RootBridgeLoop].Core20BusNum = mIioUds->PlatformData.IIO_resource[RootBridgeLoop].BusBase;
      IohInfo[RootBridgeLoop].Port[0].Device = PCIE_PORT_0_DEV;
      IohInfo[RootBridgeLoop].Port[0].Function = PCIE_PORT_0_FUNC;
      IohInfo[RootBridgeLoop].Port[1].Device = PCIE_PORT_1A_DEV;
      IohInfo[RootBridgeLoop].Port[1].Function = PCIE_PORT_1A_FUNC;
      IohInfo[RootBridgeLoop].Port[2].Device = PCIE_PORT_1B_DEV;
      IohInfo[RootBridgeLoop].Port[2].Function = PCIE_PORT_1B_FUNC;
      IohInfo[RootBridgeLoop].Port[3].Device = PCIE_PORT_2A_DEV;
      IohInfo[RootBridgeLoop].Port[3].Function = PCIE_PORT_2A_FUNC;
      IohInfo[RootBridgeLoop].Port[4].Device = PCIE_PORT_2B_DEV;
      IohInfo[RootBridgeLoop].Port[4].Function = PCIE_PORT_2B_FUNC;
      IohInfo[RootBridgeLoop].Port[5].Device = PCIE_PORT_2C_DEV;
      IohInfo[RootBridgeLoop].Port[5].Function = PCIE_PORT_2C_FUNC;
      IohInfo[RootBridgeLoop].Port[6].Device = PCIE_PORT_2D_DEV;
      IohInfo[RootBridgeLoop].Port[6].Function = PCIE_PORT_2D_FUNC;
      IohInfo[RootBridgeLoop].Port[7].Device = PCIE_PORT_3A_DEV;
      IohInfo[RootBridgeLoop].Port[7].Function = PCIE_PORT_3A_FUNC;
      IohInfo[RootBridgeLoop].Port[8].Device = PCIE_PORT_3B_DEV;
      IohInfo[RootBridgeLoop].Port[8].Function = PCIE_PORT_3B_FUNC;
      IohInfo[RootBridgeLoop].Port[9].Device = PCIE_PORT_3C_DEV;
      IohInfo[RootBridgeLoop].Port[9].Function = PCIE_PORT_3C_FUNC;
      IohInfo[RootBridgeLoop].Port[10].Device = PCIE_PORT_3D_DEV;
      IohInfo[RootBridgeLoop].Port[10].Function = PCIE_PORT_3D_FUNC;
*/
      IohInfo[RootBridgeLoop].BusNum  = mIioUds->PlatformData.IIO_resource[RootBridgeLoop].BusBase;	    
      iioGFERRSTReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(IohInfo[RootBridgeLoop].BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, ONLY_REGISTER_OFFSET(GFERRST_IIO_RAS_REG)));
      iioGNERRSTReg.Data = PciExpressRead32 (PCI_PCIE_ADDR(IohInfo[RootBridgeLoop].BusNum, CORE05_DEV_NUM, CORE05_FUNC2_NUM, ONLY_REGISTER_OFFSET(GNERRST_IIO_RAS_REG)));
      
	  if ( (iioGFERRSTReg.Data ==0) && (iioGNERRSTReg.Data==0) ) {
	     continue;
	  }
	  
      if ((iioGFERRSTReg.Bits.dmi)  || (iioGNERRSTReg.Bits.dmi)) // DMI Port
      	{
	      	Device = 0 ;
	      	Function = 0 ;
      	}
      	
      	if ((iioGFERRSTReg.Bits.pcie0)  || (iioGNERRSTReg.Bits.pcie0))
      	{
      		Device = 0 ;    // PORT 0
      		Function = 0 ;
      	}
      		
      	if ((iioGFERRSTReg.Bits.pcie1)  || (iioGNERRSTReg.Bits.pcie1))
      	{
      		Device = 1 ;   // PORT 1A
      		Function = 0 ;
      	}
      			
      	if ((iioGFERRSTReg.Bits.pcie2)  || (iioGNERRSTReg.Bits.pcie2))
      	{
      		Device = 1 ;    // PORT 1B
      		Function = 1 ;
      	}

      	if ((iioGFERRSTReg.Bits.pcie3)  || (iioGNERRSTReg.Bits.pcie3))
      	{
      		Device = 2 ;	// PORT 2A
      		Function = 0 ;
      	}
      			
      	if ((iioGFERRSTReg.Bits.pcie4)  || (iioGNERRSTReg.Bits.pcie4))
      	{
      		Device = 2 ;	// PORT 2B
      		Function = 1 ;
      	}
      				
      	if ((iioGFERRSTReg.Bits.pcie5)  || (iioGNERRSTReg.Bits.pcie5))
      	{
      		Device = 2 ;	// PORT 2C
      		Function = 2 ;
      	}

      	if ((iioGFERRSTReg.Bits.pcie6)  || (iioGNERRSTReg.Bits.pcie6))
      	{
      		Device = 2 ;	// PORT 2D
      		Function = 3 ;
      	}
      						
      	if ((iioGFERRSTReg.Bits.pcie7)  || (iioGNERRSTReg.Bits.pcie7))
      	{
      		Device = 3 ;	// PORT 3A
      		Function = 0 ;
      	}
      							
      	if ((iioGFERRSTReg.Bits.pcie8)  || (iioGNERRSTReg.Bits.pcie8))
      	{
      		Device = 3 ;	// PORT 3B
      		Function = 1 ;
      	}
      								
      	if ((iioGFERRSTReg.Bits.pcie9)  || (iioGNERRSTReg.Bits.pcie9))
      	{
      		Device = 3 ;	// PORT 3C
      		Function = 2 ;
      	}
      		
      	if ((iioGFERRSTReg.Bits.pcie10)  || (iioGNERRSTReg.Bits.pcie10))
      	{
      		Device = 3 ;	// PORT 3D
      		Function = 3 ;
      	}

      	 //if( (mPrevBootErrData.SetupMcBankStsClear == 1) && (mWheaBootProtocol != NULL) )  {
      	          if(mWheaBootProtocol != NULL )  {
      	            ErrPcieDev.Segment  = 00;
      	            ErrPcieDev.Bus      = IohInfo[RootBridgeLoop].BusNum;
      	            ErrPcieDev.Device   = Device;  
      	            ErrPcieDev.Function = Function;
      	            Status = mWheaBootProtocol->WheaBootElogPcieRootDevBridge(mWheaBootProtocol, GEN_ERR_SEV_PCIE_FATAL, &ErrPcieDev);       
      	          }
      
    /*  
      for(Port = 0; Port < NUMBER_PORTS_PER_SOCKET; Port++) {

        Device = IohInfo[RootBridgeLoop].Port[Port].Device;
        Function = IohInfo[RootBridgeLoop].Port[Port].Function;
        if((IohInfo[RootBridgeLoop].BusNum == 0) && (Device == 0) && (Function == 0)) continue;
        //
        // Check if device is present
        //
        Data16 = PciExpressRead16 (PCI_PCIE_ADDR(IohInfo[RootBridgeLoop].BusNum, Device, Function, PCI_VENDOR_ID_OFFSET));
        if (Data16 == 0xFFFF) continue;

      
//        Data32 = PciExpressRead32 (PCI_PCIE_ADDR(IohInfo[RootBridgeLoop].BusNum, Device, Function, ONLY_REGISTER_OFFSET(UNCERRSTS_IIO_PCIE_REG)));
//        if(Data32 & 0x3fffff)  {  // Bits[22:31] reserved
          //if( (mPrevBootErrData.SetupMcBankStsClear == 1) && (mWheaBootProtocol != NULL) )  {
          if(mWheaBootProtocol != NULL )  {
            ErrPcieDev.Segment  = 00;
            ErrPcieDev.Bus      = IohInfo[RootBridgeLoop].BusNum;
            ErrPcieDev.Device   = Device;  
            ErrPcieDev.Function = Function;
            Status = mWheaBootProtocol->WheaBootElogPcieRootDevBridge(mWheaBootProtocol, GEN_ERR_SEV_PCIE_FATAL, &ErrPcieDev);       
          }
        }
      }*/
// Aptio V Server Override End : Changes done to log the PCIE Errors in BERT Table	  
    }
  }

  // PCH root ports
  Device  = (UINT8)PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS;
  for(Function = 0; Function <= PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_8; Function++) {
      //
      // Check if device is present
      //
      Data16 = PciExpressRead16 (PCI_PCIE_ADDR(IohInfo[0].BusNum, Device, Function, PCI_VENDOR_ID_OFFSET));
      if (Data16 == 0xFFFF) continue;

      Data32 = PciExpressRead32 (PCI_PCIE_ADDR(IohInfo[0].BusNum, Device, Function, PCH_PCIE_RROT_PORT_AER_UES));
      if(Data32 & 0x3fffff)  {  // Bits[22:31] reserved
        if( (mPrevBootErrData.SetupMcBankStsClear == 1) && (mWheaBootProtocol != NULL) )  {
          ErrPcieDev.Segment  = 00;
          ErrPcieDev.Bus      = IohInfo[0].BusNum;
          ErrPcieDev.Device   = Device;  
          ErrPcieDev.Function = Function;
          Status = mWheaBootProtocol->WheaBootElogPcieRootDevBridge(mWheaBootProtocol, GEN_ERR_SEV_PCIE_FATAL, &ErrPcieDev);       
        }
      }
    }
}

EFI_STATUS
UpdateMemoryErrorFromMc5Shadow(
	//UINT64      PhyAddr,
	UINT8				SocketId
	)
/*
Routine Description: 

  Checks and updates the previous boot fatal memory errors This is added for WHQL testing since the Previous boot error record hob is not created in MRC. 

Arguments:  
	I/P : Phyisical adrress and socket number.

	
Returns:	
	Status  : Success or  failure

*/
{

	EFI_STATUS  Status = EFI_SUCCESS;
	MEMORY_DEV_INFO     MemInfo;
	//UINT8				MemoryControllerId = 00;
	//UINT8				ChannelId = 00;
	//UINT8				DimmSlot = 00;
	//UINT8				DimmRank = 00;
	//UINT32			Row = 00;
	//UINT32			Col = 00;
	//UINT8				Bank = 00;
	//UINT64			McBankAddValue;

	MemInfo.Node = SocketId;
	MemInfo.ValidBits = PLATFORM_MEM_NODE_VALID;
/*
	if( (PhyAddr != 0) && (mMemRas != NULL) ) {
    mMemRas->SystemAddressToDimmAddress (McBankAddValue,&SocketId,&MemoryControllerId,&ChannelId,&DimmSlot,&DimmRank,&Row,&Col,&Bank,&BankGroup, &ChipSelect);
		MemInfo.ValidBits |= PLATFORM_MEM_PHY_ADDRESS_VALID;  
		MemInfo.Dimm      = DimmSlot;
		MemInfo.Column    = Col;
		MemInfo.Bank      = Bank;
		MemInfo.Row       = Row;
    MemInfo.ValidBits |=  PLATFORM_MEM_CARD_VALID | PLATFORM_MEM_MODULE_VALID | PLATFORM_MEM_ROW_VALID | PLATFORM_MEM_COLUMN_VALID;
	}
*/
  // if Mcbanks status is cleared and whea is enabled . Report the error using whea.
	if( mWheaBootProtocol != NULL )
		Status = mWheaBootProtocol->WheaBootElogMemory(mWheaBootProtocol, GEN_ERR_SEV_PLATFORM_MEMORY_FATAL, &MemInfo);

	return Status;
}


EFI_STATUS
ChecAndUpdatePrevBootErrors(
  VOID
  )
/*
Routine Description: 

  Checks and updates the previous boot errors. This function is currentlyimplemented for McBank errors.

Arguments:  
	  mPrevBootErrSrcHob contains the Prev boot fatal error sources.
	
Returns:	
	Status  : Success or  failure

*/
{
	EFI_STATUS				Status = EFI_SUCCESS;
  UINT8             Skt = 0;
  UINT8             node = 0;
	MCBANK_ERR_INFO		*McBankErrInfo;
  PCI_EX_ERR_INFO   *PcieDev;
	IA32_MCI_STATUS_MSR_STRUCT MsrIa32MCiStatusReg;	
	UINT16					Length;
  MC5_STATUS_SHADOW_N1_HA_PMON_HSX_BDX_STRUCT Mc5ShadowStatus;

	if( mPrevBootErrSrcHob == NULL)  {

    // For WHQl testing - Previous boot error collection hob is not yest inplemented, So directly reading from the sticky Shadow regisetrs to get the error information for error type 32 of EINJ table.
    if(mIioUds == NULL) return EFI_NOT_FOUND;

    for (node = 0; node < MC_MAX_NODE; node++) {

      if ( mMemRas->SystemMemInfo->NodeInfo[node].Enabled == 00 ) continue;
                  
		  Mc5ShadowStatus.Data = mMemRas->MemReadPciCfg(  NODE_TO_SKT(node), NODE_TO_MC(node),MC5_STATUS_SHADOW_N1_HA_PMON_REG);

		  if( Mc5ShadowStatus.Bits.valid && Mc5ShadowStatus.Bits.uc && Mc5ShadowStatus.Bits.pcc) {
        UpdateMemoryErrorFromMc5Shadow( Skt);
      }
    }
    return EFI_SUCCESS;
  }
	//No error record come out of the function.
	if(mPrevBootErrSrcHob->Length <= 2)	return EFI_SUCCESS;

	Length = 2;  // Offset of the Error source from HOB.

	if( Length <= mPrevBootErrSrcHob->Length)	{

		McBankErrInfo	= (MCBANK_ERR_INFO *)( (UINTN)mPrevBootErrSrcHob + Length);
		switch (McBankErrInfo->Type)	{
		
		case McBankType :
			
			// Check for memory Mcbank errors.
			if((McBankErrInfo->McBankNum <= IVT_MEM_MCBANK_NUM_END) && (McBankErrInfo->McBankNum >= IVT_MEM_MCBANK_NUM_START)) {
				
				MsrIa32MCiStatusReg.Data = McBankErrInfo->McBankStatus;
				//check for fatal error.
				if (MsrIa32MCiStatusReg.Bits.uc == 1 && MsrIa32MCiStatusReg.Bits.pcc == 1){
					Status = UpdateMemoryError(McBankErrInfo);
					if(EFI_ERROR(Status)) return Status;
				}
			}  // check for Processor core errors
			else if( (McBankErrInfo->McBankNum <= IVT_CORE_MCBANK_NUM_END) && (McBankErrInfo->McBankNum >= IVT_CORE_MCBANK_NUM_START) ) {
				MsrIa32MCiStatusReg.Data = McBankErrInfo->McBankStatus;
				//check for fatal error.
				if (MsrIa32MCiStatusReg.Bits.uc == 1 && MsrIa32MCiStatusReg.Bits.pcc == 1){
					Status = UpdateCoreError(McBankErrInfo);
					if(EFI_ERROR(Status)) return Status;
				}


			}  // check for CBO errors
			else if ( (McBankErrInfo->McBankNum <= IVT_CBO_MCBANK_NUM_END) && (McBankErrInfo->McBankNum >= IVT_CBO_MCBANK_NUM_START)) {
				MsrIa32MCiStatusReg.Data = McBankErrInfo->McBankStatus;
				//check for fatal error.
				if (MsrIa32MCiStatusReg.Bits.uc == 1 && MsrIa32MCiStatusReg.Bits.pcc == 1){
					Status = UpdateCboError(McBankErrInfo);
					if(EFI_ERROR(Status)) return Status;
				}
			} // check for QPI errors.
			else if ( (McBankErrInfo->McBankNum <= IVT_QPI_MCBANK_NUM_END) && (McBankErrInfo->McBankNum >= IVT_QPI_MCBANK_NUM_START)) {
				MsrIa32MCiStatusReg.Data = McBankErrInfo->McBankStatus;
				//check for fatal error.
				if (MsrIa32MCiStatusReg.Bits.uc == 1 && MsrIa32MCiStatusReg.Bits.pcc == 1){
					Status = UpdateQpiPcuError(McBankErrInfo);
					if(EFI_ERROR(Status)) return Status;
				}
			}
			 
			Length += sizeof (MCBANK_ERR_INFO);

			break;

		case PciExType :

      PcieDev  = (PCI_EX_ERR_INFO *) (UINTN)(McBankErrInfo);
      // Update PCI-ex errors
      UpdatePciExError(PcieDev);

      mPrevBootErrData.PciexErrFound = TRUE;
      Length += sizeof (PCI_EX_ERR_INFO);

			break;

		//case CsrOtherType :

			//Length += sizeof (CSR_ERR_INFO);

			//break;

			default:
				return EFI_NOT_FOUND;

		}
	}

	return Status;
}
// AptioV Server Override Start: Added to log Error details for Last Boot Whea Memory Errors.
VOID
UpdateSmbiosHandles(
	)
	/*++

Routine Description:

  This function updates the Type 16 and Type 17 handles and othe required information.
  
Arguments:
  
	none

Returns:

	none

--*/
{
  UINT8	Node;
  UINT8	Ch;
  UINT8	Dimm;
  EFI_STATUS	Status;
  SMBIOS_TABLE_TYPE17                     *SmbiosType17Record;
  SMBIOS_TABLE_TYPE16                     *SmbiosType16Record;
  EFI_SMBIOS_HANDLE                       SmbiosHandle;
  EFI_SMBIOS_TYPE                         SmbiosType;
  EFI_SMBIOS_TABLE_HEADER                 *SmbiosRecord;

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  SmbiosType = EFI_SMBIOS_TYPE_PHYSICAL_MEMORY_ARRAY;
  Status = mSmbiosProtocol->GetNext (mSmbiosProtocol, &SmbiosHandle, &SmbiosType, &SmbiosRecord, NULL);

  mLastBootSmbios.LastBootType16Hanlde	= 00;
  if (!(EFI_ERROR(Status))) {
    SmbiosType16Record = (SMBIOS_TABLE_TYPE16 *) SmbiosRecord;
    mLastBootSmbios.LastBootType16Hanlde	= (SmbiosType16Record->Hdr.Handle);
  } 

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  for (Node = 0; Node < MAX_MEM_NODE; Node++) { // Loop for each CPU socket
    for (Ch = 0; Ch < MAX_CH; Ch++) { // Detect DIMMs on each channel
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    	mLastBootSmbios.LastBootType17Handle[Node][Ch][Dimm] = 0;
        SmbiosType = EFI_SMBIOS_TYPE_MEMORY_DEVICE;
        Status = mSmbiosProtocol->GetNext (mSmbiosProtocol, &SmbiosHandle, &SmbiosType, &SmbiosRecord, NULL);
        if (!(EFI_ERROR(Status))) {
          SmbiosType17Record = (SMBIOS_TABLE_TYPE17 *) SmbiosRecord;
          mLastBootSmbios.LastBootType17Handle[Node][Ch][Dimm]	= (SmbiosType17Record->Hdr.Handle);
        }
      }
    }
  }
}
// AptioV Server Override End: Added to log Error details for Last Boot Whea Memory Errors.
VOID
InitGlobalData(

  )
/*
Routine Description: 

  Initilizes the global data variables used in the driver

Arguments:  
	I/P and O/P : Updates the Previous boot error sorce pointer if gPrevBootErrSrcHobGuid HOB is located properly. 
                Else returns with NULL
	
Returns:	  none;
      

*/
{
  EFI_STATUS              Status = EFI_SUCCESS;
	EFI_HOB_GUID_TYPE       *GuidHob;
  UINTN                   varSize;
  SYSTEM_CONFIGURATION    setupData;
  EFI_IIO_UDS_PROTOCOL        *iioUdsProtocol;

  // Initialize with null pointer
  mPrevBootErrSrcHob = NULL;

	// Locate the WheaBootprotocol.
	Status = gBS->LocateProtocol (
					&gEfiWheaBootProtocolGuid,
					NULL,
					&mWheaBootProtocol
					);
	if(EFI_ERROR(Status)) return;


  varSize = sizeof(SYSTEM_CONFIGURATION);
  // AptioV Server Override: Changed variable name to IntelSetup
  Status = gRT->GetVariable( 
                  L"IntelSetup",
                  &gEfiSetupVariableGuid,
                  NULL,
                  &varSize,
                  &setupData 
                  );
	if(EFI_ERROR(Status)) return;

	Status = mWheaBootProtocol->WheaBootStartElog(mWheaBootProtocol);
	if(EFI_ERROR(Status)) return;

  Status = gSmst->SmmLocateProtocol (&gEfiMemRasProtocolGuid, NULL, &mMemRas);
	if(EFI_ERROR(Status)) return;

  Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid,NULL,&iioUdsProtocol);
	if(EFI_ERROR(Status)) return;
  mIioUds = iioUdsProtocol->IioUdsPtr;

  Status = gBS->LocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &mCpuCsrAccess);
  if (EFI_ERROR(Status))return ;
// AptioV Server Override Start: Added to log Error details for Last Boot Whea Memory Errors.
  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &mSmbiosProtocol);
  if (EFI_ERROR(Status)) {
	  mSmbiosProtocol = NULL;
  }
  else {
	  UpdateSmbiosHandles();
  }
// AptioV Server Override End: Added to log Error details for Last Boot Whea Memory Errors.
  mPrevBootErrData.SetupMcBankStsClear  = TRUE;
  mPrevBootErrData.PciexErrFound        = FALSE;
// AptioV Server Override Start: Added to log Error details for Last Boot Whea Memory Errors.
	if( setupData.WheaErrorRecordFormat == 1)
		mPrevBootErrData.WheaErrorRecordFormat	= GENERIC_ERROR_SECTION_REVISION_UEFI231;
	else 
		mPrevBootErrData.WheaErrorRecordFormat	= GENERIC_ERROR_SECTION_REVISION;
// AptioV Server Override End: Added to log Error details for Last Boot Whea Memory Errors.
	// Get the HOB Pointer to update the new error source
	GuidHob = GetFirstGuidHob (&gPrevBootErrSrcHobGuid);
	if (GuidHob == NULL) {
		// HOB not found
    return;
	}
	mPrevBootErrSrcHob = (PREV_BOOT_ERR_SRC_HOB	*)GET_GUID_HOB_DATA (GuidHob);
}

//
// Driver Entry Point
//
EFI_STATUS
LastBootErrorEntry(
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
/*++

Routine Description: 

  Entry point of the Function. 
  This function collects previous boot errors if any of presents. This function is currentlyimplemented for McBank errors.

Arguments:  

  ImageHandle   EFI_HANDLE: A handle for the image that is initializing this driver
  SystemTable   EFI_SYSTEM_TABLE: A pointer to the EFI system table        

Returns:  

  EFI_SUCCESS:              Driver initialized successfully
  EFI_LOAD_ERROR:           Failed to Initialize or has been loaded 
  EFI_OUT_OF_RESOURCES:     Could not allocate needed resources

--*/
{
// AptioV Server Override Start: Added to skip LastBoot Error Handler for cold boot.
  EFI_STATUS							Status;
  struct SystemMemoryMapHob             	*systemMemoryMap;
  EFI_HOB_GUID_TYPE                     		*GuidHob;
// AptioV Server Override End: Added to skip LastBoot Error Handler for cold boot.
  // Initilizes the global data , prtocols and the mPrevBootErrSrcHob hoib used in for updating whea boot error records.
  InitGlobalData();
// AptioV Server Override Start: Added to skip LastBoot Error Handler for cold boot.
  GuidHob = GetFirstGuidHob (&gEfiMemoryMapGuid);
  if ( GuidHob != NULL) {
	    systemMemoryMap = (struct SystemMemoryMapHob *) GET_GUID_HOB_DATA (GuidHob);
  }
  else {
	    systemMemoryMap = NULL;
	    Status = EFI_DEVICE_ERROR;
  }
//Grangeville AptioV server override - CPP Check
  if ((systemMemoryMap != NULL) &&(systemMemoryMap->IsColdBoot != TRUE)) {
	  // Collect if any previous boot fatal errors and update them in whea error boot record table that arew supported.
	  ChecAndUpdatePrevBootErrors();
  
	  if(mPrevBootErrData.PciexErrFound == FALSE) {
		  CheckAndUpdatePciExError();
	  }
  }
// AptioV Server Override End: Added to skip LastBoot Error Handler for cold boot.    
  return EFI_SUCCESS;
}

