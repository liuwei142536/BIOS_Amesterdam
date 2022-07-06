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
/** @file AmiSioPeiLib.h

    SIO PEI Init Library definitions.
**/

#ifndef _AMI_SIO_PEI_LIB_H_
#define _AMI_SIO_PEI_LIB_H_

//-------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------
#include <Efi.h>
#include <Protocol/AmiSio.h>
#include <Token.h>

#ifndef ISA_GEN_DEC_REG_COUNT
#define ISA_GEN_DEC_REG_COUNT 4
#endif
//-------------------------------------------------------------------------
// Constants, Macros and Type Definitions
//-------------------------------------------------------------------------
#define IO_RANGE_TABLE_HOB_GUID \
  { \
    0x6db13816, 0x8905, 0x4917, {0xa3, 0xce, 0xeb, 0xce, 0x53, 0x5e, 0x99, 0xc2} \
  }
typedef struct _IO_DECODE_DATA{
    UINT16          BaseAdd;
    //!!!Attention!!!If type is 0xFF, UID is a IO legth
    UINT8           UID;
    SIO_DEV_TYPE    Type;
} IO_DECODE_DATA;

typedef struct _SIO_DEVICE_INIT_DATA{
    UINT16      Reg16;
    UINT8       AndData8;   // 0x00 means register don't need AndMask
                            // only write OrData8 to regisrer.
    UINT8       OrData8;
} SIO_DEVICE_INIT_DATA;

typedef struct _SIO_DATA{
    UINT16           Addr;
    //AND mask value, 0xFF means register don't need AndMask and 
    //only write OrData8 to regisrer.
    UINT8           DataMask; 
    //OR mask value.  
    UINT8           DataValue;
} SIO_DATA;

///
/// This structure represents Generic IO deocode register
///
typedef struct {
  UINT16  OrgBaseAddr;
  UINT16  BaseAddress;  ///< Base address of the IO range.
  UINT16  EndAddress;   ///< End address of the IO range.
  UINT16  BaseLimit;    ///< The limit Base address of the IO range.
  UINT16  EndLimit;     ///< The limit End address of the IO range.
  UINT16  Length;       ///< Range of this IO decode.
  BOOLEAN Enable;       ///< Indicator of whether the range is valid.
  BOOLEAN Overwrite;    ///< Status Indicate this IO decode register need to be Overwrite.
} GENERIC_IO_DECODE;

///
/// This structure represents Input IO deocode range
///
typedef struct {
  UINT16  Base;         ///< Base address of the IO range.
  UINT16  End;          ///< End address of the IO range.
  UINT16  Length;          ///< Range of this IO decode.
} IO_RANGE;

typedef struct {
    EFI_HOB_GUID_TYPE Header;
    GENERIC_IO_DECODE DecodeTable[ISA_GEN_DEC_REG_COUNT];
}DECODE_TABLE_HOB_DATA;

//-------------------------------------------------------------------------
//Variable, Prototype, and External Declarations
//-------------------------------------------------------------------------
EFI_STATUS EFIAPI AmiSioLibSetLpcDeviceDecoding(
    IN    VOID           *LpcPciIo,
    IN    UINT16         Base,
    IN    UINT8          DevUid,
    IN    SIO_DEV_TYPE   Type);

EFI_STATUS EFIAPI AmiSioLibSetLpcGenericDecoding (
    IN VOID				       *LpcPciIo,
    IN UINT16                   Base,
    IN UINT16                   Length,
    IN BOOLEAN                  Enable );

VOID EFIAPI ProgramRtRegisterTable(
    IN  UINT16  Base,
    IN  SIO_DEVICE_INIT_DATA  *Table,
    IN  UINT8   Count
);

BOOLEAN EFIAPI IsIoDecodeRangeSupport(
    IN IO_DECODE_DATA       Table 
);

EFI_STATUS EFIAPI CheckIoDecodeRange(
    IN IO_DECODE_DATA       Table
);
#endif //_AMI_SIO_PEI_LIB_H_
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
