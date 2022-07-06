//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <Token.h>
#if(AMIRedFishVlan_SUPPORT)
#define VLANSTR_SIZE   100
#include "VlanConfigImpl.h"
#include <Library/AMIRedFishVlan.h>
#include <Library/AmiNetworkPostManagerLib.h>
GLOBAL_REMOVE_IF_UNREFERENCED CONST CHAR8  HexString[] = "0123456789ABCDEFabcdef";
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

/**
  This function is to find mac of matched Redfish Vlan Data 

  @param[in]  RedfishMacString     MacString available in .
  
  @retval TRUE            		The MAC in redfish data match found.
  @retval FALSE                 The MAC in redfish data match not found.

**/

BOOLEAN IsValidMac(CHAR16 RedfishMacString [MAX_MAC_STRING_LEN])
{
    CHAR16                    MacString [MAX_MAC_STRING_LEN];
    EFI_HANDLE                    *HandleBuffer;
    EFI_SIMPLE_NETWORK_PROTOCOL     *Snp;
    EFI_STATUS                Status;
    UINTN                     HandleCount, Index;
    
    Status  = gBS->LocateHandleBuffer (ByProtocol,
            &gEfiSimpleNetworkProtocolGuid,
            NULL,
            &HandleCount,
            &HandleBuffer);
    if(EFI_ERROR(Status))
    {
        return FALSE;
    }
    for (Index = 0; Index < HandleCount; Index++) 
    {        
        Status = gBS->HandleProtocol (HandleBuffer[Index],
                &gEfiSimpleNetworkProtocolGuid,
                (VOID **) &Snp
        );
        if(EFI_ERROR(Status))
        {
            continue;
        }       
        MacAddrToStr (&Snp->Mode->PermanentAddress,6,0,MacString);
        if(!StrCmp(MacString, RedfishMacString))
        {
			FreePool(HandleBuffer);
            return TRUE;
        }
    } 
    if(HandleBuffer!=NULL)
    {
        FreePool(HandleBuffer);
    }
    return FALSE;
}

/**
  This function is to display the Redfish Vlan Data

  @param[in]  Private            Points to VLAN configuration private data.
  
  @retval EFI_SUCCESS            The redfish data displayed successfully.
  @retval Others                 failed to display the redfish data.

**/

EFI_STATUS DisplayRedfishVlanData(VLAN_CONFIG_PRIVATE_DATA  *Private)
{
    EFI_STATUS                Status;
    UINTN                     NumberOfRedfishVlan;
    UINTN                     Index;
    VOID                      *StartOpCodeHandle;
    EFI_IFR_GUID_LABEL        *StartLabel;
    VOID                      *EndOpCodeHandle;
    EFI_IFR_GUID_LABEL        *EndLabel;
    UINT16                    VlanDataId = 0;
    CHAR16                    String[255] ;    
    CHAR16                    *VlanStr;
    CHAR16                    RedfishMacString [MAX_MAC_STRING_LEN];
    REDFISH_VLAN_CONFIG_DATA  *RedfishVlanData = NULL;
    EFI_REDFISH_VLAN_CONFIG_PROTOCOL *RedfishVlanconfigProtocol;
    CHAR16    *BufString = NULL;
	//	Locates the Redfish Vlan Config Protocol.
    Status = gBS->LocateProtocol( &gEfiRedfishVlanConfigProtocolGuid, NULL, (void**)&RedfishVlanconfigProtocol);
    if(EFI_ERROR(Status))
    {
        return Status;
    }
	//	Get the Redfish Vlan Data.
    Status = RedfishVlanconfigProtocol->GetData(RedfishVlanconfigProtocol,&RedfishVlanData, &NumberOfRedfishVlan);
    if(EFI_ERROR(Status))
    {
        return Status;
    }
    //
    // Init OpCode Handle
    //
    StartOpCodeHandle = HiiAllocateOpCodeHandle ();
    ASSERT (StartOpCodeHandle != NULL);

    EndOpCodeHandle = HiiAllocateOpCodeHandle ();
    ASSERT (EndOpCodeHandle != NULL);

    //
    // Create Hii Extend Label OpCode as the start opcode
    //
    StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                                          StartOpCodeHandle,
                                          &gEfiIfrTianoGuid,
                                          NULL,
                                          sizeof (EFI_IFR_GUID_LABEL)
                                          );
    StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
    StartLabel->Number       = LABEL_REDFISH_VLAN_LIST;

    //
    // Create Hii Extend Label OpCode as the end opcode
    //
    EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                                        EndOpCodeHandle,
                                        &gEfiIfrTianoGuid,
                                        NULL,
                                        sizeof (EFI_IFR_GUID_LABEL)
                                        );
    EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
    EndLabel->Number       = LABEL_END;
    VlanStr = NetLibHiiGetString(Private->HiiHandle,STRING_TOKEN(STR_REDFISH_VLAN_ID),NULL);  //Multilanguage support    
    for (Index = 0; Index < NumberOfRedfishVlan; Index++) 
    {
        MacAddrToStr (&RedfishVlanData[Index].MacAddress,6,0,RedfishMacString); 
        if(IsValidMac(RedfishMacString))
        {
            if(RedfishVlanData[Index].VLANEnable)
            {
                BufString = NetLibHiiGetString(Private->HiiHandle,STRING_TOKEN(STR_REDFISH_VLAN_DATA_ENABLED),NULL);  //Multilanguage support
            }
            else
            {
                BufString = NetLibHiiGetString(Private->HiiHandle,STRING_TOKEN(STR_REDFISH_VLAN_DATA_DISABLED),NULL); //Multilanguage support
            }

            UnicodeSPrint (
                    String,
                    VLANSTR_SIZE,
                    L"%s %d - %s (%s)",
                    VlanStr,
                    RedfishVlanData[Index].VLANID,
                    BufString,
                    RedfishMacString
            );
            
            VlanDataId = HiiSetString(Private->HiiHandle, 0, String, NULL);
            HiiCreateGotoOpCode(
                    StartOpCodeHandle, 
                    REDFISH_VLAN_CONFIGURATION_FORM_ID, 
                    VlanDataId,
                    VlanDataId,
                    EFI_IFR_FLAG_CALLBACK,
                    REDFISH_VLAN_UPDATE_QUESTION_ID + (EFI_QUESTION_ID)Index + 1
            ); 
        }              
    }

    HiiUpdateForm (
      Private->HiiHandle,     // HII handle
      &gVlanConfigFormSetGuid,    // Formset GUID
      REDFISH_VLAN_CONFIGURATION_FORM_ID, // Form ID
      StartOpCodeHandle,          // Label for where to insert opcodes
      EndOpCodeHandle             // Replace data
      );

    HiiFreeOpCodeHandle (StartOpCodeHandle);
    HiiFreeOpCodeHandle (EndOpCodeHandle);
    if(BufString != NULL)
    {
        FreePool (BufString);
    }
    if(VlanStr != NULL)
    {
        FreePool (VlanStr);
    }
  	if (RedfishVlanData != NULL) 
	{
    	FreePool (RedfishVlanData);
  	}
	    return Status;
}
#endif
