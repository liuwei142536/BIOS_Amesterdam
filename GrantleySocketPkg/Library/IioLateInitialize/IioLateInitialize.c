/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2019   Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  IioInitialize.c

Abstract:

  Implement IIO initialization protocol to provide IIO initialization functions.
  This interfaces will be called by PciHostBridge driver in the PCI bus enumeration.

--*/
#include <Token.h> // AptioV server override
#include "IioLateInitialize.h"

// Aptio V Server Override Start : Changes done to resolve the Legacy Boot hang Issue when PAM LOCK BIT set
#define AMI_LEGACY_BOOT_PROTOCOL_GUID \
    { 0x120d28aa, 0x6630, 0x46f0, { 0x81, 0x57, 0xc0, 0xad, 0xc2, 0x38, 0x3b, 0xf5 } }

static EFI_GUID gAmiLegacyBootProtocolGuid     = AMI_LEGACY_BOOT_PROTOCOL_GUID;
// Aptio V Server Override End : Changes done to resolve the Legacy Boot hang Issue when PAM LOCK BIT set
EFI_HANDLE mDriverImageHandle;

VOID
EFIAPI
OnExitBootServices (
  EFI_EVENT                 Event,
  VOID                      *Context
  );

VOID
EFIAPI
IioInitAfterResources (
  );

VOID
EFIAPI
IioInitAfterPciBusNumberEnumeration (
);

VOID S3_PciExpressWrite32( 
  IN  IIO_GLOBALS       *IioGlobalData,
  IN  UINT64    	Address,
  IN  UINT32    	Data
  )
{
  PciExpressWrite32((UINTN)Address,  Data);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, IioGlobalData->IioVData.PciExpressBase + Address, 1, &Data );
}


VOID S3_PciExpressWrite16( 
  IN  IIO_GLOBALS       *IioGlobalData,
  IN  UINT64    	Address,
  IN  UINT16    		Data
  )
{
  PciExpressWrite16((UINTN)Address,  Data);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint16, IioGlobalData->IioVData.PciExpressBase + Address, 1, &Data );
}


VOID S3_PciExpressWrite8( 
  IN  IIO_GLOBALS       *IioGlobalData,
  IN  UINT64    	Address,
  IN  UINT8    		Data
  )
{
  PciExpressWrite8((UINTN)Address,  Data);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint8, IioGlobalData->IioVData.PciExpressBase + Address, 1, &Data );
}

VOID S3_Bios2PcodeMailBoxWrite(
  IN  IIO_GLOBALS       *IioGlobalData,
  IN  UINT8             Iio,
  IN  UINT32            MailBoxCommand,
  IN  UINT32            MailboxData
)
{
  UINTN    RegAddr;
  UINT8    Size;
  UINT32   Data;

  RegAddr = IioGlobalData->IioOutData.CpuCsrAccess->GetCpuCsrAddress( Iio, 0, BIOS_MAILBOX_DATA_PCU_FUN1_REG, &Size);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &MailboxData );

  Data = (UINT32)(MailBoxCommand | BIT31);
  RegAddr = IioGlobalData->IioOutData.CpuCsrAccess->GetCpuCsrAddress( Iio, 0, BIOS_MAILBOX_INTERFACE_PCU_FUN1_REG, &Size);
  S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, RegAddr, 1, &Data );  
}


UINT8
GetAssignedPortIndexWidthLate(
    IIO_GLOBALS             *IioGlobalData,
    UINT8                  Iio,
    UINT8                  PortIndex
)
{
  return IioGlobalData->IioOutData.CurrentPXPMap[(Iio*NUMBER_PORTS_PER_SOCKET)+PortIndex];
}

VOID
ProgProblematicPortConfig(
  IIO_GLOBALS                *IioGlobalData
  );

BOOLEAN
IioVData_ConfigurePciePortLate(
    IIO_GLOBALS                     *IioGlobalData,
    UINT8                           Iio,
    UINT8                           PortIndex
    )
{
  return (IioGlobalData->IioOutData.PciePortConfig[(Iio * NUMBER_PORTS_PER_SOCKET) + PortIndex] == 0)  \
          ? FALSE  : TRUE;
}

UINT8
CheckVarPortLinkSpeedLate(
    IIO_GLOBALS                                     *IioGlobalData,
    UINT8                                           Iio,
    UINT8                                           PortIndex
)
{
  UINT8   PcieLinkSpeed;

  if (((Iio * NUMBER_PORTS_PER_SOCKET) + PortIndex) >= MAX_TOTAL_PORTS) {
    return PCIE_LINK_SPEED_AUTO;
  }
  PcieLinkSpeed = \
      IioGlobalData->SetupData.PciePortLinkSpeed[(Iio * NUMBER_PORTS_PER_SOCKET) + PortIndex];

  if(PcieLinkSpeed == PCIE_LINK_SPEED_AUTO){
    //
    // HSD 3616041: change S0 port3D (uplink port) default to Gen1.
    if (IsThisUplinkPortLate(IioGlobalData, Iio, PortIndex) == TRUE){
      return PCIE_LINK_SPEED_GEN1;       //Gen1 speed
    }
    else if (PortIndex == NONE_GEN3_PCIE_PORT)  //port 0 cannot go to Gen3
      return PCIE_LINK_SPEED_GEN2;              //expected HW default for PCIe port 0
    else
      return PCIE_LINK_SPEED_GEN3;              //expected HW default for all other ports
  }
  else
    return PcieLinkSpeed;
}

BOOLEAN
ChangeVarPortLinkSpeedLate(
    IIO_GLOBALS                                     *IioGlobalData,
    UINT8                                           Iio,
    UINT8                                           PortIndex
)
{
  //
  // HSD 3616041: change S0 port3D (uplink port) default to Gen1.
  //
  if (IsThisUplinkPortLate(IioGlobalData, Iio, PortIndex) == TRUE)
    return TRUE;            //speed change required in case of AUTO
  else
    return FALSE;           // no speed change required in case of other ports
}

UINT8
CheckVarPortASPMLate(
    IIO_GLOBALS                                     *IioGlobalData,
    UINT8                                           Iio,
    UINT8                                           PortIndex
)
{
  UINT8       Aspm;

  if (((Iio * NUMBER_PORTS_PER_SOCKET) + PortIndex) >= MAX_TOTAL_PORTS) {
    return PCIE_ASPM_DISABLE;
  }
  Aspm = IioGlobalData->SetupData.PcieAspm[(Iio * NUMBER_PORTS_PER_SOCKET) + PortIndex];

  if(Aspm == PCIE_ASPM_AUTO){
    //as per ss4030806 - if uplink enabled than ASPM = L1
    if (IsThisUplinkPortLate(IioGlobalData, Iio, (UINT8)PortIndex) == TRUE)
      Aspm = PCIE_ASPM_L1_ONLY;     // ASPM = L1
    else
      Aspm = PCIE_ASPM_DISABLE;     // ASPM = disable
  }

  return Aspm;
}

BOOLEAN
CheckVarPortEnableLate(
    IIO_GLOBALS                                         *IioGlobalData,
    UINT8                                               Iio,
    UINT8                                               PortIndex
)
{
  UINT8 PciePortDisable;

  if (((Iio * NUMBER_PORTS_PER_SOCKET) + PortIndex) >= MAX_TOTAL_PORTS) {
    return FALSE;
  }
  PciePortDisable = \
      IioGlobalData->SetupData.PciePortDisable[(Iio * NUMBER_PORTS_PER_SOCKET) + PortIndex];

  if(PciePortDisable == 0){   //auto
    // If PciePortDisable is set to "AUTO", than take care of port specific
    // special requirements, otherwise set the ports to enable by default
    // for example, as the PO safe config the requirement was to keep the
    // uplink port (if any) disable by default....
    if (IsThisUplinkPortLate(IioGlobalData, Iio, PortIndex) == TRUE)
      return FALSE;             //to indicate port is to be disabled
    else
      return TRUE;              //to indicate port is to be enabled

  }else if(PciePortDisable == 1)   //enable
    return TRUE;                //to indicate port is to be enabled
  else if(PciePortDisable == 2)   //disable
    return FALSE;               //to indicate port is to be disabled
  else
    return TRUE;                //to indicate port is to be enabled
}

BOOLEAN
IsThisUplinkPortLate(
    IIO_GLOBALS                                     *IioGlobalData,
    UINT8                                           Iio,
    UINT8                                           PortIndex
)
{
  if(Iio < MaxIIO){
    if(IioGlobalData->PreLinkData.UplinkInfo[Iio].Valid &&
       IioGlobalData->PreLinkData.UplinkInfo[Iio].IioUplinkPortIndex == PortIndex)
      return TRUE;
    else
      return FALSE;
  }
  else
    return FALSE;
}

VOID 
WaitTrainingFinishedLate (
    IIO_GLOBALS                         *IioGlobalData,
    UINT8                           BusNumber,
    UINT8                           PciePortDeviceNumber,
    UINT8                           Port
  )
{
  BOOLEAN        PcieLinkTrained; 
  UINTN          Loop;
  UINT16         Var16;
   
  //
  // We need to wait 50ms according to SA BS to let training completed
  // We used not to care about this, but now we do the port init immediately
  // after Bifurcutation, hence have  to wait enough time and also poll on
  // LNKSTS.DLLA bit as per the IIO PCIe HAS
  //
  PcieLinkTrained = FALSE;
  Loop            = 0;    
  do {
     Var16 = PciExpressRead16 (PCI_PCIE_ADDR(BusNumber, PciePortDeviceNumber, Port, R_PCIE_PORT_PXPLNKSTS));
    if ( Var16 & B_PCIE_PORT_PXPLNKSTS_DLLA ) {   //wait on DLLA bit would never set for unpopulated slots
      //
      // if completed training
      //
      PcieLinkTrained = TRUE;
    } else {
      //
      // Wait for 10ms
      // 
      IioStall (IioGlobalData, GEN3_LINK_POLL_TIMEOUT);
      Loop++;
    }    
  } while (!PcieLinkTrained && Loop < 50);

}

VOID
IioDfxInit(
    PCI_ROOT_BRIDGE_PCI_ADDRESS                         PciAddress,
    IIO_INIT_PHASE                                      Phase,
    IIO_GLOBALS                                         *IioGlobalData
  )
/*++

Routine Description:

  do DFX specific work 

Arguments:

  PciAddress        - The IIO controller PCI address
  Phase             - Defines the phase of PCI initialization that the PCI driver is executing. 
                      Either: 
                        EfiPciEnumerationDeviceScanning
                        EfiPciEnumerationBusNumberAssigned
                        EfiPciEnumerationResourceAssigned
  IioGlobalData            - Pointer to IIO Internal Data Store

Returns:

  None
--*/
{

  UINT8                    RegVal;
  
  //
  // SA BSU 0.8.11, 7.5.7
  // #3612865: Legacy GPE (HPGPE, PMEGPE, GPE) messages not routed to IRP when DMI is non-legacy
  //
  if( PciAddress.Bus != 0 && Phase == IIOInitPhase3 ) {
    RegVal = PciExpressRead8 (PCI_PCIE_ADDR(PciAddress.Bus, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(CSIPSUBSAD_IIO_DFX_GLOBAL_REG)));
    RegVal |= BIT0;
    S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(CSIPSUBSAD_IIO_DFX_GLOBAL_REG)), RegVal);
    DEBUG((EFI_D_INFO, "Legacy GPE to IRP for DMI/PCIE on bus:%d\n",PciAddress.Bus));
  }

}

VOID
IioInit (
  IIO_GLOBALS  *IioGlobalData
)
/*++

Routine Description:

  Call back routine used to do all IIO chipset initialization
          
Arguments:


Returns:

--*/

