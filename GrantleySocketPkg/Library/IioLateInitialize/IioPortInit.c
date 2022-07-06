/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  IioPortInit.c

Abstract:

  Implement north bridge PCI express port initialization  

--*/
#include <Token.h> // AptioV server override
#include "IioLateInitialize.h"

//
// Mcast related definitions
// 
#define MCAST_MEM_AUTO      0x0
#define MCAST_MEM_BELOW_4G  0x1
//#define MCAST_MEM_ABOVE_4G  0x2

#define MC_INDEX_POS_MASK   0x3F

#define MC_ENABLE           BIT15
#define MC_NUM_GROUP_MASK   0x3F

#define MC_RCV_ENABLE_BITS  BIT1    

#define PCIE_PORT_PXPSLOTCTRL_DEFAULT 0x7C0

typedef enum {
  MCAST_RET_SUCCESS = 0,
  MCAST_ERR_NOT_SUPPORTED,
  MCAST_ERR_MEM_ALLOC_FAIL,
  MCAST_ERR_INVALID_CONFIG,
} MCAST_RETURN_STATUS; 

VOID 
ForceUnusedPortsToL1 (
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
  IIO_GLOBALS                                       *IioGlobalData,
  UINT8                                         Valid
  )
{
  UINT16 Var16;
  UINT8  Var8;
  UINT32 Var32;
  UINT32 PortIndex = 0;
  UINT8  IioDfxDev;
  UINT8  IioDfxClusterFunc;

  if ( OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == TRUE)
    return; //Can't do the DMI link
    
  PCIEDEVFUNC2PORTINDEX(PortIndex, PciAddress.Device, PciAddress.Function);
  if (PortIndex >= NUMBER_PORTS_PER_SOCKET) return;
  
  if (PciAddress.Device == 0x3 && PciAddress.Function == 0) {
     // Check if it is in NTB mode
     Var8 = PciExpressRead8(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                              R_NTB_PPD));
     if( (Var8 & B_NTB_PORT_DEF) != 0) {
       // In NTB mode
       Var16 = PciExpressRead16(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                              R_NTB_PORT_PXPLNKSTS));
     } else {
       // In PCIE mode
       Var16 = PciExpressRead16(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                              R_PCIE_PORT_PXPLNKSTS));
     }
  } else {
       Var16 = PciExpressRead16(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                              R_PCIE_PORT_PXPLNKSTS));
  }

  IioDfxDev = IioGlobalData->PreLinkData.PcieInfo.PortInfo[PortIndex].DfxDevice;
  IioDfxClusterFunc = IioGlobalData->PreLinkData.PcieInfo.PortInfo[PortIndex].DfxClusterFunction;

  // 7.5.22 Unconnected PCIe ports dissipate L0s power levels
  if ( !Valid || ((Var16 & B_PCIE_PORT_PXPLNKSTS_DLLA) == 0)) { 

    DEBUG ((EFI_D_WARN, "  Force port bus%d,dev:%d func:%d into L1!\n",PciAddress.Bus,PciAddress.Device,PciAddress.Function));
    
    Var32 = PciExpressRead32(PCI_PCIE_ADDR(PciAddress.Bus, IioDfxDev, IioDfxClusterFunc, R_DBGCLST1));
    Var32 |= BIT30;  //enable control
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, IioDfxDev, IioDfxClusterFunc, R_DBGCLST1), Var32);

    Var32 = PciExpressRead32(PCI_PCIE_ADDR(PciAddress.Bus, IioDfxDev, IioDfxClusterFunc, R_REUTENGLTRCON));
    Var32 &= ~(BIT25 | BIT26);
    Var32 |= IioGlobalData->PreLinkData.PcieInfo.PortInfo[PortIndex].DfxReutLinkSel << 25;
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, IioDfxDev, IioDfxClusterFunc, R_REUTENGLTRCON), Var32);

    Var32 = PciExpressRead32(PCI_PCIE_ADDR(PciAddress.Bus, IioDfxDev, IioDfxClusterFunc, R_REUTPHCTR));
    Var32 |= BIT0;
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, IioDfxDev, IioDfxClusterFunc, R_REUTPHCTR), Var32);
   
  }

}


BOOLEAN
IsPciePortAvail( 
    PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
    IIO_GLOBALS                                        *IioGlobalData,
    UINT8                                          IioIndex
    ) 
{
    UINT8                       i;

    
    for (i = 0 ; i < NUMBER_PORTS_PER_SOCKET ; i++) {
      if( IioGlobalData->PreLinkData.PcieInfo.PortInfo[i].Device == PciAddress.Device && 
              IioGlobalData->PreLinkData.PcieInfo.PortInfo[i].Function == PciAddress.Function)
         return TRUE;
    }
    return FALSE;

}

BOOLEAN
IsPciePortDLActive(
    IIO_GLOBALS                    *IioGlobalData,
    PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress
)
/*++

Routine Description:
  Helper routine which physically varies the port status

Arguments:
  Param1: protocol based PCI address for the root port device

Returns:
  boolean indicating TRUE for port active or FALSE for not active

--*/
{
  UINT16  wdata;
  UINTN   RegLocation = 0;

  if (OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == TRUE)
    RegLocation = ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIEDMI_REG);
  else{
    if(OemIdentifyPCIeNTBPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == TRUE)
      RegLocation = ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIENTB_REG);
    else
      RegLocation = ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIE_REG);
  }

  wdata = PciExpressRead16(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, RegLocation));
  if( wdata & B_PCIE_PORT_PXPLNKSTS_DLLA )    //data link layer active
    return  TRUE;
  else
    return  FALSE;
}

BOOLEAN
IsPciePortTPHCapable(
    PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
    IIO_GLOBALS                                        *IioGlobalData
)
{
  UINT16  Data16;

  Data16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                              R_PCIE_PORT_PXPDEVCAP2));
  if(Data16 & BIT12)      //check the TPH completer capability
    return TRUE;
  else
    return FALSE;
}

/**
  Initialize MultiCast as per validation requirement.
  (It is specific DualCast configuration using NTB as documented in the IIO spec.
  Regular system-wide Multicast configuration/policy is expected to be configured by OS/Device drivers.)

  @param[in]  PciAddress  Port to be configured
  @param[in]  IioGlobalData      Platform configuration info

  @retval     MCAST_RET_SUCCESS Configured without any error
  @retval     Non-zero values   Other specific failures 
**/
UINT8
EnableMultiCast(
    PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
    IIO_GLOBALS                                        *IioGlobalData
    )
{
  EFI_STATUS              Status;
  UINTN                   i;
  UINT32                  Data32;
  UINT16                  Data16;
  EFI_PHYSICAL_ADDRESS    McastBaseAddress;
  EFI_ALLOCATE_TYPE       McastMemRangeType;
  UINTN                   McastRsvdMemSize;
  UINT8                   McastNumGrp;
  UINT8                   McastIndxPos;
   
  if ((IioGlobalData->IioVData.CpuType != CPU_HSX) && (IioGlobalData->IioVData.CpuType != CPU_BDX))
    return MCAST_ERR_NOT_SUPPORTED;

  //
  // Read in configured number of groups and index position
  //
  McastNumGrp   = IioGlobalData->SetupData.McastNumGroup;
  McastIndxPos  = IioGlobalData->SetupData.McastIndexPosition;
    
  //
  // Allocate memory for Mcast test if not already allocated
  //  
  if (IioGlobalData->IioOutData.McastRsvdMemory != 0) {
    //
    // Already allocated, get the address
    // 
    McastBaseAddress = IioGlobalData->IioOutData.McastRsvdMemory;
    
  } else {
    //
    // Allocate memory for Mcast as per user config 
    // 
    if (IioGlobalData->SetupData.McastBaseAddrRegion == MCAST_MEM_AUTO) {
      //
      // Any location
      // 
      McastMemRangeType = AllocateAnyPages;
      McastBaseAddress  = 0;
      
    } else if (IioGlobalData->SetupData.McastBaseAddrRegion == MCAST_MEM_BELOW_4G) {
      //
      // Force below 4G
      // 
      McastMemRangeType = AllocateMaxAddress;
      McastBaseAddress  = 0xFFFFFFFF;          // Below 4G
                                                
    } else {
      
      return MCAST_ERR_MEM_ALLOC_FAIL;    
    }
    
    //
    // Calculate size required as per number of groups (McastNumGroup) and size as per (McastIndexPosition)
    //   
    McastRsvdMemSize = McastNumGrp * (UINTN)( 1 << McastIndxPos );
    
    //
    // Now allocate memory 
    //
    Status = gBS->AllocatePages (
                     McastMemRangeType,
                     EfiBootServicesData,
                     EFI_SIZE_TO_PAGES( (UINTN)McastRsvdMemSize ),
                     &McastBaseAddress 
                     );
    
    if (McastBaseAddress == 0) {
      DEBUG((EFI_D_ERROR, "\nMCAST: Memory Alloc Error\n"));
      return MCAST_ERR_MEM_ALLOC_FAIL;
      
    } else {
      IioGlobalData->IioOutData.McastRsvdMemory = McastBaseAddress;
      DEBUG((EFI_D_INFO, "\nMCAST: Allocated Membase at 0x%x, Size 0x%x\n", IioGlobalData->IioOutData.McastRsvdMemory, McastRsvdMemSize));
      
    }

  } 

  //  
  // Set MC Base Address and MC Index Position
  //

  Data32 = MC_INDEX_POS_MASK & McastIndxPos;    // Index pos bits
  
  Data32 = Data32 | ((UINT32)McastBaseAddress >> 12) << 12;  // 31:12 bits of Base address
  
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                        ONLY_REGISTER_OFFSET(MCAST_BASE_N0_IIO_PCIE_REG) ), Data32);
 
  Data32 = (UINT32) (McastBaseAddress >> 32);               // Rest of the 63:32 address bits

  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                        ONLY_REGISTER_OFFSET(MCAST_BASE_N1_IIO_PCIE_REG) ), Data32);
  
  //
  // Clear MCAST block all
  // 
  Data32 = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, 
                                        ONLY_REGISTER_OFFSET(MCAST_BLK_ALL_N0_IIO_PCIE_REG) ) ); 
  
  Data32 &= ~(0xFFFF);
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                        ONLY_REGISTER_OFFSET(MCAST_BLK_ALL_N0_IIO_PCIE_REG) ), Data32);

  //
  //  MCAST Receive should be set for NTB 
  // 
  if( (PciAddress.Device == IIO_NTB_DEVICE_NUM) && (PciAddress.Function == IIO_NTB_FUNC_NUM)  ) {

    Data32 = 0;
    
    for( i = 0 ; i < McastNumGrp; i++ ) {
      Data32 |= 1 << i;
    }
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                     ONLY_REGISTER_OFFSET(MCAST_RCV_N0_IIO_PCIE_REG) ), Data32);  
  }
  
  //
  // Set MC Number of groups MC_NUM_GROUP and MC_ENABLE
  //
  Data16 = MC_NUM_GROUP_MASK & ( (McastNumGrp == 0) ? (McastNumGrp) : (McastNumGrp - 1) );
  Data16 |= MC_ENABLE;
  S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                        ONLY_REGISTER_OFFSET(MCAST_CTRL_IIO_PCIE_REG) ), Data16);

  //#if 1 
  //
  // Dump some of the mcast registers
  //
  DEBUG((EFI_D_INFO, "MCAST B:%x D:%x F:%x\n", PciAddress.Bus, PciAddress.Device, PciAddress.Function )); 
  for ( i = 0; i < 10; i++ ) {
    Data32 = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, 
                                        ONLY_REGISTER_OFFSET(MCAST_CAP_HDR_IIO_PCIE_REG) + i*4 ) ); 
    DEBUG((EFI_D_INFO, "MCAST Reg %x: 0x%x\n", i*4, Data32 ));
    
  }
  //#endif
    
  return    MCAST_RET_SUCCESS;
}

