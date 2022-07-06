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

/** @file IpmiSystemInfo.c
    Updates System info to BMC ex :Firmware version.

**/

#include "IpmiBmc.h"

#define MAX_RECOMMENDED_STRING_LENGTH         256
#define DATA_BLOCK_SIZE                        16
//
// external Variables
//
extern EFI_IPMI_BMC_INSTANCE_DATA   *gIpmiInstance;

#if (!IPMI_DEFAULT_HOOK_SUPPORT)
extern IPMI_OEM_SYSTEM_INFO_UPDATE_HOOK IPMI_OEM_SYSTEM_INFO_UPDATE_HOOK_LIST EndOfIpmiOemSystemInfoHookListFunctions;
IPMI_OEM_SYSTEM_INFO_UPDATE_HOOK* gIpmiOemSystemInfoHookList[] = { IPMI_OEM_SYSTEM_INFO_UPDATE_HOOK_LIST NULL};
#endif

/**
  Acquire the string associated with the Index from SMBIOS structure and return it.

  @param    OptionalStrStart  The start position to search the string
  @param    Index             The index of the string to extract
  @param    String            The string that is extracted

  @retval   EFI_SUCCESS       The function returns EFI_SUCCESS if successful.
  @retval   EFI_NOT_FOUND     The function returns EFI_NOT_FOUND if unsuccessful.

**/
EFI_STATUS
SmbiosGetStringByIndex(
  IN    CHAR8   *OptionalStrStart,
  IN    UINT8   Index, 
  OUT   UINT8  **String
  )
{
    UINTN          StrSize;

    if (Index == 0) {
      return EFI_NOT_FOUND;
    }
    
    DEBUG((EFI_D_INFO, "SmbiosGetStringByIndex -  %d Entry \n", Index));
    
    StrSize = 0;
    do {
      Index--;
      OptionalStrStart += StrSize;
      StrSize           = AsciiStrSize (OptionalStrStart); // size includes null terminator
    // SMBIOS strings are NULL terminated, and end of all strings is indicated with NULL
    // loop until at end of all SMBIOS strings (found NULL terminator at string index past string's NULL), and Index != 0
    } while ((OptionalStrStart[StrSize] != 0) && (Index != 0));

    if ((Index != 0) || (StrSize == 1)) {
        DEBUG((EFI_D_INFO, "SmbiosGetStringByIndex - NOT_FOUND \n"));
      //
      // Meet the end of strings set but Index is non-zero
      return EFI_NOT_FOUND;
    } else {
        DEBUG((EFI_D_INFO, "SmbiosGetStringByIndex - SUCCESS  String: %a \n", OptionalStrStart));
        (*String) = (UINT8*)OptionalStrStart;
        return EFI_SUCCESS;
    }
}

/**
    Checks for Set-In Progress Bit and Wait to get it Clear

    @retval  EFI_SUCCESS - Set-In Progress Bit is Cleared
    @retval  EFI_TIMEOUT - Specified Time out over

**/

EFI_STATUS
IpmiSysteminfoWaitSetInProgressClear (
  VOID
  ) 
{
    EFI_STATUS                      Status;
    UINT8                           ResponseSize;
    UINT8                           Retries = 10;
    EFI_GET_SYSTEM_INFO_RESPONSE    GetSetInProgResponse;
    EFI_GET_SYSTEM_INFO_REQUEST     GetSystemInfoParamCmd;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry...\n", __FUNCTION__));
    
    GetSystemInfoParamCmd.Reserved= 0;
    GetSystemInfoParamCmd.GetParam = 0;
    GetSystemInfoParamCmd.ParamSelector = 0;
    GetSystemInfoParamCmd.SetSelector = 0;
    GetSystemInfoParamCmd.BlockSelector = 0;

    // Wait for Set-In progress bit to clear
    do {
    
        ResponseSize = sizeof (GetSetInProgResponse);
        Status = gIpmiInstance->IpmiTransport.SendIpmiCommand (
                                                  &gIpmiInstance->IpmiTransport,
                                                  IPMI_NETFN_APP,
                                                  BMC_LUN,
                                                  IPMI_APP_GET_SYSTEM_INFO,
                                                  (UINT8*) &GetSystemInfoParamCmd,
                                                  sizeof (GetSystemInfoParamCmd),
                                                  (UINT8*) &GetSetInProgResponse,
                                                  &ResponseSize );

        if (EFI_ERROR (Status)) {
            return Status;
        }

        if (GetSetInProgResponse.Data1.Progress.State == SysInfoStateSetComplete) {
            break;
        }
        gBS->Stall (1000);
    } while (Retries-- > 0);

    if (Retries == 0) {
        return EFI_TIMEOUT;
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));
    return EFI_SUCCESS;
}

