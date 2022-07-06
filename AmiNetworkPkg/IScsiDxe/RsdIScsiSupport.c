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
//; Name: RsdIScsiSupport.c
//;
//; Description: This file reads different blocks of iSCSI MDR region.
//;
//;<AMI_FHDR_END>
//;*************************************************************************

#include "RsdIScsiSupport.h"

#define RSD_ISCSI_ABORT_SESSION     0x00
#define RSD_ISCSI_MAX_TIMEOUT       0xFFFF
#define RSD_ISCSI_MAX_READ_CAPACITY 0xFF
#define RSD_ISCSI_RESP_DATA_START   0x03

GLOBAL_REMOVE_IF_UNREFERENCED CONST CHAR8  IScsiHexString[] = "0123456789ABCDEFabcdef";

static BOOLEAN  gRsdIScsiIpmiFound = FALSE;
static UINT16   gRsdIScsiUsedSize = 0;
static UINT16   gRsdIScsiDataLeft = 0;
static ISCSI_ATTEMPT_CONFIG_NVDATA gRsdIScsiAttemptConfigData = {0};


/**
     Convert Hex decimal MAC address to Ascii String
    
  @param[in]  MacAddr     The mac address  Ex: {0x00 , 0x50, 0x56, 0x0C0, 0X00, 0X02}
  @param[in]  Len         Length in bytes of the mac address Ex: 6 bytes.
  @param[in]  MacString8  The Mac String. Ex: "00:50:56:C0:00:02"

**/
VOID
EFIAPI
RsdIscsiConvertMacAddrToAsciiStr(
  IN UINT8 *MacAddr,
  IN UINT8 Len, 
  OUT UINT8 *MacString8)
{
    UINT8 Index, Pos;

    if(NULL == MacAddr || 0 == Len) {
        MacString8[0] = '\0';
        return;
    }

    for(Pos = 0, Index = 0 ; Pos < Len ; Pos++) {
        MacString8[Index++] = IScsiHexString[ (MacAddr[Pos] >> 4) & 0x0F ] ;
        MacString8[Index++] = IScsiHexString[ (MacAddr[Pos]) & 0x0F ];   
        MacString8[Index++] = ':';
    }
    MacString8[--Index] = '\0';     // Remove the ':' at the end and place NULL character
}

/**
     Save the BMC parsed data using variable services .
     Section 4.1 says: If no structure or only version structure is available, then it indicates to the BIOS that there is no iSCSI Configuration available and 
     BIOS should delete its internal copies of prior iSCSI configuration data 
     To compliant with Section 4.1
     Will set the variable without setting EFI_VARIABLE_NON_VOLATILE attribute flag,So that firmware stores the variable in normal memory and it is NOT maintained 
     across a power cycle.

  @param[in]    RsdAttemptConfigIndex   Current RSD iSCSI attempt Index
     

  @retval       EFI_SUCCESS             ISCSI_ATTEMPT_CONFIG_NVDATA has been framed from the input arguments.

**/
EFI_STATUS
EFIAPI
RsdIScsiConvertBmcDataToAttemptConfigNvData(IN UINT8 RsdAttemptConfigIndex)
{
    CHAR8        MacString8[ISCSI_MAX_MAC_STRING_LEN]; 
    CHAR16       MacString16[ISCSI_MAX_MAC_STRING_LEN];
    CHAR16       MacPortString[ISCSI_NAME_IFR_MAX_SIZE];
    EFI_STATUS   Status; 
    UINTN        RsdAttemptConfigOrderSize;
    UINT8        *RsdAttemptConfigOrder = 0;

    // Convert Hex decimal MAC address to Ascii String 
    RsdIscsiConvertMacAddrToAsciiStr(gRsdIScsiAttemptConfigData.MacString, RSD_NIC_SIZE, MacString8);

    AsciiStrToUnicodeStrS (MacString8, MacString16, sizeof (MacString16) / sizeof (MacString16[0]));

    UnicodeSPrint (
            MacPortString,
            (UINTN) 128,
            L"%s%d",
            MacString16,
            (UINTN) RsdAttemptConfigIndex
            );

    //
    // If the EFI_VARIABLE_NON_VOLATILE attribute is not set, the firmware stores the variable in normal memory and it is not maintained across a power cycle.
    // 
    Status =  gRT->SetVariable (
            MacPortString,
            &gEfiRsdIScsiAttemptConfigGuid,
            EFI_VARIABLE_BOOTSERVICE_ACCESS,             
            sizeof (ISCSI_ATTEMPT_CONFIG_NVDATA),
            &gRsdIScsiAttemptConfigData
            );

    if(EFI_ERROR(Status)) {
        DEBUG((EFI_D_ERROR,"\nIn %a, SetVariable failed. Status :: %r", __FUNCTION__, Status));
        return Status;
    }

    //
    // Rsd Attempt order
    //
    RsdAttemptConfigOrderSize = gRsdIScsiAttemptCount * sizeof (UINT8);
    RsdAttemptConfigOrder = AllocateZeroPool (RsdAttemptConfigOrderSize);
    if (RsdAttemptConfigOrder == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }

    gRT->GetVariable (
            L"RsdAttemptOrder", 
            &gEfiRsdIScsiAttemptConfigGuid, 
            NULL, 
            &RsdAttemptConfigOrderSize, 
            RsdAttemptConfigOrder
            );

    RsdAttemptConfigOrder[RsdAttemptConfigIndex - 1] = RsdAttemptConfigIndex;           

    //
    // If the EFI_VARIABLE_NON_VOLATILE attribute is not set, the firmware stores the variable in normal memory and it is not maintained across a power cycle.
    //
    Status = gRT->SetVariable (
            L"RsdAttemptOrder",
            &gEfiRsdIScsiAttemptConfigGuid,
            EFI_VARIABLE_BOOTSERVICE_ACCESS,  
            RsdAttemptConfigOrderSize,
            RsdAttemptConfigOrder
            );

    if (RsdAttemptConfigOrder != NULL) {
        FreePool (RsdAttemptConfigOrder);
    }

    return Status;
}

