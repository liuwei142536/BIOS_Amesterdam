/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

Copyright (c)2009-2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  SoftAdr.c

Abstract:

  Software ADR framework.

------------------------------------------------------------------*/
#include "SoftAdr.h"
#pragma optimize("", off)
EFI_SMM_VARIABLE_PROTOCOL       *mSmmVariable;
EFI_CPU_CSR_ACCESS_PROTOCOL     *mCpuCsrAccess = NULL;
EFI_IIO_UDS_PROTOCOL  *mIioUds;

#ifndef EFI_DEADLOOP 
#define EFI_DEADLOOP()    { volatile int __iii; __iii = 1; while (__iii); }
#endif

static EFI_SMM_BASE2_PROTOCOL *mSmmBase2;
EFI_SMM_GPI_DISPATCH2_PROTOCOL *mGpiDispatch;
EFI_SMM_SYSTEM_TABLE2*        mSmst = NULL;


BOOLEAN
EfiInManagementInterrupt(
VOID
)
/*++

Routine Description:

GC_TODO: Add function description

Arguments:


Returns:

GC_TODO: add return values

--*/
{

  BOOLEAN InSmm;

  if (mSmmBase2 == NULL) {
    InSmm = FALSE;
  }
  else {
    mSmmBase2->InSmm(mSmmBase2, &InSmm);
    mSmmBase2->GetSmstLocation(mSmmBase2, &mSmst);
  }

  return InSmm;
}


void
FlushCache(
void *node
)
/*++

Routine Description:

Flushes the cache from all threads

Arguments:


Returns:

None

--*/
{
  AsmWbinvd();
}


VOID SoftADRTrigger(
  )
  /*++

  Routine Description:

 Triggers the software ADR by sending Mailbox message to pcode.
 Also runs the MP routine to flush all the cache.

  Arguments:


  Returns:

  None

  --*/
{
  UINTN       numberOfCpus;
  UINTN       thread;
  UINT8       Socket;
  UINT32  MailBoxCommand, MailboxData;
  numberOfCpus = mSmst->NumberOfCpus;

  DEBUG((EFI_D_ERROR, "SoftADR Trigger - Flush all the cache. Threads = %d\n", numberOfCpus));
  for (thread = 0; thread < numberOfCpus; thread++) {
    mSmst->SmmStartupThisAp(FlushCache, thread, NULL);
  }

  AsmWbinvd();


  DEBUG((EFI_D_ERROR, "Triggering  SoftADR via mailbox command\n"));

  // Enable Soft ADR using b2p mailbox command.
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((mIioUds->IioUdsPtr->SystemStatus.socketPresentBitMap & (1 << Socket)) == 0) {
       continue;
    }
    MailBoxCommand = (UINT32)MAILBOX_BIOS_CMD_SOFTWARE_ADR_TRIGGER;
    MailboxData = mCpuCsrAccess->Bios2PcodeMailBoxWrite(Socket, MailBoxCommand, 1);
    if (MailboxData != 0) {
      DEBUG((EFI_D_ERROR, "Mailbox command to trigger SoftADR failed\n"));
    }
  }

  EFI_DEADLOOP();

  return;
}

VOID SetADRResetMB()
{
  UINT32  MailBoxCommand, MailboxData;
  DEBUG((EFI_D_ERROR, "Enable ADR On Reset\n"));
  MailBoxCommand = (UINT32)MAILBOX_BIOS_CMD_ADR_IIO_BLOCK;
  MailboxData = mCpuCsrAccess->Bios2PcodeMailBoxWrite(0, MailBoxCommand, 2);
  if (MailboxData != 0) {
    DEBUG((EFI_D_ERROR, "Mailbox command to enable ADR On Reset failed\n"));
  }

}

EFI_STATUS
GpiSoftADRCallback(
IN EFI_HANDLE  DispatchHandle,
IN CONST VOID  *mGpiContext,
IN OUT VOID    *CommBuffer      OPTIONAL,
IN OUT UINTN   *CommBufferSize  OPTIONAL
)
/*++

Routine Description:

Callback routine that gets called when Soft ADR GPIO is asserted. 
This routine will also call the routine that actually triggers the ADR

Arguments:


Returns:
Function will not return if the software ADR is successful.

--*/
{
  EFI_SMM_GPI_REGISTER_CONTEXT *Context = (EFI_SMM_GPI_REGISTER_CONTEXT *)mGpiContext;
  //
  // GPIO15 for SoftADR 
  //
  DEBUG((EFI_D_ERROR, "GpiSoftADRCallback\n"));
  if (Context->GpiNum != ICH_GPI_SMI_SOFTADR || mSmst == NULL)   {
    return EFI_NOT_FOUND;
  }
  SoftADRTrigger();
  return EFI_SUCCESS;
}





