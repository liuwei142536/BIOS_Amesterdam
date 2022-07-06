//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*++

Copyright (c)  1999 - 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  Configuration.h

Abstract:

  Driver configuration include file.

Revision History:

--*/

#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

#include <token.h> //AptioV server override: Resolving redefinition error for VFR_FORMID_INTELRCSETUP

#define EFI_NON_DEVICE_CLASS            0x00
#define EFI_DISK_DEVICE_CLASS           0x01
#define EFI_VIDEO_DEVICE_CLASS          0x02
#define EFI_NETWORK_DEVICE_CLASS        0x04
#define EFI_INPUT_DEVICE_CLASS          0x08
#define EFI_ON_BOARD_DEVICE_CLASS       0x10
#define EFI_OTHER_DEVICE_CLASS          0x20

#define VFR_MAIN_CLASS                  0x01
// AptioV server override: Changed to INTELRCSETUP
#define VFR_INTELRCSETUP_CLASS              0x02
#define VFR_BOOT_CLASS                  0x03
#define BOOT_OPTIONS_CLASS              0x04
#define VFR_SYSINFO_CLASS               0x05

// AptioV server override: Changed to INTELRCSETUP. Commented to avoid redefinition errors when including Token.h
//#define VFR_FORMID_INTELRCSETUP             0x01
#define VFR_FORMID_PROCESSOR            0x02
#define VFR_FORMID_CSI                  0x03
#define VFR_FORMID_MEMORY               0x04
#define VFR_FORMID_IIO                  0x05
#define VFR_FORMID_PCH                  0x06
#define VFR_FORMID_IDE                  0x07
#define VFR_FORMID_ELOG                 0x08
#define VFR_FORMID_MISC                 0x09
#define VFR_FORMID_USB                  0x0A
#define VFR_FORMID_NETWORK              0x0B
#define VFR_FORMID_PWRMGT               0x0C
#define VFR_FORMID_MEMORY_RAS           0x0E
#define VFR_FORMID_ME                   0x10
#define VFR_FORMID_FWDEBUG              0x11
#define VFR_FORMID_SIEN                 0x12
#define VFR_FORMID_NM                   0x13
#define VFR_FORMID_DCMI                 0x14
#define VFR_FORMID_ICC_ENABLES          0x15
#define VFR_FORMID_ICC_SPREAD           0x16
#define VFR_FORMID_COMMONRC             0x17
#define MAIN_PAGE_SYS_INFO              0x18
#define VFR_RESERVE_MEMORY              0x19
#define VFR_FORMID_WHEA                 0x1a
#define VFR_FORMID_MEM_ERR		0x1b
//Aptio V Server Override - Start : Changes done for loading the IntelRcSetup page in Defaults.bin during build time itself.
#define	VFR_RC_SETUP_HIDE_FORMID		0x1c
//Aptio V Server Override - End : Changes done for loading the IntelRcSetup page in Defaults.bin during build time itself.
#define VFR_FORMID_PCH_ERR		0x1d
#define VFR_FORMID_QPI_ERR		0x1e
#define VFR_FORMID_PROC_ERR		0x1f
#define VFR_FORMID_PCI_ERR		0x20
#define VFR_FORMID_IRRP_ERR     0x400

#define VFR_FORMID_BMCLAN		0x120

//
// CSI Form for SV
//

#define VFR_FORMID_CSI_GENERAL          0x21
#define VFR_FORMID_CSISYSTEMWIDE        0x22
#define VFR_FORMID_CSISYSTEM_PHY_LINK   0x23
#define VFR_FORMID_CSISYSTEM_OSB        0x24
#define VFR_FORMID_CSIPERSOCKET         0x25
#define VFR_FID_CSI_CPU0                0x26
#define VFR_FID_CSI_CPU1                0x27
#define VFR_FID_CSI_CPU2                0x28
#define VFR_FID_CSI_CPU3                0x29
#define VFR_FID_CSI_CPU4                0x2A
#define VFR_FORMID_CSISYSTEM_BGF        0x2B

#define VFR_FID_CSI_CPU5                0x2E
#define VFR_FID_CSI_CPU6                0x2F