void
InitPortClock (
    IIO_GLOBALS                                   *IioGlobalData,
    PCI_ROOT_BRIDGE_PCI_ADDRESS                   PciAddress,
    UINT8                                         PcieSlotClockSource,
    UINT8                                         PciePortClockConfiguration
)
{
  UINT32         ClkCtrlReg, ClkStsReg;
  UINT16         Var16;

  if (OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == FALSE)
    ClkCtrlReg = R_PCIE_PORT_PXPLNKCTRL;
  else
    ClkCtrlReg = R_DMI_PORT_PXPLNKCTRL;
  //
  // Program PCIe port clock source
  //
  if(PcieSlotClockSource != 0xFF) {   //skip programming the slot clock source configuration if FFh
    if (OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == FALSE)
      ClkStsReg = R_PCIE_PORT_PXPLNKSTS;
    else
      ClkStsReg = R_DMI_PORT_PXPLNKSTS;

    Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
              ClkStsReg));
    Var16 &= ~(B_PCIE_PORT_PXPLNKSTS_SCCON);
    Var16 |= (PcieSlotClockSource) ? B_PCIE_PORT_PXPLNKSTS_SCCON : 0;
    S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
        ClkStsReg), Var16);
  }

  //
  // Program PCIe port clocking
  //
  if (OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == FALSE)
    ClkCtrlReg = R_PCIE_PORT_PXPLNKCTRL;
  else
    ClkCtrlReg = R_DMI_PORT_PXPLNKCTRL;

  Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
            ClkCtrlReg));
  Var16 &= ~(B_PCIE_PORT_PXPLNKCTRL_CCCON + B_PCIE_PORT_PXPLNKCTRL_LNKDIS);
  Var16 |= (PciePortClockConfiguration) ? B_PCIE_PORT_PXPLNKCTRL_CCCON : 0;
  S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      ClkCtrlReg), Var16);
  IioStall (IioGlobalData, GEN3_STALL_SBR); //2ms delay = SBR flip interval
}

void
EnableSlotPower (
  IIO_GLOBALS                                   *IioGlobalData,
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress
  )
{
  SLTCON_IIO_PCIE_STRUCT      SlotControl;
  //
  // Enable Power to Slot: PWRCTRL on SLOTCTRL
  //
  // HSD 4166825: HSX: Polarity for PCIe Slot Power is Wrong When HotPlug is Enabled
  //
  DEBUG ((EFI_D_INFO, "Enabling PCIE Dev:%d Func:%d Slot Power...\n", PciAddress.Device, PciAddress.Function));
  SlotControl.Data = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, ONLY_REGISTER_OFFSET(SLTCON_IIO_PCIE_REG)));
  SlotControl.Bits.power_controller_control = 0;
  S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                    ONLY_REGISTER_OFFSET(SLTCON_IIO_PCIE_REG)), SlotControl.Data);
  IioStall (IioGlobalData, LINK_INIT_POLL_TIMEOUT); //10ms delay
}


void
SlotPowerControl ( 
  IIO_GLOBALS                                   *IioGlobalData,
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress
  )
{
  UINT16                        LoopCount;
  SLTCON_IIO_PCIE_STRUCT        SlotControl;
  SLTSTS_IIO_PCIE_STRUCT        SlotStatus;

  //
  // HSD 4166825: HSX: Polarity for PCIe Slot Power is Wrong When HotPlug is Enabled
  //
  // Check PCIE slot status
  //
  LoopCount = 0;
  SlotStatus.Data = 0;
  while (LoopCount < 50) {
    SlotStatus.Data = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, ONLY_REGISTER_OFFSET(SLTSTS_IIO_PCIE_REG)));
    if (SlotStatus.Bits.presence_detect_changed)
      break;
    IioStall (IioGlobalData, SLOT_POWER_DELAY); //1ms delay
    LoopCount++;
  }
  
  if (SlotStatus.Bits.presence_detect_state == 1) {
    if (SlotStatus.Bits.mrl_sensor_state == 0) {
      //
      // Enable Power to Slot: PWRCTRL on SLOTCTRL, turn PWD indicator ON
      //
      DEBUG ((EFI_D_INFO, "Enabling PCIE Dev:%d Func:%d Slot Power...\n", PciAddress.Device, PciAddress.Function));
      SlotControl.Data = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, ONLY_REGISTER_OFFSET(SLTCON_IIO_PCIE_REG)));
      SlotControl.Bits.power_controller_control = 0;          // Power ON
      SlotControl.Bits.power_indicator_control = 1;           // Indicator ON (2 bit field)
      S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                        ONLY_REGISTER_OFFSET(SLTCON_IIO_PCIE_REG)), SlotControl.Data);
    } else {
      //
      // Card Present and MRL Open - Error Condition
      // Just Blink Attn Indicator without powering card
      //
      DEBUG ((EFI_D_INFO, "Disabling PCIE Dev:%d Func:%d Slot Power (MRL Open)...\n", PciAddress.Device, PciAddress.Function));
      SlotControl.Data = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, ONLY_REGISTER_OFFSET(SLTCON_IIO_PCIE_REG)));
      SlotControl.Bits.power_controller_control = 1;          // Power OFF
      SlotControl.Bits.power_indicator_control = 3;           // Indicator OFF (2 bit field)
      SlotControl.Bits.attention_indicator_control = 2;       // Attention Indicator BLINK (2 bit field)
      S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                        ONLY_REGISTER_OFFSET(SLTCON_IIO_PCIE_REG)), SlotControl.Data);
    }
  } else {
    //
    // Empty slot    
    // Remove Power to Slot: PWRCTRL on SLOTCTRL
    //
    DEBUG ((EFI_D_INFO, "Disabling PCIE Dev:%d Func:%d Slot Power (No Device)...\n", PciAddress.Device, PciAddress.Function));
    SlotControl.Data = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, ONLY_REGISTER_OFFSET(SLTCON_IIO_PCIE_REG)));
    SlotControl.Bits.power_controller_control = 1;          // Power OFF
    SlotControl.Bits.power_indicator_control = 3;           // Indicator OFF (2 bit field)
    if (SlotStatus.Bits.mrl_sensor_state == 0)
      SlotControl.Bits.attention_indicator_control = 3;       // Attention Indicator OFF (2 bit field)
    S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                      ONLY_REGISTER_OFFSET(SLTCON_IIO_PCIE_REG)), SlotControl.Data);
  }
  
  //
  // wait on slot control command to complete (max. 100ms)
  //
  LoopCount = 0;
  while (LoopCount < 100) {
    SlotStatus.Data = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, ONLY_REGISTER_OFFSET(SLTSTS_IIO_PCIE_REG)));
    if (SlotStatus.Bits.command_completed)
      break;
    IioStall (IioGlobalData, 1000); //1ms delay
    LoopCount++;
  }
  
  //
  // Clear all the pending status  
  //
  SlotStatus.Data = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, ONLY_REGISTER_OFFSET(SLTSTS_IIO_PCIE_REG)));
  SlotStatus.Bits.command_completed = 1;
  SlotStatus.Bits.presence_detect_changed = 1;
  SlotStatus.Bits.mrl_sensor_changed = 1;
  SlotStatus.Bits.power_fault_detected = 1;
  SlotStatus.Bits.attention_button_pressed = 1;
  S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                    ONLY_REGISTER_OFFSET(SLTSTS_IIO_PCIE_REG)), SlotStatus.Data);

  //
  // Enable all Hot Plug Interrupts regardless of card presence
  // except Hot Plug Interrupt HPINTEN bit-5 reg 84
  //
  SlotControl.Data = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, ONLY_REGISTER_OFFSET(SLTCON_IIO_PCIE_REG)));
  SlotControl.Bits.hot_plug_interrupt_enable = 0;
  SlotControl.Bits.command_completed_interrupt_enable = 0;
  SlotControl.Bits.presence_detect_changed_enable = 1;
  SlotControl.Bits.mrl_sensor_changed_enable = 1;
  SlotControl.Bits.power_fault_detected_enable = 1;
  SlotControl.Bits.attention_button_pressed_enable = 1;

  S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                    ONLY_REGISTER_OFFSET(SLTCON_IIO_PCIE_REG)), SlotControl.Data);
}

/*++
=========================================================================================
=========================================================================================
--*/

VOID
PcieDeviceInit ( 
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress, 
  IIO_GLOBALS   *IioGlobalData
)
{
  UINT8   *Array;
  UINT16  DevCtrl = 0;
  UINT16  Var16;
  UINT32  DevCtrl32;
  UINT32  Var32;
  UINT8   i;
  UINT32  PortIndex;
  UINT32  RegLocation;
  DEVCTRL2_IIO_PCIE_HSX_BDX_STRUCT    DevCtrl2;
  INTPIN_IIO_PCIE_STRUCT          IntPinReg;
  PortIndex = 0;
  // Find which IIO we are configuring (as shown in setup) 
  for ( i = 0 ; i < MaxIIO ; i++ ){
      if ((PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[i]) && IioGlobalData->IioVData.SocketPresent[i]) break;
  }

  if (i >= MaxIIO) return;
  
  PCIEDEVFUNC2PORTINDEX(PortIndex, PciAddress.Device, PciAddress.Function);  
  if (((i*NUMBER_PORTS_PER_SOCKET)+PortIndex) >= MAX_TOTAL_IIO_PORTS) return;

  if (OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == FALSE) {
    //
    // Set Max payload size
    //
    Array = &(IioGlobalData->SetupData.PcieMaxPayload[0]);
    //
    // Check custome maxpayload has been selecteced
    //

    Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                                    R_PCIE_PORT_PXPDEVCTRL));
    Var16 &= ~(B_PCIE_PORT_PXPDEVCTRL_MPS);
    Var16 |= DevCtrl;


    //s3427199: register name below was incorrect - start
    S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                            R_PCIE_PORT_PXPDEVCTRL), Var16);
    //s3427199 - end

    //
    // Set D0 or D3Hot state based on setup option
    //
    Array = &(IioGlobalData->SetupData.PcieDState[0]);
    if (Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex] == 1) {
      DevCtrl32 = V_PCIE_PORT_PXPPMCSR_PWR_STATE_D3HOT;
    } else {
      DevCtrl32 = V_PCIE_PORT_PXPPMCSR_PWR_STATE_D0;
    }

    Var32 = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                                R_PCIE_PORT_PXPPMCSR));
    Var32 &= ~(B_PCIE_PORT_PXPPMCSR_PWR_STATE);
    Var32 |= DevCtrl32;
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                            R_PCIE_PORT_PXPPMCSR), Var32);  
  }
  //
  // Set Completion Time Out and value
  // HSD 4165823: PCIE: Global Completion Timeout Options needed
  //
  if (OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == TRUE)    //DMI port
    RegLocation = ONLY_REGISTER_OFFSET(DEVCTRL2_IIO_PCIEDMI_REG);
  else
    RegLocation = ONLY_REGISTER_OFFSET(DEVCTRL2_IIO_PCIE_REG);

  DevCtrl2.Data = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, RegLocation));

  if (IioGlobalData->SetupData.CompletionTimeoutGlobal) {
    DevCtrl2.Bits.compltodis = 0;                                       // enable the completion TO counter
    DevCtrl2.Bits.compltoval = IioGlobalData->SetupData.CompletionTimeoutGlobalValue;
  } else {
    Array = &(IioGlobalData->SetupData.CompletionTimeout[0]);
    if (Array[i]) {
      DevCtrl2.Bits.compltodis = 0;                                       // enable the completion TO counter
      Array = &(IioGlobalData->SetupData.CompletionTimeoutValue[0]);
      DevCtrl2.Bits.compltoval = Array[i];
    } else {
      DevCtrl2.Bits.compltodis = 1;                                       // disable the completion TO counter
      DevCtrl2.Bits.compltoval = 0;
    }
  }

  S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, RegLocation), DevCtrl2.Data);
  
  //
  // Read and Write back INTPIN register to lock RW-O INTPIN
  //
  IntPinReg.Data = PciExpressRead8 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, ONLY_REGISTER_OFFSET(INTPIN_IIO_PCIE_REG)));
  S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, ONLY_REGISTER_OFFSET(INTPIN_IIO_PCIE_REG)),IntPinReg.Data);
}

