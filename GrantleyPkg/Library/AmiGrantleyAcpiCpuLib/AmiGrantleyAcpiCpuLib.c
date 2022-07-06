//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2013, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/*++
Copyright (c) 1996 - 2013, Intel Corporation.

This source code and any documentation accompanying it ("Material") is furnished
under license and may only be used or copied in accordance with the terms of that
license.  No license, express or implied, by estoppel or otherwise, to any
intellectual property rights is granted to you by disclosure or delivery of these
Materials.  The Materials are subject to change without notice and should not be
construed as a commitment by Intel Corporation to market, license, sell or support
any product or technology.  Unless otherwise provided for in the license under which
this Material is provided, the Material is provided AS IS, with no warranties of
any kind, express or implied, including without limitation the implied warranties
of fitness, merchantability, or non-infringement.  Except as expressly permitted by
the license for the Material, neither Intel Corporation nor its suppliers assumes
any responsibility for any errors or inaccuracies that may appear herein.  Except
as expressly permitted by the license for the Material, no part of the Material
may be reproduced, stored in a retrieval system, transmitted in any form, or
distributed by any means without the express written consent of Intel Corporation.
--*/



//Note: the MADT code works with Madt3.aslc, Madt4.aslc replacing AcpiPlatform.c and AcpiPlatformHooks.c.

#include <IndustryStandard/Acpi.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/MpService.h>
#include "Platform.h"
#include <Token.h>


BOOLEAN gHtEnabled;
BOOLEAN gX2ApicEnabled;

/////////////////////////////////////////////////////////////////////////

#define EFI_CPUID_CORE_TOPOLOGY 0x0B
#define THREAD_MASK_64          0x3f
#define THREAD_MASK_32          0x1f
#define APICID_SHIFTBIT_32      5
UINT32  mNumOfBitShift;

typedef struct {
  UINT32   AcpiProcessorId;
  UINT32   ApicId;
  UINT32   Flags;
  UINT32   SwProcApicId;
  UINT32   SocketNum;
} EFI_CPU_ID_ORDER_MAP;


static BOOLEAN mCpuOrderSorted = FALSE;

EFI_CPU_ID_ORDER_MAP           mCpuApicIdOrderTable[MAX_CPU_NUM];

UINT32 mCpuIndexTable[MAX_THREAD * MAX_CORE];


UINT32
ApicId2SwProcApicId (
  UINT32 ApicId
  )
{
  UINT32 Index;

  for (Index = 0; Index < MAX_CPU_NUM; Index++) {
    if ((mCpuApicIdOrderTable[Index].Flags == 1) && (mCpuApicIdOrderTable[Index].ApicId == ApicId)) {
      return Index;
    }
  }

  return (UINT32) -1;

}


