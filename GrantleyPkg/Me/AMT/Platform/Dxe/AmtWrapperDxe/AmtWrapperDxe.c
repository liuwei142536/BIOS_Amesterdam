//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2014, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
//**********************************************************************

//
//**********************************************************************

//<AMI_FHDR_START>
//----------------------------------------------------------------------------
//
// Name:            AmtWrapperDxe.c
//
// Description:     Amt Wrapper Functions
//
//----------------------------------------------------------------------------
//<AMI_FHDR_END>
/*++
Copyright (c) 2009 - 2013 Intel Corporation. All rights reserved.
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.
--*/
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <CommonIncludes.h>
#include <Protocol/LegacyBios.h>
//AptioV server override  #include <Csm/LegacyBiosDxe/LegacyBiosInterface.h>
#include <Protocol/SmmPciRootBridgeIo.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/PciIo.h>
#include "Protocol/HeciProtocol.h"
#include "AmtWrapperDxe.h"
#include "Protocol/AmtWrapperProtocol.h"
#include "Protocol/MeBiosPayloadData.h"
#include "Protocol/AlertStandardFormat.h"
#include "Protocol/ActiveManagementProtocol.h"
#include "Protocol/AmtPlatformPolicy.h"
#include "MeState.h"
#include "Library/MeTypeLib.h"
// AptioV server override < Support AMT function
#if defined(Terminal_SUPPORT) && (Terminal_SUPPORT == 1)
#include <TerminalSetupVar.h>
#endif
#if defined(LEGACYSREDIR_SUPPORT) && (LEGACYSREDIR_SUPPORT == 1)
#include <Protocol\LegacySredir.h>
#include <Protocol\PciRootBridgeIo.h>
#include "CoreBiosMsg.h"
#endif
#include <Setup.h>
#include <AmiProtocol.h>
#include <Protocol\Security.h>
#include <Amt.h>

//AptioV server override start
#define MAX_BBS_ENTRIES 0x100
//AptioV server override end
EFI_GUID gSetupGuid                 	= SETUP_GUID;
// AptioV server override Support AMT function >
AMT_WRAPPER_PROTOCOL AmtWrapperInstance = {
                                        AmtWrapperInit,
                                        AmtWrapperGet,
                                        AmtWrapperSet,
                                        AmtWrapperEnableSol,
                                        AmtWrapperEnableIdeR,
                                        AmtWrapperPauseBoot,
                                        AmtWrapperEnterSetup,
                                        AmtWrapperBootOptionExist,
                                        AmtWrapperBdsBootViaAsf,
                                        BdsLibUpdateConsoleVariable,
                                        ActiveManagementConsoleLocked,
                                        AmtWrapperEnableKvm,
                                        //AptioV server override GetSetupPrompt,
                                        //AptioV server override DetermineSetupHotKey,
                                        //AptioV server override GetSupportedHotkeys
                                  };

extern EFI_ACTIVE_MANAGEMENT_PROTOCOL   *mActiveManagement;
extern EFI_ASF_BOOT_OPTIONS             *mAsfBootOptions;

extern EFI_GUID gEfiMeBiosExtensionSetupGuid;
extern CHAR16   gEfiMeBiosExtensionSetupName[] = EFI_ME_BIOS_EXTENSION_SETUP_VARIABLE_NAME;

//
// Protocol GUID definition
//
UINTN gEnterRemoteAssistance = 0;
UINTN gEnterIntelMeSetup = 0;
extern DXE_AMT_POLICY_PROTOCOL          *mDxePlatformAmtPolicy;
ME_BIOS_EXTENSION_SETUP                 MeBiosExtensionSetup;
BOOLEAN                                 MeBiosExtensionSetupReady = FALSE;
UINT8                                   mFwImageType;

