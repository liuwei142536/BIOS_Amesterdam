/******************************************************************************
Copy To Flash Sample Code 

Copyright 2011 Intel Corporation All Rights Reserved.

The source code, information and material ("Material") contained herein
is owned by Intel Corporation or its suppliers or licensors, and title
to such Material remains with Intel Corporation or its suppliers or licensors.
The Material contains proprietary information of Intel or its suppliers and 
licensors. The Material is protected by worldwide copyright laws and treaty 
provisions. No part of the Material may be used, copied, reproduced, modified,
published, uploaded, posted, transmitted, distributed or disclosed in any way
without Intel's prior express written permission. No license under any patent,
copyright or other intellectual property rights in the Material is granted to
or conferred upon you, either expressly, by implication, inducement, estoppel
or otherwise. Any license under such intellectual property rights must be 
express and approved by Intel in writing.

Unless otherwise agreed by Intel in writing, you may not remove or alter this
notice or any other notice embedded in Materials by Intel or Intel’s suppliers
or licensors in any way.

******************************************************************************/

#if !defined(EDK_RELEASE_VERSION) || (EDK_RELEASE_VERSION < 0x00020000)
	#include <Library/DebugLib.h>
	#include <Library/IoLib.h>
	#include <Library/BaseMemoryLib.h>
	#include <Library/HobLib.h>
	#include <Library/UefiBootServicesTableLib.h>
	#include <Library/UefiRuntimeServicesTableLib.h>
	#include <Guid\SetupVariable.h>
	#include <Guid/HobList.h>
	//
	// Consumed Protocols
	//
	#include <Protocol/Cpu.h>
	#include <Protocol/Timer.h>
	

#endif

#include "c2f.h"

extern void isr();
extern void AsmSti();
extern void AsmPause();

UINT8 PortVal = 0x50;
UINT64 mOldTimer=0;
///
/// Pointer to the CPU Architectural Protocol instance
///
EFI_CPU_ARCH_PROTOCOL     *mCpu;
EFI_TIMER_ARCH_PROTOCOL   *mTimer;

/**
  ACHI Interrupt Handler

  @param[in] InterruptType        The type of interrupt that occured
  @param[in] SystemContext        A pointer to the system context when the interrupt occured

  @retval None
**/

VOID
EFIAPI
AhciInterruptHandler (
  IN EFI_EXCEPTION_TYPE   InterruptType,
  IN EFI_SYSTEM_CONTEXT   SystemContext
  )
{
	UINT32 Data32;

	IoWrite8(0x80, PortVal++);
	// Clear the PxIS register
	Data32  = MmioRead32 (ABAR_LOC + AHCI_ABAR_PxIS + (PORT_NUM * 0x80));
	MmioWrite32 ((ABAR_LOC + AHCI_ABAR_PxIS + (PORT_NUM * 0x80)), Data32);

	// Clear the Master IS register
	Data32  = MmioRead32 (ABAR_LOC + AHCI_ABAR_IS_MASTER);
	MmioWrite32 ((ABAR_LOC + AHCI_ABAR_IS_MASTER), Data32);

	// Write the EOI register in the local APIC
	Data32  = MmioRead32 (APIC_BASE + APIC_EOI);
	MmioWrite32 ((APIC_BASE + APIC_EOI), Data32);

	*((volatile UINTN*)(UINTN)MONITOR_LOC) = 1;
	
	IoWrite8(0x80, PortVal++);
}

void spinloop(UINT32 count)
{
	volatile UINT32 i;
	for (i = 0; i < count; i++);
}


void shutdown_platform()
{
	gRT->ResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);

}

UINT32 get_upper32(UINT64 value)
{
	UINT32 upper;
	upper = (UINT32)((value & 0xffffffff00000000) >> 32);
	#if 0
	_asm {
		mov eax, dword ptr value[4]
		mov upper, eax
	}
	#endif
	return upper;
}

UINT32 get_lower32(UINT64 value)
{
	UINT32 lower;
	lower = (UINT32)(value & 0xffffffff);
	#if 0
	_asm {
		mov     eax, dword ptr value[0]
		mov lower, eax
	}
	#endif
	return lower;
}

//Prints a 64-bit number
void print64( UINT64 number)
{
	if (number >= 0x100000000)
		DEBUG ((EFI_D_ERROR, "0x%x%08x", get_upper32(number),get_lower32(number)));
	else
		DEBUG ((EFI_D_ERROR, "0x%08x", get_lower32(number)));
}

// Prints a 64-bit number with a newline at the end
void print64n( UINT64 number)
{
	print64( number);
	DEBUG ((EFI_D_ERROR, "\n"));    
}

void dumpdata( void * start, UINT32 size)
{
	UINT32 count;
	UINT32 * pointer;

	DEBUG ((EFI_D_ERROR, "Dumping 0x%x bytes starting at 0x%08x\n", size, (UINTN*)start ));

	for (count = 0; count < size; count+=4)
	{
		pointer = (UINT32*)((UINT8*)start + count);
		DEBUG ((EFI_D_ERROR, " 0x%08x : 0x%08x\n",pointer, *pointer));
	}
}


void memclear(void * start, UINT32 size)
{

	UINT32 count;
	UINT8 * volatile pointer; //Use volatile to avoid intrinsic memset substitution by compiler
#ifdef EXTRA_DEBUG
	DEBUG ((EFI_D_ERROR, "Clearing 0x%x bytes starting at 0x%08x\n",size,start));
#endif
	for (count = 0; count < size; count++)
	{
		pointer = ((UINT8*)start + count);
		*pointer = 0;
	}


}

void buildIdentifyCommand( struct CommandHeader * CommandHeaderEntry, struct CommandTable * command, UINTN destination)
{
	void *dest = (void *)(UINTN) destination;
	DEBUG ((EFI_D_ERROR, "Building IDENTIFY descriptor\n"));

	memclear(command,sizeof(struct CommandTable));
	memclear(CommandHeaderEntry,sizeof(struct CommandHeader));
	memclear(dest,512);

	command->cfis.FISType = 0x27; //Register H2D FIS
	command->cfis.MiscFlags = 0x80;	// Set "C" Bit
	command->cfis.Command = 0xEC; // Identify

	command->prdt[0].DBA = (UINT32)(UINTN)dest;
	command->prdt[0].DBAU = 0;
	command->prdt[0].Reserved = 0;
	command->prdt[0].DBC = (512 - 1);  // 0 Based

	CommandHeaderEntry->Misc = 5;
	CommandHeaderEntry->PRDTL = 1;
	CommandHeaderEntry->PRDBC = 0;
	CommandHeaderEntry->CTBA = (UINT32)(UINTN)(command);
//	CommandHeaderEntry->CTBAU = (UINT32)((UINT64)command >> 32);
	CommandHeaderEntry->CTBAU = 0;
}

