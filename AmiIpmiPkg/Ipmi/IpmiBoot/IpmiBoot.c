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

/** @file IpmiBoot.c
    Sets the Boot Option Priority based on the IPMI Boot Flags

**/

//-----------------------------------------------------------------------

#include "IpmiBoot.h"

//-----------------------------------------------------------------------

//
// Global Variables
//
EFI_IPMI_TRANSPORT  *gIpmiTransport;
BOOLEAN             gPersistentBoot;
BOOLEAN             gUefiBoot;
UINT8               gDeviceInstanceSelector;
IPMI_BOOT_DEVICE    gIpmiForceBootDevice;

//
// Extern Variables
//

extern  DLIST                   *BootOptionList;

/**
    This function reads the Boot Info Acknowledge data bytes from BMC.

    @param BootInfoAcknowledge Buffer to return
                Boot Info Acknowledge.

    @retval EFI_SUCCESS Read Boot Info Acknowledge data successfully.

**/

EFI_STATUS
IpmiGetBootInfoAcknowledgeData(
  IN IPMI_BOOT_INFO_ACKNOWLEDGE        *Param4 )
{
    EFI_STATUS                              Status;
    UINT8                                   ResponseDataSize;
    IPMI_GET_BOOT_OPTIONS_REQUEST           GetRequest;
    IPMI_BOOT_INFO_ACKNOWLEDGE_RESPONSE     BootInfoAcknowledgeResponse;

    //
    // Wait until Set In Progress field is cleared
    //
    Status = IpmiWaitSetInProgressClear ();
    if (EFI_ERROR (Status)) {
        return Status;
    }

    ResponseDataSize = sizeof (BootInfoAcknowledgeResponse);

    GetRequest.ParameterSelector = BootInfoAck;
    GetRequest.Reserved = 0x0;
    GetRequest.SetSelector = 0x0;
    GetRequest.BlockSelector = 0x0;

    Status = gIpmiTransport->SendIpmiCommand (
                 gIpmiTransport,
                 IPMI_NETFN_CHASSIS,
                 0,
                 IPMI_CHASSIS_GET_SYSTEM_BOOT_OPTIONS,
                 (UINT8*) &GetRequest,
                 sizeof (GetRequest),
                 (UINT8*)&BootInfoAcknowledgeResponse,
                 &ResponseDataSize);
    if (!EFI_ERROR(Status)) {
        EfiCopyMem(
            Param4,
            &(BootInfoAcknowledgeResponse.Param4),
            sizeof(BootInfoAcknowledgeResponse.Param4));
    }

    return Status;
}

#ifdef CSM_SUPPORT
/**
    This function saves the Legacy device order details in NVRAM.

    @param  VOID.

    @retval VOID.
**/

VOID
SaveLegacyDeviceOrderData (
  VOID )
{
    UINT8           *DeviceOrderBuffer;
    UINTN           DeviceOrderBufferSize;
    EFI_STATUS      Status;

    //
    // Getting LegacyDevOrder variable and save in IpmiLegacyDevOrder variable.
    //
    Status = GetVariable2 (
                L"LegacyDevOrder",
                &LegacyDevOrderGuid,
                (VOID**)&DeviceOrderBuffer,
                &DeviceOrderBufferSize);
    if (!EFI_ERROR(Status)) {
        Status = gRT->SetVariable (
                          L"IpmiLegacyDevOrder",
                          &gEfiIpmiBootGuid,
                          VAR_FLAG,
                          DeviceOrderBufferSize,
                          DeviceOrderBuffer);
        if (EFI_ERROR (Status)) {
            SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI Boot: IpmiLegacyDevOrder SetVariable fails.\n"));
        }
        gBS->FreePool (DeviceOrderBuffer); // Freeing the memory allocated in GetVariable2().
        DeviceOrderBuffer = NULL;
    }

    //
    // Getting OldLegacyDevOrder variable and save in IpmiOldLegacyDevOrder variable.
    //
    Status = GetVariable2 (
                L"OldLegacyDevOrder",
                &LegacyDevOrderGuid,
                (VOID**)&DeviceOrderBuffer,
                &DeviceOrderBufferSize);
    if (!EFI_ERROR(Status)) {
        Status = gRT->SetVariable (
                          L"IpmiOldLegacyDevOrder",
                          &gEfiIpmiBootGuid,
                          VAR_FLAG,
                          DeviceOrderBufferSize,
                          DeviceOrderBuffer);
        if (EFI_ERROR (Status)) {
            SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI Boot: IpmiOldLegacyDevOrder SetVariable fails.\n"));
        }
        gBS->FreePool (DeviceOrderBuffer); // Freeing the memory allocated in GetVariable2().
    }
}
#endif

/**
    This function saves the matched boot option details in NVRAM.

    @param BootOptionNumber Boot option number to save the 
                            entire structure in NVRAM.

    @retval VOID
**/

VOID
SaveMatchedBootOptionData(
  IN UINT16 BootOptionNumber )
{
    //
    // Saving the matched Boot option details in NVRAM variable
    // for legacy case
    //

    CHAR16          BootStr[9];
    UINTN           NvramOptionSize;
    VOID            *Buffer;
    UINT8           *MatchedBootOptionDetails;
    EFI_STATUS      Status;

    Swprintf (BootStr, L"Boot%04X", BootOptionNumber);
    Status = GetVariable2 (
                BootStr,
                &EfiVariableGuid,
                &Buffer,
                &NvramOptionSize);
    if (!EFI_ERROR(Status)) {
        MatchedBootOptionDetails = AllocateZeroPool (NvramOptionSize + BOOT_OPTION_NUMBER_SIZE);
        if (MatchedBootOptionDetails != NULL) {
            *(UINT16 *)MatchedBootOptionDetails = BootOptionNumber;
            EfiCopyMem ((MatchedBootOptionDetails + BOOT_OPTION_NUMBER_SIZE), \
                Buffer, NvramOptionSize);

            Status = gRT->SetVariable (
                              L"IpmiMatchBootOptionData",
                              &gEfiIpmiBootGuid,
                              VAR_FLAG,
                              (NvramOptionSize + BOOT_OPTION_NUMBER_SIZE),
                              MatchedBootOptionDetails);
            if (EFI_ERROR (Status)) {
                SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI Boot: IpmiMatchBootOptionData SetVariable fails.\n"));
            }
            gBS->FreePool (MatchedBootOptionDetails);
        }
        gBS->FreePool (Buffer); // Freeing the memory allocated in GetVariable2().
    }

#ifdef CSM_SUPPORT
    SaveLegacyDeviceOrderData();
#endif
}

/**
    This function checks for the given device path type and sub type
    in the device path returns TRUE if present and FALSE if not.

    @param pDp The device path to be checked.
    @param DpType Type to be checked in Device path nodes
    @param DpSubType SubType to be checked in Device path nodes

    @retval BOOLEAN Returns TRUE if present or FALSE
**/

BOOLEAN 
CheckNodeInDevicePath (
  IN  EFI_DEVICE_PATH_PROTOCOL  *pDp,
  IN  UINT16  DpType,
  IN  UINT16  DpSubType )
{

    if (!pDp)
        return FALSE;

    for (;!isEndNode(pDp); pDp = NEXT_NODE(pDp)) {
         if (((pDp->Type == DpType) && (pDp->SubType == DpSubType)))
             return TRUE;
    }
    return FALSE;
}

/**
    Save the IPMI Set Boot order to NVRAM Variable if IPMI forced boot happened

    @param VOID

    @retval VOID
**/

