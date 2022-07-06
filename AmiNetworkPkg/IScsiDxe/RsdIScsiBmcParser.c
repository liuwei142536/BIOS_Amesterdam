//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

//;*************************************************************************
//;<AMI_FHDR_START>
//;
//; Name: RsdIScsiBmcParser.c
//;
//; Description: This file parses the data read from BMC.
//;
//;<AMI_FHDR_END>
//;*************************************************************************
#include "RsdIScsiSupport.h"

#define RSD_ISCSI_RESPONSE_DATA_OFFSET      0x03

#define RSD_ISCSI_NO_STRINGS    0x00

#define RSD_ISCSI_TARGET_NAME   0x01
#define RSD_ISCSI_CHAP_NAME     0x02
#define RSD_ISCSI_CHAP_SECRET   0x03
#define RSD_ISCSI_RCHAP_NAME    0x04
#define RSD_ISCSI_RCHAP_SECRET  0x05

/**
  Adds the NewNode to the linked list.

  @param[in]   NewNode              The data to be added to the Linked List.
  @param[in]   TypeId               The TypeId to which this NewNode belongs to.

  @retval EFI_SUCCESS               The given node has been added to the Linked List successfully.
  @retval EFI_INVALID_PARAMETER     Either the input node is NULL or failed to add the node to Linked List.

**/
EFI_STATUS
EFIAPI
RsdIScsiAddNode(
    RSD_ISCSI_BLOCK_DATA    *NewNode,
    UINT8                   TypeId
    )
{
    EFI_STATUS  Status = EFI_INVALID_PARAMETER;
    //
    // Check if the input Node is NULL.
    //
    if(NULL == NewNode)
        return Status;

    //
    // Insert if gRsdIScsiRootHandle node is NULL.
    //
    if(NULL == gRsdIScsiRootHandle){
        gRsdIScsiRootHandle = NewNode;
    } else {
        //
        // Add Node at the beginning.
        //
        NewNode->NextHandle = (struct RSD_ISCSI_BLOCK_DATA*)gRsdIScsiRootHandle;
        gRsdIScsiRootHandle = NewNode;
    }
    return EFI_SUCCESS;
}

/**
  Creates a NewNode for RSD_ISCSI_BLOCK_DATA of given Size.

  @param[in]   AddNode              The data to be added to the Linked List.

  @retval RSD_ISCSI_BLOCK_DATA      A pointer to the newly created node
  @retval NULL                      Failed to create a new node of requested Size.

**/
RSD_ISCSI_BLOCK_DATA *
RsdIScsiCreateNewNode(
    UINT16  Size
)
{
    RSD_ISCSI_BLOCK_DATA    *Temp;
    Temp = (RSD_ISCSI_BLOCK_DATA *)AllocateZeroPool(sizeof(RSD_ISCSI_BLOCK_DATA));
    if(NULL == Temp)
        return NULL;
    Temp->Data = AllocateZeroPool(Size);
    Temp->NextHandle = NULL;

    return Temp;
}