void buildSetDMACommand( struct CommandHeader * CommandHeaderEntry, struct CommandTable * command, UINT8 DMAMode)
{
#ifdef EXTRA_DEBUG
	DEBUG ((EFI_D_ERROR, "Building Set DMA Mode descriptor, new mode=%d\n",DMAMode));
#endif
	memclear(command,sizeof(struct CommandTable));
	memclear(CommandHeaderEntry,sizeof(struct CommandHeader));

	command->cfis.FISType = 0x27; //Register H2D FIS
	command->cfis.MiscFlags = 0x80;	// Set "C" Bit
	command->cfis.Command = 0xEF; // SET FEATURES
	command->cfis.Features = 0x3;
	command->cfis.SectorCount = (0x40 | DMAMode);

	CommandHeaderEntry->Misc = 5;
	CommandHeaderEntry->PRDTL = 0;
	CommandHeaderEntry->PRDBC = 0;
	CommandHeaderEntry->CTBA = (UINT32)(UINTN)command;
//	CommandHeaderEntry->CTBAU = (UINT32)((UINT64)command >> 32);
	CommandHeaderEntry->CTBAU = 0;
}


void buildDMACommand( struct CommandHeader * CommandHeaderEntry, struct CommandTable * command, UINT64 hostaddr, UINT32 lba, UINT32 sectorcount, UINT32 sectorsize, UINT8 intflag, UINT8 write)
{
	UINT32 bytesleft;
	UINT16 prdtcount;

#ifdef EXTRA_DEBUG
	if (write)
		DEBUG ((EFI_D_ERROR, "Building DMA Write descriptor\n"));
	else
		DEBUG ((EFI_D_ERROR, "Building DMA Read descriptor\n"));
	DEBUG ((EFI_D_ERROR, "Host Address="));
	print64( hostaddr);
	DEBUG ((EFI_D_ERROR, " LBA=0x%08x sectorcount=%d,sectorsize=%d,intflag=%d\n",lba,sectorcount,sectorsize,intflag));
#endif

	memclear(command,sizeof(struct CommandTable));
	memclear(CommandHeaderEntry,sizeof(struct CommandHeader));

	command->cfis.FISType = 0x27; //Register H2D FIS
	command->cfis.MiscFlags = 0x80;	// Set "C" Bit

	if (write)
		command->cfis.Command = 0x3D; // Write DMA Ext
	else
		command->cfis.Command = 0x25; // Read DMA Ext


	command->cfis.Features = 0;	// ?
	command->cfis.FeaturesExp = 0; // ?
	command->cfis.LBALow = (lba & 0xFF);
	command->cfis.LBAMid = ((lba >> 8) & 0xFF);
	command->cfis.LBAHigh = ((lba >> 16) & 0xFF);
	command->cfis.LBALowExp = ((lba >> 24) & 0xFF);
	command->cfis.LBAMidExp = 0;
	command->cfis.LBAHighExp = 0;
	command->cfis.Device = (1 << 6) | (1 << 4);
	command->cfis.SectorCount = (sectorcount & 0xFF);
	command->cfis.SectorCountExp = ((sectorcount >> 8) & 0xFF);
	command->cfis.Reserved = 0;
	command->cfis.Control = 0;

	bytesleft = sectorcount * sectorsize;
	prdtcount = 0;

	while (bytesleft)
	{
		command->prdt[prdtcount].DBA = get_lower32(hostaddr);
		command->prdt[prdtcount].DBAU = get_upper32(hostaddr);
		command->prdt[prdtcount].Reserved = 0;
		if (bytesleft > (MAX_PRDT_BYTECOUNT))
		{
			command->prdt[prdtcount].DBC = ((MAX_PRDT_BYTECOUNT) - 1);	// 0 Based
			bytesleft -= MAX_PRDT_BYTECOUNT;
			hostaddr += MAX_PRDT_BYTECOUNT;
		} else
		{
			command->prdt[prdtcount].DBC = (bytesleft - 1);	 // 0 Based
			bytesleft = 0;
		}
		prdtcount++;
	}

	if (intflag)
	{
		command->prdt[prdtcount-1].DBC |= (1 << 31);
	}

	CommandHeaderEntry->Misc = (write << 6) | 5;
	CommandHeaderEntry->PRDTL = prdtcount;
	CommandHeaderEntry->PRDBC = 0;
	CommandHeaderEntry->CTBA = (UINT32)(UINTN)command;
//	CommandHeaderEntry->CTBAU = (UINT32)((UINT64)command >> 32);
	CommandHeaderEntry->CTBAU = 0;
}

#if 0
__declspec( naked ) void isr(void)
{
	_asm {
		push eax
		push ebx

		mov al,55h
		out 80h, al
		//mov eax, 0xf8000110	// Clear the PxIS register
		mov eax, (ABAR_LOC + AHCI_ABAR_PxIS + (PORT_NUM * 0x100))
		mov ebx, [eax]
		mov [eax], ebx

		//mov eax, 0xf8000008	// Clear the Master IS register
		mov eax, (ABAR_LOC + AHCI_ABAR_IS_MASTER)
		mov ebx, [eax]
		mov [eax], ebx

		//mov eax, 0xfee000b0	// Write the EOI register in the local APIC
		mov eax, (APIC_BASE + APIC_EOI)
		mov [eax], 0

		//mov eax, 0xfec00040     // Write the EOI regsiter in the IO-APIC
		mov eax, (IOAPIC_BASE + IOAPIC_EOI)		// Write the EOI regsiter in the IO-APIC
		mov [eax], VECTOR_NUM

		mov eax, MONITOR_LOC	// Write the monitor location
		mov [eax], 1

		pop ebx
		pop eax
		iretd
	}
}
#endif

void sipiroutine(void)
{
	while (1)
	{
		_mm_monitor(SIPI_MONITOR_LOC,0,0);
		_mm_mwait(0x0,0x1);
	}
}

