//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

//*************************************************************************
// $Header: $
//
// $Revision: $
//
// $Date: $
//*************************************************************************
// Revision History
// ----------------
// $Log: $
// 
// 
//*************************************************************************
//<AMI_FHDR_START>
//
// Name:        CRBDXE.c
//
// Description: This file contains code for Chipset Reference Board Template
//              initialization in the DXE stage
//
//<AMI_FHDR_END>
//*************************************************************************

//----------------------------------------------------------------------------
// Include(s)
//----------------------------------------------------------------------------
#include <IndustryStandard\Pci30.h>  // GrangevilleNS changes EIP#432131 - Support for Optimize boot
#include <Library/UefiLib.h>  // GrangevilleNS changes EIP#432131 - Support for Optimize boot
#include <Efi.h>
#include <token.h>
#include <AmiLib.h>
#include <AmiDxeLib.h>
#include <Setup.h>
#include <Dxe.h>
#include <PCI.h>
#include <AmiHobs.h>
#include <AmiCspLib.h>
#if defined CIRA_SAMPLE_SUPPORT && CIRA_SAMPLE_SUPPORT == 1
#include <ME.h>
#include <AMT.h>
#endif
//AptioV
#include <CrbDxeInitElink.h>
// Produced Protocols

// Consumed Protocols
#include <Protocol\PciIO.h>
#include <Protocol\PciRootBridgeIo.h>
#include <Protocol\BootScriptSave.h>

// GrangevilleNS changes Starts EIP#432131 - Support for Optimize boot
#include <Protocol/AmiOptionRomPolicy.h>
#include <NetworkStackSetup.h>
// GrangevilleNS changes Ends EIP#432131 - Support for Optimize boot
//----------------------------------------------------------------------------
// Constant, Macro and Type Definition(s)
//----------------------------------------------------------------------------
// Constant Definition(s)

// Macro Definition(s)

// Type Definition(s)

// Function Prototype(s)

//----------------------------------------------------------------------------
// Variable and External Declaration(s)
//----------------------------------------------------------------------------
// Variable Declaration(s)

// GUID Definition(s)

VOID        *gEfiPciIoProtocolRegistration = NULL;  // GrangevilleNS changes EIP#432131 - Support for Optimize boot
// Protocol Definition(s)

// External Declaration(s)

// Function Definition(s)


//----------------------------------------------------------------------------

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   CRBDXE_Init
//
// Description: This function is the entry point for this DXE. This function
//              initializes the CRB
//
// Parameters:  ImageHandle Image handle
//              SystemTable Pointer to the system table
//
// Returns:     Return Status based on errors that occurred while waiting for
//              time to expire.
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

// GrangevilleNS changes Starts EIP#432131 - Support for Optimize boot
/**
    Restricts connecting Network devices using BdsConnect Policy protocol
    
    @param   Event            The event that occurred
    @param   Context          For EFI compatibility.  Not used.

    @return  VOID
**/
VOID
EFIAPI
EfiPciIoProtocolReady (
  IN  EFI_EVENT         Event,
  IN  VOID              *Context )
{
    
    EFI_STATUS                     Status;
    EFI_HANDLE                     Handle;
    UINTN                          BufferSize = sizeof(Handle);
    AMI_OPTION_ROM_POLICY_PROTOCOL *OpRomPolicy = NULL;
    EFI_PCI_IO_PROTOCOL            *PciIo = NULL;
    PCI_TYPE00                      PciConfig;
    UINT8                           ItemPolicy;

    // Get Handle from Protocol registration
    Status = pBS->LocateHandle(
                    ByRegisterNotify, 
                    NULL, 
                    gEfiPciIoProtocolRegistration, 
                    &BufferSize, 
                    &Handle );
    if(EFI_ERROR(Status)) {
        //DEBUG ((EFI_D_ERROR,"Locate EfiPciIoProtocol handle = %r\n", Status ));
        return;
    }

    // Locate PciIo protocol on Handle
    Status = pBS->HandleProtocol(
                    Handle, 
                    &gEfiPciIoProtocolGuid, 
                    &PciIo );
    if (EFI_ERROR(Status)) {
        //DEBUG ((EFI_D_ERROR,"HandleProtocol for PciIo = %r\n", Status ));
        return;
    }

    // Locate AmiOptionRomPolicy protocol on Handle
    Status = pBS->HandleProtocol(   
                    Handle, 
                    &gAmiOptionRomPolicyProtocolGuid, 
                    &OpRomPolicy );
    if (EFI_ERROR(Status)) {
        //DEBUG ((EFI_D_ERROR,"HandleProtocol for OpRomPolicy = %r\n", Status ));
        return;
    }

    // Read Class code and subclass code
    Status = PciIo->Pci.Read (  
                    PciIo,
                    EfiPciIoWidthUint8,
                    0,
                    sizeof (PCI_TYPE00),
                    &PciConfig );
    if (EFI_ERROR(Status)) {
        DEBUG ((EFI_D_ERROR,"Get Class and subclass code Status = %r\n", Status ));
        return;
    }

    // Check for Network Controller class type
    if (IS_CLASS1 (&PciConfig, PCI_CLASS_NETWORK)) {
        DEBUG ((EFI_D_INFO,"Class code is Network.\n" ));

        // Do not disable Oprom for Fabric controller
        if (!IS_CLASS2 (&PciConfig, PCI_CLASS_NETWORK, 0x08)) {
            Status = OpRomPolicy->GetOptionRomPolicy(OpRomPolicy, &ItemPolicy,NULL);
            if (EFI_ERROR(Status)) {
                DEBUG ((EFI_D_ERROR,"GetOptionRomPolicy = %r\n", Status ));
                return;
            }
            DEBUG ((EFI_D_INFO,"OptionRomPolicy  = %d\n", ItemPolicy ));

            // Check if Oprom policy is UEFI
            if(ItemPolicy==AMI_OPTION_ROM_POLICY_UEFI){
                DEBUG ((EFI_D_INFO,"Oprom policy is UEFI, disable Oprom.\n" ));
                Status = OpRomPolicy->SetOptionRomPolicy( OpRomPolicy, AMI_OPTION_ROM_POLICY_DISABLE );
                DEBUG ((EFI_D_INFO,"SetOptionRomPolicy  = %r\n", Status )); 
            }else{
                DEBUG ((EFI_D_INFO,"OptionRomPolicy is not UEFI\n" ));
                return;
            }
        }else{
            DEBUG ((EFI_D_INFO,"This is Fabric controller, return..\n" ));
            return; 
        }
    }

    return;
}