PLATFORM_PCI_SERIAL_OVER_LAN_DEVICE_PATH gSerialOverLanDevicePath = {
            gPciRootBridge,
            {
                HARDWARE_DEVICE_PATH,
                HW_PCI_DP,
                (UINT8)(sizeof(PCI_DEVICE_PATH)),
                (UINT8)((sizeof(PCI_DEVICE_PATH)) >> 8),
                SOL_FUNCTION_NUMBER,
                ME_DEVICE_NUMBER
            },
            {
                MESSAGING_DEVICE_PATH,
                MSG_UART_DP,
                (UINT8)(sizeof(UART_DEVICE_PATH)),
                (UINT8)((sizeof(UART_DEVICE_PATH)) >> 8),
                0,
                115200,
                8,
                1,
                1
            },
            {
                MESSAGING_DEVICE_PATH,
                MSG_VENDOR_DP,
                (UINT8)(sizeof(VENDOR_DEVICE_PATH)),
                (UINT8)((sizeof(VENDOR_DEVICE_PATH)) >> 8),
                DEVICE_PATH_MESSAGING_PC_ANSI
            },
            gEndEntire
        };

// TODO, TODO, TODO
EFI_HANDLE mBdsImageHandle;
UINT16 gPlatformBootTimeOutDefault = 1;
// TODO, TODO, TODO
// AptioV server override < Support AMT function
//
// Function implementations
//


EFI_STATUS EmptyFileAuthentication
(
    IN CONST EFI_SECURITY_ARCH_PROTOCOL *This,
    IN UINT32 AuthenticationStatus,
    IN CONST EFI_DEVICE_PATH_PROTOCOL *File
)
{
    if (File==NULL) return EFI_INVALID_PARAMETER;
    return EFI_SUCCESS;
}