#define VFR_FID_CSI_CPU7                0x41
#define VFR_FID_CSI_CPU0_LINK0          0x42
#define VFR_FID_CSI_CPU0_LINK1          0x43
#define VFR_FID_CSI_CPU0_LINK2          0x44
#define VFR_FID_CSI_CPU1_LINK0          0x45
#define VFR_FID_CSI_CPU1_LINK1          0x46
#define VFR_FID_CSI_CPU1_LINK2          0x47
#define VFR_FID_CSI_CPU2_LINK0          0x48
#define VFR_FID_CSI_CPU2_LINK1          0x49
#define VFR_FID_CSI_CPU2_LINK2          0x4A
#define VFR_FID_CSI_CPU3_LINK0          0x4B
#define VFR_FID_CSI_CPU3_LINK1          0x4C
#define VFR_FID_CSI_CPU3_LINK2          0x4D
#define VFR_FID_CSI_CPU4_LINK0          0x4E
#define VFR_FID_CSI_CPU4_LINK1          0x4F
#define VFR_FID_CSI_CPU4_LINK2          0x90
#define VFR_FID_CSI_CPU5_LINK0          0x91
#define VFR_FID_CSI_CPU5_LINK1          0x92
#define VFR_FID_CSI_CPU5_LINK2          0x93
#define VFR_FID_CSI_CPU6_LINK0          0x94
#define VFR_FID_CSI_CPU6_LINK1          0x95
#define VFR_FID_CSI_CPU6_LINK2          0x96
#define VFR_FID_CSI_CPU7_LINK0          0x97
#define VFR_FID_CSI_CPU7_LINK1          0x98
#define VFR_FID_CSI_CPU7_LINK2          0x99

//
// CSI Form IDs
//
#define VFR_FORMID_CPU_CSI              0x30
#define VFR_FID_CSI_STATUS              0x31
#define VFR_FID_CSI_TOPOLOGY            0x32
#define VFR_FID_CSI_TOPOLOGY_MATRIX     0x33
#define VFR_FID_IIO_DEV_HIDE            0x34
#define VFR_FID_CSI_SOCKET_RES          0x35
#define VFR_FID_CSI_SOCKET_RES_REQUEST  0x36
#define VFR_FID_CSI_SOCKET_RES_STATUS   0x37

#define VFR_FID_CSI_WARNING_LOG         0x9E

//
// MEMORY Form IDs
//
#define VFR_FORMID_MEM                      0x40
#define VFR_FORMID_BRANCH_RANK_CONFIG       0x71
#define VFR_FORMID_VALHOOKS_CONFIG          0x72
#define VFR_FORMID_THERMTHRT_CONFIG         0x73
#define VFR_FORMID_MEMTOPOLOGY_DISPLAY      0x74
#define VFR_FORMID_PAGE_POLICY_DISPLAY      0x7D
#define VFR_FORMID_MEMORY_TRAINING_DISPLAY  0x7E
#define VFR_FORMID_MEM_PWR_SAVE_ADV_ID      0x7F
#define VFR_FORMID_MEM_THERMAL_ID           0x80
#define VFR_FORMID_XMP_DISPLAY              0x81
#define VFR_FORMID_MEM_MAP                  0x8A

//
// IIO Form IDs
//
#define VFR_FORMID_IIO_CONFIG           0x50
#define VFR_FORMID_VTD                  0x51
#define VFR_FORMID_PCIE                 0x52
#define VFR_FORMID_PCIE_IIO0P0          0x53
#define VFR_FORMID_PCIE_IIO0P1          0x54
#define VFR_FORMID_PCIE_IIO0P2          0x55
#define VFR_FORMID_PCIE_IIO0P3          0x56
#define VFR_FORMID_PCIE_IIO0P4          0x57
#define VFR_FORMID_PCIE_IIO0P5          0x58
#define VFR_FORMID_PCIE_IIO0P6          0x59
#define VFR_FORMID_PCIE_IIO0P7          0x5A
#define VFR_FORMID_PCIE_IIO0P8          0x5B
#define VFR_FORMID_PCIE_IIO0P9          0x5C
#define VFR_FORMID_PCIE_IIO0P10         0x5D
#define VFR_FORMID_PCIE_IIO1P0          0x5E
#define VFR_FORMID_PCIE_IIO1P1          0x5F
#define VFR_FORMID_PCIE_IIO1P2          0x69
#define VFR_FORMID_PCIE_IIO1P3          0x6A
#define VFR_FORMID_PCIE_IIO1P4          0x6B
#define VFR_FORMID_PCIE_IIO1P5          0x6C
#define VFR_FORMID_PCIE_IIO1P6          0x6D
#define VFR_FORMID_PCIE_IIO1P7          0x6E
#define VFR_FORMID_PCIE_IIO1P8          0x6F
#define VFR_FORMID_PCIE_IIO1P9          0x70
#define VFR_FORMID_PCIE_IIO1P10         0x75
#define VFR_FORMID_IIO0                 0x76
#define VFR_FORMID_IIO1                 0x77
#define VFR_FORMID_IOAT_CONFIG          0x78
#define VFR_FORMID_IIO0IOAT             0x79
#define VFR_FORMID_IIO1IOAT             0x7A
#define VFR_FORMID_IIO2IOAT             0x7B
#define VFR_FORMID_IIO3IOAT             0x7C
#define VFR_FORMID_IIO_PCIE_SLOT        0x87