UINT32
GetApicID (
  VOID
)
{
  EFI_CPUID_REGISTER  CpuidRegisters;

  AsmCpuid (EFI_CPUID_VERSION_INFO, &CpuidRegisters.RegEax, &CpuidRegisters.RegEbx, &CpuidRegisters.RegEcx, &CpuidRegisters.RegEdx);
  return (UINT32)(CpuidRegisters.RegEbx >> 24);
}

UINT32
GetCpuFamilyName(
  VOID
)
{
UINT32 RegEax = 0;
AsmCpuid (EFI_CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
return (UINT32)(RegEax >> 4);
}

void DisableMSI()
{
	volatile UINT8 *pointer8;
	volatile UINT16 *pointer16;
	volatile UINT32 *pointer32;
	UINT16 MsgCtrl = 0;
	UINT8 CapPtr;

	// Uninstall the interrupt handler
	mCpu->RegisterInterruptHandler (mCpu, VECTOR_NUM, NULL);

	pointer8 = (UINT8*)(UINTN)(MMCONFIG_BASE | (AHCI_BUS << 20) | (AHCI_DEV << 15) | (AHCI_FUN << 12) | AHCI_DEV_CAP);
	CapPtr = *pointer8;

	// Disable MSI
	DEBUG((EFI_D_ERROR, "Clear Generation of MSI\n"));
	pointer16 = (UINT16*)(UINTN)(MMCONFIG_BASE | (AHCI_BUS << 20) | (AHCI_DEV << 15) | (AHCI_FUN << 12) | CapPtr + 2);
	*pointer16 = 0;

	// Clear MSI msg register
	DEBUG((EFI_D_ERROR, "Clear MSI Message Register\n"));
	pointer32 = (UINT32*)(UINTN)(MMCONFIG_BASE | (AHCI_BUS << 20) | (AHCI_DEV << 15) | (AHCI_FUN << 12) | CapPtr + 4);
	*pointer32 = 0;

	// Clear MSI Data register
	DEBUG((EFI_D_ERROR, "Clear MSI Data Register\n"));
	pointer16 = (UINT16*)(UINTN)(MMCONFIG_BASE | (AHCI_BUS << 20) | (AHCI_DEV << 15) | (AHCI_FUN << 12) | CapPtr + 8);
	*pointer16 = 0;


	// Enable PCI Interrupt and disable bus master
	DEBUG((EFI_D_ERROR, "Enable PCI Interrupts and disable bus master\n"));
	pointer16 = (UINT16*)(UINTN)(MMCONFIG_BASE | (AHCI_BUS << 20) | (AHCI_DEV << 15) | (AHCI_FUN << 12) | 04);
	MsgCtrl = *pointer16;
	MsgCtrl &= ~(1 << 10 | 1 << 2) ; // Interrupt Enable. Bus master disable
	*pointer16 = MsgCtrl;

}

UINT32 EnableMSI()
{
	volatile UINT16 *pointer16;
	volatile UINT8 *pointer8;
	volatile UINT32 *pointer32;

	UINT8 CapPtr;
	UINT32 MsgAdr = 0xFEE00000;
	UINT16 MsgData = 0;
	UINT16 MsgCtrl = 0;
	
	// 1.Read the capabilities List Pointer to find the location of the first capabilities structure
	pointer8 = (UINT8*)(UINTN)(MMCONFIG_BASE | (AHCI_BUS << 20) | (AHCI_DEV << 15) | (AHCI_FUN << 12) | AHCI_DEV_CAP);
	CapPtr = *pointer8;

	DEBUG((EFI_D_ERROR, "Read Capabilities list Pointer addr = 0x%x, value = 0x%x\n", pointer8, CapPtr));
	pointer16 = (UINT16*)(UINTN)(MMCONFIG_BASE | (AHCI_BUS << 20) | (AHCI_DEV << 15) | (AHCI_FUN << 12) | CapPtr);
	if(((*pointer16) & 0xFF) != 0x5) // Not MSI Capable
	{
		DEBUG((EFI_D_ERROR, "Device not MSI capable\n"));
		return 0;
	}
	MsgAdr |= ((UINT8)GetApicID()) << 12;
	DEBUG((EFI_D_ERROR, "Destination ID = 0x%x\n", MsgAdr));
	
	pointer32 = (UINT32*)(UINTN)(MMCONFIG_BASE | (AHCI_BUS << 20) | (AHCI_DEV << 15) | (AHCI_FUN << 12) | CapPtr + 4);
	*pointer32 = MsgAdr;
	DEBUG((EFI_D_ERROR, "Program MsgAdr Register = 0x%x, Vallue = 0x%x\n", pointer32, MsgAdr));

	MsgData |= VECTOR_NUM; // All other fields are zero, fixed mode, edge triggered
	pointer16 = (UINT16*)(UINTN)(MMCONFIG_BASE | (AHCI_BUS << 20) | (AHCI_DEV << 15) | (AHCI_FUN << 12) | CapPtr + 8);
	*pointer16 = MsgData;
	DEBUG((EFI_D_ERROR, "Program MsgData Register = 0x%x, Vallue = 0x%x\n", pointer16, MsgData));
	
	// Enable MSI enable bit
	//
	DEBUG((EFI_D_ERROR, "Enable MSI\n"));
	pointer16 = (UINT16*)(UINTN)(MMCONFIG_BASE | (AHCI_BUS << 20) | (AHCI_DEV << 15) | (AHCI_FUN << 12) | CapPtr + 2);
	MsgCtrl = *pointer16;
	MsgCtrl |= 1;
	*pointer16 = MsgCtrl;

	DEBUG((EFI_D_ERROR, "Disable Interrupts in PCI Command register\n"));
	pointer16 = (UINT16*)(UINTN)(MMCONFIG_BASE | (AHCI_BUS << 20) | (AHCI_DEV << 15) | (AHCI_FUN << 12) | 04);
	MsgCtrl = *pointer16;
	MsgCtrl |= 1 << 10 ; // Interrupt Disable. Bus master enable
	DEBUG((EFI_D_ERROR, "Addr = 0x%x, Value = 0x%x\n", pointer16, MsgCtrl));
	*pointer16 = MsgCtrl;
	MsgCtrl = *pointer16;
	MsgCtrl |= 1 << 2;
	*pointer16 = MsgCtrl;
	DEBUG((EFI_D_ERROR, "Readback Value = 0x%x\n", *pointer16));
	return 1;
}

VOID AhciCleanup(UINT32 abar)
{
	volatile UINT32 *pointer32;
	
	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxCLB + (PORT_NUM * 0x80));
	*pointer32 = 0;
	
	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxCLBU + (PORT_NUM * 0x80));
	*pointer32 = 0;

	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxFB + (PORT_NUM * 0x80));
	*pointer32 = 0;

	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxFBU + (PORT_NUM * 0x80));
	*pointer32 = 0;

	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_GHC);
	*pointer32 = 1; // Reset the controller

	pointer32 = (UINT32*)(UINTN)(MMCONFIG_BASE | (AHCI_BUS << 20) | (AHCI_DEV << 15) | (AHCI_FUN << 12) | AHCI_REG_ABAR);
	*pointer32 = 0;	// Clear ABAR
}