#if defined(Terminal_SUPPORT) && (Terminal_SUPPORT == 1)
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   SetSOLCOMEnable
//
// Description: Enable/Disable SOL COM.
//
// Input:
//
// Output:
//
// Notes:
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID
SetSOLCOMEnable(
    IN BOOLEAN         Enabled
)
{
    UINT32      ComPort;
    UINTN       SetupDataSize;
    SETUP_DATA  gSetupData;
    EFI_STATUS  Status;
#if (TOTAL_PCI_SERIAL_PORTS > 0)
    UINT32      i = 0;
    EFI_GUID    gTerminalVarGuid   = TERMINAL_VAR_GUID;
    UINTN       PciSerialPortsLocationVarSize = 
                                    sizeof(PCI_SERIAL_PORTS_LOCATION_VAR);
    UINT32      PciSerialPortsLocationVarAttributes=0;
    PCI_SERIAL_PORTS_LOCATION_VAR PciSerialPortsLocationVar; 
    UINT32 gTotalSioSerialPorts = TOTAL_SIO_SERIAL_PORTS;
    Status = gRT->GetVariable(L"PciSerialPortsLocationVar", 
                                &gTerminalVarGuid, 
                                &PciSerialPortsLocationVarAttributes, 
                                &PciSerialPortsLocationVarSize, 
                                &PciSerialPortsLocationVar);

    if(EFI_ERROR(Status)) {
        ComPort = gTotalSioSerialPorts;
    }

    ComPort = gTotalSioSerialPorts; 

    for (i = 0; i < TOTAL_PCI_SERIAL_PORTS; i++) {
        if ((PciSerialPortsLocationVar.Device[i] == SOL_DEVICE_NUMBER) && 
            (PciSerialPortsLocationVar.Function[i] == SOL_FUNCTION_NUMBER)) {
                ComPort = gTotalSioSerialPorts+i; 
                break;            
        }
    } 
#else 
    return;
#endif

    SetupDataSize = sizeof(SETUP_DATA);
    Status = gRT->GetVariable ( L"Setup", \
                                &gSetupGuid, \
                                NULL, \
                                &SetupDataSize, \
                                &gSetupData );

    if(Enabled)
        gSetupData.ConsoleRedirectionEnable[ComPort] = 1;
    else
        gSetupData.ConsoleRedirectionEnable[ComPort] = 0;

    Status = gRT->SetVariable ( L"Setup", \
                                &gSetupGuid, \
                                EFI_VARIABLE_NON_VOLATILE | \
                                EFI_VARIABLE_BOOTSERVICE_ACCESS | \
                                EFI_VARIABLE_RUNTIME_ACCESS, \
                                SetupDataSize, \
                                &gSetupData );
    
}
#endif
// AptioV server override Support AMT function >
EFI_STATUS
EFIAPI
AmtWrapperDxeEntryPoint (
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
)
/*++

Routine Description:

  Entry point for the Active Management Driver.

Arguments:

  ImageHandle       Image handle of this driver.
  SystemTable       Global system service table.

Returns:

  EFI_SUCCESS           Initialization complete.
  EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  EFI_DEVICE_ERROR      Device error, driver exits abnormally.

--*/
{
    EFI_STATUS             Status;
    EFI_EVENT              ReadyToBootEvent;
    DXE_MBP_DATA_PROTOCOL  *MbpData;

#if defined(SPS_SUPPORT) && SPS_SUPPORT
    if (!MeTypeIsAmt()) {
      return EFI_UNSUPPORTED;
    }
#endif //SPS_SUPPORT

//    INITIALIZE_SCRIPT (ImageHandle, SystemTable);

    mAsfBootOptions = NULL;
    mActiveManagement = NULL;

    InitAmtWrapperLib();

    mFwImageType = FW_IMAGE_TYPE_1_5MB;

    Status = gBS->LocateProtocol (&gMeBiosPayloadDataProtocolGuid, NULL, &MbpData);
    if (!EFI_ERROR(Status)) {
      mFwImageType = (UINT8)MbpData->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType;
    }

    //
    // Install the EFI_ACTIVE_MANAGEMENT_PROTOCOL interface
    //
    Status = gBS->InstallMultipleProtocolInterfaces (
                 &ImageHandle,
                 &gEfiAmtWrapperProtocolGuid,   &AmtWrapperInstance,
                 NULL
             );
// AptioV server override <	Support AMT function		 
#if defined(Terminal_SUPPORT) && (Terminal_SUPPORT == 1)
    // Enable/Disable Sol redirection
    SetSOLCOMEnable(AmtWrapperEnableSol());
#endif
// AptioV server override Support AMT function >
    //
    // Register Ready to Boot Event for AMT Wrapper
    //
    Status = EfiCreateEventReadyToBootEx (
                TPL_CALLBACK,
               	AmtWrapperReadyToBoot,
               	(VOID *)&ImageHandle,
                &ReadyToBootEvent
                );

    return Status;
}

//TODO, TODO, TODO
EFI_STATUS
BdsRefreshBbsTableForBoot (
    IN BDS_COMMON_OPTION *Entry
)
{
    return EFI_NOT_FOUND;
}

VOID
PrintBbsTable (
    IN BBS_TABLE *LocalBbsTable
)
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  LocalBbsTable - GC_TODO: add argument description

Returns:

  GC_TODO: add return values

