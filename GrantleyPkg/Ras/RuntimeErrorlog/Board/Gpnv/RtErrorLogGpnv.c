//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2014, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

//**********************************************************************
// $Header:  $
//
// $Revision:  $
//
// $Date:  $
//**********************************************************************
// Revision History
// ----------------
// $Log:  $
// 
// 
// 
//**********************************************************************
//<AMI_FHDR_START>
//
// Name:  RtErrorLogGpnv.c
//
// Description: Runtime error log GPNV library function definitions
//
//<AMI_FHDR_END>
//**********************************************************************

#include "RtErrorLogGpnv.h"


EFI_SM_ELOG_PROTOCOL      *gGenericElogProtocol     = NULL;


//<AMI_PHDR_START>
//-------------------------------------------------------------------------
// Name:   EfiSmSetEventLogData
//
// Description:
//  This function sends event log data to the destination such as LAN, ICMB,
// BMC etc.  
//
// Input:
//  IN  UINT8                             *ElogData,
//  IN  EFI_SM_ELOG_TYPE                  DataType,
//  IN  BOOLEAN                           AlertEvent,
//  IN  UINTN                             DataSize,
//  OUT UINT64                            *RecordId
//
// Output:
//     EFI_STATUS
//
//--------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS
EfiSmSetEventLogData (
  IN  UINT8                             *ElogData,
  IN  EFI_SM_ELOG_TYPE                  DataType,
  IN  BOOLEAN                           AlertEvent,
  IN  UINTN                             DataSize,
  OUT UINT64                            *RecordId  )
{
    EFI_STATUS              Status;

    if (gGenericElogProtocol == NULL) {

        Status = gSmst->SmmLocateProtocol (
                &gEfiGenericElogProtocolGuid,
                NULL,
                &gGenericElogProtocol);
        if ((!EFI_ERROR(Status)) && (gGenericElogProtocol != NULL)) {
            return gGenericElogProtocol->SetEventLogData (
                    gGenericElogProtocol,
                    ElogData,
                    DataType,
                    AlertEvent,
                    DataSize,
                    RecordId );
        }
    }else {  
        return gGenericElogProtocol->SetEventLogData (
                gGenericElogProtocol,
                ElogData,
                DataType,
                AlertEvent,
                DataSize,
                RecordId );
    }

    return Status;

}

//<AMI_PHDR_START>
//-------------------------------------------------------------------------
// Name:   LogMemErrorToGpnv
//
// Description:
//  This function log memory errors to GPNV.  
//
// Input:
//  IN MEMORY_DEV_INFO     *MemInfo
//
// Output:
//     EFI_STATUS
//
//--------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS
LogMemErrorToGpnv(
  IN MEMORY_DEV_INFO     *MemInfo )
{
    EFI_STATUS              Status = EFI_SUCCESS;
    UINT64                  RecordId;
    EFI_GPNV_RUNTIME_ERROR  ToWrite;


    ToWrite.RecordType = EFI_EVENT_LOG_TYPE_OEM_GPNV_RUNTIME;
    ToWrite.EventLogType = EFI_EVENT_LOG_TYPE_SINGLE_BIT_ECC_MEMORY_ERROR;
    ToWrite.EventLogType = (MemInfo->Type == COR_ERROR_TYPE ?
                        EFI_EVENT_LOG_TYPE_SINGLE_BIT_ECC_MEMORY_ERROR :
                        EFI_EVENT_LOG_TYPE_MULTI_BIT_ECC_MEMORY_ERROR);

    Status = EfiSmSetEventLogData (
                (UINT8*)&ToWrite,
                EfiElogSmSMBIOS,
                FALSE,
                sizeof (EFI_GPNV_RUNTIME_ERROR),
                &RecordId );

    return Status;
}

//<AMI_PHDR_START>
//-------------------------------------------------------------------------
// Name:   LogPciErrorToGpnv
//
// Description:
//  This function log PCI errors to GPNV.  
//
// Input:
//  IN UINT8    ErrorSource,
//  IN UINT8    ErrType,
//  IN UINT8    Segment,
//  IN UINT8    Bus,
//  IN UINT8    Dev,
//  IN UINT8    Func
//
// Output:
//     EFI_STATUS
//
//--------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS
LogPciErrorToGpnv (
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
  IN UINT32   RootErrStatus)
{
    EFI_STATUS              Status = EFI_SUCCESS;
    UINT16                  Data16;
    UINT64                  RecordId;
    EFI_GPNV_RUNTIME_ERROR  ToWrite;


    ToWrite.RecordType = EFI_EVENT_LOG_TYPE_OEM_GPNV_RUNTIME;

    // Determine what sort of error occurred.
    Data16 = *(UINT16*)PCIE_CFG_ADDR( Bus, Dev, Func, PCI_STS );
    Data16 &= (PCI_STS_MSTR_DATA_PERR | PCI_STS_SIG_SERR | PCI_STS_DET_PERR);
    if (Data16){

        // Determine whether PERR or SERR.
        if (Data16 & (PCI_STS_MSTR_DATA_PERR | PCI_STS_DET_PERR))
            ToWrite.EventLogType = EFI_EVENT_LOG_TYPE_PCI_PARITY_ERROR;
        else
            ToWrite.EventLogType = EFI_EVENT_LOG_TYPE_PCI_SYSTEM_ERROR;
    }

    Status = EfiSmSetEventLogData (
                (UINT8*)&ToWrite,
                EfiElogSmSMBIOS,
                FALSE,
                sizeof (EFI_GPNV_RUNTIME_ERROR),
                &RecordId );

    return EFI_SUCCESS;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2014, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
