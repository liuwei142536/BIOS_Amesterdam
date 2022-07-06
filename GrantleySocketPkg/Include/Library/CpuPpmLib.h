//
// This file contains a 'Sample Driver' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may be modified by the user, subject to  
// the additional terms of the license agreement               
//
/*++

Copyright (c) 2009 - 2019 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

    CpuPpmLib.h
    
Abstract:

  This is an implementation of the BootScript at run time.  

--*/

#ifndef _CPU_PPM_LIB_H_
#define _CPU_PPM_LIB_H_


#include <PiDxe.h>
#include <Guid/MemoryMapData.h>
#include <Protocol/CpuCsrAccess.h>


#define NUM_CST_LAT_MSR     3

#define CPU_JAKETOWN			0x2D   //AS CPUID-206D0
#define CPU_IVYTOWN				0x3E   //AS CPUID- 0x306E
#define CPU_HASWELLSERVER       0x3F   //As CPUID-0x306F
#define CPU_BDX_DE_SERVER       0x56  //AS CPUID-0x5066x
#define CPU_BDX_EP_SERVER       0x4F  //AS CPUID-406Fx

#define CPU_HASWELLSERVER_CPUID 0x306F   //As CPUID-0x306F
#define CPU_BDX_DE_SERVER_CPUID 0x5066  //AS CPUID-0x5066x
#define CPU_BDX_EP_SERVER_CPUID 0x406F  //AS CPUID-406Fx


//
// Value definition for CpuPCPSCtrl
//
#define PCD_CPU_PCPS_SINGLEPCTL                  0x00000004
#define PCD_CPU_PCPS_SPD                         0x00000008
#define PCD_CPU_PCPS_PSTATEDOMAIN                0x00000010

// Bits configuration for PcdAdvPwrMgtFlags
#define PCD_CPU_PM_TUNING               0x00000001
#define PCD_CPU_SAPM_CTL_VAL_CTL        0x00000002
#define PCD_CPU_PKG_CST_ENTRY_VAL_CTL   0x00000004
#define PCD_CPU_SW_LTR_OVRD_CTL         0x00000008
#define PCD_CPU_PRI_PLN_CURR_CFG_CTL    0x00000010
#define PCD_CPU_CURRENT_CONFIG          0x00000020
#define PCU_CPU_EFFICIENT_BOOT          0x00000040
#define CPU_MSR_LOCK                    0x00000080
#define MPLL_OFF_ENA_AUTO               0x00000100

//CSR_PCU_BIOS_SPARE2 bit definition
#define PCU_BIOS_SPARE2_MASK        (1 << 12) | (1 << 13) | (1 << 14) | (1 << 15)
#define TYPE_OF_DIMM_SHIFT          12      
#define RDIMM_DIMM                  0 
#define UDIMM_DIMM                  1 
#define LRDIMM_DIMM                 9
#define DIMM_VDD_SHIFT              14      
#define VDD_150                     0 
#define VDD_135                     (1 << 1)
#define VDD_125                     (1 << 2)

// DYNAMIC_PERF_POWER_CTL bit definition
#define EEP_L_OVERRIDE_SHIFT                    	  26      										// Bit 29:26
#define EEP_L_OVERRIDE_ENABLE_SHIFT                 25      										// Bit 25
#define I_TURBO_OVERRIDE_ENABLE_SHIFT               24      										// Bit 24
#define CST_DEMOTION_OVERRIDE_ENABLE_SHIFT          23      										// Bit 23
#define TURBO_DEMOTION_OVERRIDE_ENABLE_SHIFT        22      										// Bit 23
#define UNCORE_PERF_PLIMIT_OVERRIDE_ENABLE_SHIFT    20      										// Bit 20
#define EET_OVERRIDE_ENABLE_SHIFT                   18      										// Bit 18
#define IO_BW_PLIMIT_OVERRIDE_ENABLE_SHIFT          15      										// Bit 15
#define IMC_APM_OVERRIDE_ENABLE_SHIFT               10      										// Bit 10
#define IOM_APM_OVERRIDE_ENABLE_SHIFT               5       										// Bit 5
#define QPI_APM_OVERRIDE_ENABLE_SHIFT               0       										// Bit 0
#define EEP_L_OVERRIDE                     		    (0xf << EEP_L_OVERRIDE_SHIFT)      				// Bit 29:26
#define EEP_L_OVERRIDE_ENABLE               		(1 << EEP_L_OVERRIDE_ENABLE_SHIFT)      		// Bit 25
#define I_TURBO_OVERRIDE_ENABLE             		(1 << I_TURBO_OVERRIDE_ENABLE_SHIFT)      		// Bit 24
#define CST_DEMOTION_OVERRIDE_ENABLE        		(1 << CST_DEMOTION_OVERRIDE_ENABLE_SHIFT)      	// Bit 23
#define TURBO_DEMOTION_OVERRIDE_ENABLE      		(1 << TURBO_DEMOTION_OVERRIDE_ENABLE_SHIFT)     // Bit 23
#define UNOCRE_PERF_PLIMIT_OVERRIDE_ENABLE  		(1 << UNCORE_PERF_PLIMIT_OVERRIDE_ENABLE_SHIFT) // Bit 20
#define EET_OVERRIDE_ENABLE                             (1 << EET_OVERRIDE_ENABLE_SHIFT)                // Bit 18
#define IO_BW_PLIMIT_OVERRIDE_ENABLE        		(1 << IO_BW_PLIMIT_OVERRIDE_ENABLE_SHIFT)      	// Bit 15
#define IMC_APM_OVERRIDE_ENABLE             		(1 << IMC_APM_OVERRIDE_ENABLE_SHIFT)      		// Bit 10
#define IOM_APM_OVERRIDE_ENABLE             		(1 << IOM_APM_OVERRIDE_ENABLE_SHIFT)       		// Bit 5
#define QPI_APM_OVERRIDE_ENABLE             		(1 << QPI_APM_OVERRIDE_ENABLE_SHIFT)       		// Bit 0