/**
  Determines the type of data to be inserted in the Linked List base on
  the Param.

  @param[in]   Data             The data to be inserted in the Linked List.
  @param[in]   Param            The type of data to be inserted in the Linked List.
  @param[in]   Size             The size of data to be inserted in the Linked List.

  @retval EFI_SUCCESS               The data has been added to the Linked List successfully.
  @retval EFI_OUT_OF_RESOURCES      If creation of new node fails.

**/
EFI_STATUS
EFIAPI
RsdIScsiInsertBmcDataByHandle(
    VOID    *Data,
    UINT8   Param
)
{
    EFI_STATUS              Status;
    RSD_ISCSI_BLOCK_DATA    *Node;

    //
    // Create a Node to insert data in the linked list.
    //
    if (InitiatorBlock == Param) {
        Node = RsdIScsiCreateNewNode(sizeof(RSD_ISCSI_INITIATOR));
        Node->Handle = ((RSD_ISCSI_INITIATOR*)Data)->Handle;
    } else if (TargetBlock == Param) {
        Node = RsdIScsiCreateNewNode(sizeof(RSD_ISCSI_TARGET));
        Node->Handle = ((RSD_ISCSI_TARGET*)Data)->Handle;
    } else {
        Node = RsdIScsiCreateNewNode(sizeof(RSD_ISCSI_ATTEMPT));
        Node->Handle = ((RSD_ISCSI_ATTEMPT*)Data)->Handle;
    }

    if(NULL == Node) {
        Status = EFI_OUT_OF_RESOURCES;
        DEBUG((EFI_D_ERROR,"RSD_ISCI_2_2:In %a, RsdIScsiCreateNewNode failed. Status :: %r \n", __FUNCTION__, Status));
        return Status;
    }

    switch(Param) {
    case InitiatorBlock:
        Node->Handle = ((RSD_ISCSI_INITIATOR *)Data)->Handle;
        CopyMem((RSD_ISCSI_INITIATOR*)Node->Data, (RSD_ISCSI_INITIATOR*)Data, sizeof(RSD_ISCSI_INITIATOR));
        break;

    case TargetBlock:
        Node->Handle = ((RSD_ISCSI_TARGET *)Data)->Handle;
        CopyMem((RSD_ISCSI_TARGET*)Node->Data, (RSD_ISCSI_TARGET*)Data, sizeof(RSD_ISCSI_TARGET));
        break;

    case IScsiAttemptBlock:
        Node->Handle = ((RSD_ISCSI_ATTEMPT *)Data)->Handle;
        CopyMem((RSD_ISCSI_ATTEMPT*)Node->Data, (RSD_ISCSI_ATTEMPT*)Data, sizeof(RSD_ISCSI_ATTEMPT));
        break;

    case VersionBlock:
    default:
        break;
    }

    return RsdIScsiAddNode(Node, Param);
}

/**
  Deletes either all the data in the linked list or the data related to the Handle passed.

  @param[in]  DelAllHandles     If TRUE, the list will be emptied.
  @param[in]  Handle            The Handle to be removed from the list.

  @retval EFI_SUCCESS               The operation has been completed successfully.
  @retval EFI_INVALID_PARAMETER     If the handle is NULL and DelAllHandles is FALSE.

**/
EFI_STATUS
EFIAPI
RsdIScsiDeleteByHandle (
    BOOLEAN     DelAllHandles,
    UINT16      Handle
)
{
    RSD_ISCSI_BLOCK_DATA    *Current, *Prev;
    
    if(NULL == gRsdIScsiRootHandle) {
        return EFI_INVALID_PARAMETER;
    }
    Current = gRsdIScsiRootHandle;

    if (FALSE == DelAllHandles && 0 == Handle) {
        DEBUG((EFI_D_ERROR,"\nRSD_ISCI_2_2:In %a, Invalid parameter passed.\n", __FUNCTION__));
        return EFI_INVALID_PARAMETER;
    }

    if (DelAllHandles) {
        while(NULL != Current) {
            Prev = (RSD_ISCSI_BLOCK_DATA*)(Current->NextHandle);
            FreePool(Current->Data);
            FreePool(Current);
            Current = Prev;
        }
        gRsdIScsiRootHandle = NULL;
        return EFI_SUCCESS;
    }

    // Check if it is the head node.
    if(NULL != Current && 
            (Handle == (Current->Handle))) {
        gRsdIScsiRootHandle = (RSD_ISCSI_BLOCK_DATA*)Current->NextHandle;
        FreePool(Current->Data);
        FreePool(Current);
    }
    while(NULL != Current && (Handle != (Current->Handle))) {
        Prev = Current;
        Current = (RSD_ISCSI_BLOCK_DATA*)Current->NextHandle;
    }

    if(NULL == Current) {
        DEBUG((DEBUG_INFO,"\n In %a, Handle :: 0x%X is not found.", __FUNCTION__, Handle));
        return EFI_NOT_FOUND;
    }

    Prev->NextHandle = Current->NextHandle;
    
    FreePool(Current->Data);
    FreePool(Current);

    return EFI_SUCCESS;
}