EFI_STATUS HookInterrupt()
{
	EFI_STATUS Status;

	Status = gBS->LocateProtocol (&gEfiTimerArchProtocolGuid, NULL, (VOID **) &mTimer);
	ASSERT_EFI_ERROR (Status);
	if(Status != EFI_SUCCESS)
	{
		DEBUG((EFI_D_ERROR, "Error Locating Timer Arch Protocol Timer will not be disabled. Error = %r\n", Status));
	}

	mTimer->GetTimerPeriod(mTimer, &mOldTimer);
	mTimer->SetTimerPeriod(mTimer, 0);

	Status = mCpu->RegisterInterruptHandler (mCpu, VECTOR_NUM, AhciInterruptHandler);
	ASSERT_EFI_ERROR (Status);
	if(Status != EFI_SUCCESS)
	{
		DEBUG((EFI_D_ERROR, "Error Installing the Interrupt Handler. Error = %r\n", Status));
	}
	return Status;
}

#if 0
void hookInterrupt()
{

	UINT8 buffer[16];
	struct InterruptGate * intstruct;
	UINT64 isrloc;
//	UINT8 * pointer8;
//	UINT32 * pointer32;
//	UINT32 data32;
	//UINT8 vectornum;


	memclear(buffer,16);

#if 0
	pointer8 = (UINT8*)(IOAPIC_BASE + IOAPIC_INDEX);
	pointer32 = (UINT32*)(IOAPIC_BASE + IOAPIC_DATA);

	// Set the APIC ID for Patsburg APIC incase it isn't set yet.
	*pointer8 = 0;
	*pointer32 = (IOAPIC_AID << 24);

	*pointer8 = (UINT8)(0x10 + (INT_NUM * 2) + 1);
	*pointer32 = 0;

	*pointer8 = (UINT8)(0x10 + (INT_NUM * 2));
/*
	data32 = *pointer32;
	data32 &= 0x000000FF;
	vectornum = (UINT8)data32;
*/
	data32 = VECTOR_NUM;
	//data32 |= (1 << 15); // Set to Level Mode
	// Set IRQs 16-23 to active low
	if (INT_NUM > 15)
		data32 |= (1 << 13);
	*pointer32 = data32;
	data32 = *pointer32;
#endif
	__sidt(buffer);


#ifdef EXTRA_DEBUG
	DEBUG ((EFI_D_ERROR, "SIDT results\n"));
	dumpdata(buffer,16);
	
#endif
	intstruct = *((struct InterruptGate **)(&buffer[2]));

	DEBUG ((EFI_D_ERROR, "\nidt location=0x%08x\n", (UINT32)(UINTN)intstruct));

#ifdef EXTRA_DEBUG
	DEBUG ((EFI_D_ERROR, "Dumping AHCI Interrupt Gate\n"));
	dumpdata(intstruct,16);
#endif
	intstruct+= VECTOR_NUM;
	//isrloc = (((UINT32)intstruct->UpperOffset << 16)) + intstruct->LowerOffset;
	isrloc = ((UINTN)intstruct->UpperOffset << 32) + ((UINTN)intstruct->MiddleOffset << 16) + intstruct->LowerOffset;
	DEBUG ((EFI_D_ERROR, "Current Interrupt Loccation = 0x%08x\n",isrloc));

	
	isrloc = (UINTN)&isr;

	DEBUG ((EFI_D_ERROR, "Setting Interrupt Loccation = 0x%08x\n",isrloc));

	intstruct->LowerOffset = isrloc & 0xFFFF;
	intstruct->MiddleOffset = (isrloc >> 16) & 0xFFFF;
	intstruct->UpperOffset = (isrloc >> 32) & 0xFFFFFFFF;
	intstruct->Segment = 0x38;
	intstruct->Misc = (1 << 9) | (1 << 10) | (1 << 11) | (1 << 15);
	intstruct->Rsvd = 0;

#ifdef EXTRA_DEBUG
	DEBUG ((EFI_D_ERROR, "Dumping AHCI Interrupt Gate\n"));
	dumpdata( intstruct, 16);
#endif
}

#endif



