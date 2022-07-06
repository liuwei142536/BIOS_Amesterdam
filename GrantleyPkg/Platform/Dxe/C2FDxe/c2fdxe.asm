
TITLE   c2fdxe.asm: Assembly code 

text    SEGMENT

ABAR_LOC 			equ 	091000000h
AHCI_ABAR_PxIS 		equ		0110h
PORT_NUM 			equ 	2h
AHCI_ABAR_IS_MASTER equ		8h
APIC_BASE 			equ		0FEE00000h
APIC_EOI			equ		0B0h
IOAPIC_BASE 		equ		0FEC00000h
IOAPIC_EOI			equ		040h
VECTOR_NUM			equ		0cfh
MONITOR_LOC			equ		0203000h

AsmSti proc PUBLIC 
		sti
		ret
AsmSti ENDP

AsmPause proc PUBLIC 
		pause
		ret
AsmPause ENDP

;//__declspec( naked ) void isr(void)
isr   PROC  PUBLIC
		push rax
		push rbx

		cld
		mov al,55h
		out 80h, al
		;//mov eax, 0xf8000110	// Clear the PxIS register
		mov rax, (ABAR_LOC + AHCI_ABAR_PxIS + (PORT_NUM * 0x100))
		mov rbx, [rax]
		mov [rax], rbx

		;//mov eax, 0xf8000008	// Clear the Master IS register
		mov rax, (ABAR_LOC + AHCI_ABAR_IS_MASTER)
		mov rbx, [rax]
		mov [rax], rbx

		;//mov eax, 0xfee000b0	// Write the EOI register in the local APIC
		mov rax, (APIC_BASE + APIC_EOI)
		xor rbx,rbx
		mov rbx,1
		mov [rax], rbx

		;//mov eax, 0xfec00040     // Write the EOI regsiter in the IO-APIC
		;mov rax, (IOAPIC_BASE + IOAPIC_EOI)		// Write the EOI regsiter in the IO-APIC
		;mov [rax], VECTOR_NUM

		mov rax, MONITOR_LOC	// Write the monitor location
		xor rbx, rbx
		mov rbx, 1	
		;mov [rax], rbx
		
		mov al,56h
		out 80h, al

		pop rbx
		pop rax
		iret
		
isr ENDP

text ENDS
END

