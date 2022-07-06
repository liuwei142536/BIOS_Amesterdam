//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
/** @file AmiSioPeiLib.c

    Library Class for AMI SIO Driver.
**/

//-------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------
#include <Efi.h>
#include <Pei.h>
#include <Token.h>
#include <AmiPeiLib.h>
#include <Protocol/AmiSio.h>
#include <Library/AmiSioPeiLib.h>
#include <Library/AmiIoDecodeLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>

/**
    Update the IO range decode table if request range is Overlap or out.

    @param[in] TableCount           The number of DecodeTable.
    @param[in] Base                 The base address of IO range.
    @param[in] Length               The length of IO range.
    @param[in,out] DecodeTable      The array to shadow current IO range
  
    @retval EFI_SUCCESS             Memory region successfully combined.
    @retval EFI_OUT_OF_RESOURCES    Memory region cannot be combined.
**/
EFI_STATUS
UpdateIoRangeOverlap (
    IN      UINT32                 TableCount,
    IN      UINT16                 BaseAddr,
    IN      UINT16                 Length,
    IN OUT  GENERIC_IO_DECODE      *DecodeTable
  )
{
    UINT8   Index,Count;
    UINT8   RangeCount = 1;
    IO_RANGE  InputRange[2]={0};

    //Preprocess input data.
    InputRange[0].Base      = BaseAddr;
    InputRange[0].End       = BaseAddr + Length - 1;
    InputRange[0].Length    = Length;

    //If input range not in same region, will split two ranges to decode  
    if( (InputRange[0].Base & 0xFF00) != (InputRange[0].End & 0xFF00)){
        //Record the rest of the range.
        InputRange[1].Base      = InputRange[0].End & 0xFF00;
        InputRange[1].Length    = InputRange[0].End - InputRange[1].Base + 1;
        InputRange[1].End       = InputRange[0].End;
        //Update first range
        InputRange[0].End       = InputRange[0].Base | 0x00FF;
        InputRange[0].Length    = InputRange[0].End - InputRange[0].Base + 1;

        RangeCount = 2;
    }
    
    for(Count = 0; Count < RangeCount; Count++){
        for (Index = 0; Index < TableCount; Index++) {
            if(DecodeTable[Index].Enable && (InputRange[Count].Base >= DecodeTable[Index].BaseLimit) && (InputRange[Count].End <= DecodeTable[Index].EndLimit)) {
                // If the request range out of the current range, continue to check next range.
                if ( (InputRange[Count].Base > (DecodeTable[Index].EndAddress + 1)) || ((InputRange[Count].End + 1) < DecodeTable[Index].BaseAddress) ) {
                    continue;
                }
                    
                // If the current range contain the request range, no need to decode and return.
                if ( (DecodeTable[Index].BaseAddress <= InputRange[Count].Base) && (DecodeTable[Index].EndAddress >= InputRange[Count].End) ) {
                    break;
                }
                    
                // Program the combine range if Io Range Overlap, Record the usage in DecodeTable array.
                DecodeTable[Index].OrgBaseAddr = DecodeTable[Index].BaseAddress;
                DecodeTable[Index].BaseAddress  = InputRange[Count].Base < DecodeTable[Index].BaseAddress ? InputRange[Count].Base : DecodeTable[Index].BaseAddress;
                DecodeTable[Index].EndAddress   = InputRange[Count].End > DecodeTable[Index].EndAddress ? InputRange[Count].End : DecodeTable[Index].EndAddress;
                DecodeTable[Index].BaseLimit    = DecodeTable[Index].BaseAddress & 0xFF00;
                DecodeTable[Index].EndLimit     = DecodeTable[Index].BaseAddress | 0x00FF;;
                DecodeTable[Index].Length       = DecodeTable[Index].EndAddress - DecodeTable[Index].BaseAddress + 1;
                DecodeTable[Index].Overwrite    = TRUE;
                DecodeTable[Index].Enable       = TRUE;
                break;
            }
        }
        
        //If not match any range which is Enable, will be filled in idle range.
        if(Index == TableCount){
            for (Index = 0; Index < TableCount; Index++) {
                if(!DecodeTable[Index].Enable) {
                    // Record the new range in DecodeTable array.
                    DecodeTable[Index].BaseAddress  = InputRange[Count].Base;
                    DecodeTable[Index].EndAddress   = InputRange[Count].End;
                    DecodeTable[Index].BaseLimit    = DecodeTable[Index].BaseAddress & 0xFF00;
                    DecodeTable[Index].EndLimit     = DecodeTable[Index].BaseAddress | 0x00FF;
                    DecodeTable[Index].Length       = InputRange[Count].Length;
                    DecodeTable[Index].Overwrite    = TRUE;
                    DecodeTable[Index].Enable       = TRUE;
                    break;
                }
            }
            if(Index == TableCount) return EFI_OUT_OF_RESOURCES;
        }
    } 
    return EFI_SUCCESS;
}