{
  PCI_ROOT_BRIDGE_PCI_ADDRESS                 PciAddress;
  UINT8                                       Phase;
  UINT8                                       i,j;

  //
  // Get necessary protocols
  //

  for ( i = 0 ; i < MaxIIO ; i++ ) {
    if (!IioGlobalData->IioVData.SocketPresent[i])
       continue;
    for (Phase = 1; Phase <= IIOInitPhase3; Phase = Phase << 1) {

      // 
      //Warning: If anyone should decide that changing order of silicon functional module calls,
      //make sure you examine all code to verify that the functions you are changing are
      //not order dependent!
      //
      
      //
      // Make WA first
      // 
      PciAddress.Bus = IioGlobalData->IioVData.SocketBaseBusNumber[i]; // iio bus here

      IioCpuWorkAround ( PciAddress, Phase, IioGlobalData, IioPortEnumeration, NULL); 
      IioWorkAround ( PciAddress, Phase, IioGlobalData, IioPortEnumeration, NULL);

      //
      // Then do the DMI and PCIE ports
      //
      for ( j = 0; j < NUMBER_PORTS_PER_SOCKET ;j++) {
        PciAddress.Bus = IioGlobalData->IioVData.SocketBaseBusNumber[i];
        PciAddress.Device   = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
        PciAddress.Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;
        if( IioVData_ConfigurePciePortLate(IioGlobalData, i, j) == FALSE)  //check whether the PCIe port can be configured
          continue;
        PciePortInit (PciAddress, Phase, IioGlobalData);
        DmiInit (PciAddress, Phase, IioGlobalData);
      }

      if (Phase == IIOInitPhase1) {
        //
        // intended fix for s4030382:Pcie cards are not powering up after
        //                  a CLDRST with Serial Debug Msgs: Disabled and no PCIE_ON jumper
        // note: the write to RP SLTCAP register triggers the "Set_Slot_Power_Limit"
        //        message to be send which is used by IIO, hence their should be some delay here
        IioStall (IioGlobalData, SLTCAP_DELAY);   //200ms delay
      }

      PciAddress.Device   = APIC_DEV_NUM;
      PciAddress.Function = APIC_FUNC_NUM;
      IioIoApicInit (PciAddress, Phase, IioGlobalData);

      PciAddress.Device   = CORE05_DEV_NUM;
      PciAddress.Function = CORE05_FUNC_NUM;
      VtdInitialization (PciAddress, Phase, IioGlobalData);

      PciAddress.Device   = IOAT_DEVICE_NUM;
      for ( j = IOAT_FUNC_START; j < IOAT_TOTAL_FUNCS ;j++) {
        PciAddress.Function = j;
        EarlyIoatInit (PciAddress, Phase, IioGlobalData);
      }

      IioDfxInit(PciAddress, Phase, IioGlobalData);

    }
  }
}

//
// Function that writes the CSR that enables PECI as trusted interface.
// HSD 4166549: Implement Power Management Security Configuration as per SeCoE recommendation
//
VOID
TxtTrustPeci(
  IN IIO_GLOBALS *pIioGlobalData)
{
  UINT8          Iio;
  UINT8          UncoreBus;
  UINT32         RegLocation;  
  PCU_LT_CTRL_PCU_FUN3_STRUCT PcuLtCtrl;

  for (Iio = 0; Iio < MaxIIO; Iio++)
  {
    if (pIioGlobalData->IioVData.SocketPresent[Iio])
    {
#ifdef EFI_DEBUG
      { //
        // PECI is Trusted (PIT) bit is sampled at RST_CPL3, make sure RST_CPL3 was not asserted yet
        //
        BIOS_RESET_CPL_PCU_FUN1_STRUCT RstCplReg;
        RstCplReg.Data = pIioGlobalData->IioOutData.CpuCsrAccess->ReadCpuCsr(Iio, 0, BIOS_RESET_CPL_PCU_FUN1_REG);
        ASSERT(!RstCplReg.Bits.rst_cpl3);
      }
#endif
      PcuLtCtrl.Data = pIioGlobalData->IioOutData.CpuCsrAccess->ReadCpuCsr(Iio, 0, PCU_LT_CTRL_PCU_FUN3_REG);
      if (PcuLtCtrl.Bits.pit == 1) {
        PcuLtCtrl.Bits.pit = 0;
        pIioGlobalData->IioOutData.CpuCsrAccess->WriteCpuCsr(Iio, 0, PCU_LT_CTRL_PCU_FUN3_REG, PcuLtCtrl.Data);
      }
      UncoreBus = pIioGlobalData->IioVData.SocketUncoreBusNumber[Iio];
      RegLocation = ONLY_REGISTER_OFFSET(PCU_LT_CTRL_PCU_FUN3_REG);
      S3BootScriptSaveMemWrite(
        S3BootScriptWidthUint32,  
        pIioGlobalData->IioVData.PciExpressBase + PCI_PCIE_ADDR(UncoreBus, PCU22_DEV_NUM, PCU22_FUNC_NUM, RegLocation), 
        1, 
        &PcuLtCtrl.Data
        );
    }
  }
}

//
// Function that writes all the CSRs and MSRs defined in the SeCoE BIOS
// recommendations document to allow the ACM to later do and SCHECK and
// find the platform to be secure.
//
// ******************************************************************** 
// ******************   WARNING  **************************************
// *******  Settings within this function must not be changed *********
// *******  without reviewing the changes with SeCoE or the   *********
// *******  TXT BIOS owner.  Changes to this function will    *********
// *******  produce corresponding failures to the ACM Lock    *********
// *******  Configuration Checks and will prevent Trusted     *********
// *******  Boot.                                             *********
// ********************************************************************
// ********************************************************************
//
VOID
TxtSecurePlatform(
    IN  IIO_GLOBALS    *IioGlobalData
)
{
  UINT8    Iio;
  UINT8    IioBus;
  UINT32   IioPortDevice, IioPortFunction;
  UINT32   RegLocation, Data32;
  ERRINJCON_IIO_PCIE_STRUCT                     ErrInjCon;
  IIO_DFX_LCK_CTL_CSR_IIO_DFX_GLOBAL_HSX_BDX_STRUCT IioDfxLock;
  TSWCTL0_IIO_DFX_GLOBAL_HSX_BDX_STRUCT             TswCtl0;
  DMIRCBAR_IIO_PCIEDMI_STRUCT                   DmiRcBar;

  DEBUG ((EFI_D_INFO, "IioInit Secure the Platform (TXT)..\n"));

  for (Iio = 0; Iio < MaxIIO; Iio++) {
    if (!IioGlobalData->IioVData.SocketPresent[Iio]) continue;

    // IIO LOCKS BELOW
    IioBus = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];

    //
    // 4986493: Error Injection changes to enable Windows Hardware Error Architecture 
    //          This change request supercedes the following HSDs: 4167177, 4168974, 4167708, 4986303
    //
    //
    // HSD 4167177: TXT BIOS Must Lock Error Injections CSRs
    //       Revised by HSD 4986493 to only lock DMI port error injection
    //
    if (Iio == 0) {
      IioPortDevice   = IioGlobalData->PreLinkData.PcieInfo.PortInfo[0].Device;
      IioPortFunction = IioGlobalData->PreLinkData.PcieInfo.PortInfo[0].Function;
      RegLocation = ONLY_REGISTER_OFFSET(ERRINJCON_IIO_PCIE_REG);
      ErrInjCon.Data = PciExpressRead16(PCI_PCIE_ADDR(IioBus, IioPortDevice, IioPortFunction, RegLocation));
      ErrInjCon.Bits.errinjdis = 1;
      S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(IioBus, IioPortDevice, IioPortFunction, RegLocation), ErrInjCon.Data);
    }

    //
    // HSD 4166541: Implement HSX/BDX Processor Security Locks as per SeCoE Recommendation
    //
    RegLocation = ONLY_REGISTER_OFFSET(IIO_DFX_LCK_CTL_CSR_IIO_DFX_GLOBAL_REG);
    IioDfxLock.Data = PciExpressRead16(PCI_PCIE_ADDR(IioBus, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, RegLocation));
    IioDfxLock.Data |= 0xFF;
    // HSD 4986116: Set IIO_DFX_LCK_CTL.ntblck bit
    IioDfxLock.Data |= 0x100;
    S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(IioBus, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, RegLocation), IioDfxLock.Data);

    //
    // HSD 4166541: Implement HSX/BDX Processor Security Locks as per SeCoE Recommendation
    //
    RegLocation = ONLY_REGISTER_OFFSET(TSWCTL0_IIO_DFX_GLOBAL_REG);
    TswCtl0.Data = PciExpressRead32(PCI_PCIE_ADDR(IioBus, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, RegLocation));
    TswCtl0.Bits.me_disable = 1;
    TswCtl0.Bits.allow_ib_mmio_cfg = 0;
    TswCtl0.Bits.ignore_acs_p2p_ma_lpbk = 0;
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(IioBus, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, RegLocation), TswCtl0.Data);

/*    
    This security lock is done in VtdInitialization()

    //
    // HSD 4166541: Implement HSX/BDX Processor Security Locks as per SeCoE Recommendation
    //
    RegLocation = ONLY_REGISTER_OFFSET(0x184);          // Pseudo reg - VTGENCTRL_IIO_VTD_REG
    VtGenCtrl.Data = PciExpressRead32(PCI_PCIE_ADDR(IioBus, CORE05_DEV_NUM, CORE05_FUNC_NUM, RegLocation));
    VtGenCtrl.Bits.lockvtd = 1;
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(IioBus, CORE05_DEV_NUM, CORE05_FUNC_NUM, RegLocation), VtGenCtrl.Data);
*/

    //
    // HSD 4166543: Implement Secure Memory Map Configuration as per SeCoE recommendation
    // ivt_eco: s4540054:  [legacy] DMIRCBAR registers that have *_LB fields are not lockable by LT-lock (or anything)
    //  [clone to s4030142] - permanent workaround
    //
    RegLocation = ONLY_REGISTER_OFFSET(DMIRCBAR_IIO_PCIEDMI_REG);
    DmiRcBar.Data = PciExpressRead32(PCI_PCIE_ADDR(IioBus, DMI_DEV_NUM, DMI_FUNC_NUM, RegLocation));
    DmiRcBar.Bits.dmircbaren = 0;
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(IioBus, DMI_DEV_NUM, DMI_FUNC_NUM, RegLocation), DmiRcBar.Data);
  }

  // Wellsburg Locks Below

  //
  // HSD 4167092: BIOS should program RCBA lock bit in WBG
  //
  Data32 = PciExpressRead32(PCI_PCIE_ADDR(0, PCH_LPC_PCI_DEVICE_NUMBER, PCH_LPC_PCI_FUNCTION_NUMBER, R_PCH_LPC_ULKMC));
  Data32 |= BIT13;
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(0, PCH_LPC_PCI_DEVICE_NUMBER, PCH_LPC_PCI_FUNCTION_NUMBER, R_PCH_LPC_ULKMC), Data32);

  //
  // HSD 4168685: WHQL: PCI Compliance fails due to SSVID registers.
  // Setting PLKCTL bit (bit0) locks down SSID and SVID registers
  //
  Data32 = PciExpressRead32(PCI_PCIE_ADDR(0, PCH_PCIE_VIRTUAL_PORT_DEVICE, PCH_PCIE_VIRTUAL_PORT_FUNCTION, PCH_VIRTUAL_PLKCTL));
  Data32 |= BIT0;
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(0, PCH_PCIE_VIRTUAL_PORT_DEVICE, PCH_PCIE_VIRTUAL_PORT_FUNCTION, PCH_VIRTUAL_PLKCTL), Data32);

}

//
// Function that writes all the CSRs and MSRs defined in the SeCoE BIOS
// recommendations document that need to be done late in the boot process.
// These are all done as a ready to boot event.
//
// ******************************************************************** 
// ******************   WARNING  **************************************
// *******  Settings within this function must not be changed *********
// *******  without reviewing the changes with SeCoE or the   *********
// *******  TXT BIOS owner.  Changes to this function will    *********
// *******  produce corresponding failures to the ACM Lock    *********
// *******  Configuration Checks and will prevent Trusted     *********
// *******  Boot.                                             *********
// ********************************************************************
// ********************************************************************
//
VOID
TxtLateSecurePlatform(
    IN  IIO_GLOBALS    *IioGlobalData
)
{
  UINT8                                 Iio;
  UINT8                                 IioBus, UncoreBus;
  UINT32                                RegLocation;
  PAM0123_CBOBC_MEMSAD_HSX_BDX_STRUCT   Pam0123;
  LTDPR_IIO_VTD_STRUCT                  LtDpr;

  DEBUG ((EFI_D_INFO, "IioInit Late Secure the Platform (TXT)..\n"));

  for (Iio = 0; Iio < MaxIIO; Iio++) {
    if (!IioGlobalData->IioVData.SocketPresent[Iio]) continue;

// IIO LOCKS BELOW
    IioBus = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];
    //
    //5003465: Fixing the CHIPSEC failures
    //  [Intel TXT DMA Protected Range register should be locked]
    LtDpr.Data = IioGlobalData->IioOutData.CpuCsrAccess->ReadCpuCsr(Iio, 0, LTDPR_IIO_VTD_REG);
    LtDpr.Bits.lock = 1;
    IioGlobalData->IioOutData.CpuCsrAccess->WriteCpuCsr(Iio, 0, LTDPR_IIO_VTD_REG, LtDpr.Data);

// UNCORE LOCKS BELOW
    UncoreBus = IioGlobalData->IioVData.SocketUncoreBusNumber[Iio];
    //
    // HSD 4166062: Add New register lock bit that would Lock down PAM configuration
    // HSD 4166542: Implement CONFIG_LOCK as per SeCoE recommendation
    //
    if ((IioGlobalData->IioVData.CpuStepping >= B0_REV && IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
      RegLocation = ONLY_REGISTER_OFFSET(PAM0123_CBOBC_MEMSAD_REG);
      Pam0123.Data = PciExpressRead32(PCI_PCIE_ADDR(UncoreBus, 15, 4, RegLocation));
      Pam0123.Bits.lock = 1;
      PciExpressWrite32(PCI_PCIE_ADDR(UncoreBus, 15, 4, RegLocation), Pam0123.Data);
    }
  }
}


