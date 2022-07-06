//**********************************************************************//
//**********************************************************************//
//**                                                                  **//
//**        (C)Copyright 1985-2015, American Megatrends, Inc.         **//
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
// Name:		RtErrorLogBoard.h
//
// Description:	
//
//----------------------------------------------------------------------------
//<AMI_FHDR_END>

#ifndef _RT_ERRORLOG_BOARD_H_
#define _RT_ERRORLOG_BOARD_H_

#include "token.h"
#include  <RtErrorlogElink.h>
#include "CommonErrorHandlerIncludes.h"

// Changes done for LastBoot Error handler support
extern BOOLEAN mLastbootMemErrFlag;

//
// PCI Error Status Bits
//
#define PCI_STS_MSTR_DATA_PERR      BIT8
#define PCI_STS_SIG_TRGT_ABRT       BIT11
#define PCI_STS_SIG_SERR            BIT14
#define PCI_STS_DET_PERR            BIT15

#define PCIE_AER_UNCERRSTS_OFFSET   0x04
#define PCIE_AER_UNCERRMSK_OFFSET   0x08
#define PCIE_AER_CORERRSTS_OFFSET   0x10
#define PCIE_AER_CORERRMSK_OFFSET   0x14
#define PCIE_AER_RPERRSTS_OFFSET    0x30
#define	PCIE_TYPE_ROOTPORT			4

#define LASTBOOT_PCIE_ERROR                             0x0F

typedef enum {
	Correctable	= 0,
	Recoverable = 1,
	Fatal,
	UnCorrectable,
	OtherSev
} ERROR_SEVERITY;

typedef enum {
	Warm=1,
	Cold,
	PowerGood,
} RESET_TYPE;

typedef struct {
  ERROR_SOURCE   	ErrorType;
  ERROR_SEVERITY   	ErrorSeverity;
} ERROR_HEADER;

typedef struct {
  ERROR_HEADER   	Header;
  UINT8   			ErrorSubType;
  UINT8   			Socket;
  UINT8   			Port;
  UINT8   			Data[1];
} ERROR_DATA_TYPE1;

typedef struct _IOH_QPI_COMBINED_ERROR_RECORD {
  ERROR_DATA_TYPE1             IohQpiErrorRecord;
}IOH_QPI_COMBINED_ERROR_RECORD;

typedef struct _IOH_CORE_COMBINED_ERROR_RECORD {
  ERROR_DATA_TYPE1             IohCoreErrorRecord;
}IOH_CORE_COMBINED_ERROR_RECORD;

typedef struct _IOH_VTD_COMBINED_ERROR_RECORD {
  ERROR_DATA_TYPE1             IohVtdErrorRecord;
}IOH_VTD_COMBINED_ERROR_RECORD;

typedef struct {
  //
  // Header-like information...
  //
  UINT8                                   ErrorType; // first one
  UINT8                                   ErrorSeverity;  // second type
  //
  // Essential information...
  //
  UINT8                                   Socket;
  UINT8                                   QpiPort;
  BOOLEAN                            Valid;
  UINT8                                   CorrectedErrorCount;
  UINT64				  Address;
  UINT16                                 ModelSpecificCode; // Will go as extended data...
  UINT16                                 MachineCheckCode; // Will go as extended data...
} QPI_ERROR_RECORD;

typedef struct {
  UINT8   				Socket;
  UINT8   				Branch;
  UINT8   				MemoryController;
  UINT8   				FbdChannel;
  UINT8   				DdrChannel;
  UINT8   				Dimm;
} MEM_ERROR_LOCATION;

typedef struct {
  //
  // Header-like information...
  //
  UINT8                                   ErrorType; // first one
  UINT8                                   ErrorSeverity;  // second type
  //
  // Essential information...
  //
  UINT8                                   Socket;
  UINT8                                   CoreMcaBank;
  BOOLEAN                            	  Valid;
  UINT8                                   CorrectedErrorCount;
  UINT16                                  ModelSpecificCode; // Will go as extended data...
  UINT16                                  MachineCheckCode; // Will go as extended data...
} CORE_ERROR_RECORD;

typedef struct {
  //
  // Header-like information...
  //
  UINT8                                   ErrorType; // first one
  UINT8                                   ErrorSeverity;  // second type
  //
  // Essential information...
  //
  UINT8                                   Socket;
  UINT8                                   CoreId;
  UINT8					 				  ThreadId;
  UINT8                                   CboMcaBank;
  BOOLEAN                            	  Valid;
  UINT8                                   CorrectedErrorCount;
  UINT64				  				  Address;
  UINT16                                  ModelSpecificCode; // Will go as extended data...
  UINT16                                  MachineCheckCode; // Will go as extended data...
} CBO_ERROR_RECORD;
#endif

//**********************************************************************//
//**********************************************************************//
//**                                                                  **//
//**        (C)Copyright 1985-2015, American Megatrends, Inc.         **//
//**                                                                  **//
//**                       All Rights Reserved.                       **//
//**                                                                  **//
//**       5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093       **//
//**                                                                  **//
//**                       Phone: (770)-246-8600                      **//
//**                                                                  **//
//**********************************************************************//
//**********************************************************************//