/**
    Implements Optimized Boot functionality

    @param   VOID

    @return  VOID
**/
VOID 
OptimizedBoot (VOID)
{
    EFI_STATUS              Status = EFI_SUCCESS;
    EFI_GUID                SetupGuid = SETUP_GUID;
    UINTN                   SetupDataSize;
    SETUP_DATA              SetupData;
    UINT32                  Attributes = 0;
    NETWORK_STACK           NetworkStackSetupData;

    // Get Setup Data variable
    SetupDataSize = sizeof(SETUP_DATA);
    Status = pRS->GetVariable (  
                    L"Setup",
                    &SetupGuid,
                    &Attributes,
                    &SetupDataSize,
                    &SetupData );
    if(EFI_ERROR (Status)){
        DEBUG ((EFI_D_ERROR,"Optimized Boot Get Setup Variable = %r\n", Status ));
        return;
    }

    // Check for Optimized Boot setup option
    if (SetupData.OptimizedBoot){
        DEBUG ((EFI_D_INFO,"Optimized Boot = %d, Optimized is Enabled\n", SetupData.OptimizedBoot ));

        // Check for Csm Support option
        if (SetupData.CsmSupport == 1){
            // CSM is enabled so disable CSM Support using Setup Variable.
            SetupData.CsmSupport = 0;
            Status = pRS->SetVariable( 
                            L"Setup", 
                            &SetupGuid, 
                            Attributes,  
                            SetupDataSize, 
                            &SetupData );
            if(EFI_ERROR (Status)){
                DEBUG ((EFI_D_ERROR,"Optimized Boot Set Setup Variable = %r\n", Status ));
                return;
            }
            DEBUG ((EFI_D_INFO,"Disable CSM Support.\n"));
        }

        // Get NetworkStack support option
        SetupDataSize = sizeof(NETWORK_STACK);
        Status = pRS->GetVariable(
                        L"NetworkStackVar",
                        &gEfiNetworkStackSetupGuid, 
                        &Attributes, 
                        &SetupDataSize, 
                        &NetworkStackSetupData );
        if(EFI_ERROR (Status)){
            DEBUG ((EFI_D_ERROR,"Optimized Boot Get Network Stack Variable = %r\n", Status ));
            return;
        }

        // Create Notification event for gEfiPciIoProtocolGuid GUID
        if (NetworkStackSetupData.Enable == 0){
            DEBUG ((EFI_D_INFO,"Register protocol notification for PciIoProtocol, to disable Network devices \n" ));
            EfiCreateProtocolNotifyEvent (
                    &gEfiPciIoProtocolGuid,
                    TPL_CALLBACK,
                    EfiPciIoProtocolReady,
                    NULL,
                    &gEfiPciIoProtocolRegistration
                    );
        }
    }else{
        DEBUG ((EFI_D_INFO,"Optimized Boot = %d, Optimized is Disable\n", SetupData.OptimizedBoot ));
        return;
    }

    return;
}
// GrangevilleNS changes Ends EIP#432131 - Support for Optimize boot
EFI_STATUS CRBDXE_Init (
    IN EFI_HANDLE       ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable )
{
    EFI_STATUS Status = EFI_SUCCESS;

#if defined CIRA_SAMPLE_SUPPORT && CIRA_SAMPLE_SUPPORT == 1
    AMI_AMT_PROTOCOL *mAmtProtocol;
    EFI_GUID mAmtProtocolGuid = AMI_AMT_PROTOCOL_GUID;
    EFI_GUID mSetupGuid = SETUP_GUID;
    UINTN mSetupDataSize = NULL;
    SETUP_DATA mSetupData;
#endif

    InitAmiLib(ImageHandle, SystemTable);

#if defined CIRA_SAMPLE_SUPPORT && CIRA_SAMPLE_SUPPORT == 1
    mSetupDataSize = sizeof(SETUP_DATA);
    Status = pRS->GetVariable (  
                                L"Setup",
                                &mSetupGuid,
                                NULL,
                                &mSetupDataSize,
                                &mSetupData 
                              );

    if (mSetupData.CIRATrigger == 1)
    {
        Status = pBS->LocateProtocol(&mAmtProtocolGuid, NULL, &mAmtProtocol);
        if (!EFI_ERROR(Status))
            mAmtProtocol->SetCIRA(mAmtProtocol, CIRA_ENABLE, CIRA_DEFFAULT_TIME);
    }
#endif

   OptimizedBoot();  // GrangevilleNS changes EIP#432131 - Support for Optimize boot
    return EFI_SUCCESS;
}

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
