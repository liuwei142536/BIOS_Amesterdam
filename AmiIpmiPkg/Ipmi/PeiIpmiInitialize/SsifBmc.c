//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file SsifBmc.c
    SSIF Transport Hook.

**/

//----------------------------------------------------------------------

#include "IpmiBmc.h"
#include "SsifBmc.h"

//----------------------------------------------------------------------

/**
    Sends the command to BMC port

    @param IpmiInstance Data structure describing BMC
                        variables and used for sending commands
    @param Context      NULL here
    @param Data         Pointer to command data that will be sent to BMC
                        along with Command
    @param DataSize     Size of the command data

    @return EFI_STATUS  Status of the Sending data to BMC port

**/

EFI_STATUS
SendDataToSsifBmcPort (
  IN  EFI_IPMI_BMC_INSTANCE_DATA      *IpmiInstance,
  IN  VOID                            *Context,
  IN  UINT8                           *Data,
  IN  UINT8                           DataSize )
{
    EFI_STATUS                  Status = EFI_SUCCESS;
    EFI_SMBUS_DEVICE_ADDRESS    BmcAddress;
    UINTN                       IpmiWriteCommand;
    UINT8                       IpmiData[SMBUS_BLOCK_LENGTH] = {0};
    EFI_PEI_SMBUS2_PPI          *EfiPeiSmbus2Ppi = NULL;
    UINT8                       RetryCount = 0;
    UINTN                       DataLength = DataSize;
    UINT8                       DataIndex = 0;
    BOOLEAN                     PECSupport;
    UINT8                       OriginalDataSize = DataSize;
    EFI_HOB_GUID_TYPE           *GuidHob;
    AMI_PEI_IPMI_DATA_HOB       *AmiPeiIpmiDataHob;

    GuidHob = (EFI_HOB_GUID_TYPE *) GetFirstGuidHob (&gAmiPeiIpmiDataHobGuid);
    if (GuidHob == NULL) {
        ASSERT (GuidHob != NULL);
        return EFI_NOT_FOUND;
    }

    AmiPeiIpmiDataHob = (AMI_PEI_IPMI_DATA_HOB *)GET_GUID_HOB_DATA (GuidHob);
    EfiPeiSmbus2Ppi = (EFI_PEI_SMBUS2_PPI *)AmiPeiIpmiDataHob->EfiPeiSmbus2Ppi;
    if (!EfiPeiSmbus2Ppi) {
        return EFI_UNSUPPORTED;
    }

    PECSupport = IpmiInstance->Interface.SSIF.PecSupport;
    BmcAddress.SmbusDeviceAddress = IPMI_SSIF_SLAVE_ADDRESS;

    do {
        if (OriginalDataSize == DataSize) {
            if (DataSize <= SMBUS_BLOCK_LENGTH) {
                //
                // Working single writes start
                //
                IpmiWriteCommand = SMBUS_SINGLE_WRITE_CMD;
                DataLength = DataSize;
                CopyMem (&IpmiData, &Data[DataIndex*SMBUS_BLOCK_LENGTH], DataLength);
            } else {
                //
                // Working multi-part writes start
                //
                IpmiWriteCommand = SMBUS_MULTI_WRITE_START_CMD;
                DataLength = SMBUS_BLOCK_LENGTH;
                CopyMem (&IpmiData, &Data[DataIndex*SMBUS_BLOCK_LENGTH], DataLength);
            }
        } else {
            if (DataSize > SMBUS_BLOCK_LENGTH) {
                //
                // Working multi-part writes middle
                //
                IpmiWriteCommand = SMBUS_MULTI_WRITE_MIDDLE_CMD;
                DataLength = SMBUS_BLOCK_LENGTH;
                CopyMem (&IpmiData, &Data[DataIndex*SMBUS_BLOCK_LENGTH], DataLength);
            } else {
                //
                // Working multi-part writes end
                //
                IpmiWriteCommand = SMBUS_MULTI_WRITE_END_CMD;
                DataLength = DataSize;
                CopyMem (IpmiData, &Data[DataIndex*SMBUS_BLOCK_LENGTH], DataLength);
            }
        }

        Status = EfiPeiSmbus2Ppi->Execute(
                                    EfiPeiSmbus2Ppi,
                                    BmcAddress,
                                    IpmiWriteCommand,
                                    EfiSmbusWriteBlock,
                                    PECSupport,
                                    &DataLength,
                                    IpmiData
                                    );
        if (!EFI_ERROR(Status)) {
            if (DataSize >=  SMBUS_BLOCK_LENGTH) {
                RetryCount = 0;
                DataSize -= SMBUS_BLOCK_LENGTH;
                DataIndex++;
            } else {
                DataSize = 0;
            }
        } else {
            if (RetryCount == IpmiInstance->Interface.SSIF.SsifRetryCounter) {
                break;
            } else {
                RetryCount++;
                //
                // Failed retries delay about 60ms to 250ms
                //
                MicroSecondDelay (IPMI_SSIF_TIME_BETWEEN_REQUEST_RETRIES_DELAY);
                ///
                /// If the Multi-part write fails, then try to write the
                /// data from the beginning.
                ///
                if (IpmiWriteCommand != SMBUS_SINGLE_WRITE_CMD) {
                    DataSize = OriginalDataSize;
                    DataIndex = 0;
                }
            }
        }
    } while (DataSize);

    return Status;
}