VOID
EFIAPI
SaveIPMISetBootOrder(
  VOID )
{
    UINT16          *CurrBootOrder = NULL;
    UINTN           CurrBootOrderSize;
    VOID*           Buffer = NULL;
    UINTN           BufferSize;
    EFI_STATUS      Status;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI Boot: Entered %a\n", __FUNCTION__));
    //
    // Checking if IPMI Force boot requested
    //
    Status = GetVariable2 (
                L"BootOrderBeforeIpmiBoot",
                &gEfiIpmiBootGuid,
                &Buffer,
                &BufferSize);
    if (!EFI_ERROR(Status)) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI Boot: Saving IPMI Set BootOrder.\n"));
        //
        // Saving Boot order Set by IPMI
        //
        Status = GetVariable2 (
                    L"BootOrder",
                    &EfiVariableGuid,
                    &CurrBootOrder,
                    &CurrBootOrderSize);
        if (!EFI_ERROR(Status)) {
            Status = gRT->SetVariable (
                             L"IpmiSetBootOrder",
                             &gEfiIpmiBootGuid,
                             VAR_FLAG,
                             CurrBootOrderSize,
                             CurrBootOrder);
            gBS->FreePool (CurrBootOrder);
        }
        gBS->FreePool (Buffer); // Freeing the memory allocated in GetVariable2().
    }
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI Boot: Exiting SaveIPMISetBootOrder.\n"));
    return;
}

#if CSM_SUPPORT
/**
    Check if IPMI forced device priority for legacy differ
    from Current device priority for legacy.

    @param VOID

    @return BOOLEAN 
    @retval TRUE If Device priority changed by User
    @retval FALSE if Device priority not Changed by User
**/

BOOLEAN
CheckDevicePriorityChange(
VOID )
{
    VOID                *Buffer1 = NULL;
    VOID                *Buffer2 = NULL;
    LEGACY_DEVICE_ORDER *DevOrder = NULL;
    LEGACY_DEVICE_ORDER *OldDevOrder = NULL;
    UINT16*             OldDev = NULL;
    UINT16*             NewDev = NULL;
    UINTN               Length = 0;
    UINTN               BufferSize = 0;
    UINTN               CurrSize = 0;
    EFI_STATUS          Status;

    //
    // Getting Current Legacy device order. If user change Legacy Priorities it will be reflected.
    //
    Status = GetVariable2 (
                L"LegacyDevOrder",
                &LegacyDevOrderGuid,
                &Buffer1,
                &BufferSize);
    if (!EFI_ERROR(Status)) {
        //
        // Getting Old Legacy device order
        //
        Status = GetVariable2 (
                    L"OldLegacyDevOrder",
                    &LegacyDevOrderGuid,
                    &Buffer2,
                    &BufferSize);
        if (!EFI_ERROR(Status)) {
            //
            // Checking Current Legacy Device order is same as Old Legacy device order.
            // If user changes BBS priorities then it will not be same
            //
            DevOrder = (LEGACY_DEVICE_ORDER *)Buffer1;
            OldDevOrder = (LEGACY_DEVICE_ORDER *)Buffer2;
            do {
                //
                // Getting Length of device data.
                // Each device denoted by UINT16 type.
                // Length = sizeof(Length) + sizeof(Device1) + ... + sizeof(DeviceN).
                // For two device, Length = 6. ( 2 + 2 + 2)
                //
                Length = DevOrder->Length - 2;
    
                //
                // Getting device number pointer
                //
                OldDev = OldDevOrder->Device;
                NewDev = DevOrder->Device;
                //
                //Checking each device number using Length field
                //Each two bytes denotes device number
                //
                while (Length) {
                    //
                    // Checking Current device number is same as Old device number
                    //
                    if( *NewDev != *OldDev) {
                        return TRUE;
                    }
                    Length = Length - 2;
                    NewDev++; // Moving to next device number
                    OldDev++;
                }
                CurrSize = CurrSize + ((UINT8*)NewDev - (UINT8*)DevOrder); // Getting completed Legacy device type data size
                DevOrder = (LEGACY_DEVICE_ORDER *)NewDev; // Moving to next Legacy device type
                OldDevOrder = (LEGACY_DEVICE_ORDER *)OldDev;
            } while(CurrSize < BufferSize);
            gBS->FreePool (Buffer2); // Freeing the memory allocated in GetVariable2().
        }
        gBS->FreePool (Buffer1); // Freeing the memory allocated in GetVariable2().
    }
    return FALSE;
}
#endif //#if CSM_SUPPORT

/**
    Clear the IPMI Boot NVRAM Variables if IPMI forced boot order or device differ
    from Current boot order or device.

    @param VOID

    @retval VOID
**/

