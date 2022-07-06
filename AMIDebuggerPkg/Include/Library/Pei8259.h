//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2012, American Megatrends, Inc.        **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
// $Header: /AptioV/SRC/AMIDebugRxPkg/Include/Library/Pei8259.h 1     11/02/12 10:07a Sudhirv $
//
// $Revision: 1 $
//
// $Date: 11/02/12 10:07a $
//*****************************************************************
//*****************************************************************
//
//	Pei8259.h
//
//
// Revision History
// ----------------
// $Log: /AptioV/SRC/AMIDebugRxPkg/Include/Library/Pei8259.h $
// 
// 1     11/02/12 10:07a Sudhirv
// AMIDebugRxPkg modulepart for AMI Debug for UEFI 3.0
// 
// 2     8/03/12 6:32p Sudhirv
// Updated before making binaries.
//
// 1     7/15/12 8:12p Sudhirv
// AMIDebugRx Module for Aptio 5
//
//*****************************************************************

//
// 8259 Hardware definitions
//
#define LEGACY_MODE_BASE_VECTOR_MASTER                    0x08
#define LEGACY_MODE_BASE_VECTOR_SLAVE                     0x10

#define PROTECTED_MODE_BASE_VECTOR_MASTER                 0x68
#define PROTECTED_MODE_BASE_VECTOR_SLAVE                  0x70

#define LEGACY_8259_CONTROL_REGISTER_MASTER               0x20
#define LEGACY_8259_MASK_REGISTER_MASTER                  0x21
#define LEGACY_8259_CONTROL_REGISTER_SLAVE                0xA0
#define LEGACY_8259_MASK_REGISTER_SLAVE                   0xA1
#define LEGACY_8259_EDGE_LEVEL_TRIGGERED_REGISTER_MASTER  0x4D0
#define LEGACY_8259_EDGE_LEVEL_TRIGGERED_REGISTER_SLAVE   0x4D1

#define LEGACY_8259_EOI									  0x20

#define Efi8259LegacyMode									0
#define Efi8259ProtectedMode								1
#define Efi8259MaxMode										2

#define Efi8259Irq0											0
#define Efi8259Irq1											1
#define Efi8259Irq2											2
#define Efi8259Irq3											3
#define Efi8259Irq4											4
#define Efi8259Irq5											5
#define Efi8259Irq6											6
#define Efi8259Irq7											7
#define Efi8259Irq8											8
#define Efi8259Irq9											9
#define Efi8259Irq10										10
#define Efi8259Irq11										11
#define Efi8259Irq12										12
#define Efi8259Irq13										13
#define Efi8259Irq14										14
#define Efi8259Irq15										15
#define Efi8259IrqMax										16

#define NULL												0

// Init CPU

#define	APIC_SPURIOUS_VECTOR_REGISTER					0xf0
#define	APIC_LVT_LINT0_REGISTER							0x350
#define	APIC_LVT_LINT1_REGISTER							0x360

typedef struct {
	UINT8	mMasterBase;
	UINT8	mSlaveBase;
	UINT16	mMode;
	UINT16	mProtectedModeMask;
	UINT16  mLegacyModeMask;
	UINT16  mProtectedModeEdgeLevel;
	UINT16  mLegacyModeEdgeLevel;
} PEI8259GblData_T;

void PEIIoWrite8 (UINT16, UINT8);
UINT8 PEIIoRead8 (UINT16);
void PEI8259WriteMask (UINT16, UINT16);
void PEI8259ReadMask (UINT16*, UINT16*);
void PEI8259SetVectorBase (UINT16, UINT16);
void PEI8259SetMode (UINT16,UINT16*, UINT16*);
void PEI8259GetVector (UINT16, UINT16*);
void PEI8259EnableIRQ (UINT16, UINT16);
void PEI8259DisableIRQ (UINT16);
void PEI8259EndofInterrupt (UINT16);
void PEI8259InitGblData ();
VOID InitializeCpu();
VOID EnableSerIRQInICH ();
VOID WritePCIRegWord(UINT8,UINT8,UINT8,UINT16);

//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2012, American Megatrends, Inc.        **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//