/*++
=========================================================================================
=========================================================================================
--*/
/**
  Program Link related setting for vendor specific Pcie ports
**/
VOID
VendorSpecificPcieLinkInit (  
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
  IIO_GLOBALS   *IioGlobalData
  )
{
#ifdef DE_SKU
  UINT16         Aspm;
  UINT8          L0sLatency;
  UINT8          L1Latency;
  UINT16         Var16;
  UINT32         LNKCAPSet;
  UINT8          *Array;
  UINT32         CAPGen;
  UINT8          i;
  UINT32         PortIndex;

  DEBUG ((EFI_D_INFO, "Vendor specific pcie Link Init port:%d Func%d...\n",PciAddress.Device,PciAddress.Function));
  // Find which IIO we are configuring (as shown in setup) 
  for ( i = 0 ; i < MaxIIO ; i++ ){
      if ((PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[i]) && IioGlobalData->IioVData.SocketPresent[i]) break;
  }
 
  if (i >= MaxIIO) return;
  
  PortIndex = 0;
  PCIEDEVFUNC2PORTINDEX(PortIndex, PciAddress.Device, PciAddress.Function);
  if (((i*NUMBER_PORTS_PER_SOCKET)+PortIndex) >= MAX_TOTAL_IIO_PORTS) return;
  
  //
  // Program PCI Express Link Capabilities - Port# (31:24), 
  //                                         L1 exit Latecy (17:15), 
  //                                         LOs exit Latecy (14:12), 
  //                                         ASPM(11:10) 
  //  

  LNKCAPSet = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,
                                              PciAddress.Function, R_PCIE_PORT_PXPLNKCAP));
  //
  // Port# (31:24)
  //
  LNKCAPSet= LNKCAPSet & ~(B_PCIE_PORT_PXPLNKCAP_PORTNUM);                       // clear bits 31:24
  CAPGen = (PortIndex << 24);                                                   // put port number in bits 31:24
  LNKCAPSet |= CAPGen;

  //
  // L1 exit Latecy (17:15)
  //
  Array= &(IioGlobalData->SetupData.PcieL1Latency[0]);
  L1Latency =   Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex];
  
  CAPGen = (L1Latency << V_PCIE_PORT_PXPLNKCAP_L1EXITL_SHIFT) & B_PCIE_PORT_PXPLNKCAP_L1EXITL;     // Set programmed L1 Latency Value
  LNKCAPSet = LNKCAPSet & ~(B_PCIE_PORT_PXPLNKCAP_L1EXITL);                      // clear bits 17:15
  LNKCAPSet |= CAPGen;                                                           // program L1 Exit Latency

  if ((IioGlobalData->IioVData.CpuType == CPU_HSX)  || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
    //
    // L0s exit Latecy (14:12)
    //
    Array= &(IioGlobalData->SetupData.PcieL0sLatency[0]);
    L0sLatency =   Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex];
  
    CAPGen = (L0sLatency << V_PCIE_PORT_PXPLNKCAP_L0SEXITL_SHIFT) & B_PCIE_PORT_PXPLNKCAP_L0SEXITL;   // Set programmed L0s Latency Value
    LNKCAPSet = LNKCAPSet & ~(B_PCIE_PORT_PXPLNKCAP_L0SEXITL);                     // clear bits 14:12
    LNKCAPSet |= CAPGen;                                                           // program L0s Exit Latency
  }
 
  //
  // ASPM(11:10): 00=disabled, 10=L1 only, 01 = L0s entry support, 11=Both L1 & L0s support
  //
  LNKCAPSet = LNKCAPSet & ~(B_PCIE_PORT_PXPLNKCAP_ASPMCTRL);                     // clear ASPM bits 11:10
  //
  // as per the 4030522/4030608, overriding the WA for the below ECO (to keep
  // the ASPM disabled for A0/1), since L1 is functional.
  // ivt_eco: s4570709: PUSH from jaketown: Disable PCIe L0s with unCore patch
  //  [cloned to s4030134] - (WA to keep the ASPM disabled for A0 - is abondoned)
  //
  //
  // HSD 4165822:PCIE: Global PCIE ASPM option needed 
  //             Code added to check a global setup option first to see if the SV team just wants all the ports
  //             configured for L1.  If this option is set, then all ports get the L1 setting, but if the option is
  //             DISABLED, then the individual port setting uses the older setup option.
  //
  
  // 4168647: BIOS to enable ASPM L1 on DMI and PCIe ports by default

//AptioV Server override - Start: Set ASPM capabilities based on setup option
//LNKCAPSet |= B_PCIE_PORT_PXPLNKCAP_ASPMCTRL_L1; // Set ASPM L1 cap by default.

  if (IioGlobalData->SetupData.PcieGlobalAspm) {
     Aspm = IioGlobalData->SetupData.PcieGlobalAspm;
   } else {
     Aspm = IioGlobalData->SetupData.PcieAspm[(i * NUMBER_PORTS_PER_SOCKET) + PortIndex];
     //If Aspm setup option is set to Auto, program L1 capabilities.
     if ( Aspm == PCIE_ASPM_AUTO ) {
         Aspm = BIT1;
     }
   }
  LNKCAPSet |= (Aspm << 10);
//AptioV Server override - End: Set ASPM capabilities based on setup option


  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,PciAddress.Function, 
                                   R_PCIE_PORT_PXPLNKCAP), LNKCAPSet);
  //
  // Program PCI Express Link Control
  //
  Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                          R_PCIE_PORT_PXPLNKCTRL));
  
  Var16 &= ~(BIT1+BIT0);
  // 4168647: BIOS to enable ASPM L1 on DMI and PCIe ports by default
  if (IioGlobalData->SetupData.PcieGlobalAspm) {
    Aspm = IioGlobalData->SetupData.PcieGlobalAspm;
  } else {
    Aspm = CheckVarPortASPMLate(IioGlobalData, i, (UINT8)PortIndex);
  }
  Var16 |= Aspm;

  S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                   R_PCIE_PORT_PXPLNKCTRL), Var16);
#endif // DE_SKU                         
}

/*++
=========================================================================================
=========================================================================================
--*/

VOID
PcieLinkInit (  
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
  IIO_GLOBALS   *IioGlobalData
  )
{
  PCIE_LNK_STS   PcieLinkStatus;
  BOOLEAN        PcieLinkTrained; 
  UINT16         Aspm;
  UINT8          L0sLatency;
  UINT8          L1Latency;
  UINT16         DataSet;
  UINT16         Sccon;
  UINT16         Var16;
  UINT32         LNKCAPSet;
  UINT32         LNKCAP2Set;
  UINT32         CAPGen;
  UINT32         LinkCtrl;
  UINTN          Loop;
  BOOLEAN        DisableLink;
  UINT8          *Array;
  UINT8          *Array2;
  UINT8          *ArrayPortDis;
  UINT8          i;
  UINT8          RetrainFlg;
  UINT32         PortIndex;

  // skip DMI port
  if ( OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == TRUE) {
	 return;
  }
  
  // Skip IOSF port 
  if(OemIdentifyVendorSpecificPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function, FALSE) == TRUE) {
    VendorSpecificPcieLinkInit (PciAddress, IioGlobalData);
    return;
  }

  // Find which IIO we are configuring (as shown in setup) 
  for ( i = 0 ; i < MaxIIO ; i++ ){
      if ((PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[i]) && IioGlobalData->IioVData.SocketPresent[i]) break;
  }
 
  if (i >= MaxIIO) return;
  
  PortIndex = 0;
  PCIEDEVFUNC2PORTINDEX(PortIndex, PciAddress.Device, PciAddress.Function);
  if (((i*NUMBER_PORTS_PER_SOCKET)+PortIndex) >= MAX_TOTAL_IIO_PORTS) return;
  
  //
  // Program PCI Express Link Capabilities - Port# (31:24), 
  //                                         L1 exit Latecy (17:15), 
  //                                         LOs exit Latecy (14:12), 
  //                                         ASPM(11:10) 
  //                                         MaxLnkWidth (9:4), 
  //                                         Link Speed Supported (3:0)
  //  

  LNKCAPSet = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,
                                              PciAddress.Function, R_PCIE_PORT_PXPLNKCAP));
  //
  // Port# (31:24)
  //
  LNKCAPSet= LNKCAPSet & ~(B_PCIE_PORT_PXPLNKCAP_PORTNUM);                       // clear bits 31:24
  CAPGen = (PortIndex << 24);                                                   // put port number in bits 31:24
  LNKCAPSet |= CAPGen;

  //
  // L1 exit Latecy (17:15)
  //
  Array= &(IioGlobalData->SetupData.PcieL1Latency[0]);
  L1Latency =   Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex];
  
  CAPGen = (L1Latency << V_PCIE_PORT_PXPLNKCAP_L1EXITL_SHIFT) & B_PCIE_PORT_PXPLNKCAP_L1EXITL;     // Set programmed L1 Latency Value
  LNKCAPSet = LNKCAPSet & ~(B_PCIE_PORT_PXPLNKCAP_L1EXITL);                      // clear bits 17:15
  LNKCAPSet |= CAPGen;                                                           // program L1 Exit Latency

  if ((IioGlobalData->IioVData.CpuType == CPU_HSX)  || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
    //
    // L0s exit Latecy (14:12)
    //
    Array= &(IioGlobalData->SetupData.PcieL0sLatency[0]);
    L0sLatency =   Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex];
  
    CAPGen = (L0sLatency << V_PCIE_PORT_PXPLNKCAP_L0SEXITL_SHIFT) & B_PCIE_PORT_PXPLNKCAP_L0SEXITL;   // Set programmed L0s Latency Value
    LNKCAPSet = LNKCAPSet & ~(B_PCIE_PORT_PXPLNKCAP_L0SEXITL);                     // clear bits 14:12
    LNKCAPSet |= CAPGen;                                                           // program L0s Exit Latency
  }
 
  //
  // ASPM(11:10): 00=disabled, 10=L1 only, 01 = L0s entry support, 11=Both L1 & L0s support
  //
  LNKCAPSet = LNKCAPSet & ~(B_PCIE_PORT_PXPLNKCAP_ASPMCTRL);                     // clear ASPM bits 11:10
  //
  // as per the 4030522/4030608, overriding the WA for the below ECO (to keep
  // the ASPM disabled for A0/1), since L1 is functional.
  // ivt_eco: s4570709: PUSH from jaketown: Disable PCIe L0s with unCore patch
  //  [cloned to s4030134] - (WA to keep the ASPM disabled for A0 - is abondoned)
  //
  //
  // HSD 4165822:PCIE: Global PCIE ASPM option needed 
  //             Code added to check a global setup option first to see if the SV team just wants all the ports
  //             configured for L1.  If this option is set, then all ports get the L1 setting, but if the option is
  //             DISABLED, then the individual port setting uses the older setup option.
  //
  // 4168647: BIOS to enable ASPM L1 on DMI and PCIe ports by default