/**
    Receives the Data from BMC port

    @param IpmiInstance Data structure describing BMC
                        variables and used for sending commands
    @param Context      NULL here
    @param Data         Pointer to Response data that is received from BMC
    @param DataSize     Size of the Response data

    @return EFI_STATUS  Status of the Receiving data from BMC port

**/

EFI_STATUS
ReceiveBmcDataFromSsifPort (
  IN  EFI_IPMI_BMC_INSTANCE_DATA      *IpmiInstance,
  IN  VOID                            *Context,
  OUT UINT8                           *Data,
  OUT UINT8                           *DataSize )
{
    EFI_STATUS                  Status = EFI_SUCCESS;
    EFI_SMBUS_DEVICE_ADDRESS    BmcAddress;
    UINTN                       IpmiReadCommand;
    UINT8                       IpmiData[SMBUS_BLOCK_LENGTH] = {0};
    EFI_PEI_SMBUS2_PPI          *EfiPeiSmbus2Ppi = NULL;
    UINTN                       DataLength = *DataSize;
    BOOLEAN                     PECSupport;
    UINT8                       RetryCount = 0;
    UINT8                       OriginalDataSize = *DataSize;
    EFI_HOB_GUID_TYPE           *GuidHob;
    AMI_PEI_IPMI_DATA_HOB       *AmiPeiIpmiDataHob;

    GuidHob = (EFI_HOB_GUID_TYPE *) GetFirstGuidHob (&gAmiPeiIpmiDataHobGuid);
    if (GuidHob == NULL) {
        ASSERT (GuidHob != NULL);
        return EFI_NOT_FOUND;
    }

    AmiPeiIpmiDataHob = (AMI_PEI_IPMI_DATA_HOB *)GET_GUID_HOB_DATA (GuidHob);
    EfiPeiSmbus2Ppi = (EFI_PEI_SMBUS2_PPI *)AmiPeiIpmiDataHob->EfiPeiSmbus2Ppi;
    if (!EfiPeiSmbus2Ppi) {
        return EFI_UNSUPPORTED;
    }

    PECSupport = IpmiInstance->Interface.SSIF.PecSupport;
    BmcAddress.SmbusDeviceAddress = IPMI_SSIF_SLAVE_ADDRESS;
    IpmiReadCommand = SMBUS_SINGLE_READ_CMD;

    while (RetryCount <= IpmiInstance->Interface.SSIF.SsifRetryCounter) {
        Status = EfiPeiSmbus2Ppi->Execute(
                                    EfiPeiSmbus2Ppi,
                                    BmcAddress,
                                    IpmiReadCommand,
                                    EfiSmbusReadBlock,
                                    PECSupport,
                                    &DataLength,
                                    IpmiData
                                    );

        if (EFI_ERROR(Status)) {
            RetryCount++;
            ///
            /// Failed retries delay about 60ms to 250ms
            ///
            MicroSecondDelay (IPMI_SSIF_TIME_BETWEEN_REQUEST_RETRIES_DELAY);
            ///
            /// If the Multi-part Read command fails, then try to read the
            /// data from the beginning.
            ///
            if (IpmiReadCommand != SMBUS_SINGLE_READ_CMD) {
                IpmiReadCommand = SMBUS_SINGLE_READ_CMD;
            }
            DataLength = OriginalDataSize;
            continue;
        }

        if (IpmiReadCommand == SMBUS_SINGLE_READ_CMD) {
            if ((IpmiData[0] == MULTI_READ_ZEROTH_STRT_BIT) && (IpmiData[1] == MULTI_READ_FIRST_STRT_BIT)) {
                //
                // Working multi-part reads start
                //
                CopyMem(Data, &IpmiData[2], DataLength-2);
                *DataSize = DataLength-2;
                IpmiReadCommand = SMBUS_MULTI_READ_MIDDLE_CMD;
            } else {
                //
                // Working single reads start
                //
                CopyMem(Data, &IpmiData[0], DataLength);
                *DataSize = DataLength;
                break;
            }
        } else {
            if(IpmiData[0] == 0xFF) {
                //
                // Working multi-part reads end
                //
                CopyMem(&Data[*DataSize], &IpmiData[1], DataLength-1);
                *DataSize += DataLength-1;
                break;
            } else {
                //
                // Working multi-part reads middle
                //
                CopyMem(&Data[*DataSize], &IpmiData[1], DataLength-1);
                *DataSize += DataLength-1;
                IpmiReadCommand = SMBUS_MULTI_READ_MIDDLE_CMD;
            }
        }
    }

    return Status;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