/**

    Updates specified parameter string to BMC

    @param StringPtr             Pointer to String buffer
    @param ParameterSelector     Parameter to be updated
    @param StringEncoding        String encoding format in String Buffer

    @return EFI_SUCCESS          System Info with specified Parameter updated successfully

**/
EFI_STATUS
AddSystemInfo(
  IN  VOID                                  *StringPtr,
  IN  SYSTEM_INFO_PARAMETER_SELECTOR        ParameterSelector,
  IN  SYSTEM_INFO_STRING_ENCODING           StringEncoding
  )
{
    EFI_STATUS                             Status;
    EFI_SET_SYSTEM_INFO_REQUEST            SetSysInfoReq;
    UINTN                                  StrLength;
    UINT8                                  SysInfoStrLength;
    UINT8                                  StrCpyLength;
    UINT8                                  BlockNumber = 0;
    UINT8                                  ResponseDataSize;
    
    
    if (StringPtr == NULL) {
        DEBUG((DEBUG_ERROR, "%a String buffer pointer is NULL\n",__FUNCTION__));
        return EFI_INVALID_PARAMETER;
    }
    
    if (StringEncoding > SysInfoUnicode)
        return EFI_INVALID_PARAMETER;
    
    if (StringEncoding == SysInfoUnicode) {
        SysInfoStrLength = (UINT8)StrLen (StringPtr);
        StrLength = SysInfoStrLength * 2;          //Unicode is 2 byte for each Char
        
    } else {
        SysInfoStrLength = (UINT8)AsciiStrLen (StringPtr);
        StrLength = SysInfoStrLength;
    }
    
    //String length includes NULL terminator. If NULL string is passed, length will be less than or eqaul to 2
    if ((StrLength > MAX_RECOMMENDED_STRING_LENGTH) || (StrLength <= 2)) {
        DEBUG((DEBUG_ERROR, "%a String length is greater or Lesser than recommended string length\n",__FUNCTION__));
        return EFI_INVALID_PARAMETER;
    }
    
    ResponseDataSize = 0;
    
    do {
        
        ZeroMem (&SetSysInfoReq, sizeof (SetSysInfoReq));
        
        SetSysInfoReq.ParamSelector = ParameterSelector;   
        SetSysInfoReq.SetSelector = BlockNumber;
        
        if (SetSysInfoReq.SetSelector == 0) {
            //For the first block of string data (set selector = 0), the first two bytes indicate the encoding of the string and its overall length
            SetSysInfoReq.Data[0] = StringEncoding;
            SetSysInfoReq.Data[1] = SysInfoStrLength;
            
            if (StrLength > (DATA_BLOCK_SIZE - 2)) {
                StrCpyLength = DATA_BLOCK_SIZE - 2;
                StrLength -= StrCpyLength;
            } else {
                StrCpyLength = (UINT8)StrLength;
                StrLength -= StrCpyLength;
            }
            
            CopyMem ((UINT8 *)&SetSysInfoReq.Data[2], (UINT8 *)StringPtr, StrCpyLength);  //Exclude String length and encoding in first block
            
        }else {
            
            if (StrLength > DATA_BLOCK_SIZE) {
                StrCpyLength = DATA_BLOCK_SIZE;
                StrLength -= StrCpyLength;
            } else {
                StrCpyLength = (UINT8)StrLength;
                StrLength -= StrCpyLength;
            }
            
            CopyMem (SetSysInfoReq.Data, (UINT8 *)StringPtr, StrCpyLength);
        }
        Status = IpmiSysteminfoWaitSetInProgressClear();
        
        if (EFI_ERROR (Status)) {
            DEBUG((DEBUG_ERROR, "IPMI IpmiSysteminfoWaitSetInProgressClear status :%r\n",Status));
            return Status;
        }
        //Send System info Command
        Status = gIpmiInstance->IpmiTransport.SendIpmiCommand (
                                                    &gIpmiInstance->IpmiTransport,
                                                    IPMI_NETFN_APP,
                                                    BMC_LUN,
                                                    IPMI_APP_SET_SYSTEM_INFO,
                                                    (UINT8*) &SetSysInfoReq,
                                                    sizeof (SetSysInfoReq),
                                                    NULL,
                                                    &ResponseDataSize );
        
        if (EFI_ERROR(Status) || gIpmiInstance->IpmiTransport.CommandCompletionCode != 0x00) {
            DEBUG((DEBUG_ERROR, "%a Command Completion code: %x, Status%r\n",__FUNCTION__, gIpmiInstance->IpmiTransport.CommandCompletionCode, Status));
            return EFI_DEVICE_ERROR;
        }
        
        //Break the loop when no string available to send.
        if (!StrLength)
            break;
        
        StringPtr = (UINT8 *)StringPtr + StrCpyLength;
        
        BlockNumber++;
        
    }while (TRUE);
        
    return Status;
}