VOID
EFIAPI
ClearIPMIBootVariable (
  VOID )
{
    EFI_STATUS      Status;
    UINT16          *IpmiBootOrder = NULL;
    UINTN           IpmiBootOrderSize;
    UINT16          *CurrBootOrder = NULL;
    UINTN           CurrBootOrderSize;
    BOOLEAN         BootOrderChange = FALSE;
    UINTN           Index = 0;
    VOID*           Buffer = NULL;
    UINTN           BufferSize;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI Boot: Entered %a\n", __FUNCTION__));
    //
    // Getting the boot order set by IPMI if previous boot is IPMI boot 
    //
    Status = GetVariable2 (
                L"IpmiSetBootOrder",
                &gEfiIpmiBootGuid,
                &IpmiBootOrder,
                &IpmiBootOrderSize);
    if (!EFI_ERROR(Status)) {
        //
        // Clear the "IpmiSetBootOrder" NVRAM Variable
        //
        Status = gRT->SetVariable (
                         L"IpmiSetBootOrder",
                         &gEfiIpmiBootGuid,
                         VAR_FLAG,
                         0,
                         NULL);
    } else {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI Boot: Exiting as IPMI Set Boot Order not available.\n"));
        return;
    }
    //
    // Getting Current boot order
    //
    Status = GetVariable2 (
                L"BootOrder",
                &EfiVariableGuid,
                &CurrBootOrder,
                &CurrBootOrderSize);
    if (!EFI_ERROR(Status)) {
        //
        // Check if IPMI Set boot order different from Current boot order
        //
        while (Index < (CurrBootOrderSize / sizeof (UINT16))) {
            if (IpmiBootOrder[Index] != CurrBootOrder[Index]) {
                BootOrderChange = TRUE;
                SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI Boot: Boot Order Changed by user.\n"));
            }
            Index = Index + 1;
        }
        gBS->FreePool (CurrBootOrder);
    }
    gBS->FreePool (IpmiBootOrder);  // Free the memory allocated for IpmiBootOrder.
#if CSM_SUPPORT
    if(!BootOrderChange) {
        if (CheckDevicePriorityChange ()) {
            BootOrderChange = TRUE;
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI Boot: Legacy device priority Changed by user.\n"));
        }
    }
#endif //#if CSM_SUPPORT
    if (BootOrderChange) {
        //
        // Clear "IpmiMatchBootOptionData" NVRAM Variable if the variable exists.
        //
        Status = GetVariable2 (
                    L"IpmiMatchBootOptionData",
                    &gEfiIpmiBootGuid,
                    &Buffer,
                    &BufferSize);
        if (!EFI_ERROR(Status)) {
            Status = gRT->SetVariable (
                            L"IpmiMatchBootOptionData",
                            &gEfiIpmiBootGuid,
                            VAR_FLAG,
                            0,
                            NULL);
            gBS->FreePool (Buffer);
        }
        //
        // Clear "IpmiBootFlags" NVRAM Variable if the variable exists.
        //
        Status = GetVariable2 (
                    L"IpmiBootFlags",
                    &gEfiIpmiBootGuid,
                    &Buffer,
                    &BufferSize);
        if (!EFI_ERROR(Status)) {
            Status = gRT->SetVariable (
                            L"IpmiBootFlags",
                            &gEfiIpmiBootGuid,
                            VAR_FLAG,
                            0,
                            NULL);
            gBS->FreePool (Buffer);
        }
        //
        // Clear the "BootOrderBeforeIpmiBoot" NVRAM Variable if the variable exists.
        //
        Status = GetVariable2 (
                    L"BootOrderBeforeIpmiBoot",
                    &gEfiIpmiBootGuid,
                    &Buffer,
                    &BufferSize);
        if (!EFI_ERROR(Status)) {
            Status = gRT->SetVariable (
                                L"BootOrderBeforeIpmiBoot",
                                &gEfiIpmiBootGuid,
                                VAR_FLAG,
                                0,
                                NULL);
            gBS->FreePool (Buffer);
        }
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI Boot: Exiting %a\n", __FUNCTION__));
 
    return;
}

/**
    Restore the Boot Order if last boot was an IPMI forced boot

    @param VOID

    @retval VOID
**/

VOID
EFIAPI
RestoreBootorder (
  VOID )
{
    UINT16          *BootOrder;
    UINTN           BootOrderSize;
    UINTN           BootOptionSize;
    CHAR16          TempBootStr[9];
    VOID            *Buffer;
    EFI_STATUS      Status;
#ifdef CSM_SUPPORT
    UINT32          Attributes = 0;
#endif

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI Boot: Entered %a\n", __FUNCTION__));
    //
    // Restoring the matched boot option details into the respective 
    // boot option
    //
    Status = GetVariable2 (
                L"IpmiMatchBootOptionData",
                &gEfiIpmiBootGuid,
                &Buffer,
                &BootOptionSize);
    if (!EFI_ERROR(Status)) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI Boot: IpmiMatchBootOptionData Available.\n"));
        Swprintf (TempBootStr, L"Boot%04X", *((UINT16 *)Buffer));
        Status = gRT->SetVariable (
                          TempBootStr,
                          &EfiVariableGuid,
                          BOOT_VARIABLE_ATTRIBUTES,
                          (BootOptionSize - BOOT_OPTION_NUMBER_SIZE),
                          ((UINT8 *)Buffer + BOOT_OPTION_NUMBER_SIZE));
        //
        // Clear the "IpmiMatchBootOptionData" NVRAM Variable
        //
        if (!EFI_ERROR(Status)) {
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI Boot: Clearing IpmiMatchBootOptionData.\n"));
            Status = gRT->SetVariable (
                              L"IpmiMatchBootOptionData",
                              &gEfiIpmiBootGuid,
                              VAR_FLAG,
                              0,
                              NULL);
        }
        gBS->FreePool (Buffer); // Freeing the memory allocated in GetVariable2().
        Buffer = NULL;
    }

#ifdef CSM_SUPPORT

    BootOptionSize = 0;

    //
    // Get the attributes for LegacyDevOrder variable and allocate the buffer if Buffer is too small.
    //
    Status = gRT->GetVariable (
                    L"LegacyDevOrder",
                    &LegacyDevOrderGuid,
                    &Attributes,
                    &BootOptionSize,
                    Buffer);
    if (!Attributes && (Status == EFI_BUFFER_TOO_SMALL)) {
        Buffer = AllocateZeroPool (BootOptionSize);
        Status = gRT->GetVariable (
                        L"LegacyDevOrder",
                        &LegacyDevOrderGuid,
                        &Attributes,
                        &BootOptionSize,
                        Buffer);
    }
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI Boot: Get LegacyDevOrder variable Attributes:%X Buffer:%X.\n", Attributes, Buffer));
    if (Attributes) {
        if (Buffer != NULL) {
            gBS->FreePool (Buffer); // Freeing the memory allocated for Buffer.
        }
        Status = GetVariable2 (
                    L"IpmiLegacyDevOrder",
                    &gEfiIpmiBootGuid,
                    &Buffer,
                    &BootOptionSize);
        if (!EFI_ERROR(Status)) {
            //
            // Restoring the Legacy device order details into the "LegacyDevOrder" variable.
            //
            Status = gRT->SetVariable (
                              L"LegacyDevOrder",
                              &LegacyDevOrderGuid,
                              Attributes,
                              BootOptionSize,
                              ((UINT8 *)Buffer));
            //
            // Clear the "IpmiLegacyDevOrder" NVRAM Variable
            //
            if (!EFI_ERROR(Status)) {
                Status = gRT->SetVariable (
                                  L"IpmiLegacyDevOrder",
                                  &gEfiIpmiBootGuid,
                                  VAR_FLAG,
                                  0,
                                  NULL);
                if (!EFI_ERROR(Status)) {
                    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI Boot: Restored LegacyDevOrder and Cleared IpmiLegacyDevOrder.\n"));
                }
            }
            gBS->FreePool (Buffer); // Freeing the memory allocated in GetVariable2().
        } // if (!EFI_ERROR(Status))
        Attributes = 0;
    } // if (Attributes)

    BootOptionSize = 0;
    Buffer = NULL;

    //
    // Get the attributes for OldLegacyDevOrder variable.
    //
    Status = gRT->GetVariable (
                    L"OldLegacyDevOrder",
                    &LegacyDevOrderGuid,
                    &Attributes,
                    &BootOptionSize,
                    Buffer);
    if (!Attributes && (Status == EFI_BUFFER_TOO_SMALL)) {
        Buffer = AllocateZeroPool (BootOptionSize);
        Status = gRT->GetVariable (
                        L"OldLegacyDevOrder",
                        &LegacyDevOrderGuid,
                        &Attributes,
                        &BootOptionSize,
                        Buffer);
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI Boot: Get OldLegacyDevOrder variable Attributes:%X Buffer:%X.\n", Attributes, Buffer));
    if (Attributes) {
        if (Buffer != NULL) {
            gBS->FreePool (Buffer); // Freeing the memory allocated for Buffer.
        }
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI Boot: Get OldLegacyDevOrder variable Attributes:%X Buffer:%X.\n", Attributes, Buffer));
        //
        // Restoring the Old Legacy device order details into the "OldLegacyDevOrder" variable.
        //
        Status = GetVariable2 (
                    L"IpmiOldLegacyDevOrder",
                    &gEfiIpmiBootGuid,
                    &Buffer,
                    &BootOptionSize);
        if (!EFI_ERROR(Status)) {
            Status = gRT->SetVariable (
                              L"OldLegacyDevOrder",
                              &LegacyDevOrderGuid,
                              Attributes,
                              BootOptionSize,
                              ((UINT8 *)Buffer));
            //
            // Clear the "IpmiOldLegacyDevOrder" NVRAM Variable
            //
            if (!EFI_ERROR(Status)) {
                Status = gRT->SetVariable (
                                  L"IpmiOldLegacyDevOrder",
                                  &gEfiIpmiBootGuid,
                                  VAR_FLAG,
                                  0,
                                  NULL);
                if (!EFI_ERROR(Status)) {
                    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI Boot: Restored OldLegacyDevOrder and Cleared IpmiOldLegacyDevOrder.\n"));
                }
            }
            gBS->FreePool (Buffer); // Freeing the memory allocated in GetVariable2().
        } // if (!EFI_ERROR(Status))
    } // if (Attributes)
#endif

    //
    // Get the previous boot order if the last boot was an IPMI forced boot
    //
    Status = GetVariable2 (
                L"BootOrderBeforeIpmiBoot",
                &gEfiIpmiBootGuid,
                &BootOrder,
                &BootOrderSize);
    if (!EFI_ERROR(Status)) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI Boot: Restoring Original boot order.\n"));
        //
        // Save the restored boot order back to NVRAM now too.
        // Any modifications to the boot order will get saved again at the end of BDS.
        //
        Status = gRT->SetVariable (
                          L"BootOrder",
                          &EfiVariableGuid,
                          BOOT_VARIABLE_ATTRIBUTES,
                          BootOrderSize,
                          BootOrder);
        ASSERT_EFI_ERROR(Status);

        //
        // Clear the "BootOrderBeforeIpmiBoot" NVRAM Variable
        //

        Status = gRT->SetVariable (
                          L"BootOrderBeforeIpmiBoot",
                          &gEfiIpmiBootGuid,
                          VAR_FLAG,
                          0,
                          NULL);
        gBS->FreePool (BootOrder); // Freeing the memory allocated in GetVariable2().
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI Boot: Exiting %a\n", __FUNCTION__));
    return;
}

/**
    Sets the Boot Option Priority based on the IPMI Boot Flags

    @param VOID

    @retval VOID
**/

VOID
EFIAPI
SetIpmiBootOptionPriorities (
  VOID )
{
    EFI_STATUS                      Status;
    IPMI_BOOT_INFO_ACKNOWLEDGE      Param4;
    GET_BOOT_FLAG_RESPONSE          BootFlags;
    UINTN                           BootFlagsDataSize;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "\n IPMI Boot: Entered %a\n", __FUNCTION__));

    //
    // Locate the IPMI Transport Protocol and return if status is not success
    //
    Status = gBS->LocateProtocol (
                 &gEfiDxeIpmiTransportProtocolGuid,
                 NULL,
                 (VOID **)&gIpmiTransport );
    SERVER_IPMI_DEBUG((DEBUG_INFO, "IPMI Boot: LocateProtocol gEfiDxeIpmiTransportProtocolGuid. Status: %r\n", Status));
    if ( EFI_ERROR (Status) ) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "SetIpmiBootOptionPriorities: Error in locating \
                        the IPMI Transport Protocol. Status: %r\n", Status));
        return ;
    }

    //
    // Read the boot info acknowledge bytes from BMC
    //
    Status = IpmiGetBootInfoAcknowledgeData (&Param4);

    if ( EFI_ERROR(Status) ) {
        return;
    }

    //
    // Support IPMI boot only if BootInitiatorAcknowledgeData is zero
    // or BiosOrPostBit is set in BootInitiatorAcknowledgeData
    //
    if ((Param4.BootInitiatorAcknowledgeData.RawData == 0)
        || (Param4.BootInitiatorAcknowledgeData.BitFields.BiosOrPostBit)) {

        gIpmiForceBootDevice = 0;

        //
        // Read the boot flag bytes from BMC
        //
        Status = IpmiGetBootFlags (&BootFlags);

        if (EFI_ERROR(Status)) {
            return;
        }
        if ( BootFlags.Param5.BootFlagValid ) {

            //
            // Update the IpmiBoot global variables
            //
            UpdateIpmiBootGlobalVariables (BootFlags);

            //
            // If persistent boot, save the Boot flags into NVRAM to use it in further boots
            //
            if ( gPersistentBoot ) {
                Status = gRT->SetVariable (
                             L"IpmiBootFlags",
                             &gEfiIpmiBootGuid,
                             VAR_FLAG,
                             sizeof(GET_BOOT_FLAG_RESPONSE),
                             &BootFlags);
            } else {
                //
                // If Non Persistent boot, Clear the BootFlags NVRAM data stored in previous persistent boots.
                //
                BootFlagsDataSize = sizeof (GET_BOOT_FLAG_RESPONSE);
                Status = gRT->GetVariable (L"IpmiBootFlags",
                             &gEfiIpmiBootGuid,
                             NULL,
                             &BootFlagsDataSize,
                             &BootFlags);

                if (!EFI_ERROR(Status)) {
                    Status = gRT->SetVariable (
                                 L"IpmiBootFlags",
                                 &gEfiIpmiBootGuid,
                                 VAR_FLAG,
                                 0,
                                 &BootFlags);
                }
            }
            //
            // Write to Boot Info Acknowledge "BIOS/POST has handled boot info"
            //
            Status = IpmiSetBootInfoAck (BOOT_INFO_HANDLED_BY_BIOS);

            //
            // Clear the BMC BootFlags
            //
            if ( !EFI_ERROR(Status)) {
                Status = IpmiClearBootFlags ();
            }

            if ( EFI_ERROR(Status) || gIpmiForceBootDevice == NoOverride ) {
                return;
            }
        } else {
            //
            // Get the IpmiBootFlags NVRAM Variable stored in previous persistent boots.
            //
            BootFlagsDataSize = sizeof(GET_BOOT_FLAG_RESPONSE);
            Status = gRT->GetVariable (
                         L"IpmiBootFlags",
                         &gEfiIpmiBootGuid,
                         NULL,
                         &BootFlagsDataSize,
                         &BootFlags );

            if ( !EFI_ERROR(Status)) {
                //
                // Update the IpmiBoot global variables
                //
                UpdateIpmiBootGlobalVariables (BootFlags);
            }

            if ( EFI_ERROR(Status) || (gIpmiForceBootDevice == NoOverride) ) {
                return;
            }
        }

    } else {
        //
        // Clear the stored NVRAM BootFlags data, if any fields other than BIOS/POST
        // bit is selected in BootInfoAcknowledge data.
        //
        BootFlagsDataSize = sizeof(GET_BOOT_FLAG_RESPONSE);
        Status = gRT->GetVariable (
                     L"IpmiBootFlags",
                     &gEfiIpmiBootGuid,
                     NULL,
                     &BootFlagsDataSize,
                     &BootFlags);

        if (!EFI_ERROR(Status)) {
            Status = gRT->SetVariable (
                         L"IpmiBootFlags",
                         &gEfiIpmiBootGuid,
                         VAR_FLAG,
                         0,
                         &BootFlags);
        }

        return;
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "\n IPMI Boot: Setting requested Boot option to High priority\n"));
    //
    // Currently PXE, HDD, CdRom and Floppy are supported for IPMI boot
    //

    switch ( gIpmiForceBootDevice ) {

        case ForcePxe:
            Status = SetBootOptionToHighestPriority (
                         BBS_BEV_DEVICE,
                         MESSAGING_DEVICE_PATH,
                         MSG_MAC_ADDR_DP );
            if ( (Status == EFI_NOT_FOUND) && (!gUefiBoot) ) {
                Status = SetBootOptionToHighestPriority (
                             BBS_EMBED_NETWORK,
                             0,
                             0 );
            } 
            break;

        case ForceBootHardDrive:
            Status = SetBootOptionToHighestPriority (
                         BBS_HARDDISK,
                         MEDIA_DEVICE_PATH,
                         MEDIA_HARDDRIVE_DP );
            break;

        case ForceBootHardDriveSafeMode:
            Status = SetBootOptionToHighestPriority (
                         BBS_HARDDISK,
                         MEDIA_DEVICE_PATH,
                         MEDIA_HARDDRIVE_DP);
            break;

        case ForceBootDiagnosticPartition:
            Status = SetBootOptionToHighestPriority (
                         BBS_HARDDISK,
                         MEDIA_DEVICE_PATH,
                         MEDIA_HARDDRIVE_DP);
            break;
        case ForceBootCdDvd:
            Status = SetBootOptionToHighestPriority (
                         BBS_CDROM,
                         MEDIA_DEVICE_PATH,
                         MEDIA_CDROM_DP );
            break;

        case ForceBootBiosSetup:
        {
            UINT32 BootFlow = BOOT_FLOW_CONDITION_FIRST_BOOT;
            EFI_GUID guidBootFlow = BOOT_FLOW_VARIABLE_GUID;
            Status = gRT->SetVariable (
                         L"BootFlow",
                         &guidBootFlow,
                         EFI_VARIABLE_BOOTSERVICE_ACCESS,
                         sizeof(BootFlow),
                         &BootFlow );
        }
            break;

        case ForceBootFloppy:
            Status = SetBootOptionToHighestPriority (
                         BBS_FLOPPY,
                         ACPI_DEVICE_PATH,
                         ACPI_DP );
            break;
        //
        // Newly added boot options - START
        //
        case ForceBootRemoteFloppy:
        case ForceBootRemoteCdDvd:
        case ForceBootRemoteHardDrive:
            Status = RemoteDeviceBoot(gIpmiForceBootDevice);
            break;

/*
        case ForceBootPrimaryRemoteMedia:
            Status = SetBootOptionToHighestPriority (
                        BBS_HARDDISK,
                        0,
                        0,
                        TRUE );
            break;
*/

        //
        //Newly added boot options - END
        //
        default:
            Status = EFI_NOT_FOUND;
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, " IPMI Boot: Exiting %a\n", __FUNCTION__));
    return;
}