//AptioV Server override - Start: Set ASPM capabilities based on setup option
//LNKCAPSet |= B_PCIE_PORT_PXPLNKCAP_ASPMCTRL_L1; // Set ASPM L1 cap by default.

  if (IioGlobalData->SetupData.PcieGlobalAspm) {
     Aspm = IioGlobalData->SetupData.PcieGlobalAspm;
   } else {
     Aspm = IioGlobalData->SetupData.PcieAspm[(i * NUMBER_PORTS_PER_SOCKET) + PortIndex];
     //If Aspm setup option is set to Auto, program L1 capabilities.
     if ( Aspm == PCIE_ASPM_AUTO ) {
         Aspm = BIT1;
     }
   }
  LNKCAPSet |= (Aspm << 10);
//AptioV Server override - End: Set ASPM capabilities based on setup option

  //
  // MaxLnkWidth (9:4)
  //
  LNKCAPSet = LNKCAPSet & ~(B_PCIE_PORT_PXPLNKCAP_MAXLNKWH);                     // clear Max Link width 9:4
  
  Array = &(IioGlobalData->IioOutData.CurrentPXPMap[0]); 
  Array2 = &(IioGlobalData->IioOutData.MaxPXPMap[0]); 
  
  // If Override Max Link Width was not specified in MaxPXPMap, then copy CurrentPXPMap (bifurcated max width) to MaxPXPMap (override max width)
  if(Array2[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex] == 0) {
    Array2[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex] = Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex];
  }
  CAPGen = Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex];       // get max port link width
  LNKCAPSet |=  (CAPGen << 4);

  //
  // HSD 4166781: HSX,  PCIE - Mismatch in advertising link capability between lnkcap and lnkcap2 
  //                    can cause legacy hardware(prior to 3.0 base PCIE spec) to train only to Gen1.
  //
  if(IioGlobalData->SetupData.PciePortLinkSpeed[i*NUMBER_PORTS_PER_SOCKET + PortIndex] != 0) {
    LNKCAPSet &= ~B_PCIE_PORT_PXPLNKCAP_LNKSPEED;

    switch (IioGlobalData->SetupData.PciePortLinkSpeed[i*NUMBER_PORTS_PER_SOCKET + PortIndex]) {
    case 0:                                  // if AUTO we leave HW defaults except for port index 0
      //4167004: ESCALATE from hexaii_hsx_mock:PCIE: Link capabilities set wrong by BIOS
      LNKCAPSet |= V_PCIE_PORT_PXPLNKCAP_LS_GEN3;
      break;
    case 1:
      LNKCAPSet |= V_PCIE_PORT_PXPLNKCAP_LS_GEN1;
      break;
    case 2:
      LNKCAPSet |= V_PCIE_PORT_PXPLNKCAP_LS_GEN2;
      break;
    case 3:
    default:
      LNKCAPSet |= V_PCIE_PORT_PXPLNKCAP_LS_GEN3;
      break;
    }
  } else {   //in case of "PciePortLinkSpeed" == auto
    //
    // ivt_eco s4677507: PCIE - Mismatch in advertising link capability between
    //                   lnkcap and lnkcap2 can cause legacy hardware(prior to 3.0
    //                   base PCIE spec) to train only to Gen1.
    //  [cloned to s4031727] - only for DMI/PCIe port 0 , leave other ports to default Gen3
    if((IioGlobalData->IioVData.CpuType == CPU_HSX || IioGlobalData->IioVData.CpuType == CPU_BDX ) && PortIndex == 0) {
      LNKCAPSet &= ~(B_PCIE_PORT_PXPLNKCAP_LNKSPEED);
      LNKCAPSet |= V_PCIE_PORT_PXPLNKCAP_LS_GEN2;
    }
  }
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,PciAddress.Function, 
                                   R_PCIE_PORT_PXPLNKCAP), LNKCAPSet);

  LNKCAP2Set = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,
                                              PciAddress.Function, R_PCIE_PORT_PXPLINKCAP2));
  
  LNKCAP2Set &= ~B_PCIE_PORT_PXPLNKCAP2_LNKSPDVEC;
  switch (IioGlobalData->SetupData.PciePortLinkSpeed[i*NUMBER_PORTS_PER_SOCKET + PortIndex]) {
  case 0:
    LNKCAP2Set |= V_PCIE_PORT_PXPLNKCAP2_LNKSPDVEC_3G_TOP;
    break;
  case 1:
    LNKCAP2Set |= V_PCIE_PORT_PXPLNKCAP2_LNKSPDVEC_1G_TOP;
    break;
  case 2:
    LNKCAP2Set |= V_PCIE_PORT_PXPLNKCAP2_LNKSPDVEC_2G_TOP;
    break;
  case 3:
  default:
    LNKCAP2Set |= V_PCIE_PORT_PXPLNKCAP2_LNKSPDVEC_3G_TOP;
    break;
  }
  
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,PciAddress.Function, 
                                   R_PCIE_PORT_PXPLINKCAP2), LNKCAP2Set);

  //
  // Program Link Control 2  
  //
  RetrainFlg = 0;
  PcieLinkStatus.Word = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                                R_PCIE_PORT_PXPLNKSTS));
  DataSet = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,
                                              PciAddress.Function, R_PCIE_PORT_PXPLNKCTRL2));

  DataSet &= ~(B_PCIE_PORT_PXPLNKCTRL2_DEEMPH | B_PCIE_PORT_PXPLNKCTRL2_ENCmpl);
  if(IioGlobalData->SetupData.PciePortLinkSpeed[i*NUMBER_PORTS_PER_SOCKET + PortIndex] != PCIE_LINK_SPEED_AUTO) {
    //
    // If link speed not set to Auto, set link speed to user-specified speed
    //
    DataSet &= ~(B_PCIE_PORT_PXPLNKCAP_LNKSPEED);
    // Program Link Speed Supported (3:0)
    switch(IioGlobalData->SetupData.PciePortLinkSpeed[i*NUMBER_PORTS_PER_SOCKET + PortIndex])
    {
    case 1:
      DEBUG ((EFI_D_INFO, "  Program Gen1!\n"));
      DataSet |= V_PCIE_PORT_PXPLNKCTRL2_LS_GEN1;
      RetrainFlg = 1;
      break;

    case 2:
      DEBUG ((EFI_D_INFO, "  Program Gen2!\n"));
      DataSet |= V_PCIE_PORT_PXPLNKCTRL2_LS_GEN2;
      RetrainFlg = 1;
      break;

    case 3:
    default:
      DEBUG ((EFI_D_INFO, "  Program Gen3!\n"));
      DataSet |= V_PCIE_PORT_PXPLNKCTRL2_LS_GEN3;
      RetrainFlg = 1;
      break;
    }
    if(PcieLinkStatus.Bits.LinkSpeed == IioGlobalData->SetupData.PciePortLinkSpeed[i*NUMBER_PORTS_PER_SOCKET + PortIndex]) {
      // Do not retrain if the current linkspeed already matches the desired linkspeed
      DEBUG((EFI_D_INFO, "  (No need to retrain)\n"));
      RetrainFlg = 0;
    }
  } else {      //if desired speed is set to auto
    //
    // overwrite special port specific settings in case of auto otherwise leave to its
    // default HW state.....
    //
    if(ChangeVarPortLinkSpeedLate(IioGlobalData, i, (UINT8)PortIndex) == TRUE){
      DataSet &= ~(B_PCIE_PORT_PXPLNKCAP_LNKSPEED);
      DataSet |= CheckVarPortLinkSpeedLate(IioGlobalData, i, (UINT8)PortIndex);
      DEBUG ((EFI_D_INFO, "  Program Gen%d!\n", (DataSet & B_PCIE_PORT_PXPLNKCAP_LNKSPEED) ));
      RetrainFlg = 1;
    }
    else
      DEBUG ((EFI_D_INFO, "  (auto mode) No speed change required!\n"));
  }

  Array = &(IioGlobalData->SetupData.ComplianceMode[0]);
  // Set Compliance Mode
  if (Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) {
    DataSet |= B_PCIE_PORT_PXPLNKCTRL2_ENCmpl;
  }

  //
  // Program De-Emphasis select (bit 6) 
  //
  if(IioGlobalData->SetupData.DeEmphasis[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) {
    DataSet |= B_PCIE_PORT_PXPLNKCTRL2_DEEMPH;
    RetrainFlg = 1;
  } 
  
  //
  // Update PCI Express Link Control Register 2
  //
  S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,PciAddress.Function, 
                                   R_PCIE_PORT_PXPLNKCTRL2), DataSet);
  if(CheckSoftSimulationPlatform())
    RetrainFlg = 0;

  //
  // Retrain link if required.
  //
  if (RetrainFlg) {
 
      //
      // Avoid race conditions.  Loop on Link Training bit to wait for any previous Training to finish.
      //  (Note: If link bad than don't want to wait forever, 100ms wait would be enough)
      if (CheckSoftSimulationPlatform())
          // SoftSDV Hangs here as Bit11 neaver clears. Added 1ms delay for now.
	      // Need to resolve this with SoftSDV team.
	      gBS->Stall (1000); //1ms. 
      else {
        Loop = 0;
	      do {
              Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                              R_PCIE_PORT_PXPLNKSTS));
              Var16 &= BIT11;
              if(Var16)
                IioStall (IioGlobalData, LINK_INIT_POLL_TIMEOUT);   //10ms
              Loop++;
          } while (Var16 && Loop < 10);   //max 100ms wait
      }

      //
      // Now order the link to retrain.
      //
      Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                              R_PCIE_PORT_PXPLNKCTRL));
      Var16 |= B_PCIE_PORT_PXPLNKCTRL_RETRAIN_LINK;
      S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                        R_PCIE_PORT_PXPLNKCTRL), Var16);
  
      //
      // wait for the training to finish, max wait time 100ms
      //
      Loop = 0;
      do {
        Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
            R_PCIE_PORT_PXPLNKSTS));
        Var16 &= BIT11;
        if(Var16)
        IioStall (IioGlobalData, LINK_INIT_POLL_TIMEOUT);   //10ms
        Loop++;
      } while (Var16 && Loop < 10);
  } 

  //
  // Program PCIe port clock source
  //  
  Array = &(IioGlobalData->SetupData.PcieSlotClockSource[0]);
  Sccon = Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex];

  //
  // Program PCIe port clocking
  //
  Array = &(IioGlobalData->SetupData.PcieCommonClock[0]);
  Var16 = Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex];

  InitPortClock(IioGlobalData, PciAddress, (UINT8)Sccon, (UINT8)Var16);

  //
  // Traversing PCI Express port to check health of link
  //        
  PcieLinkTrained = FALSE;
  Loop            = 0;    
  do {
     PcieLinkStatus.Word = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                              R_PCIE_PORT_PXPLNKSTS));

    if (PcieLinkStatus.Bits.LinkTraining == 0) {
      //
      // if completed training
      //
      PcieLinkTrained = TRUE;
    } else {
      //
      // Wait for 10ms
      //
      IioStall (IioGlobalData, LINK_INIT_POLL_TIMEOUT);
      Loop++;
    }    
  } while (!PcieLinkTrained && Loop < 100);
        
  LinkCtrl = 0xFFFF;

  //
  // Check link status
  //
  DisableLink = FALSE;
  if (!PcieLinkTrained && Loop == 100) {
    //
    // If link training expired and still not complete, PORT is declared bad
    // If Link training completed with error, PORT is declared bad
    // PORT is disabled as a result
    //
    DisableLink = TRUE;    
    DEBUG ((EFI_D_ERROR, "  Link retrain failed!.....hence (B%d:D%d:F%d) DISABLED\n",
        PciAddress.Bus, PciAddress.Device, PciAddress.Function));
  }else{
    //
    // The link retraining is complete here, but wait for 100ms on the DL_Active state if not set
    //
    if(!(PcieLinkStatus.Word & B_PCIE_PORT_PXPLNKSTS_DLLA))
      IioStall (IioGlobalData, WAIT_FOR_DOWNSTREAM_DEV);   //100ms - min delay as per the spec to wait for the downstream devices to come up
  }
  
  //
  // Detect DL_active status
  //  
  Array = &(IioGlobalData->SetupData.PcieLinkDis[0]);
  ArrayPortDis = &(IioGlobalData->SetupData.PciePortDisable[0]); 
  
  if(ArrayPortDis[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex] == 0x01) {
    // If PciePortDisable is set to "Enable", force link on even if it did not train.
    // PcieLinkDis (below) can still override this.
    DisableLink = FALSE;
  }else if(ArrayPortDis[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex] == 0) {
    // If PciePortDisable is set to "AUTO", than take care of port specific
    // special requirements, otherwise set the ports to enable by default
    // for example, as the PO safe config the requirement was to keep the
    // uplink port (if any) disable by default....
    if(CheckVarPortEnableLate(IioGlobalData, i, (UINT8)PortIndex) == FALSE)   //port needs to be disabled
      DisableLink = TRUE;
  }

  if (Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex] || 
    ArrayPortDis[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex] == 0x02) {    //
    //
    // User need to disable link
    //
    DisableLink = TRUE;    
  } 

  Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                          R_PCIE_PORT_PXPLNKCTRL));

  if (DisableLink) {
    Var16 |= B_PCIE_PORT_PXPLNKCTRL_LNKDIS;
    LinkCtrl = Var16;
  } else {
    //s3427267 - start
    Var16 &= ~(BIT1+BIT0);
    // 4168647: BIOS to enable ASPM L1 on DMI and PCIe ports by default
    if (IioGlobalData->SetupData.PcieGlobalAspm) {
       Aspm = IioGlobalData->SetupData.PcieGlobalAspm;
     } else {
       Aspm = CheckVarPortASPMLate(IioGlobalData, i, (UINT8)PortIndex);
     }
    Var16 |= Aspm;
    //s3427267 - end
  }
  S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                   R_PCIE_PORT_PXPLNKCTRL), Var16);
  
  //
  // Program Extended Sync
  //  
  Array = &(IioGlobalData->SetupData.ExtendedSync[0]);
  
  if (Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) {
    //
    // Need to disable Extended Sync - PCIE_PORT_REG_ADDR (PortNumber, R_PCIE_PORT_PXPLNKCTRL),
    //
    DisableLink = TRUE;    
  } 

  if (DisableLink) {
    Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                              R_PCIE_PORT_PXPLNKCTRL));
    Var16 |= B_PCIE_PORT_PXPLNKCTRL_EXT_SYNCH;
    S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                        R_PCIE_PORT_PXPLNKCTRL), Var16);
    LinkCtrl = Var16;
  }

  if (LinkCtrl != 0xFFFF) {
    S3BootScriptSaveMemWrite (
            S3BootScriptWidthUint16,
            IioGlobalData->IioVData.PciExpressBase + PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, R_PCIE_PORT_PXPLNKCTRL),
            1,
            &LinkCtrl
          );
  }                          
}