//Note: This match the same function in AcpiPlatformHooks.c
EFI_STATUS
SortCpuLocalApicInTable (
  IN  EFI_MP_SERVICES_PROTOCOL  *MpService,
  IN  UINTN                     NumberOfCPUs
  )
{
  EFI_STATUS                                Status;
  EFI_PROCESSOR_INFORMATION                 ProcessorInfoBuffer;
  UINTN                                     BufferSize;
  UINT32                                    Index;
  UINT32                                    CurrProcessor;
  UINT32                                    BspApicId;
  UINT32                                    TempVal = 0;
  EFI_CPU_ID_ORDER_MAP                      *CpuIdMapPtr;
  UINT32                                    CoreThreadMask;
//  UINT32                                    tempCoreNum, currentCore, currentSkt;

  BufferSize = 0;
  Index      = 0;
  Status     = EFI_SUCCESS;

  CoreThreadMask = THREAD_MASK_64;        //default: bit mask up to 64 threads
  if(mNumOfBitShift == APICID_SHIFTBIT_32) {
    CoreThreadMask = THREAD_MASK_32;      //5bit mask up to 32 threads
  }

  if(!mCpuOrderSorted) {

  //Init mCpuIndexTable 
    for (CurrProcessor = 0; CurrProcessor < (MAX_THREAD * MAX_CORE); CurrProcessor++) {
        mCpuIndexTable[CurrProcessor] = (UINT32) -1;
    }

   Index  = 0;

   // currentSkt = 0;
   // tempCoreNum = 0;
   // currentCore = 0;
    for (CurrProcessor = 0; CurrProcessor < NumberOfCPUs; CurrProcessor++) {
      Status = MpService->GetProcessorInfo (
                                            MpService,
                                            CurrProcessor,
                                            &ProcessorInfoBuffer
                                            );

      if ((ProcessorInfoBuffer.StatusFlag & PROCESSOR_ENABLED_BIT) != 0) {
        if(ProcessorInfoBuffer.ProcessorId & 1) { //is 2nd thread
          CpuIdMapPtr = (EFI_CPU_ID_ORDER_MAP *)&mCpuApicIdOrderTable[(Index - 1) + MAX_CPU_NUM / 2];
        } else { //is primary thread       
          CpuIdMapPtr = (EFI_CPU_ID_ORDER_MAP *)&mCpuApicIdOrderTable[Index];
          Index++;
        }
   /**     if(ProcessorInfoBuffer.Location.Package != currentSkt) {
          currentSkt = ProcessorInfoBuffer.Location.Package;
          tempCoreNum = 0;      //reset current core number BUGBUG: as the CORE code order thread per ApicId, so we can use this
        }
        if(currentCore != ProcessorInfoBuffer.Location.Core) {
          tempCoreNum++;
          currentCore = ProcessorInfoBuffer.Location.Core;
        }**/
   //  DEBUG ((EFI_D_ERROR, "proc[%02x] ApicId %08X Pkg %02X, CORE: %02X, T: %x: tempCore: %02X\n", CurrProcessor,(UINT32)ProcessorInfoBuffer.ProcessorId,ProcessorInfoBuffer.Location.Package,ProcessorInfoBuffer.Location.Core,ProcessorInfoBuffer.Location.Thread,tempCoreNum));
        CpuIdMapPtr->ApicId  = (UINT32)ProcessorInfoBuffer.ProcessorId;
        CpuIdMapPtr->Flags   = ((ProcessorInfoBuffer.StatusFlag & PROCESSOR_ENABLED_BIT) != 0);
        /**CpuIdMapPtr->AcpiProcessorId = ((UINT32)ProcessorInfoBuffer.Location.Package * MAX_CORE * 2) + \
                                        + (tempCoreNum * 2) + (ProcessorInfoBuffer.Location.Thread);**/
        CpuIdMapPtr->AcpiProcessorId = CpuIdMapPtr->ApicId;
                                        
        CpuIdMapPtr->SwProcApicId = ((UINT32)(ProcessorInfoBuffer.Location.Package << mNumOfBitShift) + (((UINT32)ProcessorInfoBuffer.ProcessorId) & CoreThreadMask));
        CpuIdMapPtr->SocketNum = (UINT32)ProcessorInfoBuffer.Location.Package;
        if(gX2ApicEnabled) { //if X2Apic, re-order the socket # so it starts from base 0 and contiguous
          //may not necessory!!!!!
        }
      } else { //not enabled
        CpuIdMapPtr = (EFI_CPU_ID_ORDER_MAP *)&mCpuApicIdOrderTable[Index];
        CpuIdMapPtr->ApicId  = (UINT32)-1;
        CpuIdMapPtr->Flags   = 0;
        CpuIdMapPtr->AcpiProcessorId = (UINT32)-1;
        CpuIdMapPtr->SwProcApicId = (UINT32)-1;
        CpuIdMapPtr->SocketNum = (UINT32)-1;

      }
      
    }

    //make sure 1st entry is BSP
    if(gX2ApicEnabled) {
      BspApicId = (UINT32)AsmReadMsr64(0x802);
    } else {
      BspApicId = (*(volatile UINT32 *)(UINTN)0xFEE00020) >> 24;
    }

    if(mCpuApicIdOrderTable[0].ApicId != BspApicId) {
      //check to see if 1st entry is BSP, if not swap it
      Index = ApicId2SwProcApicId(BspApicId);

    if(MAX_CPU_NUM <= Index) {
	      DEBUG ((EFI_D_ERROR, "Asserting the SortCpuLocalApicInTable Index Bufferflow\n"));
      ASSERT_EFI_ERROR(EFI_INVALID_PARAMETER);
    }
      TempVal = mCpuApicIdOrderTable[Index].ApicId;
      mCpuApicIdOrderTable[Index].ApicId = mCpuApicIdOrderTable[0].ApicId;
      mCpuApicIdOrderTable[0].ApicId = TempVal;
      mCpuApicIdOrderTable[Index].Flags = mCpuApicIdOrderTable[0].Flags;
      mCpuApicIdOrderTable[0].Flags = 1;
      TempVal = mCpuApicIdOrderTable[Index].SwProcApicId;
      mCpuApicIdOrderTable[Index].SwProcApicId = mCpuApicIdOrderTable[0].SwProcApicId;
      mCpuApicIdOrderTable[0].SwProcApicId = TempVal;
      //swap AcpiProcId
      TempVal = mCpuApicIdOrderTable[Index].AcpiProcessorId;
      mCpuApicIdOrderTable[Index].AcpiProcessorId = mCpuApicIdOrderTable[0].AcpiProcessorId;
      mCpuApicIdOrderTable[0].AcpiProcessorId = TempVal;
      
    }
		
    //Make sure no holes between enabled threads
    for(CurrProcessor = 0; CurrProcessor < MAX_CPU_NUM; CurrProcessor++) {
  
      if(mCpuApicIdOrderTable[CurrProcessor].Flags == 0) {
        //make sure disabled entry has ProcId set to FFs
        mCpuApicIdOrderTable[CurrProcessor].ApicId = (UINT32)-1;
        mCpuApicIdOrderTable[CurrProcessor].AcpiProcessorId = (UINT32)-1;
        mCpuApicIdOrderTable[CurrProcessor].SwProcApicId = (UINT32)-1;
        
        for(Index = CurrProcessor+1; Index < MAX_CPU_NUM; Index++) {
          if(mCpuApicIdOrderTable[Index].Flags == 1) {
            //move enabled entry up
            mCpuApicIdOrderTable[CurrProcessor].Flags = 1;
            mCpuApicIdOrderTable[CurrProcessor].ApicId = mCpuApicIdOrderTable[Index].ApicId;
            mCpuApicIdOrderTable[CurrProcessor].AcpiProcessorId = mCpuApicIdOrderTable[Index].AcpiProcessorId;
            mCpuApicIdOrderTable[CurrProcessor].SwProcApicId = mCpuApicIdOrderTable[Index].SwProcApicId;
            mCpuApicIdOrderTable[CurrProcessor].SocketNum = mCpuApicIdOrderTable[Index].SocketNum;
            //disable moved entry
            mCpuApicIdOrderTable[Index].Flags = 0;
            mCpuApicIdOrderTable[Index].ApicId = (UINT32)-1;
            mCpuApicIdOrderTable[Index].AcpiProcessorId = (UINT32)-1;
            mCpuApicIdOrderTable[Index].SwProcApicId = (UINT32)-1;
            break;
          }
        }
      }
    }

  //Update mCpuIndexTable based on ProcessorBitMask (CPU0) 
    CurrProcessor = 0;
    for(Index = 0; Index < MAX_CPU_NUM; Index++) {
      if ((mCpuApicIdOrderTable[Index].Flags == 1) && (mCpuApicIdOrderTable[Index].SocketNum == 0))
      {        
        if(CurrProcessor < (MAX_THREAD*MAX_CORE) ){            
            mCpuIndexTable[CurrProcessor] = mCpuApicIdOrderTable[Index].ApicId;
            DEBUG ((EFI_D_ERROR, " mCpuIndexTable[%d] = %x \n", CurrProcessor, mCpuIndexTable[CurrProcessor]));
            CurrProcessor++;
        }
      }
    }

    mCpuOrderSorted = TRUE;
  }

  return Status;
}