/**
  Get the Data based on the Handle

  @param[in]  Handle     The Handle of Initiator or Target or iSCSI Attempt data.

  @return       Pointer to the RSD_ISCSI_BLOCK_DATA data.
  @retval NULL  The Handle cannot be found.

**/
RSD_ISCSI_BLOCK_DATA*
EFIAPI
RsdIScsiGetDataByHandle (
    UINT16      Handle
)
{
    RSD_ISCSI_BLOCK_DATA *Attempt;

    if (0 == Handle) {
        DEBUG((EFI_D_ERROR,"\nRSD_ISCI_2_2:In %a, Invalid parameter passed.\n", __FUNCTION__));
        return NULL;
    }
    Attempt = gRsdIScsiRootHandle;
    while(Attempt != NULL) {
        if(Handle == Attempt->Handle) {
            return Attempt->Data;
        }
        Attempt = (RSD_ISCSI_BLOCK_DATA*)(Attempt->NextHandle);
    }
    return NULL;
}

/**
  Displays the IP address.

  @param[in]  Address   Pointer to the IP address to be displayed.
  @param[in]  Length    Length of to print.

  @retval VOID

**/
VOID
EFIAPI
DisplayIpAddress(
    UINT8*      Address,
    UINT8       Length
)
{
    UINT8   Byte = 0;
    for(; Byte < Length; Byte++) {
        DEBUG((DEBUG_INFO,"%d.",Address[Byte]));
    }
    DEBUG((DEBUG_INFO,"\n\n"));
}