/*++
=========================================================================================
=========================================================================================
--*/

VOID
PcieSlotInit (  
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
  IIO_INIT_PHASE                                 Phase,
  IIO_GLOBALS                                    *IioGlobalData
  )
{
  UINT64         ExtCtrl;
  UINT32         CTRL;
  UINT16         SlotConfig;  
  UINT16         Var16;
  UINT64         Var64;
  UINT32         Var32;
  UINT8          *Array;
  UINT8          *Array2;
  UINT8          *Array3;
  UINT8          i;
  UINT32         PortIndex;
  UINT16         *Array4;
  VPP_INVERTS_IIO_RAS_STRUCT	VppInverts;


  // Skip IOSF port
  if(OemIdentifyVendorSpecificPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function, FALSE) == TRUE)
    return;

  PortIndex = 0;

  // Find which IIO we are configuring (as shown in setup) 
  for ( i = 0 ; i < MaxIIO ; i++ ){
      if ((PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[i]) && IioGlobalData->IioVData.SocketPresent[i]) break;
  }

  if (i >= MaxIIO) return;

  PCIEDEVFUNC2PORTINDEX(PortIndex, PciAddress.Device, PciAddress.Function);
  if (((i*NUMBER_PORTS_PER_SOCKET)+PortIndex) >= MAX_TOTAL_IIO_PORTS) return;

  if(Phase == IIOInitPhase1) {
    Array = &(IioGlobalData->SetupData.SLOTIMP[0]);
    SlotConfig =(Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]);
    //
    // Set slot implemented bit
    //
    Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, R_PCIE_PORT_PXPCAP));
    Var16 &= ~(SlotConfig ? 0:B_PCIE_PORT_PXPCAP_SLOTIMPL);
    Var16 |= (SlotConfig ? B_PCIE_PORT_PXPCAP_SLOTIMPL:0);
    S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, R_PCIE_PORT_PXPCAP), Var16);

    //
    // HSD 4166776: PCIE slot power loss on warm reset
    //              BIOS needs to ensure that SLOTCAP is zero if HP disabled
    //              Also MRLSP, the code below uses these two to decide which
    //              .. slot power control function to use
    //
    if (!IioGlobalData->SetupData.PcieHotPlugEnable) {
      Array3 = &(IioGlobalData->SetupData.SLOTHPCAP[0]);
      Array3[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex] = 0;
      Array3 = &(IioGlobalData->SetupData.SLOTMRLSP[0]);
      Array3[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex] = 0;
    }
  
    //
    // Program VPP, enable VPP and misc
    //
    ExtCtrl = ((UINT64)PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_VPPCTL+4)) << 0x20);
    ExtCtrl |= (UINT64)PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_VPPCTL));
  
    ExtCtrl &= ~((UINT64)B_IIO_VPPCTL_Enb << (UINT8)PortIndex);
    ExtCtrl &= ~(((UINT64)(B_IIO_VPPCTL_RPrt | B_IIO_VPPCTL_AAddr)) << ((UINT8)PortIndex*4));

    Array = &(IioGlobalData->SetupData.VppEnable[0]);
    Array2 = &(IioGlobalData->SetupData.VppPort[0]);
    Array3 = &(IioGlobalData->SetupData.VppAddress[0]);

    //
    // HSD 4166776: PCIE slot power loss on warm reset
    //              BIOS needs to ensure that VPP address and enable are zero if HP disabled
    //
    if (IioGlobalData->SetupData.PcieHotPlugEnable) {
      if (Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) { // If zero, VPP for this port is disabled...
        Var64 = (UINT64)(Array2[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex] & 1);
        Var64 |= ((Array3[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) & 0xf);      // mask off the upper unused SMBus address bits
        Var64 = Var64 << (PortIndex*4);
        ExtCtrl |= Var64;

        Var64 = ((UINT64)B_IIO_VPPCTL_Enb << PortIndex);
        ExtCtrl |= Var64;
        //
        // 4168364: ACPI:  Bit 3 in register 188h of IIO Root Ports is set during OS boot, but not restored during S3 resume
        //
        Var32 = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, MISCCTRLSTS_0_IIO_PCIE_REG));
        Var32 |= BIT3; // enable_acpi_mode_for_hotplug
        S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, MISCCTRLSTS_0_IIO_PCIE_REG), Var32);
      }
    }

    //
    //  HSD 4167027: PCIE: BIOS needs to program PCA9555 for correct polarity
    //
    //  HSD 4987677: The PCA9555 polarity setting will not effect for CRB builds
    //  
    if (IioGlobalData->SetupData.PcieHotPlugEnable) {
      VppInverts.Data = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM,
                                                        ONLY_REGISTER_OFFSET(VPP_INVERTS_IIO_RAS_REG )));
      VppInverts.Bits.dfr_inv_pwren = OemGetVppInvertRegValue(IioGlobalData , i );
      S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM,
                                    ONLY_REGISTER_OFFSET(VPP_INVERTS_IIO_RAS_REG)), VppInverts.Data);
    }

    //
    // Update PCIE Slot Capabilities Register  
    //
    Array = &(IioGlobalData->SetupData.PcieLinkDis[0]); 
    Array2 = &(IioGlobalData->SetupData.SLOTIMP[0]);
    Array3 = &(IioGlobalData->SetupData.SLOTHPCAP[0]);
  
    if (!Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) {             // Valid PCIE Port? 
      if (Array2[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) {           // Slot Implemented?
        CTRL = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, R_PCIE_PORT_PXPSLOTCAP));
  
        //
        // set physical slot number
        // 
        // HSD: 4168942: PCIe physical slot number register is 13 bit, 
        // but reference code interface can only program 8 bit
        //
        Array4 = &(IioGlobalData->SetupData.SLOTPSP[0]);
        CTRL |= ((Array4[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex] & 0x1FFF) << 19);
  
        //
        // set Electromechanical Interlock Present
        //
        Array3 = &(IioGlobalData->SetupData.SLOTEIP[0]);
        if (Array3[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex])
          CTRL |= B_PCIE_PORT_PXPSLOTCAP_EIP;
  
        //
        // set Slot Power Limit Scale
        //
        Array3 = &(IioGlobalData->SetupData.SLOTSPLS[0]);
        CTRL |= (Array3[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex] << 15);
  
        //
        // set slot power limit value
        //
        Array3 = &(IioGlobalData->SetupData.SLOTSPLV[0]);
        CTRL |= (Array3[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex] << 7);
  
        //
        // set Hot Plug Capable
        // If slot supports HP then enable set the correct bit in 
        // in the SLOTCAp register & call slot power control
        //
        Array3 = &(IioGlobalData->SetupData.SLOTHPCAP[0]);
        if (Array3[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex])
          CTRL |= B_PCIE_PORT_PXPSLOTCAP_HPC;
  
        //
        // set Power Indicator Present 
        //
        Array3 = &(IioGlobalData->SetupData.SLOTPIP[0]);
        if (Array3[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) 
          CTRL |= B_PCIE_PORT_PXPSLOTCAP_PIP;
  
        //
        // set Attention Indicator Present 
        //
        Array3 = &(IioGlobalData->SetupData.SLOTAIP[0]);
        if (Array3[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex])
          CTRL |= B_PCIE_PORT_PXPSLOTCAP_AIP;
  
        //
        // set MRL Senso Present 
        //
        Array3 = &(IioGlobalData->SetupData.SLOTMRLSP[0]);
        if (Array3[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex])
          CTRL |= B_PCIE_PORT_PXPSLOTCAP_MRLSP;
  
        //
        // set Power Controller Present 
        //
        Array3 = &(IioGlobalData->SetupData.SLOTPCP[0]);
        if (Array3[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex])
          CTRL |= B_PCIE_PORT_PXPSLOTCAP_PCP;
  
        //
        // set Attention Button Present 
        //
        Array3 = &(IioGlobalData->SetupData.SLOTABP[0]);
        if (Array3[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex])
          CTRL |= B_PCIE_PORT_PXPSLOTCAP_ABP;
  
        S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, R_PCIE_PORT_PXPSLOTCAP), CTRL);

        /* Delay moveded to IioInit() in order to have a common delay for all ports
          //
          // intended fix for s4030382:Pcie cards are not powering up after
          //                  a CLDRST with Serial Debug Msgs: Disabled and no PCIE_ON jumper
          // note: the write to RP SLTCAP register triggers the "Set_Slot_Power_Limit"
          //        message to be send which is used by IIO, hence their should be some delay here
          IioStall (IioGlobalData, SLTCAP_DELAY);   //200ms delay
        */
      } // Slot Implemented?
    } // Valid PCIe Port?

   //
   // 4987686: VPP_EN has to be driven after SLTCAP is set
   //
   S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_VPPCTL), (UINT32)ExtCtrl);
   S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, CORE05_DEV_NUM, CORE05_FUNC2_NUM, R_IIO_VPPCTL+4), (UINT32)(ExtCtrl >> 0x20));
  } // IIOInitPhase1

  //
  // 4987686: After VPP is enabled on all relevant IIOs, then go back through and enable all relevant ports.
  //
  if(Phase == IIOInitPhase2) {
    Array = &(IioGlobalData->SetupData.PcieLinkDis[0]); 
    Array2 = &(IioGlobalData->SetupData.SLOTIMP[0]);
    if (!Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) {             // Valid PCIE Port? if yes continue 
      if (Array2[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) {
        //
        // 4987686: After VPP is enabled on all relevant IIOs, write Slot Control to default value
        //
        PciExpressWrite16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                          R_PCIE_PORT_PXPSLOTCTRL), PCIE_PORT_PXPSLOTCTRL_DEFAULT);
        //
        // Previously we were not calling the SlotPower Control routine for HP init so added the call if
        // the slot is HP capable.  This CALL MUST occur after programming HC Capability and MRL present capable bits
        //
        CTRL = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, R_PCIE_PORT_PXPSLOTCAP));
        if( (CTRL & (B_PCIE_PORT_PXPSLOTCAP_HPC | B_PCIE_PORT_PXPSLOTCAP_MRLSP)) ) {
          SlotPowerControl (IioGlobalData, PciAddress);
        } else {
          EnableSlotPower (IioGlobalData, PciAddress);
        }
      }
    }
  }
}