/**
    Check the decode table is valid or not.

    @param[in] Table    Decode table.
  
    @retval TRUE        Decode table is valid.
    @retval FALSE       Decode table is invalid.
**/
BOOLEAN EFIAPI IsIoDecodeRangeSupport(
    IN IO_DECODE_DATA       Table )
{
    if(Table.Type == 0xFF && Table.BaseAdd != 0x2E && Table.BaseAdd != 0x4E && Table.BaseAdd != 0x62)
        return TRUE;
    else 
        return FALSE;
}

/**
    Check the IO range decode table if request range need to be program to ISA bridge.

    @param[in] Table                Decode table.
  
    @retval EFI_SUCCESS             Decode table successfully combined.
    @retval EFI_INVALID_PARAMETER   Decode table invalid.
    @retval EFI_UNSUPPORTED         Hob build fail.  
    @retval EFI_OUT_OF_RESOURCES    Memory region cannot be combined.
**/
EFI_STATUS EFIAPI CheckIoDecodeRange(
    IN IO_DECODE_DATA       Table )
{
    EFI_STATUS              Status;
    UINT16                  Base;
    UINT16                  Length;
    EFI_GUID                IoRangeTableHobGuid = IO_RANGE_TABLE_HOB_GUID;
    GENERIC_IO_DECODE       DecodeTable[ISA_GEN_DEC_REG_COUNT] = {0};
    UINTN                   BufferSize;
    DECODE_TABLE_HOB_DATA   *GuidHob = NULL;
    UINT16                  IoGenDecIndex;
    GENERIC_IO_DECODE       *HobDecodeTable = NULL;

    if(Table.UID == 0) return EFI_INVALID_PARAMETER;
    if(!IsIoDecodeRangeSupport(Table)) return EFI_INVALID_PARAMETER;
    
    Base = Table.BaseAdd;
    Length = Table.UID;
    if (Length < 4) Length = 4;
    
    BufferSize = sizeof(GENERIC_IO_DECODE) * ISA_GEN_DEC_REG_COUNT;
    // Get Hob data for Io Decode Table.
    GuidHob = GetFirstGuidHob (&IoRangeTableHobGuid);    
    if(GuidHob == NULL) {
        // If not found create one.
        HobDecodeTable = BuildGuidHob (&IoRangeTableHobGuid, BufferSize);
        if(HobDecodeTable == NULL)
            return EFI_UNSUPPORTED;
        ZeroMem(HobDecodeTable,BufferSize);
    }
    else {
        HobDecodeTable = &GuidHob->DecodeTable[0];
        CopyMem(DecodeTable,GuidHob->DecodeTable,BufferSize);
    }

    // Update the I/O Generic Decode Table.
    Status = UpdateIoRangeOverlap(ISA_GEN_DEC_REG_COUNT, Base, Length, DecodeTable);
    if(EFI_ERROR(Status))
        return Status;

    // Write to I/O Generic Decodes Register.
    for(IoGenDecIndex = 0;IoGenDecIndex < ISA_GEN_DEC_REG_COUNT;IoGenDecIndex ++){
        if(DecodeTable[IoGenDecIndex].Overwrite){
            // Clear exist generic decode range if need to overwrite.
            if(DecodeTable[IoGenDecIndex].OrgBaseAddr) {
                AmiSioLibSetLpcGenericDecoding(NULL, DecodeTable[IoGenDecIndex].OrgBaseAddr, DecodeTable[IoGenDecIndex].Length, 0);
            }
            AmiSioLibSetLpcGenericDecoding(NULL, DecodeTable[IoGenDecIndex].BaseAddress, DecodeTable[IoGenDecIndex].Length, 1);
            DecodeTable[IoGenDecIndex].Overwrite = FALSE;
        }
    }
    // Update I/O decode table to HOB
    CopyMem(HobDecodeTable,DecodeTable,BufferSize);
    
    return Status;
}

/**
    This function will program the runtime register.

    @param[in] Base      Runtime register IO base.
    @param[in] Table     Initial table.
    @param[in] Count     Table data count.
**/
VOID EFIAPI ProgramRtRegisterTable(
    IN  UINT16                  Base,
    IN  SIO_DEVICE_INIT_DATA    *Table,
    IN  UINT8                   Count
)
{
    UINT8   i;
    UINT8   Value8;

    for(i=0;i<Count;i++) {
        //Some registers may not allow to read, and also we can write some registers without any read operations. 
        if(Table[i].AndData8 == 0x00)  Value8 = Table[i].OrData8;
        else Value8 = (IoRead8(Base+Table[i].Reg16) & Table[i].AndData8) | Table[i].OrData8;
        IoWrite8(Base + Table[i].Reg16, Value8 );
    }
}


//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