/**
    Save the current Boot Order for use on the next boot

    @param VOID

    @retval VOID
**/

VOID
SaveCurrentBootorder (
  VOID )
{

    EFI_STATUS          Status = EFI_SUCCESS;
    UINTN               BootOptionCount = 0x00;
    UINTN               Index;
    UINT16              *BootOrderBuffer = NULL;
    BOOT_OPTION         *Option;
    DLINK               *Link;

    BootOrderBuffer = AllocateZeroPool (MAXIMUM_BOOT_OPTION_NUMBER * sizeof (Option->BootOptionNumber));
    if (BootOrderBuffer == NULL) {
        SERVER_IPMI_DEBUG((DEBUG_ERROR, "SaveCurrentBootorder: AllocateZeroPool Fails!!! \n"));
        return;
    }
    //
    // Sorting BootOptionList based on priority - First boot option will be high priority device
    //
    SortList (BootOptionList, ComparePriorityThenBootOptionNumber);

    FOR_EACH_BOOT_OPTION(BootOptionList, Link, Option){
        //
        // Check for valid BootOptionNumber
        //
        if(Option->BootOptionNumber != INVALID_BOOT_OPTION_NUMBER){

            for(Index = 0; Index < BootOptionCount; Index++){
                //
                // Check BootOptionNumber is already available in BootOrderBuffer
                //
                if (*(BootOrderBuffer + Index) == Option->BootOptionNumber) {
                    break;
                }
            }
            if (Index < BootOptionCount) {
                //
                // Current BootOptionNumber is already available in BootOrderBuffer,
                // goto next BootOptionNumber
                //
                continue;
            }
            //
            // Store the BootOptionNumber in BootOrderBuffer
            //
            *(BootOrderBuffer + BootOptionCount) = Option->BootOptionNumber;
            BootOptionCount++;

        } // if(Option->BootOptionNumber != INVALID_BOOT_OPTION_NUMBER)
    } // FOR_EACH_BOOT_OPTION(BootOptionList,Link,Option)

    //
    // Save current Boot Order for use on the next boot
    //
    Status = gRT->SetVariable (
                 L"BootOrderBeforeIpmiBoot",
                 &gEfiIpmiBootGuid,
                 VAR_FLAG,
                 ( sizeof(Option->BootOptionNumber)*BootOptionCount ),
                 BootOrderBuffer);
    
    if (EFI_ERROR(Status)) {
        SERVER_IPMI_DEBUG((DEBUG_ERROR, "SaveCurrentBootorder: BootOrderBeforeIpmiBoot SetVariable Fails!!! \n"));
    }
    gBS->FreePool (BootOrderBuffer);
    return;
}

