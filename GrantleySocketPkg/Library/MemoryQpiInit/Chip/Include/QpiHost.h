//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/*++
// **************************************************************************
// *                                                                        *
// *      Intel Restricted Secret                                           *
// *                                                                        *
// *      QPI Reference Code                                                *
// *                                                                        *
// *      ESS - Enterprise Silicon Software                                 *
// *                                                                        *
// *      Copyright (c) 2004 - 2015 Intel Corp.                             *
// *                                                                        *
// *      This program has been developed by Intel Corporation.             *
// *      Licensee has Intel's permission to incorporate this source code   *
// *      into their product, royalty free.  This source code may NOT be    *
// *      redistributed to anyone without Intel's written permission.       *
// *                                                                        *
// *      Intel specifically disclaims all warranties, express or           *
// *      implied, and all liability, including consequential and other     *
// *      indirect damages, for the use of this code, including liability   *
// *      for infringement of any proprietary rights, and including the     *
// *      warranties of merchantability and fitness for a particular        *
// *      purpose.  Intel does not assume any responsibility for any        *
// *      errors which may appear in this code nor any responsibility to    *
// *      update it.                                                        *
// *                                                                        *
// **************************************************************************
// **************************************************************************
// *                                                                        *
// *  PURPOSE:                                                              *
// *                                                                        *
// *      This file contains the input parameter structure for QPI RC       *
// *      main entry point.                                                 *
// *                                                                        *
// **************************************************************************
--*/

// Definition Flag:
//                  1. QPI_HW_PLATFORM   -> run with real hardware or SoftIVT 
//                  2. QPI_SW_SIMULATION -> run with QPIRC Simulation
//                  3. IA32              -> run with IA32 mode 


#ifndef _QPI_HOST_H_
#define _QPI_HOST_H_

#pragma warning (disable: 4127 4214 4100)     // disable C4127: constant conditional expression
#include "DataTypes.h"
#include "PlatformHost.h"
#include "QpiSi.h"
#include "QpiDisc.h"

#pragma pack(1)

typedef INT32  QPI_STATUS;
#ifndef NULL
#define NULL  0
#endif
#define CONST       const
#define STATIC      static
#define VOID        void
#define VOLATILE    volatile
#define QPI_SUCCESS 0
#define QPI_REBOOT  1
#define QPI_FAILURE -1

//
// Warning log
//
#define MAX_WARNING_LOGS      16
#define WARNING_LOG_OVERFLOW  - 1



/*********************************************************
                QPIRC Host Structure Related
*********************************************************/


//
// Link Speed Modes
//
#define QPI_LINK_SLOW_SPEED_MODE            0x0
#define QPI_LINK_FULL_SPEED_MODE            0x1

#define LINK0  0x0
#define LINK1  0x1
#define LINK2  0x2

//
// RTIDs related
//

// MIN RTIDs required for local & remote communication
#define MIN_LOCAL_RTID       3
#define MIN_REMOTE_RTID      2

//
// ISOC
//
#define DEFAULT_ISOC_RTIDS      11
#define DEFAULT_ISOC_VCp_RTIDS  6
#define DEFAULT_ISOC_VC1_RTIDS  ((DEFAULT_ISOC_RTIDS) - (DEFAULT_ISOC_VCp_RTIDS))

//
// MESEG
//
#define DEFAULT_MESEG_RTIDS     1  
 
#define RTID_ALLOC_OPTIMUM      0
#define RTID_ALLOC_SYMMETRIC    1
#define RTID_ALLOC_ALTERNATE    2

//
// Link speed
//

typedef enum {
  RESERVED0         = 0, 
  SPEED_REC_64GT,
  SPEED_REC_72GT,
  SPEED_REC_80GT,
  RESERVED1,
  SPEED_REC_96GT,
  MAX_QPI_LINK_SPEED,            // Implies "Auto"
  MAX_LIMITED_QPI_LINK_SPEED,
  FREQ_PER_LINK,
} QPI_LINK_SPEED;

typedef enum {
  FULL_SPEED = 0,           
  HALF_SPEED
} QPI_LINK_SPEED_TYPE;


//
// LL credit allocation

#define  QPI_LL_VNA_NORMAL_IVT       100
#define  QPI_LL_VNA_NORMAL_HSX       126
#define  QPI_LL_VNA_MIN       0

