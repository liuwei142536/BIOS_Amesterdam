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
// Revision History
// ----------------
// $Log: $
// 

//<AMI_FHDR_START>
//----------------------------------------------------------------------------
//
// Name:		RtErrorLogBmc.h
//
// Description:	 Runtime error log BMC includes and declarations 
//
//----------------------------------------------------------------------------
//<AMI_FHDR_END>

#ifndef _BMC_SEL_LOG_H
#define _BMC_SEL_LOG_H

#include "token.h"
#include "RtErrorLogBoard.h"
#include <Protocol/IPMITransportProtocol.h>

#define ADD_SEL_ENTRY                   0x44
#define EFI_SM_STORAGE      			  0xA

//
// General Info related to SEL Record
//

#define EFI_EVM_REVISION    0x04
#define EFI_BIOS_ID         0x18
#define EFI_FORMAT_REV      0x00
#define EFI_FORMAT_REV1     0x01
#define EFI_SOFTWARE_ID     0x01
#define EFI_SENSOR_TYPE_EVENT_CODE_DISCRETE     0x6F

#define EFI_MEMORY_SENSOR_TYPE              0x0C
#define EFI_PROCESSOR_SENSOR_TYPE           0x07
#define SEL_SYSTEM_RECORD                   0x2  

//
// SEL Sensor Types and Error Codes for PCI Errors
//
#define SEL_SENS_TYPE_CRIT_ERR      0x13
#define SEL_SENS_PCI_PERR           0x04      
#define SEL_SENS_PCI_SERR           0x05

#define MEMORY_SENSOR_CE_OFFSET              0x00
#define MEMORY_SENSOR_UCE_OFFSET              0x01
#define MEMORY_SENSOR_PARITY_ERROR_OFFSET              0x02
#define MEMORY_SENSOR_CEMAX_OFFSET              0x05

#define CRITICAL_INTERRUPT_PCI_PERR             0x04
#define CRITICAL_INTERRUPT_PCI_SERR             0x05
#define CRITICAL_INTERRUPT_BUS_CORERR     0x07
#define CRITICAL_INTERRUPT_BUS_UNCERR     0x08
#define CRITICAL_INTERRUPT_BUS_FATERR       0x0A

#define EFI_SOFTWARE_ID_SELECT                    0x01
#define SMI_HANDLER_SOFTWARE_ID                   0x10
#define EFI_GENERATOR_ID(SOFTWARE_ID)             ( (SOFTWARE_ID << 1) | (EFI_SOFTWARE_ID_SELECT) )

#define SOCKET_NUM_BITS              0xC0

#define OEMEvData3_SOCKET_NUM_BITS             0x6
#define OEMEvData3_CHANNEL_NUM_BITS             0x4
#define DIMM_NUM_BITS                0x0F

/*typedef enum {
  EFI_MEMORY_SENSOR_OFFSET_ECC_CORRECTABLE=0,
  EFI_MEMORY_SENSOR_OFFSET_ECC_UNCORRECTABLE,
  EFI_MEMORY_SENSOR_OFFSET_PARITY,
  EFI_MEMORY_SENSOR_OFFSET_SCRUB_FAILED,
  EFI_MEMORY_SENSOR_OFFSET_MEM_DEVICE_DISABLED,
  EFI_MEMORY_SENSOR_OFFSET_ECC_CORRECTABLE_MAX,
}EFI_SEL_SENSOR_MEMORY_OFFSET;*/

typedef enum {
  EFI_PROCESSOR_SENSOR_OFFSET_CONFIGURATION_ERROR=5,
}EFI_SEL_SENSOR_PROCESSOR_OFFSET;

/*#pragma pack(1)
typedef struct {
  UINT16  RecordId;
  UINT8   RecordType;
  UINT32  TimeStamp;
  UINT16  GeneratorId;
  UINT8   EvMRevision;
  UINT8   SensorType;
  UINT8   SensorNumber;
  UINT8   EventDirType;
  UINT8   OEMEvData1;
  UINT8   OEMEvData2;
  UINT8   OEMEvData3;
}EFI_SEL_RECORD_DATA;
#pragma pack()
*/

EFI_STATUS
LogMeToSel (
		MEMORY_DEV_INFO		*MemInfo
 );

/*Aptio V Server Override Start : Arguments passed from LogPciErrorRecord() varies.
EFI_STATUS
LogPciErrorToBmc (
    IN UINT8    Bus,
    IN UINT8    Dev,
    IN UINT8    Func,
    IN BOOLEAN  IsBridge,
    IN UINT16   SecondaryStatus,
    IN UINT8    PcieOffset,
    IN UINT16   PcieDeviceStatus,
    IN UINT16   AerOffset,
    IN UINT32   UncorrErrStatus,
    IN UINT32   CorrErrStatus,
    IN UINT32   RootErrStatus
 );

*/ // Aptio V Server Override Start : Arguments passed from LogPciErrorRecord() varies.
EFI_STATUS
LogPciErrorToBmc (
    IN UINT8    ErrorSource,
    IN UINT8    ErrType,
    IN UINT8    Segment,
    IN UINT8    Bus,
    IN UINT8    Dev,
    IN UINT8    Func,
    IN BOOLEAN  IsBridge,
    IN UINT16   SecondaryStatus,
    IN UINT8    PcieCapOffset,
    IN UINT16   PcieDeviceStatus,
    IN UINT16   AerOffset,
    IN UINT32   UncorrErrStatus,
    IN UINT32   CorrErrStatus,
    IN UINT32   RootErrStatus
);

EFI_STATUS
LogCoreToSel (
	CORE_ERROR_RECORD                  	*CoreErrorData
);

EFI_STATUS
LogQpiToSel (
    IN  QPI_ERROR_RECORD              	*QpiErrorData
);

EFI_STATUS
LogCboToSel (
	CBO_ERROR_RECORD                  	*CboErrorData
);
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