EFI_STATUS
RegisterSoftADRGpiChildHandler(
UINT8     GpiNumber
)
/*++

Routine Description:

Registers a callback function to be called when SoftADR GPIO is asserted and SMI is generated.

Arguments:


Returns:

Success or the actual status of registering the callback.

--*/


{
  EFI_SMM_GPI_REGISTER_CONTEXT  mGpiContext;
  EFI_HANDLE                    GpiHandle;
  EFI_STATUS                    Status;

  Status = EFI_SUCCESS;
  //
  // Register our SMI handlers through a child protocol
  //
  Status = mSmst->SmmLocateProtocol(&gEfiSmmGpiDispatch2ProtocolGuid, NULL, &mGpiDispatch);
  ASSERT_EFI_ERROR(Status);

  DEBUG((EFI_D_ERROR, "Register SoftADR GpiChildHandler\n"));
  //
  // Register the enable handler
  //
  mGpiContext.GpiNum = GpiNumber;
  Status = mGpiDispatch->Register(
    mGpiDispatch,
    GpiSoftADRCallback,
    &mGpiContext,
    &GpiHandle
    );
  ASSERT_EFI_ERROR(Status);

  return Status;
}




VOID EnableSoftADR(
  )
  /*++

  Routine Description:

 Enables Soft ADR SMI generation by setting all the required bits in the PCH.
 Also sets the GPIO for generating SMI on a power failure detection (by CPLD)
 which would then cause soft adr mailbox message to pcode.
  Arguments:


  Returns:

  None.

  --*/
{
  SMICTRL_UBOX_MISC_HSX_BDX_STRUCT uboxSMICtrlReg;
  UINT16                      Data16;
  UINT32                      Data32;
  UINT32                      GpiRouteData;
  UINT16                      PmBase;

  DEBUG((EFI_D_ERROR, "EnableSoftADR\n"));
  // Enable or disable Global SMI generation
  //
  uboxSMICtrlReg.Data = mCpuCsrAccess->ReadMemCpuCsr(0, 0, SMICTRL_UBOX_MISC_REG);
  uboxSMICtrlReg.Bits.smidis = 0;
  mCpuCsrAccess->WriteMemCpuCsr(0, 0, SMICTRL_UBOX_MISC_REG, uboxSMICtrlReg.Data);

  // Enable SMI Logic such that an SMI is generated upon any of the anticipated errors.
  //

  //
  // Get the PM Base Address
  //
  PmBase = PciRead16(PCI_LIB_ADDRESS(
    DEFAULT_PCI_BUS_NUMBER_PCH, // Bus
    PCI_DEVICE_NUMBER_PCH_LPC,
    PCI_FUNCTION_NUMBER_PCH_LPC,
    R_PCH_LPC_ACPI_BASE         // Register
    ));

  //
  // Base Address should be in bits 15:7 of PMBASE register 9.1.1 page 280, but 0x400 is bigger
  // so they use the whole 16 bit
  //
  PmBase = (UINT16)(PmBase & 0xFF80);

  //
  // enable ICH error generation
  // Setup the GBL_SMI_EN=1 to only cause SMI.
  // Set bit 13 TCO_EN and bit 0 GBL_SMI_EN
  //
  Data16 = IoRead16((PmBase + R_ACPI_SMI_EN));
  Data16 = (UINT16)(Data16 | B_GBL_SMI_EN);
  IoWrite16((PmBase + R_ACPI_SMI_EN), Data16);

  //
  // Clear all Spurious Sources of the SMI at the SMI_STS register offset PMBASE + 34
  //
  Data32 = IoRead32((PmBase + R_ACPI_SMI_STS));
  IoWrite32((PmBase + R_ACPI_SMI_STS), Data32);


  // Set ALT_GPI_SMI_EN 
  Data32 = IoRead32((PmBase + R_ACPI_ALT_GP_SMI_EN));

  Data32 |= 1 << ICH_GPI_SMI_SOFTADR;

  IoWrite32((PmBase + R_ACPI_ALT_GP_SMI_EN), Data32);

  //
  // Route GPI to cause SMI 
  //
  GpiRouteData = PciRead32(PCI_LIB_ADDRESS(
    DEFAULT_PCI_BUS_NUMBER_PCH, // Bus
    PCI_DEVICE_NUMBER_PCH_LPC,
    PCI_FUNCTION_NUMBER_PCH_LPC,
    R_PCH_LPC_GPI_ROUT  // Register
    ));

  //
  // Route GPI to SMI.
  //
  GpiRouteData = (GpiRouteData & ~(BIT8 | BIT9)) | BIT8;

  PciWrite32(PCI_LIB_ADDRESS(
    DEFAULT_PCI_BUS_NUMBER_PCH, // Bus
    PCI_DEVICE_NUMBER_PCH_LPC,
    PCI_FUNCTION_NUMBER_PCH_LPC,
    R_PCH_LPC_GPI_ROUT), // Register
    GpiRouteData
    );

  //
  // Register SMM handler for triggering Soft ADR (gpio15)
  //
  RegisterSoftADRGpiChildHandler(ICH_GPI_SMI_SOFTADR);

   //
  // clear any status that occured b/c of enabling GPI
  //
  Data16 = IoRead16((PmBase + R_ACPI_ALT_GP_SMI_STS));
  Data16 |= 1 << ICH_GPI_SMI_SOFTADR;
  IoWrite16((PmBase + R_ACPI_ALT_GP_SMI_STS), Data16);
}