typedef enum {
  PROBE_TYPE_NO_PROBE   = 0,
  PROBE_TYPE_COHASSET,
  PROBE_TYPE_VSR,
} PROBE_TYPE;

typedef enum {
  PER_LANES_TXEQ_ENABLED   = 0,         // each lane use different TXEQ value
  ALL_LANES_TXEQ_ENABLED                // all lanes use same TXEQ value
} LANE_TXEQ_TYPE;


// CRC Mode
typedef enum {
  CRC_MODE_8BIT = 0,
  CRC_MODE_ROLLING_16BIT,
  MAX_CRC_MODE                  // Implies "Per Link Setting of the preceding values, i.e. CRC_MODE_8BIT or CRC_MODE_ROLLING_16BIT.
} CRC_MODE;

// L0p Mode
typedef enum {
  L0P_DISABLE = 0,
  L0P_ENABLE,
  MAX_L0P_MODE      // Implies "Per Link Setting of the preceding values, i.e. L0P_DISABLE or L0P_ENABLE.
} L0P_MODE;

// L1 Mode
typedef enum {
  L1_DISABLE = 0,
  L1_ENABLE,
  MAX_L1_MODE      // Implies "Per Link Setting of the preceding values, i.e. L1_DISABLE or L1_ENABLE.
} L1_MODE;

typedef enum {
  WIDTH_Q3Q2Q1Q0 = 0,
  WIDTH_Q1Q0,
  WIDTH_Q2Q0,
  WIDTH_Q3Q0,
  WIDTH_Q2Q1,
  WIDTH_Q3Q1,
  WIDTH_Q3Q2,
  WIDTH_Q0,
  WIDTH_Q1,
  WIDTH_Q2,
  WIDTH_Q3,
  MAX_QPI_LINK_WIDTH,      // Implies "Auto"
  QPI_LINK_WIDTH_PER_LINK  // each link has its own link width setting.
} QPI_LINK_WIDTH;

typedef enum {
  LCC = 0,
  MCC,
  HCC,
  LCC_DE,
  HCC_DE,
  MAX_CHOP_TYPES
} PHYSICAL_CHOP;

typedef enum {
  EXA_8S_HCC = 0,
  EXA_4S_HCC,
  EP_4S_HCC,

  EP_HCC_DHT_ENABLED,
  EP_HCC_SHT_ISOC_DISABLED,
  EP_HCC_ES_ISOC_DISABLED,
  EP_HCC_SHT_ISOC_ENABLED,

  EP_MCC_DHT_ENABLED,
  EP_MCC_SHT_ISOC_DISABLED,
  EP_MCC_ES_ISOC_DISABLED,
  EP_MCC_SHT_ISOC_ENABLED,

  EP_LCC_DHT_ENABLED,
  EP_LCC_SHT_ISOC_DISABLED,
  EP_LCC_SHT_ISOC_ENABLED,

  EN_MCC_DHT_ENABLED,
  EN_LCC_DHT_ENABLED, 
  SYS_1S,
  MAX_CONFIG_TYPE
} CONFIG_TYPE;






typedef enum {
  TOPOLOGY_PRECEDENCE = 0,      // Default
  FEATURE_PRECEDENCE = 1
} DEGRADE_PRECEDENCE;

//
// PHY settings that are system dependent.   Need 1 of these for each socket/link/freq.   
// For any probe to be used on a link, a entry will need to be created.
//
typedef struct {                      // This is for IVT full speed 10 bundles setting
  UINT8     SocketID;
  UINT8     ProbeType;
  UINT8     Freq;
  UINT8     Link;
  UINT32    PRT;
  UINT32    PTV;
  UINT32    TXEQB[10];
  UINT32    CTLEPEAK0;
  UINT32    CTLEPEAK1;
} EPARAM_LINK_INFO;

typedef struct {                     // This is for IVT half speed 10 bundles setting
  UINT8     SocketID;
  UINT8     ProbeType;
  UINT8     Freq;
  UINT8     Link;
  UINT32    TXEQB_HS[10];
} HS_EPARAM_LINK_INFO;

typedef struct {                      // This is for HSX 20 lanes setting  
  UINT8     SocketID;
  UINT8     ProbeType;
  UINT8     AllLanesUseSameTxeq;
  UINT8     Freq;
  UINT8     Link;  
  UINT32    PRT;
  UINT32    PTV;
  UINT32    TXEQL[20];                // AllLanesUseSameTxeq = 1, only use TXEQ[0],
  UINT32    CTLEPEAK[5];              // AllLanesUseSameTxeq = 1, only use CTLEPEAK[0],
} HSX_PER_LANE_EPARAM_LINK_INFO;