/**
  Parses and stores the BMC Initiator data into a linked list. Also checks if the next block
  of data is also of initiator data or not. If yes, the previous block of initiator data
  will be added in a linked list and the iSCSI MDR Read request will be issued again.

  @param[in]  MdrResp           If known the length to read from BMC with this READ request.
  @param[in]  BmcInitRespSize   If known the Offset of the data from where we need to read.

  @retval EFI_SUCCESS               Parsed and stored the initiator data successfully.
  @retval EFI_NO_MAPPING            MdrResp doesn't contain Initiator data.
  @retval EFI_INVALID_PARAMETER     Bmc Initiator response data size is zero.
  @retval EFI_OUT_OF_RESOURCES      Adding the data to linked list failed due to some reason.
  @retval EFI_DEVICE_ERROR          Initiator Name is not found.

**/
EFI_STATUS
EFIAPI
RsdIScsiParseInitiatorResponse (
    UINT8   *MdrInitResp,
    UINT8   BmcInitRespSize
)
{
    EFI_STATUS              Status = EFI_NOT_FOUND;
    RSD_ISCSI_INITIATOR     InitData = {0};
    UINT8                   CurrentOffset = 0, Pos = 0;
    UINT8                   *TempInitRespData = MdrInitResp + RSD_ISCSI_RESPONSE_DATA_OFFSET;

    //
    // Check whether it is the block we are looking for or not.
    //
    if(!IS_EXPECTED_DATA(TempInitRespData, InitiatorBlock)) {
        Status = EFI_NO_MAPPING;
        DEBUG((EFI_D_ERROR,"\nRSD_ISCI_2_2:In %a, ERROR: Not the expected data.\n", __FUNCTION__));
        return Status;
    }

    // If the BmcInitRespSize is ZERO, return from here.
    if( (0 == BmcInitRespSize)  || IS_VALID_SIZE(TempInitRespData)) {
        DEBUG((EFI_D_ERROR,"\nRSD_ISCI_2_2:In %a, ERROR: Invalid/ZERO Size.\n", __FUNCTION__));
        return Status;
    }

    //   Start Reading the initiator data.
    //   Initiator Block is to be considered as below.
    //
    //    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |  Param Id (1) |            Length (2)         |    Handle (2) |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |               |   TaDhEn (1)  | IPAddrType(1) |               |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |             Initiator IP Address (16 bytes)   |               |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |     Initiator Subnet Address (16 bytes)       |               |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |         Initiator Gateway (16 bytes)          |               |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |             Primary DNS (16 bytes)            |               |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |             Secondary DNS (16 bytes)          |               |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |             Initiator Name (113 bytes)                        |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //

    //
    // 1. Save 'Parameter ID' : 0x00
    //
    CopyMem(&InitData.ParameterId, TempInitRespData, sizeof(InitData.ParameterId));
    INCREMENT_BY_SIZE(TempInitRespData, CurrentOffset, sizeof(InitData.ParameterId));

    //
    // 2. Save 'Length' of this INITIATOR unit : 0x01
    //
    CopyMem(&InitData.Length, TempInitRespData, sizeof(InitData.Length));
    INCREMENT_BY_SIZE(TempInitRespData, CurrentOffset, sizeof(InitData.Length));

    //
    // 3. Save 'Handle' of this Initiator unit : 0x03
    //
    CopyMem(&InitData.Handle, TempInitRespData, sizeof(InitData.Handle));
    INCREMENT_BY_SIZE(TempInitRespData, CurrentOffset, sizeof(InitData.Handle));

    //
    // 4. Save 'Initiator DHCP Enabled' : 0x05
    //
    CopyMem(&InitData.InitiatorDhcpEnabled, TempInitRespData, sizeof(InitData.InitiatorDhcpEnabled));
    INCREMENT_BY_SIZE(TempInitRespData, CurrentOffset, sizeof(InitData.InitiatorDhcpEnabled));

    //
    // 5. Save 'Initiator Name' : 0x06
    //
    
    // Return EFI_DEVICE_ERROR if Initiator Name is not present
    if(RSD_ISCSI_NO_STRINGS == (UINT8*)TempInitRespData) {
        return EFI_DEVICE_ERROR;
    }
    INCREMENT_BY_SIZE(TempInitRespData, CurrentOffset, 1);

    //
    // 6. Save 'IP Address Type' : 0x07
    //
    CopyMem(&InitData.IpAddressType, TempInitRespData, sizeof(InitData.IpAddressType));
    INCREMENT_BY_SIZE(TempInitRespData, CurrentOffset, sizeof(InitData.IpAddressType));

    //
    // 7. Save 'Initiator IP Address' : 0x08
    //
    CopyMem(&InitData.InitiatorIpAddress, TempInitRespData, RSD_MAX_IP_BYTES);
    INCREMENT_BY_SIZE(TempInitRespData, CurrentOffset, RSD_MAX_IP_BYTES);
//  DisplayIpAddress(InitData.InitiatorIpAddress, (InitData.IpAddressType == 1 ? 4 : 16));

    //
    // 8. Save 'Initiator Subnet Mask' : 0x18
    //
    CopyMem(&InitData.InitiatorSubnetMask, TempInitRespData, RSD_MAX_IP_BYTES);
    INCREMENT_BY_SIZE(TempInitRespData, CurrentOffset, RSD_MAX_IP_BYTES);
//  DisplayIpAddress(InitData.InitiatorSubnetMask, (InitData.IpAddressType == 1 ? 4 : 16));

    //
    // 9. Save 'Initiator Gateway' : 0x28
    //
    CopyMem(&InitData.InitiatorGateway, TempInitRespData, RSD_MAX_IP_BYTES);
    INCREMENT_BY_SIZE(TempInitRespData, CurrentOffset, RSD_MAX_IP_BYTES);
//  DisplayIpAddress(InitData.InitiatorGateway, (InitData.IpAddressType == 1 ? 4 : 16));

    //
    // 10. Save 'Primary DNS' : 0x38
    //
    CopyMem(&InitData.PrimaryDns, TempInitRespData, RSD_MAX_IP_BYTES);
    INCREMENT_BY_SIZE(TempInitRespData, CurrentOffset, RSD_MAX_IP_BYTES);
//    DisplayIpAddress(InitData.PrimaryDns, (InitData.IpAddressType == 1 ? 4 : 16));

    //
    // 11. Save 'Secondary DNS' : 0x48
    //
    CopyMem(&InitData.SecondaryDns, TempInitRespData, RSD_MAX_IP_BYTES);
    INCREMENT_BY_SIZE(TempInitRespData, CurrentOffset, RSD_MAX_IP_BYTES);
//    DisplayIpAddress(InitData.SecondaryDns, (InitData.IpAddressType == 1 ? 4 : 16));

    //
    // 12. Save 'NIC MAC Address' : 0x58
    //
    CopyMem(&InitData.NicMacAddress, TempInitRespData, RSD_NIC_SIZE);
    INCREMENT_BY_SIZE(TempInitRespData, CurrentOffset, RSD_NIC_SIZE);
//    DisplayIpAddress(InitData.NicMacAddress, RSD_NIC_SIZE);

    //
    // At the end of the formatted portion, STRING will be present.
    // In initiator block, only one string (Initiator Name) will be present.
    //
    for(Pos = 0; TempInitRespData[Pos] != 0x00;Pos++); 
    CopyMem(InitData.InitiatorName, TempInitRespData, Pos);
    InitData.InitiatorName[Pos + 1] = '\0';

    //
    // Once the Initiator data is read, save this in the list.
    //
    Status = RsdIScsiInsertBmcDataByHandle(&InitData, InitiatorBlock/*, CurrentOffset*/);
    DEBUG ((DEBUG_INFO, "RSD_ISCI_2_2:In %a, RsdIScsiInsertBmcDataByHandle Status = %r \n", __FUNCTION__, Status));

    return Status;
}