// CSR_PCIE_ILTR_OVRD  (CSR 1:10:1:78)
// SW_LTR_OVRD (MSR 0xa02) -- not used
//CSR_PCIE_ILTR_OVRD bit definition
#define SNOOP_LATENCY_VLD_SHIFT			31												// Bits 31
#define FORCE_SNOOP_OVRD_SHIFT			30												// Bits 30
#define SNOOP_LATENCY_MUL_SHIFT			26												// Bits 28:26
#define SNOOP_LATENCY_Value_SHIFT		16												// Bits 25:16
#define NON_SNOOP_LATENCY_VLD_SHIFT		15												// Bits 15
#define FORCE_NON_SNOOP_OVRD_SHIFT		14												// Bits 14
#define NON_SNOOP_LATENCY_MUL_SHIFT		10												// Bits 12:10
#define NON_SNOOP_LATENCY_Value_SHIFT	0												// Bits 9:0
#define SNOOP_LATENCY_VLD_MASK			(1 << SNOOP_LATENCY_VLD_SHIFT)					// Bits 31
#define FORCE_SNOOP_OVRD_MASK			(1 << FORCE_SNOOP_OVRD_SHIFT)					// Bits 30
#define SNOOP_LATENCY_MUL_MASK			(0x7 << SNOOP_LATENCY_MUL_SHIFT)				// Bits 28:26
#define SNOOP_LATENCY_Value_MASK		(0x3FF << SNOOP_LATENCY_Value_SHIFT)			// Bits 25:16
#define NON_SNOOP_LATENCY_VLD_MASK		(1 << NON_SNOOP_LATENCY_VLD_SHIFT)				// Bits 15
#define FORCE_NON_SNOOP_OVRD_MASK		(1 << FORCE_NON_SNOOP_OVRD_SHIFT)				// Bits 14
#define NON_SNOOP_LATENCY_MUL_MASK		(0x7 << NON_SNOOP_LATENCY_MUL_SHIFT)			// Bits 12:10
#define NON_SNOOP_LATENCY_Value_MASK	(0x3FF << NON_SNOOP_LATENCY_Value_SHIFT)		// Bits 9:0

//CSR_PKG_CST_ENTRY_CRITERIA_MASK bit definition
#define DRAM_IN_SR_SHIFT                   28
#define QPI_2_IN_L1_SHIFT                  27
#define QPI_1_IN_L1_SHIFT                  26
#define QPI_0_IN_L1_SHIFT                  25
#define QPI_2_IN_L0S_SHIFT                 24
#define QPI_1_IN_L0S_SHIFT                 23
#define QPI_0_IN_L0S_SHIFT                 22
#define PCIE_IN_L1_SHIFT                   11
#define PCIE_IN_L0S_SHIFT                  0
#define DRAM_IN_SR                         (1 << DRAM_IN_SR_SHIFT)
#define QPI_2_IN_L1                        (1 << QPI_2_IN_L1_SHIFT)
#define QPI_1_IN_L1                        (1 << QPI_1_IN_L1_SHIFT)
#define QPI_0_IN_L1                        (1 << QPI_0_IN_L1_SHIFT)
#define QPI_2_IN_L0S                       (1 << QPI_2_IN_L0S_SHIFT)
#define QPI_1_IN_L0S                       (1 << QPI_1_IN_L0S_SHIFT)
#define QPI_0_IN_L0S                       (1 << QPI_0_IN_L0S_SHIFT)
#define PCIE_IN_L1                         (1 << PCIE_IN_L1_SHIFT)
#define PCIE_IN_L0S                        (1 << PCIE_IN_L0S_SHIFT)
#define PCIE_IN_LX_MASK                    0x7FF	        // Bit[10:0]
#define MASK_PCIE_BITS					   0xFFC00000		// clear bits 21:0

