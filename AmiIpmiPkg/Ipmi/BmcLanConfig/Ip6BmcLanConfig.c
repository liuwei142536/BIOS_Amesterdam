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

/** @file Ip6BmcLanConfig.c
    The function of the driver is to configure IPV6 BMC LAN 
    This driver can get these parameters from a
    DHCP server or as user input from BIOS setup.

**/

//---------------------------------------------------------------------

#include <Library/DebugLib.h>
#include "Ip6BmcLanConfig.h"

//---------------------------------------------------------------------

//
// Global Variables
//
EFI_IPMI_TRANSPORT                  *gIpmiTransport;
SERVER_MGMT_CONFIGURATION_DATA      gServerMgmtConfiguration;

/**
    DriverEntryPoint 
    In this driver we will configure Ipv6 address to BMC LAN channel using static
    information available through setup or using a DHCP client as
    per user request.

    @param ImageHandle - Variable of EFI_HANDLE
    @param SystemTable - Pointer to EFI_SYSTEM_TABLE

    @return EFI_STATUS

**/

EFI_STATUS
InitializeIp6BmcLanConfig (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable )
{

    EFI_STATUS                         Status;
    UINTN                              Size;
    UINT32                             Attributes;
    IPMI_IPV6_ADDRESS                  Ipv6Addr;
    UINT8                              StaticSupport;
    CHAR16                             StaticIp6Lan[40];
    CHAR16                             StaticIp6LanRouterIp[40];
    UINT8                              BmcLan1= 0;
#if BMC_LAN_COUNT == 2
    UINT8                              BmcLan2 = 0;
#endif
    UINT8                              ChannelNumberBuffer[BMC_LAN_COUNT];
    UINT8                              ChannelCount = BMC_LAN_COUNT;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry... \n", __FUNCTION__));

    Status = gBS->LocateProtocol (
                &gEfiDxeIpmiTransportProtocolGuid,
                NULL,
                (VOID **)&gIpmiTransport );
    SERVER_IPMI_DEBUG ((DEBUG_INFO, " IPMI : gEfiDxeIpmiTransportProtocolGuid Status: %r \n", Status));
    if (EFI_ERROR (Status)) {
        return Status;
    }

    Size = sizeof (gServerMgmtConfiguration);
    Status = gRT->GetVariable (
                    L"ServerSetup",
                    &gEfiServerMgmtSetupVariableGuid,
                    &Attributes,
                    &Size,
                    &gServerMgmtConfiguration );

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI : GetVariable status of Server Setup %r \n", Status));
    if (EFI_ERROR(Status)) {
        BmcLan1 = IpmiUnspecified;
#if BMC_LAN_COUNT == 2
        BmcLan2 = IpmiUnspecified;
#endif
    } else {
        BmcLan1 = gServerMgmtConfiguration.Ip6BmcLan1;
#if BMC_LAN_COUNT == 2
        BmcLan2 = gServerMgmtConfiguration.Ip6BmcLan2;
#endif
    }

    Status = GetLanChannelNumber (gIpmiTransport, ChannelNumberBuffer, &ChannelCount);
    DEBUG ((DEBUG_INFO, "%a ChannelNumberBuffer[0]: %x ChannelNumberBuffer[1]: %x ChannelCount = %x\n",__FUNCTION__, ChannelNumberBuffer[0], ChannelNumberBuffer[1], ChannelCount));
    if (EFI_ERROR(Status))
        return Status;
    
    //
    // Check if Ipv6 supported or not.
    //
    Status = GetIp6Supported (ChannelNumberBuffer[0]);
    DEBUG ((DEBUG_INFO, "IPMI : BMC LAN1 Ip6 configuration Status: %r\n", Status));    if (EFI_ERROR(Status)) {
        gServerMgmtConfiguration.Ip6Support1 = 0;
    }
    if (gServerMgmtConfiguration.Ip6Support1) {
        //
        // Setting to unspecified since request is processed.
        //
        gServerMgmtConfiguration.Ip6BmcLan1 = IpmiUnspecified;
    
        if (BmcLan1 == IpmiStaticAddress) {
            StaticSupport = 1;
        } else if (BmcLan1 == IpmiDynamicAddressBmcDhcp){
            StaticSupport = 0;
        }
    
        if (BmcLan1 != IpmiUnspecified) {
            //
            // Setting LAN1 address source to BMC.
            //
            SetIp6AddressSource (ChannelNumberBuffer[0], StaticSupport);
            SetIp6RouterAddressSource (ChannelNumberBuffer[0]);
    
            if (StaticSupport) {
                //
                // Setting Static Ipv6 address.
                // Local copy is made to prevent ASSERT due to alignment checking in ConvertStrToBmcIp6Address.
                //
                CopyMem ((UINT8*)StaticIp6Lan, (UINT8*)gServerMgmtConfiguration.StaticIp6Lan1, 40 * 2);
                DEBUG ((DEBUG_INFO, "InitializeIp6BmcLanConfig: BMC LAN1 Static Ipv6 Address: %S\n", StaticIp6Lan));
                ConvertStrToBmcIp6Address(StaticIp6Lan, &Ipv6Addr);
                SetIp6StaticAddressParam(ChannelNumberBuffer[0], Ipv6Addr, gServerMgmtConfiguration.PrefixLength1);
    
                CopyMem ((UINT8*)StaticIp6LanRouterIp, (UINT8*)gServerMgmtConfiguration.StaticIp6Lan1Router1Ip, 40 * 2);
                DEBUG ((DEBUG_INFO, "InitializeIp6BmcLanConfig: BMC LAN1 Static Router Ipv6 Address: %S\n", StaticIp6LanRouterIp));
                ConvertStrToBmcIp6Address(StaticIp6LanRouterIp, &Ipv6Addr);
                SetIp6StaticRouterAddressParam(ChannelNumberBuffer[0], Ipv6Addr);
            }
        }
    }

#if BMC_LAN_COUNT == 2
    if (ChannelNumberBuffer[1] != 0) {

        // Check if Ipv6 supported or not.
        Status = GetIp6Supported (ChannelNumberBuffer[1]);
        DEBUG ((DEBUG_INFO, "IPMI: BMC LAN2 Ip6 configuration Status: %r\n", Status));        if (EFI_ERROR(Status)) {
            gServerMgmtConfiguration.Ip6Support2 = 0;
        }

        if (gServerMgmtConfiguration.Ip6Support2) {
            // Setting to unspecified since request is processed.
            gServerMgmtConfiguration.Ip6BmcLan2 = IpmiUnspecified;
    
            if (BmcLan2 == IpmiStaticAddress) {
                StaticSupport = 1;
            } else if (BmcLan2 == IpmiDynamicAddressBmcDhcp){
                StaticSupport = 0;
            }
    
            if (BmcLan2 != IpmiUnspecified) {
                // Setting LAN1 address source to BMC.
                SetIp6AddressSource (ChannelNumberBuffer[1], StaticSupport);
                SetIp6RouterAddressSource (ChannelNumberBuffer[0]);
    
                if (StaticSupport) {
                    // Setting Static Ipv6 address.
                    // Local copy is made to prevent ASSERT due to alignment checking in ConvertStrToBmcIp6Address.
                    CopyMem ((UINT8*)StaticIp6Lan, (UINT8*)gServerMgmtConfiguration.StaticIp6Lan2, 40 * 2);
                    DEBUG ((DEBUG_INFO, "InitializeIp6BmcLanConfig: BMC LAN2 Static Ipv6 Address: %S\n", StaticIp6Lan));
                    ConvertStrToBmcIp6Address(StaticIp6Lan, &Ipv6Addr);
                    SetIp6StaticAddressParam(ChannelNumberBuffer[1], Ipv6Addr, gServerMgmtConfiguration.PrefixLength2);
    
                    CopyMem ((UINT8*)StaticIp6LanRouterIp, (UINT8*)gServerMgmtConfiguration.StaticIp6Lan2Router1Ip, 40 * 2);
                    DEBUG ((DEBUG_INFO, "InitializeIp6BmcLanConfig: BMC LAN1 Static Router Ipv6 Address: %S\n", StaticIp6LanRouterIp));
                    ConvertStrToBmcIp6Address(StaticIp6LanRouterIp, &Ipv6Addr);
                    SetIp6StaticRouterAddressParam(ChannelNumberBuffer[1], Ipv6Addr);
                }
            }
        }
    }
#endif
    Status = gRT->SetVariable (
                    L"ServerSetup",
                    &gEfiServerMgmtSetupVariableGuid,
                    Attributes,   // Attributes
                    sizeof(SERVER_MGMT_CONFIGURATION_DATA),
                    &gServerMgmtConfiguration);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI :SetVariable status of ServerSetup %r \n", Status));

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));
    return EFI_SUCCESS;
}

