/** @file
  This file is SampleCode for Intel PCH Common Platform Policy initialzation.

@copyright
  Copyright (c) 2013 - 2016 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement

**/

#include <Token.h> // AptioV server override

#include <Library/PchPlatformPolicyInitCommonLib.h>
#include <Protocol/PchPlatformPolicy.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include "Library/CpuPpmLib.h"
#include <PlatformInfo.h>


/**
  Initilize Intel USB Common Platform Policy

  @param[in] PchUsbConfig         Usb platform policy structure.

  @retval NONE
**/
VOID
InitPchUsbConfig (
  IN PCH_USB_CONFIG                     *PchUsbConfig
  )
{
  UINTN           PortIndex;
  UINT16          LpcDeviceId;
  PCH_SERIES      PchSeries;

  if (PchUsbConfig == NULL) {
    return;
  }

  PchSeries = GetPchSeries();
  LpcDeviceId = MmioRead16 (MmPciAddress (0, 0, PCI_DEVICE_NUMBER_PCH_LPC, PCI_FUNCTION_NUMBER_PCH_LPC, 0) + R_PCH_LPC_DEVICE_ID);

  //
  // EHCI Host Controller Enable/Disable
  //
  PchUsbConfig->Usb20Settings[0].Enable       = PCH_DEVICE_ENABLE;

  //For BDX-DE , EHCI 2 controller is not used and none of the EHCI 2 ports are connected in DE  so we need to disbale the EHCI2 controller 

 #ifdef GRANGEVILLE_FLAG
  PchUsbConfig->Usb20Settings[1].Enable       = PCH_DEVICE_DISABLE;
 #else
  PchUsbConfig->Usb20Settings[1].Enable		 = PCH_DEVICE_ENABLE;
 #endif

 
  //
  // Set to Enable if BIOS has its own xHCI driver
  //
  PchUsbConfig->Usb30Settings.PreBootSupport  = PCH_DEVICE_ENABLE;
  ///
  /// PCH BIOS Spec Rev 0.5.0 Section 13.1 xHCI controller options in Reference Code
  /// Please refer to Table 13-1 in PCH BIOS Spec for USB Port Operation with no xHCI
  /// pre-boot software.
  /// Please refer to Table 13-2 in PCH BIOS Spec for USB Port Operation with xHCI
  /// pre-boot software.
  ///
  /// The xHCI modes that available in BIOS are:
  /// Disabled   - forces only USB 2.0 to be supported in the OS. The xHCI controller is turned off
  ///              and hidden from the PCI space.
  /// Enabled    - allows USB 3.0 to be supported in the OS. The xHCI controller is turned on. The
  ///              shareable ports are routed to the xHCI controller. OS needs to provide drivers
  ///              to support USB 3.0.
  /// Auto       - This mode uses ACPI protocol to provide an option that enables the xHCI controller
  ///              and reroute USB ports via the _OSC ACPI method call. Note, this mode switch requires
  ///              special OS driver support for USB 3.0.
  /// Smart Auto - This mode is similar to Auto, but it adds the capability to route the ports to xHCI
  ///              or EHCI according to setting used in previous boots (for non-G3 boot) in the pre-boot
  ///              environment. This allows the use of USB 3.0 devices prior to OS boot. Note, this mode
  ///              switch requires special OS driver support for USB 3.0 and USB 3.0 software available
  ///              in the pre-boot enviroment.
  ///
  /// Manual Mode - For validation and experimental purposes only. Do not create setup option for end-user BIOS.
  ///
  /// Recommendations:
  ///  - If BIOS supports xHCI pre-boot driver then use Smart Auto mode as default
  ///  - If BIOS does not support xHCI pre-boot driver then use AUTO mode as default
  ///
  if (PchUsbConfig->Usb30Settings.PreBootSupport == PCH_DEVICE_ENABLE) {
    PchUsbConfig->Usb30Settings.Mode = PCH_XHCI_MODE_SMARTAUTO;
  } else {
    PchUsbConfig->Usb30Settings.Mode = PCH_XHCI_MODE_AUTO;
  }

  //
  // Manual Mode is for validation and experimental purposes only.
  // Do not create setup option for end-user BIOS.
  //
  PchUsbConfig->Usb30Settings.ManualMode = PCH_DEVICE_DISABLE;

  //
  // XhciIdleL1 can be set to disable for LPT-LP Ax stepping to workaround USB3 hot plug will fail after 1 hot plug removal. 
  //
  PchUsbConfig->Usb30Settings.XhciIdleL1 = PCH_DEVICE_ENABLE;

  //
  // Btcg is for enabling/disabling trunk clock gating.
  //
  PchUsbConfig->Usb30Settings.Btcg = PCH_DEVICE_DISABLE;

  for (PortIndex = 0; PortIndex < GetPchUsbMaxPhysicalPortNum (); PortIndex++) {
    PchUsbConfig->Usb30Settings.ManualModeUsb20PerPinRoute[PortIndex]   = 0;
  }

  for (PortIndex = 0; PortIndex < GetPchXhciMaxUsb3PortNum (); PortIndex++) {
    PchUsbConfig->Usb30Settings.ManualModeUsb30PerPinEnable[PortIndex]  = PCH_DEVICE_DISABLE;
  }

  //
  // Use by AMT/MEBx to enable USB-R support.
  //
  PchUsbConfig->Ehci1Usbr                 = PCH_DEVICE_DISABLE;
  PchUsbConfig->Ehci2Usbr                 = PCH_DEVICE_DISABLE;

  //
  // UsbPrecondition = Enable , Force USB Init happen in PEI as part of 2Sec Fast Boot bios optimization
  // UsbPrecondition = Disable, USB Init happen in DXE just like traditionally where it happen.
  //
  PchUsbConfig->UsbPrecondition           = PCH_DEVICE_DISABLE;

  //
  // USB Per-Port Control is use to Enable/Disable individual port.
  //
  PchUsbConfig->UsbPerPortCtl           = PCH_DEVICE_DISABLE;

  PchUsbConfig->PortSettings[0].Enable  = PCH_DEVICE_ENABLE;
  PchUsbConfig->PortSettings[1].Enable  = PCH_DEVICE_ENABLE;
  PchUsbConfig->PortSettings[2].Enable  = PCH_DEVICE_ENABLE;
  PchUsbConfig->PortSettings[3].Enable  = PCH_DEVICE_ENABLE;
  PchUsbConfig->PortSettings[4].Enable  = PCH_DEVICE_ENABLE;
  PchUsbConfig->PortSettings[5].Enable  = PCH_DEVICE_ENABLE;
  PchUsbConfig->PortSettings[6].Enable  = PCH_DEVICE_ENABLE;
  PchUsbConfig->PortSettings[7].Enable  = PCH_DEVICE_ENABLE;
  PchUsbConfig->PortSettings[8].Enable  = PCH_DEVICE_ENABLE;
  PchUsbConfig->PortSettings[9].Enable  = PCH_DEVICE_ENABLE;
  PchUsbConfig->PortSettings[10].Enable = PCH_DEVICE_ENABLE;
  PchUsbConfig->PortSettings[11].Enable = PCH_DEVICE_ENABLE;
  PchUsbConfig->PortSettings[12].Enable = PCH_DEVICE_ENABLE;
  PchUsbConfig->PortSettings[13].Enable = PCH_DEVICE_ENABLE;

  PchUsbConfig->Port30Settings[0].Enable  = PCH_DEVICE_ENABLE;
  PchUsbConfig->Port30Settings[1].Enable  = PCH_DEVICE_ENABLE;
  PchUsbConfig->Port30Settings[2].Enable  = PCH_DEVICE_ENABLE;
  PchUsbConfig->Port30Settings[3].Enable  = PCH_DEVICE_ENABLE;
  PchUsbConfig->Port30Settings[4].Enable  = PCH_DEVICE_ENABLE;
  PchUsbConfig->Port30Settings[5].Enable  = PCH_DEVICE_ENABLE;
  //
  // USB Port Over Current Pins mapping, please set as per board layout.
  //
  PchUsbConfig->Usb20OverCurrentPins[ 0] = PchUsbOverCurrentPin0;
  PchUsbConfig->Usb20OverCurrentPins[ 1] = PchUsbOverCurrentPin0;
  PchUsbConfig->Usb20OverCurrentPins[ 2] = PchUsbOverCurrentPin1;
  PchUsbConfig->Usb20OverCurrentPins[ 3] = PchUsbOverCurrentPin1;
  PchUsbConfig->Usb20OverCurrentPins[ 4] = PchUsbOverCurrentPin2;
  PchUsbConfig->Usb20OverCurrentPins[ 5] = PchUsbOverCurrentPin2;
  PchUsbConfig->Usb20OverCurrentPins[ 6] = PchUsbOverCurrentPin3;
  PchUsbConfig->Usb20OverCurrentPins[ 7] = PchUsbOverCurrentPin3;
  PchUsbConfig->Usb20OverCurrentPins[ 8] = PchUsbOverCurrentPin4;
  PchUsbConfig->Usb20OverCurrentPins[ 9] = PchUsbOverCurrentPin4;
  PchUsbConfig->Usb20OverCurrentPins[10] = PchUsbOverCurrentPin5;
  PchUsbConfig->Usb20OverCurrentPins[11] = PchUsbOverCurrentPin5;
  PchUsbConfig->Usb20OverCurrentPins[12] = PchUsbOverCurrentPin6;
  PchUsbConfig->Usb20OverCurrentPins[13] = PchUsbOverCurrentPin6;

  PchUsbConfig->Usb30OverCurrentPins[0]  = PchUsbOverCurrentPin0;
  PchUsbConfig->Usb30OverCurrentPins[1]  = PchUsbOverCurrentPin0;
  PchUsbConfig->Usb30OverCurrentPins[2]  = PchUsbOverCurrentPin1;
  PchUsbConfig->Usb30OverCurrentPins[3]  = PchUsbOverCurrentPin1;
  PchUsbConfig->Usb30OverCurrentPins[4]  = PchUsbOverCurrentPin2;
  PchUsbConfig->Usb30OverCurrentPins[5]  = PchUsbOverCurrentPin2;

  //
  // USB 2.0 D+/D- trace length in inchs*10 or 1000mils/10 measurement eg. 12.3" = 0x123
  // Please set as per board layout.
  //
  PchUsbConfig->PortSettings[ 0].Usb20PortLength = 0x100;
  PchUsbConfig->PortSettings[ 1].Usb20PortLength = 0x100;
  PchUsbConfig->PortSettings[ 2].Usb20PortLength = 0x100;
  PchUsbConfig->PortSettings[ 3].Usb20PortLength = 0x100;
  PchUsbConfig->PortSettings[ 4].Usb20PortLength = 0x100;
  PchUsbConfig->PortSettings[ 5].Usb20PortLength = 0x100;
  PchUsbConfig->PortSettings[ 6].Usb20PortLength = 0x100;
  PchUsbConfig->PortSettings[ 7].Usb20PortLength = 0x100;
  PchUsbConfig->PortSettings[ 8].Usb20PortLength = 0x100;
  PchUsbConfig->PortSettings[ 9].Usb20PortLength = 0x100;
  PchUsbConfig->PortSettings[10].Usb20PortLength = 0x100;
  PchUsbConfig->PortSettings[11].Usb20PortLength = 0x100;
  PchUsbConfig->PortSettings[12].Usb20PortLength = 0x100;
  PchUsbConfig->PortSettings[13].Usb20PortLength = 0x100;

  //
  // Port Location
  //
  PchUsbConfig->PortSettings[ 0].Location = PchUsbPortLocationFrontPanel;
  PchUsbConfig->PortSettings[ 1].Location = PchUsbPortLocationFrontPanel;
  PchUsbConfig->PortSettings[ 2].Location = PchUsbPortLocationFrontPanel;
  PchUsbConfig->PortSettings[ 3].Location = PchUsbPortLocationFrontPanel;
  PchUsbConfig->PortSettings[ 4].Location = PchUsbPortLocationFrontPanel;
  PchUsbConfig->PortSettings[ 5].Location = PchUsbPortLocationFrontPanel;
  PchUsbConfig->PortSettings[ 6].Location = PchUsbPortLocationFrontPanel;
  PchUsbConfig->PortSettings[ 7].Location = PchUsbPortLocationFrontPanel;
  PchUsbConfig->PortSettings[ 8].Location = PchUsbPortLocationFrontPanel;
  PchUsbConfig->PortSettings[ 9].Location = PchUsbPortLocationFrontPanel;
  PchUsbConfig->PortSettings[10].Location = PchUsbPortLocationFrontPanel;
  PchUsbConfig->PortSettings[11].Location = PchUsbPortLocationFrontPanel;
  PchUsbConfig->PortSettings[12].Location = PchUsbPortLocationFrontPanel;
  PchUsbConfig->PortSettings[13].Location = PchUsbPortLocationFrontPanel;

  return;
}