///////////////////////////////////////////////////////////////////////////


SYSTEM_CONFIGURATION  gSystemConfiguration;
EFI_MP_SERVICES_PROTOCOL *gMpService;
UINT8 *gMadt;
UINT32 gMadtNumEntries = 0;
UINT32 gMadtTableSize;

#define MSR_CORE_THREAD_COUNT 0x35

static BOOLEAN CheckIfHtEnabled()
{
    UINT8   NumLogCPUs, NumCpuCores;
    UINT64  MsrData = AsmReadMsr64 (MSR_CORE_THREAD_COUNT);

    NumCpuCores = (UINT8)((UINT32)MsrData >> 16);
    NumLogCPUs = (UINT8)MsrData;

    if ((NumLogCPUs / NumCpuCores) <= 1) return FALSE;
    return TRUE;
}

static EFI_STATUS GetSetupandMpServices()
{
    UINTN  VariableSize = sizeof(gSystemConfiguration);
    EFI_STATUS Status;
    static BOOLEAN CollectedInfo = FALSE;

    if (CollectedInfo) return EFI_SUCCESS;

    VariableSize = sizeof (SYSTEM_CONFIGURATION);
      Status = gRT->GetVariable (
                                L"IntelSetup",
                                &gEfiSetupVariableGuid,
                                NULL,
                                &VariableSize,
                                &gSystemConfiguration
                                );
      if (EFI_ERROR(Status)) return Status;

       Status = gBS->LocateProtocol (
                       &gEfiMpServiceProtocolGuid,
                       NULL,
                       &gMpService
                       );
      if (EFI_ERROR(Status)) return Status;
      CollectedInfo = TRUE;
      return EFI_SUCCESS;
}


