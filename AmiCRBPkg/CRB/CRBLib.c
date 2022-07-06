//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2010, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

//*************************************************************************
// $Header: $
//
// $Revision: $
//
// $Date: $
//*************************************************************************
// Revision History
// ----------------
// $Log: $
// 
//*************************************************************************
//<AMI_FHDR_START>
//
// Name:        CRBLib.c
//
// Description: This file contains Chipset Reference Board related code 
//              that is needed for both PEI & DXE stage. 
//              To avoid code duplication this file is made as a library 
//              and linked both in PEI & DXE CRB FFS.
//
// Notes:       MAKE SURE NO PEI OR DXE SPECIFIC CODE IS NEEDED
//
//<AMI_FHDR_END>
//*************************************************************************

//----------------------------------------------------------------------------
// Include(s)
//----------------------------------------------------------------------------

#include <Efi.h>
#include <Token.h>
#include <PciBus.h>
#include <AmiPeiLib.h>
#include <AmiDxeLib.h>
#include <Iio\IioRegs.h>
#include <AmiCspLib.h>

// Produced Protocols

// Consumed Protocols

//----------------------------------------------------------------------------
// Constant, Macro and Type Definition(s)
//----------------------------------------------------------------------------
// Constant Definition(s)

// Macro Definition(s)

// Type Definition(s)

// Function Prototype(s)

//----------------------------------------------------------------------------
// Variable and External Declaration(s)
//----------------------------------------------------------------------------
// Variable Declaration(s)

// GUID Definition(s)

// Protocol Definition(s)

// External Declaration(s)

// Function Definition(s)

//----------------------------------------------------------------------------

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   NbRuntimeShadowRamWrite
//
// Description: This function provides runtime interface to enable/disable
//              writing in E000-F000 segment
//
// Input:       IN BOOLEAN Enable - if TRUE - enable writing, if FALSE - disable
//
// Output:      None
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
#define SYS_ADD_DEC_DEV                     15
#define SYS_ADD_DEC_FUN                    4

#define R_CQ1_CR_PAM0                             0x40
#define R_CQ1_CR_PAM1                             0x41
#define R_CQ1_CR_PAM2                             0x42
#define R_CQ1_CR_PAM3                             0x43
#define R_CQ1_CR_PAM4                             0x44
#define R_CQ1_CR_PAM5                             0x45
#define R_CQ1_CR_PAM6                             0x46

VOID NbRuntimeShadowRamWrite(
    IN BOOLEAN Enable
)
{
	UINT8                            busUncore[MAX_SOCKET];	
	UINT8                            busCore = 0;		
	UINT8                            socket;
	UINT32							 Data32;
	//NB shadow programming.
	static UINT8	CPUX_F000Data[MAX_SOCKET] =  {0xFF, 0xFF, 0xFF, 0xFF};
	static UINT8	CPUX_E000Data[MAX_SOCKET] =  {0xFF, 0xFF, 0xFF, 0xFF};
	static UINT8	CPUX_E800Data[MAX_SOCKET] =  {0xFF, 0xFF, 0xFF, 0xFF};

 	for(socket = 0;socket < MAX_SOCKET;socket++){
 		Data32 = *(UINT32 *)PCIE_CFG_ADDR (busCore, IIO_VTD_DEV, IIO_VTD_FUN, R_IIO_CPUBUSNO);
		if (Data32 & BIT16){ // check if the IIO is valid
			busUncore[socket] = (UINT8)(Data32 >> 8); //get bus1
			if (busUncore[socket]!=0xFF)
				busCore = busUncore[socket]+1;						
		
			if (Enable) { 
				CPUX_F000Data[socket] = *(UINT8*)PCIE_CFG_ADDR (busUncore[socket] , SYS_ADD_DEC_DEV, SYS_ADD_DEC_FUN, R_CQ1_CR_PAM0);
				CPUX_E000Data[socket] = *(UINT8*)PCIE_CFG_ADDR (busUncore[socket] , SYS_ADD_DEC_DEV, SYS_ADD_DEC_FUN, R_CQ1_CR_PAM5);
				CPUX_E800Data[socket] = *(UINT8*)PCIE_CFG_ADDR (busUncore[socket] , SYS_ADD_DEC_DEV, SYS_ADD_DEC_FUN, R_CQ1_CR_PAM6);
				*(UINT8 *)(UINT32)PCIE_CFG_ADDR (busUncore[socket], SYS_ADD_DEC_DEV, SYS_ADD_DEC_FUN, R_CQ1_CR_PAM0) = 0x30;
				*(UINT8 *)(UINT32)PCIE_CFG_ADDR (busUncore[socket], SYS_ADD_DEC_DEV, SYS_ADD_DEC_FUN, R_CQ1_CR_PAM5) = 0x33;
				*(UINT8 *)(UINT32)PCIE_CFG_ADDR (busUncore[socket], SYS_ADD_DEC_DEV, SYS_ADD_DEC_FUN, R_CQ1_CR_PAM6) = 0x33;
		       } else{
				if (CPUX_F000Data[socket] != 0xFF) {
					*(UINT8 *)(UINT32)PCIE_CFG_ADDR (busUncore[socket], SYS_ADD_DEC_DEV, SYS_ADD_DEC_FUN, R_CQ1_CR_PAM0) = CPUX_F000Data[socket];
					CPUX_F000Data[socket] = 0xFF;
				}
				if (CPUX_E000Data[socket] != 0xFF) {
					*(UINT8 *)(UINT32)PCIE_CFG_ADDR (busUncore[socket], SYS_ADD_DEC_DEV, SYS_ADD_DEC_FUN, R_CQ1_CR_PAM5) = CPUX_E000Data[socket];
					CPUX_E000Data[socket] = 0xFF;
				}
				if (CPUX_E800Data[socket] != 0xFF) {
					*(UINT8 *)(UINT32)PCIE_CFG_ADDR (busUncore[socket], SYS_ADD_DEC_DEV, SYS_ADD_DEC_FUN, R_CQ1_CR_PAM6) = CPUX_E800Data[socket];
					CPUX_E800Data[socket] = 0xFF;
				}
			}
		} //iif (Data16 & BIT16)
	}   //for(socket=0;socket<MAX_SOCKET;socket++)
}


VOID OemRuntimeShadowRamWrite(
    IN BOOLEAN Enable
)
{
    NbRuntimeShadowRamWrite(Enable);
}

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2010, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