// CSR Perf PLIMIT bit definition for HSX <= B Stepping
#define I_TURBO_WAIT_PERIOD_SHIFT        		    19      									// Bits 31:19
#define PERF_P_LIMIT_THRESHOLD_SHIFT            13      									// Bits 18:13
#define I_TURBO_EN_SHIFT                        12      									// Bit 12
#define PERF_P_LIMIT_CLIP_SHIFT                 6       									// Bits 11:6
#define DISABLE_PERF_P_INPUT_SHIFT              5       									// Bit 5
#define RESOLUTION_MODE_SHIFT                   1       									// Bits 2:1
#define REPERF_P_LIMIT_EN_SHIFT                 0       									// Bit 0
#define I_TURBO_WAIT_PERIOD               		(0x1fff << I_TURBO_WAIT_PERIOD_SHIFT)      	// Bits 31:19
#define PERF_P_LIMIT_THRESHOLD            		(0x3f << PERF_P_LIMIT_THRESHOLD_SHIFT)      // Bits 18:13
#define I_TURBO_EN                        		(1 << I_TURBO_EN_SHIFT)      // Bit 12
#define PERF_P_LIMIT_CLIP                 		(0x3f << PERF_P_LIMIT_CLIP_SHIFT)       	// Bits 11:6
#define DISABLE_PERF_P_INPUT              		(1 << DISABLE_PERF_P_INPUT_SHIFT)      	 	// Bit 5
#define RESOLUTION_MODE                   		(3 << RESOLUTION_MODE_SHIFT)       			// Bits 2:1
#define REPERF_P_LIMIT_EN                 		(1 << REPERF_P_LIMIT_EN_SHIFT)       		// Bit 0

// CSR Perf PLIMIT bit definition for HSX >= C Stepping
#define PERF_PLIMIT_DIFFERENTIAL_SHIFT          15                                     // Bits 17:15
#define PERF_PLIMIT_DIFFERENTIAL                (7 << PERF_PLIMIT_DIFFERENTIAL_SHIFT)  // Bits 17:15
#define PERF_PLIMIT_CLIP_SHIFT                  7                                      // Bits 11:6
#define PERF_PLIMIT_CLIP                        (0x1f << PERF_P_LIMIT_CLIP_SHIFT)      // Bits 11:6
#define PERF_PLIMIT_THRESHOLD_SHIFT             1                                      // Bits 5:1
#define PERF_PLIMIT_THRESHOLD            	(0x1f << PERF_P_LIMIT_THRESHOLD_SHIFT) // Bits 5:1
#define REPERF_PLIMIT_EN_SHIFT                  0                                      // Bit 0
#define REPERF_PLIMIT_EN                 	(1 << REPERF_P_LIMIT_EN_SHIFT)         // Bit 0
//BDX bit definitons for PERF_P_LIMIT_CONTROL_PCU_FUN2_REG
#define BDX_PERF_PLIMIT_THRESHOLD_SHIFT			1								//perf_plimit_threshold - Bits[7:1]
#define BDX_PERF_PLIMIT_CLIP_SHIFT				8								//perf_plimit_clip - Bits[14:8]
#define BDX_PERF_PLIMIT_DIFFERENTIAL_SHIFT		15								//perf_plimit_differential - Bits[17:15]
#define BDX_REPERF_PLIMIT_EN_SHIFT		        0								//perf_plimit_enable - Bits[0:0]
//CSR SAPMCTLbit definition
#define SAPMCTL_LOCK_SHIFT                      31          								// Bit 31//Only for IVT