VOID
IioPamRegSave(
  //IN      EFI_EVENT                 Event, //AptioV server override: Update for PAM save/restore
  //IN      VOID                      *Context //AptioV server override: Update for PAM save/restore
  VOID
)
/*++

Routine Description:

  Routine to read and store the PAM registers to the S3 boot script

Arguments:

  IN      EFI_EVENT                 Event,
  IN      VOID                      *Context

Returns:

--*/
{
  EFI_STATUS                  Status;
  UINT8                       Iio;
  UINT8                       UncoreBus;
  UINT32                      RegLocation;
  PAM0123_CBOBC_MEMSAD_HSX_BDX_STRUCT		Pam0123;
  PAM456_CBOBC_MEMSAD_STRUCT  Pam456;
  EFI_IIO_SYSTEM_PROTOCOL     *IioSystemProtocol = NULL;
  IIO_GLOBALS                 *IioGlobalData = NULL;
  //VOID                        *ProtocolPointer; //AptioV server override: Update for PAM save/restore
 
  //
  // Make sure ExitPmAuth is really installed
  // 
  //AptioV server override start: Update for PAM save/restore
  //Status = gBS->LocateProtocol (&gExitPmAuthProtocolGuid, NULL, &ProtocolPointer);
  //if (EFI_ERROR (Status)) {
  //  return;
  //}
  //AptioV server override end: Update for PAM save/restore
  //
  // Need to get the IioGlobalData
  //
  Status = gBS->LocateProtocol (&gEfiIioSystemProtocolGuid, NULL, &IioSystemProtocol);
  if (EFI_ERROR(Status))
    return;

  IioGlobalData = IioSystemProtocol->IioGlobalData;  
  
  for (Iio = 0; Iio < MaxIIO; Iio++) {
    if (!IioGlobalData->IioVData.SocketPresent[Iio]) continue;

    UncoreBus = IioGlobalData->IioVData.SocketUncoreBusNumber[Iio];

    //
    // Need to save PAM register settings to be restored on S3 resume.
    //
    if ((IioGlobalData->IioVData.CpuStepping >= B0_REV && IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
      RegLocation = ONLY_REGISTER_OFFSET(PAM0123_CBOBC_MEMSAD_REG);
      Pam0123.Data = PciExpressRead32(PCI_PCIE_ADDR(UncoreBus, 15, 4, RegLocation));
      //
      // Save lock bit for restore, but do not set it yet since we are not at runtime yet
      //
      if ((IioGlobalData->IioVData.LockChipset) && (!IioGlobalData->IioVData.EVMode)) {
        Pam0123.Bits.lock = 1;
        PciExpressWrite32(PCI_PCIE_ADDR(UncoreBus, 15, 4, RegLocation), Pam0123.Data); //AptioV server override: Update for PAM save/restore	
      }
      S3BootScriptSaveMemWrite(
              S3BootScriptWidthUint32,  
              IioGlobalData->IioVData.PciExpressBase + PCI_PCIE_ADDR(UncoreBus, 15, 4, RegLocation), 
              1, 
              &Pam0123.Data
              );

      RegLocation = ONLY_REGISTER_OFFSET(PAM456_CBOBC_MEMSAD_REG);      
      Pam456.Data = PciExpressRead32(PCI_PCIE_ADDR(UncoreBus, 15, 4, RegLocation));

      S3BootScriptSaveMemWrite(
              S3BootScriptWidthUint32,  
              IioGlobalData->IioVData.PciExpressBase + PCI_PCIE_ADDR(UncoreBus, 15, 4, RegLocation), 
              1, 
              &Pam456.Data
              );      
    }
  }
}

VOID
IioPostInit (
  IIO_GLOBALS  *IioGlobalData,
  EFI_HANDLE   DriverImageHandle
  )
/*++

Routine Description:

  Call back routine used to do all IIO post PCie port configuration, initialization

Arguments:


Returns:

--*/
{
  PCI_ROOT_BRIDGE_PCI_ADDRESS         PciAddress;
  UINT8   Phase;
  UINT8   Iio;
  UINT32  Data32 = 0;
  //VOID    *Registration; //AptioV server override: Update for PAM save/restore  

  for (Phase = 1; Phase <= IIOInitPhase3; Phase = Phase << 1) {

    for ( Iio = 0 ; Iio < MaxIIO ; Iio++ ){
      if (!IioGlobalData->IioVData.SocketPresent[Iio]) continue;

      PciAddress.Bus = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];

      IioWorkAround (PciAddress, Phase, IioGlobalData, IioEnumEnd, NULL);

      //
      // init the IIO misc. control features (Dev 5, Fun 0)
      //
      PciAddress.Device   = CORE05_DEV_NUM;
      PciAddress.Function = CORE05_FUNC_NUM;
      IioWorkAround (PciAddress, Phase, IioGlobalData, IioPortEnumProgramMISCCTRL, NULL);
      IioMiscInit(PciAddress, Phase, IioGlobalData);
      if(IioGlobalData->SetupData.DualCvIoFlow) {
        Data32 =  PciExpressRead32(PCI_PCIE_ADDR(PciAddress.Bus, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(TSWCTL_N_IIO_DFX_GLOBAL_REG)));
        Data32 &= ~(BIT3+BIT2+BIT1+BIT0);    // enable_irp_intlv_iou2, enable_irp_intlv_cbdma
        S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(TSWCTL_N_IIO_DFX_GLOBAL_REG)), Data32);
      }
    }   //for each IIO

    if(Phase == IIOInitPhase1){   //post IIO init phase 1

    }

    if(Phase == IIOInitPhase2){   //post IIO init phase 2

    }

    if(Phase == IIOInitPhase3){   //post IIO init phase 3
      ProgProblematicPortConfig(IioGlobalData);

      //let's switch to Ntb mode here
      NtbInit(IioGlobalData, DriverImageHandle);

      // any post PCIe init DMI settings are done below
      DmiPostInit(IioGlobalData);

      //
      // let power down the unrouted the PCIe ports of the platform
      //
      IioPortPowerDown(IioGlobalData);
      //
      // Enable PECI
      //
      TxtTrustPeci(IioGlobalData);

      //
      // HSD 4168370 Add EVMode Support to the HSX EP BIOS
      //
      // Call a function that locks down the platform for the SeCoE requirements
      //
      if ((IioGlobalData->IioVData.LockChipset) && (!IioGlobalData->IioVData.EVMode)) {
        TxtSecurePlatform(IioGlobalData);
      }
      //
      // let hide the PCIe root ports which are forced by user, or whose lanes are
      // used by other ports, or the ports which are not present
      //
      IioPcieDeviceHide(IioGlobalData);
      // now initiate any post IO APIC init setting; like signal for LT SCHECK to be performed
      IoApicPostInit(IioGlobalData);
      
      //
      // Save PAM registers for S3 restore just before Script lock
      //
      //AptioV server override start: Update for PAM save/restore	  
      //EfiCreateProtocolNotifyEvent (
      //  &gExitPmAuthProtocolGuid,
      //  TPL_CALLBACK,
      //  IioPamRegSave,
      //  NULL,
      //  &Registration
      //  );
      //AptioV server override end: Update for PAM save/restore	  
    }
  }   // end of post IIO init phases
}

VOID 
PciCallback (
  IN  EFI_HANDLE                                   RootBridgeHandle,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS   PciAddress,
  IN  EFI_PCI_ENUMERATION_PHASE                    Phase,
  IN  EFI_PCI_CALLBACK_CONTEXT                     *Context
  )
{
  IioInitAfterResources();
}

VOID
PciCallbackAfterPciBusNumberAssign (
  IN  EFI_HANDLE                                   RootBridgeHandle,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS  PciAddress,
  IN  EFI_PCI_ENUMERATION_PHASE                    Phase,
  IN  EFI_PCI_CALLBACK_CONTEXT                     *Context
  )
{
  IioInitAfterPciBusNumberEnumeration();
}

EFI_STATUS
IioLateInitializeEntry (
  IIO_GLOBALS                *IioGlobalData,
  EFI_HANDLE                 DriverImageHandle
  )
/*++

Routine Description:

  Install the IIO Initialization Protocol

Arguments:

  IIO Global Data
  
Returns:

  Status
    EFI_SUCCESS           - Installed successfully
    Others                - Failed to install this protocol

--*/
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  EFI_PCI_CALLBACK_PROTOCOL     *PciCallBackProtocol;
   
  mDriverImageHandle = DriverImageHandle;

  if (!IioGlobalData->IioVData.RasOperation) {
    OemIioInitDriverInstalled(IioGlobalData);
  }

  IioInit(IioGlobalData);
  
  Status = gBS->LocateProtocol ( 
               &gEfiPciCallbackProtocolGuid, 
               NULL, 
               &PciCallBackProtocol
               );
  ASSERT_EFI_ERROR(Status); 

  Status = PciCallBackProtocol->RegisterPciCallback(
                       PciCallBackProtocol,
                       PciCallback,
                       EfiPciEnumerationResourceAssigned
                       );
  ASSERT_EFI_ERROR (Status);
  DEBUG ((EFI_D_INFO, "Register a PciCallback for after PCI resources are assigned.\n"));

  Status = PciCallBackProtocol->RegisterPciCallback(
                      PciCallBackProtocol,
                      PciCallbackAfterPciBusNumberAssign,
                      EfiPciEnumerationBusNumberAssigned
                      );
  ASSERT_EFI_ERROR (Status);
  DEBUG ((EFI_D_INFO, "Register a PciCallback for after PCI bus numbers are assigned.\n"));

  IioPostInit(IioGlobalData, DriverImageHandle);
 
  if (!IioGlobalData->IioVData.RasOperation) {
    EFI_EVENT                     mExitBootServicesEvent;
    EFI_EVENT                     mLegacyEvent;
    VOID                          *EventRegistration; // Aptio V Server Override : Changes done to resolve the Legacy Boot hang Issue when PAM LOCK BIT set

    PrintIioPciePortPcieLinkStatus(IioGlobalData);
  
   /* // Aptio V Server Override Start: Changes done to resolve the Legacy Boot hang Issue when PAM LOCK BIT set
  
    //
    // Register the event to inform PAM locking proc that we are ready to boot legacy
    //
    Status = EfiCreateEventLegacyBootEx (
             TPL_NOTIFY,
             OnExitBootServices,
             NULL,
             &mLegacyEvent
             );
    ASSERT_EFI_ERROR(Status);             
    */
   
        
        //
        // Register for protocol notifactions
        //

        Status = gBS->CreateEvent (
                             	 	 EVT_NOTIFY_SIGNAL,
                             	 	 TPL_CALLBACK,
                             	 	 OnExitBootServices,
                             	 	 NULL,
                             	 	 &mLegacyEvent
                             	  );
        ASSERT_EFI_ERROR (Status);

        //
        // Register for protocol notifications on this event
        //

        Status = gBS->RegisterProtocolNotify (
            		 	 	 	 &gAmiLegacyBootProtocolGuid,
            		 	 	 	 mLegacyEvent,
            		 	 	 	 &EventRegistration
                             	 	 );

        ASSERT_EFI_ERROR (Status);

    // Aptio V Server Override End : Changes done to resolve the Legacy Boot hang Issue when PAM LOCK BIT set    


    //
    // Also need to register the callback for UEFI boot
    //
    Status = gBS->CreateEventEx (
              EVT_NOTIFY_SIGNAL,
              TPL_NOTIFY,
              OnExitBootServices,
              NULL,
              &gEfiEventExitBootServicesGuid,
              &mExitBootServicesEvent
              );

    ASSERT_EFI_ERROR (Status);           
             
  }

  return Status;
}