/*++
=========================================================================================
=========================================================================================
--*/

VOID
PcieMiscInit (  
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
  IIO_GLOBALS   *IioGlobalData
  )
{
  UINT8   MsiEn;
  UINT8   Var8;
  UINT8   *Array;
  UINT8   i;
  UINT32  PortIndex = 0;
  PERFCTRLSTS_1_IIO_PCIE_HSX_BDX_STRUCT      PerfCtrlSts1;
  PERFCTRLSTS_0_IIO_PCIEDMI_HSX_BDX_STRUCT   PerfCtrlSts0;
  // Find which IIO we are configuring (as shown in setup) 
  for ( i = 0 ; i < MaxIIO ; i++ ){
      if ((PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[i]) && IioGlobalData->IioVData.SocketPresent[i]) break;
  }

  if (i >= MaxIIO) return;
  PCIEDEVFUNC2PORTINDEX(PortIndex, PciAddress.Device, PciAddress.Function);
  if (PortIndex >= MAX_TOTAL_IIO_PORTS) return;
  
  Array = &(IioGlobalData->SetupData.MsiEn[0]);
  if (Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) {
    MsiEn = B_PCIE_PORT_MSICTRL_MSIEN;
  } else {
    MsiEn = 0;
  }

  Var8 = PciExpressRead8 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                              R_PCIE_PORT_MSICTRL));
  Var8 &= ~(B_PCIE_PORT_MSICTRL_MSIEN);
  Var8 |= (MsiEn);
  S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                        R_PCIE_PORT_MSICTRL), Var8);


  //
  // PCIE PERFCTRLSTS 
  //
  PerfCtrlSts0.Data = PciExpressRead32(PCI_PCIE_ADDR(PciAddress.Bus,PciAddress.Device , PciAddress.Function, ONLY_REGISTER_OFFSET(R_PCIE_PORT_PCIEIPERFCTRLSTS)));
   
  // apply the PERFCTRL related BIOS setup options
  if( OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == TRUE){

     //
     // 4986124: HSX Clone: [C0 PO] Isoc: Vcm appears to be the only traffic getting priority at the HA
     // open bits definition to setup Vc1 correctly and get priority on the ring
     //
     if (((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping >= C0_REV_HSX)) ||
          (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
          PerfCtrlSts0.Bits.vc1m_nosnoopopdis = 0;
       } else {
          PerfCtrlSts0.Bits.vc1m_nosnoopopdis = 1;
       }
    PerfCtrlSts0.Bits.vcp_nosnoopopen = 1;

    if(IioGlobalData->SetupData.DmiAllocatingFlow){
      PerfCtrlSts0.Bits.use_allocating_flow_wr = 1;
    } else {
      PerfCtrlSts0.Bits.use_allocating_flow_wr = 0;
    }
  } else {
    if (IioGlobalData->SetupData.PcieAllocatingFlow) {
       PerfCtrlSts0.Bits.use_allocating_flow_wr = 1;
    } else {
       PerfCtrlSts0.Bits.use_allocating_flow_wr = 0;
  }
  }

  //
  // HSD: 4987853: [BDX] IIO:Update NoSnoop BIOS Knob to only Enable/Disable options.
  //
  if (IioGlobalData->SetupData.NoSnoopVc0Cfg) {
    PerfCtrlSts0.Bits.nosnoopoprden = 1;
    PerfCtrlSts0.Bits.nosnoopopwren = 1;
  } else {
    PerfCtrlSts0.Bits.nosnoopoprden = 0;
    PerfCtrlSts0.Bits.nosnoopopwren = 0;
  }

  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, R_PCIE_PORT_PCIEIPERFCTRLSTS), PerfCtrlSts0.Data);

  PerfCtrlSts1.Data = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, 
                                                      ONLY_REGISTER_OFFSET(PERFCTRLSTS_1_IIO_PCIE_REG)));

  //
  // HSD 4165824: PCIE: BIOS option for PCIRdCurrent/DRd.UC mode select (CIPCTRL[0])
  //
  PerfCtrlSts1.Bits.full_cohrd_op = IioGlobalData->SetupData.CoherentReadFull;
  PerfCtrlSts1.Bits.partial_cohrd_op = IioGlobalData->SetupData.CoherentReadPart;

  //enable/disable TPH
  if(IsPciePortTPHCapable(PciAddress, IioGlobalData) == TRUE){
    if(IioGlobalData->SetupData.DisableTPH)
      PerfCtrlSts1.Bits.tphdis = 1;                           //disable TPH
    else
      PerfCtrlSts1.Bits.tphdis = 0;                           //enable TPH
  }
  
  if(IioGlobalData->SetupData.MaxReadCompCombSize) {
    PerfCtrlSts1.Bits.max_read_completion_combine_size = 1;  
  }

  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                    ONLY_REGISTER_OFFSET(PERFCTRLSTS_1_IIO_PCIE_REG)), PerfCtrlSts1.Data);

  //
  // 4986124: HSX Clone: [C0 PO] Isoc: Vcm appears to be the only traffic getting priority at the HA
  // open bits definition to setup Vc1 correctly and get priority on the ring
  //
  if ( OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == TRUE) { // DMI port
    if ((IioGlobalData->IioVData.CpuType == CPU_HSX && IioGlobalData->IioVData.CpuStepping == C0_REV_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
      PerfCtrlSts0.Data = PciExpressRead32(PCI_PCIE_ADDR(PciAddress.Bus, DMI_DEV_NUM, DMI_FUNC_NUM, ONLY_REGISTER_OFFSET(PERFCTRLSTS_0_IIO_PCIEDMI_REG)));
      PerfCtrlSts0.Bits.vc1m_nosnoopopdis = 0;
      S3_PciExpressWrite32(IioGlobalData,PCI_PCIE_ADDR(PciAddress.Bus, DMI_DEV_NUM, DMI_FUNC_NUM, ONLY_REGISTER_OFFSET(PERFCTRLSTS_0_IIO_PCIEDMI_REG)), PerfCtrlSts0.Data);
    } // (HSX_C0 || BDX)
  } // DMI port

  //
  // Configure Multicast if enabled
  //  
  if ( IioGlobalData->SetupData.MultiCastEnable == TRUE ) {
    EnableMultiCast (PciAddress, IioGlobalData);
  }
     
}

/*++
=========================================================================================
=========================================================================================
--*/

VOID
PciePreInit (
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,  
  IIO_GLOBALS                                             *IioGlobalData
  )
{
  //
  // set/reset any RWO features of the PCIe port capability register here, which
  // can be used to initialize any features on the PCIe port control registers later
  //
  UINT8   i;
  UINT16  Var16;
  UINT8   Var8;
  UINT8   PortIndex = 0;

  // Find which IIO we are configuring (as shown in setup)
  for ( i = 0 ; i < MaxIIO ; i++ ){
    if ((PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[i]) && IioGlobalData->IioVData.SocketPresent[i]) break;
  }

  if (i >= MaxIIO) return;
  PCIEDEVFUNC2PORTINDEX(PortIndex, PciAddress.Device, PciAddress.Function);
  if (((i*NUMBER_PORTS_PER_SOCKET)+PortIndex) >= MAX_TOTAL_IIO_PORTS) return;
  
  //
  // init & Lock DEVCAP register
  //
  Var8 = PciExpressRead8(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,
      PciAddress.Function, R_PCIE_PORT_PXPDEVCAP));
  if (((IioGlobalData->IioVData.CpuType == CPU_IVT) || (IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX)) && (PortIndex != 0)) {   //skip DMI and Port 0
    //
    // ivt_eco: s4420402: [PCIe TL]: IOU0/1 do not advertise in device capability
    //                    register for extended tag mode support
    //  [cloned to s4030298] - set the extended_tag_field_supported (bit5) to 1
    //
    // 4985836: PCIe NTB: NTB settings set in DEVCAP and CAPPTR even in PCIe mode
    //
    if (!(IioGlobalData->SetupData.NtbPpd[i] == NTB_PORT_DEF_TRANSPARENT || (IioGlobalData->SetupData.NtbPpd[i] == NTB_PORT_DEF_NTB_AUTO && IioGlobalData->BdxSetupData.NtbPpdAutoConfig[i] == NTB_PORT_DEF_TRANSPARENT))) {
      Var8 |= BIT5;
    } else {
      Var8 &= (~BIT5);
    }
  }
  S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,
      PciAddress.Function, R_PCIE_PORT_PXPDEVCAP), Var8);
  //
  // init & Lock DEVCAP2 register
  //
  Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                              R_PCIE_PORT_PXPDEVCAP2));
  //Set the TPH completer supported capability
  Var16 &= ~(BIT13 + BIT12);
  Var16 |= BIT12;           //set TPH completer supported bit

  //
  // 4985830: IIO ARI_EN Workaround is not applicable for PCIe Gen3 ports
  //
  if (IioGlobalData->IioVData.CpuType == CPU_IVT && PciAddress.Device == PCIE_PORT_0_DEV && PciAddress.Function == PCIE_PORT_0_FUNC) {
  // #3614765, #3247235: devcap2.ari_en default is "1" for root port and it
  //                     should be set to 0 by BIOS
    // This is a permanent workaround for IVT only
  //
  Var16 &= ~B_PCIE_PORT_ARI_EN;
  }

  S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                      R_PCIE_PORT_PXPDEVCAP2), Var16);
}

/*++
=========================================================================================
=========================================================================================
--*/

VOID
PciePostInit (
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
  IIO_GLOBALS   *IioGlobalData
  )
{
    UINT32  Var32;
    UINT16  Var16;
    UINT16  PciErr;
    UINT16  PortWidth;
#ifdef SDP_FLAG
    UINT16  PortMaxWidth; // HSD 4987368 SDP: PCI-E Ports Link did not train and error
#endif
    BOOLEAN PortLinked;
    UINT8   PortSpeed;
    UINT8   PortIndex = 0;
    UINT8   Var8; 
    UINT8   i;

	// skip DMI port
	if ( OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == TRUE) {
	   return;
	}
  
    // Find which IIO we are configuring (as shown in setup) 
    for ( i = 0 ; i < MaxIIO ; i++ ){
        if ((PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[i]) && IioGlobalData->IioVData.SocketPresent[i]) break;
    }
	   
    if (i >= MaxIIO) return;
    PCIEDEVFUNC2PORTINDEX(PortIndex, PciAddress.Device, PciAddress.Function);
    if (((i*NUMBER_PORTS_PER_SOCKET)+PortIndex) >= MAX_TOTAL_IIO_PORTS) return;
    
    //
    // Command regiser has to be set after PCI bus enumeration
    // PCI bus driver will change the command register during the enumeration
    //
    PciErr = 0;

    if (IioGlobalData->IioVData.Serr){
       PciErr |= B_PCIE_PORT_PCICMD_SERRE;
    }	     
    if (IioGlobalData->IioVData.Perr){
       PciErr |= B_PCIE_PORT_PCICMD_PERRE;
    }
    //
    // Programe PCI SERR/PERR
    //
    Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                              PCI_COMMAND_OFFSET));
    Var16 |= (PciErr);
    S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                        PCI_COMMAND_OFFSET), Var16);

    //
    // Update for PCI Express Port 1-10 (PciAddress.Device)
    //
    PortWidth = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                                R_PCIE_PORT_PXPLNKSTS));