VOID
UpdatePchUsbConfig (
  IN PCH_USB_CONFIG            *PchUsbConfig,
  IN SYSTEM_CONFIGURATION      *SetupVariables,
  IN UINT8                     BoardId,
  IN EFI_PLATFORM_INFO         *PlatformInfoHobData
  )
/*++

Routine Description:

  This function performs PCH USB Platform Policy initialzation

Arguments:
  PchUsbConfig                    Pointer to PCH_USB_CONFIG data buffer
  SetupVariables                  Pointer to Setup variable
  BoardType                       BoardType specified

Returns:

--*/
{
  VOID *                    Usb20OverCurrentMappings;
  VOID *                    Usb30OverCurrentMappings;
  VOID *                    UsbPortLengthTable;
  VOID *                    UsbEyeDiagramParams;

  UINTN                              PortIndex;

#ifdef GRANGEVILLE_FLAG


  PCH_USB_OVERCURRENT_PIN            BeverlyCoveCRBUsb20OverCurrentMappings[LPTH_USB_MAX_PHYSICAL_PORTS] = {	 
                                      PchUsbOverCurrentPinSkip,       //Port00: Not used                                         ,no OCn#
                                      PchUsbOverCurrentPinSkip,       //Port01: Not used                                         ,no OCn#
                                      PchUsbOverCurrentPinSkip,       //Port02: Not used                                         ,no OCn#
                                      PchUsbOverCurrentPinSkip,       //Port03: Not used                                         ,no OCn#
                                      PchUsbOverCurrentPinSkip,       //Port04: Not used                                         ,no OCn#
                                      PchUsbOverCurrentPinSkip,       //Port05: Not used                                         ,no OCn#
                                      PchUsbOverCurrentPinSkip,       //Port06: Not used                                         ,no OCn#
                                      PchUsbOverCurrentPinSkip,       //Port07: Not used                                         ,no OCn#
                                      PchUsbOverCurrentPinSkip,       //Port08: Not used                                         ,no OCn#
                                      PchUsbOverCurrentPinSkip,       //Port09: Not used                                         ,no OCn#
                                      PchUsbOverCurrentPinSkip,       //Port10: Not used                                         ,no OCn#
                                      PchUsbOverCurrentPinSkip,       //Port11: Not used                                         ,no OCn#
                                      PchUsbOverCurrentPinSkip,       //Port12: Not used                                         ,no OCn#
                                      PchUsbOverCurrentPinSkip        //Port13: Not used                                         ,no OCn#
                                    };

 PCH_USB_OVERCURRENT_PIN             BeverlyCoveCRBUsb30OverCurrentMappings[LPTH_XHCI_MAX_USB3_PORTS] = {	
                                      PchUsbOverCurrentPinSkip,       //Port00: Not used                                        ,no OCn#
                                      PchUsbOverCurrentPinSkip,       //Port01: Not used                                        ,no OCn#
                                      PchUsbOverCurrentPinSkip,       //Port02: Not used                                        ,no OCn#
                                      PchUsbOverCurrentPinSkip,       //Port03: Not used                                        ,no OCn#
                                      PchUsbOverCurrentPinSkip,       //Port04: Not used                                        ,no OCn#
                                      PchUsbOverCurrentPinSkip        //Port05: Not used                                        ,no OCn#
                                    };

  PCH_USB_OVERCURRENT_PIN            StorageBoardsUsb20OverCurrentMappings[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                      PchUsbOverCurrentPin0,      //Port00: Back Panel                      ,OC0#
                                      PchUsbOverCurrentPin0,      //Port01: Back Panel                      ,OC0#
                                      PchUsbOverCurrentPin0,      //Port02: Internal 2x10, Debug_A          ,OC1#
                                      PchUsbOverCurrentPin0,      //Port03: Back Panel                      ,OC0#
                                      PchUsbOverCurrentPinSkip,   //Port04: BMC1.0/Internal 2x5 Debug_B     ,no OCn#
                                      PchUsbOverCurrentPinSkip,      //Port05: Internal 2x5_A                  ,OC2#
                                      PchUsbOverCurrentPinSkip,      //Port06: Internal 2x5_B                  ,OC3# 
                                      PchUsbOverCurrentPinSkip,      //Port07: Internal 2x5_b                  ,OC3#
                                      PchUsbOverCurrentPinSkip,   //Port08: eUSB                            ,no OCn#
                                      PchUsbOverCurrentPinSkip,      //Port09: Internal 2x5_A                  ,OC4#
                                      PchUsbOverCurrentPinSkip,      //Port10: Front Panel (J147 2x10)         ,OC5#
                                      PchUsbOverCurrentPinSkip,      //Port11: Int type A                      ,OC6#
                                      PchUsbOverCurrentPinSkip,   //Port12: BMC2.0/Internal 2x5 Debug_B     ,no OCn#
                                      PchUsbOverCurrentPinSkip       //Port13: Front Panel                     ,OC5#
                                   }; 

  PCH_USB_OVERCURRENT_PIN            StorageBoardsUsb30OverCurrentMappings[LPTH_XHCI_MAX_USB3_PORTS] = {    
                                      PchUsbOverCurrentPin1,      //Port00: Back Panel (J230 1st of 3Stack) ,OC0#
                                      PchUsbOverCurrentPin1,      //Port01: Front Panel                     ,OC5#
                                      PchUsbOverCurrentPinSkip,      //Port02: Back Panel (J230 3rd of 3Stack) ,OC0#
                                      PchUsbOverCurrentPinSkip,      //Port03: Internal 2x10 Debug_A           ,OC1#
                                      PchUsbOverCurrentPinSkip,      //Port04: Front Panel (J147 2x10)         ,OC5#
                                      PchUsbOverCurrentPinSkip      //Port05: Back Panel                      ,OC5#
                                   };
#else

  PCH_USB_OVERCURRENT_PIN            PlatformSRPOverCurrentMappings[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                      PchUsbOverCurrentPin0,      //Port00: Zepher        ,OC0#                        
                                      PchUsbOverCurrentPin1,      //Port01: Read Connector,OC1#                        
                                      PchUsbOverCurrentPinSkip,   //Port02: User bay      ,OC0#
                                      PchUsbOverCurrentPinSkip,   //Port03: iBMC USB 1.1  ,no OCn#                     
                                      PchUsbOverCurrentPinSkip,   //Port04: NONE          ,no OCn#                                  
                                      PchUsbOverCurrentPin2,      //Port05: Read Connector,OC2#                       
                                      PchUsbOverCurrentPin1,      //Port06: Read Connector,OC1#                                              
                                      PchUsbOverCurrentPin2,      //Port07: Read Connector,OC2#                                              
                                      PchUsbOverCurrentPinSkip,   //Port08: NONE          ,no OCn#                                                         
                                      PchUsbOverCurrentPinSkip,   //Port09: NONE          ,no OCn#                                                         
                                      PchUsbOverCurrentPinSkip,   //Port10: iBMC USB 2.0  ,no OCn#                                          
                                      PchUsbOverCurrentPin4,      //Port11: Front Panel   ,OC4#                    
                                      PchUsbOverCurrentPinSkip,   //Port12: NONE          ,no OCn#                                  
                                      PchUsbOverCurrentPin4      //Port13: Front Panel   ,OC4#                    
                                   };                                          
          
  PCH_USB_OVERCURRENT_PIN            MayanSRPUsb20OverCurrentMappings[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                      PchUsbOverCurrentPin0,      //Port00: Back Panel                      ,OC0#
                                      PchUsbOverCurrentPin0,      //Port01: Back Panel                      ,OC0#
                                      PchUsbOverCurrentPin1,      //Port02: Internal 2x10, Debug_A          ,OC1#
                                      PchUsbOverCurrentPin0,      //Port03: Back Panel                      ,OC0#
                                      PchUsbOverCurrentPinSkip,   //Port04: BMC1.0/Internal 2x5 Debug_B     ,no OCn#
                                      PchUsbOverCurrentPin2,      //Port05: Internal 2x5_A                  ,OC2#
                                      PchUsbOverCurrentPin3,      //Port06: Internal 2x5_B                  ,OC3# 
                                      PchUsbOverCurrentPin3,      //Port07: Internal 2x5_b                  ,OC3#
                                      PchUsbOverCurrentPinSkip,   //Port08: eUSB                            ,no OCn#
                                      PchUsbOverCurrentPin4,      //Port09: Internal 2x5_A                  ,OC4#
                                      PchUsbOverCurrentPin5,      //Port10: Front Panel (J147 2x10)         ,OC5#
                                      PchUsbOverCurrentPin6,      //Port11: Int type A                      ,OC6#
                                      PchUsbOverCurrentPinSkip,   //Port12: BMC2.0/Internal 2x5 Debug_B     ,no OCn#
                                      PchUsbOverCurrentPin5       //Port13: Front Panel                     ,OC5#
                                   }; 

PCH_USB_OVERCURRENT_PIN            EmeraldPointUsb20OverCurrentMappings[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                   PchUsbOverCurrentPinSkip,      //Port00: Back Panel                      ,OC0#
                                   PchUsbOverCurrentPin0,      //Port01: Back Panel                      ,OC0#
                                   PchUsbOverCurrentPinSkip,      //Port02: Internal 2x10, Debug_A          ,OC1#
                                   PchUsbOverCurrentPinSkip,      //Port03: Back Panel                      ,OC0#
                                   PchUsbOverCurrentPinSkip,   //Port04: BMC1.0/Internal 2x5 Debug_B     ,no OCn#
                                   PchUsbOverCurrentPinSkip,      //Port05: Internal 2x5_A                  ,OC2#
                                   PchUsbOverCurrentPinSkip,      //Port06: Internal 2x5_B                  ,OC3# 
                                   PchUsbOverCurrentPin1,      //Port07: Internal 2x5_b                  ,OC3#
                                   PchUsbOverCurrentPinSkip,   //Port08: eUSB                            ,no OCn#
                                   PchUsbOverCurrentPinSkip,      //Port09: Internal 2x5_A                  ,OC4#
                                   PchUsbOverCurrentPinSkip,      //Port10: Front Panel (J147 2x10)         ,OC5#
                                   PchUsbOverCurrentPinSkip,      //Port11: Int type A                      ,OC6#
                                   PchUsbOverCurrentPinSkip,   //Port12: BMC2.0/Internal 2x5 Debug_B     ,no OCn#
                                   PchUsbOverCurrentPinSkip       //Port13: Front Panel                     ,OC5#
                                }; 

  PCH_USB_OVERCURRENT_PIN            MayanSRPUsb30OverCurrentMappings[LPTH_XHCI_MAX_USB3_PORTS] = {    
                                      PchUsbOverCurrentPin0,      //Port00: Back Panel (J230 1st of 3Stack) ,OC0#
                                      PchUsbOverCurrentPin5,      //Port01: Front Panel                     ,OC5#
                                      PchUsbOverCurrentPin0,      //Port02: Back Panel (J230 3rd of 3Stack) ,OC0#
                                      PchUsbOverCurrentPin1,      //Port03: Internal 2x10 Debug_A           ,OC1#
                                      PchUsbOverCurrentPin5,      //Port04: Front Panel (J147 2x10)         ,OC5#
                                      PchUsbOverCurrentPin0,      //Port05: Back Panel                      ,OC0#
                                   }; 

PCH_USB_OVERCURRENT_PIN            EmeraldPointUsb30OverCurrentMappings[LPTH_XHCI_MAX_USB3_PORTS] = {    
                                   PchUsbOverCurrentPinSkip, //PchUsbOverCurrentPin0,      //Port00: Back Panel (J230 1st of 3Stack) ,OC0#
                                   PchUsbOverCurrentPinSkip,      //Port01: Front Panel                     ,OC5#
                                   PchUsbOverCurrentPin0,      //Port02: Back Panel (J230 3rd of 3Stack) ,OC0#
                                   PchUsbOverCurrentPinSkip,      //Port03: Internal 2x10 Debug_A           ,OC1#
                                   PchUsbOverCurrentPinSkip,      //Port04: Front Panel (J147 2x10)         ,OC5#
                                   PchUsbOverCurrentPin1,      //Port05: Back Panel                      ,OC5#
                                }; 

  PCH_USB_OVERCURRENT_PIN            WildcatUsb20OverCurrentMappings[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                      PchUsbOverCurrentPin0,      //Port00: Back Panel                      ,OC0#
                                      PchUsbOverCurrentPin0,      //Port01: Back Panel                      ,OC0#
                                      PchUsbOverCurrentPin1,      //Port02: Internal 2x10, Debug_A          ,OC1#
                                      PchUsbOverCurrentPinSkip,   //Port03: Back Panel                      ,
                                      PchUsbOverCurrentPinSkip,   //Port04: BMC1.0/Internal 2x5 Debug_B     ,no OCn#
                                      PchUsbOverCurrentPin2,      //Port05: Internal 2x5_A                  ,OC2#
                                      PchUsbOverCurrentPin2,      //Port06: Internal 2x5_B                  ,
                                      PchUsbOverCurrentPinSkip,   //Port07: Internal 2x5_b                  ,
                                      PchUsbOverCurrentPinSkip,   //Port08: eUSB                            ,no OCn#
                                      PchUsbOverCurrentPin6,      //Port09: Internal 2x5_A                  ,
                                      PchUsbOverCurrentPin5,      //Port10: Front Panel (J147 2x10)         ,OC5#
                                      PchUsbOverCurrentPinSkip,   //Port11: Int type A                      ,
                                      PchUsbOverCurrentPinSkip,   //Port12: BMC2.0/Internal 2x5 Debug_B     ,no OCn#
                                      PchUsbOverCurrentPin5       //Port13: Front Panel                     ,OC5#
                                   }; 



  PCH_USB_OVERCURRENT_PIN            WildcatUsb30OverCurrentMappings[LPTH_XHCI_MAX_USB3_PORTS] = {    
                                      PchUsbOverCurrentPinSkip,   //Port00: Back Panel (J230 1st of 3Stack) ,
                                      PchUsbOverCurrentPin5,      //Port01: Front Panel                     ,OC5#
                                      PchUsbOverCurrentPin0,      //Port02: Back Panel (J230 3rd of 3Stack) ,OC0#
                                      PchUsbOverCurrentPin1,      //Port03: Internal 2x10 Debug_A           ,OC1#
                                      PchUsbOverCurrentPin5,      //Port04: Front Panel (J147 2x10)         ,OC5#
                                      PchUsbOverCurrentPin0,      //Port05: Back Panel                      ,OC0#
                                   }; 

   


  PCH_USB_OVERCURRENT_PIN            AztecSRPUsb20OverCurrentMappings[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                      PchUsbOverCurrentPin0,      //Port00: Back Panel A                    ,OC0#
                                      PchUsbOverCurrentPin0,      //Port01: Back Panel A                    ,OC0#
                                      PchUsbOverCurrentPin1,      //Port02: Internal 2x10, Debug_A          ,OC1#
                                      PchUsbOverCurrentPin2,      //Port03: Back Panel B                    ,OC2#
                                      PchUsbOverCurrentPin2,      //Port04: Back Panel B                    ,OC2#
                                      PchUsbOverCurrentPinSkip,   //Port05: Not Used                        ,no OCn#
                                      PchUsbOverCurrentPinSkip,   //Port06: Not Used                        ,no OCn# 
                                      PchUsbOverCurrentPinSkip,   //Port07: BMC1.0                          ,no OCn#
                                      PchUsbOverCurrentPin5,      //Port08: Int2FP A                        ,OC3#
                                      PchUsbOverCurrentPin4,      //Port09: INT_Type_A_1                    ,OC4#
                                      PchUsbOverCurrentPin4,      //Port10: INT_Type_A_2                    ,OC4#
                                      PchUsbOverCurrentPinSkip,   //Port11: eUSB                            ,no OCn#
                                      PchUsbOverCurrentPinSkip,   //Port12: BMC2.0                          ,no OCn#
                                      PchUsbOverCurrentPin5       //Port13: Int2FP_A                        ,OC3#
                                   }; 

  PCH_USB_OVERCURRENT_PIN            AztecSTHIUsb20OverCurrentMappings[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                      PchUsbOverCurrentPinSkip,      //Port00: Back Panel A                    ,OC0#
                                      PchUsbOverCurrentPinSkip,      //Port01: Back Panel A                    ,OC0#
                                      PchUsbOverCurrentPinSkip,      //Port02: Internal 2x10, Debug_A          ,OC1#
                                      PchUsbOverCurrentPin2,      //Port03: Back Panel B                    ,OC2#
                                      PchUsbOverCurrentPin2,      //Port04: Back Panel B                    ,OC2#
                                      PchUsbOverCurrentPinSkip,   //Port05: Not Used                        ,no OCn#
                                      PchUsbOverCurrentPinSkip,   //Port06: Not Used                        ,no OCn# 
                                      PchUsbOverCurrentPinSkip,   //Port07: BMC1.0                          ,no OCn#
                                      PchUsbOverCurrentPinSkip,      //Port08: Int2FP A                        ,OC3#
                                      PchUsbOverCurrentPinSkip,      //Port09: INT_Type_A_1                    ,OC4#
                                      PchUsbOverCurrentPinSkip,      //Port10: INT_Type_A_2                    ,OC4#
                                      PchUsbOverCurrentPinSkip,   //Port11: eUSB                            ,no OCn#
                                      PchUsbOverCurrentPinSkip,   //Port12: BMC2.0                          ,no OCn#
                                      PchUsbOverCurrentPinSkip       //Port13: Int2FP_A                        ,OC3#
                                   }; 

  PCH_USB_OVERCURRENT_PIN            AztecSRPUsb30OverCurrentMappings[LPTH_XHCI_MAX_USB3_PORTS] = {    
                                      PchUsbOverCurrentPin3,      //Port00: Int2Fp_A                        ,OC3#
                                      PchUsbOverCurrentPin3,      //Port01: Int2Fp_A                        ,OC3#
                                      PchUsbOverCurrentPinSkip,   //Port02: Back Panel (J230 3rd of 3Stack) ,no OCn#
                                      PchUsbOverCurrentPin1,      //Port03: Internal 2x10 Debug_A           ,OC1#
                                      PchUsbOverCurrentPin0,      //Port04: Back Panel A                    ,OC0#
                                      PchUsbOverCurrentPin0,      //Port05: Back Panel A                    ,OC0#
                                   }; 
  PCH_USB_OVERCURRENT_PIN            AztecSTHIUsb30OverCurrentMappings[LPTH_XHCI_MAX_USB3_PORTS] = {    
                                      PchUsbOverCurrentPinSkip,      //Port00: Int2Fp_A                        ,OC3#
                                      PchUsbOverCurrentPinSkip,      //Port01: Int2Fp_A                        ,OC3#
                                      PchUsbOverCurrentPinSkip,   //Port02: Back Panel (J230 3rd of 3Stack) ,no OCn#
                                      PchUsbOverCurrentPinSkip,      //Port03: Internal 2x10 Debug_A           ,OC1#
                                      PchUsbOverCurrentPinSkip,      //Port04: Back Panel A                    ,OC0#
                                      PchUsbOverCurrentPinSkip,      //Port05: Back Panel A                    ,OC0#
                                   }; 


  PCH_USB_OVERCURRENT_PIN            YakimaSRPUsb20OverCurrentMappings[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                      PchUsbOverCurrentPin0,      //Port00: Back Panel A                    ,OC0#
                                      PchUsbOverCurrentPin0,      //Port01: Back Panel A(BIOS debug)        ,OC0#
                                      PchUsbOverCurrentPin1,      //Port02: Internal Debug (STHI SKU)       ,OC1#
                                      PchUsbOverCurrentPin2,      //Port03: Back Panel B                    ,OC2#
                                      PchUsbOverCurrentPin2,      //Port04: Back Panel B                    ,OC2#
                                      PchUsbOverCurrentPinSkip,   //Port05: Not Used                        ,non OCn#
                                      PchUsbOverCurrentPinSkip,   //Port06: Not Used                        ,non OCn#
                                      PchUsbOverCurrentPinSkip,   //Port07: BMC USB1.1                      ,non OCn#
                                      PchUsbOverCurrentPin5,      //Port08: Front Panel                     ,OC5#
                                      PchUsbOverCurrentPin4,      //Port09: Internal Type-A                 ,OC4#
                                      PchUsbOverCurrentPin4,      //Port10: Internal Type-A                 ,OC4#
                                      PchUsbOverCurrentPinSkip,   //Port11: eUSB/SSD                        ,non OCn#
                                      PchUsbOverCurrentPinSkip,   //Port12: BMC USB2.0                      ,non OCn#
                                      PchUsbOverCurrentPin5       //Port13: Front Panel                     ,OC5#
                                   }; 

  PCH_USB_OVERCURRENT_PIN            YakimaSRPUsb30OverCurrentMappings[LPTH_XHCI_MAX_USB3_PORTS] = {    
                                      PchUsbOverCurrentPin5,      //Port00: Front Panel                     ,OC5#
                                      PchUsbOverCurrentPin5,      //Port01: Front Panel                     ,OC5#
                                      PchUsbOverCurrentPinSkip,   //Port02: Not Used                        ,non OCn#
                                      PchUsbOverCurrentPin1,      //Port03: Internal Debug(STHI SKU)        ,OC1#
                                      PchUsbOverCurrentPin0,      //Port04: Back Panel A                    ,OC0#
                                      PchUsbOverCurrentPin0,      //Port05: Back Panel A (BIOS Debug)       ,OC0#
                                   };

  PCH_USB_OVERCURRENT_PIN            IncaSRPUsb20OverCurrentMappings[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                      PchUsbOverCurrentPinSkip,   //Port00: eUSB/SSD                        ,no OCn#
                                      PchUsbOverCurrentPin1,      //Port01: Rear USB 2                      ,OC1#
                                      PchUsbOverCurrentPin0,      //Port02: INT Type A                      ,OC0#
                                      PchUsbOverCurrentPinSkip,   //Port03: BMC1.1                          ,no OCn#
                                      PchUsbOverCurrentPin1,      //Port04: Rear USB2                       ,OC1#
                                      PchUsbOverCurrentPinSkip,   //Port05: Not Connected                   ,no OCn#
                                      PchUsbOverCurrentPin2,      //Port06: Front Panel                     ,OC2# 
                                      PchUsbOverCurrentPin2,      //Port07: Front Panel                     ,OC2# 
                                      PchUsbOverCurrentPinSkip,   //Port08: Not Connected                   ,no OCn#
                                      PchUsbOverCurrentPinSkip,   //Port09: Not Connected                   ,no OCn#
                                      PchUsbOverCurrentPinSkip,   //Port10: BMC 2.0                         ,no OCn#
                                      PchUsbOverCurrentPin5,      //Port11: INT Type A                      ,OC5#
                                      PchUsbOverCurrentPin4,      //Port12: Rear USB 1                      ,OC4#
                                      PchUsbOverCurrentPin4       //Port13: Rear USB 1                      ,OC4#
                                   }; 

  PCH_USB_OVERCURRENT_PIN            IncaSRPUsb30OverCurrentMappings[LPTH_XHCI_MAX_USB3_PORTS] = {    
                                      PchUsbOverCurrentPin1,      //Port00: Rear USB 2                      ,OC1#
                                      PchUsbOverCurrentPin1,      //Port01: Rear USB 2                      ,OC1#
                                      PchUsbOverCurrentPin4,      //Port02: Rear USB 1                      ,OC4#
                                      PchUsbOverCurrentPin4,      //Port03: Rear USB 1                      ,OC4#
                                      PchUsbOverCurrentPin2,      //Port04: Front Panel                     ,OC2#
                                      PchUsbOverCurrentPin2,      //Port05: Front Panel                     ,OC2#
                                   }; 

  PCH_USB_OVERCURRENT_PIN            HedtUsb200OverCurrentMappings[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                      PchUsbOverCurrentPin0,      // Port00: Back panel/DP stack ,OC0#
                                      PchUsbOverCurrentPin0,      // Port01: Back panel/DP stack ,OC0#
                                      PchUsbOverCurrentPin1,      // Port02: Front panel stack ,OC1#
                                      PchUsbOverCurrentPin1,      // Port03: Front panel stack ,OC1#
                                      PchUsbOverCurrentPin2,      // Port04: Front panel hdr ,OC2#
                                      PchUsbOverCurrentPin2,      // Port05: Front panel hdr ,OC2#
                                      PchUsbOverCurrentPin3,      // Port06: Front panel hdr ,OC3#
                                      PchUsbOverCurrentPin3,      // Port07: Front panel hdr ,OC3#
                                      PchUsbOverCurrentPin4,      // Port08: Back panel stack ,OC4#
                                      PchUsbOverCurrentPin4,      // Port09: Back panel stack ,OC4#
                                      PchUsbOverCurrentPin5,      // Port10: Back panel stack ,OC5#
                                      PchUsbOverCurrentPin5,      // Port11: Back panel stack ,OC5#
                                      PchUsbOverCurrentPin6,      // Port12: Front panel hdr ,OC6#
                                      PchUsbOverCurrentPin6       // Port13: Front panel hdr ,OC6#
                                   };                                          
          
  PCH_USB_OVERCURRENT_PIN            HedtUsb30OverCurrentMappings[LPTH_XHCI_MAX_USB3_PORTS] = {    
                                      PchUsbOverCurrentPin0,      // Port00: Back panel stack ,OC0#
                                      PchUsbOverCurrentPin0,      // Port01: Back panel stack ,OC0#
                                      PchUsbOverCurrentPin1,      // Port02: Front panel stack ,OC1#
                                      PchUsbOverCurrentPin1,      // Port03: Front panel stack ,OC1#
                                      PchUsbOverCurrentPin2,      // Port04: Front Panel hdr, OC2#
                                      PchUsbOverCurrentPin2,      // Port05: Front panel hdr ,OC2#
                                   }; 

  UINT16                            PlatformSRPPortLength[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                      0x48,     //Port00: Zepher        ,make it biggest amplitude
                                      0x142,    //Port01: Read Connector,
                                      0x20,     //Port02: User bay      ,make it biggest amplitude
                                      0x90,     //Port03: iBMC USB 1.1  ,
                                      0x00,     //Port04: NONE          ,make it smallest amplitude
                                      0x149,    //Port05: Read Connector,
                                      0x136,    //Port06: Read Connector,
                                      0x148,    //Port07: Read Connector,
                                      0x00,     //Port08: NONE          ,make it smallest amplitude
                                      0x00,     //Port09: NONE          ,make it smallest amplitude
                                      0x81,     //Port10: iBMC USB 2.0  ,
                                      0x59,     //Port11: Front Panel   ,
                                      0x00,     //Port12: NONE          ,make it smallest amplitude
                                      0x56      //Port13: Front Panel   ,
                                   }; 
  
#endif

#ifdef GRANGEVILLE_FLAG


  UINT16                            BeverlyCoveCRBPortLength[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                      0x238,     //Not used
                                      0x159,     //Port01: Debug port        ,15.907"
                                      0x238,     //Not used
                                      0x238,     //Not used
                                      0x238,     //Not used
                                      0x238,     //Not used
                                      0x238,     //Not used
                                      0x238,     //Not used
                                      0x238,     //Not used
                                      0x238,     //Not used
                                      0x238,     //Not used
                                      0x238,     //Not used
                                      0x238,     //Not used
                                      0x101      //Not used
                                   };

#endif

  UINT16                            MayanSRPPortLength[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                      0x78,     //Port00: SSD (J2H1, mini2x5)             ,7.8"
                                      0x59,     //Port01: Back Panel(J230 1st of 3stack)  ,7.5"
                                      0x57,     //Port02: Floppy (J284)                   ,7.2"
                                      0x59,     //Port03: BMC1.0/Internal(J2 2x5)         ,7.3"
                                      0x75,     //Port04: Back Panel (J230 2nd of 3Stack) ,7.5"
                                      0x68,     //Port05: Back Panel (J230 3rd of 3Stack) ,7.5"
                                      0x76,     //Port06: Internal (J1C3 2x5)             ,6.1"
                                      0x79,     //Port07: Internal (J1C3 2x5)             ,6.1"
                                      0x64,     //Port08: Internal (J229 2x10)            ,2"
                                      0x63,     //Port09: Front Panel (J147 2x10)          ,8.2"
                                      0x114,     //Port10: Front Panel (J147 2x10)          ,8.2"
                                      0x73,     //Port11: Internal (J1 2x5)               ,6.7"
                                      0x115,     //Port12: Internal (J1 2x5)               ,6.7"
                                      0x101      //Port13: BMC2.0/Internal (J2 2x5)        ,7.3"
                                   };

#ifndef GRANGEVILLE_FLAG
UINT16                         EmeraldPointPortLength[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                       0x78,     //Port00: SSD (J2H1, mini2x5)             ,7.8"
                                       0x59,     //Port01: Back Panel(J230 1st of 3stack)  ,7.5"
                                       0x57,     //Port02: Floppy (J284)                   ,7.2"
                                       0x59,     //Port03: BMC1.0/Internal(J2 2x5)         ,7.3"
                                       0x75,     //Port04: Back Panel (J230 2nd of 3Stack) ,7.5"
                                       0x68,     //Port05: Back Panel (J230 3rd of 3Stack) ,7.5"
                                       0x76,     //Port06: Internal (J1C3 2x5)             ,6.1"
                                       0x59,     //Port07: Back Panel                      ,6.1"
                                       0x64,     //Port08: Internal (J229 2x10)            ,2"
                                       0x63,     //Port09: Front Panel (J147 2x10)          ,8.2"
                                       0x114,     //Port10: Front Panel (J147 2x10)          ,8.2"
                                       0x73,     //Port11: Internal (J1 2x5)               ,6.7"
                                       0x115,     //Port12: Internal (J1 2x5)               ,6.7"
                                       0x101      //Port13: BMC2.0/Internal (J2 2x5)        ,7.3"

                                    };



  UINT16                            WildcatPortLength[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                      0x48,     //Port00: SSD (J2H1, mini2x5)             ,
                                      0x142,    //Port01: Back Panel(J230 1st of 3stack)  ,
                                      0x20,     //Port02: Floppy (J284)                   ,
                                      0x90,     //Port03: BMC1.0/Internal(J2 2x5)         ,
                                      0x00,     //Port04: Back Panel (J230 2nd of 3Stack) ,
                                      0x149,    //Port05: Back Panel (J230 3rd of 3Stack) ,
                                      0x136,    //Port06: Internal (J1C3 2x5)             ,
                                      0x148,    //Port07: Internal (J1C3 2x5)             ,
                                      0x00,     //Port08: Internal (J229 2x10)            ,
                                      0x00,     //Port09: Front Panel (J147 2x10)         ,
                                      0x81,     //Port10: Front Panel (J147 2x10)         ,
                                      0x59,     //Port11: Internal (J1 2x5)               ,
                                      0x00,     //Port12: Internal (J1 2x5)               ,
                                      0x56      //Port13: BMC2.0/Internal (J2 2x5)        ,
                                   };
   

  UINT16                            HedtPortLength[LPTH_USB_MAX_PHYSICAL_PORTS] = {    
                                      0x116,     // Port00: Back panel/DP stack ,
                                      0x122,     // Port01: Back panel/DP stack ,
                                      0x041,     // Port02: Front panel stack ,
                                      0x028,     // Port03: Front panel stack ,
                                      0x040,     // Port04: Front panel hdr ,
                                      0x046,     // Port05: Front panel hdr ,
                                      0x035,     // Port06: Front panel hdr ,
                                      0x035,     // Port07: Front panel hdr ,
                                      0x137,     // Port08: Back panel stack ,
                                      0x140,     // Port09: Back panel stack ,
                                      0x128,     // Port10: Back panel stack ,
                                      0x090,     // Port11: Back panel stack ,
                                      0x040,     // Port12: Front panel hdr ,
                                      0x044      // Port13: Front panel hdr ,
                                   }; 
#endif
typedef struct {
  UINT8   Usb20EyeDiagramTuningParam1; // Set IOBP registers 0xE5004000 + (PortNumber * 0x100)[10:08] = {0,1,2,3,4,5,6,7}
  UINT8   Usb20EyeDiagramTuningParam2; // Set IOBP registers 0xE5004000 + (PortNumber * 0x100)[13:11] = {0,1,2,3,4,5,6,7}
  UINT8   Usb20EyeDiagramTuningParam3; // Set IOBP registers 0xE5004000 + (PortNumber * 0x100)[14] = {0,1}
} USB20_EYE_DIAGRAM_PARAMS;

#ifdef GRANGEVILLE_FLAG


 USB20_EYE_DIAGRAM_PARAMS           BeverlyCoveCRBUsb20EyeDiagramTuningParam[LPTH_USB_MAX_PHYSICAL_PORTS] = {
                                     {4, 2, 0},//Not used.
                                     {4, 4, 0},//Port01: PERPORTTXISET (BIT[10:08])= 5, PERPORTPETXISET (BIT[13:11])= 4, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Not used.
                                     {4, 2, 0},//Not used.
                                     {4, 4, 0},//Not used.
                                     {4, 2, 0},//Not used.
                                     {4, 3, 0},//Not used.
                                     {4, 3, 0},//Not used.
                                     {4, 2, 0},//Not used.
                                     {4, 2, 0},//Not used.
                                     {4, 3, 0},//Not used.
                                     {4, 2, 0},//Not used.
                                     {4, 3, 0},//Not used.
                                     {4, 3, 0},//Not used.
                                    };
#endif

 USB20_EYE_DIAGRAM_PARAMS           MayanSRPUsb20EyeDiagramTuningParam[LPTH_USB_MAX_PHYSICAL_PORTS] = {
                                     {4, 2, 0},//Port00: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port01: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port02: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port03: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 4, 0},//Port04: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 4, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port05: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 3, 0},//Port06: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 3, 0},//Port07: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port08: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port09: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 3, 0},//Port10: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port11: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 3, 0},//Port12: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 3, 0},//Port13: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                    };
#ifndef GRANGEVILLE_FLAG 

 USB20_EYE_DIAGRAM_PARAMS           AztecSRPUsb20EyeDiagramTuningParam[LPTH_USB_MAX_PHYSICAL_PORTS] = {
                                     {4, 4, 0},//Port00: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 4, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 4, 0},//Port01: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 4, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port02: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {5, 4, 0},//Port03: PERPORTTXISET (BIT[10:08])= 5, PERPORTPETXISET (BIT[13:11])= 4, PERPORTTXPEHALF (BIT[14])= 0
                                     {5, 4, 0},//Port04: PERPORTTXISET (BIT[10:08])= 5, PERPORTPETXISET (BIT[13:11])= 4, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port05: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port06: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port07: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port08: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port09: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port10: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {3, 2, 0},//Port11: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 3, 0},//Port12: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port13: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                    };

 USB20_EYE_DIAGRAM_PARAMS           AztecSTHIUsb20EyeDiagramTuningParam[LPTH_USB_MAX_PHYSICAL_PORTS] = {
                                     {4, 3, 0},//Port00: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 3, 0},//Port01: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 3, 0},//Port02: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 3, 0},//Port03: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 3, 0},//Port04: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 4, 0},//Port05: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 4, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 3, 0},//Port06: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 3, 0},//Port07: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 3, 0},//Port08: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 3, 0},//Port09: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 3, 0},//Port10: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 3, 0},//Port11: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 3, 0},//Port12: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 3, 0},//Port13: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                    };      
									
 USB20_EYE_DIAGRAM_PARAMS           HedtUsb20EyeDiagramTuningParam[LPTH_USB_MAX_PHYSICAL_PORTS] = {
                                     {4, 3, 0},//Port00: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 4, 0},//Port01: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 4, PERPORTTXPEHALF (BIT[14])= 0
                                     {3, 2, 0},//Port02: PERPORTTXISET (BIT[10:08])= 3, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {3, 2, 0},//Port03: PERPORTTXISET (BIT[10:08])= 3, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {3, 2, 0},//Port04: PERPORTTXISET (BIT[10:08])= 3, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {3, 2, 0},//Port05: PERPORTTXISET (BIT[10:08])= 3, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {3, 2, 0},//Port06: PERPORTTXISET (BIT[10:08])= 3, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {3, 2, 0},//Port07: PERPORTTXISET (BIT[10:08])= 3, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 4, 0},//Port08: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 4, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 4, 0},//Port09: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 4, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 4, 0},//Port10: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 4, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 4, 0},//Port11: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 4, PERPORTTXPEHALF (BIT[14])= 0
                                     {3, 2, 0},//Port12: PERPORTTXISET (BIT[10:08])= 3, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {3, 2, 0},//Port13: PERPORTTXISET (BIT[10:08])= 3, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                    };
 USB20_EYE_DIAGRAM_PARAMS           PlatformSRPUsb20EyeDiagramTuningParam[LPTH_USB_MAX_PHYSICAL_PORTS] = {
                                     {4, 2, 0},//Port00: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port01: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 4, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port02: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 4, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port03: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port04: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port05: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port06: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port07: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port08: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port09: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port10: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port11: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port12: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 3, PERPORTTXPEHALF (BIT[14])= 0
                                     {4, 2, 0},//Port13: PERPORTTXISET (BIT[10:08])= 4, PERPORTPETXISET (BIT[13:11])= 2, PERPORTTXPEHALF (BIT[14])= 0
                                   };

#endif

  PchUsbConfig->UsbPrecondition = SetupVariables->UsbPrecondition;

  //
  // USB Device 29 configuration
  //
  PchUsbConfig->Usb20Settings[0].Enable = SetupVariables->PchUsb20[0];
  //
  // USB Device 26 configuration
  //
  PchUsbConfig->Usb20Settings[1].Enable = SetupVariables->PchUsb20[1];

  if (SetupVariables->PchUsb30Mode == 4) {
    //
    // Ensure both EHCI must be Enabled when user manually route the USB pins
    //
    PchUsbConfig->Usb20Settings[0].Enable = 1;
    PchUsbConfig->Usb20Settings[1].Enable = 1;
  } else if (SetupVariables->PchUsb30Mode == 1) {
    //
    // AptioV server override Start: Fix for Debug Rx hang/not working issue while enabling xHCI mode.
    // Don't disable the EHCI#1 controller as it needs for Debug Rx working
    //
#if (AMIDEBUG_RX_SUPPORT || AMI_DEBUGGER_SUPPORT) == 1
    //
    // if XHCI Mode == Enabled, automatically enable EHCI#1.
    //
    PchUsbConfig->Usb20Settings[0].Enable = 1; //EHCI#1
    PchUsbConfig->Usb20Settings[1].Enable = 0; //EHCI#2
    
    //
    // AptioV server override End: Fix for Debug Rx hang/not working issue while enabling xHCI mode.
    // Don't disable the EHCI controllers as it needs for Debug Rx working
    //
#else
    //
    // if XHCI Mode == Enabled, automatically disable EHCI to save power.
    //
    PchUsbConfig->Usb20Settings[0].Enable = 0;
    PchUsbConfig->Usb20Settings[1].Enable = 0;
#endif
  }

  PchUsbConfig->UsbPerPortCtl = SetupVariables->PchUsbPerPortCtl;
  if (SetupVariables->PchUsbPerPortCtl != PCH_DEVICE_DISABLE) {
    for (PortIndex = 0; PortIndex < GetPchUsbMaxPhysicalPortNum (); PortIndex++) {
      PchUsbConfig->PortSettings[PortIndex].Enable = SetupVariables->PchUsbPort[PortIndex];
    }
	for (PortIndex = 0; PortIndex < GetPchXhciMaxUsb3PortNum (); PortIndex++) {
      PchUsbConfig->Port30Settings[PortIndex].Enable = SetupVariables->PchUsb30Port[PortIndex];
    }
  }

  //
  // xHCI (USB 3.0) related settings from setup variable
  //
  if(SetupVariables->Btcg == 1) {
    PchUsbConfig->Usb30Settings.Btcg = PCH_DEVICE_ENABLE;
  } else {
    PchUsbConfig->Usb30Settings.Btcg = PCH_DEVICE_DISABLE;
  }
  if (SetupVariables->PchUsb30Mode == 4) {
    PchUsbConfig->Usb30Settings.ManualMode = PCH_DEVICE_ENABLE;
  } else {
    PchUsbConfig->Usb30Settings.Mode = SetupVariables->PchUsb30Mode;
  }
  PchUsbConfig->Usb30Settings.XhciIdleL1 = SetupVariables->XhciIdleL1;

#ifdef PCH_SERVER_BIOS_FLAG  
  //
  // Sighting s4168449 - the s755 Workaround - set some chicken bits
  // This is to enable the WBG ES2 samples to avoid CATERRs
  // 
  if (SetupVariables->UsbS755WA == 1) {
    PchUsbConfig->Usb30Settings.XhciS755WA = 1; // Enable;
  } else {
    PchUsbConfig->Usb30Settings.XhciS755WA = 0; // Disable;
  }
#endif // PCH_SERVER_BIOS_FLAG

  //Hide MSI capability in XHCI. This is valid for HSX and BDX-DE less than V0 stepping.
  if (SetupVariables->XhciDisMSICapability == 1 && 
       (PlatformInfoHobData->CpuType == CPU_HASWELLSERVER || 
      (PlatformInfoHobData->CpuType == CPU_BDX_DE_SERVER && PlatformInfoHobData->CpuStepping < C0_REV_BDX))){
    PchUsbConfig->Usb30Settings.XhciDisMSICapability = 1; // Enable;
  } else {
    PchUsbConfig->Usb30Settings.XhciDisMSICapability = 0; // Disable;
  }


  //
  // Remove XHCI Pre-Boot Driver setup option selection from end-user view and automate loading of USB 3.0 BIOS driver based on XhciMode selection
  //  
  switch (SetupVariables->PchUsb30Mode) {
    case 0: // Disabled
      PchUsbConfig->Usb30Settings.PreBootSupport = 0;
      break;
    case 1: // Enabled
      PchUsbConfig->Usb30Settings.PreBootSupport = 1;
      //
      // AptioV server override Start: Fix for Debug Rx hang/not working issue while enabling xHCI mode.
      // Don't disable the EHCI controllers as it needs for Debug Rx working
      //
#if (AMIDEBUG_RX_SUPPORT || AMI_DEBUGGER_SUPPORT)  == 1
      //
      // Route the USB ports to xHC except debug port
      //
      for (PortIndex = 0; PortIndex < GetPchUsbMaxPhysicalPortNum (); PortIndex++) {
        if (PortIndex == 1) {
            PchUsbConfig->Usb30Settings.ManualModeUsb20PerPinRoute[PortIndex] = 0; // EHCI;
            continue;
        }
        PchUsbConfig->Usb30Settings.ManualModeUsb20PerPinRoute[PortIndex] = 1; // XHCI;
      }
      
      //
      // Disable USB30 port 2 superspeed capability from xHC.
      //
      for (PortIndex = 0; PortIndex < GetPchXhciMaxUsb3PortNum (); PortIndex++) {
        if (PortIndex == 1) {
           PchUsbConfig->Usb30Settings.ManualModeUsb30PerPinEnable[PortIndex] = 0; // Disable;           
           continue;
        }
        PchUsbConfig->Usb30Settings.ManualModeUsb30PerPinEnable[PortIndex] = 1; // Enable;
      }
#endif
      //
      // AptioV server override End: Fix for Debug Rx hang/not working issue while enabling xHCI mode.
      // Don't disable the EHCI controllers as it needs for Debug Rx working
      //
      break;
    case 2: // Auto
      PchUsbConfig->Usb30Settings.PreBootSupport = 0;
      break;
    case 3: // Smart Auto
      PchUsbConfig->Usb30Settings.PreBootSupport = 1;
      break;
    case 4: // Manual
    default:
      PchUsbConfig->Usb30Settings.PreBootSupport = SetupVariables->UsbXhciSupport;
      if (SetupVariables->RouteUsb2PinsToWhichHc == 0) {
        PchUsbConfig->Usb30Settings.ManualModeUsb20PerPinRoute[0] = SetupVariables->RouteUsb2Pin0;
        PchUsbConfig->Usb30Settings.ManualModeUsb20PerPinRoute[1] = SetupVariables->RouteUsb2Pin1;
        PchUsbConfig->Usb30Settings.ManualModeUsb20PerPinRoute[2] = SetupVariables->RouteUsb2Pin2;
        PchUsbConfig->Usb30Settings.ManualModeUsb20PerPinRoute[3] = SetupVariables->RouteUsb2Pin3;
        PchUsbConfig->Usb30Settings.ManualModeUsb20PerPinRoute[4] = SetupVariables->RouteUsb2Pin4;
        PchUsbConfig->Usb30Settings.ManualModeUsb20PerPinRoute[5] = SetupVariables->RouteUsb2Pin5;
        PchUsbConfig->Usb30Settings.ManualModeUsb20PerPinRoute[6] = SetupVariables->RouteUsb2Pin6;
        PchUsbConfig->Usb30Settings.ManualModeUsb20PerPinRoute[7] = SetupVariables->RouteUsb2Pin7;
        PchUsbConfig->Usb30Settings.ManualModeUsb20PerPinRoute[8] = SetupVariables->RouteUsb2Pin8;
        PchUsbConfig->Usb30Settings.ManualModeUsb20PerPinRoute[9] = SetupVariables->RouteUsb2Pin9;
        PchUsbConfig->Usb30Settings.ManualModeUsb20PerPinRoute[10] = SetupVariables->RouteUsb2Pin10;
        PchUsbConfig->Usb30Settings.ManualModeUsb20PerPinRoute[11] = SetupVariables->RouteUsb2Pin11;
        PchUsbConfig->Usb30Settings.ManualModeUsb20PerPinRoute[12] = SetupVariables->RouteUsb2Pin12;
        PchUsbConfig->Usb30Settings.ManualModeUsb20PerPinRoute[13] = SetupVariables->RouteUsb2Pin13;
      } else if (SetupVariables->RouteUsb2PinsToWhichHc == 1) {
        for (PortIndex = 0; PortIndex < GetPchUsbMaxPhysicalPortNum (); PortIndex++) {
          PchUsbConfig->Usb30Settings.ManualModeUsb20PerPinRoute[PortIndex] = 0; // EHCI;
        }
      } else {
        for (PortIndex = 0; PortIndex < GetPchUsbMaxPhysicalPortNum (); PortIndex++) {
          PchUsbConfig->Usb30Settings.ManualModeUsb20PerPinRoute[PortIndex] = 1; // XHCI;
        }
      }
      if (SetupVariables->Usb3PinsTermination == 0) {
        PchUsbConfig->Usb30Settings.ManualModeUsb30PerPinEnable[0] = SetupVariables->EnableUsb3Pin1;
        PchUsbConfig->Usb30Settings.ManualModeUsb30PerPinEnable[1] = SetupVariables->EnableUsb3Pin2;
        PchUsbConfig->Usb30Settings.ManualModeUsb30PerPinEnable[2] = SetupVariables->EnableUsb3Pin3;
        PchUsbConfig->Usb30Settings.ManualModeUsb30PerPinEnable[3] = SetupVariables->EnableUsb3Pin4;
        PchUsbConfig->Usb30Settings.ManualModeUsb30PerPinEnable[4] = SetupVariables->EnableUsb3Pin5;
        PchUsbConfig->Usb30Settings.ManualModeUsb30PerPinEnable[5] = SetupVariables->EnableUsb3Pin6;
      } else if (SetupVariables->Usb3PinsTermination == 1) {
        for (PortIndex = 0; PortIndex < GetPchXhciMaxUsb3PortNum (); PortIndex++) {
          PchUsbConfig->Usb30Settings.ManualModeUsb30PerPinEnable[PortIndex] = 0; // Disable
        }
      } else {
        for (PortIndex = 0; PortIndex < GetPchXhciMaxUsb3PortNum (); PortIndex++) {
          PchUsbConfig->Usb30Settings.ManualModeUsb30PerPinEnable[PortIndex] = 1; // Enable;
        }
      }
      break;
  }

// AptioV server override start : USB Per-Port/Controller Disable support. >>>
  if (((SetupVariables->PchUsb20[0] == 0) &&
      (SetupVariables->PchUsb20[1] == 0) &&
      (SetupVariables->PchUsb30Mode == 0)) || (OEM_USB_PER_CONTROLLER_DISABLE_SUPPORT))
  {
    PchUsbConfig->Usb20Settings[0].Enable = 1;
    PchUsbConfig->Usb20Settings[1].Enable = 1;
    //
    // Force "PchUsbConfig->Usb30Settings.Mode" = "Auto Mode" for Intel RC USB initialization when:
    // 1. All USB controllers are disabled.
    // 2. "OEM_USB_PER_CONTROLLER_DISABLE_SUPPORT" = "1" AND "SetupVariables->PchUsb30Mode" = "0".
    // USB Controller disable behavior will be executed at ready to boot phase (SbDxe.c).
    //
    if (SetupVariables->PchUsb30Mode == 0)
      PchUsbConfig->Usb30Settings.Mode = 2;
  }
// AptioV server override end: USB Per-Port/Controller Disable support. <<<
    
    //
    // Over Current Pins mapping
    //
  switch(BoardId) {
#ifndef GRANGEVILLE_FLAG

    case TypeMayanCityCRB:
    case TypeMayanCitySRP:
    case TypeMayanCitySRPDDR3:
    case TypeMayanCityCRBDDR3:
        Usb20OverCurrentMappings   = &MayanSRPUsb20OverCurrentMappings[0];
        Usb30OverCurrentMappings   = &MayanSRPUsb30OverCurrentMappings[0];
        UsbPortLengthTable         = &MayanSRPPortLength[0];
        UsbEyeDiagramParams        = &MayanSRPUsb20EyeDiagramTuningParam[0];
      break;

   case TypeEmeraldPoint: 
        Usb20OverCurrentMappings   = &EmeraldPointUsb20OverCurrentMappings[0];
        Usb30OverCurrentMappings   = &EmeraldPointUsb30OverCurrentMappings[0];
        UsbPortLengthTable         = &EmeraldPointPortLength[0];
        UsbEyeDiagramParams        = &MayanSRPUsb20EyeDiagramTuningParam[0];
     break;

    case TypeWildcatPass:
        Usb20OverCurrentMappings   = &WildcatUsb20OverCurrentMappings[0];
        Usb30OverCurrentMappings   = &WildcatUsb30OverCurrentMappings[0];
        UsbPortLengthTable         = &WildcatPortLength[0];
        UsbEyeDiagramParams        = &MayanSRPUsb20EyeDiagramTuningParam[0];
      break;

    case TypeAztecCityCRBSRP:
        Usb20OverCurrentMappings   = &AztecSRPUsb20OverCurrentMappings[0];
        Usb30OverCurrentMappings   = &AztecSRPUsb30OverCurrentMappings[0];
        UsbPortLengthTable         = &PlatformSRPPortLength[0];
        UsbEyeDiagramParams        = &AztecSRPUsb20EyeDiagramTuningParam[0];
      break;

    case TypeAztecCitySTHI:	  
        Usb20OverCurrentMappings   = &AztecSTHIUsb20OverCurrentMappings[0];
        Usb30OverCurrentMappings   = &AztecSTHIUsb30OverCurrentMappings[0];
        UsbPortLengthTable         = &PlatformSRPPortLength[0];
        UsbEyeDiagramParams        = &AztecSTHIUsb20EyeDiagramTuningParam[0];	
	  break;

    case TypeYakimaCityCRB: 
    case TypeYakimaCitySRP:
    case TypeYakimaCitySTHI:
        Usb20OverCurrentMappings   = &YakimaSRPUsb20OverCurrentMappings[0];
        Usb30OverCurrentMappings   = &YakimaSRPUsb30OverCurrentMappings[0];
        UsbPortLengthTable         = &PlatformSRPPortLength[0];
		    UsbEyeDiagramParams        = &PlatformSRPUsb20EyeDiagramTuningParam[0];
     break;

    case TypeIncaCityCRB:
    case TypeIncaCitySRP:
        Usb20OverCurrentMappings   = &IncaSRPUsb20OverCurrentMappings[0];
        Usb30OverCurrentMappings   = &IncaSRPUsb30OverCurrentMappings[0];
        UsbPortLengthTable         = &PlatformSRPPortLength[0];
        UsbEyeDiagramParams        = &PlatformSRPUsb20EyeDiagramTuningParam[0];
      break;

    case TypeHedtSKU1CRB:
    case TypeHedtEv:
    case TypeHedtDDR3:
      Usb20OverCurrentMappings     = &HedtUsb200OverCurrentMappings[0];
      Usb30OverCurrentMappings     = &HedtUsb30OverCurrentMappings[0];
      UsbPortLengthTable           = &HedtPortLength[0];
      UsbEyeDiagramParams          = &HedtUsb20EyeDiagramTuningParam[0];
      break;

    case TypeHalfWidthCitySRP:
    default:
      Usb20OverCurrentMappings     = &PlatformSRPOverCurrentMappings[0];
      Usb30OverCurrentMappings     = &MayanSRPUsb30OverCurrentMappings[0];
      UsbPortLengthTable           = &PlatformSRPPortLength[0];
      UsbEyeDiagramParams          = &PlatformSRPUsb20EyeDiagramTuningParam[0];
      break;

#else

    case TypeBeverlyCoveCRB:
    case TypeBeverlyCoveSODimmCRB:
      Usb20OverCurrentMappings   = &BeverlyCoveCRBUsb20OverCurrentMappings[0];
      Usb30OverCurrentMappings   = &BeverlyCoveCRBUsb30OverCurrentMappings[0];
      UsbPortLengthTable         = &BeverlyCoveCRBPortLength[0];
      UsbEyeDiagramParams        = &BeverlyCoveCRBUsb20EyeDiagramTuningParam[0];
    break;

      case TypeTrabucoCanyon:
      case TypeCamelbackMountain:
      case TypePiestewaPeak:
      case TypeTlaquepaque:
	case TypeEchoCanyon:
      Usb20OverCurrentMappings   = &StorageBoardsUsb20OverCurrentMappings[0];
      Usb30OverCurrentMappings   = &StorageBoardsUsb30OverCurrentMappings[0];
      UsbPortLengthTable         = &MayanSRPPortLength[0];
      UsbEyeDiagramParams        = &MayanSRPUsb20EyeDiagramTuningParam[0];
     break;

    default:
      Usb20OverCurrentMappings   = &BeverlyCoveCRBUsb20OverCurrentMappings[0];
      Usb30OverCurrentMappings   = &BeverlyCoveCRBUsb30OverCurrentMappings[0];
      UsbPortLengthTable         = &BeverlyCoveCRBPortLength[0];
      UsbEyeDiagramParams        = &BeverlyCoveCRBUsb20EyeDiagramTuningParam[0];
    break;
  
#endif
  }  
  CopyMem(&PchUsbConfig->Usb20OverCurrentPins[0], Usb20OverCurrentMappings, sizeof (PCH_USB_OVERCURRENT_PIN) * LPTH_USB_MAX_PHYSICAL_PORTS); 
  CopyMem(&PchUsbConfig->Usb30OverCurrentPins[0], Usb30OverCurrentMappings, sizeof (PCH_USB_OVERCURRENT_PIN) * LPTH_XHCI_MAX_USB3_PORTS);

  for(PortIndex=0;PortIndex< GetPchUsbMaxPhysicalPortNum ();PortIndex++){
      PchUsbConfig->PortSettings[PortIndex].Usb20PortLength = ((UINT16 *) UsbPortLengthTable)[PortIndex];
      PchUsbConfig->PortSettings[PortIndex].Usb20EyeDiagramTuningParam1 = ((UINT8 *) UsbEyeDiagramParams)[PortIndex*3];
      PchUsbConfig->PortSettings[PortIndex].Usb20EyeDiagramTuningParam2 = ((UINT8 *) UsbEyeDiagramParams)[(PortIndex*3) + 1];
      PchUsbConfig->PortSettings[PortIndex].Usb20EyeDiagramTuningParam3 = ((UINT8 *) UsbEyeDiagramParams)[(PortIndex*3) + 2];
  }

}