// Returns Pointer to abar
UINT32 configureAHCI()
{
	volatile UINT16 * pointer16;
	volatile UINT32 * pointer32;

	UINT16 data16;
	UINT32 data32;
	UINT32 capregister;

	UINT32 abar;

#ifdef EXTRA_DEBUG
	DEBUG ((EFI_D_ERROR, "In ConfigureAHCI1\n"));
#endif

	// Program ABAR
	pointer32 = (UINT32*)(UINTN)(MMCONFIG_BASE | (AHCI_BUS << 20) | (AHCI_DEV << 15) | (AHCI_FUN << 12) | AHCI_REG_ABAR);
	abar = *pointer32;
	if (abar > 0x1)
	{
		DEBUG ((EFI_D_ERROR, "ABAR already programmed to 0x%08x\n", abar));
	} else
	{
		DEBUG ((EFI_D_ERROR, "ABAR not programmed yet\n"));
		abar = ABAR_LOC;
		*pointer32 = abar;
		DEBUG ((EFI_D_ERROR, "ABAR programmed to 0x%08x\n", abar));
	}

	// Set the PCI Command register
	pointer16 = (UINT16*)(UINTN)(MMCONFIG_BASE | (AHCI_BUS << 20) | (AHCI_DEV << 15) | (AHCI_FUN << 12) | AHCI_REG_PCICMD);
	*pointer16 |= 0x6;

	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_CAP);
	capregister = *pointer32;

    pointer16 = (UINT16*)(UINTN)(MMCONFIG_BASE | (AHCI_BUS << 20) | (AHCI_DEV << 15) | (AHCI_FUN << 12) | AHCI_REG_PCS);
    data16 = *pointer16;
    data16 &= 0x3F00;
    data16 = data16 >> 8;
    data16 |= (1<<15);
    *pointer16 = data16;

    pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PI);
    *pointer32 = (UINT32)data16;
    spinloop(100000);
    data32 = *pointer32;
    spinloop(100000);
    data32 = *pointer32;
    spinloop(100000);


	capregister &= ~(AHCI_ABAR_CAP_SIS | AHCI_ABAR_CAP_SSS | AHCI_ABAR_CAP_SALP | 
					 AHCI_ABAR_CAP_PMS | AHCI_ABAR_CAP_SSC | AHCI_ABAR_CAP_PSC |
					 AHCI_ABAR_CAP_SXS | AHCI_ABAR_CAP_ISS_MASK);

	capregister |= (AHCI_ABAR_CAP_SCLO | AHCI_ABAR_CAP_PMD | (AHCI_ABAR_CAP_ISS_3_0_G << AHCI_ABAR_CAP_ISS));

	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_CAP);
	*pointer32 = capregister;


	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxCMD + (PORT_NUM * 0x80));
	data32 = 1 << 23;
	*pointer32 = data32;

	// BWG Section 13.1.4
	// Clear VSP Bits
	/*
	pointer32 = (UINT32*)(abar + AHCI_ABAR_VSP);
	data32 = *pointer32;
	data32 &= ~((1 << 2) | (1 << 0));
	*pointer32 = data32;
	*/
	// Clear CAP2 Bits
	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_CAP2);
	data32 = *pointer32;
	//data32 &= ~(1 << 1);
	data32 |= (1 << 2);
	*pointer32 = data32;

	pointer16 = (UINT16*)(UINTN)(MMCONFIG_BASE | (AHCI_BUS << 20) | (AHCI_DEV << 15) | (AHCI_FUN << 12) | AHCI_REG_PCS);
	data16 = *pointer16;
	if (data16 & (1 << (PORT_NUM + 8)))
	{
		DEBUG ((EFI_D_ERROR, "Device present on port %d, PCS=0x%x\n",PORT_NUM,data16));
	} else
	{
		DEBUG ((EFI_D_ERROR, "No Device present on port %d\n",PORT_NUM));
		DEBUG ((EFI_D_ERROR, "Exiting\n"));
		return 0;
	}


	// Step 5 Clear any previous pending interrupts
	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxIS + (PORT_NUM * 0x80));
	data32 = *pointer32;
	*pointer32 = data32;

	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_IS_MASTER);
	data32 = *pointer32;
#ifdef EXTRA_DEBUG
	DEBUG ((EFI_D_ERROR, "Master IS = 0x%08x\n",data32));
#endif
	*pointer32 = data32;

	// Step 6: Set GHC.31 to 1(Section 14.4.1.2 of Ibex EDS) and enable global interrupt flag
	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_GHC);
	*pointer32 |= (1 << 31) | ( 1<< 1);
	return abar;
}




UINT32 runCommands( UINT32 abar, struct CommandHeader * CommandHeaderEntry, struct ReceivedFIS * ReceivedFISArea, UINT32 commandMask)
{
	volatile UINT32 * pointer32;
	volatile UINT64 * pointer64;
	volatile UINT32 * monitorloc;
	UINT32 data32;
	UINT8 lastcommand;
	volatile int a = 5;

	UINT8 i;
	UINT16 CpuFamily = 0;
	
	monitorloc = (UINT32*)(UINTN)MONITOR_LOC;
	*monitorloc = 0;

	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxCMD + (PORT_NUM * 0x80));
	data32 = *pointer32;

	if (data32 & ( 1 << 15))
	{
		DEBUG ((EFI_D_ERROR, "Waiting for CMD.CR to be clear\n"));
		while ((*pointer32 & (1 << 15)));
	}

	if (data32 & ( 1 << 14))
	{
		DEBUG ((EFI_D_ERROR, "Waiting for CMD.FR to clear\n"));
		while ((*pointer32 & (1 << 14)));
	}

	// Set Command Base Address
	pointer64 = (UINT64*)(UINTN)(abar + AHCI_ABAR_PxCLB + (PORT_NUM * 0x80));
	*pointer64 = (UINT64)CommandHeaderEntry;

	// Set Received FIS Base Address
	pointer64 = (UINT64*)(UINTN)(abar + AHCI_ABAR_PxFB + (PORT_NUM * 0x80));
	*pointer64 = (UINT64)ReceivedFISArea;


	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxSERR + (PORT_NUM * 0x80));
	data32 = *pointer32;
	if (data32)
	{
		DEBUG ((EFI_D_ERROR, "Clearing errors in SERR Regsiter(0x%08x)\n",data32));
		*pointer32 = data32;
		data32 = *pointer32;
		DEBUG ((EFI_D_ERROR, "SERR Register After Error Clearing = 0x%08x\n",data32));
	}


	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxIS + (PORT_NUM * 0x80));
	data32 = *pointer32;
	if (data32)
	{
		DEBUG ((EFI_D_ERROR, "Clearing errors in PxIS Regsiter(0x%08x)\n",data32));
		*pointer32 = data32;
		data32 = *pointer32;
		DEBUG ((EFI_D_ERROR, "SERR Register After Error Clearing = 0x%08x\n",data32));
	}


#ifndef C2F_POLL
	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxIE + (PORT_NUM * 0x80));
	*pointer32 |= (1 << 5);
#endif;

#ifdef EXTRA_DEBUG
	DEBUG ((EFI_D_ERROR, "Setting CMD.FRE to 1\n"));
#endif
	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxCMD + (PORT_NUM * 0x80));
	*pointer32 |= (1 << 4);

#ifdef EXTRA_DEBUG
	DEBUG ((EFI_D_ERROR, "Waiting for CMD.FR to set\n"));
#endif
	while (!(*pointer32 & (1 << 14)));


	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxTFD + (PORT_NUM * 0x80));
	data32 = *pointer32;
	if (data32 & ((1 << 7) | (1 << 3)))
	{
		DEBUG ((EFI_D_ERROR, "TFD.STS.BSY or TFD.STS.DRQ is set to 1, setting CLO(PxTFD=0x%x)\n", data32));
		pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxCMD + (PORT_NUM * 0x80));
		*pointer32 |= (1<<3);