#ifdef SDP_FLAG
    // HSD 4987368 SDP: PCI-E Ports Link did not train and error
    PortMaxWidth = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                                R_PCIE_PORT_PXPLNKCAP));
#endif
    Var32 = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                              R_PCIE_PORT_PXPSLOTCAP));
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                      R_PCIE_PORT_PXPSLOTCAP), Var32);

    Var32 = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                            R_PCIE_PORT_PXPLNKCAP));
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                                      R_PCIE_PORT_PXPLNKCAP), Var32);

    // remove the following because capptr register access type = RO
    //Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, (PciAddress.Device), PciAddress.Function,
    //                                          R_PCIE_PORT_CAPPTR));
    //S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, (PciAddress.Device), PciAddress.Function,
    //                                    R_PCIE_PORT_CAPPTR), Var16);

    Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, (PciAddress.Device), PciAddress.Function,
                                              R_PCIE_PORT_PXPCAP));
    S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, (PciAddress.Device), PciAddress.Function,
                                        R_PCIE_PORT_PXPCAP), Var16);

    // Gather and store data about linked ports.
    PortSpeed = PortWidth & B_PCIE_PORT_PXPLNKSTS_LNKSPD;
    PortLinked = ((PortWidth & B_PCIE_PORT_PXPLNKSTS_DLLA) == B_PCIE_PORT_PXPLNKSTS_DLLA);
    PortWidth &= B_PCIE_PORT_PXPLNKSTS_NLNKWD;
    PortWidth >>= 4;
#ifdef SDP_FLAG
	// HSD 4987368 SDP: PCI-E Ports Link did not train and error
    PortMaxWidth &= B_PCIE_PORT_PXPLNKSTS_NLNKWD;
    PortMaxWidth >>= 4;
#endif

    // Can't trust Negotiated Link Width to be zero if not linked, so need to check port-linked bit.
    if(PortLinked) {
      IioGlobalData->IioOutData.LinkedPXPMap[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex] = (UINT8) PortWidth;
#ifdef SDP_FLAG
      // HSD 4987368 SDP: PCI-E Ports Link did not train and error
	  IioGlobalData->IioOutData.MaxPXPMap[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex] = (UINT8) PortMaxWidth;
#endif
    } else {
      IioGlobalData->IioOutData.LinkedPXPMap[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex] = 0;
#ifdef SDP_FLAG
	  // HSD 4987368 SDP: PCI-E Ports Link did not train and error
      IioGlobalData->IioOutData.MaxPXPMap[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex] = 0;
#endif
    }
    IioGlobalData->IioOutData.SpeedPXPMap[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex] = PortSpeed;

    //
    // Handle programming of RWO and RWOG registers for Iio Port functions
    //

    //
    // Lock Revision ID
    //
    Var8 = PciExpressRead8(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, 
                    PciAddress.Function, PCI_REVISION_ID_OFFSET));
    S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, 
                    PciAddress.Function, PCI_REVISION_ID_OFFSET), Var8);

    if (FeaturePcdGet (PcdLockCsrSsidSvidRegister)) {
      
      if ((PciAddress.Device == 3) && (PciAddress.Function == 0)) {
        
        Var8 = PciExpressRead8(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, R_NTB_PPD));
        if ((Var8 & B_NTB_PORT_DEF) != 0) {

          // NTB mode
          Var16 = PciExpressRead16(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, 
                        PciAddress.Function, R_PCIE_PORT_SSVID));
          S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, 
                        PciAddress.Function, R_PCIE_PORT_SSVID), Var16);
        } else {

          // PCIe mode
          Var16 = PciExpressRead16(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, 
                        PciAddress.Function, R_PCIE_PORT_SVID_OTHER));
          S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, 
                        PciAddress.Function, R_PCIE_PORT_SVID_OTHER), Var16);
        }
      } else {

        //
        // Lock Other Subsystem Vendor ID
        //
        Var16 = PciExpressRead16(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, 
                        PciAddress.Function, R_PCIE_PORT_SVID_OTHER));
        S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, 
                        PciAddress.Function, R_PCIE_PORT_SVID_OTHER), Var16);
      }
      //
      // Lock Other Subsystem Device ID
      //
      Var16 = PciExpressRead16(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, 
                      PciAddress.Function, R_PCIE_PORT_SID_OTHER));
      S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, 
                      PciAddress.Function, R_PCIE_PORT_SID_OTHER), Var16);
    }

    //
    // Lock Capabilities pointer
    //
    // remove following because PCI cap ptr is Read-Only
    //Var8 = PciExpressRead8(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, 
    //                PciAddress.Function, PCI_CAPBILITY_POINTER_OFFSET));
    //S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, 
    //                PciAddress.Function, PCI_CAPBILITY_POINTER_OFFSET), Var8);

    //
    // Lock MSINXTPTR register
    //
    Var8 = PciExpressRead8(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, 
                    PciAddress.Function, R_PCIE_PORT_MSINXTPTR));
    S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, 
                    PciAddress.Function, R_PCIE_PORT_MSINXTPTR), Var8);

    //
    // Lock PXPNXTPTR register
    //
    Var8 = PciExpressRead8(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, 
                    PciAddress.Function, R_PCIE_PORT_PXPNXTPTR));
    S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, 
                    PciAddress.Function, R_PCIE_PORT_PXPNXTPTR), Var8);

    //
    // Lock PMCSR register
    //
    Var8 = PciExpressRead8(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, 
                PciAddress.Function, R_PCIE_PORT_PXPPMCSR));
    S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, 
                PciAddress.Function, R_PCIE_PORT_PXPPMCSR), Var8);
    //
    // Clear Expected PCIe Errors (3612591, 3875328)
    //  [permanent programming consideration for all steppings]
    Var32 = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
              ONLY_REGISTER_OFFSET(UNCERRSTS_IIO_PCIE_REG)));
    Var32 |= BIT20;   //clear unsupported requests error
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
        ONLY_REGISTER_OFFSET(UNCERRSTS_IIO_PCIE_REG)), Var32);

    Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                  ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIE_REG)));
    Var16 |= (BIT14 + BIT15);   //clear Link Bandwidth Management Status & Link Autonomous Bandwidth Status
    S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
        ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIE_REG)), Var16);

    Var32 = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                  ONLY_REGISTER_OFFSET(XPCORERRSTS_IIO_PCIE_REG)));
    Var32 |= BIT0;   //clear PCILink Bandwidth Changed Status
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
        ONLY_REGISTER_OFFSET(XPCORERRSTS_IIO_PCIE_REG)), Var32);
    //
    // ivt_eco: s4676907: IVT A1: Warm resets cause width degrade from x8 to x4
    //                    @ gen2 and gen1 and gen3
    //  [cloned to s4030723] - forcing unused ports to L1 not required for IVT
    if(IioGlobalData->SetupData.ForceUnusedPortsToL1)
      ForceUnusedPortsToL1 (PciAddress, IioGlobalData,1);

}