typedef struct {                      // This is for HSX full speed all lanes have the same TXEQ setting
  UINT8     SocketID;
  UINT8     ProbeType;
  UINT8     Freq;
  UINT8     Link;
  UINT32    PRT;
  UINT32    PTV;
  UINT32    AllLanesTXEQ;
  UINT8     CTLEPEAK;
} HSX_ALL_LANES_EPARAM_LINK_INFO;


typedef struct {                     // This is for HSX half speed all lanes have the same TXEQ setting
  UINT8     SocketID;
  UINT8     ProbeType;
  UINT8     Freq;
  UINT8     Link;
  UINT32    HS_AllLanesTXEQ;
} HSX_ALL_LANES_HS_EPARAM_LINK_INFO;

// BDX specific clones
#define BDX_PER_LANE_EPARAM_LINK_INFO     HSX_PER_LANE_EPARAM_LINK_INFO
#define BDX_ALL_LANES_EPARAM_LINK_INFO    HSX_ALL_LANES_EPARAM_LINK_INFO
#define BDX_ALL_LANES_HS_EPARAM_LINK_INFO HSX_ALL_LANES_HS_EPARAM_LINK_INFO

                     
typedef struct {
  //
  //Credits to be programmed in QPIA2RCRCTRL
  //
  UINT8 egrndr_credits;
  UINT8 egrhom_credits;             
  UINT8 egrsnp_credits;                    

  //
  // Credits to be programmed in R3QRTE0CR/R3QRTE1CR for QPI -> QPI communication.
  // Except AD/BL VNA credits, other credits are per VN (i.e VN0 & VN1). But both VNs
  // have same settings.
  //
  UINT8 advnaqpi;
  UINT8 snpqpi;
  UINT8 homqpi;
  UINT8 ndrqpi;
  UINT8 blvnaqpi;
  UINT8 drsqpi;
  UINT8 ncsqpi;
  UINT8 ncbqpi;

  //
  //Credits to be programmed in R3QPCIR0CR/R3QPCIR1CR for R3QPI -> R2PCIe communication
  //
  UINT8 pcicredit;
} R3QPI_RING_CREDIT_PARAM;

typedef struct {      
  // For ring credit setting 
  UINT8 left_ad_vna_r3qpi0;               // Cbo vna ad ingress (Left cbo)    -> R3QPI
  UINT8 right_ad_vna_r3qpi0;              // Cbo vna ad ingress (Right cbo)   -> R3QPI
  UINT8 left_bl_vna_r3qpi0;               // Cbo vna bl ingress (Left cbo)    -> R3QPI
  UINT8 right_bl_vna_r3qpi0;              // Cbo vna bl ingress (Right cbo)   -> R3QPI
  UINT8 Qpi_To_Cbo_Ipq;                   // Qpi_To_Cbo_Ipq in R3QCBOHACR_R3QPI_CFG.numsnpcreditsl0
  UINT8 Qpi_To_Ha_WB_Credit;              // Qpi_To_Ha_WB_Credit in R3QCBOHACR_R3QPI_CFG_HSX_BDX_STRUCT.ha0creditr0
  //For Ha credit setting
  UINT8 AdVnaCrdt;                        // HA VNA AD Ingress  -> R3QPI
  UINT8 BlVnaCrdt;                        // HA VNA BL Ingress  -> R3QPI
  UINT8 Ipq;                              // HA IPQ -> Cbo
} CreditTablePara;

//
// IIO resource allocation info
//
typedef struct {
  UINT8         BusBase;
  UINT8         BusLimit;
  UINT16        IoBase;
  UINT16        IoLimit;
  UINT32        IoApicBase;
  UINT32        IoApicLimit;
  UINT32        MmiolBase;
  UINT32        MmiolLimit;
  UINT64_STRUCT MmiohBase;
  UINT64_STRUCT MmiohLimit;
} QPI_CPU_RESOURCE;

//R3QPI
typedef struct {
  UINT32           R3Egrerrlog0;
  UINT32           R3Egrerrlog1;
  UINT32           R3Ingerrlog0;
  UINT32           R3Ingerrlog1;
} QPI_CPU_R3QPI_ERRLOG;