#ifdef EXTRA_DEBUG
		DEBUG ((EFI_D_ERROR, "Waiting for CMD.CLO to clear\n"));
#endif
		while ((*pointer32 & (1 << 3)));
	}

#ifdef EXTRA_DEBUG
	DEBUG ((EFI_D_ERROR, "Setting CMD.ST to 1\n"));
#endif
	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxCMD + (PORT_NUM * 0x80));
	*pointer32 |= 1;

#ifdef EXTRA_DEBUG
	DEBUG ((EFI_D_ERROR, "Setting CI to 0x%08x\n",commandMask));
#endif
	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxCI + (PORT_NUM * 0x80));
	*pointer32 = commandMask;

	*monitorloc = 0;

	#if 0
	_asm {
		sti
	}
	#endif
//	AsmSti();

	lastcommand = 0;
	for (i = 0; i < 32; i++)
	{
		if (commandMask & (1 << i))
			lastcommand=i;
	}
#ifdef EXTRA_DEBUG
	DEBUG ((EFI_D_ERROR, "Last Command=%d\n",lastcommand));
#endif

	do {
		if(a == 1) {
			DEBUG ((EFI_D_ERROR, "Looping...\n"));
			a = 0;
		}
	} while ( a == 0);

#ifndef C2F_POLL
	if (lastcommand > 0)
	{

		_mm_monitor(MONITOR_LOC,0,0);

		pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxIS + (PORT_NUM * 0x80));

		CpuFamily = (UINT16)GetCpuFamilyName();
		
		while (*monitorloc == 0)
		{
          if((CPU_FAMILY_BDX_DE == CpuFamily) ||(CPU_FAMILY_BDX == CpuFamily)){
            DEBUG ((EFI_D_ERROR, "About to go into c-state(hint=0x%x), monitorloc=0x%08x\n",MWAIT_HINT_BDX,(UINT32)*monitorloc));
            _mm_mwait(0x1,MWAIT_HINT_BDX);
          } else {
            DEBUG ((EFI_D_ERROR, "About to go into c-state(hint=0x%x), monitorloc=0x%08x\n",MWAIT_HINT,(UINT32)*monitorloc));
            _mm_mwait(0x1,MWAIT_HINT);
          }
          DEBUG ((EFI_D_ERROR, "Woke up from c-state, monitorloc=0x%08x\n",(UINT32)*monitorloc));
		}
	}
#endif
#ifdef EXTRA_DEBUG
	DEBUG ((EFI_D_ERROR, "Waiting for PxCI to clear\n"));
#endif

	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxCI + (PORT_NUM * 0x80));
	while (*pointer32 != 0)
	{
		//_asm { pause};
		AsmPause();
#ifdef EXTRA_DEBUG
		spinloop(1000000);
		DEBUG ((EFI_D_ERROR, "PxCI=0x%08x ",*pointer32));
		pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxTFD + (PORT_NUM * 0x80));
		DEBUG ((EFI_D_ERROR, "PxTFD=0x%x\n",*pointer32));
		pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxCI + (PORT_NUM * 0x80));
#endif
	}

	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxTFD + (PORT_NUM * 0x80));
#ifdef EXTRA_DEBUG
	DEBUG ((EFI_D_ERROR, "TFD=0x%x\n",*pointer32));
#endif

	// Clear the Start and FIS recieve bits
	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxCMD + (PORT_NUM * 0x80));
	*pointer32 &= ~((1 << 4) | (1 << 0));

	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxIS + (PORT_NUM * 0x80));
	data32 = *pointer32;
#ifdef EXTRA_DEBUG
	DEBUG ((EFI_D_ERROR, "PxIS=0x%x\n",data32));    
#endif
	*pointer32 = data32;

	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxSERR + (PORT_NUM * 0x80));
	data32 = *pointer32;
#ifdef EXTRA_DEBUG
	DEBUG ((EFI_D_ERROR, "SERR Register=0x%08x\n",data32));
#endif
	*pointer32 = data32;

	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_IS_MASTER);
	data32 = *pointer32;
#ifdef EXTRA_DEBUG
	DEBUG ((EFI_D_ERROR, "Master IS=0x%x\n",data32));
#endif
	*pointer32 = data32;

	pointer32 = (UINT32*)(UINTN)(abar + AHCI_ABAR_PxCMD + (PORT_NUM * 0x80));
	data32 = *pointer32;
	data32 &= ~((1<<0) | (1<<4));
	*pointer32 = data32;

	return 0;
}




void transferData( UINT32 abar, UINT64 hostaddr, UINT64 bytes, UINT32 lba, UINT32 sectorsize, UINT8 write)
{
	UINT32 commandmask;
	UINT32 commandnum;
	UINT64 bytesleft;
	UINT32 bytestransfered;
	UINT32 bytesqueued;

	struct CommandHeader * CommandHeaderList  = COMMAND_HEADER_LIST;
	struct CommandTable * CommandList = COMMAND_LIST;   

	int i;

//	if(write)
	DEBUG ((EFI_D_ERROR, "Writing "));
//	else
	DEBUG ((EFI_D_ERROR, "Reading "));
	print64(bytes);
	DEBUG ((EFI_D_ERROR, " bytes from "));
	print64(hostaddr);
	DEBUG ((EFI_D_ERROR, " to lba 0x%08x\n",lba));

	bytesleft = bytes;
	while (bytesleft)
	{
		DEBUG ((EFI_D_ERROR, "Starting New Command Round hostaddr="));
		print64n((UINT64)hostaddr);
		memclear(COMMAND_HEADER_LIST,sizeof(struct CommandHeader) * COMMAND_SLOTS);
		commandmask = 0;
		bytesqueued = 0;
		for (commandnum = 0; commandnum < COMMAND_SLOTS; commandnum++)
		{
			if (bytesleft > MAX_COMMAND_BYTECOUNT)
			{
				buildDMACommand( &CommandHeaderList[commandnum], &CommandList[commandnum], hostaddr, lba, MAX_SECTOR_COUNT, sectorsize,(commandnum == (COMMAND_SLOTS - 1)), write);
				commandmask |= (1 << commandnum);
				hostaddr += MAX_COMMAND_BYTECOUNT;
				bytesleft -= MAX_COMMAND_BYTECOUNT;
				bytesqueued += MAX_COMMAND_BYTECOUNT;
				lba += MAX_SECTOR_COUNT;
			} else
			{
				buildDMACommand( &CommandHeaderList[commandnum], &CommandList[commandnum], hostaddr, lba, ((UINT32)(bytesleft)/sectorsize), sectorsize, 1, write);
				commandmask |= (1 << commandnum);
				hostaddr += MAX_COMMAND_BYTECOUNT;
				lba += (((UINT32)bytesleft)/sectorsize);
				bytesqueued += (UINT32)bytesleft;
				bytesleft = 0;
				break;
			}
		}
		if (runCommands( abar, &CommandHeaderList[0], RECEIVED_FIS_AREA, commandmask))
		{
			DEBUG ((EFI_D_ERROR, "Error Running Command\n"));
			return;
		}

		bytestransfered = 0;

		for (i = 0; i < COMMAND_SLOTS; i++)
			bytestransfered += CommandHeaderList[i].PRDBC;

		if (bytestransfered != bytesqueued)
		{
			DEBUG ((EFI_D_ERROR, "ERROR, not all bytes transfered\n"));
			for (i=0; i < COMMAND_SLOTS; i++)
				DEBUG ((EFI_D_ERROR, "Command[%d] Bytes Transfered=0x%08x\n", i, CommandHeaderList[i].PRDBC));
			while (1);

		} else
		{
			DEBUG ((EFI_D_ERROR, "0x%x bytes successfully transfered\n",bytestransfered));
		}
	}
#ifdef EXTRA_DEBUG
	DEBUG ((EFI_D_ERROR, "DMA Command(s) Complete\n"));
#endif
}