/**
  Locates the EFI_IPMI_TRANSPORT protocol and initializes the common 
  parameters of RSD_MDR_READ_REQ.

  @param[in]   VOID

  @retval EFI_NOT_FOUND     If the EFI_IPMI_TRANSPORT protocol cannot be found.
  @retval EFI_SUCCESS       If the EFI_IPMI_TRANSPORT protocol is found and initialized
                            other values to its default.
**/
EFI_STATUS
EFIAPI
RsdIScsiInitializeReadRequest(
    VOID
)
{
    EFI_STATUS  Status = EFI_NOT_FOUND;
    EFI_GUID    gEfiIpmiTransportGuid = gEfiDxeIpmiTransportProtocolGuid;

    Status = gBS->LocateProtocol (
            &gEfiIpmiTransportGuid,
            NULL,
            &gIpmiTransport
            );

    if(EFI_ERROR(Status)) {
        DEBUG((EFI_D_ERROR,"RSD_ISCI_2_2:In %a, locating IPMI_USB_TRANSPORT failed. Status :: %r \n", __FUNCTION__, Status));
        return Status;
    }
    gRsdIScsiReadReq.DefiningBodyID = RSD_INTEL_SPEC;
    gRsdIScsiReadReq.RegionType     = RSD_ISCS_BOOT_OPTIONS_REGION;
    gRsdIScsiRootHandle             = NULL;
    gRsdIScsiIpmiFound              = TRUE;

    return Status;
}

/**
  Reads the Version Structure from the iSCSI Boot options region.

  @param[in]  LengthToRead      If known, the length to read from BMC with this READ request.
                                Otherwise reads the maximum possible length.
  @param[in]  OffsetToRead      If known, the Offset of the data from where we need to read.
                                Otherwise reads from the offset in gRsdIScsiCurOffset.

  @retval EFI_INVALID_PARAMETER Any input or configured parameter is invalid.
  @retval EFI_SUCCESS           The operation is completed successfully.

**/
EFI_STATUS
EFIAPI
RsdIScsiReadVersionBlock (
    UINT16   LengthToRead,
    UINT16  OffsetToRead
)
{
    EFI_STATUS          Status = EFI_INVALID_PARAMETER;
    UINT8               RespSize = 0;
    RSD_MDR_READ_RES    *RsdIScsiVersionResp;


    //   VersionBlock is to be considered as below.
    //
    //    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |  Param Id (1) |            Length (2)         |    Handle (2) |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //   |               | Major Ver (1) | Minor Ver (1) |
    //   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //

    if(LengthToRead < (RSD_ISCSI_MAX_READ_CAPACITY - RSD_ISCSI_RESP_DATA_START)) {
        gRsdIScsiReadReq.DataLength = (UINT8)LengthToRead;
        RsdIScsiVersionResp = AllocateZeroPool(gRsdIScsiReadReq.DataLength + RSD_ISCSI_RESP_DATA_START);
        if(NULL == RsdIScsiVersionResp){
            DEBUG ((EFI_D_ERROR, "RSD_ISCI_2_2:In %a, AllocateZeroPool#1 failed. Status = %r \n", __FUNCTION__ ,Status));
            return EFI_OUT_OF_RESOURCES;
        }
    }
    gRsdIScsiReadReq.DataOffset = gRsdIScsiCurOffset;

    //
    // Issue ReadRegion request for Version Block
    //
    Status = MdrReadRegion(gIpmiTransport, &gRsdIScsiReadReq, RsdIScsiVersionResp, &RespSize);
    DEBUG ((DEBUG_INFO, "RSD_ISCI_2_2:In %a, MdrReadRegion#1 Status = %r \n", __FUNCTION__, Status));

    //
    // If Status is other than EFI_BUFFER_TOO_SMALL, return from here.
    //
    if(EFI_BUFFER_TOO_SMALL != Status &&
            EFI_ERROR(Status)) {
        DEBUG ((EFI_D_ERROR, "RSD_ISCI_2_2:In %a, MdrReadRegion#1 failed. Status = %r \n", __FUNCTION__ , Status));
        if(RsdIScsiVersionResp)
            FreePool(RsdIScsiVersionResp);
        return Status;
    }

    if(EFI_BUFFER_TOO_SMALL == Status)
    {
        if(RsdIScsiVersionResp) {
            FreePool(RsdIScsiVersionResp);
        }
        //
        // Assign the size required and send the ReadRegion request again.
        //
        RsdIScsiVersionResp = AllocateZeroPool(RespSize);
        if(NULL == RsdIScsiVersionResp){
            DEBUG ((EFI_D_ERROR, "RSD_ISCI_2_2:In %a, AllocateZeroPool#2 failed. Status = %r \n", __FUNCTION__));
            return EFI_OUT_OF_RESOURCES;
        }

        Status = MdrReadRegion(gIpmiTransport, &gRsdIScsiReadReq, RsdIScsiVersionResp, &RespSize);
        DEBUG ((DEBUG_INFO, "RSD_ISCI_2_2:In %a, MdrReadRegion#2 Status = %r \n", __FUNCTION__, Status));
    }

    if(RsdIScsiVersionResp)
        FreePool(RsdIScsiVersionResp);

    return Status;
}