/**

    Update System info to BMC ex:Firmware version

    @param SmbiosProtocol     Pointer to SMBIOS protocol instance

    @return EFI_SUCCESS       System Info updated successfully

**/
EFI_STATUS
IpmiUpdateSystemInfo (
  IN  EFI_SMBIOS_PROTOCOL             *SmbiosProtocol
  )
{
    EFI_STATUS                                Status;
    SMBIOS_TABLE_TYPE0                        *SmbiosType0Record;
    EFI_SMBIOS_TYPE                           SmbiosType;
    EFI_SMBIOS_HANDLE                         SmbiosHandle;
    VOID                                      *StringPtr = NULL;
#if (!IPMI_DEFAULT_HOOK_SUPPORT)
    SYSTEM_INFO_PARAMETER_SELECTOR            ParameterSelector,
    SYSTEM_INFO_STRING_ENCODING               StringEncoding
    UINT8                                     Index;
#endif
    
    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    SmbiosType = EFI_SMBIOS_TYPE_BIOS_INFORMATION;
    
    Status = SmbiosProtocol->GetNext (SmbiosProtocol,
                                      &SmbiosHandle,
                                      &SmbiosType,
                                      (EFI_SMBIOS_TABLE_HEADER**)&SmbiosType0Record,
                                      NULL
                                      );
    if (EFI_ERROR(Status)) {
        DEBUG((EFI_D_ERROR, "ERROR: Type 0 Entry is not available in SM BIOS table \n"));
        return Status;
    }

    Status = SmbiosGetStringByIndex((CHAR8*)((UINT8*)SmbiosType0Record + SmbiosType0Record->Hdr.Length), SmbiosType0Record->BiosVersion, (UINT8 **)&StringPtr);
    
    if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_INFO, "IPMI SmbiosGetStringByIndex status :%r\n",Status));
        return Status;
    }

    Status = AddSystemInfo (StringPtr, SysInfoFirmwareVersion, SysInfoAscii);
    
    if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_INFO, "IPMI SetSysteminfo status :%r\n",Status));
        return Status;
    }
    
#if (!IPMI_DEFAULT_HOOK_SUPPORT)
    //Hook to update Other system info parameters.
    for (Index = 0; gIpmiOemSystemInfoHookList[Index]; Index++) {
        StringPtr = NULL;
        Status = gIpmiOemBmcSystemInfoHookList [Index] (&StringPtr, &ParameterSelector, &StringEncoding);
        if (!EFI_ERROR(Status) && (StringPtr != NULL))
            Status = AddSystemInfo (StringPtr, SysInfoFirmwareVersion, SysInfoAscii);
    }
#endif
    
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