/**
    Set the highest priority to the requested USB compatible device Boot
    option.

    @param RequestedUsbDeviceInstance USB device instance to
                                      be selected
    @param BbsDevType BBS device Type of the boot option to be selected.
    @param DpType Device path type of the boot option to be selected.
    @param DpSubType Device path sub type of boot option.

    @retval BOOT_OPTION* Returns the address of requested USB boot option address.
**/
BOOT_OPTION *
SetUsbDeviceBootOptionToHighestPriority(
  IN  UINTN                     RequestedUsbDeviceInstance,
  IN  UINT16                    BbsDevType,
  IN  UINT16                    DpType,
  IN  UINT16                    DpSubType )
{
    DLINK                       *Link;
    BOOT_OPTION                 *Option;
    EFI_DEVICE_PATH_PROTOCOL    *OptionDp;
    UINTN                       UsbDeviceCount = 0;

    //
    // Proceed only if Boot Option list is not empty
    //
    if ( !DListEmpty (BootOptionList) ) {

        FOR_EACH_BOOT_OPTION (BootOptionList, Link, Option){
            OptionDp = Option->FilePathList;
            //
            // skip the group headers
            //
            if ( Option->GroupHeader ) {
                continue;
            }
            //
            // Check for BIOS Boot type as UEFI boot and
            // Check for the UEFI boot option (Not Legacy boot option)
            //
            if ( gUefiBoot && (Option->BbsEntry == NULL) && \
                    (Option->BbsIndex == INVALID_BBS_INDEX) ) {
                //
                // Check for floppy controller first as it has different 
                // device path to verify. It has ACPI device path
                //
                if ( (DpType == ACPI_DEVICE_PATH) && (DpSubType == ACPI_DP) ) {
                    if (isAcpiDevicePath( (ACPI_HID_DEVICE_PATH*)OptionDp )) {
                        continue;
                    } else {
                        //
                        // Check for USB floppy
                        //
                        if ( CheckNodeInDevicePath (OptionDp, \
                            MESSAGING_DEVICE_PATH, MSG_USB_DP) ) {

                            UsbDeviceCount = UsbDeviceCount + 1;
                        } else {
                            continue;
                        }
                    }
                } else {
                    //
                    // Check for other UEFI devices
                    //
                    if ( CheckNodeInDevicePath (OptionDp, DpType, DpSubType)) {
                        //
                        // Check whether the device is USB compatible device.
                        // If it is a USB compatible device boot option,
                        //then skip this boot option. Move to next boot option
                        //
                        if ( CheckNodeInDevicePath (OptionDp, MESSAGING_DEVICE_PATH, \
                            MSG_USB_DP)) {
                            UsbDeviceCount = UsbDeviceCount + 1;
                        } else {
                            continue;
                        }
                    } else {
                        continue;
                    }
                } // UEFI check ends
            } else {
#if ((defined (BBS_USB_DEVICE_TYPE_SUPPORT) & (!BBS_USB_DEVICE_TYPE_SUPPORT)) && (defined (CSM_SUPPORT) & CSM_SUPPORT))
                if ( !gUefiBoot && isBbsDevicePath ( \
                    (BBS_BBS_DEVICE_PATH*) OptionDp, BbsDevType)) {
                    //
                    // Legacy Devices. Check class code to find out whether USB
                    // compatible device or SATA compatible device.
                    //
                    if ( IsUsbDeviceClassCode (Option) ) {
                        UsbDeviceCount = UsbDeviceCount + 1;
                    } else {
                        continue;
                    }
                } else {
                    continue;
                }
#elif ((defined (BBS_USB_DEVICE_TYPE_SUPPORT) & (BBS_USB_DEVICE_TYPE_SUPPORT)) && (defined (CSM_SUPPORT) & CSM_SUPPORT))
                if ( !gUefiBoot && isBbsDevicePath( \
                    (BBS_BBS_DEVICE_PATH*) OptionDp, BbsDevType)) {
                    continue;
                } else {
                    switch(BbsDevType) {
                        case BBS_FLOPPY:
                            if ( !gUefiBoot && isBbsDevicePath ( \
                                (BBS_BBS_DEVICE_PATH*) OptionDp, BBS_USB) && \
                                isUsbCompatibleDevice (Option, BAID_TYPE_RMD_FDD)) {
                                //
                                // Legacy Boot Option - USB floppy device path
                                //
                                UsbDeviceCount = UsbDeviceCount + 1;
                            } else {
                                continue;
                            }
                            break;

                        case BBS_CDROM:
                            if ( !gUefiBoot && isBbsDevicePath ( \
                                (BBS_BBS_DEVICE_PATH*) OptionDp, BBS_USB) && \
                                isUsbCompatibleDevice (Option, BAID_TYPE_CDROM)) {
                                //
                                // Legacy Boot Option - USB CDROM device path
                                //
                                UsbDeviceCount = UsbDeviceCount + 1;
                            } else {
                                continue;
                            }
                            break;

                        case BBS_HARDDISK:
                            if ( !gUefiBoot && isBbsDevicePath ( \
                                (BBS_BBS_DEVICE_PATH*) OptionDp, BBS_USB) && \
                                isUsbCompatibleDevice (Option, BAID_TYPE_RMD_HDD)) {
                                //
                                // Legacy Boot Option - USB HDD device path
                                //
                                UsbDeviceCount = UsbDeviceCount + 1;
                            } else {
                                continue;
                            }
                            break;

                        default:
                            continue;
                    } 
                }
#endif
            }
            if ( UsbDeviceCount == RequestedUsbDeviceInstance) {
                return Option;
            } else {
                continue;
            }
        }
    }
    return NULL;
}

