#include "AmiCpuInfo.h"
#include <Library/PciExpressLib.h>
#include <Token.h>

#define DELAY_CALCULATE_CPU_PERIOD  200     //uS

UINT32  NumberOfCpuSocketsPopulated()
{

#if NUMBER_CPU_SOCKETS > 1
    UINT32                  NumCpuSockets = 0;
    UINT16                  CoreBus = 0;            // Start on bus 0
    UINT16                  Value;

	// Get the number of populated sockets
	do {
		// Read the Uncore Bus number for the current CPU
		Value = PciExpressRead16(PCI_EXPRESS_LIB_ADDRESS(CoreBus,5,0,0x108));
		if (Value != 0xffff) {
			Value >>= 8;
			CoreBus = Value + 1;
			NumCpuSockets++;
		} else
			Value = 0xff;

	} while (Value != 0xff);

	return NumCpuSockets;
#else
    return 1;
#endif
}

BOOLEAN IsHt0()
{
    UINT32 ApicMask;
    UINT32 ApicId;
    UINT8 ThreadsPerCore;
    UINT32 RegEbx;

    AsmCpuidEx(0xb, 0, NULL, &RegEbx, NULL, NULL);
    ThreadsPerCore = (UINT8)RegEbx;
    
    if (ThreadsPerCore < 2) return TRUE;    //Check if Ht Capable.
    ApicMask = ThreadsPerCore - 1;

    AsmCpuid(1, NULL, &RegEbx, NULL, NULL);
    ApicId = RegEbx >> 24; 

    //Use APIC ID to determine if logical CPU.
    if ((ApicId & ApicMask) == 0) return TRUE;  //All logical CPU0 will have bit 0 clear.
    return FALSE;
}


UINT8 NumSupportedCpuCores()
{
    UINT32 RegEbx;
    UINT8  TotLogicalCpus;
    UINT8  LogicalCpusPerCore;

    AsmCpuidEx(0xb, 1, NULL, &RegEbx, NULL, NULL);
    TotLogicalCpus  = (UINT8)RegEbx;

    AsmCpuidEx(0xb, 0, NULL, &RegEbx, NULL, NULL);
    LogicalCpusPerCore  = (UINT8)RegEbx;

    return TotLogicalCpus / LogicalCpusPerCore;
}


UINT8 NumSupportedThreadsPerCore()
{
    UINT32  RegEbx;

    AsmCpuidEx(0xb, 0, NULL, &RegEbx, NULL, NULL);
    return (UINT8)RegEbx;
}

BOOLEAN IsHtEnabled()
{
    UINT32  MsrData = (UINT32)AsmReadMsr64(MSR_CPU_INFO_CORE_THREAD_COUNT);
    UINT8   NumCpuCores = (UINT8)(MsrData >> 16);
    UINT8   NumLogCPUs = (UINT8)MsrData;
    
    if ((NumLogCPUs / NumCpuCores) <= 1) return FALSE;
    return TRUE;
}