#define QPI_L0S_PLL_SEN_ENABLE_SHIFT            29          								// Bit 29//Only for IVT
#define QPI_L0_PLL_SEN_ENABLE_SHIFT             28          								// Bit 28//Only for IVT
#define IIO_L0S_PLL_SEN_ENABLE_SHIFT            27          								// Bit 27//Only for IVT
#define IIO_L0_PLL_SEN_ENABLE_SHIFT             26          								// Bit 26//Only for IVT
#define QPI2_L0S_PLL_SEN_ENABLE_SHIFT           25          								// Bit 25//Only for IVT
#define QPI2_L0_PLL_SEN_ENABLE_SHIFT            24          								// Bit 24//Only for IVT
#define QPI2_PKGC_CLOCK_GATE_DISABLE_SHIFT	    18          								// Bit 18//IVT/HSX
#define QPI01_PKGC_CLOCK_GATE_DISABLE_SHIFT	    17          								// Bit 17//IVT/HSX
#define IIO_PKGC_CLOCK_GATE_DISABLE_SHIFT	    16          								// Bit 16//IVT/HSX
#define MDLL_ON_DE_SHIFT                        15          								// Bit 15//IVT/HSX
#define MPLL_ON_DE_SHIFT                        14          								// Bit 14//IVT/HSX
#define SACG_MPLL_SHIFT                         13          								// Bit 13//Only for IVT
#define NSWAKE_SREXIT_SHIFT                     12          								// Bit 12//IVT/HSX
#define SACG_SREXIT_SHIFT                       11          								// Bit 11//Only for IVT
#define MDLL_OFF_SEN_SHIFT                      10          								// Bit 10//Only for IVT
#define MPLL_OFF_SEN_SHIFT                      9           								// Bit 9//Only for IVT
#define SACG_SEN_SHIFT                          8           								// Bit 8//Only for IVT
#define FORCE_PPLL_OFF_SHIFT                    4									          // Bit 4 //IVT/HSX
#define QPLL_OFF_ENA_SHIFT                      3           								// Bit 3//Only for IVT
#define PPLL_OFF_ENA_SHIFT                      2           								// Bit 2//IVT/HSX
#define MPLL_OFF_ENA_SHIFT                      1           								// Bit 1//IVT/HSX
#define SACG_ENA_SHIFT                          0           								// Bit 0//Only for IVT
#define SAPMCTL_LOCK                      		(1 << SAPMCTL_LOCK_SHIFT)          			// Bit 31
#define QPI_L0S_PLL_SEN_ENABLE            		(1 << QPI_L0S_PLL_SEN_ENABLE_SHIFT)         // Bit 29
#define QPI_L0_PLL_SEN_ENABLE             		(1 << QPI_L0_PLL_SEN_ENABLE_SHIFT)          // Bit 28
#define IIO_L0S_PLL_SEN_ENABLE            		(1 << IIO_L0S_PLL_SEN_ENABLE_SHIFT)         // Bit 27
#define IIO_L0_PLL_SEN_ENABLE             		(1 << IIO_L0_PLL_SEN_ENABLE_SHIFT)          // Bit 26
#define QPI2_L0S_PLL_SEN_ENABLE            		(1 << QPI2_L0S_PLL_SEN_ENABLE_SHIFT)	    // Bit 25
#define QPI2_L0_PLL_SEN_ENABLE                  (1 << QPI2_L0_PLL_SEN_ENABLE_SHIFT)  	    // Bit 24
#define QPI2_PKGC_CLOCK_GATE_DISABLE			(1 << QPI2_PKGC_CLOCK_GATE_DISABLE_SHIFT)   // Bit 18//IVT
#define QPI01_PKGC_CLOCK_GATE_DISABLE			(1 << QPI01_PKGC_CLOCK_GATE_DISABLE_SHIFT)   // Bit 17//IVT
#define IIO_PKGC_CLOCK_GATE_DISABLE			    (1 << IIO_PKGC_CLOCK_GATE_DISABLE_SHIFT)   // Bit 16//IVT
#define MDLL_ON_DE                        		(1 << MDLL_ON_DE_SHIFT)          			// Bit 15
#define MPLL_ON_DE                        		(1 << MPLL_ON_DE_SHIFT)          			// Bit 14
#define SACG_MPLL                         		(1 << SACG_MPLL_SHIFT)          			// Bit 13
#define NSWAKE_SREXIT                     		(1 << NSWAKE_SREXIT_SHIFT)          		// Bit 12
#define SACG_SREXIT                       		(1 << SACG_SREXIT_SHIFT)          			// Bit 11
#define MDLL_OFF_SEN                      		(1 << MDLL_OFF_SEN_SHIFT)          			// Bit 10
#define MPLL_OFF_SEN                      		(1 << MPLL_OFF_SEN_SHIFT)           		// Bit 9
#define SACG_SEN                          		(1 << SACG_SEN_SHIFT)           			// Bit 8
#define FORCE_PPLL_OFF					        (1 << FORCE_PPLL_OFF_SHIFT)			// Bit 4 //IVT
#define QPLL_OFF_ENA                      		(1 << QPLL_OFF_ENA_SHIFT)           		// Bit 3
#define PPLL_OFF_ENA                      		(1 << PPLL_OFF_ENA_SHIFT)           		// Bit 2
#define MPLL_OFF_ENA                      		(1 << MPLL_OFF_ENA_SHIFT)           		// Bit 1
#define SACG_ENA                          		(1 << SACG_ENA_SHIFT)           			// Bit 0