#define VFR_FORMID_SKT0SVDEVHIDE     0x82
#define VFR_FORMID_SKT1SVDEVHIDE     0x84
#define VFR_FORMID_SKT2SVDEVHIDE     0x85
#define VFR_FORMID_SKT3SVDEVHIDE     0x86

#define VFR_FORMID_CONSOLE_REDIRECTION_CONFIGURATION 0x88

//
// extended IIO form IDs for 4S
//
#define VFR_FORMID_PCIE_IIO2P0          0x0190
#define VFR_FORMID_PCIE_IIO2P1          0x0191
#define VFR_FORMID_PCIE_IIO2P2          0x0192
#define VFR_FORMID_PCIE_IIO2P3          0x0193
#define VFR_FORMID_PCIE_IIO2P4          0x0194
#define VFR_FORMID_PCIE_IIO2P5          0x0195
#define VFR_FORMID_PCIE_IIO2P6          0x0196
#define VFR_FORMID_PCIE_IIO2P7          0x0197
#define VFR_FORMID_PCIE_IIO2P8          0x0198
#define VFR_FORMID_PCIE_IIO2P9          0x0199
#define VFR_FORMID_PCIE_IIO2P10         0x019A
#define VFR_FORMID_PCIE_IIO3P0          0x019B
#define VFR_FORMID_PCIE_IIO3P1          0x019C
#define VFR_FORMID_PCIE_IIO3P2          0x019D
#define VFR_FORMID_PCIE_IIO3P3          0x019E
#define VFR_FORMID_PCIE_IIO3P4          0x019F
#define VFR_FORMID_PCIE_IIO3P5          0x01A0
#define VFR_FORMID_PCIE_IIO3P6          0x01A1
#define VFR_FORMID_PCIE_IIO3P7          0x01A2
#define VFR_FORMID_PCIE_IIO3P8          0x01A3
#define VFR_FORMID_PCIE_IIO3P9          0x01A4
#define VFR_FORMID_PCIE_IIO3P10         0x01A5
#define VFR_FORMID_IIO2                 0x01A6
#define VFR_FORMID_IIO3                 0x01A7

#define VFR_FORMID_AMT                  0x01A8
#define VFR_FORMID_MEBX                 0x01A9
#define VFR_FORMID_SC                   0x01AA
#define KEY_ICC_PROFILE                 0x10B0
#define KEY_ICC_FREQ2                   0x10B1
#define KEY_ICC_FREQ3                   0x10B2
#define KEY_ICC_SPREAD2                 0x10B3
#define KEY_ICC_SPREAD3                 0x10B4
#define KEY_ICC_RATIO2                  0x10B5
#define KEY_XMP_PROFILE                 0x10B6
#define KEY_XMP_PROFILE1                0x10B7

#define ICC_ENABLES_FORM_QUESTIONID     0x10C8

