//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*++

Copyright (c)  1999 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  MemoryMapData.h

Abstract:

  GUID used for Memory Map Data entries in the HOB list.

--*/

#ifndef _MEMORY_MAP_GUID_H_
#define _MEMORY_MAP_GUID_H_

#include "UncoreCommonIncludes.h"
#include "SysHost.h"

// {F8870015-6994-4b98-95A2-BD56DA91C07F}
#define EFI_MEMORY_MAP_GUID \
  { \
    0xf8870015,0x6994,0x4b98,0x95,0xa2,0xbd,0x56,0xda,0x91,0xc0,0x7f \
  }

extern EFI_GUID gEfiMemoryMapGuid;
extern CHAR16   EfiMemoryMapVariable[];

//
//  System Memory Map HOB information
//

#pragma pack(1)

struct DimmDevice {
    UINT8  Present;
    UINT8  X4Present;
    UINT8  NumRanks;
    UINT8  keyByte;
    UINT8  actKeyByte2;        // Actual module type reported by SPD
    UINT8  actSPDModuleOrg;    // Actual number of DRAM ranks and device width
    UINT8  actSPDSDRAMBanks;   // Actual number of banks and DRAM density
    UINT8  SPDMemBusWidth;     // Width of the Primary bus and extension
    UINT16 DimmSize;
    UINT16 SPDMMfgId;       // Module Mfg Id from SPD
    UINT16 SPDSDRAMBanks;
	UINT16 SPDSecondaryDeviceType;
    UINT8  mapOut[MAX_RANK_DIMM];
    //
    //AptioV server override - Start :For filling Type 17 fields
    //
     UINT8  SPDModSN[4];        // Module Serial Number
     UINT8  SPDModPart[20];     // Module Part Number
     UINT16 DimmSpeed;
     //
     //AptioV server override - End
     //
};

struct ChannelDevice {
   UINT8 Enabled;             // 0 = channel disabled, 1 = channel enabled
   UINT8 Features;            // Bit mask of features to enable or disable
   UINT8 MaxDimm;             // Number of DIMM
   UINT8 NumRanks;            // Number of ranks on this channel
   UINT8 SpareLogicalRank[MAX_RANK_CH/2];    // Logical rank, selected as Spare
   UINT8 SparePhysicalRank[MAX_RANK_CH/2];   // Physical rank, selected as spare
   UINT8 EnabledLogicalRanks; // Bitmap of Logical ranks that are enabled
   UINT16 rirLimit[MAX_RIR];  // Rank Interleave Register Limits
   UINT8  rirWay[MAX_RIR];    // Rank Interleave Register Ways
   UINT8  rirValid[MAX_RIR];  // RIR valid bit
   UINT32 rirOffset[MAX_RIR][MAX_RIR_WAYS];
   UINT8  rirIntList[MAX_RIR][MAX_RIR_WAYS];
   struct DimmDevice  DimmInfo[MAX_DIMM];
};

struct NodeMemInfo {
   UINT8    Enabled;                // Indicates if Node is powered and initialized
   UINT8    NumChPerHA;
   UINT8    HaNodeId;               // HA NodeID associated with this Memory Node
   UINT8    DimmType;
   UINT16   DramType;
   UINT8    LrDimmPresent;
   UINT8    DdrVoltage;
   UINT16   MemFreq;

//AptioV server override: SMBIOS type 17 data is independent of flag in AMI BIOS 
//#ifdef SDP_FLAG 
   UINT32 commonTck;          // Fastest common tCK in socket for SMBIOS17
//#endif