/**
  Reads the Initiator Structure from the iSCSI Boot options region.

  @param[in]  LengthToRead      If known, the length to read from BMC with this READ request.
                                Otherwise reads the maximum possible length.
  @param[in]  OffsetToRead      If known, the Offset of the data from where we need to read.
                                Otherwise reads from the offset in gRsdIScsiCurOffset.

  @retval EFI_INVALID_PARAMETER Any input or configured parameter is invalid.
  @retval EFI_SUCCESS           The operation is completed successfully.

**/
EFI_STATUS
EFIAPI
RsdIScsiReadInitiatorBlock (
    UINT16   LengthToRead,
    UINT16  OffsetToRead
)
{
    EFI_STATUS          Status;
    UINT8               RespSize = 0;
    RSD_MDR_READ_RES    *RsdIScsiInitResp;
    BOOLEAN             ReadMultiples = FALSE;

    if(LengthToRead < (RSD_ISCSI_MAX_READ_CAPACITY - RSD_ISCSI_RESP_DATA_START)) {
        gRsdIScsiReadReq.DataLength = (UINT8)LengthToRead;
        RsdIScsiInitResp = AllocateZeroPool(gRsdIScsiReadReq.DataLength + RSD_ISCSI_RESP_DATA_START);
    } else {
        ReadMultiples = TRUE;
        gRsdIScsiReadReq.DataLength = RSD_ISCSI_MAX_READ_CAPACITY;
        RsdIScsiInitResp = AllocateZeroPool(RSD_ISCSI_MAX_READ_CAPACITY);
    }

    if(NULL == RsdIScsiInitResp) {
        DEBUG ((EFI_D_ERROR, "RSD_ISCI_2_2:In %a, AllocateZeroPool#1 failed.\n", __FUNCTION__));
        return EFI_OUT_OF_RESOURCES;
    }

    gRsdIScsiReadReq.DataOffset = gRsdIScsiCurOffset;

    //
    // Issue Read request to BMC for initiator data.
    //
    Status = MdrReadRegion(gIpmiTransport, &gRsdIScsiReadReq, RsdIScsiInitResp, &RespSize);
    DEBUG ((DEBUG_INFO, "RSD_ISCI_2_2:In %a, MdrReadRegion#1 Status = %r \n", __FUNCTION__, Status));

    //
    // If Status is other than EFI_BUFFER_TOO_SMALL, return from here.
    //
    if(EFI_BUFFER_TOO_SMALL != Status &&
            EFI_ERROR(Status)) {
        return Status;
    }

    if(EFI_BUFFER_TOO_SMALL == Status)
    {
        if(RsdIScsiInitResp) {
            FreePool(RsdIScsiInitResp);
        }
        RsdIScsiInitResp = AllocateZeroPool(RespSize);
        if(NULL == RsdIScsiInitResp) {
            DEBUG ((EFI_D_ERROR, "RSD_ISCI_2_2:In %a, AllocateZeroPool#2 failed. Status = %r \n", __FUNCTION__, Status));
            return EFI_OUT_OF_RESOURCES;
        }
    
        Status = MdrReadRegion(gIpmiTransport, &gRsdIScsiReadReq, RsdIScsiInitResp, &RespSize);
        if(EFI_ERROR(Status)) {
            DEBUG ((EFI_D_ERROR, "RSD_ISCI_2_2:In %a, MdrReadRegion#2 failed. Status = %r \n", __FUNCTION__, Status));
            if(RsdIScsiInitResp)
                FreePool(RsdIScsiInitResp);
            return Status;
        }
    }

    if(ReadMultiples == TRUE)
    {
        gRsdIScsiReadReq.DataLength = LengthToRead - RSD_ISCSI_MAX_READ_CAPACITY;
        RsdIScsiInitResp = ReallocatePool (RSD_ISCSI_MAX_READ_CAPACITY,LengthToRead,RsdIScsiInitResp);
        if(NULL == RsdIScsiInitResp) {
            DEBUG ((EFI_D_ERROR, "RSD_ISCI_2_2:In %a, ReallocatePool#1 failed. Status = %r \n", __FUNCTION__, Status));
            return EFI_OUT_OF_RESOURCES;
        }

        gRsdIScsiReadReq.DataOffset = gRsdIScsiCurOffset + RSD_ISCSI_MAX_READ_CAPACITY;
        
        //
        // Issue Read request to BMC for initiator data.
        //
        Status = MdrReadRegion(gIpmiTransport, &gRsdIScsiReadReq, RsdIScsiInitResp + RSD_ISCSI_MAX_READ_CAPACITY, &RespSize);
       
        //
        // If Status is other than EFI_BUFFER_TOO_SMALL, return from here.
        //
        if(EFI_BUFFER_TOO_SMALL != Status &&
                EFI_ERROR(Status)) {
            return Status;
        }

        if(EFI_BUFFER_TOO_SMALL == Status)
        {
            if(RsdIScsiInitResp) {
                FreePool(RsdIScsiInitResp);
            }
            RsdIScsiInitResp = ReallocatePool (RSD_ISCSI_MAX_READ_CAPACITY,LengthToRead,RsdIScsiInitResp);
            if(NULL == RsdIScsiInitResp) {
                DEBUG ((EFI_D_ERROR, "RSD_ISCI_2_2:In %a, ReallocatePool#2 failed. Status = %r \n", __FUNCTION__, Status));
                return EFI_OUT_OF_RESOURCES;
            }
        
            Status = MdrReadRegion(gIpmiTransport, &gRsdIScsiReadReq, RsdIScsiInitResp + RSD_ISCSI_MAX_READ_CAPACITY, &RespSize);
            if(EFI_ERROR(Status)) {
                DEBUG ((EFI_D_ERROR, "RSD_ISCI_2_2:In %a, MdrReadRegion failed. Status = %r \n", __FUNCTION__, Status));
                if(RsdIScsiInitResp)
                    FreePool(RsdIScsiInitResp);
                return Status;
            }
        }
    }

    //
    // Upon successful read, parse the BMC Initiator data and store locally.
    // This data will be used for iSCSI connection once we have Target and iSCSI Attempt data.
    //
    Status = RsdIScsiParseInitiatorResponse(RsdIScsiInitResp, RespSize);
    DEBUG ((DEBUG_INFO, "RSD_ISCI_2_2:In %a, RsdIScsiParseInitiatorResponse Status = %r \n", __FUNCTION__, Status));

    if(RsdIScsiInitResp)
        FreePool(RsdIScsiInitResp);
    return Status;
}

