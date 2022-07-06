//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#ifndef _AMI_REDFISH_VLAN_LIB_H_
#define _AMI_REDFISH_VLAN_LIB_H_

#define REDFISH_VLAN_CONFIG_PROTOCOL_V1 1
#define MAX_MAC_STRING_LEN  96

typedef struct _REDFISH_VLAN_CONFIG_DATA REDFISH_VLAN_CONFIG_DATA;
typedef struct _EFI_REDFISH_VLAN_CONFIG_PROTOCOL EFI_REDFISH_VLAN_CONFIG_PROTOCOL;

#pragma pack(1)

struct _REDFISH_VLAN_CONFIG_DATA {
    EFI_MAC_ADDRESS     MacAddress;
    UINT16              VLANID;
    UINT8               VLANEnable;
};

#pragma pack()

extern REDFISH_VLAN_CONFIG_DATA  *gRedfishVlanData;
UINTN  gNumberOfRedfishVlan;

/**
  Function to set the redfish vlan data to global variable.

  @param[in]    This            Redfish Vlan Protocol.
  @param[in]    VLANConfigData  Buffer which contains the array of redfish vlan config data.          
  @param[in]    DataSize		Size of the buffer which contains the array of redfish vlan config data.          

  @retval EFI_SUCCESS           Redfish Vlan Data set Successfully.
  @retval Others                Fail to set Redfish Vlan Data.

**/
typedef
EFI_STATUS        
(EFIAPI *EFI_REDFISH_VLAN_CONFIG_SET_DATA) (
 IN EFI_REDFISH_VLAN_CONFIG_PROTOCOL    *This,
 IN REDFISH_VLAN_CONFIG_DATA            *VLANConfigData,
 IN UINTN                               DataSize
);

/**
  Function to set the redfish vlan data to global variable.

  @param[in]    This            Redfish Vlan Protocol.
  @param[in]    VLANConfigData  Buffer which contains the array of redfish vlan config data.          
  @param[in]    DataSize		Size of the buffer which contains the array of redfish vlan config data.          

  @retval EFI_SUCCESS           Redfish Vlan Data set Successfully.
  @retval Others                Fail to set Redfish Vlan Data.

**/
EFI_STATUS
EFIAPI
EfiRedfishVlanConfigSetData (
 IN EFI_REDFISH_VLAN_CONFIG_PROTOCOL    *This,
 IN REDFISH_VLAN_CONFIG_DATA            *VLANConfigData,
 IN UINTN                               DataSize
 );
 
/**
  Function to check the vlanid passed is available in the Redfish global variable.

  @param[in]    RedfishVlanData  Buffer which contains the array of redfish vlan config data.          
  @param[in]    VlanId           Vlan Id to be checked.

  @retval  TRUE           	Vlan id is available in the array.
  @retval  FALSE		 	Vlan Id is not available in the array.

**/

BOOLEAN
CheckRedfishVlanData (
    IN REDFISH_VLAN_CONFIG_DATA                 *RedfishVlanData,
    IN UINT16                                   VlanId
 );

/**
  Function to get the redfish vlan data available in the global variable.

  @param[in] 	 This  				Buffer which contains the array of redfish vlan config data.          
  @param[out]    VLANConfigData     Vlan Id to be checked.
  @param[out]    DataSize           Vlan Id to be checked.
  
  @retval EFI_SUCCESS           Redfish Vlan Data obtained Successfully.
  @retval Others                Failed to get Redfish Vlan Data.

**/
EFI_STATUS
EFIAPI
EfiRedfishVlanConfigGetData (
 IN EFI_REDFISH_VLAN_CONFIG_PROTOCOL      *This,  
 OUT REDFISH_VLAN_CONFIG_DATA             **VLANConfigData,
 OUT UINTN                                *DataSize
 );
 
/**
  Function to get the redfish vlan data available in the global variable.

  @param[in] 	 This  				Buffer which contains the array of redfish vlan config data.          
  @param[out]    VLANConfigData     Vlan Id to be checked.
  @param[out]    DataSize           Vlan Id to be checked.
  
  @retval EFI_SUCCESS           Redfish Vlan Data obtained Successfully.
  @retval Others                Failed to get Redfish Vlan Data.

**/

typedef
EFI_STATUS
(EFIAPI *EFI_REDFISH_VLAN_CONFIG_GET_DATA) (
        IN EFI_REDFISH_VLAN_CONFIG_PROTOCOL      *This,  
        OUT REDFISH_VLAN_CONFIG_DATA             **VLANConfigData,
        OUT UINTN                                *DataSize
     ) ;

/**
  Function to install RedfishVlanConfigProtocol .

  @param[in]    ImageHandle           Handle of device to bind driver to.

  @retval EFI_SUCCESS           RedfishVlanConfigProtocol Installed Successfully.
  @retval Others                Fail to install RedfishVlanConfigProtocol.

**/
EFI_STATUS
RedfishVlanConfigInit (
  IN EFI_HANDLE  ImageHandle
  );

struct _EFI_REDFISH_VLAN_CONFIG_PROTOCOL {
     UINTN                                      Version;
     EFI_REDFISH_VLAN_CONFIG_SET_DATA           SetData;
     EFI_REDFISH_VLAN_CONFIG_GET_DATA           GetData;
 };

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
  );

#endif