/**
    Set the highest priority to the requested SATA compatible device boot
    option.

    @param BbsDevType BBS Device Type of the boot option to be selected.
    @param DpType Device path type of the boot option to be selected.
    @param DpSubType Device path sub type of boot option.

    @retval EFI_SUCCESS If requested boot option is found.
    @retval EFI_NOT_FOUND Proper boot option is not found to set priority.
**/

EFI_STATUS 
SetBootOptionToHighestPriority(
  IN  UINT16                    BbsDevType,
  IN  UINT16                    DpType,
  IN  UINT16                    DpSubType )
{
    DLINK                       *Link;
    BOOT_OPTION                 *Option;
    BOOT_OPTION                 *TempOption = NULL;
    BOOT_OPTION                 *UsbTempOption = NULL;
    EFI_DEVICE_PATH_PROTOCOL    *OptionDp;
    BOOLEAN                     HardDiskValidationResult;
    BOOLEAN                     IsUsbDeviceFound = FALSE;
    BOOLEAN                     IsNonUsbDeviceFound = FALSE;
    UINT16                      BootNumber = 0;
    UINTN                       UsbDeviceCount = 0;
    UINT8                       DeviceInstanceCount = 0xFF;
    UINTN                       RequiredUsbDeviceInstance;
    UINT8                       LastDeviceFound;

    DUMP_BOOT_OPTION_LIST (BootOptionList, "Before Setting IPMI Priorities");
    //
    // Proceed only if Boot Option list is not empty
    //
    if ( !DListEmpty (BootOptionList) ) {

        FOR_EACH_BOOT_OPTION (BootOptionList, Link, Option){
            LastDeviceFound = INVALID_DEVICE;
            OptionDp = Option->FilePathList;
            //
            // skip the group headers
            //
            if ( Option->GroupHeader ) {
                continue;
            }
            //
            // Check for BIOS Boot type as UEFI boot and
            // Check for the UEFI boot option (Not Legacy boot option)
            //
            if (gUefiBoot && (Option->BbsEntry == NULL) && \
                    (Option->BbsIndex == INVALID_BBS_INDEX)) {
                //
                // Check for floppy controller first as it has different
                // device path to verify. It has ACPI device path
                //
                if ((DpType == ACPI_DEVICE_PATH) && (DpSubType == ACPI_DP)) {
                    if (isAcpiDevicePath ( (ACPI_HID_DEVICE_PATH*)OptionDp )) {
                        LastDeviceFound = NON_USB_COMPATIBLE_DEVICE;
                    } else {
                        //
                        // Check for USB floppy
                        //
                        if ( CheckNodeInDevicePath (OptionDp, \
                                MESSAGING_DEVICE_PATH, MSG_USB_DP) ) {
                           
                            LastDeviceFound = USB_COMPATIBLE_DEVICE;
                        } else {
                        
                            continue;
                        }
                    }
                } else {
                    //
                    // Check for UEFI PXE Boot
                    //
                    if (DpType == MESSAGING_DEVICE_PATH && DpSubType == MSG_MAC_ADDR_DP) {
                        if ( !CheckNodeInDevicePath (OptionDp, DpType, DpSubType)) {
                            continue;
                        }
                        LastDeviceFound = NON_USB_COMPATIBLE_DEVICE;
                    //
                    // Check for other UEFI devices
                    //
                    } else if ( CheckNodeInDevicePath (OptionDp, DpType, DpSubType)) {
                        //
                        // Check whether the device is USB compatible device.
                        // If it is a USB compatible device boot option,
                        //then skip this boot option. Move to next boot option 
                        //
                        if ( CheckNodeInDevicePath (OptionDp, \
                                MESSAGING_DEVICE_PATH, MSG_USB_DP)) {
                            LastDeviceFound = USB_COMPATIBLE_DEVICE;
                        } else {
                            LastDeviceFound = NON_USB_COMPATIBLE_DEVICE;
                        }
                    //
                    // Check for boot loader file
                    //
                    } else if ( DpType == MEDIA_DEVICE_PATH && DpSubType == MEDIA_HARDDRIVE_DP ) {
                        if ( CheckNodeInDevicePath (OptionDp, MEDIA_DEVICE_PATH, MEDIA_FILEPATH_DP)){    // check if FILEPATH_DEVICE_PATH?
                            if ( CheckNodeInDevicePath (OptionDp, DpType, DpSubType)){
                                //
                                // Check whether the device is USB compatible device.
                                // If it is a USB compatible device boot option,
                                //then skip this boot option. Move to next boot option
                                //
                                if ( CheckNodeInDevicePath (OptionDp, \
                                     MESSAGING_DEVICE_PATH, MSG_USB_DP)) {
                                    LastDeviceFound = USB_COMPATIBLE_DEVICE;
                                } else {
                                    LastDeviceFound = NON_USB_COMPATIBLE_DEVICE;
                                }
                            } else {
                                continue;
                            }
                        } else {
                            continue;
                        }
                    } else {
                        continue;
                    }
                } // UEFI check ends
            } else {
#if ((defined (BBS_USB_DEVICE_TYPE_SUPPORT) & (!BBS_USB_DEVICE_TYPE_SUPPORT)) && (defined (CSM_SUPPORT) & CSM_SUPPORT))
                if ( !gUefiBoot && isBbsDevicePath ( \
                    (BBS_BBS_DEVICE_PATH*) OptionDp, BbsDevType)) {
                    //
                    // Legacy Devices. Check class code to find out whether
                    // USB compatible device or SATA compatible device.
                    //
                    if ( IsUsbDeviceClassCode (Option) ) {
                        LastDeviceFound = USB_COMPATIBLE_DEVICE;
                    } else {
                        LastDeviceFound = NON_USB_COMPATIBLE_DEVICE;
                    }
                } else {
                    continue;
                }
#elif ((defined (BBS_USB_DEVICE_TYPE_SUPPORT) & (BBS_USB_DEVICE_TYPE_SUPPORT)) && (defined (CSM_SUPPORT) & CSM_SUPPORT))
                if ( !gUefiBoot && isBbsDevicePath ( \
                    (BBS_BBS_DEVICE_PATH*) OptionDp, BbsDevType)) {
                    LastDeviceFound = NON_USB_COMPATIBLE_DEVICE;
                } else {
                    switch(BbsDevType) {
                        case BBS_FLOPPY:
                            if ( !gUefiBoot && isBbsDevicePath ( \
                                (BBS_BBS_DEVICE_PATH*) OptionDp, BBS_USB) && \
                                isUsbCompatibleDevice (Option, \
                                BAID_TYPE_RMD_FDD)) {
                                LastDeviceFound = USB_COMPATIBLE_DEVICE;
                            } else {
                                continue;
                            }
                            break;

                        case BBS_CDROM:
                            if ( !gUefiBoot && isBbsDevicePath ( \
                                (BBS_BBS_DEVICE_PATH*) OptionDp, BBS_USB) && \
                                isUsbCompatibleDevice (Option, \
                                BAID_TYPE_CDROM)) {
                                LastDeviceFound = USB_COMPATIBLE_DEVICE;
                            } else {
                                continue;
                            }
                            break;

                        case BBS_HARDDISK:
                            if ( !gUefiBoot && isBbsDevicePath ( \
                                (BBS_BBS_DEVICE_PATH*) OptionDp, BBS_USB) && \
                                isUsbCompatibleDevice (Option, \
                                BAID_TYPE_RMD_HDD)) {
                                LastDeviceFound = USB_COMPATIBLE_DEVICE;
                            } else {
                                continue;
                            }
                            break;

                        default:
                            continue;

                    } 
                }
#endif
            }

            if ((gIpmiForceBootDevice == ForceBootHardDriveSafeMode) ||
                (gIpmiForceBootDevice == ForceBootDiagnosticPartition) ) {

                HardDiskValidationResult = BootToDiagnosticPartitionOrHddRequestSafeMode(gUefiBoot, \
                                                           BbsDevType, DpType, DpSubType, Option);
                if ( HardDiskValidationResult ) {
                    continue;
                }
            }
            if ( (( !HardDiskValidationResult) &&
                (LastDeviceFound == USB_COMPATIBLE_DEVICE) ) ||
                (LastDeviceFound == USB_COMPATIBLE_DEVICE)) {
                //
                // if it is a USB compatible device then increase
                // UsbDeviceCount and if it is first matching 
                // USB device then take back of this boot option
                //
                UsbDeviceCount = UsbDeviceCount + 1;
                if (!IsUsbDeviceFound) {
                    UsbTempOption = Option;
                    IsUsbDeviceFound = TRUE;
                }
                continue;
            } 
            //
            // increase the DeviceInstanceCount value by 1, if Non USB
            // device found.
            //
            DeviceInstanceCount = DeviceInstanceCount + 1;
            if (((gDeviceInstanceSelector &
                INTERNAL_EXTERNAL_DEVICE_INSTANCE_MASK) == 0x00) || \
                ((gDeviceInstanceSelector & \
                DEVICE_INSTANCE_SELECTION_MASK) == DeviceInstanceCount)) {
                //
                // Control comes here, if gDeviceInstanceSelector is equal to
                // DeviceInstanceCount for all Non USB internal case and
                // for all Non USB external cases.
                //
                IsNonUsbDeviceFound = TRUE;
                if ( !gUefiBoot ) {
                    //
                    // Saving the matched boot option details in NVRAM
                    //
                    SaveMatchedBootOptionData( Option->BootOptionNumber );
                }
                //
                // Save the Current boot order.
                //
                SaveCurrentBootorder ();
                //
                // Set the Boot Option as Highest tag and Priority
                //
                Option->Priority = HIGHEST_BOOT_OPTION_PRIORITY;
                Option->Tag = HIGHEST_BOOT_OPTION_TAG;
                BootNumber = Option->BootOptionNumber;
                break;
            } else {
                //
                // Control comes here, if gDeviceInstanceSelector is not
                // equal to DeviceInstanceCount for all internal non USB
                // cases.
                //
                IsNonUsbDeviceFound = TRUE;
                if (DeviceInstanceCount == 0x00) {
                    TempOption = Option;
                }
                continue;
            }
        } // FOR_EACH_BOOT_OPTION ends
        if ((((gDeviceInstanceSelector & DEVICE_INSTANCE_SELECTION_MASK) != \
            DeviceInstanceCount) && \
            (gDeviceInstanceSelector & INTERNAL_EXTERNAL_DEVICE_INSTANCE_MASK) \
            != 0x00) || ((!IsNonUsbDeviceFound) && IsUsbDeviceFound) ) {
            //
            // Control comes here, if the specified device instance is not
            // available in Non USB device cases or if not even a single
            // Non USB device is found.
            //
            if (IsNonUsbDeviceFound) {
                RequiredUsbDeviceInstance = (gDeviceInstanceSelector & \
                    DEVICE_INSTANCE_SELECTION_MASK) - DeviceInstanceCount;
            } else {
                RequiredUsbDeviceInstance = (gDeviceInstanceSelector & \
                                        DEVICE_INSTANCE_SELECTION_MASK) + 1;
            }
            //
            // Call the SetUsbDeviceBootOptionToHighestPriority function
            // if it is internal device case and if
            // RequiredUsbDeviceInstance is available.
            //
            if ((UsbDeviceCount >= RequiredUsbDeviceInstance) &&  \
                ((gDeviceInstanceSelector & \
                INTERNAL_EXTERNAL_DEVICE_INSTANCE_MASK) != 0x00)) {

                Option = SetUsbDeviceBootOptionToHighestPriority (
                             RequiredUsbDeviceInstance,
                             BbsDevType,
                             DpType,
                             DpSubType );
            } else if (IsNonUsbDeviceFound) {
                //
                // if the RequiredUsbDeviceInstance is not available,
                // then set high priority to first Non USB device.
                //
                Option = TempOption;
            } else {
                //
                // if the RequiredUsbDeviceInstance is not available,
                // then set high priority to first USB device.
                //
                Option = UsbTempOption;
            }
            if ( !gUefiBoot ) {
                //
                // Saving the matched boot option details in NVRAM
                //
                SaveMatchedBootOptionData( Option->BootOptionNumber );
            }
            //
            // Set the Boot Option as Highest tag and Priority
            //
            if ( Option != NULL ) {
                //
                // Save the Current boot order.
                //
                SaveCurrentBootorder ();

                Option->Priority = HIGHEST_BOOT_OPTION_PRIORITY;
                Option->Tag = HIGHEST_BOOT_OPTION_TAG;
                BootNumber = Option->BootOptionNumber;
            }
        }
        //
        // Set the same Priority to all the same group device and
        // Tag will be used to differentiate after comparing the Priority
        //
        if ( !gUefiBoot && BootNumber ) {
            FOR_EACH_BOOT_OPTION (BootOptionList, Link, Option){
                if ( BootNumber == Option->BootOptionNumber ) {
                    Option->Priority = HIGHEST_BOOT_OPTION_PRIORITY;
                }
            }
        }
    }
    if ( BootNumber ) {
        DUMP_BOOT_OPTION_LIST(BootOptionList,"After Setting IPMI Priorities");
        return EFI_SUCCESS;
    }
    return EFI_NOT_FOUND;
}