//Config TDP
#define CONFIG_TDP_LEVEL         	(3 << CONFIG_TDP_LEVEL_SHIFT)
#define CONFIG_TDP_LEVEL_SHIFT         	1		// Bit [2:1]
#define CONFIG_TDP_SHIFT         	0		// Bit 0

// MSR 0x1FC
#define MSR_POWER_CTL                           0x1FC
#define PCH_NEG_DISABLE                       	(1 << 30)
#define PCH_NEG_DISABLE_SHIFT                 	30
#define LTR_SW_DISABLE                        	(1 << 29)  //LTR_IIO_DISABLE
#define LTR_SW_DISABLE_SHIFT                  	29
#define PROCHOT_LOCK                  	        (1 << 27)
#define PROCHOT_LOCK_SHIFT          	        27
#define PROCHOT_RESPONSE                  	(1 << 26)
#define PROCHOT_RESPONSE_SHIFT          	26
#define PWR_PERF_TUNING_CFG_MODE              	(1 << 25)
#define PWR_PERF_TUNING_CFG_MODE_SHIFT        	25
#define PWR_PERF_TUNING_ENABLE_DYN_SWITCHING  	(1 << 24)
#define PWR_PERF_TUNING_ENABLE_DYN_SHIFT      	24
#define PWR_PERF_TUNING_DISABLE_EEP_CTRL      	(1 << 23)
#define PWR_PERF_TUNING_DISABLE_EEP_SHIFT     	23
#define PWR_PERF_TUNING_DISABLE_SAPM_CTRL     	(1 << 22)
#define PWR_PERF_TUNING_DISABLE_SAPM_SHIFT    	22
#define DIS_PROCHOT_OUT                         (1 << 21)
#define DIS_PROCHOT_OUT_SHIFT                   21
#define EE_TURBO_DISABLE        	        (1 << 19)
#define EE_TURBO_DISABLE_SHIFT  	        19
#define ENERGY_EFFICIENT_PSTATE_ENABLE        	(1 << 18)
#define ENERGY_EFFICIENT_PSTATE_ENABLE_SHIFT  	18
#define PHOLD_SR_DISABLE                      	(1 << 17)
#define PHOLD_SR_DISABLE_SHIFT                	17
#define PHOLD_CST_PREVENTION_INIT             	(1 << 16)
#define PHOLD_CST_PREVENTION_INIT_SHIFT        	16
#define FAST_BRK_INT_EN                       	(1 << 4)
#define FAST_BRK_INT_EN_SHIFT                  	4
#define FAST_BRK_SNP_EN                       	(1 << 3)
#define FAST_BRK_SNP_EN_SHIFT                  	3
#define SAPM_IMC_C2_POLICY_EN                 	(1 << 2)
#define SAPM_IMC_C2_POLICY_SHIFT               	2
#define C1E_ENABLE                              (1 << 1)
#define C1E_ENABLE_SHIFT                        1
#define ENABLE_BIDIR_PROCHOT_EN                 (1 << 0)
#define ENABLE_BIDIR_PROCHOT_EN_SHIFT           0