/**
  Reads the Target Structure from the iSCSI Boot options region.

  @param[in]  LengthToRead      If known, the length to read from BMC with this READ request.
                                Otherwise reads the maximum possible length.
  @param[in]  OffsetToRead      If known, the Offset of the data from where we need to read.
                                Otherwise reads from the offset in gRsdIScsiCurOffset.

  @retval EFI_INVALID_PARAMETER Any input or configured parameter is invalid.
  @retval EFI_SUCCESS           The operation is completed successfully.

**/
EFI_STATUS
EFIAPI
RsdIScsiReadTargetBlock (
    UINT16   LengthToRead,
    UINT16  OffsetToRead
)
{
    EFI_STATUS          Status = EFI_INVALID_PARAMETER;
    UINT8               RespSize = 0;
    RSD_MDR_READ_RES    *RsdIScsiTargetResp;
    BOOLEAN             ReadMultiples = FALSE;

    if(LengthToRead < (RSD_ISCSI_MAX_READ_CAPACITY - RSD_ISCSI_RESP_DATA_START)) {
        gRsdIScsiReadReq.DataLength = (UINT8)LengthToRead;
        RsdIScsiTargetResp = AllocateZeroPool(gRsdIScsiReadReq.DataLength + RSD_ISCSI_RESP_DATA_START);
    } else {
        ReadMultiples = TRUE;
        gRsdIScsiReadReq.DataLength = RSD_ISCSI_MAX_READ_CAPACITY;
        RsdIScsiTargetResp = AllocateZeroPool(RSD_ISCSI_MAX_READ_CAPACITY);
    }
    if(NULL == RsdIScsiTargetResp) {
        DEBUG ((EFI_D_ERROR, "RSD_ISCI_2_2:In %a, AllocateZeroPool#1 failed. Status = %r \n", __FUNCTION__, Status));
        return EFI_OUT_OF_RESOURCES;
    }

    gRsdIScsiReadReq.DataOffset = gRsdIScsiCurOffset;

    //
    // Issue Read request to BMC for Target data.
    //
    Status = MdrReadRegion(gIpmiTransport, &gRsdIScsiReadReq, RsdIScsiTargetResp, &RespSize);
    DEBUG ((DEBUG_INFO, "RSD_ISCI_2_2:In %a, MdrReadRegion#1 Status = %r \n", __FUNCTION__, Status));

    //
    // If Status is other than EFI_BUFFER_TOO_SMALL, return from here.
    //
    if(EFI_BUFFER_TOO_SMALL != Status &&
            EFI_ERROR(Status)) {
        DEBUG ((DEBUG_INFO, "RSD_ISCI_2_2:In %a, MdrReadRegion#1 failed. Status = %r \n", __FUNCTION__, Status));
        return Status;
    }

    if(EFI_BUFFER_TOO_SMALL == Status)
    {
        if(RsdIScsiTargetResp) {
            FreePool(RsdIScsiTargetResp);
        }

        RsdIScsiTargetResp = AllocateZeroPool(RespSize);
        if(NULL == RsdIScsiTargetResp) {
            DEBUG ((EFI_D_ERROR, "RSD_ISCI_2_2:In %a, AllocateZeroPool#2 failed. Status = %r \n", __FUNCTION__, Status));
            return EFI_OUT_OF_RESOURCES;
        }
    
        Status = MdrReadRegion(gIpmiTransport, &gRsdIScsiReadReq, RsdIScsiTargetResp, &RespSize);
        if(EFI_ERROR(Status)) {
            DEBUG ((EFI_D_ERROR, "RSD_ISCI_2_2:In %a, MdrReadRegion#2 failed. Status = %r \n", __FUNCTION__, Status));
            if(RsdIScsiTargetResp)
                FreePool(RsdIScsiTargetResp);
            return Status;
        }
    }
    
    if(ReadMultiples)
    {
        gRsdIScsiReadReq.DataLength = LengthToRead - RSD_ISCSI_MAX_READ_CAPACITY;
        RsdIScsiTargetResp = ReallocatePool (RSD_ISCSI_MAX_READ_CAPACITY,LengthToRead,RsdIScsiTargetResp);
        if(NULL == RsdIScsiTargetResp) {
            return EFI_OUT_OF_RESOURCES;
        }

        gRsdIScsiReadReq.DataOffset = gRsdIScsiCurOffset + RSD_ISCSI_MAX_READ_CAPACITY;

        //
        // Issue Read request to BMC for initiator data.
        //
        Status = MdrReadRegion(gIpmiTransport, &gRsdIScsiReadReq, RsdIScsiTargetResp + RSD_ISCSI_MAX_READ_CAPACITY, &RespSize);

        //
        // If Status is other than EFI_BUFFER_TOO_SMALL, return from here.
        //
        if(EFI_BUFFER_TOO_SMALL != Status &&
                EFI_ERROR(Status)) {
            DEBUG ((EFI_D_ERROR, "RSD_ISCI_2_2:In %a, MdrReadRegion#3 failed. Status = %r \n", __FUNCTION__, Status));
            return Status;
        }

        if(Status == EFI_BUFFER_TOO_SMALL)
        {
            if(RsdIScsiTargetResp) {
                FreePool(RsdIScsiTargetResp);
            }
            RsdIScsiTargetResp = ReallocatePool (RSD_ISCSI_MAX_READ_CAPACITY,LengthToRead,RsdIScsiTargetResp);
            if(NULL == RsdIScsiTargetResp) {
                if(RsdIScsiTargetResp)
                    FreePool(RsdIScsiTargetResp);
                DEBUG ((EFI_D_ERROR, "RSD_ISCI_2_2:In %a, ReallocatePool#1 failed. Status = %r \n", __FUNCTION__, Status));
                return EFI_OUT_OF_RESOURCES;
            }

            Status = MdrReadRegion(gIpmiTransport, &gRsdIScsiReadReq, RsdIScsiTargetResp + RSD_ISCSI_MAX_READ_CAPACITY, &RespSize);
            if(EFI_ERROR(Status)) {
                DEBUG ((EFI_D_ERROR, "RSD_ISCI_2_2:In %a, MdrReadRegion#3 failed. Status = %r \n", __FUNCTION__, Status));
                return Status;
            }
        }
    }

    //
    // Upon successful read, parse the BMC target data and store locally.
    // This data will be used for iSCSI connection once we have Target and iSCSI Attempt data.
    //
    Status = RsdIScsiParseTargetResponse(RsdIScsiTargetResp, RespSize);
    DEBUG ((DEBUG_INFO, "RSD_ISCI_2_2:In %a, RsdIScsiParseTargetResponse Status = %r \n", __FUNCTION__, Status));

    if(RsdIScsiTargetResp)
        FreePool(RsdIScsiTargetResp);
    return Status;
}