//R2PCIe
typedef struct {
  UINT32           R2Egrerrlog;
  UINT32           R2Ingerrlog0;
  UINT32           R2Egrerrlog2;
} QPI_CPU_R2PCIE_ERRLOG;


typedef struct {
  QPI_CPU_R3QPI_ERRLOG    R3Qpi;
  QPI_CPU_R2PCIE_ERRLOG   R2Pcie;
} QPI_CPU_ERRLOG;

// CPU socket QPI info
//
typedef struct {
#ifndef ASM_INC
  UINT32              Valid : 1;
  UINT32              SocId : 3;
  UINT32              Rsvd1 : 28;
#else
  UINT32              RawData;
#endif // ASM_INC
  QPI_LINK_DATA       LepInfo[MAX_QPI_PORTS];         // QPI LEP info
  TOPOLOGY_TREE_NODE  TopologyInfo[MAX_CPU_SOCKETS];  // Topology info describing how this soecket is connected to all other sockets
  QPI_CPU_RESOURCE    CpuRes;                         // System resources assigned per CPU
  QPI_CPU_ERRLOG      CpuErrLog;                      // Log of errors that occured during previous boot
} QPI_CPU_INFO;


//
// Link layer settings, per link
//
typedef struct {
  UINT8               QpiPortDisable:1;         // TRUE - Port disabled;    FALSE- Port enabled (default)
  UINT8               QpiLinkCreditReduce:1;    // 1 - Use reduced credit;  0 - Use max credit (default)
  UINT8               Rsvd:6;  
} QPI_CPU_LINK_SETTING;

  
//
// Phy general setting, per link
//
typedef struct {
  UINT32  QpiLinkSpeed:3;       // Use QPI_LINK_SPEED excluding FREQ_PER_LINK, default SPEED_REC_64GT
  UINT32  QpiProbeType:2;       // Use PROBE_TYPE definition, default PROBE_TYPE_NO_PROBE
  UINT32  QpiConfigTxWci:4;     // One of WIDTH_Q3Q2Q1Q0, WIDTH_Q1Q0, WIDTH_Q3Q2, MAX_QPI_LINK_WIDTH - Auto (Default)
  UINT32  Rsvd:23;  
} QPI_CPU_PHY_SETTING;

//
// Per CPU setting
//
typedef struct {
  QPI_CPU_LINK_SETTING  Link[MAX_QPI_PORTS];
  QPI_CPU_PHY_SETTING   Phy[MAX_QPI_PORTS];
} QPI_CPU_SETTING;