// PRIMARY_PLANE_CURRENT_CONFIG_CONTROL  0x601
#define PSI3_CODE_SHIFT                 27	// (Bits 61:59 actully) we operate on a 32 bit register
#define PSI3_THSHLD_SHIFT               20	// (Bits 58:52 actully) we operate on a 32 bit register
#define PSI2_CODE_SHIFT                 17	// (Bits 51:49 actully) we operate on a 32 bit register
#define PSI2_THSHLD_SHIFT               10	// (Bits 48:42 actully) we operate on a 32 bit register
#define PSI1_CODE_SHIFT                 7	// (Bits 41:39 actully) we operate on a 32 bit register
#define PSI1_THSHLD_SHIFT               0	// (Bits 38:32 actully) we operate on a 32 bit register
#define PPCCC_LOCK_SHIFT                31
#define CURRENT_LIMIT_SHIFT             0
#define PSI3_CODE               		(0x7 << PSI3_CODE_SHIFT)	// (Bits 61:59 actully) we operate on a 32 bit register
#define PSI3_THSHLD               		(0x7f << PSI3_THSHLD_SHIFT)	// (Bits 58:52 actully) we operate on a 32 bit register
#define PSI2_CODE                 		(0x7 << PSI2_CODE_SHIFT)	// (Bits 51:49 actully) we operate on a 32 bit register
#define PSI2_THSHLD               		(0x7f << PSI2_THSHLD_SHIFT)	// (Bits 48:42 actully) we operate on a 32 bit register
#define PSI1_CODE                 		(0x7 << PSI1_CODE_SHIFT)	// (Bits 41:39 actully) we operate on a 32 bit register
#define PSI1_THSHLD               		(0x7f << PSI1_THSHLD_SHIFT)	// (Bits 38:32 actully) we operate on a 32 bit register
#define PPCCC_LOCK                		(1 << PPCCC_LOCK_SHIFT)
#define CURRENT_LIMIT             		(0x1fff << CURRENT_LIMIT_SHIFT)

#define B_PCPS_DISABLE                (1 << 25) // Bit 25


// MSR_TURBO_POWER_LIMIT  0x610
// CSR_TURBO_POWER_LIMIT  
#define POWER_LIMIT_ENABLE_SHIFT    15   
#define POWER_LIMIT_ENABLE          (1 << POWER_LIMIT_ENABLE_SHIFT)   // Used as Bit 15 and Bit 47
#define PKG_CLMP_LIM_SHIFT          16
#define PKG_CLMP_LIM                (1 <<  PKG_CLMP_LIM_SHIFT)        // used as Bit 16 and Bit48
#define POWER_LIMIT_MASK            (0x7FFF)                          // Bits 14:0 and 46:32
#define POWER_LIMIT_1_TIME_SHIFT    17  
#define POWER_LIMIT_1_TIME_MASK     (0xFE0000)                        // Bits 23:17
#define POWER_LIMIT_LOCK_SHIFT      31
#define POWER_LIMIT_LOCK            (1 << POWER_LIMIT_LOCK_SHIFT)     // Bit 63

//  MSR_ENERGY_PERF_BIAS_CONFIG             0xA01
#define AVG_TIME_Window                 		(0xff << AVG_TIME_Window_SHIFT)				// Bits 31:24
#define PO_TOTAL_TIME_THSHLD_LOW        		(0x3f << PO_TOTAL_TIME_THSHLD_LOW_SHIFT)	// Bits 23:18
#define PO_TOTAL_TIME_THSHLD_HIGH	    		(0x3f << PO_TOTAL_TIME_THSHLD_HIGH_SHIFT)	// Bis 17:12
#define ALT_ENERGY_PERF_BIAS            		(0xf << ALT_ENERGY_PERF_BIAS_SHIFT)			// Bits 6:3
#define WORKLD_CONFIG                   		(0x7 << WORKLD_CONFIG_SHIFT)				// Bits 2:0
#define AVG_TIME_Window_SHIFT                 	24											// Bits 31:24
#define PO_TOTAL_TIME_THSHLD_LOW_SHIFT        	18											// Bits 23:18
#define PO_TOTAL_TIME_THSHLD_HIGH_SHIFT	      	12						// Bis 17:12
#define ALT_ENERGY_PERF_BIAS_SHIFT           	3		// Bits 6:3
#define WORKLD_CONFIG_SHIFT                   	0		// Bits 2:0

//MSR VR_MISC_CONFIG config bit definitions for BDX-DE
//Low DWord
#define DYN_LOAD_LINE_R_SHIFT                   0
#define IDLE_LOAD_LINE_R_SHIFT                  8
#define LEAK_LOAD_LINE_R_SHIFT                  16
#define MIN_VID_SHIFT                                    24
//High DWord
#define IOUT_OFFSET_SHIFT                            0                 //32
#define IOUT_SLOPE_SHIFT                              8                 //40
#define IDLE_EXIT_RAMP_RATE_SHIFT            18               //50
#define IDLE_ENTRY_RAMP_RATE_SHIFT         19               //51
#define IDLE_ENTRY_DECAY_ENABLE_SHIFT   20               //52 

