//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#define MAX_VLAN_VALUE    4095
#define MIN_VLAN_VALUE    0

#include <Library/AMIRedFishVlan.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Include/Base.h>


GLOBAL_REMOVE_IF_UNREFERENCED CONST CHAR8  HexString[] = "0123456789ABCDEFabcdef";


//Initialize the redfish vlan form
REDFISH_VLAN_CONFIG_DATA  *gRedfishVlanData;

/**
  Function to install RedfishVlanConfigProtocol .

  @param[in]    ImageHandle           Handle of device to bind driver to.

  @retval EFI_SUCCESS           RedfishVlanConfigProtocol Installed Successfully.
  @retval Others                Fail to install RedfishVlanConfigProtocol.

**/
EFI_STATUS
RedfishVlanConfigInit (
  IN EFI_HANDLE  ImageHandle
  )
{
    EFI_STATUS Status;
    EFI_REDFISH_VLAN_CONFIG_PROTOCOL *RedfishVlanConfig;
    
    DEBUG((DEBUG_INFO,"\nIn %a ENTRY", __FUNCTION__));
    
    RedfishVlanConfig = AllocateZeroPool( sizeof(EFI_REDFISH_VLAN_CONFIG_PROTOCOL) );
    
    RedfishVlanConfig->Version = REDFISH_VLAN_CONFIG_PROTOCOL_V1;
    RedfishVlanConfig->SetData = EfiRedfishVlanConfigSetData;
    RedfishVlanConfig->GetData = EfiRedfishVlanConfigGetData;
    
	//
	//Installing the Redfish Vlan Config protocol.
	//
	
    Status = gBS->InstallMultipleProtocolInterfaces (
                      &ImageHandle,
                      &gEfiRedfishVlanConfigProtocolGuid,
                      RedfishVlanConfig,
                      NULL
                      );

    DEBUG((DEBUG_INFO,"\nIn %a, InstallMultipleProtocolInterfaces Status :: %r", __FUNCTION__, Status));
    
    return Status;
}

/**
  Function to set the redfish vlan data to global variable.

  @param[in]    This            Redfish Vlan Protocol.
  @param[in]    VLANConfigData  Buffer which contains the array of redfish vlan config data.          
  @param[in]    DataSize        Size of the buffer which contains the array of redfish vlan config data.          

  @retval EFI_SUCCESS           Redfish Vlan Data set Successfully.
  @retval Others                Fail to set Redfish Vlan Data.

**/

EFI_STATUS
EFIAPI
EfiRedfishVlanConfigSetData (
 IN EFI_REDFISH_VLAN_CONFIG_PROTOCOL    *This,
 IN REDFISH_VLAN_CONFIG_DATA            *VLANConfigData,
 IN UINTN                               DataSize
 )
{
    EFI_STATUS                          Status = EFI_SUCCESS;
    UINTN                               Index = 0;
    UINTN                               Index2 = 0;
    UINTN                               count = 0;
    UINT16                              TempVlanId ;
 
    gNumberOfRedfishVlan = DataSize /(UINTN)sizeof(REDFISH_VLAN_CONFIG_DATA);
    gRedfishVlanData = AllocatePool(DataSize);
	
	//Copying the Redfish Vlan Data to global buffer excludes duplicate entries
	
    for(Index = 0 ; Index < gNumberOfRedfishVlan ; Index++)
    {
        TempVlanId = VLANConfigData[Index].VLANID;
        if(TempVlanId < MIN_VLAN_VALUE || TempVlanId > MAX_VLAN_VALUE)
        {
            return EFI_INVALID_PARAMETER;
        }
        for( Index2 = 0 ; Index2 < count ; Index2++)
        {
            if( TempVlanId == gRedfishVlanData[Index2].VLANID)              //To check the duplicates in the redfish vlan data.
            {
                goto ON_EXIT;
            }
        }
        CopyMem (&gRedfishVlanData[count], &VLANConfigData[Index], sizeof(REDFISH_VLAN_CONFIG_DATA));
        count++;
     ON_EXIT:
     DEBUG ((DEBUG_INFO, "AMIRedFishVlan : duplicate vlan data\n"));
    }
    return Status;
} 

/**
  Function to check the vlanid passed is available in the Redfish global variable.

  @param[in]    RedfishVlanData  Buffer which contains the array of redfish vlan config data.          
  @param[in]    VlanId           Vlan Id to be checked.

  @retval  TRUE             Vlan id is available in the array.
  @retval  FALSE            Vlan Id is not available in the array.

**/

BOOLEAN
CheckRedfishVlanData (
        IN REDFISH_VLAN_CONFIG_DATA                 *RedfishVlanData,
        IN UINT16                                    VlanId
 )
{
    UINTN  Index = 0; 
    for(Index = 0 ; Index < gNumberOfRedfishVlan ; Index++)
    {
        if(VlanId == RedfishVlanData[Index].VLANID)
        {
            return TRUE;
        }
    }   
    return FALSE;
} 

/**
  Function to get the redfish vlan data available in the global variable.

  @param[in]     This               Buffer which contains the array of redfish vlan config data.          
  @param[out]    VLANConfigData     Vlan Id to be checked.
  @param[out]    DataSize           Vlan Id to be checked.
  
  @retval EFI_SUCCESS           Redfish Vlan Data obtained Successfully.
  @retval Others                Failed to get Redfish Vlan Data.

**/

EFI_STATUS
EFIAPI
EfiRedfishVlanConfigGetData (
 IN EFI_REDFISH_VLAN_CONFIG_PROTOCOL     *This,         
 OUT REDFISH_VLAN_CONFIG_DATA            **VLANConfigData,
 OUT UINTN                               *DataSize
 )
{ 
    *DataSize =  gNumberOfRedfishVlan;
    if ( *DataSize == 0 )
    {
        return EFI_UNSUPPORTED;
    }
    *VLANConfigData = AllocatePool(gNumberOfRedfishVlan * sizeof(REDFISH_VLAN_CONFIG_DATA));
    CopyMem (*VLANConfigData, gRedfishVlanData, gNumberOfRedfishVlan * sizeof(REDFISH_VLAN_CONFIG_DATA));
    return EFI_SUCCESS;    
} 

/**
  Convert the mac address into a hexadecimal encoded "-" seperated string.

  @param[in]  Mac     The mac address.
  @param[in]  Len     Length in bytes of the mac address.
  @param[in]  VlanId  VLAN ID of the network device.
  @param[out] Str     The storage to return the mac string.

**/
VOID
MacAddrToStr (
  IN  EFI_MAC_ADDRESS  *Mac,
  IN  UINT32           Len,
  IN  UINT16           VlanId,
  OUT CHAR16           *Str
  )
{
  UINT32  Index;
  CHAR16  *String;

  for (Index = 0; Index < Len; Index++) {
    Str[3 * Index]      = (CHAR16) HexString[(Mac->Addr[Index] >> 4) & 0x0F];
    Str[3 * Index + 1]  = (CHAR16) HexString[Mac->Addr[Index] & 0x0F];
    Str[3 * Index + 2]  = L':';
  }

  String = &Str[3 * Index - 1] ;
  if (VlanId != 0) {
    String += UnicodeSPrint (String, 6 * sizeof (CHAR16), L"\\%04x", (UINTN) VlanId);
  }

  *String = L'\0';
}