/**
    This function update the IpmiBoot global variables.

    @param BootFlags IpmiBoot flags data.

    @retval VOID

**/

VOID
UpdateIpmiBootGlobalVariables (
  IN GET_BOOT_FLAG_RESPONSE     BootFlags
  )
{

    gIpmiForceBootDevice = BootFlags.Param5.BootDeviceSelector;
    gPersistentBoot = BootFlags.Param5.Persistent;
    gUefiBoot = BootFlags.Param5.BiosBootType;
    gDeviceInstanceSelector = BootFlags.Param5.DeviceInstanceSelector;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI Boot: gIpmiForceBootDevice = %d gPersistentBoot = %d gUefiBoot = %d gDeviceInstanceSelector = %d\n", \
                         gIpmiForceBootDevice, gPersistentBoot, gUefiBoot, gDeviceInstanceSelector));

    return;

}

/**
    Send the Get boot options command to get the boot flags parameter

    @param BootFlagsResponse - Pointer to get the boot flags response

    @retval BootFlagsResponse Input pointer updated with boot flags response data

**/

EFI_STATUS
IpmiGetBootFlags (
  OUT GET_BOOT_FLAG_RESPONSE        *BootFlagsResponse )
{
    EFI_STATUS                        Status;
    UINT8                             ResponseSize;
    IPMI_GET_BOOT_OPTIONS_REQUEST     GetRequest;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entered...\n", __FUNCTION__));

    //
    // Wait until Set In Progress field is cleared
    //
    Status = IpmiWaitSetInProgressClear ();
    SERVER_IPMI_DEBUG ((DEBUG_INFO, " IpmiWaitSetInProgressClear: Status: %r \n", Status));
    if (EFI_ERROR (Status)) {
        return Status;
    }

    ResponseSize = sizeof (GET_BOOT_FLAG_RESPONSE);

    GetRequest.ParameterSelector = BootFlags;
    GetRequest.Reserved = 0x0;
    GetRequest.SetSelector = 0x0;
    GetRequest.BlockSelector = 0x0;

    Status = gIpmiTransport->SendIpmiCommand (
                 gIpmiTransport,
                 IPMI_NETFN_CHASSIS,
                 0,
                 IPMI_CHASSIS_GET_SYSTEM_BOOT_OPTIONS,
                 (UINT8*) &GetRequest,
                 sizeof (GetRequest),
                 (UINT8*) BootFlagsResponse,
                 &ResponseSize);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a: IPMI_CHASSIS_GET_SYSTEM_BOOT_OPTIONS Status: %r \n", __FUNCTION__, Status));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a: Exiting........ Status: %r \n", __FUNCTION__, Status));

    return Status;
}