static VOID CreateLxApicEntry(
    IN OUT UINT8 **MadtEntry,
    IN UINT8 ProcId,
    IN UINT8 ApicId,
    IN UINT32 Flags
)
{
     EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC_STRUCTURE    *xApicEntry;
     xApicEntry = (EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC_STRUCTURE*)*MadtEntry;
     xApicEntry->Type = EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC;
     xApicEntry->Length = sizeof(EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC_STRUCTURE);
     xApicEntry->AcpiProcessorId = ProcId;
     xApicEntry->ApicId = ApicId;
     xApicEntry->Flags = Flags;
    *MadtEntry += sizeof(EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC_STRUCTURE);
}

static VOID CreateLxApicNmiEntry(
    IN OUT UINT8 **MadtEntry,
    IN UINT8 ProcId
)
{
    EFI_ACPI_4_0_LOCAL_APIC_NMI_STRUCTURE           *xApicNmiEntry;
    xApicNmiEntry = (EFI_ACPI_4_0_LOCAL_APIC_NMI_STRUCTURE*)*MadtEntry;
    xApicNmiEntry->Type = EFI_ACPI_4_0_LOCAL_APIC_NMI;
    xApicNmiEntry->Length =sizeof(EFI_ACPI_4_0_LOCAL_APIC_NMI_STRUCTURE);
    xApicNmiEntry->AcpiProcessorId = ProcId;
    xApicNmiEntry->Flags = (LAPIC_1_TRIGGER_MODE<<2) | LAPIC_1_POLARITY;
    xApicNmiEntry->LocalApicLint = LAPIC_1_DEST_LINTIN;
    *MadtEntry += sizeof(EFI_ACPI_4_0_LOCAL_APIC_NMI_STRUCTURE);
}