/**
  Extracts the strings from the Target block.

  @param[in]    TargetData      Pointer to RSD_ISCSI_TARGET to which the extracted string should be copied.
  @param[in]    ChapSection     The pointer which holds the strings section.
  @param[in]    ChapType        The type of the CHAP.
  @param[in]    Offset          The offset from which the CHAP section starts in ChapSection.

  @retval EFI_SUCCESS           The Strings have been extracted successfully.

**/
EFI_STATUS
EFIAPI
RsdIScsiExtractChapStrings(
    RSD_ISCSI_TARGET    *TargetData,
    UINT8**             ChapSection,
    UINT8               ChapType,
    UINT8*              Offset
)
{
    UINT8       Length, StringId;
    UINT8*      String = *ChapSection;

    if(RSD_ISCSI_MUTUAL_CHAP != ChapType && RSD_ISCSI_UNI_CHAP != ChapType)
        return EFI_INVALID_PARAMETER;

    for (StringId = ChapName; StringId <= MutualChapSecret; StringId++) {
        Length = 0;
        RSD_ISCSI_STR_LEN(String, Length);  // Get the string.
        if(ChapName == StringId) {
            ZeroMem(TargetData->ChapName, AsciiStrLen(TargetData->ChapName));
            //
            // Copy CHAP Name
            //
            CopyMem(TargetData->ChapName, String, Length);
            TargetData->ChapName[Length + 1] = '\0';
        } else if (ChapSecret == StringId) {
            ZeroMem(TargetData->ChapSecret, AsciiStrLen(TargetData->ChapSecret));
            //
            // Copy CHAP Secret
            //
            CopyMem(TargetData->ChapSecret, String, Length);
            TargetData->ChapSecret[Length + 1] = '\0';
            if(RSD_ISCSI_UNI_CHAP == ChapType)
                break;
        } else if (MutualChapName == StringId) {
            ZeroMem(TargetData->MutualChapName, AsciiStrLen(TargetData->ChapSecret));
            //
            // Copy Mutual CHAP Name
            //          
            CopyMem(TargetData->MutualChapName, String, Length);
            TargetData->MutualChapName[Length + 1] = '\0';
        } else {
            ZeroMem(TargetData->MutualChapSecret, AsciiStrLen(TargetData->MutualChapSecret));
            //
            // Copy Mutual CHAP Secret
            //
            CopyMem(TargetData->MutualChapSecret, String, Length);
            TargetData->MutualChapSecret[Length + 1] = '\0';
        }
        INCREMENT_BY_SIZE(String, *ChapSection, ++Length);
        *Offset += Length;
    }
    return EFI_SUCCESS;
}