/**
  Reads the iSCSI Attempt Data from the iSCSI Boot options region.

  @param[in]  LengthToRead      If known, the length to read from BMC with this READ request.
                                Otherwise reads the maximum possible length.
  @param[in]  OffsetToRead      If known, the Offset of the data from where we need to read.
                                Otherwise reads from the offset in gRsdIScsiCurOffset.

  @retval EFI_INVALID_PARAMETER Any input or configured parameter is invalid.
  @retval EFI_SUCCESS           The operation is completed successfully.

**/
EFI_STATUS
EFIAPI
RsdIScsiReadIScsiAttemptBlock (
    UINT16   LengthToRead,
    UINT16  OffsetToRead
)
{
    EFI_STATUS          Status = EFI_INVALID_PARAMETER;
    UINT8               RespSize = 0;
    RSD_MDR_READ_RES    *RsdIScsiBootResp;

    if(LengthToRead < (RSD_ISCSI_MAX_READ_CAPACITY - RSD_ISCSI_RESP_DATA_START)) {
        gRsdIScsiReadReq.DataLength = (UINT8)LengthToRead;
        RsdIScsiBootResp = AllocateZeroPool(gRsdIScsiReadReq.DataLength + RSD_ISCSI_RESP_DATA_START);
        if(NULL == RsdIScsiBootResp) {
            return EFI_OUT_OF_RESOURCES;
        }
    }
    gRsdIScsiReadReq.DataOffset = gRsdIScsiCurOffset;

    Status = MdrReadRegion(gIpmiTransport, &gRsdIScsiReadReq, RsdIScsiBootResp, &RespSize);
    DEBUG ((DEBUG_INFO, "RSD_ISCI_2_2:In %a, MdrReadRegion#1 Status = %r \n", __FUNCTION__, Status));

    //
    // If Status is other than EFI_BUFFER_TOO_SMALL, return from here.
    //
    if(EFI_BUFFER_TOO_SMALL != Status &&
            EFI_ERROR(Status)) {
        DEBUG ((EFI_D_ERROR, "RSD_ISCI_2_2:In %a, MdrReadRegion#3 failed. Status = %r \n", __FUNCTION__, Status));
        return Status;
    }

    if(EFI_BUFFER_TOO_SMALL == Status)
    {
        if(NULL != RsdIScsiBootResp)
            FreePool(RsdIScsiBootResp);

        RsdIScsiBootResp = AllocateZeroPool(RespSize);

        if(NULL == RsdIScsiBootResp)
            return EFI_OUT_OF_RESOURCES;
    
        Status = MdrReadRegion(gIpmiTransport, &gRsdIScsiReadReq, RsdIScsiBootResp, &RespSize);
        if(EFI_ERROR(Status)) {
            DEBUG ((EFI_D_ERROR, "RSD_ISCI_2_2:In %a, MdrReadRegion#2 Status = %r \n", __FUNCTION__, Status));
            return Status;
        }
    }

    //
    // Upon successful read, parse the BMC iSCSI Attempt data and store locally.
    //
    Status = RsdIScsiParseIScsiAttemptData(RsdIScsiBootResp, RespSize);
    DEBUG ((DEBUG_INFO, "RSD_ISCI_2_2:In %a, RsdIScsiParseIScsiAttemptResponse Status = %r \n", __FUNCTION__, Status));

    if(RsdIScsiBootResp)
        FreePool(RsdIScsiBootResp);

    return Status;
}

/**
  This function Issues the READ REQUESTS for each block of the iSCSI data region.
  Based on the parameter id, the read request will be sent.

  @param[in]  ParameterId           The Parameter ID to read.

  @param[out] EFI_SUCCESS           If data from BMC is read successfully.
  @param[out] EFI_INVALID_PARAMETER If any error happens while reading the data from BMC.

**/
EFI_STATUS
EFIAPI
RsdIScsiReadUsedSize ()
{
    EFI_STATUS          Status = EFI_NOT_FOUND;
    RSD_MDR_READ_RES    *MdrReadResp = (RSD_MDR_READ_RES*)NULL;
    UINT8               RespSize = 2 * RSD_ISCSI_RESP_DATA_START;

    //
    // Allocate 6 bytes. 
    // 1. Intel® RSD Specification  2. Read Length   3. Update Count
    // 4. ParameterId   (5,6). BlockLength
    //

    if((RSD_MDR_READ_RES*)NULL == MdrReadResp)
        MdrReadResp = AllocateZeroPool(RespSize);
    if(NULL == MdrReadResp) {
        return EFI_OUT_OF_RESOURCES;
    }
    
    gRsdIScsiReadReq.DefiningBodyID = RSD_INTEL_SPEC;
    gRsdIScsiReadReq.RegionType     = RSD_ISCS_BOOT_OPTIONS_REGION;
    
    while(gRsdIScsiDataLeft > 0) {
        gRsdIScsiReadReq.DataLength     = RSD_ISCSI_RESP_DATA_START;
        gRsdIScsiReadReq.DataOffset     = gRsdIScsiCurOffset;

        Status = MdrReadRegion(
                gIpmiTransport,
                &gRsdIScsiReadReq,
                MdrReadResp,
                &RespSize);
        DEBUG((DEBUG_INFO,"\nRSD_ISCSI: In %a, MdrReadRegion#1 Status :: %r, RespSize :: 0x%X", __FUNCTION__, Status, RespSize));
        
        if(EFI_BUFFER_TOO_SMALL != Status && 
                EFI_ERROR(Status)) {
            break;
        }

        if(EFI_BUFFER_TOO_SMALL == Status) {
            if(NULL != MdrReadResp) {
                FreePool(MdrReadResp);
            }
            MdrReadResp = AllocateZeroPool(RespSize);
            if(NULL == MdrReadResp) {
                Status = EFI_OUT_OF_RESOURCES;
                break;
            }

            Status = MdrReadRegion(
                    gIpmiTransport,
                    &gRsdIScsiReadReq,
                    MdrReadResp,
                    &RespSize);
            DEBUG((DEBUG_INFO,"\nRSD_ISCSI: In %a, MdrReadRegion#2 Status :: %r, RespSize :: 0x%X", __FUNCTION__, Status, RespSize));
            
            if(EFI_ERROR(Status)) {
                return Status;
            }
        }

        if(!EFI_ERROR(Status)) {
            UINT8   BlockType = 0;
            UINT16  BlockSize = 0;

            CopyMem(&BlockType, (UINT8*)MdrReadResp + RSD_ISCSI_RESP_DATA_START, sizeof(UINT8));
            CopyMem(&BlockSize, (UINT8*)MdrReadResp + RSD_ISCSI_RESP_DATA_START + 1, sizeof(UINT16));

            if(BlockType < VersionBlock || BlockType > IScsiAttemptBlock) {
                // TODO: Handle all memory and delete DS.
                if(NULL != MdrReadResp) {
                    FreePool(MdrReadResp);
                    Status = EFI_NOT_FOUND;
                    break;
                }
            }

            switch(BlockType) {
            case VersionBlock:
                //
                // Read Version data.
                //
                Status = RsdIScsiReadVersionBlock(BlockSize, gRsdIScsiCurOffset);
                DEBUG((DEBUG_INFO,"RSD_ISCI_2_2:In %a, RsdIScsiReadVersionBlock Status :: %r \n", __FUNCTION__, Status));
                break;

            case InitiatorBlock:
                //
                // Read Initiator data.
                //
                Status = RsdIScsiReadInitiatorBlock(BlockSize, gRsdIScsiCurOffset);
                DEBUG((DEBUG_INFO,"RSD_ISCI_2_2:In %a, RsdIScsiReadInitiatorBlock Status :: %r \n", __FUNCTION__, Status));
                break;
            case TargetBlock:
                //
                // Read Target data.
                //
                Status = RsdIScsiReadTargetBlock(BlockSize, gRsdIScsiCurOffset);
                DEBUG((DEBUG_INFO,"RSD_ISCI_2_2:In %a, RsdIScsiReadTargetBlock Status :: %r \n", __FUNCTION__, Status));
                break;

            case IScsiAttemptBlock:
                //
                // Read iSCSI Attempt data.
                //
                Status = RsdIScsiReadIScsiAttemptBlock(BlockSize, gRsdIScsiCurOffset);
                DEBUG((DEBUG_INFO,"RSD_ISCI_2_2:In %a, RsdIScsiReadIScsiAttemptBlock Status :: %r \n", __FUNCTION__, Status));
                break;

            default:
                // Never should happen.
                break;
            }
            
            if(!EFI_ERROR(Status)) {
                gRsdIScsiCurOffset += BlockSize;
                gRsdIScsiDataLeft -= BlockSize;
            }
            if(NULL != MdrReadResp &&
                    0 == gRsdIScsiDataLeft) {
                FreePool(MdrReadResp);
            }
        }
    }
    return Status;
}