static VOID CreateLx2ApicEntry(
    IN OUT UINT8 **MadtEntry,
    IN UINT32  ProcId,
    IN UINT32 ApicId,
    IN UINT32 Flags
)
{
EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC_STRUCTURE   *x2ApicEntry;

    x2ApicEntry = (EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC_STRUCTURE*)*MadtEntry;
    x2ApicEntry->Type = EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC;
    x2ApicEntry->Length = sizeof(EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC_STRUCTURE);
    x2ApicEntry->Reserved[0] = 0;
    x2ApicEntry->Reserved[1] = 0;
    x2ApicEntry->X2ApicId = ApicId;
    x2ApicEntry->Flags = Flags;
    x2ApicEntry->AcpiProcessorUid = ProcId;
    *MadtEntry+=sizeof(EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC_STRUCTURE);
}

static VOID CreateLx2ApicNmiEntry(
    IN OUT UINT8 **MadtEntry,
    IN UINT32 ProcId
)
{
    EFI_ACPI_4_0_LOCAL_X2APIC_NMI_STRUCTURE     *x2ApicNmiEntry;

    x2ApicNmiEntry = (EFI_ACPI_4_0_LOCAL_X2APIC_NMI_STRUCTURE*)*MadtEntry;
    x2ApicNmiEntry->Type = EFI_ACPI_4_0_LOCAL_X2APIC_NMI;
    x2ApicNmiEntry->Length = sizeof(EFI_ACPI_4_0_LOCAL_X2APIC_NMI_STRUCTURE);
    x2ApicNmiEntry->Flags = (LAPIC_1_TRIGGER_MODE<<2) | LAPIC_1_POLARITY;
    x2ApicNmiEntry->AcpiProcessorUid = ProcId;
    x2ApicNmiEntry->LocalX2ApicLint = LAPIC_1_DEST_LINTIN;
    x2ApicNmiEntry->Reserved[0] = 0;
    x2ApicNmiEntry->Reserved[1] = 0;
    x2ApicNmiEntry->Reserved[2] = 0;
    *MadtEntry += sizeof(EFI_ACPI_4_0_LOCAL_X2APIC_NMI_STRUCTURE);
}

EFI_STATUS CreateCpuMadtEntries(IN UINT32 TableVer, OUT VOID **Entries, OUT UINT32 *NumEntries)
{
    EFI_STATUS Status;
    UINTN      NumberOfCPUs;
    UINTN      NumberOfEnabledCPUs;
    
    static BOOLEAN TableCreated = FALSE;

    if (!TableCreated) {
        INT32 i;
        UINT8* MadtEntry;

        AsmCpuidEx(EFI_CPUID_CORE_TOPOLOGY, 1, &mNumOfBitShift, NULL, NULL, NULL);
        mNumOfBitShift &= 0x1F;
        
        Status = GetSetupandMpServices();
        if (EFI_ERROR(Status)) return Status;

        gX2ApicEnabled =  gSystemConfiguration.ProcessorXapic;

        // Determine the number of processors
        gMpService->GetNumberOfProcessors (
                   gMpService,
                   &NumberOfCPUs,
                   &NumberOfEnabledCPUs
        );

       ASSERT (NumberOfCPUs <= MAX_CPU_NUM && NumberOfEnabledCPUs >= 1);

       Status = SortCpuLocalApicInTable(
           gMpService,
           NumberOfCPUs
       );
       ASSERT_EFI_ERROR(Status);

       
       //Allocate max possible entries in MADT. Note that x2Apic entries are bigger than xapic entries.
       gMadt = AllocatePool(MAX_CPU_NUM * (sizeof(EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC_STRUCTURE) + sizeof(EFI_ACPI_4_0_LOCAL_APIC_NMI_STRUCTURE)));

       MadtEntry = gMadt;
       for (i = 0; i < NumberOfCPUs; ++i) {
           if (mCpuApicIdOrderTable[i].ApicId <= 255) {           
               CreateLxApicEntry (
                   &MadtEntry,
                   mCpuApicIdOrderTable[i].AcpiProcessorId,
                   mCpuApicIdOrderTable[i].ApicId,
                   mCpuApicIdOrderTable[i].Flags
               );
               ++gMadtNumEntries;
           } else if (gX2ApicEnabled) {
               CreateLx2ApicEntry(
                   &MadtEntry,
                   mCpuApicIdOrderTable[i].AcpiProcessorId,
                   mCpuApicIdOrderTable[i].ApicId,
                   mCpuApicIdOrderTable[i].Flags
               );
               ++gMadtNumEntries;
           } else {
               //x2APIC will be disabled.
           }
           
       }
           
       for (i = 0; i < NumberOfCPUs; ++i) {
           if (mCpuApicIdOrderTable[i].ApicId <= 255) {           
               CreateLxApicNmiEntry (
                   &MadtEntry,
                   mCpuApicIdOrderTable[i].AcpiProcessorId
               );
               ++gMadtNumEntries;
           } else if (gX2ApicEnabled) {
               CreateLx2ApicNmiEntry(&MadtEntry,
                   mCpuApicIdOrderTable[i].AcpiProcessorId
               );
               ++gMadtNumEntries;
           } else {
               //x2APIC will be disabled.
           }
       }

       gMadtTableSize = (UINT32)(MadtEntry - gMadt);
       TableCreated = TRUE;
    }

    *Entries =  AllocatePool(gMadtTableSize);
    ASSERT(*Entries != NULL);
    
    CopyMem(*Entries, gMadt, gMadtTableSize);
    
    *NumEntries = gMadtNumEntries;
    
    return EFI_SUCCESS;
}