//MSR VR_MISC_CONFIG config bit definitions for BDX-DE
#define DYN_LOAD_LINE_R_VCCSCSUS_SHIFT          0
#define DYN_LOAD_LINE_R_VCCGBE_SHIFT            8
#define DYN_LOAD_LINE_R_PCH105S_SHIFT           16
#define IDLE_EXIT_RAMP_RATE_VCCSCSUS_SHIFT      24
#define IDLE_ENTRY_RAMP_RATE_VCCSCSUS_SHIFT     25
#define IDLE_EXIT_RAMP_RATE_VCCGBE_SHIFT        26
#define IDLE_ENTRY_RAMP_RATE_VCCGBE_SHIFT       27
#define IDLE_EXIT_RAMP_RATE_PCH105S_SHIFT       28
#define IDLE_ENTRY_RAMP_RATE_PCH105S_SHIFT      29   

//
// Cross over Mode
//
#define XOVER_MODE_2TO2   1
#define XOVER_MODE_1TO1   2

#pragma pack(1)
/**************************
 Processor Power Management Data structs
***************************/

typedef struct _PPM_FROM_PPMINFO_HOB {
  UINT8       NumberOfSockets;                      // # of populated sockets in the system 
  UINT8       SocketNumber;                         // which socket
  UINT32      SocketPresentBitMap;                  // bitmap for present CPU packages/nodes
  UINT8       IioBusNumber[MAX_SOCKET];             // Bus# for IIO, indexed by CPU Socket/Node ID 
  UINT8       UncoreBusNumber[MAX_SOCKET];          // Bus# for Uncore, indexed by CPU Socket/Node ID
  UINT8       TotHa[MAX_SOCKET];                  // Total HAs; All CPUs must have same number of HA
  UINT32      mmCfgBase;
  UINT8       dimmTypePresent[MAX_SOCKET];          // 1 = UDIMM;  0 = RDIMM; Other = LRDIMM
  UINT8       ddrVoltage[MAX_SOCKET];               // 0 - 1.25V; 1 - 1.3V; 2 - 1.5V
  UINT8       DDRXOVERMODE;                         // DDR 2.2 Mode
  UINT16      BoardId;                              // Platform Type
  UINT8       ProcessorPowerUnit;                   //
  UINT8       ProcessorTimeUnit;                    //
  UINT16      PackageTdp;                           // Package TDP 
  UINT32      CapId4;                               //CapId4 CSR value
  UINT32      CapId5;                               //CapId5 CSR value
  UINT32      CpuType;							                // CpuType
  UINT8       CpuStepping;                          // CpuStepping
  UINTN       NumberOfProcessors;                   // number of active threads
  BOOLEAN     EistCap;                              // EIST Capability

  struct SystemMemoryMapHob     *SystemMemoryMap;

}PPM_FROM_PPMINFO_HOB;

typedef struct {
  UINT8           Major;                   // Major Vesion
  UINT8           Minor;                   // Minor Vesion
  UINT8           Rev;                     // Release Version
  UINT8           Build;                   //
} PPM_VERSION;

typedef union _MSR_REGISTER {
	UINT64  Qword;

	struct _DWORDS {
		UINT32  Low;
		UINT32  High;
	} Dwords;

	struct _BYTES {
		UINT8 FirstByte;
		UINT8 SecondByte;
		UINT8 ThirdByte;
		UINT8 FouthByte;
		UINT8 FifthByte;
		UINT8 SixthByte;
		UINT8 SeventhByte;
		UINT8 EighthByte;
	} Bytes;

} MSR_REGISTER;

typedef struct {
  BOOLEAN   C1e;

  UINT32          PkgCstEntryCriteriaMask[MAX_SOCKET];


} CSTATE_STRUCT;
//HWPM-start
typedef struct {
  UINT32   HWPMNative;
  UINT32   HWPMOOB;
  UINT32   AutoCState;
} HWPM_STRUCT;
//HWPM-end


typedef struct {

  BOOLEAN     EistEnabled;            // option to enable GV3

} PSTATE_STRUCT;

typedef struct {
  BOOLEAN   Enable;
  UINT32    Voltage;
  UINT16    RatioLimit[MAX_CORE];
} XE_STRUCT;

typedef struct {
  UINT32          Config1;
  UINT32          Config2;
} DTS_STRUCT;