/**
  Parses and stores the BMC Target data into a linked list. Also checks if the next block
  of data is also of initiator data or not. If yes, the previous block of initiator data
  will be added in a linked list and the iSCSI MDR Read request will be issued again.

  @param[in]  MdrResp           If known the length to read from BMC with this READ request.
  @param[in]  BmcInitRespSize   If known the Offset of the data from where we need to read.

  @retval EFI_SUCCESS               Parsed and stored the initiator data successfully.
  @retval EFI_NO_MAPPING            MdrResp doesn't contain Target data.
  @retval EFI_INVALID_PARAMETER     Bmc Initiator response data size is zero.
  @retval EFI_OUT_OF_RESOURCES      Adding the data to linked list failed due to some reason.

**/
EFI_STATUS
EFIAPI
RsdIScsiParseTargetResponse (
    UINT8   *MdrTarResp,
    UINT8   BmcTarRespSize
)
{
    EFI_STATUS          Status = EFI_NO_MAPPING;
    RSD_ISCSI_TARGET    TargetData = {0};
    UINT8               CurrentOffset = 0, Pos = 0;
    UINT8               *TempTarRespData = MdrTarResp + RSD_ISCSI_RESPONSE_DATA_OFFSET;
    UINT8               IsStringsPresent = 0;

    //
    // Check whether it is the block we are looking for or not.
    //
    if(!IS_EXPECTED_DATA(TempTarRespData, TargetBlock)) {
        DEBUG((EFI_D_ERROR,"\nRSD_ISCI_2_2:In %a, Not Expected data.", __FUNCTION__));
        return Status;
    }

    // If the BmcTarRespSize is ZERO, return from here.
    if(0 == BmcTarRespSize || IS_VALID_SIZE(TempTarRespData)) {
        DEBUG((EFI_D_ERROR,"\nRSD_ISCI_2_2:In %a, invalid data size received.", __FUNCTION__));
        return Status;
    }

    //    Target Block is to be considered as below.
    //
    //    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |  Param Id (1) |            Length (2)         |    Handle (2) |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |               |   TaDhEn (1)  |  Target Name  | IPAddrType(1) |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |                Target IP Address (16 bytes)                   |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |         Target Port(2)        |         Target LUN(2)         |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   | VlanEnable(1) |          VLAN ID (2)          | Router Ad (1) |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   | AuthMethod(1) |   CHAP Name   |  CHAP Secret  |MutualChapName |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |MutuaChapSecret|            Target Name (113 bytes)            |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |                               |     Chap Name (113 bytes)     |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |                                               |               |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |                       Chap Secret                             |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |                Mutual Chap Name (113 bytes)                   |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |               |             Mutual Chap Secret                |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |                               |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

    //
    // 1. Save 'Parameter ID' : 0x00
    //
    CopyMem(&TargetData.ParameterId, TempTarRespData, sizeof(TargetData.ParameterId));
    INCREMENT_BY_SIZE(TempTarRespData, CurrentOffset, sizeof(TargetData.ParameterId));

    //
    // 2. Save 'Length' of this INITIATOR unit : 0x01
    //
    CopyMem(&TargetData.Length, TempTarRespData, sizeof(TargetData.Length));
    INCREMENT_BY_SIZE(TempTarRespData, CurrentOffset, sizeof(TargetData.Length));

    //
    // 3. Save 'Handle' of this Initiator unit : 0x03
    //
    CopyMem(&TargetData.Handle, TempTarRespData, sizeof(TargetData.Handle));
    INCREMENT_BY_SIZE(TempTarRespData, CurrentOffset, sizeof(TargetData.Handle));

    //
    // 4. Save 'Target DHCP Enabled' : 0x05
    //
    CopyMem(&TargetData.TargetDhcpEnabled, TempTarRespData, sizeof(TargetData.TargetDhcpEnabled));
    INCREMENT_BY_SIZE(TempTarRespData, CurrentOffset, sizeof(TargetData.TargetDhcpEnabled));

    //
    // 5. Save 'Target Name' : 0x06
    //
    if(RSD_ISCSI_NO_STRINGS != (*TempTarRespData)) {
        IsStringsPresent = RSD_ISCSI_TARGET_NAME;
    }
    INCREMENT_BY_SIZE(TempTarRespData, CurrentOffset, 1);

    //
    // 6. Save 'IP Address Type' : 0x07
    //
    CopyMem(&TargetData.IpAddressType, TempTarRespData, sizeof(TargetData.IpAddressType));
    INCREMENT_BY_SIZE(TempTarRespData, CurrentOffset, sizeof(TargetData.IpAddressType));

    //
    // 7. Save 'Target IP Address' : 0x08
    //
    CopyMem(&TargetData.TargetIpAddress, TempTarRespData, RSD_MAX_IP_BYTES);
    INCREMENT_BY_SIZE(TempTarRespData, CurrentOffset, RSD_MAX_IP_BYTES);
//    DisplayIpAddress(TargetData.TargetIpAddress, (TargetData.IpAddressType == 1 ? 4 : 16));

    //
    // 8. Save 'Target Port' : 0x18
    //
    CopyMem(&TargetData.TargetPort, TempTarRespData, sizeof(TargetData.TargetPort));
    INCREMENT_BY_SIZE(TempTarRespData, CurrentOffset, sizeof(TargetData.TargetPort));

    //
    // 9. Save 'Target LUN' : 0x1A
    //
    CopyMem(&TargetData.TargetLun, TempTarRespData, sizeof(TargetData.TargetLun));
    TargetData.TargetLun = HTONS(TargetData.TargetLun);    
    INCREMENT_BY_SIZE(TempTarRespData, CurrentOffset, sizeof(TargetData.TargetLun));

    //
    // 10. Save 'VLAN Enable' : 0x1C
    //
    CopyMem(&TargetData.VlanEnable, TempTarRespData, sizeof(TargetData.VlanEnable));
    INCREMENT_BY_SIZE(TempTarRespData, CurrentOffset, sizeof(TargetData.VlanEnable));

    //
    // 11. Save 'VLAN ID' : 0x1D
    //
    CopyMem(&TargetData.VlanId, TempTarRespData, sizeof(TargetData.VlanId));
    INCREMENT_BY_SIZE(TempTarRespData, CurrentOffset, sizeof(TargetData.VlanId));

    //
    // 12. Save 'Router Advertisement' : 0x1F
    //
    CopyMem(&TargetData.RouterAdvertisement, TempTarRespData, sizeof(TargetData.RouterAdvertisement));
    INCREMENT_BY_SIZE(TempTarRespData, CurrentOffset, sizeof(TargetData.RouterAdvertisement));

    //
    // 13. Save 'Authentication Method' : 0x20
    //
    CopyMem(&TargetData.AuthenticationMethod, TempTarRespData, sizeof(TargetData.AuthenticationMethod));
    INCREMENT_BY_SIZE(TempTarRespData, CurrentOffset, sizeof(TargetData.AuthenticationMethod));

    //
    // After formatted portion starts the STRING(s) Section.
    //
    for(Pos = 2; Pos <= 5; Pos++) {
        if(RSD_ISCSI_NO_STRINGS != TempTarRespData[Pos - 2]) {
            IsStringsPresent |= (1 << Pos);
        }
        INCREMENT_BY_SIZE(TempTarRespData, CurrentOffset, 1);
    }

    //
    // Copy TargetName if needed.
    //
    if(!(TargetData.TargetDhcpEnabled) &&
            IsStringsPresent & 1) {
        for(Pos = 0; TempTarRespData[Pos] != 0x00; Pos++);
        CopyMem(TargetData.TargetName, TempTarRespData, Pos);
        TargetData.TargetName[Pos + 1] = '\0';
        INCREMENT_BY_SIZE(TempTarRespData, CurrentOffset, Pos+1);
    }

    //
    // AuthenticationMethod decides the type of CHAP to be used i.e., either One-way or Mutual.
    //
    if(RSD_ISCSI_NO_CHAP != TargetData.AuthenticationMethod) {
        //
        // After the formatted portion, the STRING(s) are in the below order.
        // i) CHAP User Name ii) CHAP Secret iii) Mutual CHAP Name iv) Mutual CHAP Secret
        //
        Status = RsdIScsiExtractChapStrings(&TargetData, &TempTarRespData, TargetData.AuthenticationMethod, &CurrentOffset);
        DEBUG ((DEBUG_INFO, "RSD_ISCI_2_2:In %a, RsdIScsiExtractChapStrings Status = %r \n", __FUNCTION__, Status));
    }

    Status = RsdIScsiInsertBmcDataByHandle(&TargetData, TargetBlock);

    return Status;
}