#define MP_TABLE_CPU_ENTRY_TYPE                  0

typedef struct _MP_TABLE_CPU_ENTRY
{
    UINT8   EntryType;
    UINT8   LocalApicId;
    UINT8   LocalApicVersion;
    struct
    {
        UINT8   CpuEnable    : 1;
        UINT8   CpuBootStrap : 1;
        UINT8   Reserved     : 6;
    } CpuFlags;
    UINT32  CpuSignature;
    UINT32  FeatureFlags;
    UINT32  Reserved[2];
} MP_TABLE_CPU_ENTRY;

typedef struct {
    UINT32 ProcNum;
    UINT32 ApicId;
    UINT32 StatusFlag;
} PROC_NUM_APICID;

PROC_NUM_APICID *gProcNumApicId;

VOID *gMpEntries;
UINT32 gMpNumEntries;
UINT32 gMpTableSize;

UINTN gNumberOfCPUs;

VOID FillMpEntry(IN OUT VOID  *Buffer)
{
	UINTN ProcessorNumber;
	UINT32 i;
	UINT32 MpTableIndex;
	UINT32 CpuSignature;
	UINT32 FeatureFlags;
	volatile MP_TABLE_CPU_ENTRY *Entry;

    gHtEnabled = CheckIfHtEnabled();
	
	gMpService->WhoAmI(gMpService, &ProcessorNumber);
	for (i = 0; i < (UINT32)gNumberOfCPUs;++i) {
		if ((UINT32)ProcessorNumber == gProcNumApicId[i].ProcNum) break;
	}
	ASSERT(i < gNumberOfCPUs);

	if (gHtEnabled) {
		if (gProcNumApicId[i].ApicId & 1) return;	//Don't create entry for Thread 1.
		MpTableIndex = i / 2;
	} else {
		MpTableIndex = i;
	}

    AsmCpuid (1, &CpuSignature, NULL, NULL, &FeatureFlags);

	Entry = &((volatile MP_TABLE_CPU_ENTRY*)gMpEntries)[MpTableIndex];
	Entry->EntryType = MP_TABLE_CPU_ENTRY_TYPE;
    Entry->LocalApicId = gProcNumApicId[i].ApicId;
    Entry->LocalApicVersion = 21;
    Entry->CpuFlags.CpuEnable = (gProcNumApicId[i].StatusFlag > 1) & 1;
    Entry->CpuFlags.CpuBootStrap = gProcNumApicId[i].StatusFlag & 1;
    Entry->CpuFlags.Reserved = 0;
    Entry->CpuSignature = CpuSignature;
    Entry->FeatureFlags = FeatureFlags;
    Entry->Reserved[0] = 0;
    Entry->Reserved[1] = 0;
}