VOID
IioMiscHide (
  IIO_GLOBALS                           *IioGlobalData,
  UINT8                                 Iio
  )

{

	/*
	 *  4168685
	 *  need to hide the devices for all systems as described:
	 *
	devhide0 |= 0x000000c0
    devhide1 |= 0x000000c0
    devhide2 |= 0x000400c0
    devhide3 |= 0x000000e0
    devhide4 |= 0x000000c0
    devhide5 |= 0x000007e0
    devhide6 |= 0x000407e0  // Also hide the IIO PMON function
    devhide7 |= 0x000005e0
    * This code hides the additional QPI function and DDR channel 0 not in EN SKUs
   if (EN SKU) {
     devhide0 |= 0x00000200
     devhide2 |= 0x00080200
     devhide3 |= 0x00000200
     devhide4 |= 0x00100000
     devhide5 |= 0x00100000
   }

   * This code hides the other memory channels not needed when each MC only drives 2 channels
   if (EP 2HA SKU) {
     devhide0 |= 0x01200000
     devhide1 |= 0x01200000
     devhide2 |= 0x01200000
     devhide3 |= 0x01200000
     devhide4 |= 0x00480000
     devhide5 |= 0x00480000
   }

   * This code hides the second R3QPI unit which is only used in EX
   If (NOT EX SKU) {
     devhide4 |= 0x00000800
     devhide5 |= 0x00000800
     devhide6 |= 0x00000800
   }

	* Hide untrained QPI ports because an extra security requirement. (Done in QPI Main)
	if (QPI0 untrained) {
	   devhide0 |= 0x00000100
	   devhide2 |= 0x00000100
	   devhide3 |= 0x00000100
	}
	if (QPI1 untrained) {
	   devhide0 |= 0x00000200
	   devhide2 |= 0x00000200
	   devhide3 |= 0x00000200
	}
	if (EX and QPI2 untrained) {
	   devhide0 |= 0x00000400
	   devhide2 |= 0x00000400
	   devhide3 |= 0x00000400
	}

  * Hide the South Complex PMC device for security concerns
  if (BDX-DE SKU) {
    devhide7 |= 0x40000000
  }
	*/

  UINT32  Value32;
  UINT32  Data32;
  UINT8   BusNumber = IioGlobalData->IioVData.SocketUncoreBusNumber[Iio];
  
  DEBUG ((EFI_D_ERROR, "Hide devices in Bus:%d\n",BusNumber));
  // devhide0 |= 0x000000c0
  Value32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN0));
  Value32 |= BIT6 | BIT7;
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN0),Value32);
  // devhide1 |= 0x000000C0
  Value32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN1));
  Value32 |= BIT6 | BIT7;
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN1),Value32);
  // devhide2 |= 0x000400C0
  Value32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN2));
  Value32 |= BIT6 | BIT7 | BIT18;
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN2),Value32);
  // devhide3 |= 0x000000E0
  Value32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN3));
  Value32 |= BIT5 |  BIT6 | BIT7;
  if(IioGlobalData->SetupData.PCUF3Hide != 0) {
    Value32 |= BIT30;
  }
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN3),Value32);
  // devhide4 |= 0x000000C0
  Value32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN4));
  Value32 |= BIT6 | BIT7;
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN4),Value32);
  // devhide5 |= 0x000007E0
  Value32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN5));
  Value32 |= BIT5 | BIT6 | BIT7 | BIT8 | BIT9 | BIT10;
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN5),Value32);
  // devhide6 |= 0x000407E0 also hide the IIO PMON function.
  Value32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN6));
  Value32 |= BIT5 | BIT6 | BIT7 | BIT8 | BIT9 | BIT10 | BIT18;
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN6),Value32);
  // devhide7 |= 0x000005e0
  Value32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN7));
  Value32 |= BIT5 | BIT7 |BIT6 |BIT8 | BIT10;
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN7),Value32);

  // This code hides the additional QPI function and DDR channel 0 not in EN SKUs
  // if EN SKU:
  if (((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuSubType == CPU_HSX_1HA )) ||
	  ((IioGlobalData->IioVData.CpuType == CPU_BDX) && (IioGlobalData->IioVData.CpuSubType == CPU_BDX_1HA )) ) {

    // [5002007]: QPI1 should not be hidden for 1 HA
  }

  //
  // QP11 & DDRIO0 should be hidden for EN-2S (EN-UP)
  //
  if( IioGlobalData->IioVData.CpuType == CPU_HSX || IioGlobalData->IioVData.CpuType == CPU_BDX )
  {
    UINT32  capid0 = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(BusNumber,
                                                                      PCU22_DEV_NUM,
                                                                      PCU22_FUNC_NUM,
                                                                      ONLY_REGISTER_OFFSET(CAPID0_PCU_FUN3_REG)));
    UINT32  devhide[5];

    // devhide0 |= 0x00000200
    devhide[0] = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN0));
    //devhide2 |= 0x00000200
    devhide[1] = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN2));
    //devhide3 |= 0x00000200
    devhide[2] = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN3));
    // devhide4 |= 0x00100000
    devhide[3] = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN4));
    // devhide5 |= 0x00100000
    devhide[4] = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN5));

    switch(capid0 & 0x0000003F){
      case 2: //EN-2S
      case 1: //EN-UP
        devhide[0] |= BIT9;
        devhide[1] |= BIT9;
        devhide[2] |= BIT9;
        devhide[3] |= BIT20;
        devhide[4] |= BIT20;
        break;
      default:
        break;
    }
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN0),devhide[0]);
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN2),devhide[1]);
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN3),devhide[2]);
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN4),devhide[3]);
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN5),devhide[4]);

  }

  // This code hides the other memory channels not needed when each MC only drives 2 channels
  //   if EP 2HA SKU
  if (((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuSubType == CPU_HSX_2HA )) ||
	  ((IioGlobalData->IioVData.CpuType == CPU_BDX) && (IioGlobalData->IioVData.CpuSubType == CPU_BDX_2HA )) ) {
    // devhide0 |= 0x01200000
	Value32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN0));
    Value32 |= BIT21 | BIT24;
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN0),Value32);

    // devhide1 |= 0x01200000
  Value32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN1));
    Value32 |= BIT21 | BIT24;
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN1),Value32);

    // devhide2 |= 0x01200000
  Value32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN2));
    Value32 |= BIT21 | BIT24;
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN2),Value32);
                
    // devhide3 |= 0x01200000
  Value32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN3));
    Value32 |= BIT21 | BIT24;
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN3),Value32);

    // devhide4 |= 0x00480000
    Value32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN4));
    Value32 |= BIT19 | BIT22;
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN4),Value32);

    // devhide5 |= 0x00480000
    Value32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN5));
    Value32 |= BIT19 | BIT22;
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN5),Value32);
  }

  //  This code hides the second R3QPI unit which is only used in EX
  if (((IioGlobalData->IioVData.CpuType == CPU_HSX) && !(IioGlobalData->IioVData.CpuSubType == CPU_HSX_EX )) ||
      ((IioGlobalData->IioVData.CpuType == CPU_BDX) && !(IioGlobalData->IioVData.CpuSubType == CPU_BDX_EX )) ) {
    // devhide4 |= 0x00000800
    Value32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN4));
    Value32 |= BIT11;
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN4),Value32);

    // devhide5 |= 0x00000800
    Value32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN5));
    Value32 |= BIT11;
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN5),Value32);

    // devhide6 |= 0x00000800
    Value32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN6));
    Value32 |= BIT11;
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN6),Value32);
  }

#ifdef DE_SKU  
  if (IioGlobalData->IioVData.CpuType == CPU_BDX) {
    //
    // bdx_eco : 281007 : CLONE from broadwell_server: Untrusted code can disable LAN ports permanently until cold reset affecting availability of network features
    // [cloned to 4987263] - permanent (Hide the South Complex PMC device 30 , Funtion 7 for security concern)
    //
    Value32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN7));
    Value32 |= BIT30;
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, R_IIO_DEVHIDE_FUN7),Value32);
  } // CPU_BDX
#endif // DE_SKU 
  //
  //HSD-4987330 :The bus-device-function combinations which are under devhide in customer-reference-BIOS.
  //


  // Hide untrained QPI ports because an extra security requirement.
  // Done in QPI MAin.

  //
  // BDX new enhancement 4988536: BIOS needs to program devhide settings in PCU/VCU appropriately
  //
  //  [5002641]: Revert back the DEVHIDE settings for PCU/VCU of BDX-EP
  if (IioGlobalData->IioVData.CpuType == CPU_BDX) {
    UINT8 DevHideFn, DevHideSz = sizeof(DEVHIDE0_UBOX_MISC_STRUCT);
    UINTN TotalDevHideFns = ONLY_REGISTER_OFFSET(DEVHIDE7_UBOX_MISC_REG) - ONLY_REGISTER_OFFSET(DEVHIDE0_UBOX_MISC_REG);
    TotalDevHideFns = (TotalDevHideFns + DevHideSz) / DevHideSz;
    for(DevHideFn = 0; DevHideFn < (UINT8)TotalDevHideFns; DevHideFn++){
      Data32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, ONLY_REGISTER_OFFSET(DEVHIDE0_UBOX_MISC_REG) + (DevHideFn * DevHideSz)));
      S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, PCU22_DEV_NUM, PCU22_FUNC_NUM, ONLY_REGISTER_OFFSET(DEVHIDE0_PCU_FUN3_REG) + (DevHideFn * DevHideSz)),Data32);
      S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, VCU2_DEV_NUM, VCU2_FUNC_NUM, ONLY_REGISTER_OFFSET(DEVHIDE0_VCU_FUN_2_REG) + (DevHideFn * DevHideSz)),Data32);
    }
    //
    // Hide MC devices even if it is not restricted from software access since Dengate
    // is ready with PCS command to access MC device PMON counters
    //
    Data32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, PCU22_DEV_NUM, PCU22_FUNC_NUM, ONLY_REGISTER_OFFSET(DEVHIDE0_PCU_FUN3_REG)));
    Data32 |= BIT20 + BIT21 + BIT23 + BIT24;  //MC0 CH0; MC0 CH2; MC1 CH0; MC1 CH2
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, PCU22_DEV_NUM, PCU22_FUNC_NUM, ONLY_REGISTER_OFFSET(DEVHIDE0_PCU_FUN3_REG)),Data32);
    Data32 = PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, PCU22_DEV_NUM, PCU22_FUNC_NUM, ONLY_REGISTER_OFFSET(DEVHIDE1_PCU_FUN3_REG)));
    Data32 |= BIT20 + BIT21 + BIT23 + BIT24;  //MC0 CH1; MC0 CH3; MC1 CH1; MC1 CH3
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, PCU22_DEV_NUM, PCU22_FUNC_NUM, ONLY_REGISTER_OFFSET(DEVHIDE1_PCU_FUN3_REG)),Data32);
  }

}

