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
// 
//**********************************************************************
//<AMI_FHDR_START>
//
// Name:    CSP_MEUD.h
//
// Description:
//
//<AMI_FHDR_END>
//**********************************************************************
#ifndef _EFI_CSP_MEUD_H_
#define _EFI_CSP_MEUD_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <Token.h>

#define SPI_BASE  (SB_RCBA + SPI_BASE_ADDRESS)


#ifndef _EFI_MMIO_ACCESS_H_
#define _EFI_MMIO_ACCESS_H_

#define MmioAddress(BaseAddr, Register) \
 ( (UINTN)BaseAddr + (UINTN)(Register) )

// 32-bit
#define Mmio32Ptr(BaseAddr, Register) \
 ( (volatile UINT32 *)MmioAddress(BaseAddr, Register) )

#define Mmio32(BaseAddr, Register) \
 *Mmio32Ptr(BaseAddr, Register)

// 16-bit
#define Mmio16Ptr(BaseAddr, Register) \
 ( (volatile UINT16 *)MmioAddress(BaseAddr, Register) )

#define Mmio16(BaseAddr, Register) \
 *Mmio16Ptr(BaseAddr, Register)

// 8-bit
#define Mmio8Ptr(BaseAddr, Register) \
 ( (volatile UINT8 *)MmioAddress(BaseAddr, Register) )

#define Mmio8(BaseAddr, Register) \
 *Mmio8Ptr(BaseAddr, Register)

#define MmioRW32(Addr, set, reset) \
    (Mmio32(Addr, 0) = ((Mmio32(Addr, 0) & (UINT32)~(reset)) | (UINT32)set))
#endif

#define HeciPciRead32(Register) PciRead32 (PCI_LIB_ADDRESS (0, 0x16, 0, Register))

#define SPS_CLIENTID_BIOS 0x00
#define SPS_CLIENTID_ME_MKHI 0x07

VOID InitializeSpiBase( VOID );

#pragma pack(1)

typedef union _ME_HECI_MESSAGE_HEADER
{
    UINT32      Data;
    struct
    {
        UINT32  MeAddress       :8;
        UINT32  HostAddress     :8;
        UINT32  Length          :9;
        UINT32  Reserved        :6;
        UINT32  MessageComplete :1;
    }Fields;
}ME_HECI_MESSAGE_HEADER, *ME_HECI_MESSAGE_HEADER_PTR;

typedef union _ME_MKHI_MESSAGE_HEADER
{
    UINT32     Data;
    struct
    {
        UINT32  GroupId     :8;
        UINT32  Command     :7;
        UINT32  IsResponse  :1;
        UINT32  Reserved    :8;
        UINT32  Result      :8;
    }Fields;
}ME_MKHI_MESSAGE_HEADER, *ME_MKHI_MESSAGE_HEADER_PTR;

typedef struct
{
    ME_HECI_MESSAGE_HEADER  Header;
    ME_MKHI_MESSAGE_HEADER  MKHIHeader;
}HMRFPO_GET_STATUS_HECI_MSG;

typedef struct
{
    ME_HECI_MESSAGE_HEADER  Header;
    ME_MKHI_MESSAGE_HEADER  MKHIHeader;
    UINT8           Status;
    UINT8           Reserved0;
    UINT8           Reserved1;
    UINT8           Reserved2;
}HMRFPO_GET_STATUS_RESPONSE_HECI_MSG;

typedef struct
{
    ME_HECI_MESSAGE_HEADER  Header;
    ME_MKHI_MESSAGE_HEADER  MKHIHeader;
    UINT8           Nounce[8];
}HMRFPO_ENABLE_HECI_MSG;

typedef struct
{
    ME_HECI_MESSAGE_HEADER  Header;
    ME_MKHI_MESSAGE_HEADER  MKHIHeader;
    UINT32          Factory_Base;
    UINT32          Factory_Limit;
    UINT8           Status;
    UINT8           Reserved0;
    UINT8           Reserved1;
    UINT8           Reserved2;
}HMRFPO_ENABLE_RESPONSE_HECI_MSG;

typedef struct
{
    ME_HECI_MESSAGE_HEADER  Header;
    ME_MKHI_MESSAGE_HEADER  MKHIHeader;
}HMRFPO_LOCK_HECI_MSG;

typedef struct
{
    ME_HECI_MESSAGE_HEADER  Header;
    ME_MKHI_MESSAGE_HEADER  MKHIHeader;
    UINT8           Nounce[8];
    UINT32          Factory_Base;
    UINT32          Factory_Limit;
    UINT8           Status;
    UINT8           Reserved0;
    UINT8           Reserved1;
    UINT8           Reserved2;
}HMRFPO_LOCK_RESPONSE_HECI_MSG;

typedef struct
{
    ME_HECI_MESSAGE_HEADER Header;
    ME_MKHI_MESSAGE_HEADER MKHIHeader;
    UINT8  Request_Origin;
    UINT8  Request_Type;
} MEI_GLOBAL_RST_MSG;

#pragma pack()


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