/**
  Parses and stores the BMC iSCSI Attempt data into a linked list. Also checks if the next block
  of data is also of iSCSI Attempt data or not. If yes, the previous block of iSCSI Attempt data
  will be added in a linked list and the iSCSI MDR Read request will be issued again.

  @param[in]  MdrAttemptResp        If known the length to read from BMC with this READ request.
  @param[in]  BmcAttemptRespSize    If known the Offset of the data from where we need to read.

  @retval EFI_SUCCESS               Parsed and stored the iSCSI Attempt data successfully.
  @retval EFI_NO_MAPPING            MdrResp doesn't contain iSCSI Attempt data.
  @retval EFI_INVALID_PARAMETER     BMC iSCSI Attempt response data size is zero.
  @retval EFI_OUT_OF_RESOURCES      Adding the data to linked list failed due to some reason.

**/
EFI_STATUS
EFIAPI
RsdIScsiParseIScsiAttemptData (
    UINT8   *MdrAttemptResp,
    UINT8   BmcAttemptRespSize
)
{
    EFI_STATUS              Status = EFI_INVALID_PARAMETER;
    RSD_ISCSI_ATTEMPT       AttemptData = {0};
    UINT8                   CurrentOffset = 0;
    UINT8                   BlockSize = 0;
    UINT8                   *TempAttemptRespData = MdrAttemptResp + RSD_ISCSI_RESPONSE_DATA_OFFSET;

    //
    // Check whether it is the block we are looking for or not.
    //
    if(!IS_EXPECTED_DATA(TempAttemptRespData, IScsiAttemptBlock)) {
        DEBUG((EFI_D_ERROR,"\nRSD_ISCI_2_2:In %a, Not Expected data.", __FUNCTION__));
        return Status;
    }

    // If the BmcTarRespSize is ZERO, return from here.
    if(0 == BmcAttemptRespSize) {
        DEBUG((EFI_D_ERROR,"\nRSD_ISCI_2_2:In %a, Invalid data size.", __FUNCTION__));
        return Status;
    } else {
        BlockSize = TempAttemptRespData[1];
    }

    //    iSCSI Attempt Block is to be considered as below.
    //
    //    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |  Param Id (1) |            Length (2)         |    Handle (2) |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |               |     Initiator Handle(2)       |    Target -   |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |   Handle (2)  |    Connection wait Time (2)   | Conn.RetCnt(1)|
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

    //
    // 1. Save 'Parameter ID' : 0x00
    //
    CopyMem(&AttemptData.ParameterId, TempAttemptRespData, sizeof(UINT8));
    INCREMENT_BY_SIZE(TempAttemptRespData, CurrentOffset, sizeof(AttemptData.ParameterId));

    //
    // 2. Save 'Length' : 0x01
    //
    CopyMem(&AttemptData.Length, TempAttemptRespData, sizeof(AttemptData.Length));
    INCREMENT_BY_SIZE(TempAttemptRespData, CurrentOffset, sizeof(AttemptData.Length));

    //
    // 3. Save 'Handle' : 0x03
    //
    CopyMem(&AttemptData.Handle, TempAttemptRespData, sizeof(AttemptData.Handle));
    INCREMENT_BY_SIZE(TempAttemptRespData, CurrentOffset, sizeof(AttemptData.Handle));
    gRsdAttemptHandles[gRsdIScsiAttemptCount++] = AttemptData.Handle;

    //
    // 4. Save 'Initiator Handle' : 0x03
    //
    CopyMem(&AttemptData.InitiatorHandle, TempAttemptRespData, sizeof(AttemptData.InitiatorHandle));
    INCREMENT_BY_SIZE(TempAttemptRespData, CurrentOffset, sizeof(AttemptData.InitiatorHandle));

    //
    // 5. Save 'Target Handle' : 0x05
    //
    CopyMem(&AttemptData.TargetHandle, TempAttemptRespData, sizeof(AttemptData.TargetHandle));
    INCREMENT_BY_SIZE(TempAttemptRespData, CurrentOffset, sizeof(AttemptData.TargetHandle));

    //
    // 6. Save 'Connection Wait Time' : 0x09
    //
    CopyMem(&AttemptData.ConnectionWaitTime, TempAttemptRespData, sizeof(AttemptData.ConnectionWaitTime));
    INCREMENT_BY_SIZE(TempAttemptRespData, CurrentOffset, sizeof(AttemptData.ConnectionWaitTime));

    //
    // 7. Save 'Connection Retry Count' : 0x0B
    //
    CopyMem(&AttemptData.ConnectionRetryCount, TempAttemptRespData, sizeof(AttemptData.ConnectionRetryCount));
    INCREMENT_BY_SIZE(TempAttemptRespData, CurrentOffset, sizeof(AttemptData.ConnectionRetryCount));

    Status = RsdIScsiInsertBmcDataByHandle(&AttemptData, IScsiAttemptBlock);

    return Status;
}

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