void coresleep()

{
	volatile UINT32 * icr_low = (UINT32 *)(UINTN)(APIC_BASE + APIC_ICR_LOW);
	volatile UINT32 * icr_high = (UINT32 *)(UINTN)(APIC_BASE + APIC_ICR_HIGH);
	UINT32 * source;
	UINT32 * dest;
	UINT32 count;


	source = (UINT32*)(UINTN)&sipiroutine;
	dest = (UINT32*)(UINTN)SIPI_AREA;
	for (count = 0; count < 0x1000; count++)
	{
		*dest = *source;
		dest++;
		source++;
	}

	// Send an Init IPI incase the cores aren't already in the Wait for SIPI state.
	*icr_high = 0;
	*icr_low = (3 << 18) | (1 << 14) | (1 << 11) | (5 << 8);

	// Send the start up IPI with 
	*icr_high = 0;
	*icr_low = (3 << 18) | (1 << 14) | (1 << 11) | (6 << 8) | (SIPI_AREA >> 12);
}



UINT32 c2f_entry( UINT32 lowstart, UINT32 lowsize, UINT64 highstart, UINT64 highsize, UINT8 task)
{
	volatile UINT32 * pointer32=0;
	UINT32 abar;
	UINT32 sectorcount, sectorsize, capacity;
	UINT16 * dest = (UINT16*)SCRATCH_PAD_AREA;
	UINT32 testmbytes;
	EFI_STATUS Status;

	//coresleep();

	if (task == WRITE)
	{
		IoWrite8(CMOS_INDEX, C2F_CMOS_LOC);
		IoWrite8(CMOS_DATA, 1);	// Indicate that a transfer began
	}

	DEBUG ((EFI_D_ERROR, "About to Hook the interrupt\n"));
	Status = HookInterrupt();
	if(Status != EFI_SUCCESS)
	{
		if(task == WRITE)
			shutdown_platform();
		else
			return(UINT32)-1;
	}

	DEBUG ((EFI_D_ERROR, "About to Setup MSI\n"));
	if(!EnableMSI())
	{
		DEBUG ((EFI_D_ERROR, "Error setting up MSI\n"));
		if (task == WRITE)
			shutdown_platform();
		else
			return(UINT32)-1;

	}

	DEBUG ((EFI_D_ERROR, "About to initialize AHCI\n"));
	abar = configureAHCI();
	if (abar == 0)
	{
		if (task == WRITE)
			shutdown_platform();
		else
			return(UINT32)-1;
	}


	memclear(COMMAND_HEADER_LIST, sizeof(struct CommandHeader)*COMMAND_SLOTS);
	memclear(RECEIVED_FIS_AREA, sizeof(struct ReceivedFIS));

	buildIdentifyCommand( COMMAND_HEADER_LIST, COMMAND_LIST, SCRATCH_PAD_AREA);
	DEBUG ((EFI_D_ERROR, "About to issue the Identify command\n"));
	if (runCommands( abar, COMMAND_HEADER_LIST, RECEIVED_FIS_AREA, 1))
	{
		DEBUG ((EFI_D_ERROR, "Error Running Command\n"));
		if (task == WRITE)
			shutdown_platform();
		else
			return(UINT32)-1;
	}

	pointer32 = (UINT32*)&dest[100];
	sectorcount= *pointer32;
	if (dest[106] & (1 << 12))
	{
		pointer32 = (UINT32*)&dest[117];
		sectorsize = 2 * (*pointer32);
		DEBUG ((EFI_D_ERROR, "[117]Logical Sector Size=%d\n",sectorsize));
	} else
	{
		DEBUG ((EFI_D_ERROR, "Word 106.12 not set, assuming 512 Byte sector\n"));
		sectorsize = 512;
	}


	capacity = sectorcount / (1024 / sectorsize) / 1024; // FIXE ME: Broken for >1KB sectors
	DEBUG ((EFI_D_ERROR, "Drive Capacity = %dMB\n",capacity));

	testmbytes = (lowsize >> 20) + ((get_upper32(highsize) << 12) + (get_lower32(highsize) >> 20));

	DEBUG ((EFI_D_ERROR, "Total transfer size = 0x%08x\n",(UINT32)testmbytes));

	if (capacity < testmbytes)
	{
		DEBUG ((EFI_D_ERROR, "FATAL ERROR: Storage Device smaller than test size(%dMB)\n",testmbytes));
		if (task == WRITE)
			shutdown_platform();
		else
			return(UINT32)-1;
	}


	buildSetDMACommand( COMMAND_HEADER_LIST, COMMAND_LIST, 6);
	if (runCommands( abar, COMMAND_HEADER_LIST, RECEIVED_FIS_AREA, 1))
	{
		DEBUG ((EFI_D_ERROR, "Error Running Command\n"));
		if (task == WRITE)
			shutdown_platform();
		else
			return(UINT32)-1;  
	}


	if (lowsize > 0)
	{
		DEBUG ((EFI_D_ERROR, "Starting lower memory copy\n"));      
		transferData( abar, lowstart, lowsize, 0, sectorsize, task);
		DEBUG ((EFI_D_ERROR, "Finished lower memory copy\n"));      
	} else
	{
		DEBUG ((EFI_D_ERROR, "No Low Zone\n"));
	}

	if (highsize > 0)
	{
		DEBUG ((EFI_D_ERROR, "Starting high memory copy\n"));       
		transferData( abar, highstart, highsize, (lowsize/sectorsize), sectorsize, task);
		DEBUG ((EFI_D_ERROR, "Finished high memory copy\n"));       
	} else
	{
		DEBUG ((EFI_D_ERROR, "No High Zone\n"));
	}

	IoWrite8(CMOS_INDEX, C2F_CMOS_LOC);
	if (task == WRITE)
	{
		IoWrite8(CMOS_DATA, 2);	// Indicate that a transfer finished
	} else
	{
		IoWrite8(CMOS_DATA, 0);	// Clear the flag
	}

	if (task == WRITE)
	{
		DEBUG ((EFI_D_ERROR, "About to call shutdown\n"));
		shutdown_platform();
	}
//	else

	DisableMSI();
	AhciCleanup(abar);
	// Restore Timer 
	mTimer->SetTimerPeriod(mTimer, mOldTimer);


	return EFI_SUCCESS;
}