/**
  Constructs one copy of ISCSI_ATTEMPT_CONFIG_NVDATA from the set of the three blocks passed.

  @param[in]    AttemptData     The iSCSI attempt parameter structure.
  @param[in]    InitData        The Target parameter structure defines the Target configuration.
  @param[in]    TargetData      The Initiator parameter structure defines the Initiator configuration.

  @retval EFI_SUCCESS           ISCSI_ATTEMPT_CONFIG_NVDATA has been framed from the input arguments.

**/
EFI_STATUS
EFIAPI
RsdIScsiFrameAttemptConfigData(
    RSD_ISCSI_ATTEMPT   *AttemptData,
    RSD_ISCSI_INITIATOR *InitData,
    RSD_ISCSI_TARGET    *TargetData,
    UINT8               RsdAttemptConfigIndex
)
{
    EFI_STATUS  Status = EFI_SUCCESS;
    
    //
    // INITIATOR DATA
    //

    // Copy Initiator Name.
    CopyMem(gRsdIScsiInitiatorName, &InitData->InitiatorName, AsciiStrLen(InitData->InitiatorName));
    Status = IScsiNormalizeName (gRsdIScsiInitiatorName, AsciiStrLen (gRsdIScsiInitiatorName));
    
    if(EFI_ERROR(Status)) {
        DEBUG((EFI_D_ERROR,"\nRSD_ISCI_2_2:In %a, BMC's Initiator name is invalid.\n", __FUNCTION__));
        //
        // We can't make use of this Initiator info. Delete it.
        //
        if(NULL != gRsdIScsiRootHandle)
            RsdIScsiDeleteByHandle(FALSE, InitData->Handle);
        return Status;
    }

    // Copy IpMode
    if(RSD_IP_MODE_AUTOCONFIG == InitData->IpAddressType)
        gRsdIScsiAttemptConfigData.SessionConfigData.IpMode = IP_MODE_AUTOCONFIG;
    else if(RSD_IP_MODE_IP4 == InitData->IpAddressType)
        gRsdIScsiAttemptConfigData.SessionConfigData.IpMode     = IP_MODE_IP4;
    else if(RSD_IP_MODE_IP6 == InitData->IpAddressType)
        gRsdIScsiAttemptConfigData.SessionConfigData.IpMode     = IP_MODE_IP6;
    
    // Copy InitiatorInfoFromDhcp
    gRsdIScsiAttemptConfigData.SessionConfigData.InitiatorInfoFromDhcp = (BOOLEAN)InitData->InitiatorDhcpEnabled;

    // Copy LocalIp
    CopyMem(&gRsdIScsiAttemptConfigData.SessionConfigData.LocalIp, InitData->InitiatorIpAddress, sizeof(EFI_IP_ADDRESS));

    // Copy SubnetMask
    CopyMem(&gRsdIScsiAttemptConfigData.SessionConfigData.SubnetMask, &InitData->InitiatorSubnetMask, sizeof(EFI_IP_ADDRESS));

    // Copy Gateway
    CopyMem(&gRsdIScsiAttemptConfigData.SessionConfigData.Gateway, &InitData->InitiatorGateway, sizeof(EFI_IP_ADDRESS));

    // Copy PrimaryDns
    CopyMem(&gRsdIScsiAttemptConfigData.PrimaryDns, &InitData->PrimaryDns, sizeof(EFI_IP_ADDRESS));

    // Copy SecondaryDns
    CopyMem(&gRsdIScsiAttemptConfigData.SecondaryDns, &InitData->SecondaryDns, sizeof(EFI_IP_ADDRESS));

    // Copy MacString and ISID
    CopyMem(gRsdIScsiAttemptConfigData.MacString, &InitData->NicMacAddress, RSD_NIC_SIZE);
    CopyMem (gRsdIScsiAttemptConfigData.SessionConfigData.IsId, gRsdIScsiAttemptConfigData.MacString, RSD_NIC_SIZE);
    gRsdIScsiAttemptConfigData.SessionConfigData.IsId[0] = (UINT8) (gRsdIScsiAttemptConfigData.SessionConfigData.IsId[0] & 0x3F);

    //
    // TARGET DATA
    //

    // Copy TargetName
    CopyMem(gRsdIScsiAttemptConfigData.SessionConfigData.TargetName, &TargetData->TargetName, AsciiStrLen(TargetData->TargetName));

    // Copy TargetInfoFromDhcp
    gRsdIScsiAttemptConfigData.SessionConfigData.TargetInfoFromDhcp = (BOOLEAN)TargetData->TargetDhcpEnabled;

    // Copy TargetIp
    CopyMem(&gRsdIScsiAttemptConfigData.SessionConfigData.TargetIp, &TargetData->TargetIpAddress, sizeof(TargetData->TargetIpAddress));

    // Copy TargetPort
    CopyMem(&gRsdIScsiAttemptConfigData.SessionConfigData.TargetPort, &TargetData->TargetPort, sizeof(TargetData->TargetPort));

    // Copy BootLun
    CopyMem(gRsdIScsiAttemptConfigData.SessionConfigData.BootLun, &TargetData->TargetLun, sizeof(TargetData->TargetLun));

    // Copy CHAP Details.
    gRsdIScsiAttemptConfigData.AuthenticationType = TargetData->AuthenticationMethod;

    if(RSD_ISCSI_NO_CHAP == gRsdIScsiAttemptConfigData.AuthenticationType) {
        gRsdIScsiAttemptConfigData.AuthenticationType = ISCSI_AUTH_TYPE_NONE;
        gRsdIScsiAttemptConfigData.AuthConfigData.CHAP.CHAPType = 0x02;
    } else if (RSD_ISCSI_UNI_CHAP == gRsdIScsiAttemptConfigData.AuthenticationType) {
        gRsdIScsiAttemptConfigData.AuthenticationType = ISCSI_AUTH_TYPE_CHAP;
        gRsdIScsiAttemptConfigData.AuthConfigData.CHAP.CHAPType = ISCSI_CHAP_UNI;
        CopyMem(gRsdIScsiAttemptConfigData.AuthConfigData.CHAP.CHAPName, &TargetData->ChapName, AsciiStrLen(TargetData->ChapName));
        CopyMem(gRsdIScsiAttemptConfigData.AuthConfigData.CHAP.CHAPSecret, &TargetData->ChapSecret, AsciiStrLen(TargetData->ChapSecret));
    } else {
        gRsdIScsiAttemptConfigData.AuthenticationType = ISCSI_AUTH_TYPE_CHAP;
        gRsdIScsiAttemptConfigData.AuthConfigData.CHAP.CHAPType = ISCSI_CHAP_MUTUAL;
        CopyMem(gRsdIScsiAttemptConfigData.AuthConfigData.CHAP.CHAPName, &TargetData->ChapName, AsciiStrLen(TargetData->ChapName));
        CopyMem(gRsdIScsiAttemptConfigData.AuthConfigData.CHAP.CHAPSecret, &TargetData->ChapSecret, AsciiStrLen(TargetData->ChapSecret));
        CopyMem(gRsdIScsiAttemptConfigData.AuthConfigData.CHAP.ReverseCHAPName, &TargetData->MutualChapName, AsciiStrLen(TargetData->MutualChapName));
        CopyMem(gRsdIScsiAttemptConfigData.AuthConfigData.CHAP.ReverseCHAPSecret, &TargetData->MutualChapSecret, AsciiStrLen(TargetData->MutualChapSecret));
    }

    //
    // ATTEMPT DATA
    //

    // Copy ConnectTimeout
    gRsdIScsiAttemptConfigData.SessionConfigData.ConnectTimeout = AttemptData->ConnectionWaitTime;

    // Copy ConnectRetryCount
    gRsdIScsiAttemptConfigData.SessionConfigData.ConnectRetryCount = AttemptData->ConnectionRetryCount;

    // Other default values.
    gRsdIScsiAttemptConfigData.AutoConfigureSuccess         = FALSE;
    gRsdIScsiAttemptConfigData.SessionConfigData.Enabled    = TRUE;     // Always treat as enabled

    // Using variable services save the BMC parsed data 
    Status = RsdIScsiConvertBmcDataToAttemptConfigNvData(RsdAttemptConfigIndex + 1);

    //
    // Set gRsdIScsiInitDone and gRsdIScsiDataAvailable to true to know
    // i) BMC initilization has been done and
    // ii) at least one iSCSI attempt data is available.
    //
    gRsdIScsiInitDone = gRsdIScsiDataAvailable = TRUE;

    return Status;
}