//
// PCH Form IDs
//
#define PCH_OPTIONS_FORM_ID             0x60
#define PCH_LAN_OPTIONS_FORM_ID         0x61
#define PCH_PCIERP1_OPTIONS_FORM_ID     0x62
#define PCH_PCIERP2_OPTIONS_FORM_ID     0x63
#define PCH_PCIERP3_OPTIONS_FORM_ID     0x64
#define PCH_PCIERP4_OPTIONS_FORM_ID     0x65
#define PCH_PCIERP5_OPTIONS_FORM_ID     0x66
#define PCH_PCIERP6_OPTIONS_FORM_ID     0x67
#define PCH_PCIERP7_OPTIONS_FORM_ID     0x102
#define PCH_PCIERP8_OPTIONS_FORM_ID     0x103
#define PCH_SATA_OPTIONS_FORM_ID        0x68
#define PCH_EVA_OPTIONS_FORM_ID         0x104
#define PCH_USB_OPTIONS_FORM_ID         0x107
#define PCH_EVA_CLOCK_GATING_FORM_ID    0x10A
#define PCH_AZALIA_OPTIONS_FORM_ID      0x10B
#define PCH_THERMAL_FORM_ID             0x10C
#define PCH_SATA_SFMS_OPTIONS_FORM_ID   0x10D
#define PCH_PCIE_OPTIONS_FORM_ID        0x10E
#define PCH_SECURITY_FORM_ID            0x10F
#define PCH_LPSS_OPTIONS_FORM_ID        0x110
#define PCH_SSATA_OPTIONS_FORM_ID       0x119
#define PCH_SSATA_SFMS_OPTIONS_FORM_ID  0x11A
//
// Processor labels
//
#define PROCESSOR_HT_MODE               0x0100
#define PROCESSOR_FSB_MULTIPLIER        0x0101

#define BOOT_TIMEOUT_KEY_ID             0x1
#define BOOT_MANAGER_KEY_ID             0x2
#define BOOT_ORDER_KEY_ID               0x3
#define LEGACY_DEV_KEY_ID               0x4
#define LEGACY_ORDER_KEY_ID             0x5
#define AMI_CALLBACK_KEY_ID             0x6
#define EFI_SHELL_KEY_ID                0x7
#define ICC_KEY_ID                      0x8

#define VFR_FORMID_CPUCORE       0x83
//
// Memory labels
//
#define MEMORY_SLOT1_SPEED              0x0200
#define MEMORY_SLOT2_SPEED              0x0201
#define MEMORY_SLOT3_SPEED              0x0202
#define MEMORY_SLOT4_SPEED              0x0203
#define END_MEMORY_SLOT_SPEED           0x020F

#define VAR_EQ_ADMIN_DECIMAL_NAME       L"65"
#define VAR_EQ_USER_DECIMAL_NAME        L"66"
#define VAR_EQ_CONFIG_MODE_DECIMAL_NAME L"67"
#define VAR_EQ_HT_MODE_NAME             0x0048  // H
#define VAR_EQ_HT_MODE_DECIMAL_NAME     L"72"
#define VAR_EQ_RAID_MODE_NAME           0x0052  // R
#define VAR_EQ_RAID_MODE_DECIMAL_NAME   L"82"

//
// New variables used for VFR
//
#define VAR_EQ_PORT_WIDTH_NAME          0x000A
#define VAR_EQ_PORT_WIDTH_DECIMAL_NAME  L"10"

//Per Socket forms for active core count and IOT options
#define VFR_FORMID_PER_SOCKET          0x300
#define VFR_FORMID_CPU_SOCKET0         0x301
#define VFR_FORMID_CPU_SOCKET1         0x302
#define VFR_FORMID_CPU_SOCKET2         0x303
#define VFR_FORMID_CPU_SOCKET3         0x304
#define VFR_FORMID_CPU_SOCKET4         0x305
#define VFR_FORMID_CPU_SOCKET5         0x306
#define VFR_FORMID_CPU_SOCKET6         0x307
#define VFR_FORMID_CPU_SOCKET7         0x308
#define VFR_FORMID_IIO_ERR     	       0x309

#define SOCKET0_CPUPWRADVPMCFG_FORMID  0x310
#define SOCKET1_CPUPWRADVPMCFG_FORMID  0x311
#define SOCKET2_CPUPWRADVPMCFG_FORMID  0x312
#define SOCKET3_CPUPWRADVPMCFG_FORMID  0x313

//P State Control Form
#define P_STATE_CONTROL_FORMID          0x380
#define XE_RATIO_LIMIT_FORMID           0x381
#define VID_OPTIONS_FORM_ID             0x382
//HWPM-start
//HWPM control form id
#define HWPM_STATE_CONTROL_FORMID       0x385
//HWPM-end
//C State Control Form
#define CPU0_CSTATE_CONTROL_FORM_ID     0x390
#define HLV_SASV_FORM_ID                0x391