--*/
{
    UINT16 Idx;

    DEBUG ((EFI_D_ERROR, "\n"));
    DEBUG ((EFI_D_ERROR, " NO  Prio bb/dd/ff cl/sc Type Stat segm:offs\n"));
    DEBUG ((EFI_D_ERROR, "=============================================\n"));
    for (Idx = 0; Idx < MAX_BBS_ENTRIES; Idx++)
    {
        if (LocalBbsTable[Idx].BootPriority == BBS_IGNORE_ENTRY)
        {
            continue;
        }

        DEBUG (
            (EFI_D_ERROR,
             " %02x: %04x %02x/%02x/%02x %02x/02%x %04x %04x %04x:%04x\n",
             (UINTN) Idx,
             (UINTN) LocalBbsTable[Idx].BootPriority,
             (UINTN) LocalBbsTable[Idx].Bus,
             (UINTN) LocalBbsTable[Idx].Device,
             (UINTN) LocalBbsTable[Idx].Function,
             (UINTN) LocalBbsTable[Idx].Class,
             (UINTN) LocalBbsTable[Idx].SubClass,
             (UINTN) LocalBbsTable[Idx].DeviceType,
             (UINTN) * (UINT16 *) &LocalBbsTable[Idx].StatusFlags,
             (UINTN) LocalBbsTable[Idx].BootHandlerSegment,
             (UINTN) LocalBbsTable[Idx].BootHandlerOffset,
             (UINTN) ((LocalBbsTable[Idx].MfgStringSegment << 4) + LocalBbsTable[Idx].MfgStringOffset),
             (UINTN) ((LocalBbsTable[Idx].DescStringSegment << 4) + LocalBbsTable[Idx].DescStringOffset))
        );
    }

    DEBUG ((EFI_D_ERROR, "\n"));
}

BOOLEAN
AmtWrapperBootOptionExist (
    IN VOID
)
{
    return (BOOLEAN) (mAsfBootOptions->SubCommand == ASF_BOOT_OPTIONS_PRESENT);
}

BOOLEAN
AmtWrapperGet(
    IN UINTN Index,
    IN OUT VOID *pVar
)
{
    BOOLEAN st = FALSE;

    InitAmtWrapperLib();

    if (Index == GET_SOL_DP) {
        *((PLATFORM_PCI_SERIAL_OVER_LAN_DEVICE_PATH **)pVar) = &gSerialOverLanDevicePath;
        st = TRUE;
    /* AptioV server override Don't use intel code.
    } else if (Index == GET_TIME_OUT) {
        *((UINT16 *)pVar) = BdsLibGetTimeout();
        st = TRUE;
    AptioV server override Don't use intel code */
    } else if (Index == GET_BOOT_OPTION) {
        *((UINT16 *)pVar) = mAsfBootOptions->BootOptions;
         st = TRUE;
    } else if (Index == GET_IS_SOL) {
        *((UINTN *)pVar) = IsSOL(*((EFI_HANDLE *)pVar));
         st = TRUE;
    } else if (Index == GET_IS_IDER) {
        *((UINTN *)pVar) = IsIDER(*((EFI_HANDLE *)pVar));
         st = TRUE;
    }

    return st;
}

UINTN IsSOL(
	IN EFI_HANDLE Controller
)
{
	EFI_STATUS 					Status;
	EFI_PCI_IO_PROTOCOL         *PciIo;
    UINTN                       Segment=0;
    UINTN                       Bus=0;
    UINTN                       Device=0;
    UINTN                       Function=0;
	UINTN						flag = FALSE;

	Status = gBS->HandleProtocol(Controller,
                                 &gEfiPciIoProtocolGuid,
                                 &PciIo);
    if (!EFI_ERROR(Status)) {
	    Status = PciIo->GetLocation(PciIo,
	    							&Segment,
	    							&Bus,
	                                &Device,
	                                &Function);
		if (!EFI_ERROR(Status)) {
			if (Device == SOL_DEVICE_NUMBER && Function == SOL_FUNCTION_NUMBER) {
				flag = TRUE;
			}
		}
	}

   	return flag;
}