EFI_STATUS CreateCpuMpTableEntries(OUT VOID **Entries, OUT UINT32 *NumEntries, OUT UINT32 *TableSize)
{
	EFI_STATUS Status;
	static BOOLEAN TableCreated = FALSE;

	if (!TableCreated) {
		EFI_PROCESSOR_INFORMATION ProcInfo;
		UINTN                         NumberOfEnabledCPUs;
		UINT32 i;

		Status = GetSetupandMpServices();
		if (EFI_ERROR(Status)) return Status;

		gHtEnabled = CheckIfHtEnabled();

		// Determine the number of processors
		gMpService->GetNumberOfProcessors (
	               gMpService,
	               &gNumberOfCPUs,
	               &NumberOfEnabledCPUs
	               );

		gMpNumEntries = gHtEnabled ? (UINT32)gNumberOfCPUs / 2 : (UINT32)gNumberOfCPUs;
		gMpTableSize = gMpNumEntries * sizeof(MP_TABLE_CPU_ENTRY);
		gMpEntries = AllocatePool(gMpTableSize);
		ASSERT(gMpEntries != NULL);

		gProcNumApicId = AllocatePool(gNumberOfCPUs * sizeof(PROC_NUM_APICID));
		ASSERT(gProcNumApicId != NULL);

		for (i = 0; i < (UINT32)gNumberOfCPUs;++i) {
			gMpService->GetProcessorInfo(gMpService, i, &ProcInfo);
			gProcNumApicId[i].ProcNum = i;
			gProcNumApicId[i].ApicId = (UINT32)ProcInfo.ProcessorId;
			gProcNumApicId[i].StatusFlag = ProcInfo.StatusFlag;
		}

	    //Sort entries. Maybe pre-sorted, thus no sort done.
	    for (i = 0; i < gNumberOfCPUs - 1; ++i) {
	        UINT32 i2;
	        BOOLEAN Sorted = TRUE;
	        for (i2 = 0; i2 < gNumberOfCPUs - 1; ++i2) {
	            if (gProcNumApicId[i2].ApicId > gProcNumApicId[i2 + 1].ApicId) {
	            	PROC_NUM_APICID Temp;
	                Temp.ProcNum = gProcNumApicId[i2].ProcNum;
	                Temp.ApicId = gProcNumApicId[i2].ApicId;
	                Temp.StatusFlag = gProcNumApicId[i2].StatusFlag;
	                gProcNumApicId[i2].ProcNum = gProcNumApicId[i2+1].ProcNum;
	                gProcNumApicId[i2].ApicId = gProcNumApicId[i2+1].ApicId;
	                gProcNumApicId[i2].StatusFlag = gProcNumApicId[i2+1].StatusFlag;
	                gProcNumApicId[i2+1].ProcNum = Temp.ProcNum;
	                gProcNumApicId[i2+1].ApicId = Temp.ApicId;
	                gProcNumApicId[i2+1].StatusFlag = Temp.StatusFlag;
	                Sorted = FALSE;
	            }
	        }
	        if (Sorted) break;
	    }

	    FillMpEntry(NULL);
	    gMpService->StartupAllAPs(gMpService, FillMpEntry, FALSE, NULL, 0, NULL, NULL);

		TableCreated = TRUE;
	}

	*NumEntries = gMpNumEntries;
	*TableSize = gMpTableSize;
	*Entries = AllocatePool(gMpTableSize);
	ASSERT(*Entries != NULL);
	CopyMem(*Entries, gMpEntries, gMpTableSize);

	return EFI_SUCCESS;
}