   UINT16   MemSize;                // Memory size of this node
   UINT16   MappedMemSize;          // Size of memory mapped to the system space
   UINT32   RequiredMemoryBase;     // Required memory base for hot plug (64MB units)
   UINT8    RasModesSupported;      // RAS modes that can be supported
   UINT8    ExRasModesSupported;    // Extended RAS modes that can be supported
   UINT8    RasModesEnabled;        // RAS modes that are enabled
   UINT8    ExRasModesEnabled;      // Extended RAS modes that are enabled
   BOOLEAN  IsMapped;               // Is this node mapped to system address space
   UINT8    MigrationMap;           // If this node is migrated, this field contains the
                                    // Src Memory node from which it was migrated. 0xFF indicates no migration
   BOOLEAN  IsMigrationSrcMirrored; // Indicates if the Migration source was mirrored or not
   struct   ChannelDevice  ChannelInfo[MAX_CH];
   UINT8    TADintList[TAD_RULES][MAX_TAD_WAYS];     // TAD interleave list for node
   UINT8    TADChnIndex[TAD_RULES][MAX_TAD_WAYS];    // Corresponding TAD channel indexes
   UINT32   TADOffset[TAD_RULES][MAX_TAD_WAYS];      // Corresponding TAD offsets
   struct   TADTable TAD[TAD_RULES];                 // TAD table

};

struct SystemMemoryMapElement {
   UINT8    NodeId;         // Node ID of the HA Owning the memory
   UINT32   BaseAddress;    // Base Address of the element in 64MB chunks
   UINT32   ElementSize;    // Size of this memory element in 64MB chunks
};

struct SystemMemoryMapHob {
   UINT8    version;
   UINT8    numberEntries;  // Number of Memory Map Elements

   struct   SystemMemoryMapElement Element[MAX_MEM_NODE * MAX_CRS_ENTRIES_PER_NODE];
   struct   NodeMemInfo NodeInfo[MAX_MEM_NODE];
   struct   memSetup MemSetup;
   UINT8    SADintList[SAD_RULES][MAX_SAD_WAYS];                  // SAD interleave list
   struct   SADTable SAD[SAD_RULES];                              // SAD table
   UINT16   Tolm;
   UINT16   HighMemBase;
   UINT32   lowMemBase;                                           // Mem base in 64MB units for below 4GB mem.
   UINT32   lowMemSize;                                           // Mem size in 64MB units for below 4GB mem.
   UINT32   highMemBase;                                          // Mem base in 64MB units for above 4GB mem.
   UINT32   highMemSize;                                          // Mem size in 64MB units for above 4GB mem.
   UINT32   memSize;									          // Total physical memory size
   UINT16   memFreq;  											  // in MHz
   UINT16   vmseFreq; 											  // in MHz
   UINT8    memMode;											  // 0 - Independent, 1 - Lockstep
   UINT8    SADNum;
   UINT8    EccEn;
   UINT8    XMPProfilesSup;
   UINT8    cpuType;
   //
   // AptioV Server Override Start : Added to get the number of HA per socket for the Smbios Type17 structure.
   //
   UINT8    NoHAPerSocket;
   //
   // AptioV Server Override End : Added to get the number of HA per socket for the Smbios Type17 structure.
   //
   BOOLEAN   IsColdBoot;      // AptioV Server Override: Added to skip LastBoot Error Handler for cold boot.
   struct   memTiming  currentMemTime;
   struct   memTiming  profileMemTime[2];
//AptioV server Override for XTU - START 
#if defined PERF_TUNE_SUPPORT && PERF_TUNE_SUPPORT == 1
  UINT8   CsrReg;
  UINT8   Bclk;
  struct   memTiming  defaultMemTime;  
#endif
//AptioV server Override for XTU - END
#ifdef BDAT_SUPPORT
#ifdef SSA_FLAG
  UINT32    bssaBdatSize;
  UINT8     bssaNumberHobs[MAX_NUMBER_SSA_BIOS_RESULTS];
  EFI_GUID  bssaBdatGuid[MAX_NUMBER_SSA_BIOS_RESULTS];
  UINT8     bssaNumberDistinctGuids;
#endif //SSA_FLAG
#endif //BDAT_SUPPORT
  UINT8     pprStatus[MAX_PPR_ADDR_ENTRIES];
};

#pragma pack()

#endif