UINTN IsIDER(
	IN EFI_HANDLE Controller
)
{
	EFI_STATUS 					Status;
	EFI_PCI_IO_PROTOCOL         *PciIo;
    UINTN                       Segment=0;
    UINTN                       Bus=0;
    UINTN                       Device=0;
    UINTN                       Function=0;
	UINTN						flag = FALSE;

	Status = gBS->HandleProtocol(Controller,
                                 &gEfiPciIoProtocolGuid,
                                 &PciIo);
    if (!EFI_ERROR(Status)) {
		    Status = PciIo->GetLocation(PciIo,
		    							&Segment,
		    							&Bus,
		                                &Device,
		                                &Function);
	    if (!EFI_ERROR(Status)) {
			if (Device == IDER_DEVICE_NUMBER && Function == IDER_FUNCTION_NUMBER) {
				flag = TRUE;
			}
		}
	}

   	return flag;
}
// AptioV server override < Support AMT function
UINT16 ConvertSetupDataToUART(
	UINT8 Parity,
	UINT8 DataBits,
	UINT8 StopBits
)
{
	UINT16 DataParityStopBit = 0;
	
    switch (Parity) {
        case NoParity:
            DataParityStopBit &= ~(PAREN | EVENPAR | STICPAR);
        break;
        case EvenParity:
            DataParityStopBit |= (PAREN | EVENPAR);
            DataParityStopBit &= ~STICPAR;
        break;
        case OddParity:
            DataParityStopBit |= PAREN;
            DataParityStopBit &= ~(EVENPAR | STICPAR);
            break;
        case SpaceParity:
            DataParityStopBit |= (PAREN | EVENPAR | STICPAR);
            break;
        case MarkParity:
            DataParityStopBit |= (PAREN | STICPAR);
            DataParityStopBit &= ~EVENPAR;
        break;
    }
    
    // Set stop bits.
    switch (StopBits) {
        case OneStopBit :
            DataParityStopBit &= ~STOPB;
        break;
        case OneFiveStopBits :
        case TwoStopBits :
            DataParityStopBit |= STOPB;
            break;
    }

    // Set data bits.
    DataParityStopBit &= ~SERIALDB;
    DataParityStopBit |= (UINT8)((DataBits - 5) & 0x03);
            
    return DataParityStopBit;    
}
// AptioV server override Support AMT function>
BOOLEAN
AmtWrapperSet(
    IN UINTN Index,
    IN OUT VOID *pVar
)
{
// AptioV server override <Support AMT function
#if defined(LEGACYSREDIR_SUPPORT) && (LEGACYSREDIR_SUPPORT == 1)
    EFI_LEGACY_SREDIR_PROTOCOL  *pLegacySredir;
    UINT64                      PciAddress;
    UINT32                      SerialAddress;
    UINT8                       SerialIRQ;
    EFI_STATUS                  Status;
    EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo;
    BOOLEAN st = FALSE;

    InitAmtWrapperLib();

    if (Index == SET_BIOS_WDT_START) {
        AsfStartWatchDog(ASF_START_BIOS_WDT);
        st = TRUE;
    }
    else if (Index == SET_OS_WDT_START) {
        AsfStartWatchDog(ASF_START_OS_WDT);
        st = TRUE;
    }
    else if (Index == SET_WDT_STOP) {
        AsfStopWatchDog();
        st = TRUE;
    }
    else if (Index == SET_OS_SOL) {
      DEBUG((EFI_D_ERROR, "<SET_OS_SOL>"));

      //
      // LegacySredir is not enabled currently in IAG code.
      //  No effect because the EnableLegacySredir() also is not called in Greens Glacier code.
      //
        Status = gBS->LocateProtocol(&gEfiLegacySredirProtocolGuid, NULL, &pLegacySredir);
        ASSERT_EFI_ERROR(Status);

        Status = gBS->LocateProtocol(&gEfiPciRootBridgeIoProtocolGuid, NULL, &PciRootBridgeIo);
        ASSERT_EFI_ERROR(Status);

        PciAddress = EFI_PCI_ADDRESS (SOL_BUS_NUMBER, SOL_DEVICE_NUMBER, SOL_FUNCTION_NUMBER, 0x10);
        PciRootBridgeIo->Pci.Read (
            PciRootBridgeIo,
            EfiPciWidthUint32,
            PciAddress,
            1,
            &SerialAddress
        );
        SerialAddress &= ~1;            // mask IO/MEM bit

        PciAddress = EFI_PCI_ADDRESS (SOL_BUS_NUMBER, SOL_DEVICE_NUMBER, SOL_FUNCTION_NUMBER, 0x3C);
        PciRootBridgeIo->Pci.Read (
            PciRootBridgeIo,
            EfiPciWidthUint8,
            PciAddress,
            1,
            &SerialIRQ
        );

        Status = pLegacySredir->EnableLegacySredir(pLegacySredir); 
        ASSERT_EFI_ERROR (Status);
        st = TRUE;
    }

    return st;
#else
    return FALSE;
#endif
// AptioV server override Support AMT function>
}