//
// QPIRC input structure
//
typedef struct {
  //
  // Protocol layer and other general options; note that "Auto" is provided only options whose value will change depending
  // on the topology, not for all options.
  //

  //
  // Indicates the ratio of Bus/MMIOL/IO resource to be allocated for each CPU's IIO.
  // Value 0 indicates, that CPU is not relevant for the system. If resource is
  // requested for an CPU that is not currently populated, QPIRC will assume
  // that the ratio is 0 for that CPU and won't allocate any resources for it.
  // If resource is not requested for an CPU that is populated, QPIRC will force
  // the ratio for that CPU to 1.
  //

  UINT8               PPINrOptIn;                     // 0 - No (default), 1 - Yes 
  UINT8               BusRatio[MAX_CPU_SOCKETS];
  UINT8               IoRatio[MAX_CPU_SOCKETS];
  UINT8               MmiolRatio[MAX_CPU_SOCKETS];

  UINT8               LegacyVgaSoc;       // Socket that claims the legacy VGA range; valid values are 0-3; 0 is default.
  UINT8               MmioP2pDis;         // 1 - Disable (default); 0 - Enable
  UINT8               IsocAzaliaVc1En;    // 1 - Enable; 0 - Disable (i.e use VCp if Azalia is enabled), default; 2 - Auto.(EP only)
  UINT8               DebugPrintLevel;    // Bit 0 - Fatal, Bit1 - Warning, Bit2 - Info Summary; Bit 3 - Info detailed. 1 - Enable; 0 - Disable
  UINT8               AltRtid2S;          // 1 - Enable; 0 - Disable;  Normally disabled, but if enabled can boost performance in some 2S environments
                                          // only affects 2S configurations (EP only)
  UINT8               ClusterOnDieEn;     // 1 - Enable; 0 - Disable (Only available in 1/2S EP)
  UINT8               IBPECIEn;      // 1 - Enable; 0 - Disable
  UINT8               E2EParityEn;        // 1 - Enable; 0 - Disable
  UINT8               EarlySnoopEn;       // 1 - Enable; 0 - Disable (Only available in 1/2S EP 2Q)
  UINT8               HomeDirWOSBEn;      // 1 - Enable; 0 - Disable (Only available in 2S-2Q and 2S-1Q)
  UINT8               DegradePrecedence;  // Use DEGRADE_PRECEDENCE definition; TOPOLOGY_PRECEDENCE is default
  UINT8               SnoopMode;          // Snoop Mode: 0 - Early Snoop; 1 - Home Snoop /wo Directory; 2 - HS /w Directory; 3 - HS /w Dir & OSB; 4 - Auto (default)

  //
  // Phy/Link Layer Options (System-wide and per socket)
  //
  UINT8               QpiLinkSpeedMode;   // Link speed mode selection; 0 - Slow Speed; 1- Full Speed (default)
  UINT8               QpiLinkSpeed;       // Use QPI_LINK_SPEED definition; MAX_QPI_LINK_SPEED - Auto (i.e BIOS choosen speed) default is SPEED_REC_64GT
  UINT8               QpiLinkL0sEn;       // 0 - Disable (default), 1 - Enable  
  UINT8               QpiLinkL0pEn;       // 0 - Disable (default), 1 - Enable
  UINT8               QpiLinkL1En;        // 0 - Disable, 1 - Enable (default)
  UINT8               QpiLinkL0rEn;       // 0 - Disable; 1 - Enable (default); 
  UINT8               QpiLbEn;            // 0 - Disable(default), 1 - Enable
  UINT8               IioUniphyDisable[MAX_CPU_SOCKETS]; // 0 - No (default), 1 - Yes, without Memory Hot Add,  2 - Yes, with Memory Hot Add
  UINT8               QpiLinkCreditReduce;   //  0 - Use max credit (default); 1 - Use reduced credit; 2 - use per link option
  UINT8               QpiConfigTxWci;        //  One of WIDTH_Q3Q2Q1Q0, WIDTH_Q1Q0, WIDTH_Q3Q2, MAX_QPI_LINK_WIDTH - Auto (Default) or QPI_LINK_WIDTH_PER_LINK (to use per link option) (EX only)
  UINT8               QpiAdVNACreditArbThreshold;   // 0 - default, 4 bits value, R3QPI credit merging configuration field

  UINT8               QpiCrcMode;         // Use CRC_MODE definition excluding MAX_CRC_MODE; default CRC_MODE_8BIT (EX only)

  UINT8               QpiCpuSktHotPlugEn;     // 0 - Disable (default), 1 - Enable  
  UINT8               QpiCpuSktHotPlugTopology;     // 0 - 4S Topology (default), 1 - 8S Topology
  UINT8               QpiSkuMismatchCheck;          // 0 - No, 1 - Yes (default)

  //
  // Phy/Link Layer Options (per Port)
  //
  QPI_CPU_SETTING     PhyLinkPerPortSetting[MAX_CPU_SOCKETS];



} QPI_HOST_IN;