VOID 
IioPcieDeviceHide(  
  IIO_GLOBALS                           *IioGlobalData
  )
{
  UINTN   PortIndex;
  UINT8   Iio;
  UINT8   BusNumber, BusBase, i;
  UINT8   FuncX=0, DevNum=0;
  UINT32  Value32, Value32b;
  UINT8   ClrHdrMfd[MaxPortDevices];           // Clear Header register per-Iou    
  UINT8   DeviceNum, ClrHdrMfdVal8 = 0; // Clear Header register Multi-function device bit. 

  DEBUG ((EFI_D_INFO, "IioInit PCIe device hide..\n"));

  for ( Iio = 0 ; Iio < MaxIIO ; Iio++ ){
      // Reset ClrHdr value for each Iio.
      ClrHdrMfdVal8 = 0;      // Value to program into ClrHdrMfd field
      ClrHdrMfd[0] = 0;       // Device 0 is always single-function
      ClrHdrMfd[1] = BIT1;    // Device 1 only has 2 ports (Func 1 represented here)
      for(DeviceNum = 2; DeviceNum < MaxPortDevices; DeviceNum++) {
        ClrHdrMfd[DeviceNum] = BIT3 | BIT2 | BIT1;   // All other devices have 4 ports (Funcs 1-3 represented here)
      }
      if (!IioGlobalData->IioVData.SocketPresent[Iio]) continue;

      BusNumber = IioGlobalData->IioVData.SocketUncoreBusNumber[Iio];

      for( PortIndex = 0; PortIndex < NUMBER_PORTS_PER_SOCKET; PortIndex++ ){
        if( !Iio && !PortIndex )  continue;   //DMI port

        // Per PCI specification, if any function is present on a device, then Function 0 must be present as well
        switch (PortIndex) {
        case 1:
          if (!(IioGlobalData->SetupData.PEXPHIDE[ (Iio * NUMBER_PORTS_PER_SOCKET) + PortIndex + 1])) {
            IioGlobalData->SetupData.PEXPHIDE[ (Iio * NUMBER_PORTS_PER_SOCKET) + PortIndex] = 0;
          }
          break;

        case 3:
        case 7:
          for (i = 1 ; i < 4 ; i++) {
            if (!(IioGlobalData->SetupData.PEXPHIDE[ (Iio * NUMBER_PORTS_PER_SOCKET) + PortIndex + i])) {
              IioGlobalData->SetupData.PEXPHIDE[ (Iio * NUMBER_PORTS_PER_SOCKET) + PortIndex] = 0;
            }
          }
          break;
        }

        if(IioGlobalData->SetupData.PEXPHIDE[ (Iio * NUMBER_PORTS_PER_SOCKET) + PortIndex]){

          Value32 = 0;
          switch(PortIndex){
          case  0:    //PCIe Port 0
            Value32 |= BIT0;        //hide D0, F0
            FuncX = 0;
            DevNum = 0;
            break;
          case  1:    //PCIe Port 1A
            Value32 |= BIT1;    //hide D1, F0
            FuncX = 0;
            DevNum = 1;
            break;
          case  2:    //PCIe Port 1B
            Value32 |= BIT1;    //hide D1, F1
            FuncX = 1;
            DevNum = 1;
            break;
          case  3:    //PCIe Port 2A
            Value32 |= BIT2;        //hide D2, F0
            FuncX = 0;
            DevNum = 2;
            break;
          case  4:    //PCIe Port 2B
            Value32 |= BIT2;        //hide D2, F1
            FuncX = 1;
            DevNum = 2;
            break;
          case  5:    //PCIe Port 2C
            Value32 |= BIT2;        //hide D2, F2
            FuncX = 2;
            DevNum = 2;
            break;
          case  6:    //PCIe Port 2D
            Value32 |= BIT2;        //hide D2, F3
            FuncX = 3;
            DevNum = 2;
            break;
          case  7:    //PCIe Port 3A
            Value32 |= BIT3;        //hide D3, F0
            FuncX = 0;
            DevNum = 3;
            break;
          case  8:    //PCIe Port 3B
            Value32 |= BIT3;        //hide D3, F1
            FuncX = 1;
            DevNum = 3;
            break;
          case  9:    //PCIe Port 3C
            Value32 |= BIT3;        //hide D3, F2
            FuncX = 2;
            DevNum = 3;
            break;
          case  10:    //PCIe Port 3D
            Value32 |= BIT3;        //hide D3, F3
            FuncX = 3;
            DevNum = 3;
            break;
          default:
            break;
          }
          Value32b = Value32;
          ClrHdrMfd[DevNum] &= ~(1 << FuncX);  // Clear the bit representing the function being disabled in this device.

          // Mask the device as hide in UBOX device 11 function 3
          Value32 |= PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, \
                                      (R_IIO_DEVHIDE_FUN0 + (FuncX * 4))));
          S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, \
                            (R_IIO_DEVHIDE_FUN0 + (FuncX * 4))),Value32);

          // Mask the device as hide in PCU device 10 function 3
          Value32b |= PciExpressRead32 (PCI_PCIE_ADDR(BusNumber, PCU22_DEV_NUM, PCU22_FUNC_NUM, \
              (R_IIO_PCU_DEVHIDE_FUN0 + (FuncX * 4))));
          S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, PCU22_DEV_NUM, PCU22_FUNC_NUM, \
              (R_IIO_PCU_DEVHIDE_FUN0 + (FuncX * 4))),Value32b);

          DEBUG ((EFI_D_INFO, "Bus=%d, Device=%d, Function=%d is hidden.\n", BusNumber, DevNum, FuncX));
        }
      }
      for(DeviceNum = 1; DeviceNum < 4; DeviceNum++) {
        // If all Functions 1-3 of each Device are hidden, then this device should be a 
        // single-function device, so set the matching bit in ClrHdrMfd bit field.
        if(ClrHdrMfd[DeviceNum] == 0) {
          ClrHdrMfdVal8 |= 1 << (DeviceNum - 1);
        }
      }
      
      // Program Clr_HdrMfd register with the intended value
      BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];
      S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(BusBase, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IIO_HDRTYPECTRL), ClrHdrMfdVal8);
  }
}

VOID
EFIAPI
OnExitBootServices (
      EFI_EVENT                 Event,
      VOID                      *Context
  )
{
  EFI_STATUS                    Status;
  EFI_IIO_SYSTEM_PROTOCOL       *IioSystemProtocol = NULL;
  IIO_GLOBALS                   *IioGlobalData = NULL;

  Status = gBS->LocateProtocol (&gEfiIioSystemProtocolGuid, NULL, &IioSystemProtocol);
  if (EFI_ERROR(Status))
    return;
  //
  // If this callback routine on its ready to boot event, than avoid executing
  // multiple times (fix for s4030764)
  //
  if (IioGlobalData->IioVData.CompletedReadyToBootEventServices == TRUE)
    return;

  DEBUG ((EFI_D_INFO, "OnExitBootServices..\n"));

  IioGlobalData = IioSystemProtocol->IioGlobalData;

  //
  //
  // HSD 4168370 Add EVMode Support to the HSX EP BIOS
  //
  // Late TXT Secure Platform Locks
  //
  //AptioV server override start: Update for PAM save/restore  
  //if ((IioGlobalData->IioVData.LockChipset) && (!IioGlobalData->IioVData.EVMode)) {
  //      TxtLateSecurePlatform(IioGlobalData);
  //}
  IioPamRegSave();
  //AptioV server override end: Update for PAM save/restore
  
  IioGlobalData->IioVData.CompletedReadyToBootEventServices = TRUE;    //fix for s4030764  
}

VOID
UpdateUncoreSsids (
  IIO_GLOBALS *IioGlobalData
  )
{
  UINT8       Iio;
  UINT8       Bus;
  UINT8       Device;
  UINT8       Function;
  UINT32      Value;

  for (Iio = 0; Iio < MaxIIO; Iio++) {

    //
    // If the IIO is not present, skip to the next index.
    //
    if (!IioGlobalData->IioVData.SocketPresent [Iio]) {
      continue;
    }

    //
    // Get the uncore bus number assigned to this IIO.
    //
    Bus = IioGlobalData->IioVData.SocketUncoreBusNumber [Iio];

    for (Device = 0; Device < 32; Device++) {
      for (Function = 0; Function < 8; Function++) {

        //
        // Read the VID/DID for this BDF from offset 0x00.
        //
        Value = PciExpressRead32 (PCI_PCIE_ADDR (Bus, Device, Function, 0x00));

        //
        // Check for invalid functions. If function 0 is invalid, the whole
        // device is considered invalid (no more function numbers are tried).
        //
        if (Value == 0xffffffff) {
          if (Function == 0) {
            break;            // try the next device number.
          } else {
            continue;         // try the next function number.
          }
        }

        //
        // Program the SSID register at offset 0x2c with the VID/DID value.
        //
        S3_PciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR (Bus, Device, Function, 0x2c), Value);
      }
    }
  }
}

VOID
EFIAPI
IioInitAfterResources (
  )
{
    EFI_STATUS Status;
    UINT8   Iio;
    UINT8   Phase;
    PCI_ROOT_BRIDGE_PCI_ADDRESS PciAddress;
    EFI_IIO_SYSTEM_PROTOCOL       *IioSystemProtocol = NULL;
    IIO_GLOBALS                   *IioGlobalData = NULL;

    ///
    /// Check whether this is real ExitPmAuth notification, or just a SignalEvent
    ///

    Status = gBS->LocateProtocol (&gEfiIioSystemProtocolGuid, NULL, &IioSystemProtocol);
    if (EFI_ERROR(Status))
      return;

    IioGlobalData = IioSystemProtocol->IioGlobalData;

    //
    // If this callback routine on its ready to boot event, than avoid executing
    // multiple times (fix for s4030764)
    //
    if (IioGlobalData->IioVData.CompletePostPciResourceAllocationServices == TRUE)
      return;

    DEBUG ((EFI_D_INFO, "IIO PCI callback event after PCI resource allocation..\n"));

    for (Phase = 1; Phase <= IIOInitPhase3; Phase = Phase << 1) {

      for ( Iio = 0 ; Iio < MaxIIO ; Iio++ ){
        if (!IioGlobalData->IioVData.SocketPresent[Iio]) continue;

        PciAddress.Bus = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];
        /*
         * Note:  move any IIO Si WA or IIO PCIe ports configuration after PCI Bus number
         *        enumeration phase, there is no need to wait for the completion of
         *        system PCI resource allocation; there is no requirement to apply WA
         *        or configure IIO PCIe ports after the PCI Memory / IO space allocation.
         */
        //
        // init IOAT features here as the CB BAR needs to be programmed by the
        // system PCI bus driver
        //
        IioIoatInitBootEvent(IioGlobalData, Iio, Phase);

        if(Phase == IIOInitPhase1){
          //
          // init the IOAPIC after the MBAR allocation during the system PCI bus enumeration
          //
          IioIoApicInitBootEvent(IioGlobalData, Iio);
        }

        if(Phase == IIOInitPhase2){

        }

        if(Phase == IIOInitPhase3){

        }
      }   //phases wise init for each IIO
   }   //end of post PCI resource allocation completion of IIO init phases
#ifndef DE_SKU
   //
   // BDX s5097328: :[CBDMA] Lock TOR timeout with CBDMA and CPU traffic
   // [cloned to 5001827]
   if(IioGlobalData->IioVData.CpuFamily == CPU_FAMILY_BDX && IioGlobalData->IioVData.CpuStepping == A0_REV_BDX){
     if(IioGlobalData->BdxSetupData.Apply_s328_cbdma_WA)
       IioIoat_s328_WA(IioGlobalData);
   }
#endif  //DE_SKU
   UpdateUncoreSsids (IioGlobalData);

   IioGlobalData->IioVData.CompletePostPciResourceAllocationServices = TRUE;    //fix for s4030764
}