/**
  Convert one Null-terminated ASCII string to EFI_IPv6_ADDRESS. 

  @param[in]      String         The pointer to the Ascii string.
  @param[out]     Ip6Address     The pointer to the converted IPv6 address.

  @retval EFI_SUCCESS            Convert to IPv6 address successfully.
  @retval EFI_INVALID_PARAMETER  The string is mal-formated or Ip6Address is NULL.

**/
EFI_STATUS
EFIAPI
ConvertAsciiStrToBmcIp6 (
  IN CONST CHAR8                 *String,
  OUT      IPMI_IPV6_ADDRESS      *Ip6Address
  )
{
  UINT8                          Index;
  CHAR8                          *Ip6Str;
  CHAR8                          *TempStr;
  CHAR8                          *TempStr2;
  UINT8                          NodeCnt;
  UINT8                          TailNodeCnt;
  UINT8                          AllowedCnt;
  UINTN                          NodeVal;
  BOOLEAN                        Short;
  BOOLEAN                        Update;
  BOOLEAN                        LeadZero;
  UINT8                          LeadZeroCnt;
  UINT8                          Cnt;

  if ((String == NULL) || (Ip6Address == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Ip6Str      = (CHAR8 *) String;
  AllowedCnt  = 6;
  LeadZeroCnt = 0;

  //
  // An IPv6 address leading with : looks strange.
  //
  if (*Ip6Str == ':') {
    if (*(Ip6Str + 1) != ':') {
      return EFI_INVALID_PARAMETER;
    } else {
      AllowedCnt = 7;
    }
  }

  ZeroMem (Ip6Address, sizeof (EFI_IPv6_ADDRESS));

  NodeCnt     = 0;
  TailNodeCnt = 0;
  Short       = FALSE;
  Update      = FALSE;
  LeadZero    = FALSE;

  for (Index = 0; Index < 15; Index = (UINT8) (Index + 2)) {
    TempStr = Ip6Str;

    while ((*Ip6Str != '\0') && (*Ip6Str != ':')) {
      Ip6Str++;
    }

    if ((*Ip6Str == '\0') && (Index != 14)) {
      return EFI_INVALID_PARAMETER;
    }

    if (*Ip6Str == ':') {
      if (*(Ip6Str + 1) == ':') {
        if ((NodeCnt > 6) || 
            ((*(Ip6Str + 2) != '\0') && (AsciiStrHexToUintn (Ip6Str + 2) == 0))) {
          //
          // ::0 looks strange. report error to user.
          //
          return EFI_INVALID_PARAMETER;
        }
        if ((NodeCnt == 6) && (*(Ip6Str + 2) != '\0') && 
            (AsciiStrHexToUintn (Ip6Str + 2) != 0)) {
          return EFI_INVALID_PARAMETER;
        }

        //
        // Skip the abbreviation part of IPv6 address.
        //
        TempStr2 = Ip6Str + 2;
        while ((*TempStr2 != '\0')) {
          if (*TempStr2 == ':') {
            if (*(TempStr2 + 1) == ':') {
              //
              // :: can only appear once in IPv6 address.
              //
              return EFI_INVALID_PARAMETER;
            }

            TailNodeCnt++;
            if (TailNodeCnt >= (AllowedCnt - NodeCnt)) {
              //
              // :: indicates one or more groups of 16 bits of zeros.
              //
              return EFI_INVALID_PARAMETER;
            }
          }

          TempStr2++;
        }

        Short  = TRUE;
        Update = TRUE;

        Ip6Str = Ip6Str + 2;
      } else {
        if (*(Ip6Str + 1) == '\0') {
          return EFI_INVALID_PARAMETER;
        }
        Ip6Str++;
        NodeCnt++;
        if ((Short && (NodeCnt > 6)) || (!Short && (NodeCnt > 7))) {
          //
          // There are more than 8 groups of 16 bits of zeros.
          //
          return EFI_INVALID_PARAMETER;
        }
      }
    }

    //
    // Convert the string to IPv6 address. AsciiStrHexToUintn stops at the first
    // character that is not a valid hexadecimal character, ':' or '\0' here.
    //
    NodeVal = AsciiStrHexToUintn (TempStr);
    if ((NodeVal > 0xFFFF) || (Index > 14)) {
      return EFI_INVALID_PARAMETER;
    }
    if (NodeVal != 0) {
      if ((*TempStr  == '0') && 
          ((*(TempStr + 2) == ':') || (*(TempStr + 3) == ':') || 
          (*(TempStr + 2) == '\0') || (*(TempStr + 3) == '\0'))) {
        return EFI_INVALID_PARAMETER;
      }
      if ((*TempStr  == '0') && (*(TempStr + 4) != '\0') && 
          (*(TempStr + 4) != ':')) { 
        return EFI_INVALID_PARAMETER;
      }
    } else {
      if (((*TempStr  == '0') && (*(TempStr + 1) == '0') && 
          ((*(TempStr + 2) == ':') || (*(TempStr + 2) == '\0'))) ||
          ((*TempStr  == '0') && (*(TempStr + 1) == '0') && (*(TempStr + 2) == '0') && 
          ((*(TempStr + 3) == ':') || (*(TempStr + 3) == '\0')))) {
        return EFI_INVALID_PARAMETER;
      }
    }

    Cnt = 0;
    while ((TempStr[Cnt] != ':') && (TempStr[Cnt] != '\0')) {
      Cnt++; 
    }
    if (LeadZeroCnt == 0) {
      if ((Cnt == 4) && (*TempStr  == '0')) {
        LeadZero = TRUE;
        LeadZeroCnt++;
      }
      if ((Cnt != 0) && (Cnt < 4)) {
        LeadZero = FALSE;
        LeadZeroCnt++;
      }
    } else {
      if ((Cnt == 4) && (*TempStr  == '0') && !LeadZero) {
        return EFI_INVALID_PARAMETER;
      }
      if ((Cnt != 0) && (Cnt < 4) && LeadZero) {
        return EFI_INVALID_PARAMETER;
      }
    } 

    Ip6Address->Addr[Index] = (UINT8) (NodeVal >> 8);
    Ip6Address->Addr[Index + 1] = (UINT8) (NodeVal & 0xFF);

    //
    // Skip the groups of zeros by ::
    //
    if (Short && Update) {
      Index  = (UINT8) (16 - (TailNodeCnt + 2) * 2);
      Update = FALSE;
    }
  }

  if ((!Short && Index != 16) || (*Ip6Str != '\0')) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

/**
  Convert one Null-terminated Unicode string to Ipv6 address of 16 bytes.

  @param[in]      String         The pointer to the Unicode string.
  @param[out]     Ip6Address     The pointer to the converted IPv6 address.

  @retval EFI_SUCCESS            Converted IPv6 address successfully.
  @retval EFI_INVALID_PARAMETER  The string is mal-formated or Ip6Address is NULL.
  @retval EFI_OUT_OF_RESOURCES   Fail to perform the operation due to lack of resource.

**/

EFI_STATUS
EFIAPI
ConvertStrToBmcIp6Address (
  IN CONST CHAR16                *String,
  OUT      IPMI_IPV6_ADDRESS      *Ip6Address
  )
{
  CHAR8                          *Ip6Str;
  EFI_STATUS                     Status;

  if ((String == NULL) || (Ip6Address == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Ip6Str = (CHAR8 *) AllocatePool ((StrLen (String) + 1) * sizeof (CHAR8));
  if (Ip6Str == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  UnicodeStrToAsciiStr (String, Ip6Str);

  Status = ConvertAsciiStrToBmcIp6 (Ip6Str, Ip6Address);

  FreePool (Ip6Str);

  return Status;
}

/**
    Checks for Set-In Progress Bit and Wait to get it Clear

    @param  LanChannelNumber - Channel Number of LAN

    @return EFI_STATUS
    @retval  EFI_SUCCESS - Set-In Progress Bit is Cleared
    @retval  EFI_TIMEOUT - Specified Time out over
    
**/

EFI_STATUS
IpmiWaitSetInProgressClear (
   UINT8 LanChannelNumber ) 
{
    EFI_STATUS                      Status;
    UINT8                           ResponseSize;
    UINT8                           Retries = 10;
    EFI_IPMI_GET_LAN_CONFIG_PRAM    GetLanParamCmd;
    EFI_GET_LAN_CONFIG_PARAM0       GetSetInProgResponse;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry...\n", __FUNCTION__));

    GetLanParamCmd.LanChannel.ChannelNumber = LanChannelNumber;
    GetLanParamCmd.LanChannel.Reserved = 0;
    GetLanParamCmd.LanChannel.GetParam = 0; 
    GetLanParamCmd.ParameterSelector = 0;       
    GetLanParamCmd.SetSelector = 0;
    GetLanParamCmd.BlockSelector = 0;

    //
    // Wait for Set-In progress bit to clear
    //
    do {
        ResponseSize = sizeof (GetSetInProgResponse);
        Status = gIpmiTransport->SendIpmiCommand (
                     gIpmiTransport,
                     IPMI_NETFN_TRANSPORT,
                     BMC_LUN,
                     IPMI_TRANSPORT_GET_LAN_CONFIG_PARAMETERS,
                     (UINT8*) &GetLanParamCmd,
                     sizeof (GetLanParamCmd),
                     (UINT8*) &GetSetInProgResponse,
                     &ResponseSize );

        if (EFI_ERROR (Status)) {
            return Status;
        }

        if (GetSetInProgResponse.Param0.SetInProgress == 0) {
            break;
        }
        gBS->Stall (IPMI_STALL);
    } while (Retries-- > 0);

    if (Retries == 0) {
        return EFI_TIMEOUT;
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));
    return EFI_SUCCESS;
}

/**
    Set the LAN IPV6 address source values from BIOS setup to BMC

    @param LanChannelNumber - LAN channel to use

    @param IsStaticSource - Selection of source as static or dynamic.

    @return EFI_STATUS

**/

EFI_STATUS
SetIp6AddressSource (
  IN  UINT8      LanChannelNumber,
  IN  UINT8      IsStaticSource )
{
    EFI_STATUS                    Status;
    EFI_IPMI_GET_LAN_CONFIG_PRAM  GetParamCommand;
    EFI_IPMI_SET_IPV6_ADDRESS     SetParamCommand;
    EFI_IPMI_GET_IPV6_ADDRESS     Ipv6AddressData;
    UINT8                         DataSize;
    UINT8                         ResponseDataSize = 0;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entered... \n", __FUNCTION__));

    //
    //Get IPV6 address
    //
    GetParamCommand.LanChannel.ChannelNumber = LanChannelNumber;
    GetParamCommand.LanChannel.GetParam = 0;
    GetParamCommand.LanChannel.Reserved = 0;
    GetParamCommand.ParameterSelector = EfiIpv6StaticAddress;
    GetParamCommand.BlockSelector = 0;
    GetParamCommand.SetSelector = 0;
    DataSize = sizeof (Ipv6AddressData);

    //
    // Wait until Set In Progress field is cleared
    //
    Status = IpmiWaitSetInProgressClear (LanChannelNumber);
    if (EFI_ERROR(Status)) {
       return Status;
    }

    Status = gIpmiTransport->SendIpmiCommand (
                gIpmiTransport,
                IPMI_NETFN_TRANSPORT,
                BMC_LUN,
                IPMI_TRANSPORT_GET_LAN_CONFIG_PARAMETERS,
                (UINT8*)&GetParamCommand,
                sizeof (GetParamCommand),
                (UINT8*)&Ipv6AddressData,
                &DataSize );

    SERVER_IPMI_DEBUG ((DEBUG_INFO, " IPMI : IPMI_TRANSPORT_GET_LAN_CONFIG_PARAMETERS Status: %r\n", Status));

    if (EFI_ERROR(Status)) {
        return Status;
    }

    SetParamCommand.LanChannel.ChannelNumber = LanChannelNumber;
    SetParamCommand.LanChannel.GetParam = 0;
    SetParamCommand.LanChannel.Reserved = 0;
    SetParamCommand.Param = EfiIpv6StaticAddress;
    SetParamCommand.Ip6AddressParam.SetSelector = Ipv6AddressData.Ip6AddressParam.SetSelector;

    if (IsStaticSource) {
        SetParamCommand.Ip6AddressParam.AddressSource.EnableStat = IsStaticSource;
        SetParamCommand.Ip6AddressParam.AddressSource.Reserved = 0;
        SetParamCommand.Ip6AddressParam.AddressSource.Type = 0;
    } else if (!IsStaticSource) {
        SetParamCommand.Ip6AddressParam.AddressSource.EnableStat = IsStaticSource;
        SetParamCommand.Ip6AddressParam.AddressSource.Reserved = 0;
        SetParamCommand.Ip6AddressParam.AddressSource.Type = 0;
    }

    CopyMem (SetParamCommand.Ip6AddressParam.Ipv6Address.Addr, Ipv6AddressData.Ip6AddressParam.Ipv6Address.Addr, 16);
    SetParamCommand.Ip6AddressParam.AddressPrefixLen = Ipv6AddressData.Ip6AddressParam.AddressPrefixLen;

    //According to latest IPMI spec(IPMI v2.0/v1.5 errata document revision 7),
    //"data 20 - Address Status (Read-only parameter. Writes to this location are ignored.)"
    //so BMC firmware with older spec revision of IPMI might not support it. 
    DataSize = sizeof (SetParamCommand); //Supports for latest BMC firmware 

    //
    // Wait until Set In Progress field is cleared
    //
    Status = IpmiWaitSetInProgressClear (LanChannelNumber);
    if (EFI_ERROR(Status)) {
       return Status;
    }

    Status = gIpmiTransport->SendIpmiCommand (
                gIpmiTransport,
                IPMI_NETFN_TRANSPORT,
                BMC_LUN,
                IPMI_TRANSPORT_SET_LAN_CONFIG_PARAMETERS,
                (UINT8*)&SetParamCommand,
                DataSize,
                NULL,
                &ResponseDataSize );

    SERVER_IPMI_DEBUG ((DEBUG_INFO, " IPMI: IPMI_TRANSPORT_SET_LAN_CONFIG_PARAMETERS Status: %r\n", Status));

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));

    return Status;
}

/**
    Configure the LAN IPV6 Dynamic Router address source values from BIOS setup to BMC

    @param LanChannelNumber - LAN channel to use

    @return EFI_STATUS

**/

EFI_STATUS
SetIp6RouterAddressSource (
  IN  UINT8      LanChannelNumber )
{
    EFI_STATUS                               Status;
    EFI_IPMI_SET_IPV6_ROUTER_CONFIG          SetIpv6RouterConfig;
    UINT8                                    DataSize;
    UINT8                                    ResponseDataSize = 0;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entered... \n", __FUNCTION__));
    
    //
    //Set Router IP address
    //
    SetIpv6RouterConfig.LanChannel.ChannelNumber = LanChannelNumber;
    SetIpv6RouterConfig.LanChannel.GetParam = 0;
    SetIpv6RouterConfig.LanChannel.Reserved = 0;
    SetIpv6RouterConfig.Param = EfiIpv6RouterConfig;
    SetIpv6RouterConfig.Data = 2;
    DataSize = sizeof (SetIpv6RouterConfig);
    //
    // Wait until Set In Progress field is cleared
    //
    Status = IpmiWaitSetInProgressClear (LanChannelNumber);
    if (EFI_ERROR(Status)) {
       return Status;
    }

    Status = gIpmiTransport->SendIpmiCommand (
                gIpmiTransport,
                IPMI_NETFN_TRANSPORT,
                BMC_LUN,
                IPMI_TRANSPORT_SET_LAN_CONFIG_PARAMETERS,
                (UINT8*)&SetIpv6RouterConfig,
                DataSize,
                NULL,
                &ResponseDataSize );

    SERVER_IPMI_DEBUG ((DEBUG_INFO, " IPMI: IPMI_TRANSPORT_SET_LAN_CONFIG_PARAMETERS Status: %r, %x\n", Status, gIpmiTransport->CommandCompletionCode));

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));

    return Status;
}

/**
    Set the LAN IPV6 address source values from BIOS setup to BMC

    @param LanChannelNumber - LAN channel to use
    
    @Param Ipv6 Address - Setting the Ipv6 Address.
    
    @param PrefixLength - Ipv6 Address Prefix length

    @return EFI_STATUS

**/
EFI_STATUS
SetIp6StaticAddressParam (
  IN UINT8              LanChannelNumber,
  IN IPMI_IPV6_ADDRESS  Ip6Addr,
  IN  UINT8             PrefixLength
  )
{

    EFI_STATUS                     Status;
    EFI_IPMI_SET_IPV6_ADDRESS      SetParamCommand;
    UINT8                          DataSize;
    UINT8                          ResponseDataSize = 0;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry...\n", __FUNCTION__));

    SetParamCommand.LanChannel.ChannelNumber = LanChannelNumber;
    SetParamCommand.LanChannel.GetParam = 0;
    SetParamCommand.LanChannel.Reserved = 0;
    SetParamCommand.Param = EfiIpv6StaticAddress;
    SetParamCommand.Ip6AddressParam.SetSelector = 0;

    SetParamCommand.Ip6AddressParam.AddressSource.EnableStat = 1;
    SetParamCommand.Ip6AddressParam.AddressSource.Reserved = 0;
    SetParamCommand.Ip6AddressParam.AddressSource.Type = 0;

    CopyMem ( SetParamCommand.Ip6AddressParam.Ipv6Address.Addr, Ip6Addr.Addr, 16);
    SetParamCommand.Ip6AddressParam.AddressPrefixLen = PrefixLength;
    
    //According to latest IPMI spec(IPMI v2.0/v1.5 errata document revision 7),
    //"data 20 - Address Status (Read-only parameter. Writes to this location are ignored.)"
    //so BMC firmware with older spec revision of IPMI might not support it. 
    DataSize = sizeof (SetParamCommand); //Supports for latest BMC firmware 

    //
    // Wait until Set In Progress field is cleared
    //
    Status = IpmiWaitSetInProgressClear (LanChannelNumber);
    if (EFI_ERROR(Status)) {
       return Status;
    }

    Status = gIpmiTransport->SendIpmiCommand (
                gIpmiTransport,
                IPMI_NETFN_TRANSPORT,
                BMC_LUN,
                IPMI_TRANSPORT_SET_LAN_CONFIG_PARAMETERS,
                (UINT8*)&SetParamCommand,
                DataSize,
                NULL,
                &ResponseDataSize );

    SERVER_IPMI_DEBUG ((DEBUG_INFO, " IPMI: IPMI_TRANSPORT_SET_LAN_CONFIG_PARAMETERS Status: %r\n", Status));

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Exiting...\n", __FUNCTION__));

    return Status;

}

/**
    Set the LAN IPV6 static Router address source values from BIOS setup to BMC

    @param LanChannelNumber - LAN channel to use

    @Param Ipv6 Address - Setting the Ipv6 Router IP Address.

    @return EFI_STATUS

**/

EFI_STATUS
SetIp6StaticRouterAddressParam (
  IN  UINT8             LanChannelNumber,
  IN  IPMI_IPV6_ADDRESS Ip6Addr )
{
    EFI_STATUS                               Status;
    EFI_IPMI_SET_IPV6_ROUTER_CONFIG          SetIpv6RouterConfig;
    EFI_IPMI_SET_IPV6_ROUTER1_IP_ADDRESS     SetIpv6RouterAddress;
    UINT8                                    DataSize;
    UINT8                                    ResponseDataSize = 0;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry... \n", __FUNCTION__));
    
    //
    // Configure Static Router IP
    //
    SetIpv6RouterConfig.LanChannel.ChannelNumber = LanChannelNumber;
    SetIpv6RouterConfig.LanChannel.GetParam = 0;
    SetIpv6RouterConfig.LanChannel.Reserved = 0;
    SetIpv6RouterConfig.Param = EfiIpv6RouterConfig;
    SetIpv6RouterConfig.Data = 1;
    DataSize = sizeof (SetIpv6RouterConfig);
    //
    // Wait until Set In Progress field is cleared
    //
    Status = IpmiWaitSetInProgressClear (LanChannelNumber);
    if (EFI_ERROR(Status)) {
       return Status;
    }

    Status = gIpmiTransport->SendIpmiCommand (
                gIpmiTransport,
                IPMI_NETFN_TRANSPORT,
                BMC_LUN,
                IPMI_TRANSPORT_SET_LAN_CONFIG_PARAMETERS,
                (UINT8*)&SetIpv6RouterConfig,
                DataSize,
                NULL,
                &ResponseDataSize );

    SERVER_IPMI_DEBUG ((DEBUG_INFO, " IPMI: IPMI_TRANSPORT_SET_LAN_CONFIG_PARAMETERS Status: %r, %x\n", Status, gIpmiTransport->CommandCompletionCode));
    
    //
    //Set Static Router IP address
    //
    SetIpv6RouterAddress.LanChannel.ChannelNumber = LanChannelNumber;
    SetIpv6RouterAddress.LanChannel.GetParam = 0;
    SetIpv6RouterAddress.LanChannel.Reserved = 0;
    SetIpv6RouterAddress.Param = EfiIpv6StaticRouter1IpAddr;

    CopyMem ( SetIpv6RouterAddress.Ip6AddressParam.Ipv6Address.Addr, Ip6Addr.Addr, 16 );    
    DataSize = sizeof (SetIpv6RouterAddress);

    //
    // Wait until Set In Progress field is cleared
    //
    Status = IpmiWaitSetInProgressClear (LanChannelNumber);
    if (EFI_ERROR(Status)) {
       return Status;
    }

    Status = gIpmiTransport->SendIpmiCommand (
                gIpmiTransport,
                IPMI_NETFN_TRANSPORT,
                BMC_LUN,
                IPMI_TRANSPORT_SET_LAN_CONFIG_PARAMETERS,
                (UINT8*)&SetIpv6RouterAddress,
                DataSize,
                NULL,
                &ResponseDataSize );

    SERVER_IPMI_DEBUG ((DEBUG_INFO, " IPMI: IPMI_TRANSPORT_SET_LAN_CONFIG_PARAMETERS Status: %r, %x\n", Status, gIpmiTransport->CommandCompletionCode));

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));

    return Status;
}