typedef struct{
  //
  // These variables are initialized and used like global variable by QPIRC. Not meant for outside QPIRC usage.
  //
  UINT8         CurrentReset;                  // Reset type witnessed by the system (viz PWRGD, WARM)
  UINT8         SbspSoc;                       // System BSP socket ID  
  UINT8         SkuType[MAX_CPU_SOCKETS];      // SKU type
  UINT8         SubSkuType[MAX_CPU_SOCKETS];   // EXA sub SKU type
  UINT8         ChopType[MAX_CPU_SOCKETS];     // Physical chop type
  UINT8         TotCbo[MAX_CPU_SOCKETS];       // Total Cbos/LLCs; All CPUs must have same number of Cbo/LLCs
  UINT32        CboList[MAX_CPU_SOCKETS];      // List of Cbos/LLCs
  UINT8         TotSbo[MAX_CPU_SOCKETS];       // Total Sbos; All CPUs must have same number of Sbo
  UINT8         TotHa[MAX_CPU_SOCKETS];        // Total HAs; All CPUs must have same number of HA
  UINT8         TotR3Qpi[MAX_CPU_SOCKETS];     // Total R3QPIs; All CPUs must have same number of R3Qpi 
  UINT8         TotQpiAgent[MAX_CPU_SOCKETS];  // Total QPIAgents; All CPUs must have same number of QpiAgents 
  UINT8         TotCpu;                        // Total CPUs populated in the system
  UINT32        CpuList;                       // List of CPUs populated in the system   
  UINT8         OrigTotCpu;                    // Total CPUs populated in the system before topology reduction
  UINT32        OrigCpuList;                   // List of CPUs populated in the system before topology reduction  
  UINT8         BtModeEn;                      // HA Backup Tracker Mode Enabled
  UINT8         BtMode;                        // HA Backup Tracker Mode   
  UINT8         BtMode2Alt;                    // BT Mode 2 Alternate  0 - 96/36 RTID (non-overlap on local between non-xrtid & xrtid); 1 - 128/48 RTID (overlap on local between non-xrtid & xrtid) for Mode 2
  UINT8         IodcEn;                        // Iodc enable  
  UINT8         CboPoolMode;                   // Cbo Pool Mode
  UINT8         SysSnoopMode;                  // System snoop mode: 0 - Early Snoop; 1 - Home Snoop /wo Directory; 2 - HS /w Directory; 3 - HS /w Dir & OSB
  UINT8         SnoopFanoutEn;                 // Enable snoop fanout 
  UINT8         Vn1En;                         // VN1 is enabled for the system or not 
  UINT8         D2cEn;                         // Direct To Core Enable: 1 - Enable; 0 - Disable
  BOOLEAN       ExtRtidMode;                   // Extended RTID mode
  BOOLEAN       ForceColdResetFlow;            // Force cold reset flow
  BOOLEAN       TwistedQpiLinks;               // Indicates if the links are twisted in 2S config
  BOOLEAN       Is4SRing;                      // Indicates if 4S ring
  BOOLEAN       IsRouteThroughConfig;          // Indicates we have a route-through config
  UINT8         HitMeEn;                       // HitME Cache enable
  UINT8         UboxRtids;                     // UBOX RTIDS size based on CPU type 
  UINT8         SysConfigType;                 // System configuration type based on core number, system topology, isoc, bt mode, etc.. HSX use only 
  UINT8         ClusterOnDieActive;            // Track if COD already enabled in SI
  BOOLEAN       LaneDropPcodeFixEn;            // Fix to lane drop: 1 = Enable; 0 - Disable

  //
  // RTID Base/Size for WB, Ubox, ISOC, Cbos and ExtraRTIDs. The Cbos operate in 2 pool mode
  // we need MAX_CBO*2 + 4 elements. We also need to allocate different number of RTIDs for
  // Local, Neighbor and Remote sockets. So we need 3 elements; 0 for Local, 1 for Neighbor
  // and 2 for Remote socket.
  //
  UINT8         RtidBase[3][MAX_CBO * 2 + 4];
  UINT8         RtidSize[3][MAX_CBO * 2 + 4];

  //
  // 2S SHT RTID allocation schemes not agnostic of socket number; so we need data structure to track
  // RTID allocation that is per socket.
  //
  UINT8         RtidBaseIsocS0[2][MAX_CBO * 2 + 4];
  UINT8         RtidSizeIsocS0[2][MAX_CBO * 2 + 4];
  UINT8         RtidBaseIsocS1[2][MAX_CBO * 2 + 4];
  UINT8         RtidSizeIsocS1[2][MAX_CBO * 2 + 4];

  //
  // Matrix of RBT/HT base, size allocated to each Cbo. Each row indicates the socket that allocates
  // the RBT/HT and the column indicates the socket that consumes those RBTs/HTs.
  //
  UINT8         RbtHtBase[MAX_CPU_SOCKETS][MAX_CPU_SOCKETS];
  UINT8         RbtHtSize[MAX_CPU_SOCKETS][MAX_CPU_SOCKETS];
  UINT8         QpiCpuSktHotPlugEn;           // hot plug enabled or not (for IVT EXA only)
  
  // HT Base values; used only for HSX/BDX with SHT mode
  UINT32        HtBase[MAX_CPU_SOCKETS];

  BOOLEAN       QpiRunAdaptation;             // Whether to run the adaptation or not
  UINT8         CurrentSpeedAdapted;          // Speed that is being adapted; Bit0 - 5.6, Bit1 - 6.4, etc
  UINT8         Sys4SClusterOnDieEn;          // Indicates whether we are in 4S-COD- mode, 1 - Enable; 0 - Disable
  UINT8         SkippedCheckedInCheck;        // Bitmap of 1-hop away sockets not checked for check-in

} QPI_HOST_INTERNAL_GLOBAL;