VOID
EFIAPI
IioInitAfterPciBusNumberEnumeration (
  )
{
    EFI_STATUS                    Status;
    UINT8                         Iio;
    UINT8                         Phase;
    PCI_ROOT_BRIDGE_PCI_ADDRESS   PciAddress;
    EFI_IIO_SYSTEM_PROTOCOL       *IioSystemProtocol = NULL;
    IIO_GLOBALS                   *IioGlobalData = NULL;

    Status = gBS->LocateProtocol (&gEfiIioSystemProtocolGuid, NULL, &IioSystemProtocol);
    if (EFI_ERROR(Status))
      return;

    IioGlobalData = IioSystemProtocol->IioGlobalData;

    //
    // If this callback routine on its ready to boot event, than avoid executing
    // multiple times (fix for s4030764)
    //
    if (IioGlobalData->IioVData.CompletePostPciBusNumberAssignServices == TRUE)
      return;

    DEBUG ((EFI_D_INFO, "IIO PCI callback event after PCI bus assignment..\n"));

    for (Phase = 1; Phase <= IIOInitPhase3; Phase = Phase << 1) {

      for ( Iio = 0 ; Iio < MaxIIO ; Iio++ ){
        if (!IioGlobalData->IioVData.SocketPresent[Iio]) continue;

        PciAddress.Bus = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];
        //
        // perform any Si WA after the PCI Bus assignment completion here
        //
        IioWorkAround ( PciAddress, Phase, IioGlobalData, IioPostInitEnd, NULL);
        //
        // perform any PCIe port specific configuration after PCI Bus assignment completion here
        //
        IioPortsPreBootInit(IioGlobalData,Iio, Phase);


        if(Phase == IIOInitPhase1){

        }

        if(Phase == IIOInitPhase2){
        }

        if(Phase == IIOInitPhase3){
          //
          // HSD 4168370 Add EVMode Support to the HSX EP BIOS
          //
          if ((!IioGlobalData->IioVData.EVMode))
            IioMiscHide(IioGlobalData, Iio);  // Use uncore bus to hide devices
        }

      }   //phases wise init for each IIO

      if(Phase == IIOInitPhase2){   //common phase 2 for all IIOs

      }

      if(Phase == IIOInitPhase3){   //common phase 3 for all IIOs

     }
   }   //end of IIO init phases post PCI Bus enumeration completion

   IioGlobalData->IioVData.CompletePostPciBusNumberAssignServices = TRUE;    //fix for s4030764
}
//
// ******************************************************************** 
// ******************   WARNING  **************************************
// *******  Settings within this function must not be changed *********
// *******  without reviewing the changes with SeCoE or the   *********
// *******  TXT BIOS owner.  Changes to this function will    *********
// *******  produce corresponding failures to the ACM Lock    *********
// *******  Configuration Checks and will prevent Trusted     *********
// *******  Boot.                                             *********
// ********************************************************************
// ********************************************************************
//
VOID
IioSecurityFeatureInit (
    PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
    UINTN                                          Iio,
    IIO_GLOBALS                                       *IioGlobalData
)
{
  UINT32  Data32;

  //
  //  Legacy address ranges need to be protected from inbound memory reads to prevent CTOs.
  // (3614978, 3876023, 3247123, 3876024, 3876008)
  Data32 = 0x0000;   // Base address of PAM region [63:32]
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      ONLY_REGISTER_OFFSET(GENPROTRANGE2_BASE_1_IIO_VTD_REG)), Data32);
  Data32 = 0xA0000;   // Base address of PAM region [31:16], [15:0] assumed zero
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      ONLY_REGISTER_OFFSET(GENPROTRANGE2_BASE_0_IIO_VTD_REG)), Data32);

  Data32 = 0x0000;   // Limit address of PAM region [63:32]
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      ONLY_REGISTER_OFFSET(GENPROTRANGE2_LIMIT_1_IIO_VTD_REG)), Data32);
  Data32 = 0xF0000;   // Limit address of PAM region [31:16], [15:0] assumed one
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      ONLY_REGISTER_OFFSET(GENPROTRANGE2_LIMIT_0_IIO_VTD_REG)), Data32);

  //
  // HSD 4167093: TXT BIOS Must Program GenProtect Range to Overlap RCBA
  // Cover the RCBA address space
  //
  Data32 = 0x0000;   // Base address of RCBA region 
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      ONLY_REGISTER_OFFSET(GENPROTRANGE1_BASE_1_IIO_VTD_REG)), Data32);
  Data32 = PCH_RCBA_ADDRESS & 0xFFFF0000;   // Base address of RCBA region overlaps at 0xFED1C000
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      ONLY_REGISTER_OFFSET(GENPROTRANGE1_BASE_0_IIO_VTD_REG)), Data32);

  Data32 = 0x0000;   // Limit address of RCBA region
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      ONLY_REGISTER_OFFSET(GENPROTRANGE1_LIMIT_1_IIO_VTD_REG)), Data32);
  Data32 = (PCH_RCBA_ADDRESS + 0x4000) & 0xFFFF0000;   // End of RCBA Base address 0xFED20000
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      ONLY_REGISTER_OFFSET(GENPROTRANGE1_LIMIT_0_IIO_VTD_REG)), Data32);
}

VOID
IioMiscFeatureInit (
    PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
    UINTN                                          Iio,
    IIO_GLOBALS                                       *IioGlobalData
)
{
  UINT32  IioMiscCtrlDataL = 0;

  //
  //Setup the EN1K feature of IIOMISCCTRL (Register offset 0x1C0, bit 2 )
  //
  IioMiscCtrlDataL = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, R_IIO_IIOMISCCTRL));
  IioMiscCtrlDataL &= ~B_IIO_MISC_EN_1K;
  if(IioGlobalData->SetupData.EN1K)
    IioMiscCtrlDataL |= B_IIO_MISC_EN_1K;
  //
  // Remote VGA enable : default for non-legacy sockets
  //
  if(IioGlobalData->IioVData.LegacyVgaSoc == Iio )
    IioMiscCtrlDataL &= ~BIT19;                    //rvgaen = 0
  else
    IioMiscCtrlDataL |= BIT19;                    //rvgaen = 1
  // NOTE: We are asking user to explicitly indicate on which socket its VGA card
  // would be connected for the IIO apart from other configuration settings to
  // select onboard vode device or detect the VGA device from the PCIe port.....
  // TODO - further scoping on interfacing with video driver stack to make it
  // auto rather than manual.

  IioMiscCtrlDataL |= BIT8;   //enable TOCM
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, R_IIO_IIOMISCCTRL), IioMiscCtrlDataL);

  IioMiscCtrlDataL = PciExpressRead32(PCI_PCIE_ADDR( PciAddress.Bus, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IIO_IIOMISCCTRL));
  DEBUG ((EFI_D_INFO, "IIOMISCCTRL for IIO %d = 0x%x\n",Iio, IioMiscCtrlDataL));

}

VOID
IioMiscInit (
    PCI_ROOT_BRIDGE_PCI_ADDRESS                   PciAddress,
    IIO_INIT_PHASE                                Phase,
    IIO_GLOBALS                                       *IioGlobalData
)
{
  UINTN i;

  for ( i = 0 ; i < MaxIIO ; i++ ) {
      if((PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[i]) && IioGlobalData->IioVData.SocketPresent[i]) {
          break;
      }
  }
  if (i >= MaxIIO) return;
  if ( Phase == IIOInitPhase1 && i < MaxIIO ) {
    IioSecurityFeatureInit(PciAddress, i, IioGlobalData);
  }

  if ( Phase == IIOInitPhase2 && i < MaxIIO ) {
    // add code for phase 2 here
 }

  if ( Phase == IIOInitPhase3 && i < MaxIIO ) {
    IioMiscFeatureInit(PciAddress, i, IioGlobalData);
  }
}

VOID
ProgProblematicPortConfig(
  IIO_GLOBALS                *IioGlobalData
  )
{
  UINT8                                   i;
  UINT8                                   j;
  UINT8                                   BusNumber;
  UINT8                                   Device;
  UINT8                                   Function;
  UINT16                                  AcsCtrl;
  UINT32                                  IioMiscCtrl;
  UINT64                                  MiscCtrlSts;
 

  //
  // If Problematic Port functionality is disabled, do nothing
  //
  if (!IioGlobalData->SetupData.ProblematicPort) return;
   
  for (i = 0 ; i < MaxIIO ; i++) {
    // If socket is not present, no need to try to set it up!
    if (!IioGlobalData->IioVData.SocketPresent[i]) continue;
        
    BusNumber = IioGlobalData->IioVData.SocketBaseBusNumber[i];

    if(IioGlobalData->SetupData.ProblematicPort == 0x01){
        //
        // If NP_PROBLEMATIC flow, enable Lock Thawing Mode
        //
        Device = CORE05_FUNC_NUM;
        Function = CORE05_FUNC_NUM;
        IioMiscCtrl = PciExpressRead32(PCI_PCIE_ADDR(BusNumber, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IIO_IIOMISCCTRL));
        IioMiscCtrl |= B_LOCK_THAWING_MODE;   // A and B step are the same bit offset
        S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, Device, Function, R_IIO_IIOMISCCTRL), IioMiscCtrl); //.Data, S3_SAVE);
    }

    for (j = 0 ; j < NUMBER_PORTS_PER_SOCKET ; j++) {
      Device   = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
      Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;

      if (PciExpressRead16(PCI_PCIE_ADDR(BusNumber, Device, Function, 0)) != 0x8086) { //INTEL_VID) {
          continue;
      }

      //
      // Force ACS functionality to disabled
      //
      AcsCtrl = 0x00;
      S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(BusNumber, Device, Function, R_ACSCTRL), AcsCtrl);
      //
      // Program Problematic_Port_for_Lock_flows bit
      //
      MiscCtrlSts = PciExpressRead32(PCI_PCIE_ADDR(BusNumber, Device, Function, (R_PCIE_PORT_PCIEMISCCTRLSTS + 4)));
      MiscCtrlSts <<= 32;
      MiscCtrlSts |= PciExpressRead32(PCI_PCIE_ADDR(BusNumber, Device, Function, R_PCIE_PORT_PCIEMISCCTRLSTS));

      // 
      // HSD 4166662: HSX Clone: Supercollider: 3-strike timeout (Ubox Lock FSM hang)
      //                         Only set B_PROBLEMATIC_PORT_FOR_LOCK_FLOW in IVT
      //
      if (IioGlobalData->IioVData.CpuType == CPU_IVT) {
        MiscCtrlSts |= B_PROBLEMATIC_PORT_FOR_LOCK_FLOW; 
      }

      if(IioGlobalData->SetupData.ProblematicPort == 0x01){
        //
        // If NP_PROBLEMATIC flow,  Disable P2P memory reads
        //
        MiscCtrlSts |= B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_INB_P2PRdDis;
      } else {
        //
        // P_PROBLEMATIC flow, Disable P2P memory writes
        MiscCtrlSts |= B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_INB_P2PWrtDis;
      }

      S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, Device, Function, R_PCIE_PORT_PCIEMISCCTRLSTS), (UINT32)MiscCtrlSts);
      S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusNumber, Device, Function, (R_PCIE_PORT_PCIEMISCCTRLSTS + 4)), (UINT32)(MiscCtrlSts >> 32));
    } 
  }

  return;
}

VOID  PrintIioPciePortPcieLinkStatus(IIO_GLOBALS *IioGlobalData)
{
  PCI_ROOT_BRIDGE_PCI_ADDRESS PciAddress;
  UINT8 i, j;
  UINT16  wdata;

  DEBUG((EFI_D_VARIABLE, "\tDumpIioPcieLinkStatus()..\n"));
  for (i = 0 ; i < MaxIIO ; i++) {
    if (!IioGlobalData->IioVData.SocketPresent[i])
           continue;

    for (j = 0 ; j < NUMBER_PORTS_PER_SOCKET ; j++) {
      PciAddress.Bus = IioGlobalData->IioVData.SocketBaseBusNumber[i];
      PciAddress.Device   = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
      PciAddress.Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;
      if( IioVData_ConfigurePciePortLate(IioGlobalData, i, j) == FALSE)  //check whether the PCIe port can be configured
        continue;
      if( j < 1)  continue;

      wdata = PciExpressRead16(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, R_PCIE_PORT_PXPLNKSTS));
      if(((wdata >> 4)& 0x3F) == 0) {
        DEBUG((EFI_D_INFO, " Skt[%d], D[%d]:F[%d] ", i, PciAddress.Device, PciAddress.Function));
        DEBUG((EFI_D_INFO, " Link Down! \n"));
      } else {
        if((((wdata >> 4) & 0x3F) == 1) ||
           (((wdata >> 4) & 0x3F) == 2) ||
           (((wdata >> 4) & 0x3F) == 4) ||
           (((wdata >> 4) & 0x3F) == 8) ||
           (((wdata >> 4) & 0x3F) == 16)) {
          DEBUG((EFI_D_INFO, " Skt[%d], D[%d]:F[%d] ", i, PciAddress.Device, PciAddress.Function));
          DEBUG((EFI_D_INFO, "\t\tLink up as x%02d Gen%d! \n", ((wdata >> 4) & 0x3F), (wdata & 0xF)));
        } 
      }
    }
  }
}