EFI_STATUS
InitializeSoftADR(
IN EFI_HANDLE             ImageHandle,
IN EFI_SYSTEM_TABLE       *SystemTable
)
/*++

Routine Description:

Main routine to initialize Soft ADR as a SMI handler.

Arguments:


Returns:

Success or the actual status of soft adr init.

--*/
{
  EFI_STATUS                        Status;
  UINTN                             varSize;
  SYSTEM_CONFIGURATION  SetupData;
 
  Status = gBS->LocateProtocol(
    &gEfiSmmBase2ProtocolGuid,
    NULL,
    (VOID **)&mSmmBase2
    );
  DEBUG((EFI_D_ERROR, "Initialize SoftADR\n"));
  if (EfiInManagementInterrupt()) {
      
    // AptioV Server Override Start: EFI SMM Variable Protocol is not installed.  Using AMI NVRAM Runtime Get/Variable functions
    Status = InitAmiSmmLib (ImageHandle, SystemTable);
    /*    
    //
    // Locate SmmVariableProtocol.
    //
    Status = mSmst->SmmLocateProtocol(
      &gEfiSmmVariableProtocolGuid,
      NULL,
      (VOID**)&mSmmVariable
      );
    ASSERT_EFI_ERROR(Status);
	*/
	// AptioV Server Override End: EFI SMM Variable Protocol is not installed.  Using AMI NVRAM Runtime Get/Variable functions

    varSize = sizeof (SYSTEM_CONFIGURATION);
	// AptioV Server Override Start: EFI SMM Variable Protocol is not installed.  Using AMI NVRAM Runtime Get/Variable functions
    // Status = mSmmVariable->SmmGetVariable(
	Status = pRS->GetVariable(
      L"IntelSetup",
      &gEfiSetupVariableGuid,
      NULL,
      &varSize,
      &SetupData
      );
	DEBUG((EFI_D_ERROR, "Status after getting IntelSetup variable = %r\n", Status));
	// AptioV Server Override End: EFI SMM Variable Protocol is not installed.  Using AMI NVRAM Runtime Get/Variable functions
	
    if (EFI_ERROR(Status)) return Status;
    
    Status = mSmst->SmmLocateProtocol(&gEfiCpuCsrAccessGuid, NULL, &mCpuCsrAccess);
    ASSERT_EFI_ERROR(Status);

    // Locate the IIO Protocol Interface
    Status = gBS->LocateProtocol(&gEfiIioUdsProtocolGuid, NULL, &mIioUds);
    ASSERT_EFI_ERROR(Status);

    if (SetupData.ADREn == ADR_SOFTWARE_TRIGGER) {
        EnableSoftADR();
      } 

    if (SetupData.ADREn && SetupData.ADROnReset) {
      SetADRResetMB();
    }
   }
    return Status;
}