//
// QPIRC output structure
//
typedef struct {


  //
  // All CPUs are assigned pre-determined array element, viz, element 0 is for CPU0,
  // element 1 is for CPU1 etc. There is valid bit to track if the CPU is populated or not.
  //
  QPI_CPU_INFO  CpuInfo[MAX_CPU_SOCKETS];      // QPI related info per CPU
  UINT8         SysConfig;                     // System Topology
  UINT32        WarningLog[MAX_WARNING_LOGS];  // Warning Logs

  //
  // There is a corresponding input option for these options. If the input option can not be met,
  // due to the current system topology not allowing it, the options will be forced to supported
  // value and platform BIOS can use these variables to know if any such overriding has happened.
  //
  UINT8         OutLegacyVgaSoc;               // Socket that claims the legacy VGA range
  UINT8         OutIsocEn;                     // 1 - Enable; 0 - Disable (BIOS will force this for 4S)
  UINT8         OutMesegEn;                    // 1 - Enable; 0 - Disable (Mutually exclusive with 'OutIsocEn')
  UINT8         OutIsocAzaliaVc1En;            // 1 - Enable; 0 - Disable (i.e use VCp if Azalia is enabled) . This is N/A for 4S.
  UINT8         OutClusterOnDieEn;             // 1 - Enable; 0 - Disable (Only available in 1/2S/4S EP- and EX-)  
  UINT8         OutIBPECIEn;                   // 1 - Enable; 0 - Disable
  UINT8         OutE2EParityEn;                // 1 - Enable; 0 - Disable
  UINT8         OutEarlySnoopEn;               // 1 - Enable; 0 - Disable (Only available in 1/2S EP) 2Q
  UINT8         OutHomeDirWOSBEn;
  UINT8         QpiCurrentLinkSpeedMode;       // Current link speed mode; 0 - Slow Speed; 1- Full Speed.  
  UINT8         OutQpiLinkSpeed;               // Link speed/freq
  UINT8         OutPerLinkSpeed[MAX_CPU_SOCKETS][MAX_QPI_PORTS]; // Per link speed
  UINT8         OutQpiLinkL0sEn;               // 0 - Disabled, 1 - Enabled  
  UINT8         OutQpiLinkL0pEn;               // 0 - Disabled, 1 - Enabled
  UINT8         OutQpiLinkL1En;                // 0 - Disabled, 1 - Enabled
  UINT8         OutQpiLinkL0rEn;               // 0 - Disabled, 1 - Enabled
  UINT8         OutIioUniphyDisable[MAX_CPU_SOCKETS]; // 0 - No (default), 1 - Yes, without Memory Hot Add,  2 - Yes, with Memory Hot Add
  UINT8         OutQpiCrcMode;                 // 0 - 8 bit CRC 1 - 16 bit CRC Mode
  UINT8         OutClusterOnDieReduction;      // COD  enabled and doing a topo reduction
  UINT8         OutPointerSeperationHA;        // Valid range : 0 - 3. Used to inform MRC on what pointer seperation to use for BGF credit programming

  //
  // These variables are initialized and used like global variable by QPIRC.
  // Not meant for outside QPIRC consumption.
  //
  UINT8         RasInProgress;  // Set to TRUE when the QPIRC is called to handle a CPU/IOH O*L event
  UINT8         RasEvtType;  // 0 - Online; 1 - Offline
  UINT8         RasSocId;  // RAS event socket ID
  BOOLEAN       ProgramNonBC;  //Flag to indicate if the Non BC RTA entries to be programmed
  BOOLEAN       ProgramBC;   //Flag to indicate if the BC RTA entries to be programmed

} QPI_HOST_OUT;

//
// QPIRC VNRAM structure for S3 resume
//
typedef struct {
  UINT32            Cpus;           // List of CPUs populated in the system
  UINT32            Cbos;           // List of Cbos/LLCs in SBSP
  QPI_LINK_DATA     LepInfo[MAX_CPU_SOCKETS][MAX_QPI_PORTS];  // QPI LEP info
} QPI_HOST_NVRAM;


#pragma pack()

#endif // _QPI_HOST_H_