/**
  Checks the MDR region status and tries to acquire the preemptable lock 
  on the iSCSI MDR region if there are no active locks already and then 
  reads each block from the iSCSI MDR region.

  @retval EFI_INVALID_PARAMETER     Any parameter is invalid.
  @retval EFI_SUCCESS               The operation is completed successfully.

**/
EFI_STATUS
EFIAPI
RsdIScsiReadBMCAttemptData()
{
    EFI_STATUS              Status = EFI_NOT_FOUND;

    // MDR Region STATUS Request and Response.
    RSD_GET_MDR_STATUS_REQ  RsdMdrStatReq;
    RSD_GET_MDR_STATUS_RES  *RsdMdrStatResp = (RSD_GET_MDR_STATUS_RES*)NULL;

    // MDR Region LOCK Request and Response.
    UINT8   LockHandle = 0;
    //UINT8   ParamId;

    UINT8   CurrentParamId = VersionBlock;

    // Check the status of the iSCSI Boot options region.
    RsdMdrStatReq.DefiningBodyID = RSD_INTEL_SPEC;
    RsdMdrStatReq.RegionType = RSD_ISCS_BOOT_OPTIONS_REGION;
    
    RsdMdrStatResp = AllocateZeroPool(sizeof(RSD_GET_MDR_STATUS_RES));
    
    if(NULL == RsdMdrStatResp) {
        DEBUG((EFI_D_ERROR,"RSD_ISCSI:IN %a, Memory Allocation failed for RsdMdrStatResp\n", __FUNCTION__));
        return EFI_OUT_OF_RESOURCES;
    }

    Status = GetSmbiosMdrRegionStatus (gIpmiTransport, &RsdMdrStatReq, RsdMdrStatResp);

    DEBUG((DEBUG_INFO,"RSD_ISCI_2_2:In %a, GetSmbiosMdrRegionStatus Status :: %r \n", __FUNCTION__, Status));
    if(EFI_ERROR(Status)) {
        DEBUG((EFI_D_ERROR,"RSD_ISCI_2_2:In %a, GetSmbiosMdrRegionStatus failed. Status :: %r \n", __FUNCTION__, Status));
        return Status;
    }

    // On success check the following
    // 1. Data region identifier is iSCSI Boot Options or not &&
    // 2. Data is valid or not &&
    // 3. If it is in unlocked state or not
    if((RSD_ISCS_BOOT_OPTIONS_REGION != (RsdMdrStatResp->RegionType)) ||
            (RSD_VALID_DATA_REGION != (RsdMdrStatResp->DataValid)) ||
            (Unlocked != (RsdMdrStatResp->LockStatus))) {
        //
        // If above conditions are not met, then the region is being used by some other and should not be disturbed.
        //
        DEBUG((EFI_D_ERROR,"RSD_ISCI_2_2:In %a, Region is being used by someone else.\n", __FUNCTION__));
        return EFI_NOT_READY;
    }
    //
    // Save the used iSCSI region
    //
    gRsdIScsiUsedSize = gRsdIScsiDataLeft = RsdMdrStatResp->UsedRegionSize;

    // If everything is okay, try to acquire a preemptible lock to read.
    Status = GetMdrLockHandle(
            gIpmiTransport,
            &LockHandle,
            PreemptibleLock,
            RSD_ISCS_BOOT_OPTIONS_REGION,
            RSD_ISCSI_MAX_TIMEOUT);

    DEBUG((DEBUG_INFO,"RSD_ISCI_2_2:In %a, GetMdrLockHandle for connection establishment Status :: %r, LockHandle :: 0x%X \n", __FUNCTION__, Status, LockHandle));
    if(EFI_ERROR(Status)) {
        DEBUG((EFI_D_ERROR,"RSD_ISCI_2_2:In %a, GetMdrLockHandle failed. Status :: %r \n", __FUNCTION__, Status));
        return Status;
    }

    if(!EFI_ERROR(Status) &&
            gRsdIScsiUsedSize > RSD_ISCSI_VERISON_MAX_SIZE) {
        Status = RsdIScsiReadUsedSize();
        DEBUG((DEBUG_INFO,"RSD_ISCI_2_2:In %a, RsdIScsiReadUsedSize Status :: %r \n", __FUNCTION__, Status));
    }

    //
    // Once the reading is successful, call Abort to release the lock.
    //
    GetMdrLockHandle(
            gIpmiTransport,
            &LockHandle,
            AbortLock,
            RSD_ISCS_BOOT_OPTIONS_REGION,
            RSD_ISCSI_ABORT_SESSION);

    return Status;
}