EFI_STATUS
AmtWrapperInit(
    IN EFI_HANDLE ImageHandle,
    IN OUT EFI_SYSTEM_TABLE *SystemTable
)
{
    mBdsImageHandle = ImageHandle;
    InitAmtWrapperLib();

    return EFI_SUCCESS;
}


VOID
InitAmtWrapperLib(VOID)
{
    if (mActiveManagement == NULL)
        AmtLibInit();

    if (mAsfBootOptions == NULL) {
		BdsAsfInitialization();
	}
}

BOOLEAN
AmtWrapperEnableSol(
	IN VOID
)
{
    BOOLEAN st;

    InitAmtWrapperLib();
    st = ActiveManagementEnableSol();
	// AptioV server override <Support AMT function
    if(((mAsfBootOptions->OemParameters) & 0x01) && (mAsfBootOptions != NULL))
        st = TRUE;
    else
        st = FALSE;
    // AptioV server override Support AMT function>
    return st;
}

BOOLEAN
AmtWrapperEnableIdeR(
	IN VOID
)
{
    BOOLEAN st;
	// AptioV server override <Support AMT function
    EFI_STATUS  Status;
    EFI_SECURITY_ARCH_PROTOCOL *mSecurity;

    InitAmtWrapperLib();    
    st = ActiveManagementEnableIdeR();
    if(!st)
        return st;
    // IDER enabled and Enforce Secure Boot enabled
    if((mAsfBootOptions->SpecialCommandParam & ENFORCE_SECURE_BOOT) 
                                            != ENFORCE_SECURE_BOOT)
    {
        Status = gBS->LocateProtocol(&gEfiSecurityArchProtocolGuid, 
                                     NULL, &mSecurity);

        if(EFI_ERROR(Status))
            return st;
        // Set a empty File Authentication to skip Secure check.
        // This will not been restore, because IDER boot fail system HALT
        mSecurity->FileAuthenticationState = EmptyFileAuthentication;

    }
	// AptioV server override Support AMT function>

    return st;
}

BOOLEAN
AmtWrapperPauseBoot(
	IN VOID
)
{
    BOOLEAN st;

    InitAmtWrapperLib();
    st = ActiveManagementPauseBoot();

    return st;
}

BOOLEAN
AmtWrapperEnterSetup(
	IN VOID
)
{
    BOOLEAN st;

    InitAmtWrapperLib();
    st = ActiveManagementEnterSetup();

    return st;
}

EFI_STATUS
AmtWrapperBdsBootViaAsf(
	IN VOID
)
{
	EFI_STATUS Status = EFI_NOT_FOUND;

    InitAmtWrapperLib();

    if (mActiveManagement != NULL && mAsfBootOptions != NULL) {
    	Status = BdsBootViaAsf();
    }

    return Status;
}



VOID
AmtWrapperReadyToBoot(
	EFI_EVENT           Event,
    VOID                *ParentImageHandle
)
{
	gBS->CloseEvent(Event);
	return;
}

BOOLEAN
AmtWrapperEnableKvm(
	IN VOID
)
{
  BOOLEAN Status;

  InitAmtWrapperLib();
  Status = ActiveManagementEnableKvm();

  return Status;
}

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2014, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
