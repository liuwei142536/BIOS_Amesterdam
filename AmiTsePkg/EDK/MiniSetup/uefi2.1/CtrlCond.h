//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2022, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file CtrlCond.h
    Header File

**/
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#define COND_NONE			0x0
#define COND_SUPPRESS		0x1
#define COND_HIDDEN			0x2
#define COND_GRAYOUT		0x3
#define COND_INCONSISTENT	0x4
#define COND_NOSUBMIT		0x5
#define COND_WARN_IF		0x6
#define COND_DISABLE        0x7
//---------------------------------------------------------------------------


// functions
//UINT8 CheckControlCondition(UINT32 ControlAccess, UINT8  *CtrlPtr, UINT32 CtrlVar, UINT32 CtrlVar2, UINT8 *CondPtr);
UINT8 CheckControlCondition( CONTROL_INFO *controlInfo );
BOOLEAN CheckInconsistence( PAGE_INFO *pPageInfo );
UINT8 CheckOneOfOptionSuppressIf(CONTROL_INFO *ParentCtrlInfo, UINT8* CtrlCondPtr, UINT8* CtrlPtr);
BOOLEAN CheckNoSubmitIf( VOID );
BOOLEAN DrawMessageBox(CONTROL_INFO *pControlInfo, UINT16 Title, EFI_STRING_ID  Error);
UINT8 OEMCheckControlCondition(UINT8 FinalCond, CONTROL_INFO *controlInfo);
VOID HandleWriteOpOnChanged(CONTROL_INFO *ControlInfo);
VOID HandleReadOpOnRetrieve();
VOID HandleReadOpOnChanged(UINT32 CurrentPage);

