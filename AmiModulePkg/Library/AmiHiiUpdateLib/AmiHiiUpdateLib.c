//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

#include <Uefi.h>
#include <Library/HiiLib.h>


UINT8* AmiHiiCreateSuppresGrayIdVal(
  IN VOID               *OpCodeHandle,
  IN EFI_QUESTION_ID    QuestionId,
  UINT16                Value,
  BOOLEAN               Suppress    //if TRUE Suppress; False Gray out.
  )
{
    EFI_IFR_EQ_ID_VAL   *Condition;
    EFI_IFR_OP_HEADER   *Header;
    struct {
        EFI_IFR_OP_HEADER Header;
        EFI_IFR_EQ_ID_VAL Condition;
    } Buffer;

    //Init Pointers;
    Header = &Buffer.Header;
    Condition = &Buffer.Condition;

    //Update OpCodes...
    //EFI_IFR_SUPPRESS_IF_OP/EFI_IFR_GRAY_OUT_IF_OP first;
    if(Suppress) Header->OpCode=EFI_IFR_SUPPRESS_IF_OP;
    else Header->OpCode=EFI_IFR_GRAY_OUT_IF_OP;
    Header->Length=sizeof(EFI_IFR_OP_HEADER);
    Header->Scope=1;

    //then goes conditions.
    //First update OpCode Header
    Condition->Header.OpCode=EFI_IFR_EQ_ID_VAL_OP;
    Condition->Header.Scope=0;
    Condition->Header.Length=sizeof(EFI_IFR_EQ_ID_VAL);
    //Then goes Opcode Data..
    Condition->QuestionId=QuestionId;
    Condition->Value=Value;

    //Here we go...
    return HiiCreateRawOpCodes(OpCodeHandle, (UINT8*)&Buffer, sizeof(Buffer));
}

//*************************************************************************
UINT8* EFIAPI AmiHiiTerminateScope(IN VOID *OpCodeHandle)
{
    EFI_IFR_END         IfrEnd;

    //Update Header...
    IfrEnd.Header.OpCode = EFI_IFR_END_OP;
    IfrEnd.Header.Scope  = 0;
    IfrEnd.Header.Length = sizeof (EFI_IFR_END);

    //Submit it...
    return HiiCreateRawOpCodes(OpCodeHandle, (UINT8*)&IfrEnd, sizeof(IfrEnd) );
}

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
