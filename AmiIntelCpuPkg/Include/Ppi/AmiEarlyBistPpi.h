//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2011, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

//**********************************************************************
// $Header: $
//
// $Revision: $
//
// $Date: $
//**********************************************************************
// Revision History
// ----------------
// $Log: $
// 
// 
//**********************************************************************

//<AMI_FHDR_START>
//---------------------------------------------------------------------------
//
// Name:	AmiEarlyBistPpi.h
//
// Description: AmiEarlyBistPpi.h
//
//---------------------------------------------------------------------------
//<AMI_FHDR_END>

#ifndef __AMI_EARLY_BIST_PPI_H__
#define __AMI_EARLY_BIST_PPI_H__
#ifdef __cplusplus
extern "C" {
#endif

//a7e2ce72-dc32-4bc0-9e35-feb30ae5cc47
#define AMI_EARLY_BIST_PPI_GUID \
    {0xa7e2ce72, 0xdc32, 0x4bc0, 0x9e, 0x35, 0xfe, 0xb3, 0xa, 0xe5, 0xcc, 0x47}

typedef struct {
    UINT32 ApicId;
    UINT32 Bist;
} CPU_BIST;


typedef struct {
	UINT32      NumBists;       //Number of Bists in array.
    CPU_BIST    CpuBist[1];     //Variable length array
} AMI_EARLY_BIST_PPI;

extern EFI_GUID gAmiEarlyBistPpiGuid;

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2011, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