/*++
=========================================================================================
=========================================================================================
--*/
VOID
PcieEarlyInit (  
    PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
    IIO_INIT_PHASE                                Phase,
    IIO_GLOBALS                                       *IioGlobalData
  )
{
    //
    // The purpose of this routine is to do any {CIE Port programming prior to actually training the PCIE links
    //

    UINT64  ExtCtrl;
    UINT8   *Array;
    UINT8   i;
    UINT32  PortIndex = 0;
#ifdef  DE_SKU    
	CAPID1_PCU_FUN3_HSX_BDX_STRUCT	PcuF3CAP1; 
#endif

    // Find which IIO we are configuring (as shown in setup) 
    for ( i = 0 ; i < MaxIIO ; i++ ){
        if ((PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[i]) && IioGlobalData->IioVData.SocketPresent[i]) break;
    }

    if (i >= MaxIIO) return;
    PCIEDEVFUNC2PORTINDEX(PortIndex, PciAddress.Device, PciAddress.Function);
    if (((i*NUMBER_PORTS_PER_SOCKET)+PortIndex) >= MAX_TOTAL_IIO_PORTS) return;
    
    DEBUG ((EFI_D_INFO, "PciEarlyInit at device scanning...\n"));

    // Program MISCCTRL 
    // This was moved here because there are RWO registers that must be programmed.  
    //
    ExtCtrl = ((UINT64)PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,PciAddress.Function, R_PCIE_PORT_PCIEMISCCTRLSTS+4)) << 0x20);
    ExtCtrl |= (UINT64)PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,PciAddress.Function, R_PCIE_PORT_PCIEMISCCTRLSTS));

    //
    // clear all configurable bits and update accordingly 
    //
    ExtCtrl &= ~(B_PCIE_PORT_PCIEMISCCTRLSTS_FormFact | B_PCIE_PORT_PCIEMISCCTRLSTS_MSIFATEN |
                      B_PCIE_PORT_PCIEMISCCTRLSTS_MSINFATEN | B_PCIE_PORT_PCIEMISCCTRLSTS_MSICOREN |
                      B_PCIE_PORT_PCIEMISCCTRLSTS_ACPI_PMEEN |B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_L0sTx |
                      /*B_PCIE_PORT_PCIEMISCCTRLSTS_InboundIOEN | */B_PCIE_PORT_PCIEMISCCTRLSTS_SIOLnkBWSts |
                      B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_EOI | B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_INB_P2PWrtDis |
                      B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_INB_P2PRdDis | B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_PHold |
                      B_PCIE_PORT_PCIEMISCCTRLSTS_PME_Ack_TOCtl | B_PCIE_PORT_PCIEMISCCTRLSTS_Rcv_PME_TO_ACK |
                      B_PCIE_PORT_PCIEMISCCTRLSTS_Send_PME_OFF | B_PCIE_PORT_PCIEMISCCTRLSTS_SysERR_only4_AER |
                      B_PCIE_PORT_PCIEMISCCTRLSTS_EN_ACPI_HotPlug | B_PCIE_PORT_PCIEMISCCTRLSTS_EN_ACPI_PM/* |
                      B_PCIE_PORT_PCIEMISCCTRLSTS_Inbound_CFGEN |B_PCIE_PORT_PCIEMISCCTRLSTS_Set_HostBrdg_CC*/);


    //
    // Adding this for SV PRD requirement
    //
	Array = &(IioGlobalData->SetupData.EOI[0]);

  if (Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) {
      ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_EOI;
    } 

  //if port7 is Ntb, don't forward EOI
  if (!(IioGlobalData->SetupData.NtbPpd[i] == NTB_PORT_DEF_TRANSPARENT || (IioGlobalData->SetupData.NtbPpd[i] == NTB_PORT_DEF_NTB_AUTO && IioGlobalData->BdxSetupData.NtbPpdAutoConfig[i] == NTB_PORT_DEF_TRANSPARENT)) &&
    ((PciAddress.Device == 3) && (PciAddress.Function == 0))) {
    ExtCtrl &= ~B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_EOI;
  }


    // Override System Error on PCIE Fatal Errors  - Bit 35 of MISCCTRLSTS
	Array = &(IioGlobalData->SetupData.MSIFATEN[0]);
  if (Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) {
      ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_MSIFATEN;
    } 

    // Override System Error on PCIE Non-Fatal Errors  - Bit 34 of MISCCTRLSTS
	Array = &(IioGlobalData->SetupData.MSINFATEN[0]);
    if (Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) {
      ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_MSINFATEN;
    }

    // Override System Error on PCIE Correctable  Errors  - Bit 33 of MISCCTRLSTS
	Array = &(IioGlobalData->SetupData.MSICOREN[0]);
    if (Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) {
      ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_MSICOREN;
    }

    // ACPI PME Interrupt Enable  - Bit 32 of MISCCTRLSTS
	Array = &(IioGlobalData->SetupData.ACPIPMEn[0]);
    if (Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) {
      ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_ACPI_PMEEN;
    }

    //
    // 4166584: L0s Feature Is Still Partially Present in the HSX BIOS
    //
    if (IioGlobalData->IioVData.CpuType == CPU_IVT) {
    // Disable L0s on transmitter  - Bit 31 of MISCCTRLSTS
    Array = &(IioGlobalData->SetupData.DISL0STx[0]);
    if (Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) {
      ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_L0sTx;
    }
    } else if ((IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
      ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_L0sTx;
    }

 /*   // Inbound IO Enable  - Bit 30 of MISCCTRLSTS
	Array = &(IioGlobalData->SetupData.INBOUNDIOEnIoh0f0);
    if (Array[(i*NUMBER_PORTS_PER_SOCKET)+PciAddress.Device]) {
      ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_InboundIOEN;
    }*/

    // P2P Memory Write Disable  - Bit 25 of MISCCTRLSTS
	Array = &(IioGlobalData->SetupData.P2PWrtDis[0]);
    if (Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) {
      ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_INB_P2PWrtDis;
    }

    // P2P Memory Read Disable  - Bit 24 of MISCCTRLSTS
	Array = &(IioGlobalData->SetupData.P2PRdDis[0]);
    if (Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) {
      ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_INB_P2PRdDis;
    }

    // PME_TO_ACK_Timeout  - Bit 8:7 of MISCCTRLSTS
	/*Array = &(IioGlobalData->SetupData.PMEAckTOCtl[0]);
    if (Array[(i*NUMBER_PORTS_PER_SOCKET)+PciAddress.Device]) {
      ExtCtrl |= ((Array[PciAddress.Device]) << V_PCIE_PORT_PCIEMISCCTRLSTS_PME_AckTOCtl);
    }*/

    // Disable Timeout 4 Rcv PME TO ACK  - Bit 6 of MISCCTRLSTS
	Array = &(IioGlobalData->SetupData.DisPMETOAck[0]);
    if (!Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) {
      ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_PME_TO_ACK_TIMEOUT;
      //
      // if receiving of PME to ACK TO is enabled than use global
      // option to set the timeout value in control bits 8:7
      ExtCtrl |= ((IioGlobalData->SetupData.GlobalPme2AckTOCtrl & 3) << V_PCIE_PORT_PCIEMISCCTRLSTS_PME_AckTOCtl);
    }
    else{
      ExtCtrl &= ~B_PCIE_PORT_PCIEMISCCTRLSTS_PME_TO_ACK_TIMEOUT;
    }

    //
    // Enable ACPI mode HOTPLUG  - Bit 3 of MISCCTRLSTS
    //
    // 4168626 : SCI for Hot Plug is not working
    //
    if (IioGlobalData->SetupData.PcieAcpiHotPlugEnable == 1) {
         ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_EN_ACPI_HotPlug;
    } else if (IioGlobalData->SetupData.PcieAcpiHotPlugEnable == 2) {
        Array = &(IioGlobalData->SetupData.ACPIHP[0]);
        if (Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) {
          ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_EN_ACPI_HotPlug;
        }
    }

    //
    // Enable ACPI PM  - Bit 2 of MISCCTRLSTS
    //
	Array = &(IioGlobalData->SetupData.ACPIPM[0]);
    if (Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) {
      ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_EN_ACPI_PM;
    }

    /*//
    // Inbound CFG Enable  - Bit 1 of MISCCTRLSTS
    //
	Array = &(IioGlobalData->SetupData.INBCFGEn[0]);
    if (Array[(i*NUMBER_PORTS_PER_SOCKET)+PciAddress.Device]) {
      ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_Inbound_CFGEN;
    }*/
    //
    // Enable / disable MCTP broadcast to this link - bit 37 of MISCCTRLSTS
    //
    if (IioGlobalData->IioVData.MctpEn)
    	ExtCtrl &= ~B_PCIE_PORT_PCIEMISCCTRLSTS_Dis_MCTP_BC_to_lnk;
    else
    	ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_Dis_MCTP_BC_to_lnk;    // 4166696: HSX: PCIE_PORT_PCIEMISCCTRLSTS Upper 32 Bits All Being Set

    //
    // Perform any overrides for the non-regular vendor-specific PCIe ports
    //
    if(OemIdentifyVendorSpecificPort(IioGlobalData, PciAddress.Bus, PciAddress.Device,PciAddress.Function, FALSE) == TRUE){
#ifdef  DE_SKU
    	//
    	//  CBDMA & GbE behind IOSF cannot respond to PME TO message (4169014)
    	//
    	ExtCtrl |= (B_PCIE_PORT_PCIEMISCCTRLSTS_dis_send_pme_to_port | B_PCIE_PORT_PCIEMISCCTRLSTS_mimic_pme_to_ack_rcvd);

        if (IioGlobalData->IioVData.SkuType == SKU_NS) { //NS
          PcuF3CAP1.Data =  PciExpressRead32 ( PCI_LIB_ADDRESS (IioGlobalData->IioVData.SocketUncoreBusNumber[0],
													            PCU22_DEV_NUM,
													            PCU22_FUNC_NUM,
													            ONLY_REGISTER_OFFSET(CAPID1_PCU_FUN3_REG)));
          //NS Storage Sku CBDMA 
		  if(OemIdentifyVendorSpecificPort_CBDMA_CPM(IioGlobalData,PciAddress.Bus, PciAddress.Device,PciAddress.Function) == TRUE && \
		  	PcuF3CAP1.Bits.storage_seg_sel == 1){
    	    ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_EOI;
    	    ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_Dis_MCTP_BC_to_lnk;
    	  }

		  //NS Comms Sku GbE1
		  if(OemIdentifyVendorSpecificPort_CPM_GbE1(IioGlobalData,PciAddress.Bus, PciAddress.Device,PciAddress.Function) == TRUE && \
		  	PcuF3CAP1.Bits.storage_seg_sel == 0){
			ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_EOI;
		  }     	  
        } else {  //DE 
        
    	  //
    	  // Disable EOIFD & MCTP broadcast to IOSF CBDMA port (4168259)
    	  //
    	  if(OemIdentifyVendorSpecificPort_CBDMA_CPM(IioGlobalData,PciAddress.Bus, PciAddress.Device,PciAddress.Function) == TRUE){
    	    ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_EOI;
    	    ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_Dis_MCTP_BC_to_lnk;
    	  }
    	  
        }
		//
    	// Disable EOIFD to IOSF GbE port only (4168259)
    	//
    	if(OemIdentifyVendorSpecificPort_GbE(IioGlobalData,PciAddress.Bus, PciAddress.Device,PciAddress.Function) == TRUE){
    	  ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_EOI;
    	}
#endif  //DE_SKU
    }

  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,PciAddress.Function, R_PCIE_PORT_PCIEMISCCTRLSTS), (UINT32)ExtCtrl);
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,PciAddress.Function, R_PCIE_PORT_PCIEMISCCTRLSTS+4), (UINT32)(ExtCtrl >> 0x20));

  if (IioGlobalData->SetupData.NtbPpd[i] == NTB_PORT_DEF_TRANSPARENT || (IioGlobalData->SetupData.NtbPpd[i] == NTB_PORT_DEF_NTB_AUTO && IioGlobalData->BdxSetupData.NtbPpdAutoConfig[i] == NTB_PORT_DEF_TRANSPARENT) && 
    ((PciAddress.Device == 3) && (PciAddress.Function == 0))) {
    //
    // s4968679: if port7 is TB (default), write CapPtr to point to subsystem cap ID
    //
    S3_PciExpressWrite32(IioGlobalData,PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,PciAddress.Function, R_PCIE_PORT_CAPPTR), R_PCIE_PORT_SCAPID);
  }
}


/*++
=========================================================================================
=========================================================================================
--*/

VOID
PciePortInit (  
  PCI_ROOT_BRIDGE_PCI_ADDRESS                    PciAddress,
  IIO_INIT_PHASE                                 Phase,
  IIO_GLOBALS                                    *IioGlobalData
  )
{
  UINT8   Var8=0;
  UINT8   *Array;
  UINT8   i;
  UINT32  PortIndex;

  PortIndex = 0;
  for ( i = 0 ; i < MaxIIO ; i++ ) {
    if ((PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[i]) && IioGlobalData->IioVData.SocketPresent[i]) {
        break;
    }
  }
  if (i >= MaxIIO) return;
  PCIEDEVFUNC2PORTINDEX(PortIndex, PciAddress.Device, PciAddress.Function);
  if (((i*NUMBER_PORTS_PER_SOCKET)+PortIndex) >= MAX_TOTAL_IIO_PORTS) return;

  if( OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == TRUE ){
     return; //skip DMI port, as this routine should only handle the flow for regular PCIe port
  }

  //
  // If the port designated for initialization is hidden, do not attempt to initialize
  //
  Array = &(IioGlobalData->SetupData.PEXPHIDE[0]);
  if (Array[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex]) return;
  Var8 = IioGlobalData->IioOutData.CurrentPXPMap[(i*NUMBER_PORTS_PER_SOCKET)+PortIndex];

  if (!Var8) {
    //
    // Put links into low power mode which are not connected to slot.
    //
    if ( Phase == IIOInitPhase1 ) {
      DEBUG ((EFI_D_INFO, "  Spare port --> [%s(%d)]\n", IIO_PORT_LABEL(PortIndex), PortIndex));
      // TBD: Need to be revisited
      // This may not be applicable if we consider the HP functionality.
      // For HP slots even if the card is connected when the system is powered up will show the link status bit as down until in phase 3
      // where the driver wiil power up the slots and retrains, until that point the endpoint device will not be visible in the slots
      // which are HP capable, and if we try to pull down the power level in phase 1, the cards connected to the HP slots will fail to train,
      // so this is not applicable as per the current flow
      //ForceUnusedPortsToL1 (PciAddress, IioGlobalData,0);
    }
    return;
  } else {
    //
    // Only initialize the present port per the port map
    //
    switch(Phase) {
      case IIOInitPhase1:
        DEBUG ((EFI_D_INFO, "Initialize IIO:%d PCIE port:%d Func:%d...\n", i, PciAddress.Device,PciAddress.Function));
        PcieEarlyInit (PciAddress, Phase, IioGlobalData);
        PciePreInit    (PciAddress, IioGlobalData);
        PcieDeviceInit (PciAddress, IioGlobalData);
        PcieSlotInit   (PciAddress, Phase, IioGlobalData);
        break;

      case IIOInitPhase2:
        PcieSlotInit   (PciAddress, Phase, IioGlobalData);
        PcieLinkInit   (PciAddress, IioGlobalData);
        PcieMiscInit   (PciAddress, IioGlobalData);
        break;

      case IIOInitPhase3:
        DEBUG ((EFI_D_INFO, "PciPostInit port:%d Func%d...\n",PciAddress.Device,PciAddress.Function));
        PciePostInit   (PciAddress, IioGlobalData);
        break;
    }
  }
}