typedef struct {

  UINT8           ConfigTDP;	
  UINT8           PcpsCtrl;
  UINT8           FastRaplDutyCycle;
  UINT8           Pl2SafetyNetEnable;
  UINT8           FuseTjMaxOffset;
  UINT8           OverclockingLock;
  UINT32          AdvPwrMgtCtlFlags;

  MSR_REGISTER	  PowerCtl;
  MSR_REGISTER    TurboPowerLimit;
  MSR_REGISTER    PP0CurrentCfg;  
  MSR_REGISTER    PerfBiasConfig;
  MSR_REGISTER    VrMiscConfig;
  MSR_REGISTER    VrMiscConfig2;
  UINT32          SapmCtl;
  UINT32          PerPLimitCtl;
  UINT32          C2C3TT;
  UINT32          DynamicPerPowerCtl;
  UINT32          PcieIltrOvrd;

  CSTATE_STRUCT   Cst;
  PSTATE_STRUCT   Pst;
  XE_STRUCT       Xe;
  HWPM_STRUCT     Hwpm;  //hwpm

#ifdef DE_SKU
  UINT8        UsePchHot;
  UINT8        UsePchTempFromME;
  UINT8        CpuToPchThrottle;
#endif

  UINT8        CpuThermalVR1OverrideEnable;
  UINT8        CpuThermalVR2OverrideEnable;

} EFI_PPM_STRUCT;


typedef struct {

  PPM_VERSION                   Version;

  EFI_CPU_CSR_ACCESS_PROTOCOL   *CpuCsrAccess;

  PPM_FROM_PPMINFO_HOB          *Info;

  EFI_PPM_STRUCT                *Setup;

  UINTN                        ProcessorNumber;

} EFI_CPU_PM_STRUCT;


#pragma pack()

VOID
PStateTransition (
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
InitializeCpuPPMLib (
		EFI_CPU_PM_STRUCT   *ppm
		);

VOID
PpmSetBiosInitDone (
    EFI_CPU_PM_STRUCT   *ppm
    );

VOID
PpmSetCsrLockBit (
    EFI_CPU_PM_STRUCT   *ppm
    );

VOID
PpmSetTurboLimitPcuSemaphore (
    EFI_CPU_PM_STRUCT   *ppm
    );

VOID
PpmSetMsrLockBit (
    EFI_CPU_PM_STRUCT   *ppm
    );

VOID
ProgramCsrPcuBiosSpare2 (
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
ProgramCSRTurboPowerLimit (
		EFI_CPU_PM_STRUCT *ppm
		);

VOID
ProgramCsrDynamicPerfPowerCtl (
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
ProgramCsrSapmCtl (
  EFI_CPU_PM_STRUCT   *ppm
		);

VOID
ProgramCsrSwLtrOvrd (
  EFI_CPU_PM_STRUCT   *ppm
		);

VOID
ProgramCsrPkgCstEntryCriteriaMask(
  EFI_CPU_PM_STRUCT   *ppm
		);

VOID
EFIAPI
SetupPCIEPkgCstEntryCriteria (
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
ProgramCsrPerfPlimitControl (
		EFI_CPU_PM_STRUCT   *ppm
		);

VOID
ProgramMsrPowerCtl (
  EFI_CPU_PM_STRUCT   *ppm
		);

VOID
ProgramMsrTurboPowerLimit (
		EFI_CPU_PM_STRUCT *ppm
		);

VOID
ProgramEnergyPerfBiasConfigMsr (
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
ProgMsrPriPlaneCurtCfgCtrL (
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
ProgramMsrTurboRatioLimit (
  EFI_CPU_PM_STRUCT *ppm
  );

VOID
ProgramMsrTemperatureTarget (
		EFI_CPU_PM_STRUCT *ppm
		);

VOID
GetMsrTemperatureTarget (
		EFI_CPU_PM_STRUCT *ppm
		);

VOID
ProgramMsrMiscPwrMgmt (
   EFI_CPU_PM_STRUCT *ppm
  );

VOID
ProgramVrMiscConfigMsr (
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
ProgramVrMiscConfig2Msr (
  EFI_CPU_PM_STRUCT   *ppm
  );

VOID
ProgramB2PFastRaplDutyCycle (
    EFI_CPU_PM_STRUCT *ppm
  );

 
//
//HWPM -start
//
VOID
HWPMInterfaceReg (
 EFI_CPU_PM_STRUCT   *ppm
);

VOID
HWPMOOBModeReg (
 EFI_CPU_PM_STRUCT   *ppm
);

VOID
HWPMAutonomousCstateModeReg (
EFI_CPU_PM_STRUCT   *ppm
);

/*
function to lock MSR_MISC_PWR_MGMT register.
*/
VOID
CpuPpmMISCPWRMGMTLock( 
EFI_CPU_PM_STRUCT   *ppm

);

/*
 * configures and lock the HWPM mode
 */
VOID
ConfigureHWPMmode (
    EFI_CPU_PM_STRUCT   *PPM
);
//
//HWPM end
//

VOID
ProgramB2PPcuMiscConfig (
    EFI_CPU_PM_STRUCT *ppm
  );



#endif