/**
    Checks in the LAN IPV6 addressing is supported or not. If support disabled means, enable the Ipv6 support.

    @param LanChannelNumber - LAN channel to use

    @return EFI_STATUS

**/

EFI_STATUS
GetIp6Supported (
  IN  UINT8      LanChannelNumber )
{
    EFI_STATUS                                Status;
    EFI_IPMI_GET_LAN_CONFIG_PRAM              CommandData;
    EFI_GET_LAN_CONFIG_IP_4_6_SUPPORT_ENABLE  ResponseData;
    UINT8                                     ResponseDataSize;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entered.... \n", __FUNCTION__));

    //
    //Get Ipv6 support details
    //
    CommandData.LanChannel.ChannelNumber = LanChannelNumber;
    CommandData.LanChannel.GetParam = 0;
    CommandData.LanChannel.Reserved = 0;
    CommandData.ParameterSelector = EfiIpv4OrIpv6AddressEnable;
    CommandData.SetSelector = 0;
    CommandData.BlockSelector = 0;
    ResponseDataSize = sizeof(ResponseData);

    //
    // Wait until Set In Progress field is cleared
    //
    Status = IpmiWaitSetInProgressClear (LanChannelNumber);
    if (EFI_ERROR (Status)) {
        return Status;
    }

    Status = gIpmiTransport->SendIpmiCommand (
                gIpmiTransport,
                IPMI_NETFN_TRANSPORT,
                BMC_LUN,
                IPMI_TRANSPORT_GET_LAN_CONFIG_PARAMETERS,
                (UINT8*)&CommandData,
                sizeof(CommandData),
                (UINT8*)&ResponseData,
                &ResponseDataSize );

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI : IPMI_TRANSPORT_GET_LAN_CONFIG_PARAMETERS Status: %r\n", Status));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI : LanChannelNumber: %x Completion Code: %x\n", LanChannelNumber, gIpmiTransport->CommandCompletionCode));

    if (!EFI_ERROR(Status) && (gIpmiTransport->CommandCompletionCode == 0x80)) {
       return EFI_UNSUPPORTED;
    }

    if (!EFI_ERROR(Status)) {
        if ( ResponseData.Data1 == EFI_BMC_IPV6_DIS) {
            SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI : Ipv6 Support Disabled \n"));
            return EFI_UNSUPPORTED;
        }
    }
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Exiting...\n", __FUNCTION__));

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