/**
  This is the parent function that is responsible for 
  i) Initializing the common parameters,
  ii) Locates EFI_IPMI_TRANSPORT protocol,
  iii) Issuing MDR read requests and
  iv) Framing the ISCSI_ATTEMPT_CONFIG_NVDATA from the BMC data.

  @param[in]  VOID

  @param[out] EFI_SUCCESS               If data is read from BMC successfully and 
                                        gRsdIScsiAttemptConfigData is initialized.
  @param[out] EFI_NOT_FOUND             The data needed is not present in the list or
                                        EFI_IPMI_TRANSPORT is not found.
  @param[out] EFI_INAVLID_PARAMETER     BMC command execution failed.
**/
EFI_STATUS
EFIAPI
RsdIScsiInitializeBmcInformation()
{
    EFI_STATUS              Status = EFI_NOT_READY;
    UINT8                   CurrentAttempt = 0;
    RSD_ISCSI_INITIATOR     *InitData;
    RSD_ISCSI_TARGET        *TargetData;
    RSD_ISCSI_ATTEMPT       *AttemptData;

    ZeroMem(&gRsdIScsiAttemptConfigData, sizeof(ISCSI_ATTEMPT_CONFIG_NVDATA));

    //
    // Fill the common REQUEST parameters which are i) DefiningBodyID and ii) RegionType
    // Also initialize RootHandle which stores multiple units of Initiator and Target data.
    //
    if (!gRsdIScsiIpmiFound)
    {
        Status = RsdIScsiInitializeReadRequest();
        if(EFI_ERROR(Status)) {
            DEBUG((EFI_D_ERROR,"\nRSD_ISCI_2_2:In %a, RsdIScsiInitializeReadRequest failed. Status = %r\n", __FUNCTION__, Status));
            return Status;
        }
    }

    Status = RsdIScsiReadBMCAttemptData();
    DEBUG((DEBUG_INFO,"\nRSD_ISCI_2_2:In %a, RsdIScsiReadBMCAttemptData Status :: %r \n", __FUNCTION__, Status));
    if(EFI_ERROR(Status))
    {
        DEBUG((EFI_D_ERROR,"\nRSD_ISCI_2_2:In %a, RsdIScsiReadBMCAttemptData failed. Status :: %r \n", __FUNCTION__, Status));
        return Status;
    }

    //
    // BMC data is retrieved successfully. Now choose the attempt data to be used.
    //
    while(CurrentAttempt < gRsdIScsiAttemptCount) {
      //
      // Get iSCSI Attempt data first.
      //
      AttemptData = (RSD_ISCSI_ATTEMPT*)RsdIScsiGetDataByHandle(gRsdAttemptHandles[CurrentAttempt]);
      if(NULL == AttemptData) {
       return EFI_NOT_FOUND;
      }

      //
      // Get Initiator data now.
      //
      InitData = (RSD_ISCSI_INITIATOR*)RsdIScsiGetDataByHandle(AttemptData->InitiatorHandle);
      if(NULL == InitData) {
          ++CurrentAttempt;
           continue;
           //return EFI_NOT_FOUND;
      }

      //
      // Get Target data now.
      //
      TargetData = (RSD_ISCSI_TARGET*)RsdIScsiGetDataByHandle(AttemptData->TargetHandle);
      if(NULL == TargetData) {
          ++CurrentAttempt;
           continue;
           //return EFI_NOT_FOUND;
      }
      //
      // Now we have the combination of data to be used for iSCSI Connection. Combine the data
      // and pass it to store into NVRAM if there are multiple attempt config data available.
      //
      Status = RsdIScsiFrameAttemptConfigData(AttemptData, InitData, TargetData,CurrentAttempt);
      DEBUG((DEBUG_INFO,"\nRSD_ISCI_2_2:In %a, RsdIScsiFrameAttemptConfigData Status :: %r", __FUNCTION__, Status));
      if(EFI_ERROR(Status) && !gRsdIScsiInitDone && 
                (CurrentAttempt == gRsdIScsiAttemptCount)) {
            DEBUG((EFI_D_ERROR,"\nRSD_ISCI_2_2:In %a, RsdIScsiFrameAttemptConfigData Status :: %r", __FUNCTION__, Status));
            return Status;
        }
        ++CurrentAttempt;
    }
    return Status;
}

/**
  This function determines whether there is any attempt data available in BMC.

  @param[in]  VOID

  @param[out]  BOOLEAN      Value of gRsdIScsiDataAvailable.

**/
BOOLEAN
EFIAPI
RsdIScsiIsBmcDataAvailable()
{
    EFI_STATUS  Status;
    // Initialize BMC information if first call.
    if(!gRsdIScsiInitDone)
    {
        Status = RsdIScsiInitializeBmcInformation();

            if(NULL != gRsdIScsiRootHandle)
                RsdIScsiDeleteByHandle(TRUE, 0);

    }

    // return the BMC information availability status.
    return gRsdIScsiDataAvailable;
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
