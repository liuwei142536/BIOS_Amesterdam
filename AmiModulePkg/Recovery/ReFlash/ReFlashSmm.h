//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2012, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file
  This file contains Reflash driver structures definitions

**/
#ifndef __REFLASH__SMM__H__
#define __REFLASH__SMM__H__
#ifdef __cplusplus
extern "C"
{
#endif

#define AMI_REFLASH_SMM_HANDLER_GUID \
    { 0xe90a7b5e, 0x37b2, 0x43e4,  { 0xb2, 0x81, 0x4c, 0x8e, 0x34, 0x90, 0x45, 0xe9 }}

#define AMI_REFLASH_SMM_UPDATE_NVRAM_IN_BUFFER 0
#define AMI_REFLASH_SMM_UPDATE_NVRAM_IN_PLACE  1
#define AMI_REFLASH_SMM_MIGRATE_NVRAM		   2
#define AMI_REFLASH_SMM_VALIDATE_IMAGE		   3
#define AMI_REFLASH_SMM_UPDATE_IMAGE		   4

typedef struct {
    UINT64 Action;
    UINT64 Status;
    UINT64 Data[4];
} AMI_REFLASH_SMM_COMMUNICATE_HEADER;

typedef struct {
    EFI_GUID Handler;
    UINTN Size;
    AMI_REFLASH_SMM_COMMUNICATE_HEADER Header;
} AMI_REFLASH_SMM_COMM_BUFFER;

typedef struct {
	UINT32 *FrameBuf;
	UINT32 FrameBufSz;
	UINT16 HorRes;
	UINT16 VerRes;
	UINT16 PxScanLine;
	UINT16 PxMask;
} AMI_REFLASH_FRAMEBUFFER_INFO;

extern AMI_REFLASH_SMM_COMM_BUFFER *SmmCommBuffer;

VOID DrawProgress (AMI_REFLASH_FRAMEBUFFER_INFO *Info, UINT16 pc);
VOID DrawLogo (AMI_REFLASH_FRAMEBUFFER_INFO *Info);

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2012, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