BOOLEAN
IsIioPortsPopulated(
    IIO_GLOBALS         *IioGlobalData,
    UINT8               Iio
)
/*++

Routine Description:
  Dtermines if the PCIe slots have devices connected or not.

Arguments:
  IioGlobalData  - Pointer to UDS
  Iio     - index to CPU/IIO

Returns:
  TRUE  - if one or more PCIe slots are populated
  FALSE - if all the PCIe slots for that IIO are open / empty

--*/
{
  UINTN                   j;
  BOOLEAN                 IioPortsActive = FALSE;
  PCI_ROOT_BRIDGE_PCI_ADDRESS         PciAddress;

  PciAddress.Bus      = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];

  for(j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {
    if( IioVData_ConfigurePciePortLate(IioGlobalData, Iio, (UINT8)j) == FALSE)  //check whether the PCIe port can be configured
      continue;
    

    PciAddress.Device   = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
    PciAddress.Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;
    if(IsPciePortDLActive(IioGlobalData, PciAddress) == TRUE)    //data link layer active
      IioPortsActive = TRUE;
  }

  if(IioPortsActive == FALSE && j == NUMBER_PORTS_PER_SOCKET){
    DEBUG ((EFI_D_WARN, "  No PCIe devices connected to IIO%d!\n",Iio));
  }

  return IioPortsActive;
}


VOID
IioClockDisables (
    UINTN       Iio,
    IIO_GLOBALS *IioGlobalData,
    UINT32      *IioDisableBitMap
)
{
  UINT8                                         j;
  UINT8                                         BusBase;
  UINT32                                        RegLocation;
  UINT8                                         AssignedPortWidth = 0;
  UINT8                                         GateIioClock = 1;
  UINT32                                        DisableBitMap = 0;
  STAGGER_GLOBAL_REG_IIO_DFX_GLOBAL_STRUCT      StaggerGlobal;
  CC_SPARE2_IIO_DFX_GLOBAL_STRUCT               CcSpare2;
  PCI_ROOT_BRIDGE_PCI_ADDRESS                   PciAddress;

  if (IioGlobalData->IioVData.CpuType == CPU_IVT) {

      for (j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {

        if (IioVData_ConfigurePciePortLate(IioGlobalData,(UINT8)Iio, j) == FALSE)  
          continue;   // check whether the PCIe port can be configured

        if( !Iio && !j) 
          continue;   //skip DMI port

        if (IioGlobalData->SetupData.PciePortDisable[(Iio * NUMBER_PORTS_PER_SOCKET) + j] == 2)
          continue;   // skip the ports which are forced by user to disable

        AssignedPortWidth = GetAssignedPortIndexWidthLate(IioGlobalData, (UINT8)Iio, j);
        if ((AssignedPortWidth == 0) || (AssignedPortWidth == 0xFF))
          continue;   // if port lanes assigned to primary port, or invalid port width, than skip

        PciAddress.Bus      = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];
        PciAddress.Device   = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
        PciAddress.Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;

        if (IsPciePortDLActive(IioGlobalData, PciAddress) == FALSE) {
          DisableBitMap |= (1 << j);    //data link layer not active
        } // DLL Active
      } // j
  } // IVT

  if ((IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
    //
    // HSD 4166457: Need to define/finalize PCode/BIOS interactions for IIO_DFX_BNDL_DISBL_REG
    //              HSX changed the layout of this CR so the BIOS has a different algorithm.
    //
    // HSD 4166814: BIOS programming of cc_spare2 for Uniphy Clock disables
    //              Added code below that uses the variable "IOUnActivity" to determine whether to
    //              program the CC_SPARE2 CSR or not (HSX Only).
    //

      BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];

      //
      // HSX Bug/ECO 243740: If en_port_staggering is '1' and the stepping is earlier than B0, 
      // we cannot disable so we exit, otherwise if it is '0' then we can go and see if there    
      // are any clocks that can be disabled.
      //
      RegLocation = ONLY_REGISTER_OFFSET(STAGGER_GLOBAL_REG_IIO_DFX_GLOBAL_REG);
      StaggerGlobal.Data = PciExpressRead32(PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, RegLocation));
      if ((StaggerGlobal.Bits.en_port_staggering == 1) && (IioGlobalData->IioVData.CpuType == CPU_HSX && IioGlobalData->IioVData.CpuStepping < B0_REV)) {
        *IioDisableBitMap = 0;
        return;
      }
      //
      // preinitialize the bit map to default to all clocks disabled. 
      //    Bits[27, 26, 24] - tx_clk_dis, Bits[23:20] - ln_clk_dis, Bits[19:16] - ph_clk_dis. 
      //    If we find a link active, we will clear the corresponding clk disable bits.
      //
      DisableBitMap = (BIT27|BIT26|BIT24) | (BIT23|BIT22|BIT21|BIT20) | (BIT19|BIT18|BIT17|BIT16);

      for (j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {
        DEBUG((EFI_D_INFO, "IioClockDisables: Socket=%d, Port=%d\n", Iio, j));

        if (IioVData_ConfigurePciePortLate(IioGlobalData,(UINT8)Iio, j) == FALSE)  
          continue;    // check whether the PCIe port can be configured

        if(IioGlobalData->SetupData.PciePortDisable[(Iio * NUMBER_PORTS_PER_SOCKET) + j] == 2)
          continue;    // skip the ports which are forced by user to disable

        PciAddress.Bus      = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];
        PciAddress.Device   = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
        PciAddress.Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;

        if (IsPciePortDLActive(IioGlobalData, PciAddress) == TRUE || \
            OemIdentifyVendorSpecificPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function, FALSE) == TRUE || \
            OemIdentifyPCIeNTBPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == TRUE /* ignore NTB port for Power saving option (5002311) */
            ) {
          DEBUG((EFI_D_INFO, "IioClockDisables: Data Link Active or skipped for D%d : F%d\n", PciAddress.Device, PciAddress.Function));
          GateIioClock = 0;
          switch (PciAddress.Device) {
          case 0:                                               // IOU2, device 0: function 0
            if (IioGlobalData->IioVData.CpuType == CPU_HSX && IioGlobalData->IioVData.CpuStepping < B0_REV)
              DisableBitMap &= ~(BIT24|BIT20|BIT17|BIT16);      // enable clocks
            else
              DisableBitMap &= ~(BIT24|BIT20|BIT16);            // B0 stepping or later value
            DEBUG((EFI_D_INFO, "IioClockDisables: DisableBitMap=%x\n", DisableBitMap));
            break;

          case 1:                                               // IOU2, device 1: functions 0 & 1
            if (IioGlobalData->IioVData.CpuType == CPU_HSX && IioGlobalData->IioVData.CpuStepping < B0_REV)
              DisableBitMap &= ~(BIT24|BIT21|BIT17|BIT16);      // enable clocks
            else
              DisableBitMap &= ~(BIT24|BIT21|BIT17);            // B0 stepping or later value
            DEBUG((EFI_D_INFO, "IioClockDisables: DisableBitMap=%x\n", DisableBitMap));
            break;

          case 2:                                               // IOU0, device 2: functions 0, 1, 2, 3
            DisableBitMap &= ~(BIT26|BIT22|BIT18);
            DEBUG((EFI_D_INFO, "IioClockDisables: DisableBitMap=%x\n", DisableBitMap));
            break;

          case 3:                                               // IOU1, device 3: functions 0, 1, 2, 3
            DisableBitMap &= ~(BIT27|BIT23|BIT19);
            DEBUG((EFI_D_INFO, "IioClockDisables: DisableBitMap=%x\n", DisableBitMap));
            break;

          default:
            break;
          } // Switch Device
        } else {
          // This device is not active so we can hide the config space and the menu in the BIOS setup ..
          // .. but for Socket 0: we never hide DMI, IOU2, or IOU1
          if ((BusBase == 0) && (PciAddress.Device != 2))
            continue;
          IioGlobalData->SetupData.PEXPHIDE[ (Iio * NUMBER_PORTS_PER_SOCKET) + j] = 1;
          IioGlobalData->SetupData.HidePEXPMenu[ (Iio * NUMBER_PORTS_PER_SOCKET) + j] = 1;
        }
      } // j

      //
      // Here we can assess the DisableBitMap variable and "OR" in the lower 10 bits as needed
      // 
      if (((DisableBitMap & (BIT24|BIT20|BIT16)) == (BIT24|BIT20|BIT16))) {
        DisableBitMap |= (BIT0);
      }
      if (((DisableBitMap & (BIT24|BIT21|BIT20|BIT17|BIT16)) == (BIT24|BIT21|BIT20|BIT17|BIT16))) {
        DisableBitMap |= (BIT2|BIT1);
      }
      if (((DisableBitMap & (BIT26|BIT22|BIT18)) == (BIT26|BIT22|BIT18))) {
        DisableBitMap |= (BIT6|BIT5|BIT4|BIT3);
      }
      if (((DisableBitMap & (BIT27|BIT24|BIT23|BIT21|BIT20|BIT19|BIT17|BIT16)) == (BIT27|BIT24|BIT23|BIT21|BIT20|BIT19|BIT17|BIT16))) {
        DisableBitMap |= (BIT10|BIT9|BIT8|BIT7);
      }
      else{   //do not disable the IOU1 Tx CLK when its port 3A/B/C/D bits 7 to 10 are not set (5002399)
        DisableBitMap &= ~(BIT27|BIT23|BIT19);
      }

      //
      // HSX Bug/ECO 255642: IOU1 clock gating breaks config ring transactions not sourced from message channel
      //                      Cannot disable the physical or link layer clocks for all steppings
      //
      DisableBitMap &= ~(BIT23|BIT19);

      //
      // HSD 4166814: BIOS programming of cc_spare2 for Uniphy Clock disables only for non-legacy sockets
      //
      RegLocation = ONLY_REGISTER_OFFSET(CC_SPARE2_IIO_DFX_GLOBAL_REG);
      CcSpare2.Data = PciExpressRead32(PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, RegLocation));
      CcSpare2.Bits.cc_spare2 &= ~(BIT8 | BIT9);
      if (!IioGlobalData->SetupData.PcieHotPlugEnable) {
        if (DisableBitMap & BIT3)                                                         // no activity on IOU0
          CcSpare2.Bits.cc_spare2 |= BIT9;
        if ((DisableBitMap & (BIT0 | BIT1 | BIT7)) == (BIT0 | BIT1 | BIT7))               // no activity on IOU1 and IOU2
          CcSpare2.Bits.cc_spare2 |= BIT8;
      }
      S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusBase, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC, RegLocation), CcSpare2.Data);      

  } // HSX

  *IioDisableBitMap = DisableBitMap;
}