//T State Control Form
#define CPU_TSTATE_CONTROL_FORM_ID      0x392

//CPU Theraml Management
#define CPU_THERMMAL_MANAGE_FORM_ID     0x393

//Advacned PM Tuning Form
#define CPU_POWER_ADVANCED_CONFIG_FORM_ID   0x3A0
#define ENERGY_PERF_BIAS_FORM_ID            0x3A1
#define PROG_POWERCTL_MSR_FORM_ID           0x3A2
#define PROG_MSR_PP_CURT_CFG_CTRL_FORM_ID   0x3A3
#define PROG_MSR_PP_CURT_PSI_CONFIG_FORM_ID 0x3A4
#define PROG_ENTRY_CRITERIA_FORM_ID         0x3A5
#define PROG_CSR_SWLTROVRD_FORM_ID          0x3A6
#define PROG_CSR_DYNAMIC_PERF_POWER_CTL_FORM_ID   0x3A7
#define PROG_CSR_SAPMCTL_FORM_ID            0x3A8
#define PROG_CSR_PERF_P_LIMIT_FORM_ID       0x3A9

#define DRAM_RAPL_FORMID                    0x3B0
#define SOCKET_RAPL_FORMID                  0x3C0

#define PROG_MSR_VR_MISC_CONFIG_FORM_ID		0x3C1	
#define PROG_MSR_VR_MISC_CONFIG2_FORM_ID	0x3C2

#define ACPI_S_STATE_FORMID                 0x3D0

//Boot All Options Form
#define VFR_BOOT_ALL_OPTIONS                0x3D1

// HEDT Form ID's
#define VFR_FORMID_OVERCLOCKING             0x3F0
#define MEMORY_CONFIG_FORM_ID               0x3F1
#define GT_CONFIG_FORM_ID                   0x3F2
#define CLR_CONFIG_FORM_ID                  0x3F3
#define UNCORE_CONFIG_FORM_ID               0x3F4
#define SVID_FIVR_CONFIG_FORM_ID            0x3F5
#define PROCESSOR_CONFIG_FORM_ID            0x3F6

//BMC LAN Form
#define BMC_LAN_CFG_MIN_KEY_ID              0x600
#define ONBOARD_NIC_IPV4_ADDRESS_KEY_ID     0x601 // BMC_LAN_CONFIGURATION_DATA.NicIpv4.Address
#define ONBOARD_NIC_IPV4_SUBNET_MASK_KEY_ID 0x602 // BMC_LAN_CONFIGURATION_DATA.NicIpv4.SubnetMask
#define ONBOARD_NIC_IPV4_GATEWAY_KEY_ID     0x603 // BMC_LAN_CONFIGURATION_DATA.NicIpv4.Gateway
#define ONBOARD_NIC_IPV6_ADDRESS_KEY_ID     0x604 // BMC_LAN_CONFIGURATION_DATA.NicIpv6.Address
#define ONBOARD_NIC_IPV6_GATEWAY_KEY_ID     0x605 // BMC_LAN_CONFIGURATION_DATA.NicIpv6.Gateway
#define RMM_NIC_IPV4_ADDRESS_KEY_ID         0x606 // BMC_LAN_CONFIGURATION_DATA.RmmIpv4.Address
#define RMM_NIC_IPV4_SUBNET_MASK_KEY_ID     0x607 // BMC_LAN_CONFIGURATION_DATA.RmmIpv4.SubnetMask
#define RMM_NIC_IPV4_GATEWAY_KEY_ID         0x608 // BMC_LAN_CONFIGURATION_DATA.RmmIpv4.Gateway
#define RMM_NIC_IPV6_ADDRESS_KEY_ID         0x609 // BMC_LAN_CONFIGURATION_DATA.RmmIpv6.Address
#define RMM_NIC_IPV6_GATEWAY_KEY_ID         0x60a // BMC_LAN_CONFIGURATION_DATA.RmmIpv6.Gateway
#define BMC_LAN_DHCP_NAME_KEY_ID            0x60b // BMC_LAN_CONFIGURATION_DATA.DhcpName
#define BMC_LAN_CFG_MAX_KEY_ID              0x610

#endif // #ifndef _CONFIGURATION_H