/**
    Wait for IPMI Get System Boot Option Set-in-Progress bit to clear

    @param VOID

    @retval EFI_SUCCESS Set in progress clear successful
    @retval EFI_TIMEOUT Set in progress clear failed

**/

EFI_STATUS
IpmiWaitSetInProgressClear (
  VOID )
{
    EFI_STATUS                                Status;
    UINT8                                     ResponseSize;
    UINT32                                    Retries = 10;
    IPMI_GET_BOOT_OPTIONS_REQUEST             GetRequest;
    GET_SET_IN_PROGRESS_RESPONSE              GetSetInProgResponse;

    GetRequest.ParameterSelector = SetInProgress;
    GetRequest.Reserved = 0x0;
    GetRequest.SetSelector = 0x0;
    GetRequest.BlockSelector = 0x0;

    //
    // Wait for in-progress bit to clear
    //
    do {
        ResponseSize = sizeof (GetSetInProgResponse);
        Status = gIpmiTransport->SendIpmiCommand (
                     gIpmiTransport,
                     IPMI_NETFN_CHASSIS,
                     0,
                     IPMI_CHASSIS_GET_SYSTEM_BOOT_OPTIONS,
                     (UINT8*) &GetRequest,
                     sizeof (GetRequest),
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

    if (++Retries == 0) {  // time out after Retires and first attempt.
        return EFI_TIMEOUT;
    }

    return EFI_SUCCESS;
}

/**
    Set the boot acknowledgment info to BMC using Set boot options command

    @param BootInitAckData Boot initiated acknowledge data

    @retval EFI_SUCCESS Boot acknowledgment info set successfully

**/

EFI_STATUS
IpmiSetBootInfoAck (
  IN UINT8      BootInitAckData )
{
    EFI_STATUS                        Status;
    UINT8                             ResponseSize;
    SET_BOOT_INFO_ACK_REQUEST         SetRequest;
    SET_BOOT_OPTIONS_RESPONSE         SetResponse;

    //
    // Check if Boot Flag Valid bit is set
    //
    Status = IpmiWaitSetInProgressClear ();
    if (EFI_ERROR (Status)) {
        return Status;
    }

    ResponseSize = sizeof (SetResponse);

    SetRequest.Request.ParameterSelector = BootInfoAck;
    SetRequest.Request.MarkParameterInvalid = 0;
    SetRequest.Param4.WriteMask = BootInitAckData;
    SetRequest.Param4.BootInitiatorAcknowledgeData.RawData = 0;

    Status = gIpmiTransport->SendIpmiCommand (
                 gIpmiTransport,
                 IPMI_NETFN_CHASSIS,
                 0,
                 IPMI_CHASSIS_SET_SYSTEM_BOOT_OPTIONS,
                 (UINT8*) &SetRequest,
                 sizeof (SetRequest),
                 (UINT8*) &SetResponse,
                 &ResponseSize );

    if (EFI_ERROR (Status)) {
        return Status;
    }

    return EFI_SUCCESS;
}

/**
    This is an workaround to clear boot device intentionally for 
    testing/demo purpose.

    @param VOID

    @return EFI_STATUS

**/

EFI_STATUS
IpmiClearBootFlags (
  VOID )
{

    EFI_STATUS      Status;
    UINT8           Temp[10] = {0};
    UINT8           ResponseSize;
    //
    // Check if Boot Flag Valid bit is set
    //
    Status = IpmiWaitSetInProgressClear ();
    if (EFI_ERROR (Status)) {
        return Status;
    }

    Temp[0] = BootFlags;
    ResponseSize = 0;

    Status = gIpmiTransport->SendIpmiCommand (
                 gIpmiTransport,
                 IPMI_NETFN_CHASSIS,
                 0,
                 IPMI_CHASSIS_SET_SYSTEM_BOOT_OPTIONS,
                 (UINT8*) &Temp,
                 6,
                 NULL,
                 &ResponseSize );
    if (EFI_ERROR (Status)) {
        return Status;
    }

    return EFI_SUCCESS;

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