VOID
IioPortPowerDown(
    IIO_GLOBALS *IioGlobalData
)
{
  UINT8   i;
  UINT32  IioDisableBitMap;
  UINT32  MailBoxCommand, MailboxData;
  UINT8   BusBase, BusNumber;
  UINT32  RegLocation;
  UINT32  Data32;
  IIOMISCCTRL_N1_IIO_VTD_HSX_BDX_STRUCT         IioMiscCtrlN1;
  //
  // HSD 4166457, 4166467: Correct default values for Bits10-0 of HSX IIO_DFX_BNDL_DISBL_REG
  //              The current IVT method where the PCODE writes the IIO_DFX_BNDL_DISBL_REG to disable
  //              the ports does not work for HSX. A new HSV algorithm must be implemented in place of
  //              this code.
  //
  //
  // Force unused ports to power down as per the user defined configuration
  //    0x0 is No
  //    0x2 is Yes (Dynamic Port Disable)
  //

  if (IioGlobalData->SetupData.PowerdownUnusedPorts) {
    //
    // PowerDowns ports in all Non-Legacy CPUs
    //
    for ( i = 0 ; i < MaxIIO ; i++ ) {
      if (!IioGlobalData->IioVData.SocketPresent[i])
        continue;

      //4988278: BIOS should set IIO power saving mailbox on legacy socket

      IioDisableBitMap = 0;

      DEBUG((EFI_D_INFO, "Calling IioClockDisables: Socket=%d\n", i));    // 2 - dynamically determine if there are
      IioClockDisables(i, IioGlobalData, &IioDisableBitMap);              // .. any ports that can be disabled

      //
      // Verify that all IIO ports are empty  
      //
      if(IsIioPortsPopulated(IioGlobalData,i) == FALSE) {
        IioDisableBitMap &= ~BIT31;  // If all ports are empty then clear BIT31
      } else {
        IioDisableBitMap |= BIT31;   // If there are ports populated then set BIT 31
      }

      if (IioDisableBitMap)  {
        BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[i];
        BusNumber = IioGlobalData->IioVData.SocketUncoreBusNumber[i];
        //
        // Set IIOMISCCTRL.uniphy_en_fuse4_pwrdn = 1
        //
        RegLocation = ONLY_REGISTER_OFFSET(IIOMISCCTRL_N1_IIO_VTD_REG);
        IioMiscCtrlN1.Data = PciExpressRead32(PCI_PCIE_ADDR(BusBase, 5, 0, RegLocation));
        IioMiscCtrlN1.Bits.uniphy_en_fuse4_pwrdn = 1;
        S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusBase, 5, 0, RegLocation), IioMiscCtrlN1.Data);
      }

      //
      //4166682: HSX Clone: IIO_SQUELCH Stuck High on Unconnected/unpopulated socket
      //

      ////////////////////////////////////////////
//      Bit 31 - This bit does not exist in the actual register, but is used to communicate information
//      to pcode via the mailbox.
//      If '1, the bit directs pcode to enable PCU to monitor IIO squelch activity.
//      If '0, the bit directs pcode to disable PCU to monitor IIO squelch activity.
//      Due to the unpredictable nature of PCIe squelch the PCU
//      should only monitor squelch if a device exists(or if it might exist due to a hot plug event)
//      on any of the 44 lanes of PCIe/DMI.
//      If this bit is set to 1 and no devices are attached (for a hotplug case) the package may never
//      enter C6 due to false squelch detection.
//
//      Recommended settings:
//      1)For single socket systems, set bit 31==1 since you always have at least DMI.
//      2)For multi socket systems where a given sockets IIO always has a device attached, set bit 31==1..
//      3)For a multi socket system where no ports of IIO are used and hot plug is not supported, set bit 31==0.
//      4)For a multi socket system where hot plug is supported, set bit 31==1, regardless of what devices are or
//        are not active during boot. Note that this case the potential for never entering C6 if no PCIE devices are attached on a socket.
      if(IioDisableBitMap != 0){
        if(IioGlobalData->SetupData.PcieHotPlugEnable)
          IioDisableBitMap = IioDisableBitMap & 0x80000000;  //if HP enabled just retain bit 31 as recommendations (1) & (4)

        DEBUG((EFI_D_INFO, "IIO Port/Clocks Powering down: Socket=%d, Disable Bit Map=%x\n", i, IioDisableBitMap));
        MailBoxCommand = MAILBOX_BIOS_CMD_IIO_DISABLE;
        MailboxData = IioDisableBitMap;

        Data32 = IioGlobalData->IioOutData.CpuCsrAccess->Bios2PcodeMailBoxWrite(i, MailBoxCommand, MailboxData);
        S3_Bios2PcodeMailBoxWrite(IioGlobalData, i, MailBoxCommand, MailboxData);
        if (Data32)
          DEBUG((EFI_D_WARN, "ERROR! [IIO%d:Errcode:0x%x] unable to power down the  ports.\n", i, Data32));
      }
    } // i
  } // Setup Option
}

/*
 * API Name: IioGetCpuUplinkPort
 * Input :    None
 * Output :
 *     (1)    Pointer to PORT_DESCRIPTOR data structure which contains the PCI
 *            address (Bus, Device, function)
 *     (2)    Pointer to boolean which if TRUE indicates Uplink port is enabled,
 *            else FALSE for disabled
 *     (3)    Pointer to PORT_ATTRIB data structure which gives the uplink ports
 *            width and speed
 *  Return:   EFI_UNSUPPORTED - if system does not support the uplink port,
 *            EFI_DEVICE_ERROR - if internal error encountered,
 *            EFI_SUCCESS - for returning valid data
 *  This is the API for the EFI_IIO_SYSTEM_PROTOCOL interface which returns the details
 *  of the uplink port if connected to the platform
 */
EFI_STATUS
IioGetCpuUplinkPort (
    UINT8             Iio,
    PORT_DESCRIPTOR   *PortDescriptor,    //Bus, Device, function
    BOOLEAN           *PortStatus,        //TRUE if enabled else disabled
    PORT_ATTRIB       *PortAttrib         //width and speed
)
{
  UINT8   PortIndex;
  UINT8   Bus;
  UINT8   Dev;
  UINT8   Func;
  UINT16  data16;

  OemIioUplinkPortDetails(Iio, &PortIndex, &Bus, &Dev, &Func);

  if (PortIndex >= NUMBER_PORTS_PER_SOCKET)
    return EFI_DEVICE_ERROR;

  PortDescriptor->Bus       = Bus;
  PortDescriptor->Device    = Dev;
  PortDescriptor->Function  = Func;

  data16 = PciExpressRead16(PCI_PCIE_ADDR(PortDescriptor->Bus, PortDescriptor->Device, PortDescriptor->Function,
              0));
  if(data16 == 0xFFFF)        //port not found! could be hidden
    return EFI_DEVICE_ERROR;

  data16 = PciExpressRead16(PCI_PCIE_ADDR(PortDescriptor->Bus, PortDescriptor->Device, PortDescriptor->Function,
            R_PCIE_PORT_PXPLNKSTS));
  PortAttrib->PortWidth = (data16 >> 4) & 0x3F;   //width could be 1, 2, 4, 8, 16
  PortAttrib->PortSpeed = data16 & 0xF;           //speed : 1=Gen1, 2=Gen2, 3=Gen3

  data16 = PciExpressRead16(PCI_PCIE_ADDR(PortDescriptor->Bus, PortDescriptor->Device, PortDescriptor->Function,
            R_PCIE_PORT_PXPLNKCTRL));
  *PortStatus = (data16 & B_PCIE_PORT_PXPLNKCTRL_LNKDIS) ? FALSE : TRUE;

  return EFI_SUCCESS;
}

#ifdef  DE_SKU
VOID
IosfPortPreBootInit(
  IN  IIO_GLOBALS                                    *IioGlobalData,
  IN  UINT8											 Iio,
  IN  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress
)
{
  PCI_ROOT_BRIDGE_PCI_ADDRESS	CbdmaPciAddress;
  PCI_ROOT_BRIDGE_PCI_ADDRESS	GbEPciAddress;
  PCI_ROOT_BRIDGE_PCI_ADDRESS	CpmPciAddress;
  CPUBUSNO2_UBOX_MISC_STRUCT                    CpuBusNo2;
  CPUBUSNO03_D2F3_PCU_FUN7_STRUCT               CpuBusNo3;
  UINT8   BusNumber = IioGlobalData->IioVData.SocketUncoreBusNumber[Iio];

  DEBUG ((DEBUG_INFO, "IosfPortPreBootInit Start!\n"));
  
  //
  // Program the UBox CPUBUSNO2 for the CBDMA(DE / NS Storage Sku) /CPM (NS Comms Sku) (4985595)
  //
  if(OemIdentifyVendorSpecificPort_CBDMA_CPM(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == TRUE){

	//IOSF CBDMA port found, now get the PCI address of the CBDMA device
	if(OemRetrieve_IOSF_CBDMA_CPM_PciAddress(IioGlobalData, PciAddress, &CbdmaPciAddress) == TRUE){
	  //program the CBDMA bus# to the UBox CPUBUSNO2
		DEBUG((EFI_D_INFO, "NS PO MSG: Found the CBDMA/CPM on bus:%d\n",CbdmaPciAddress.Bus));
		CpuBusNo2.Data = IioGlobalData->IioOutData.CpuCsrAccess->ReadCpuCsr(Iio, 0, CPUBUSNO2_UBOX_MISC_REG);
		CpuBusNo2.Bits.cbdma3_3busno = CbdmaPciAddress.Bus;
		IioGlobalData->IioOutData.CpuCsrAccess->WriteCpuCsr(Iio, 0, CPUBUSNO2_UBOX_MISC_REG, CpuBusNo2.Data);			
		//save the CBDMA 3.3 base address internally
		SaveCbDmaDeviceBaseAddress(IioGlobalData, Iio, CbdmaPciAddress);
	}
	else
	  DEBUG((EFI_D_INFO, "NS PO MSG: CBDMA 3.3 or CPM not found!\n"));
  }
  //
  // Program the UBox CPUBUSNO2 for the GbE (4985595)
  //
  if(OemIdentifyVendorSpecificPort_GbE(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == TRUE){
	//IOSF GbE port found, now get the PCI address of the GbE device
	if(OemRetrieve_IOSF_GbE_PciAddress(IioGlobalData, PciAddress, &GbEPciAddress) == TRUE){
	  //program the GbE bus# to the Ubox CPUBUSNO2
	  DEBUG((EFI_D_INFO, "Found the GbE on bus:%d\n",GbEPciAddress.Bus));
	  CpuBusNo2.Data = IioGlobalData->IioOutData.CpuCsrAccess->ReadCpuCsr(Iio, 0, CPUBUSNO2_UBOX_MISC_REG);
	  CpuBusNo2.Bits.gbebusno = GbEPciAddress.Bus;
	  IioGlobalData->IioOutData.CpuCsrAccess->WriteCpuCsr(Iio, 0, CPUBUSNO2_UBOX_MISC_REG, CpuBusNo2.Data);

	}
	else
	  DEBUG((EFI_D_INFO, "GbE not found!\n"));
  }

  //
  // program the PCU CPUBUSNO03 for the CMP/GbE1 device for BDX-NS
  //
  if(OemIdentifyVendorSpecificPort_CPM_GbE1(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == TRUE){
	//IOSF GbE port found, now get the PCI address of the CPM/GbE1 device
	if(OemRetrieve_IOSF_CPM_GbE1_PciAddress(IioGlobalData, PciAddress, &CpmPciAddress) == TRUE){
	  //program the CPM/GbE1 bus# to the PCU CPUBUSNO03
	  DEBUG((EFI_D_INFO, "NS PO MSG: Found the CPM/GbE1 on bus:%d\n",CpmPciAddress.Bus));	  
 	  CpuBusNo3.Data = PciExpressRead16 ( PCI_LIB_ADDRESS (BusNumber,
                                                           PCU22_DEV_NUM,
                                                           PCU7_FUNC_NUM,
                                                           ONLY_REGISTER_OFFSET(CPUBUSNO03_D2F3_PCU_FUN7_REG)));
	  
	  CpuBusNo3.Bits.bus_number = CpmPciAddress.Bus;
	  CpuBusNo3.Bits.bus_valid |= BIT0;
	  PciExpressWrite16 ( PCI_LIB_ADDRESS (BusNumber,
                                           PCU22_DEV_NUM,
                                           PCU7_FUNC_NUM,
                                           ONLY_REGISTER_OFFSET(CPUBUSNO03_D2F3_PCU_FUN7_REG)), CpuBusNo3.Data);	  
	} 
	else
	  DEBUG((EFI_D_INFO, "NS PO MSG: CPM/GbE1 not found!\n"));
  }  
  return;
}
#endif  //DE_SKU

VOID
PortPreBootInit (
  IIO_GLOBALS                                    *IioGlobalData,
  UINT8                                          Iio,
  IIO_INIT_PHASE                                 Phase,
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress
)
{
  if(Phase == IIOInitPhase1){
    //
    // program any DMI port specific init post PCI enumeration
    //
    if(OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == TRUE){
      //TODO: (if any)
    }
    //
    // program any regular PCIe port specific init post PCI enumeration
    //
    if(OemIdentifyPCIeRootPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == TRUE){
      //TODO: (if any)
    }
    //
    // program any vendor-specific port init/config, post PCI enumeration
    //
    if(OemIdentifyVendorSpecificPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function, FALSE) == TRUE){
#ifdef  DE_SKU
      //
      // Perform any IOSF Port specific init/config, post PCI enumeration
      //
      IosfPortPreBootInit(IioGlobalData, Iio, PciAddress);
#endif  //DE_SKU
    }
  }

  if(Phase == IIOInitPhase2){
    //TODO: (if any)
  }

  if(Phase == IIOInitPhase3){
    //TODO: (if any)
  }
}

VOID
IioPortsPreBootInit(
  IN  IIO_GLOBALS       *IioGlobalData,
  IN  UINT8             Iio,
  IN  IIO_INIT_PHASE    Phase
)
{
  UINT8               pxp;
  PCI_ROOT_BRIDGE_PCI_ADDRESS   PciAddress;

  for ( pxp = 0; pxp < NUMBER_PORTS_PER_SOCKET; pxp++) {
    PciAddress.Bus      = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];
    PciAddress.Device   = IioGlobalData->PreLinkData.PcieInfo.PortInfo[pxp].Device;
    PciAddress.Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[pxp].Function;
    if( IioVData_ConfigurePciePortLate(IioGlobalData, Iio, pxp) == FALSE)  //check whether the PCIe port can be configured
      continue;
    PortPreBootInit (IioGlobalData, Iio, Phase, PciAddress);
  }
}