UINT8 c2f_recovery_flag(void)
{
	IoWrite8(CMOS_INDEX, C2F_CMOS_LOC);
	if (IoRead8(CMOS_DATA) == 2)
		return 1;
	else
		return 0;
}

UINT8 c2f_entry_flag(void)
{
#ifdef GRANGEVILLE_FLAG
// Check AC_GOOD_LATCHED GPIO37
	if ((IoRead32(0x538) & (1 << 5)))
#else
// Check AC_GOOD_LATCHED GPIO12
	if ((IoRead32(0x50C) & (1 << 12)))
#endif
		return 0;
	else
		return 1;
}

EFI_STATUS
EFIAPI
C2FInit (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
	EFI_STATUS				      	Status = EFI_SUCCESS;
	UINTN	            			VariableSize;
	//	VOID                            *VariableData = NULL;
	SYSTEM_CONFIGURATION            SysConfig;
	EFI_PEI_HOB_POINTERS        	Hob;
	EFI_HOB_RESOURCE_DESCRIPTOR 	*ResourceHob;

	UINT32 c2f_low_start = 0;
	UINT32 c2f_low_length = 0;
	UINT64 c2f_high_start = 0;
	UINT64 c2f_high_length = 0;
	UINT8 highfound = 0, lowfound = 0;
	mCpu = NULL;
	SysConfig.ADREn = 0;
	SysConfig.C2FMode = 0;

	VariableSize = sizeof(SYSTEM_CONFIGURATION);
	
	Status = gRT->GetVariable(
                            L"IntelSetup",  // AptioV server override - use Intel setup variable
                            &gEfiSetupVariableGuid,
                            NULL,
                            &VariableSize,
                            &SysConfig
                            );

	///
	/// Find the CPU architectural protocol.  ASSERT if not found.
	///
	Status = gBS->LocateProtocol (&gEfiCpuArchProtocolGuid, NULL, (VOID **) &mCpu);
	ASSERT_EFI_ERROR (Status);
	if(Status != EFI_SUCCESS)
	{
		DEBUG((EFI_D_ERROR, "Could not locate CPU Architectural Protocol\n"));
		return Status;
	}

	if (SysConfig.ADREn && SysConfig.ADRDataSaveMode == ADR_C2F)
    {
		DEBUG ((EFI_D_ERROR, "C2F Mode Enabled\n"));
		
		for (Hob.Raw = GetHobList (); !END_OF_HOB_LIST (Hob); Hob.Raw = GET_NEXT_HOB (Hob)) {
			if (Hob.Raw != NULL && GET_HOB_TYPE (Hob) == EFI_HOB_TYPE_RESOURCE_DESCRIPTOR) {
				DEBUG ((EFI_D_ERROR, "Found Resource HOB, Start=0x%lX, Length=0x%lX, Type=0x%08x, Attributes=0x%08x\n", Hob.ResourceDescriptor->PhysicalStart, Hob.ResourceDescriptor->ResourceLength, Hob.ResourceDescriptor->ResourceType, Hob.ResourceDescriptor->ResourceAttribute));
				ResourceHob = Hob.ResourceDescriptor;
                if (ResourceHob->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY && ResourceHob->ResourceAttribute & EFI_RESOURCE_ATTRIBUTE_PERSISTENT) {
					if (ResourceHob->PhysicalStart < 0x100000000) {
						if(!lowfound) {					
							c2f_low_start = (UINT32)ResourceHob->PhysicalStart;
							lowfound = 1;
						}
						c2f_low_length += (UINT32)ResourceHob->ResourceLength;
						DEBUG ((EFI_D_ERROR, "Found C2F Low region start=0x%08x, length=0x%08x\n", c2f_low_start, c2f_low_length));
					} else {
						if(!highfound) {
							c2f_high_start = ResourceHob->PhysicalStart;
							highfound = 1;
						}
						c2f_high_length += ResourceHob->ResourceLength;
						DEBUG ((EFI_D_ERROR, "Found C2F High region start=0x%lx, length=0x%lx\n", c2f_high_start, c2f_high_length));
					}
				}
			}
		}


		if (c2f_recovery_flag())
		{
			DEBUG ((EFI_D_ERROR, "C2F Recovery\n"));
			c2f_entry(c2f_low_start, c2f_low_length, c2f_high_start,c2f_high_length,READ);
		}
		else if (c2f_entry_flag())
		{
			DEBUG ((EFI_D_ERROR, "C2F Entry\n"));
			c2f_entry(c2f_low_start, c2f_low_length, c2f_high_start,c2f_high_length,WRITE);
		}
	}
	else
	{
	    DEBUG ((EFI_D_ERROR, "C2F Mode is Disabled, Status = %r\n", Status));
	}
	return Status;
}
