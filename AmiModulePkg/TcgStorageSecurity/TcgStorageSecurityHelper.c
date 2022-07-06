//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file TcgStorageSecurityHelper.c
    TcgStorageSecurity helper functions

**/
//---------------------------------------------------------------------------

#include "TcgStorageSecOPAL.h"
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>

#if defined(TCG_STORAGE_SEC_VERBOSE_PRINT) && (TCG_STORAGE_SEC_VERBOSE_PRINT == 1)
UIDTOSTRING Uidtable [] = {
    { "NULL_UID",                   TCGUID(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)},
    { "THISSP_UID",                 TCGUID(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01)},
    { "SM_UID",                     TCGUID(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF)},
    { "CPIN_UID",                   TCGUID(0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x01)},
    {"PROPERTIES_UID",              TCGUID(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x01)},
    {"START_SESSION_UID",           TCGUID(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x02)},
    {"SYNC_SESSION_UID",            TCGUID(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03)},
    {"START_TRUSTED_SESSION_UID",   TCGUID(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x04)},
    {"SYNC_TRUSTED_SESSION_UID",    TCGUID(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x05)},
    {"CLOSE_SESSION_UID",           TCGUID(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x06)},
    {"SPINFO_UID",                  TCGUID(0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0xFF, 0x01)},
    {"TPERINFO_UID",                TCGUID(0x00, 0x00, 0x02, 0x01, 0x00, 0x03, 0x00, 0x01)},
    {"LOCKINGINFO_UID",             TCGUID(0x00, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x01)},
    {"MBRCONTROL_UID",              TCGUID(0x00, 0x00, 0x08, 0x03, 0x00, 0x00, 0x00, 0x01)},
    {"C_PIN_SID_UID",               TCGUID(0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x01)},
    {"SP_ADMIN_UID",                TCGUID(0x00, 0x00, 0x02, 0x05, 0x00, 0x00, 0x00, 0x01)},
    {"CLOCK_TIME_CLOCK_UID",        TCGUID(0x00, 0x00, 0x04, 0x01, 0x00, 0x00, 0x00, 0x01)},
    {"LOGLIST_LOG_UID",             TCGUID(0x00, 0x00, 0x0A, 0x02, 0x00, 0x00, 0x00, 0x01)},
    {"LOCKING_GLOBAL_RANGE_UID",    TCGUID(0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x00, 0x01)},
    {"GEN_KEY_UID",                 TCGUID(0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x10)},
    {"GET_KEY_UID",                 TCGUID(0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x16)},
    {"GET_KEY_UID_ENTERPRISE",      TCGUID(0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06)},
    {"SET_KEY_UID",                 TCGUID(0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x17)},
    {"SET_KEY_UID_ENTERPRISE",      TCGUID(0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x07)},
    {"AUTH_UID",                    TCGUID(0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x1C)},
    {"AUTH_UID_ENTERPRISE",         TCGUID(0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0C)},
    {"REVERT_UID",                  TCGUID(0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x02, 0x02)},
    {"REVERTSP_UID",                TCGUID(0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x11)},
    {"ACTIVATE_UID",                TCGUID(0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x02, 0x03)}, 
    {"ANYBODY_AUTH_UID",            TCGUID(0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x01)},
    {"ADMINS_AUTH_UID",             TCGUID(0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x02)},
    {"SID_AUTH_UID",                TCGUID(0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x06)},
    {"PSID_AUTH_UID",               TCGUID(0x00, 0x00, 0x00, 0x09, 0x00, 0x01, 0xFF, 0x01)}, 
    {"C_PIN_MSID_UID",              TCGUID(0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x84, 0x02)},
    {"C_PIN_ADMIN1_UID",            TCGUID(0x00, 0x00, 0x00, 0x0B, 0x00, 0x01, 0x00, 0x01)},
    {"LOCKING_SP_UID",              TCGUID(0x00, 0x00, 0x02, 0x05, 0x00, 0x00, 0x00, 0x02)},
    {"LOCKING_SP_UID_ENYTERPRISE",  TCGUID(0x00, 0x00, 0x02, 0x05, 0x00, 0x01, 0x00, 0x01)},
    {"LOCKING_SP_ADMIN1_UID",       TCGUID(0x00, 0x00, 0x00, 0x09, 0x00, 0x01, 0x00, 0x01)},
    {"LOCKING_GLOBAL_RANGE_UID",    TCGUID(0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x00, 0x01)},
    {"LOCKING_RANGE1_UID",          TCGUID(0x00, 0x00, 0x08, 0x02, 0x00, 0x03, 0x00, 0x01)},
    {"K_AES_256_RANGE1_KEY_UID",    TCGUID(0x00, 0x00, 0x08, 0x06, 0x00, 0x03, 0x00, 0x01)},
    {"K_AES_128_RANGE1_KEY_UID",    TCGUID(0x00, 0x00, 0x08, 0x05, 0x00, 0x03, 0x00, 0x01)},
    {"MBR_UID",                     TCGUID(0x00, 0x00, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00)},
    {"MBR_ROW_UID",                 TCGUID(0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x08, 0x04)},
    {"Authority",                   TCGUID(0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00)},
    {"UserMMMM",                    TCGUID(0x00, 0x00, 0x00, 0x09, 0x00, 0x03, 0x00, 0x00)},
    {"C_PIN_USERMMMM",              TCGUID(0x00, 0x00, 0x00, 0x0B, 0x00, 0x03, 0x00, 0x00)},
    {"ACE_Locking_GlobalRange_Set_RdLocked", TCGUID(0x00, 0x00, 0x00, 0x08, 0x00, 0x03, 0xE0, 0x00)},
    {"ACE_Locking_GlobalRange_Set_WrLocked", TCGUID(0x00, 0x00, 0x00, 0x08, 0x00, 0x03, 0xE8, 0x00)},
    {"ACE_Authority_Set_Enabled",            TCGUID(0x00, 0x00, 0x00, 0x08, 0x00, 0x03, 0x90, 0x01)},
    {"ACE_Authority_Get_All",                TCGUID(0x00, 0x00, 0x00, 0x08, 0x00, 0x03, 0x90, 0x00)},
    {"BANDMASTER0_UID",             TCGUID(0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x80, 0x01)},
    {"BANDMASTER1_UID",             TCGUID(0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x80, 0x02)},
    {"ERASEMASTER_UID",             TCGUID(0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x84, 0x01)},
    {"BANDMASTER0_PIN_UID",         TCGUID(0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x80, 0x01)},
    {"BANDMASTER1_PIN_UID",         TCGUID(0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x80, 0x02)},
    {"ERASEMASTER_PIN_UID",         TCGUID(0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x84, 0x01)},
    {"LOCKING_BAND1_UID_ENTERPRISE", TCGUID(0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x00, 0x02)}
};
#endif

//---------------------------------------------------------------------------

/** @internal
    Function converts Big endian word to Little Endian and Vice versa.
    
    In Enterprise SSC, instead of the Column #, column string is returned. ListPtr is pointing String which is preceeded with Token header. 
    DecodeTokenHeader() should be able to get the string.If String Matches, return 0.

    @param EndianWord

    @retval UINT16

    @endinternal
**/
INTN
CmpName(
        UINT8 *ListPtr, 
        UINT8 *StartColumnString
)
{
 
    UINT8   DecodedString[129];
    UINT32  DataBufferLength = sizeof (DecodedString);
    UINT32  Length = 0;
    UINT8   HeaderSize = 0;
    EFI_STATUS  Status;
   
//# AptioV_Override_START: EIP#642147 Address Security Advisory SA50093
    ZeroMem(DecodedString, DataBufferLength);
//# AptioV_Override_END: EIP#642147 Address Security Advisory SA50093
    Status = DecodeTokenHeader(ListPtr, DecodedString, &DataBufferLength, &Length, &HeaderSize);
    if (EFI_ERROR(Status)) return 1;
    return AsciiStrCmp ((const CHAR8 *)DecodedString, StartColumnString); 
}

/** @internal
    Function converts Big endian word to Little Endian and Vice versa.

    @param EndianWord

    @retval UINT16

    @endinternal
**/
UINT16
ToBigLittleEndianWord (
    UINT16 EndianWord
)
{

    return (((EndianWord >> 8) & 0xFF) + (EndianWord << 8));

}

/** @internal
    Function converts Big endian dword to Little Endian Dword and Vice versa.

    @param EndianDword

    @retval UINT32

    @endinternal
**/
UINT32
ToBigLittleEndianDword (
    UINT32 EndianDword
)
{

    return (((EndianDword & 0xFF) << 24) + ((EndianDword & 0xFF00) << 8) + \
            ((EndianDword & 0xFF0000) >>8) + ((EndianDword & 0xFF000000) >>24));


}

/** @internal
    Function converts Big endian Qword to Little Endian Qword and Vice versa.

    @param EndianDword

    @retval UINT64

    @endinternal
**/
UINT64
ToBigLittleEndianQword (
    UINT64  EndianQword
)
{

    return (LShiftU64 (EndianQword , 56) + LShiftU64 ((EndianQword & 0xFF00), 40) + \
            LShiftU64 ((EndianQword & 0xFF0000), 24) + LShiftU64 ((EndianQword & 0xFF000000), 8) + \
            RShiftU64 ((EndianQword & 0xFF00000000000000), 56) + RShiftU64 ((EndianQword & 0x00FF000000000000), 40) + \
            RShiftU64 ((EndianQword & 0x0000FF0000000000), 24) + RShiftU64 ((EndianQword & 0x000000FF00000000), 8)
    );


}


/** @internal
    Function does the Close session for the Opal Security Commands

    @param EndianDword

    @retval EFI_STATUS
    
    @endinternal
**/

EFI_STATUS
CheckCloseSession (
    VOID            *Buffer,
    UINTN           BufferLength
)
{
// For some of the cmds like revert, close session need not be called explicitly.
// Check whether Close session is executed in the response.
    COM_PACKET_FORMAT *ComPacket = Buffer;
    PACKET_FORMAT *Packet = (PACKET_FORMAT *)ComPacket->PayLoad;
    DATA_SUBPACKET_FORMAT *DataSubPacket = (DATA_SUBPACKET_FORMAT *)Packet->PayLoad;
    UINT64      SMUid = SM_UID;
    UINT64      CloseSessionUid = CLOSE_SESSION_UID;

    // Validate the SM UID and SyncSssion Method ID in the received data. The offsets are calculated based on
    // a successful startsession cmd. Look at Application note doc for details
    if (CompareMem (&(DataSubPacket->PayLoad[2]), &SMUid, 8)) {
        return EFI_NOT_FOUND;
    }

    if (CompareMem (&(DataSubPacket->PayLoad[11]), &CloseSessionUid, 8)) {
        return EFI_NOT_FOUND;
    }

    return EFI_SUCCESS;
}

/** @internal
    Decodes the response data for the correct method status codes

    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.
    @param  bData             - Status value

    @retval EFI_STATUS
    
    @endinternal
**/

// TCG_Storage_Architechture Spec v2.01 Section 5.1.5
EFI_STATUS
GetStatusValue (
    VOID            *Buffer,
    UINTN           BufferLength,
    UINT8           *bData
)
{

    COM_PACKET_FORMAT *ComPacket = Buffer;
    PACKET_FORMAT *Packet = (PACKET_FORMAT *)ComPacket->PayLoad;
    DATA_SUBPACKET_FORMAT *DataSubPacket = (DATA_SUBPACKET_FORMAT *)Packet->PayLoad;
    UINT32      Length = ToBigLittleEndianDword(DataSubPacket->Length);
    UINT8       Index = 0;

    if (DataSubPacket->PayLoad[0] == EOS_TOKEN){
        return EFI_SUCCESS;
    }
    
	// Return if Response Data is not filled in DataSubPacket
    if(Length == 0 ) {
        return EFI_DEVICE_ERROR;
    }
	
    // Skip if there are any Empty tokens at the bottom
    for (Index=1; Index<Length; Index++){
        if (DataSubPacket->PayLoad[Length - Index] != 0xFF) break;
    }


    // The last 5 bytes of DataPayload is a List token with 3 bytes of data. Only the first byte has the
    // status and the test two bytes are reserved. Sec 3.2.4.2 Core Spec (Status Code List)
    if ((DataSubPacket->PayLoad[Length - Index - 4] == START_LIST) && (DataSubPacket->PayLoad[Length - Index] == END_LIST)){
        *bData = DataSubPacket->PayLoad[Length - Index - 4 + 1];
        DEBUG ((DEBUG_VERBOSE, "Method Status : %2X\n", *bData));
        if (*bData == 0) {
            return EFI_SUCCESS;
        }
    }
    return EFI_DEVICE_ERROR;
}

/** @internal
    Initializes the COM Packet data structure.
    Length filed will be updated after constructing the entire packet.

    @param  PacketBuffer          - Payload buffer
    @param  TotalBufferLength     - Size in bytes of the payload data buffer.
    @param  ComID                 - Device COMID
    @param  ComIDExt              - Extended COMID value

    @retval EFI_STATUS

    @endinternal
**/

// TCG_Storage_Architechture Spec v2.01 Section 3.2.3.1

EFI_STATUS
InitComPacket (
    VOID                *PacketBuffer,
    UINTN               TotalBufferLength,
    UINT16              ComID,
    UINT16              ComIDExt
)
{

    COM_PACKET_FORMAT *ComPacket = PacketBuffer;

    if (sizeof (COM_PACKET_FORMAT) > TotalBufferLength) {
        return EFI_BUFFER_TOO_SMALL;
    }

    ZeroMem(ComPacket, sizeof (COM_PACKET_FORMAT));

    ComPacket->ComID = ToBigLittleEndianWord(ComID);
    ComPacket->ComIDExtension = ToBigLittleEndianWord(ComIDExt);

    return EFI_SUCCESS;

}

/** @internal
    Initializes the Packet data structure.
    Length filed will be updated after constructing the entire packet.

    @param  PacketBuffer          - Payload buffer
    @param  TotalBufferLength     - Size in bytes of the payload data buffer.
    @param  Tsn                 - TSN value
    @param  Hsn                 - HSN value

    @retval EFI_STATUS

    @endinternal
**/

// TCG_Storage_Architechture Spec v2.01 Section 3.2.3.3

EFI_STATUS
InitPacket (
    VOID                *PacketBuffer,
    UINTN               TotalBufferLength,
    UINT32              Tsn,
    UINT32              Hsn,
    UINT32              SeqNumber
)
{

    COM_PACKET_FORMAT *ComPacket = PacketBuffer;
    PACKET_FORMAT *Packet = (PACKET_FORMAT *)ComPacket->PayLoad;

    if (sizeof (PACKET_FORMAT) +  sizeof (COM_PACKET_FORMAT) - 1 > TotalBufferLength) {
        return EFI_BUFFER_TOO_SMALL;
    }

    ZeroMem(Packet, sizeof (PACKET_FORMAT));

    Packet->TSN = ToBigLittleEndianDword(Tsn);
    Packet->HSN = ToBigLittleEndianDword(Hsn);
    Packet->SeqNumber = ToBigLittleEndianDword(SeqNumber);
    ComPacket->Length = sizeof(PACKET_FORMAT) - 1;

    return EFI_SUCCESS;

}

/** @internal
    Initializes the data subpacket structure.
    Length filed will be updated after constructing the entire packet.

    @param  PacketBuffer          - Payload buffer
    @param  TotalBufferLength     - Size in bytes of the payload data buffer.
    @param  Kind                  - data subpacket kind

    @retval EFI_STATUS

    @endinternal
**/

// TCG_Storage_Architechture Spec v2.01 Section 3.2.3.4

EFI_STATUS
InitDataSubPacket (
    VOID                *PacketBuffer,
    UINTN               TotalBufferLength,
    UINT16              Kind
)
{

    COM_PACKET_FORMAT *ComPacket = PacketBuffer;
    PACKET_FORMAT *Packet = (PACKET_FORMAT *)ComPacket->PayLoad;
    DATA_SUBPACKET_FORMAT *DataSubPacket = (DATA_SUBPACKET_FORMAT *)Packet->PayLoad;

    if (sizeof (DATA_SUBPACKET_FORMAT) + sizeof (PACKET_FORMAT) +  sizeof (COM_PACKET_FORMAT) - 2 > TotalBufferLength) {
        return EFI_BUFFER_TOO_SMALL;
    }

    ZeroMem(DataSubPacket, sizeof (DATA_SUBPACKET_FORMAT));
    DataSubPacket->Kind = Kind;

    Packet->Length += (sizeof(DATA_SUBPACKET_FORMAT) - 1);
    ComPacket->Length += (sizeof(DATA_SUBPACKET_FORMAT) - 1);

    return EFI_SUCCESS;

}

/** @internal
    Add Status Code List

    @param  PacketBuffer          - Payload buffer
    @param  TotalBufferLength     - Size in bytes of the payload data buffer.
    @param  Kind                  - data subpacket kind

    @retval EFI_STATUS

    @note
     SubPacket length should be multiples of 4. So introduce padding bytes if needed at the end.
     Finally it updates the length field in ComPacket, Packet and SubPacket

    @endinternal
**/

// Add Status Code List. SubPacket length should be multiples of 4. So introduce padding bytes if needed at the end.
// Finally it updates the length field in ComPacket, Packet and SubPacket
EFI_STATUS
InsertMethodStatusAndPadList (
    VOID                *PacketBuffer,
    UINTN               TotalBufferLength
)
{

    COM_PACKET_FORMAT       *Compacket = (COM_PACKET_FORMAT *)PacketBuffer;
    PACKET_FORMAT           *Packet = (PACKET_FORMAT *)Compacket->PayLoad;
    DATA_SUBPACKET_FORMAT   *DataSubPacket = (DATA_SUBPACKET_FORMAT *)Packet->PayLoad;
    UINT32                  Offset = DataSubPacket->Length;
    UINT8                   Index, PadLength;

    DataSubPacket->PayLoad[Offset] = 0xF0;
    DataSubPacket->PayLoad[Offset + 1] = 0x00;
    DataSubPacket->PayLoad[Offset + 2] = 0x00;
    DataSubPacket->PayLoad[Offset + 3] = 0x00;
    DataSubPacket->PayLoad[Offset + 4] = 0xF1;

    DataSubPacket->Length += 5;
    Packet->Length += 5;
    Compacket->Length += 5;

    //Add Pad
    PadLength = DataSubPacket->Length % 4;
    if (PadLength){
        PadLength = 4 - PadLength;
    }
    for (Index = 0; Index < PadLength; Index ++){
        DataSubPacket->PayLoad[DataSubPacket->Length + Index] = 0;
    }

    Packet->Length += PadLength;
    Compacket->Length += PadLength;

    return EFI_SUCCESS;

}

/** @internal
    The routine decodes the various token types defined in the TCG OPAL spec.

    @param  DataSubPacket          - Packet buffer
    @param  DataBuffer             - Payload data buffer.
    @param  DataBufferLength       - Size in bytes of the payload data buffer.
    @param  Length                 - each data sub packet length
    @param  HeaderSize             - each data sub packet Header Size

    @retval EFI_STATUS

    @endinternal
**/

// TCG_Storage_Architechture Spec v2.01 Section 3.2.2.3
EFI_STATUS
DecodeTokenHeader (
    UINT8   *DataSubPacket,
    UINT8   *DataBuffer,
    UINT32  *DataBufferLength,
    UINT32  *Length,
    UINT8   *HeaderSize
)
{

    UINT32  Index;
    BOOLEAN Integer = FALSE;
    *Length = 0;
    *HeaderSize = 0;

    // Tiny Atom
    if ((*DataSubPacket & 0x80) == 0) {
        *Length = 1;
        if (*DataBufferLength >= 1){
            *DataBuffer = *DataSubPacket & 0x3F;
            return EFI_SUCCESS;
        }
        *DataBufferLength = 1;
        return EFI_BUFFER_TOO_SMALL;
    }

    // Short Atom
    if ((*DataSubPacket & 0xC0) == 0x80) {
        *HeaderSize = 1;
        *Length = *DataSubPacket & 0xF;
        if (!(*DataSubPacket & 0x20)) Integer = TRUE;

    }

    // Medium Atom
    if ((*DataSubPacket & 0xE0) == 0xC0) {
        *HeaderSize = 2;
        *Length =  ((*DataSubPacket & 0x7) << 8) + *(DataSubPacket+1);
        if (!(*DataSubPacket & 0x10)) Integer = TRUE;
    }

    //Long Atom
    if ((*DataSubPacket & 0xFC) == 0xE0) {
        *HeaderSize = 4;
        *Length =  (*(DataSubPacket+1) << 16) + (*(DataSubPacket+2) << 8) + *(DataSubPacket+3);
    }

    if (*Length == 0) {
        return EFI_DEVICE_ERROR;
    }
    if (DataBuffer == NULL) {
        *DataBufferLength = *Length;
        return EFI_SUCCESS;
    }

    if (*DataBufferLength < *Length){
        *DataBufferLength = *Length;
        return EFI_BUFFER_TOO_SMALL;
    }

    ZeroMem(DataBuffer, *DataBufferLength);

    // change it from Big to Little Endian format
    for (Index = 0; Index < *Length; Index++){
        if (Integer){
        DataBuffer[*Length - Index - 1] = DataSubPacket[Index + *HeaderSize];
        } else {
            DataBuffer[Index] = DataSubPacket[Index + *HeaderSize];
        }
    }

    return EFI_SUCCESS;

}

/** @internal
    Inserts Tiny Atom in the cmd block. Updates the length field in
    ComPacket, Packet and SubPacket

    @param  PacketBuffer          - payload buffer
    @param  TotalBufferLength     - Size in bytes of the payload data buffer.
    @param  bData                 - Data to insert
    @param  SignUnSigned          - Is data a Signed or unsigned value
    @param  Raw                   - Is data a raw data, if so will be inserted as such

    @retval EFI_STATUS
    
    @endinternal
**/

// TCG_Storage_Architechture Spec v2.01 Section 3.2.2.3.1.1

EFI_STATUS
InsertTinyAtom (
    VOID                *PacketBuffer,
    UINTN               TotalBufferLength,
    UINT8               bData,
    BOOLEAN             SignUnSigned,
    BOOLEAN             Raw
)
{

    COM_PACKET_FORMAT       *Compacket = (COM_PACKET_FORMAT *)PacketBuffer;
    PACKET_FORMAT           *Packet = (PACKET_FORMAT *)Compacket->PayLoad;
    DATA_SUBPACKET_FORMAT   *DataSubPacket = (DATA_SUBPACKET_FORMAT *)Packet->PayLoad;
    UINT32                  Offset = DataSubPacket->Length;

    if ((Compacket->Length + 1 + sizeof (COM_PACKET_FORMAT) - 1) > TotalBufferLength) {
        return EFI_BUFFER_TOO_SMALL;
    }

   if (Raw) {
        DataSubPacket->PayLoad[Offset] = bData;
    } else {
        DataSubPacket->PayLoad[Offset] = (bData & 0x3F) + (SignUnSigned << 6);
    }
    DataSubPacket->Length++;
    Packet->Length++;
    Compacket->Length++;

    return EFI_SUCCESS;

}

/** @internal
    Inserts Short, Medium, Long Atom in the cmd block. Updates the length field in
    ComPacket, Packet and SubPacket

    @param  PacketBuffer          - payload buffer
    @param  TotalBufferLength     - Size in bytes of the payload data buffer.
    @param  bData                 - Data to insert
    @param  NumofBytes            - Length of Data
    @param  SignUnSigned          - Is data a Signed or unsigned value
    @param  ByteInteger           - byte data or integer data

    @retval EFI_STATUS
    
    @endinternal
**/
// TCG_Storage_Architechture Spec v2.01 Section 3.2.2.3.1.1
EFI_STATUS
InsertShortMediumLongAtom (
    VOID                *PacketBuffer,
    UINTN               TotalBufferLength,
    UINT8               *bData,
    UINTN               NumofBytes,
    BOOLEAN             SignUnSigned,
    BOOLEAN             ByteInteger
)
{

    COM_PACKET_FORMAT       *Compacket = (COM_PACKET_FORMAT *)PacketBuffer;
    PACKET_FORMAT           *Packet = (PACKET_FORMAT *)Compacket->PayLoad;
    DATA_SUBPACKET_FORMAT   *DataSubPacket = (DATA_SUBPACKET_FORMAT *)Packet->PayLoad;
    UINT32                  Offset = DataSubPacket->Length;
    UINT32                  Index, HeaderSize;

    if ( (NumofBytes == 1) && (*bData < 0x40) && (ByteInteger == INTEGER_DATA) ) {
        return InsertTinyAtom(PacketBuffer, TotalBufferLength, *bData, SignUnSigned, TRUE);
    }

    // 1 Byte. Short Atom
    if (NumofBytes < 0x10) {

        if ((Compacket->Length + NumofBytes + 1 + sizeof (COM_PACKET_FORMAT) - 1) > TotalBufferLength) {
            return EFI_BUFFER_TOO_SMALL;
        }
        HeaderSize = 1;
        DataSubPacket->PayLoad[Offset] = 0x80 + (ByteInteger << 5) + (SignUnSigned << 4) + (NumofBytes & 0x0F);
    }
    else {
        // 2 Bytes header. Medium Atom
        if (NumofBytes <= 2047) {

            if ((Compacket->Length + NumofBytes + 2 + sizeof (COM_PACKET_FORMAT) - 1) > TotalBufferLength) {
                return EFI_BUFFER_TOO_SMALL;
            }
            // Big Endian
            HeaderSize = 2;
            DataSubPacket->PayLoad[Offset] = 0xC0 + (ByteInteger << 4) + (SignUnSigned << 3) + ((NumofBytes & 0x700) >> 8);
            DataSubPacket->PayLoad[Offset + 1] = NumofBytes & 0xFF;

        }
        else {
            // 4 bytes header.  Long Atom
            if (NumofBytes <= 16777215) {

                if ((Compacket->Length + NumofBytes + 4 +  sizeof (COM_PACKET_FORMAT) - 1) > TotalBufferLength) {
                    return EFI_BUFFER_TOO_SMALL;
                }
                //BigEndian
                HeaderSize = 4;
                DataSubPacket->PayLoad[Offset] = 0xE0 + (ByteInteger << 1) + SignUnSigned;
                DataSubPacket->PayLoad[Offset + 1] = (UINT8)((NumofBytes & 0xFF0000) >> 16);
                DataSubPacket->PayLoad[Offset + 2] = (NumofBytes & 0xFF00) >> 8;
                DataSubPacket->PayLoad[Offset + 3] = NumofBytes & 0xFF;
            }
            else {
                return EFI_INVALID_PARAMETER;
            }
        }
    }

    //BigEndian
    for (Index = 0; Index < NumofBytes; Index++) {
        if (ByteInteger) {
            DataSubPacket->PayLoad[Offset + HeaderSize + Index] = bData[Index];
        } else {
            DataSubPacket->PayLoad[Offset + HeaderSize + Index] = bData[NumofBytes - 1 -Index];
        }
    }

    DataSubPacket->Length += (UINT32)(NumofBytes + HeaderSize);
    Packet->Length += (UINT32)(NumofBytes + HeaderSize);
    Compacket->Length += (UINT32)(NumofBytes + HeaderSize);

    return EFI_SUCCESS;

}

/** @internal
    Reverses input "bData" buffer and call InsertShortMediumLongAtom function to insert
    the same in Opal Device Compacket buffer

    @param  PacketBuffer          - payload buffer
    @param  TotalBufferLength     - Size in bytes of the payload data buffer.
    @param  bData                 - Data to insert
    @param  NumofBytes            - Length of Data
    @param  SignUnSigned          - Is data a Signed or unsigned value
    @param  ByteInteger           - byte data or integer data

    @retval EFI_STATUS

    @note  "bData" Password buffer is reversed to be compatible with Password installed 
           by "TcgStorageSecurity_07 or below" Labels

    @endinternal
**/
EFI_STATUS
ReverseAndInsertShortMediumLongAtom (
    VOID                *PacketBuffer,
    UINTN               TotalBufferLength,
    UINT8               *bData,
    UINTN               NumofBytes,
    BOOLEAN             SignUnSigned,
    BOOLEAN             ByteInteger
)
{
    UINTN    Index;
    UINT8    *Data;
    EFI_STATUS   Status;
    
    Status = TcgSecAllocatePool(EfiBootServicesData, NumofBytes, (VOID**)&Data);
    TCG_STORAGE_ERR_CHECK(Status);
    
    for (Index = 0; Index < NumofBytes; Index++) {
        Data[Index] = bData[NumofBytes - 1 -Index];
    }
    
    InsertShortMediumLongAtom (PacketBuffer, TotalBufferLength, Data, NumofBytes, SignUnSigned, ByteInteger);
    
    TcgSecFreePool (Data);
    return EFI_SUCCESS;
}

/** @internal
    Builds basic command block

    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.
    @param  InvokingID        - Invoking UID
    @param  MethodUid         - Operation UID

    @retval EFI_STATUS
    
    @endinternal
**/
// TCG_Storage_Architechture Spec v2.01 Sec 3.2.4
EFI_STATUS
BuildBasicCmdBlock (
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    VOID                *Buffer,
    UINTN               BufferLength,
    UINT64              InvokingID,
    UINT64              MethodUid
)
{

    COM_PACKET_FORMAT       *Compacket = (COM_PACKET_FORMAT *)Buffer;
    PACKET_FORMAT           *Packet = (PACKET_FORMAT *)Compacket->PayLoad;
    DATA_SUBPACKET_FORMAT   *DataSubPacket = (DATA_SUBPACKET_FORMAT *)Packet->PayLoad;

// Section 5.2.3.1 StartSession Method TCG Core Spec v2.01
    InitComPacket (Buffer, BufferLength, OpalDevice->OpalFeature.BaseComID, OpalDevice->ComIDExt);
    InitPacket (Buffer, BufferLength, SessionParams->Tsn, SessionParams->Hsn, 0);
    InitDataSubPacket (Buffer, BufferLength, 0);

    InsertTinyAtom (Buffer, BufferLength, CALL_TOKEN, UNSIGNED_DATA, TRUE);
      InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(InvokingID), 8, UNSIGNED_DATA, BYTE_DATA);
        InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(MethodUid), 8, UNSIGNED_DATA, BYTE_DATA);
        InsertTinyAtom (Buffer, BufferLength, START_LIST, UNSIGNED_DATA, TRUE);   // Begin List
      InsertTinyAtom (Buffer, BufferLength, END_LIST, UNSIGNED_DATA, TRUE);     // End List
    InsertTinyAtom (Buffer, BufferLength, EOD_TOKEN, UNSIGNED_DATA, TRUE);    // Ends Method

    InsertMethodStatusAndPadList (Buffer, BufferLength);

    //change the length to Big Endian format
    DataSubPacket->Length = ToBigLittleEndianDword (DataSubPacket->Length);
    Packet->Length = ToBigLittleEndianDword (Packet->Length);
    Compacket->Length = ToBigLittleEndianDword (Compacket->Length);

    return EFI_SUCCESS;

}

/** @internal
    Builds Start Session command block

    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.
    @param  SessionParams     - information on current session.

    @retval EFI_STATUS
    
    @endinternal
**/
// TCG_Storage_Architechture Spec v2.01 Sec 5.2.3.1

EFI_STATUS
BuildStartSessionCmdBlock (
    OPAL_DEVICE     *OpalDevice,
    VOID            *Buffer,
    UINTN           BufferLength,
    SESSION_PARAMS  *SessionParams
)
{

    COM_PACKET_FORMAT       *Compacket = (COM_PACKET_FORMAT *)Buffer;
    PACKET_FORMAT           *Packet = (PACKET_FORMAT *)Compacket->PayLoad;
    DATA_SUBPACKET_FORMAT   *DataSubPacket = (DATA_SUBPACKET_FORMAT *)Packet->PayLoad;


// Section 5.2.3.1 StartSession Method TCG Core Spec v2.01
    InitComPacket (Buffer, BufferLength, OpalDevice->OpalFeature.BaseComID, OpalDevice->ComIDExt);
    InitPacket (Buffer, BufferLength, SessionParams->Tsn, SessionParams->Hsn, 0);
    InitDataSubPacket (Buffer, BufferLength, 0);

    InsertTinyAtom (Buffer, BufferLength, CALL_TOKEN, UNSIGNED_DATA, TRUE);                   // Begin Method

    InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(SessionParams->InvokingID), 8, UNSIGNED_DATA, BYTE_DATA);
    InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(SessionParams->MethodID), 8, UNSIGNED_DATA, BYTE_DATA);

    InsertTinyAtom (Buffer, BufferLength, START_LIST, UNSIGNED_DATA, TRUE);                   // Begin List

    // Required Parameters
    InsertTinyAtom (Buffer, BufferLength, (UINT8)SessionParams->HostSessionID, UNSIGNED_DATA, FALSE);    // HostSession ID
    InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(SessionParams->SPID), 8, UNSIGNED_DATA, BYTE_DATA); // UID of SP
    InsertTinyAtom (Buffer, BufferLength, SessionParams->Write, UNSIGNED_DATA, FALSE);

        // Optional Parameters. Name Value start from Zero for optional parameters
        if (SessionParams->HostChallenge){
            InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
            if (OpalDevice->OpalFeature.FeatureCode == ENTERPRISE_SSC_Feature){
                InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)"Challenge", AsciiStrLen("Challenge"), UNSIGNED_DATA, BYTE_DATA);
            } else {
                InsertTinyAtom (Buffer, BufferLength, 0, UNSIGNED_DATA, FALSE);
            }
            ReverseAndInsertShortMediumLongAtom (Buffer, BufferLength, SessionParams->HostChallenge, \
                    SessionParams->HostChallengeLength, UNSIGNED_DATA, BYTE_DATA);
            InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);
        }

        if (SessionParams->HostExchangeAuth){
            InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
            InsertTinyAtom (Buffer, BufferLength, 1, UNSIGNED_DATA, FALSE);
            InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&SessionParams->HostExchangeAuth, 8, UNSIGNED_DATA, BYTE_DATA);
            InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);
        }

        if (SessionParams->HostExchangeCert){
            InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
            InsertTinyAtom (Buffer, BufferLength, 2, UNSIGNED_DATA, FALSE);
            InsertShortMediumLongAtom (Buffer, BufferLength, SessionParams->HostExchangeCert, \
                    SessionParams->HostExchangeCertLength, UNSIGNED_DATA, BYTE_DATA);
            InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);
        }

        if (SessionParams->HostSigningAuthority){
            InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
            if (OpalDevice->OpalFeature.FeatureCode == ENTERPRISE_SSC_Feature){
                InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)"HostSigningAuthority", \
                        AsciiStrLen("HostSigningAuthority"), UNSIGNED_DATA, BYTE_DATA);
            } else {
                InsertTinyAtom (Buffer, BufferLength, 3, UNSIGNED_DATA, FALSE);
            }
            InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&SessionParams->HostSigningAuthority, 8, UNSIGNED_DATA, BYTE_DATA);
            InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);
        }

        if (SessionParams->HostSigningCert){
            InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
            InsertTinyAtom (Buffer, BufferLength, 4, UNSIGNED_DATA, FALSE);
            InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)SessionParams->HostSigningCert, \
                    SessionParams->HostSigningCertLength, UNSIGNED_DATA, BYTE_DATA);
            InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);
        }

        if (SessionParams->SessionTimeOut){
            InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
            if (OpalDevice->OpalFeature.FeatureCode == ENTERPRISE_SSC_Feature){
                InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)"SessionTimeout", AsciiStrLen("SessionTimeout"), UNSIGNED_DATA, BYTE_DATA);
            } else {
                InsertTinyAtom (Buffer, BufferLength, 5, UNSIGNED_DATA, FALSE);
            }
            InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)SessionParams->SessionTimeOut, 8, UNSIGNED_DATA, INTEGER_DATA);
            InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);
        }

        if (SessionParams->TransTimeout){
            InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
            InsertTinyAtom (Buffer, BufferLength, 6, UNSIGNED_DATA, FALSE);
            InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)SessionParams->TransTimeout, 8, UNSIGNED_DATA, INTEGER_DATA);
            InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);
        }

        if (SessionParams->InitialCredit){
            InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
            InsertTinyAtom (Buffer, BufferLength, 7, UNSIGNED_DATA, FALSE);
            InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)SessionParams->InitialCredit, 8, UNSIGNED_DATA, INTEGER_DATA);
            InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);
        }

        if (SessionParams->SignedHash){
            InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
            InsertTinyAtom (Buffer, BufferLength, 8, UNSIGNED_DATA, FALSE);
            InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)SessionParams->SignedHash, \
                    SessionParams->SignedHashLength, UNSIGNED_DATA, BYTE_DATA);
            InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);
        }

    InsertTinyAtom (Buffer, BufferLength, END_LIST, UNSIGNED_DATA, TRUE);     // End List
    InsertTinyAtom (Buffer, BufferLength, EOD_TOKEN, UNSIGNED_DATA, TRUE);    // Ends Method

    InsertMethodStatusAndPadList (Buffer, BufferLength);

    //change the length to Big Endian format
    DataSubPacket->Length = ToBigLittleEndianDword (DataSubPacket->Length);
    Packet->Length = ToBigLittleEndianDword (Packet->Length);
    Compacket->Length = ToBigLittleEndianDword (Compacket->Length);

    return EFI_SUCCESS;
}

/** @internal
    Builds SetBooleanExpression command block

    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.
    @param  TableObjUid       - UID of the table
    @param  Uid1              - User Authority UID 1
    @param  Uid2              - User Authority UID 2
    @param  BooleanOperator   - Boolean value to be set [0 or 1]

    @retval EFI_STATUS
    
    @endinternal
**/
// TCG_Storage_Architechture Spec v2.01 Sec 5.3.3.7

EFI_STATUS
BuildSetBooleanExpressionCmdBlock (
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    VOID                *Buffer,
    UINTN               BufferLength,
    UINT64              TableObjUid,
    UINT64              Uid1,
    UINT64              Uid2,
    BOOLEAN             BooleanOperator
)
{

    COM_PACKET_FORMAT       *Compacket = (COM_PACKET_FORMAT *)Buffer;
    PACKET_FORMAT           *Packet = (PACKET_FORMAT *)Compacket->PayLoad;
    DATA_SUBPACKET_FORMAT   *DataSubPacket = (DATA_SUBPACKET_FORMAT *)Packet->PayLoad;
    UINT64                  SetUid = SET_KEY_UID;
    UINT32                  AuthorityObjRef = ToBigLittleEndianDword(0x00000C05);
    UINT32                  BooleanAce = ToBigLittleEndianDword(0x0000040E);

    // Section 5.2.3.1 StartSession Method TCG Core Spec v2.01
    InitComPacket (Buffer, BufferLength, OpalDevice->OpalFeature.BaseComID, OpalDevice->ComIDExt);
    InitPacket (Buffer, BufferLength, SessionParams->Tsn, SessionParams->Hsn, 0);
    InitDataSubPacket (Buffer, BufferLength, 0);

    if (OpalDevice->OpalFeature.FeatureCode == ENTERPRISE_SSC_Feature){
        SetUid = SET_KEY_UID_ENTERPRISE;    
    }
    InsertTinyAtom (Buffer, BufferLength, CALL_TOKEN, UNSIGNED_DATA, TRUE);                   // Begin Method

    InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(TableObjUid), 8, UNSIGNED_DATA, BYTE_DATA);
    InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(SetUid), 8, UNSIGNED_DATA, BYTE_DATA);

    InsertTinyAtom (Buffer, BufferLength, START_LIST, UNSIGNED_DATA, TRUE); // Begin Parameter list
        InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
           InsertTinyAtom (Buffer, BufferLength, 1, UNSIGNED_DATA, TRUE);  // Name = value of 1 (Optional Parameter 1)

            InsertTinyAtom (Buffer, BufferLength, START_LIST, UNSIGNED_DATA, TRUE); // Begin Parameter list
              InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
                InsertTinyAtom (Buffer, BufferLength, 3, UNSIGNED_DATA, TRUE);  // BooleanExpr

                InsertTinyAtom (Buffer, BufferLength, START_LIST, UNSIGNED_DATA, TRUE); // Begin Parameter list
                  InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);

                      //Half-UID
                      InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(AuthorityObjRef), 4, UNSIGNED_DATA, BYTE_DATA);
                      InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(Uid1), 8, UNSIGNED_DATA, BYTE_DATA);

                  InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);

                  InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);

                      //Half-UID
                      InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(AuthorityObjRef), 4, UNSIGNED_DATA, BYTE_DATA);
                      InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(Uid2), 8, UNSIGNED_DATA, BYTE_DATA);

                  InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);

                  InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);

                      //Half-UID
                      InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(BooleanAce), 4, UNSIGNED_DATA, BYTE_DATA);
                      InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(BooleanOperator), 1, UNSIGNED_DATA, INTEGER_DATA);

                  InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);

                InsertTinyAtom (Buffer, BufferLength, END_LIST, UNSIGNED_DATA, TRUE);



              InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);
            InsertTinyAtom (Buffer, BufferLength, END_LIST, UNSIGNED_DATA, TRUE);

        InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);
    InsertTinyAtom (Buffer, BufferLength, END_LIST, UNSIGNED_DATA, TRUE); // End Parameter List


    InsertTinyAtom (Buffer, BufferLength, EOD_TOKEN, UNSIGNED_DATA, TRUE); // End Method

    InsertMethodStatusAndPadList (Buffer, BufferLength);

    //change the length to Big Endian format
    DataSubPacket->Length = ToBigLittleEndianDword (DataSubPacket->Length);
    Packet->Length = ToBigLittleEndianDword (Packet->Length);
    Compacket->Length = ToBigLittleEndianDword (Compacket->Length);

    return EFI_SUCCESS;

}

/** @internal
    Builds SET Command block

    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.
    @param  TableObjUid       - UID of the table
    @param  Column            - Column # to set
    @param  Pin               - Value to set
    @param  PinLength         - Value length
    @param  ByteInteger       - byte data or integer data
    @param  EncodingType      - Encoding type of the data

    @retval EFI_STATUS
    
    @endinternal
**/

//See section 5.3.3.7
// Supports only Values (Option 1)
EFI_STATUS
BuildSetCmdBlock (
    OPAL_DEVICE     *OpalDevice,
    SESSION_PARAMS  *SessionParams,
    VOID            *Buffer,
    UINTN           BufferLength,
    UINT64          TableObjUid,
    UINT8           Column,
    CHAR8           *ColumnName,
    UINT8           *Pin,
    UINTN           PinLength,
    BOOLEAN         ByteInteger,
    COLUMN_ENCODING_TYPES EncodingType,
    BOOLEAN         Header,
    BOOLEAN         Tail
)
{

    COM_PACKET_FORMAT       *Compacket = (COM_PACKET_FORMAT *)Buffer;
    PACKET_FORMAT           *Packet = (PACKET_FORMAT *)Compacket->PayLoad;
    DATA_SUBPACKET_FORMAT   *DataSubPacket = (DATA_SUBPACKET_FORMAT *)Packet->PayLoad;
    UINT64                  SetUid = SET_KEY_UID;
    UINTN                   Index = 0;

    if (Header) {
      // Section 5.2.3.1 StartSession Method TCG Core Spec v2.01
      InitComPacket (Buffer, BufferLength, OpalDevice->OpalFeature.BaseComID, OpalDevice->ComIDExt);
      InitPacket (Buffer, BufferLength, SessionParams->Tsn, SessionParams->Hsn, 0);
      InitDataSubPacket (Buffer, BufferLength, 0);

      if (OpalDevice->OpalFeature.FeatureCode == ENTERPRISE_SSC_Feature){
         SetUid = SET_KEY_UID_ENTERPRISE;    
      }
      InsertTinyAtom (Buffer, BufferLength, CALL_TOKEN, UNSIGNED_DATA, TRUE);                   // Begin Method

      InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(TableObjUid), 8, UNSIGNED_DATA, BYTE_DATA);
      InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(SetUid), 8, UNSIGNED_DATA, BYTE_DATA);

      InsertTinyAtom (Buffer, BufferLength, START_LIST, UNSIGNED_DATA, TRUE); // Begin Parameter list
      if (IS_ENTERPRISE_DEVICE(OpalDevice)) {
         InsertTinyAtom (Buffer, BufferLength, START_LIST, UNSIGNED_DATA, TRUE);  
         InsertTinyAtom (Buffer, BufferLength, END_LIST, UNSIGNED_DATA, TRUE);  
      }
      
      if (IS_ENTERPRISE_DEVICE(OpalDevice)) {
        InsertTinyAtom (Buffer, BufferLength, START_LIST, UNSIGNED_DATA, TRUE);
      } else {
        InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
           InsertTinyAtom (Buffer, BufferLength, 1, UNSIGNED_DATA, TRUE);  // Name = value of 1 (Optional Parameter 1)
      }
            InsertTinyAtom (Buffer, BufferLength, START_LIST, UNSIGNED_DATA, TRUE); // Begin Parameter list
    }
              InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
              if (OpalDevice->OpalFeature.FeatureCode == ENTERPRISE_SSC_Feature){
                  InsertShortMediumLongAtom (Buffer, BufferLength, ColumnName, AsciiStrLen(ColumnName), UNSIGNED_DATA, BYTE_DATA);
              } else {
                InsertTinyAtom (Buffer, BufferLength, Column, UNSIGNED_DATA, TRUE);  // Name = Column Value
              }

                switch (EncodingType) {
                    case SIMPLE_TYPES:
                        ReverseAndInsertShortMediumLongAtom(Buffer, BufferLength, Pin, PinLength, UNSIGNED_DATA, BYTE_DATA);
                        break;
                    case RESTRICTED_REFERNECE_TYPES:
                    case GENERAL_REFERNCE_TYPES:
                        if (ByteInteger){
                            InsertShortMediumLongAtom (Buffer, BufferLength, Pin, PinLength, UNSIGNED_DATA, BYTE_DATA);
                        }
                        else {
                            InsertShortMediumLongAtom (Buffer, BufferLength, Pin, PinLength, UNSIGNED_DATA, INTEGER_DATA);
                        }
                        break;

                    case ENUMERATION_TYPES:
                        InsertShortMediumLongAtom (Buffer, BufferLength, Pin, PinLength, UNSIGNED_DATA, INTEGER_DATA);
                        break;

                    case NAMED_VALUE_TYPES:
                        InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
                        InsertShortMediumLongAtom (Buffer, BufferLength, Pin, 1, UNSIGNED_DATA, BYTE_DATA);
                        InsertShortMediumLongAtom (Buffer, BufferLength, Pin + 1, 1, UNSIGNED_DATA, BYTE_DATA);
                        InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);
                        break;

                    case LIST_TYPES:
                    case SET_VALUE_TYPES:
                        InsertTinyAtom (Buffer, BufferLength, START_LIST, UNSIGNED_DATA, TRUE);
                        do {
                            InsertShortMediumLongAtom (Buffer, BufferLength, Pin + Index, 1, UNSIGNED_DATA, INTEGER_DATA);
                            Index++;
                        }while (--PinLength); 
                        InsertTinyAtom (Buffer, BufferLength, END_LIST, UNSIGNED_DATA, TRUE);
                        break;

                    case ALTERNATIVE_TYPES:
                    case STRUCT_VALUE_TYPES:
                        DEBUG((DEBUG_VERBOSE, "Not supported\n"));
                        return EFI_NOT_FOUND;

                    default:
                        DEBUG((DEBUG_VERBOSE, "Not recognized COLUMN_ENCODING_TYPES value passed\n"));
                        return EFI_NOT_FOUND;
                };

              InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);
      if (Tail) {
            InsertTinyAtom (Buffer, BufferLength, END_LIST, UNSIGNED_DATA, TRUE);

            if (IS_ENTERPRISE_DEVICE(OpalDevice)) {
               InsertTinyAtom (Buffer, BufferLength, END_LIST, UNSIGNED_DATA, TRUE);
            } else {
               InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);
            }
         InsertTinyAtom (Buffer, BufferLength, END_LIST, UNSIGNED_DATA, TRUE); // End Parameter List


        InsertTinyAtom (Buffer, BufferLength, EOD_TOKEN, UNSIGNED_DATA, TRUE); // End Method

       InsertMethodStatusAndPadList (Buffer, BufferLength);

       //change the length to Big Endian format
       DataSubPacket->Length = ToBigLittleEndianDword (DataSubPacket->Length);
       Packet->Length = ToBigLittleEndianDword (Packet->Length);
       Compacket->Length = ToBigLittleEndianDword (Compacket->Length);
    }

    return EFI_SUCCESS;

}

/** @internal
    Builds GET Command block

    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.
    @param  TableObjUid       - UID of the table
    @param  CellBlock         - point to the row# and Column# from
                                where the data needs to be read.

    @retval EFI_STATUS
    
    @endinternal
**/

// TCG_Storage_Architechture Spec v2.01 Sec 5.3.3.6

EFI_STATUS
BuildGetCmdBlock (
    OPAL_DEVICE     *OpalDevice,
    VOID            *Buffer,
    UINTN           BufferLength,
    SESSION_PARAMS  *SessionParams,
    UINT64          TableObjUid,
    CELL_BLOCK      *CellBlock
)
{
    COM_PACKET_FORMAT       *Compacket = (COM_PACKET_FORMAT *)Buffer;
    PACKET_FORMAT           *Packet = (PACKET_FORMAT *)Compacket->PayLoad;
    DATA_SUBPACKET_FORMAT   *DataSubPacket = (DATA_SUBPACKET_FORMAT *)Packet->PayLoad;
    UINT64                  GetUid = GET_KEY_UID;
    UINT8                   String0[] = "Table";
    UINT8                   String1[] = "startRow";
    UINT8                   String2[] = "endRow";
    UINT8                   String3[] = "startColumn";
    UINT8                   String4[] = "endColumn";

    // Section 5.2.3.1 StartSession Method TCG Core Spec v2.01
    InitComPacket (Buffer, BufferLength, OpalDevice->OpalFeature.BaseComID, OpalDevice->ComIDExt);
    InitPacket (Buffer, BufferLength, SessionParams->Tsn, SessionParams->Hsn, 0);
    InitDataSubPacket (Buffer, BufferLength, 0);
    if (OpalDevice->OpalFeature.FeatureCode == ENTERPRISE_SSC_Feature){
        GetUid = GET_KEY_UID_ENTERPRISE;    
    }

    InsertTinyAtom (Buffer, BufferLength, CALL_TOKEN, UNSIGNED_DATA, TRUE); // Begin Method

    InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(TableObjUid), 8, UNSIGNED_DATA, BYTE_DATA);
    InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(GetUid), 8, UNSIGNED_DATA, BYTE_DATA);

    InsertTinyAtom (Buffer, BufferLength, START_LIST, UNSIGNED_DATA, TRUE); // Begin Parameter list
    
    //Cell Block Table 181
    InsertTinyAtom (Buffer, BufferLength, START_LIST, UNSIGNED_DATA, TRUE); // Begin Cell Block List
    
    if (CellBlock->TableUID != (UINTN)-1) {
        InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
        if (OpalDevice->OpalFeature.FeatureCode == ENTERPRISE_SSC_Feature) {
            InsertShortMediumLongAtom (Buffer, BufferLength, String0, 5, UNSIGNED_DATA, BYTE_DATA);
        } else {
            InsertTinyAtom (Buffer, BufferLength, 0, UNSIGNED_DATA, TRUE);  // Name value is 0
        }
        InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(CellBlock->TableUID), 8, UNSIGNED_DATA, BYTE_DATA);
        InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);
    }

    if (CellBlock->StartRow != (UINTN)-1) {
        InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
        if (OpalDevice->OpalFeature.FeatureCode == ENTERPRISE_SSC_Feature) {
            InsertShortMediumLongAtom (Buffer, BufferLength, String1, 8, UNSIGNED_DATA, BYTE_DATA);
        } else {
        InsertTinyAtom (Buffer, BufferLength, 1, UNSIGNED_DATA, TRUE);  // Name value is 1
        }
        if (CellBlock->RowisUid){
            InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(CellBlock->StartRow), 8, UNSIGNED_DATA, BYTE_DATA);
        } else {
            if (OpalDevice->OpalFeature.FeatureCode == ENTERPRISE_SSC_Feature) {
                InsertShortMediumLongAtom (Buffer, BufferLength, CellBlock->StartRowString, \
                        AsciiStrLen((const CHAR8 *)CellBlock->StartRowString), UNSIGNED_DATA, BYTE_DATA);
            } else {
               InsertTinyAtom (Buffer, BufferLength, (UINT8)(CellBlock->StartRow), UNSIGNED_DATA, FALSE);
            }
        }
        InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);

        if (!CellBlock->RowisUid) {
        // Valid only if StartRow is not a UID
            InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
            if (OpalDevice->OpalFeature.FeatureCode == ENTERPRISE_SSC_Feature) {
                InsertShortMediumLongAtom (Buffer, BufferLength, String2, 6, UNSIGNED_DATA, BYTE_DATA);
                InsertShortMediumLongAtom (Buffer, BufferLength, CellBlock->EndRowString, \
                        AsciiStrLen((const CHAR8 *)CellBlock->EndRowString), UNSIGNED_DATA, BYTE_DATA);
            } else {
                InsertTinyAtom (Buffer, BufferLength, 2, UNSIGNED_DATA, TRUE);  // Name value is 2
                InsertTinyAtom (Buffer, BufferLength, (UINT8)(CellBlock->EndRow), UNSIGNED_DATA, FALSE);
            }
            InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);
        }

    }

    if (CellBlock->StartColumn != (UINT8)-1) {
        if (OpalDevice->OpalFeature.FeatureCode == ENTERPRISE_SSC_Feature) {
            InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
            InsertShortMediumLongAtom (Buffer, BufferLength, String3, 11, UNSIGNED_DATA, BYTE_DATA);
            InsertShortMediumLongAtom (Buffer, BufferLength, CellBlock->StartColumnString, \
                    AsciiStrLen((const CHAR8 *)CellBlock->StartColumnString), UNSIGNED_DATA, BYTE_DATA);
            InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE); 
            InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE); 
            InsertShortMediumLongAtom (Buffer, BufferLength, String4, 9, UNSIGNED_DATA, BYTE_DATA);
            InsertShortMediumLongAtom (Buffer, BufferLength, CellBlock->EndColumnString, \
                    AsciiStrLen((const CHAR8 *)CellBlock->EndColumnString), UNSIGNED_DATA, BYTE_DATA);
            InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE); 
        } else {
            InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE); 
            InsertTinyAtom (Buffer, BufferLength, 3, UNSIGNED_DATA, TRUE);  // Start Column is 3
            InsertTinyAtom (Buffer, BufferLength, CellBlock->StartColumn, UNSIGNED_DATA, FALSE);
            InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);


            InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
            InsertTinyAtom (Buffer, BufferLength, 4, UNSIGNED_DATA, TRUE);  // End Column is 4
            InsertTinyAtom (Buffer, BufferLength, CellBlock->EndColumn, UNSIGNED_DATA, FALSE);
            InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);
        }
    }

    InsertTinyAtom (Buffer, BufferLength, END_LIST, UNSIGNED_DATA, TRUE); // End Cell Block  List

    InsertTinyAtom (Buffer, BufferLength, END_LIST, UNSIGNED_DATA, TRUE); // End Parameter List
    InsertTinyAtom (Buffer, BufferLength, EOD_TOKEN, UNSIGNED_DATA, TRUE); // End Method

    InsertMethodStatusAndPadList (Buffer, BufferLength);

    //change the length to Big Endian format
    DataSubPacket->Length = ToBigLittleEndianDword (DataSubPacket->Length);
    Packet->Length = ToBigLittleEndianDword (Packet->Length);
    Compacket->Length = ToBigLittleEndianDword (Compacket->Length);

    return EFI_SUCCESS;

}

/** @internal
    Builds End Session Command block

    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.

    @retval EFI_STATUS
    
    @endinternal
**/

// A basic cmd block with End of session Token inserted.

EFI_STATUS
BuildEndOfSessionCmdBlock (
    OPAL_DEVICE     *OpalDevice,
    VOID            *Buffer,
    UINTN           BufferLength,
    SESSION_PARAMS  *SessionParams
)
{

    COM_PACKET_FORMAT       *Compacket = (COM_PACKET_FORMAT *)Buffer;
    PACKET_FORMAT           *Packet = (PACKET_FORMAT *)Compacket->PayLoad;
    DATA_SUBPACKET_FORMAT   *DataSubPacket = (DATA_SUBPACKET_FORMAT *)Packet->PayLoad;
    UINT8                   Index;
    UINT8                   PadLength;


// Section 5.2.3.1 StartSession Method TCG Core Spec v2.01
    InitComPacket (Buffer, BufferLength, OpalDevice->OpalFeature.BaseComID, OpalDevice->ComIDExt);
    InitPacket (Buffer, BufferLength, SessionParams->Tsn, SessionParams->Hsn, 0);
    InitDataSubPacket (Buffer, BufferLength, 0);

    InsertTinyAtom (Buffer, BufferLength, EOS_TOKEN, UNSIGNED_DATA, TRUE);                   // EOS

    //Add Pad
    PadLength = DataSubPacket->Length % 4;
    if (PadLength){
        PadLength = 4 - PadLength;
    }
    for (Index = 0; Index < PadLength; Index ++){
        DataSubPacket->PayLoad[DataSubPacket->Length + Index] = 0;
    }

    Packet->Length += PadLength;
    Compacket->Length += PadLength;

    //change the length to Big Endian format
    DataSubPacket->Length = ToBigLittleEndianDword (DataSubPacket->Length);
    Packet->Length = ToBigLittleEndianDword (Packet->Length);
    Compacket->Length = ToBigLittleEndianDword (Compacket->Length);
    
    return EFI_SUCCESS;
}

/** @internal
    Routine get the length of the Token section (i.e. List token, Name token etc )

    @param  DataPtr  -   Pointer to list buffer
    @param  ListPtrSize     - List length.
    @param  Tag             - Tag to which legth has to be found
    @param  SectionLength   - Pointer to the length of the token section found
        
    @retval EFI_STATUS
    
    @endinternal
**/

EFI_STATUS
FindSectionLength (
    UINT8       *DataPtr, 
    UINTN       Length, 
    UINT8       Tag, 
    UINT32      *SectionLength)
{
    UINTN       Index;
    UINT32      SecLength = 0, DataPtrLength = 0;
    UINT8       HeaderSize = 0;
    UINT32      LocalSectionLength = 1;
    EFI_STATUS  Status;
    
    // First byte should be valid tag
    if ((DataPtr[0] >= 0xF0 && DataPtr[0] <= 0xFF) \
                && ((DataPtr[0] & 1) == 1)){
        return EFI_NOT_FOUND;
    }
    
    for (Index = 1; Index < Length;) {
        if (DataPtr[Index] == Tag+1) {
            (*SectionLength)++;
            return EFI_SUCCESS; 
        }

        if ((DataPtr[Index] >= 0xF0 && DataPtr[Index] <= 0xFF) \
                    && ((DataPtr[Index] & 1) == 0)) {

            // Only in case of start of the tag. BIT0 is zero (0xF0. 0xF2 etc)
            // List value found inside the Token List buffer, Find length the new list buffer
            // recursively
            Status = FindSectionLength(DataPtr + Index, Length - Index, (DataPtr[Index]), &LocalSectionLength);
            TCG_STORAGE_ERR_CHECK(Status);
            Index += LocalSectionLength;
            (*SectionLength)+=LocalSectionLength;
        } else {
            DataPtrLength = 0;
            Status = DecodeTokenHeader (DataPtr + Index, NULL, &DataPtrLength, &SecLength, &HeaderSize);
            if (Status != EFI_BUFFER_TOO_SMALL) return Status;
            Index = Index + SecLength + HeaderSize;
            (*SectionLength) += (SecLength + HeaderSize);
        }
    }

    return  EFI_NOT_FOUND;
}

/** @internal
    Search for a token from a List

    @param  ListPtr  - Pointer to list buffer
    @param  ListPtrSize     - List length.
    @param  TokenName       - Token name to search
    @param  Offset          - Token offset

    @retval EFI_STATUS
    
    @endinternal
**/

// For example look at Table 10 in SSC-Opal spec Data Payload
/*
For Enterprise SSC
F0
F0 ---> ListPtr pointing here on Input
F0
F2
AF 
52 65 61 64 4C 6F 63 6B 45 6E 61 62 6C 65 64 ReadLockEnabled
00 
F3
F2
D0 10 
57 72 69 74 65 4C 6F 63 6B 45 6E 61 62 6C 65 64 WriteLockEnabled
00 
F3
F1
F1
F1

For Opal
F0 
F0 ---> ListPtr pointing here on Input
F2
5   --> ReadLockEnabled
00 
F3
F2
6  -- > WriteLockEnabled
00 
F3
F1
F1

*/

EFI_STATUS
SearchNameTokenInList (
    OPAL_DEVICE     *OpalDevice,
    UINT8           *ListPtr,
    UINTN           ListPtrSize,
    CELL_BLOCK      *CellBlock,
    UINTN           *Offset
)
{

    EFI_STATUS       Status;
    UINT32           Index, Length = 0, DataPtrLength = 0;
    UINT8           HeaderSize = 0;
    UINT32          SectionLength;
    UINT8           EHeaderSize = 0;
    UINT32          ELength = 0; // Used for Enterprise
    
    *Offset = 0;

    Index = IS_ENTERPRISE_DEVICE(OpalDevice) ? 2 : 1;

    for ( ; Index < ListPtrSize; Index++){ // Start from 1 which will point to Start name token
        DEBUG ((DEBUG_VERBOSE, "offset %x Byte0  %x  Byte1 %x TokenName %a ListPtr : %lx\n",\
                Index, ListPtr[Index], ListPtr[Index+1], CellBlock->StartColumnString, ListPtr));
        if ((OpalDevice->OpalFeature.FeatureCode != ENTERPRISE_SSC_Feature && \
            (ListPtr[Index] == START_NAME) && (ListPtr[Index+1] == CellBlock->StartColumn)) || \
            (IS_ENTERPRISE_DEVICE(OpalDevice) && (ListPtr[Index] == START_NAME) && !CmpName(ListPtr+Index+1, CellBlock->StartColumnString))
            ){

            if (IS_ENTERPRISE_DEVICE(OpalDevice)) {
                Status = DecodeTokenHeader (ListPtr + Index + 1, NULL, &DataPtrLength, &Length, &HeaderSize); 
                TCG_STORAGE_DEVICE_ERR_CHECK(Status)
                *Offset = Index + Length + HeaderSize;
            }
            else {
                *Offset = Index + 1;
            }
            DEBUG ((DEBUG_VERBOSE, "offset %x\n", *Offset));
            return EFI_SUCCESS;
        }
        
        if ((ListPtr[Index + 2] >= 0xF0) && (ListPtr[Index + 2] <= 0xFF)) {
            // Example: F2 09 F0 00 03 F1 F3
            // Data is not found instead reserved token types are present. Skip those.
            // Find th# of bytes in this START_NAME/END_NAME section
            // F0 00 03 F1
            SectionLength = 1;
            Status = FindSectionLength(ListPtr+Index+2, ListPtrSize - Index - 1, ListPtr[Index + 2], &SectionLength); // Section length will be 4

            DEBUG ((DEBUG_VERBOSE, "SearchNameTokenInList()->FindSectionLength...SectionLength-%x Status-%r", SectionLength, Status ));
            if (EFI_ERROR(Status) || SectionLength == 0) {
                return EFI_DEVICE_ERROR;
            }
            Index = Index + SectionLength + 2; // 2 is for Start name and Cell # 
            continue;
        }
        
        if (IS_ENTERPRISE_DEVICE(OpalDevice)) {
            // Get String Length
            Status = DecodeTokenHeader (ListPtr + Index + 1, NULL, &DataPtrLength, &Length, &HeaderSize);    
            TCG_STORAGE_DEVICE_ERR_CHECK(Status)
            DEBUG ((DEBUG_VERBOSE, "Length %x HeaderSize %x", Length, HeaderSize));
            //Get data Length
            ELength = EHeaderSize = 0;
            Status = DecodeTokenHeader (ListPtr + Index + 1 + Length + HeaderSize, NULL, &DataPtrLength, &ELength, &EHeaderSize);
            TCG_STORAGE_DEVICE_ERR_CHECK(Status)
            DEBUG ((DEBUG_VERBOSE, "ELength %x EHeaderSize %x", ELength, EHeaderSize));
            //Check for END_NAME
            if (*(ListPtr + Index + Length + HeaderSize + ELength + EHeaderSize +1) != END_NAME) {
                return EFI_DEVICE_ERROR;
            }
            Index += (Length + HeaderSize + ELength + EHeaderSize + 1);
        } else {
            Status = DecodeTokenHeader (ListPtr + Index + 2, NULL, &DataPtrLength, &Length, &HeaderSize);
            TCG_STORAGE_DEVICE_ERR_CHECK(Status)

            //Check for END_NAME
            if (*(ListPtr + Index + Length + HeaderSize + 2 ) != END_NAME) {
                return EFI_DEVICE_ERROR;
            }
            Index += (Length + HeaderSize + 2);
        }
    }

    return EFI_DEVICE_ERROR;

}

/** @internal
    Searches for the given column # in the response buffer
    and returns the data.

    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.
    @param  ColumnNumber      - column # to search
    @param  ValueLength       - pointer to search data length
    @param  Value             - value of the search data if found.

    @retval Value
    @retval EFI_STATUS
    
    @endinternal
**/

EFI_STATUS
GetColumnValue (
    OPAL_DEVICE     *OpalDevice,
    VOID            *Buffer,
    UINTN           BufferLength,
    CELL_BLOCK      *CellBlock,
    UINT32          *ValueLength,
    UINT8           *Value

)
{

    EFI_STATUS          Status = EFI_DEVICE_ERROR;
    COM_PACKET_FORMAT   *ComPacket = (COM_PACKET_FORMAT *) Buffer;
    PACKET_FORMAT       *Packet = (PACKET_FORMAT *)ComPacket->PayLoad;
    DATA_SUBPACKET_FORMAT *DataSubPacket = (DATA_SUBPACKET_FORMAT *)Packet->PayLoad;
    UINT8               *ListPtr;
    UINTN               ListPtrSize, Offset;
    UINT8               HeaderSize;
    UINT32              Length;
    UINT32              Index;
    UINT32              SectionLength;

    // Look for Name Token with value 3
    if ((DataSubPacket->PayLoad[0] == START_LIST) && (DataSubPacket->PayLoad[1] == START_LIST)) {
        ListPtr = &(DataSubPacket->PayLoad[1]);
        ListPtrSize = ToBigLittleEndianDword(DataSubPacket->Length) - 1;
        Status = SearchNameTokenInList (OpalDevice, ListPtr, ListPtrSize, CellBlock, &Offset);
        if (!EFI_ERROR(Status)) {

            // Check if the data is a TAG or not
            if ((ListPtr[Offset + 1] >= 0xF0) && (ListPtr[Offset + 1] <= 0xFF)) {
                SectionLength = 1;
                Status = FindSectionLength(ListPtr+Offset+1, ListPtrSize, ListPtr[Offset + 1], &SectionLength);

                DEBUG ((DEBUG_VERBOSE, "GetColumnValue()->FindSectionLength...DataLength-%x Status-%r", SectionLength, Status ));
                if (!EFI_ERROR(Status) && (*ValueLength >= SectionLength)) {
                    *ValueLength = SectionLength;
                    CopyMem(Value, ListPtr+Offset+1, SectionLength);
                    return Status;
                }
                if (!EFI_ERROR(Status) && (*ValueLength  < SectionLength)) {
                    *ValueLength = SectionLength;
                    return EFI_BUFFER_TOO_SMALL;
                }
                return Status;
            }

            Status = DecodeTokenHeader (ListPtr + Offset + 1, Value,(UINT32 *)ValueLength, &Length, &HeaderSize);
            TCG_STORAGE_ERR_CHECK(Status);
            if (Value) {
                *ValueLength = Length;
                DEBUG ((DEBUG_VERBOSE, "Value : "));
                for (Index = 0; Index < *ValueLength; Index++) {
                    DEBUG ((DEBUG_VERBOSE, "%2X ", Value[Index]));
                }
                DEBUG ((DEBUG_VERBOSE, "\n"));
                return EFI_SUCCESS;
            }
        }

    }
    return Status;

}

/** @internal
    Decodes SYNC session data

    @param  SessionParams     - information on current session.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.

    @retval EFI_STATUS
    @note
    SPSessionID will become the TSN portion of the packet header for all future communication
    
    @endinternal
**/
// TCG_Storage_Architechture Spec v2.01 Sec 5.2.3.2
// SPSessionID will become the TSN portion of the packet header for all future communication
EFI_STATUS
DecodeSyncSessionData (
    VOID                *Buffer,
    UINTN               BufferLength,
    SESSION_PARAMS      *SessionParams
)
{

    EFI_STATUS          Status;
    COM_PACKET_FORMAT   *ComPacket = (COM_PACKET_FORMAT *) Buffer;
    PACKET_FORMAT       *Packet = (PACKET_FORMAT *)ComPacket->PayLoad;
    DATA_SUBPACKET_FORMAT *DataSubPacket = (DATA_SUBPACKET_FORMAT *)Packet->PayLoad;
    UINT32              Length = ToBigLittleEndianDword(DataSubPacket->Length);
    UINT8               *pMethodStatusList;
    UINT64              SMUid = SM_UID;
    UINT64              SyncSessionUid = SYNC_SESSION_UID;
    UINT8               *DataPtr;
    UINT8               HeaderSize = 0;
    UINT32              Index1, Index, DataPtrLength = 256;

    // Skip if there are any Empty tokens at the bottom
    for (Index=1; Index<Length; Index++){
        if (DataSubPacket->PayLoad[Length - Index] != 0xFF) break;
    }
    pMethodStatusList = &(DataSubPacket->PayLoad[Length - Index - 4]);

    Status = TcgSecAllocatePool(EfiBootServicesData, DataPtrLength, (VOID**)&DataPtr);
    TCG_STORAGE_ERR_CHECK(Status);

    // Validate the SM UID and SyncSssion Method ID in the received data. The offsets are calculated based on
    // a successful startsession cmd. Look at Application note doc for details
    if (CompareMem (&(DataSubPacket->PayLoad[SYNC_SESSION_SMUID_OFFSET]), &SMUid, 8)) {
        return EFI_DEVICE_ERROR;
    }

    if (CompareMem (&(DataSubPacket->PayLoad[SYNC_SESSION_SYNC_OFFSET]), &SyncSessionUid, 8)) {
        return EFI_DEVICE_ERROR;
    }

    //DEBUG ((DEBUG_VERBOSE, "SyncSession Status : %x\ %x %x \n", *(pMethodStatusList + 1), *(pMethodStatusList + 2), *(pMethodStatusList + 3)));

    // Is the status OK in the received Data
    if ((*pMethodStatusList == START_LIST) && \
            (*(pMethodStatusList + 4) == END_LIST) && \
            (*(pMethodStatusList + 1) == 0) && \
            (*(pMethodStatusList + 2) == 0) && \
            (*(pMethodStatusList + 3) == 0)){

        if (DataSubPacket->PayLoad[SYNC_SESSION_STARTLIST] != START_LIST) {
            return EFI_DEVICE_ERROR;
        }
        // Update SP Session ID. Add one to skip the START_LIST
        Status = DecodeTokenHeader(&(DataSubPacket->PayLoad[SYNC_SESSION_STARTLIST + 1]), DataPtr, &DataPtrLength, &Length, &HeaderSize);
        TCG_STORAGE_ERR_CHECK(Status);

        SessionParams->Hsn = 0;
        for (Index1 = 0; (Index1 < Length) && (Index1 < 4); Index1++){
            *((UINT8 *)&(SessionParams->Hsn) + Index1) = DataPtr[Index1];
        }

        // Get SP SessionID
        Status = DecodeTokenHeader(&(DataSubPacket->PayLoad[SYNC_SESSION_STARTLIST + 1 + Length + HeaderSize]), \
                DataPtr, &DataPtrLength, &Length, &HeaderSize);
        TCG_STORAGE_ERR_CHECK(Status);

        SessionParams->Tsn = 0;
        for (Index1 = 0; (Index1 < Length) && (Index1 < 4); Index1++){
            *((UINT8 *)&(SessionParams->Tsn) + Index1) = DataPtr[Index1];
        }

//        DEBUG ((DEBUG_VERBOSE, "SessionParams->Hsn :%8X\n", SessionParams->Hsn));
//        DEBUG ((DEBUG_VERBOSE, "SessionParams->Tsn :%8X\n", SessionParams->Tsn));

        TcgSecFreePool (DataPtr);
        return EFI_SUCCESS;

    }

    TcgSecFreePool (DataPtr);
    return EFI_DEVICE_ERROR;

}

/** @internal
    Builds Password Authentication Session for Enterprise SED drive.

    @param  OpalDevice        - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.
    @param  MethodUid         - Method ID of Authenticate Session
    @param  AuthUid           - Authority UID of Authentication Table
    @param  Challenge         - Authentication pin
    @param  ChallengeLength   - Length of the Authentication PIN

    @retval EFI_STATUS
    
    @endinternal
**/
EFI_STATUS
BuildSpObjectUidAuthenticate( 
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    VOID                *Buffer, 
    UINTN               BufferLength,
    UINT64              InvokingUid,
    UINT64              MethodUid,
    UINT64              AuthUid,
    UINT8               *Challenge,
    UINT8               ChallengeLength
)
{

    COM_PACKET_FORMAT       *Compacket = (COM_PACKET_FORMAT *)Buffer;
    PACKET_FORMAT           *Packet = (PACKET_FORMAT *)Compacket->PayLoad;
    DATA_SUBPACKET_FORMAT   *DataSubPacket = (DATA_SUBPACKET_FORMAT *)Packet->PayLoad;
    
    // Section 5.2.3.1 StartSession Method TCG Core Spec v2.01
    InitComPacket (Buffer, BufferLength, OpalDevice->OpalFeature.BaseComID, OpalDevice->ComIDExt);
    InitPacket (Buffer, BufferLength, SessionParams->Tsn, SessionParams->Hsn, 0);
    InitDataSubPacket (Buffer, BufferLength, 0);
    
    InsertTinyAtom (Buffer, BufferLength, CALL_TOKEN, UNSIGNED_DATA, TRUE);                   // Begin Method
    
        InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(InvokingUid), 8, UNSIGNED_DATA, BYTE_DATA);
        InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(MethodUid), 8, UNSIGNED_DATA, BYTE_DATA);  
    
        InsertTinyAtom (Buffer, BufferLength, START_LIST, UNSIGNED_DATA, TRUE);          // Begin List
            InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(AuthUid), 8, UNSIGNED_DATA, BYTE_DATA);  
            
            InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
                if (OpalDevice->OpalFeature.FeatureCode == ENTERPRISE_SSC_Feature){
                    InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)"Challenge", AsciiStrLen("Challenge"), UNSIGNED_DATA, BYTE_DATA);
                }
                else {
                    InsertTinyAtom (Buffer, BufferLength, 0, UNSIGNED_DATA, FALSE);        
                }
                ReverseAndInsertShortMediumLongAtom (Buffer, BufferLength, Challenge, ChallengeLength, UNSIGNED_DATA, BYTE_DATA);
            InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);
        
        InsertTinyAtom (Buffer, BufferLength, END_LIST, UNSIGNED_DATA, TRUE);                   // End List
        
    InsertTinyAtom (Buffer, BufferLength, EOD_TOKEN, UNSIGNED_DATA, TRUE);    // Ends Method
    InsertMethodStatusAndPadList (Buffer, BufferLength);
        
    //change the length to Big Endian format
    DataSubPacket->Length = ToBigLittleEndianDword (DataSubPacket->Length);
    Packet->Length = ToBigLittleEndianDword (Packet->Length);
    Compacket->Length = ToBigLittleEndianDword (Compacket->Length);
        
    return EFI_SUCCESS;  
    
}

/** @internal
    Builds REVERT Session Command block

    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.
    @param  SPUid             - UID of SP to revert
    @param  RevertUid         - Operation UID. [Revert/RevertSP]
    @param  KeepGlobalRangeKey - Enable to preserve User data on drive after revert

    @retval EFI_STATUS
    
    @endinternal
**/

EFI_STATUS
BuildSpObjectUidRevert (
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    VOID                *Buffer,
    UINTN               BufferLength,
    UINT64              SPUid,
    UINT64              RevertUid,
    UINT8               KeepGlobalRangeKey
)
{

    COM_PACKET_FORMAT       *Compacket = (COM_PACKET_FORMAT *)Buffer;
    PACKET_FORMAT           *Packet = (PACKET_FORMAT *)Compacket->PayLoad;
    DATA_SUBPACKET_FORMAT   *DataSubPacket = (DATA_SUBPACKET_FORMAT *)Packet->PayLoad;
    UINT32                   ParameterValue = KEEP_GLOBAL_RANGE_KEY_PARAMETER;

    // Section 5.2.3.1 StartSession Method TCG Core Spec v2.01
    InitComPacket (Buffer, BufferLength, OpalDevice->OpalFeature.BaseComID, OpalDevice->ComIDExt);
    InitPacket (Buffer, BufferLength, SessionParams->Tsn, SessionParams->Hsn, 0);
    InitDataSubPacket (Buffer, BufferLength, 0);

    InsertTinyAtom (Buffer, BufferLength, CALL_TOKEN, UNSIGNED_DATA, TRUE);                   // Begin Method

    InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(SPUid), 8, UNSIGNED_DATA, BYTE_DATA);
    InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&(RevertUid), 8, UNSIGNED_DATA, BYTE_DATA);

    InsertTinyAtom (Buffer, BufferLength, START_LIST, UNSIGNED_DATA, TRUE);                   // Begin List
    // Optional Parameter applies only for OPAL SSC V200
    if ((RevertUid == REVERTSP_UID) && ((OpalDevice->OpalFeature.FeatureCode == OPAL_SSC_V200_Feature) || \
            (OpalDevice->OpalFeature.FeatureCode == PYRITE_SSC_Feature_v2_0) || \
            (OpalDevice->OpalFeature.FeatureCode == RUBY_SSC_Feature) )) {
        InsertTinyAtom (Buffer, BufferLength, START_NAME, UNSIGNED_DATA, TRUE);
        if (IS_ENTERPRISE_DEVICE(OpalDevice)) {
            InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)"KeepGlobalRangeKey",  sizeof("KeepGlobalRangeKey"), UNSIGNED_DATA, BYTE_DATA);
        }
        else {
        InsertShortMediumLongAtom (Buffer, BufferLength, (UINT8 *)&ParameterValue,  4, UNSIGNED_DATA, INTEGER_DATA);
        }
        InsertShortMediumLongAtom (Buffer, BufferLength, &KeepGlobalRangeKey, 1, UNSIGNED_DATA, INTEGER_DATA);
        InsertTinyAtom (Buffer, BufferLength, END_NAME, UNSIGNED_DATA, TRUE);
    }
    InsertTinyAtom (Buffer, BufferLength, END_LIST, UNSIGNED_DATA, TRUE);                     // End List

    InsertTinyAtom (Buffer, BufferLength, EOD_TOKEN, UNSIGNED_DATA, TRUE);    // Ends Method
    InsertMethodStatusAndPadList (Buffer, BufferLength);

    //change the length to Big Endian format
    DataSubPacket->Length = ToBigLittleEndianDword (DataSubPacket->Length);
    Packet->Length = ToBigLittleEndianDword (Packet->Length);
    Compacket->Length = ToBigLittleEndianDword (Compacket->Length);

    return EFI_SUCCESS;

}

/** @internal
    Search data in properties table

    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.
    @param  String            - property name as string
    @param  Value             - value of the search property if found.

    @retval Value
    @retval EFI_STATUS
    
    @endinternal
**/


// Look at Table 3 in SSC Opal document to decode the properties table
EFI_STATUS
SearchProperties (
    VOID                *Buffer,
    UINTN               BufferLength,
    UINT8               *String,
    UINT32              *Value
)
{

    EFI_STATUS              Status;
    UINT32                  Index, DataPtrLength = 256, Length;
    UINT8                   *DataPtr;
    UINT8                   HeaderSize = 0;

    COM_PACKET_FORMAT       *Compacket = (COM_PACKET_FORMAT *)Buffer;
    PACKET_FORMAT           *Packet = (PACKET_FORMAT *)Compacket->PayLoad;
    DATA_SUBPACKET_FORMAT   *DataSubPacket = (DATA_SUBPACKET_FORMAT *)Packet->PayLoad;
    UINT32                   PayLoadLength = ToBigLittleEndianDword (DataSubPacket->Length);

    Status = TcgSecAllocatePool(EfiBootServicesData, DataPtrLength, (VOID**)&DataPtr);
    TCG_STORAGE_ERR_CHECK(Status);

    // Skip the SMUID and Involing ID and get to the first Name token
    for (Index = 21; Index < PayLoadLength;) {

        if (DataSubPacket->PayLoad[Index] != START_NAME) return EFI_NOT_FOUND;
        Index++;

        Status = DecodeTokenHeader(&(DataSubPacket->PayLoad[Index]), DataPtr, &DataPtrLength, &Length, &HeaderSize);
        TCG_STORAGE_ERR_CHECK(Status);

        /*
        //Convert the string
        for (Index1 = 0; Index1 < Length/2; Index1++){
            bTemp = DataPtr[Index1];
            DataPtr[Index1] = DataPtr[Length - Index1 -1];
            DataPtr[Length - Index1 -1] = bTemp;
        }*/

        DataPtr[Length] = 0; // So that strCmp can work. DataPtr is big enough.

        if (AsciiStrCmp((const CHAR8 *)DataPtr, String) == 0){
            ZeroMem(DataPtr, DataPtrLength);
            Index += (Length + HeaderSize);
            Status = DecodeTokenHeader(&(DataSubPacket->PayLoad[Index]), DataPtr, &DataPtrLength, &Length, &HeaderSize);
            TCG_STORAGE_ERR_CHECK(Status);
            *Value = *(UINT32 *)DataPtr;
            TcgSecFreePool(DataPtr);
            return EFI_SUCCESS;
        }
        Index += (Length + HeaderSize);

        Status = DecodeTokenHeader(&(DataSubPacket->PayLoad[Index]), DataPtr, &DataPtrLength, &Length, &HeaderSize);
        TCG_STORAGE_ERR_CHECK(Status);

        Index += (Length + HeaderSize);

        if (DataSubPacket->PayLoad[Index] != END_NAME) return EFI_NOT_FOUND;

        Index++; // Points to End of Name Token

        // If the end of List is reached exit
        if (DataSubPacket->PayLoad[Index] == END_LIST) {
            break;
        }
    }
    TcgSecFreePool(DataPtr);
    return  EFI_NOT_FOUND;
}

#if defined(TCG_STORAGE_SEC_VERBOSE_PRINT) && (TCG_STORAGE_SEC_VERBOSE_PRINT == 1)

/** @internal
    FUnction checks input CHAR is ASCCI or not.
    @param  Buffer            - Payload buffer

    @retval VOID
    
    @endinternal
**/
BOOLEAN
IsAscii (
    UINT8   Char
)
{
    
    if (Char >= '0' && Char <= '9') return TRUE;
    if (Char >= 'A' && Char <= 'Z') return TRUE;
    if (Char >= 'a' && Char <= 'z') return TRUE;
    
    return FALSE;
    
}

/** @internal
    FUnction prints OPAL Device UID string based on UID GUID.

    @param  Buffer            - Payload buffer

    @retval VOID
    
    @endinternal
**/
VOID
CheckForGuidString(
        UINT64      Guid)
{
    
    UINTN i;
    
    for (i = 0; i < sizeof(Uidtable)/sizeof(Uidtable[0]); i++){
        if (Uidtable[i].Uid == Guid){
            DEBUG ((DEBUG_VERBOSE, " **%a**", Uidtable[i].String));    
        }
    }
}

/** @internal
    Prints command block built for current session

    @param  Buffer            - Payload buffer

    @retval VOID
    
    @endinternal
**/

VOID
PrintCmdBlock (
    VOID            *Buffer
)
{

    EFI_STATUS          Status;
    COM_PACKET_FORMAT   *ComPacket = (COM_PACKET_FORMAT *) Buffer;
    PACKET_FORMAT       *Packet = (PACKET_FORMAT *)ComPacket->PayLoad;
    DATA_SUBPACKET_FORMAT *DataSubPacket = (DATA_SUBPACKET_FORMAT *)Packet->PayLoad;
    UINT32              Index,Index1,k;
    UINT32               Length = 0, DataPtrLength = 256;
    UINT8               *DataPtr, HeaderSize = 0;
    BOOLEAN             String = FALSE;

    DEBUG ((DEBUG_VERBOSE, "ComPacket (BigEndian) :\n"));
    DEBUG ((DEBUG_VERBOSE, "  ComID           : %2X\n", ComPacket->ComID));
    DEBUG ((DEBUG_VERBOSE, "  ComIDExtension  : %2X\n", ComPacket->ComIDExtension));
    DEBUG ((DEBUG_VERBOSE, "  OutStandingData : %4X\n", ComPacket->OutStandingData));
    DEBUG ((DEBUG_VERBOSE, "  MinTransfer     : %4X\n", ComPacket->MinTransfer));
    DEBUG ((DEBUG_VERBOSE, "  Length          : %4X\n", ComPacket->Length));

    DEBUG ((DEBUG_VERBOSE, "Packet (BigEndian) :\n"));
    DEBUG ((DEBUG_VERBOSE, "  TSN             :%4X\n", Packet->TSN));
    DEBUG ((DEBUG_VERBOSE, "  HSN             :%4X\n", Packet->HSN));
    DEBUG ((DEBUG_VERBOSE, "  SeqNumber       :%4X\n", Packet->SeqNumber));
    DEBUG ((DEBUG_VERBOSE, "  AckType         :%4X\n", Packet->AckType));
    DEBUG ((DEBUG_VERBOSE, "  Acknowledgement :%4X\n", Packet->Acknowledgement));
    DEBUG ((DEBUG_VERBOSE, "  Length          :%4X\n", Packet->Length));

    DEBUG ((DEBUG_VERBOSE, "DataSubPacket  (BigEndian) :\n"));
    DEBUG ((DEBUG_VERBOSE, "  Kind            :%2X\n", DataSubPacket->Kind));
    DEBUG ((DEBUG_VERBOSE, "  Length          :%4X\n", DataSubPacket->Length));

    DEBUG ((DEBUG_VERBOSE, "DATA  :\n"));

    Status = TcgSecAllocatePool(EfiBootServicesData, DataPtrLength, (VOID**)&DataPtr);

    for (Index = 0; Index < ToBigLittleEndianDword(DataSubPacket->Length); Index++) {

        if (DataSubPacket->PayLoad[Index] >= 0xF0 && DataSubPacket->PayLoad[Index] <= 0xFF) {
            DEBUG ((DEBUG_VERBOSE, "%2X\n", DataSubPacket->PayLoad[Index]));
            continue;
        }

        Status = DecodeTokenHeader (&(DataSubPacket->PayLoad[Index]), DataPtr, &DataPtrLength, &Length, &HeaderSize);

        if (Status == EFI_BUFFER_TOO_SMALL) {
            TcgSecFreePool (DataPtr);
            Status = TcgSecAllocatePool(EfiBootServicesData, DataPtrLength, (VOID**)&DataPtr);
            TCG_STORAGE_ERR_CHECK_VOID(Status)
            Status = DecodeTokenHeader (&(DataSubPacket->PayLoad[Index]), DataPtr, &DataPtrLength, &Length, &HeaderSize);
            TCG_STORAGE_ERR_CHECK_VOID(Status)
        }

        if (HeaderSize ==1) String = (DataSubPacket->PayLoad[Index] & 0x20) ? TRUE : FALSE;
        if (HeaderSize ==2) String = (DataSubPacket->PayLoad[Index] & 0x10) ? TRUE : FALSE;
        if (HeaderSize ==4) String = (DataSubPacket->PayLoad[Index] & 0x2) ? TRUE : FALSE;

        for (Index1=0;Index1<HeaderSize;Index1++) {
            DEBUG ((DEBUG_VERBOSE, "%2X ", DataSubPacket->PayLoad[Index]));
            Index++;
        }

        if (HeaderSize) {
            DEBUG ((DEBUG_VERBOSE, "\n"));
        }

        k = Index;
        for (Index1=0; Index1 <Length; Index1++) {
            DEBUG ((DEBUG_VERBOSE, "%2X ", DataSubPacket->PayLoad[Index]));
            Index++;
        }
        if (String) {
            for (Index1=0; Index1 <Length && IsAscii(DataSubPacket->PayLoad[k]); Index1++){
                DEBUG ((DEBUG_VERBOSE, "%c", DataSubPacket->PayLoad[k]));
                k++;
            }
        }
        
        if (String && (HeaderSize ==  1) && (Length == 8)) { 
            CheckForGuidString(*(UINT64 *)&DataSubPacket->PayLoad[k]);
        }

        if (Length) {
            DEBUG ((DEBUG_VERBOSE, "\n"));
        }
        if (HeaderSize || Length) {
            Index -= 1; // For loop increases Index by 1
        }
    }

    if (DataPtr) {
        TcgSecFreePool (DataPtr);
    }
    return;

}

#endif

/** @internal
    Wrapper function for SP REVERT

    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.
    @param  SPUid             - UID of SP to revert
    @param  RevertUid         - Operation UID. [Revert/RevertSP]
    @param  KeepGlobalRangeKey - Enable to preserve User data on drive after revert

    @retval EFI_STATUS
    
    @endinternal
**/

// Refer to TCG Storage Opal SSC spec Section 5.1.2 and 5.1.3
// KeepGlobalRangeKey used with Locking SP allows to keep the same encryption key.
// KeepGlobalRangeKey is valid only when SPUid is equal to REVERTSP_UID

EFI_STATUS
SpObjectUidRevert (
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    VOID                *Buffer,
    UINTN               BufferLength,
    UINT64              SPUid,
    UINT64              RevertUid,
    UINT8               KeepGlobalRangeKey
)
{

    EFI_STATUS          Status;
    UINT16              SPSpecificID = ToBigLittleEndianWord(OpalDevice->OpalFeature.BaseComID);

    DEBUG ((DEBUG_VERBOSE, "\nSpObjectUidRevert  : %lx\n", Buffer));
    Status = BuildSpObjectUidRevert( OpalDevice, SessionParams, Buffer, BufferLength, SPUid, RevertUid, KeepGlobalRangeKey);
    TCG_STORAGE_ERR_CHECK(Status);

    Status = ExecuteSendTcgCmd(OpalDevice, SECURITY_PROTOCOL_1, SPSpecificID, Buffer, BufferLength);
    DEBUG ((DEBUG_VERBOSE, "\nSpObjectUidRevert  Status : %r\n", Status));

    return Status;

}

/** @internal
    Prepares and Starts a Session. Builds command block.

    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.

    @retval EFI_STATUS
    
    @endinternal
**/

// TCG_Storage_Architechture Spec v2.01 Section 5.2.3.1
// Both Mandatory and optional parameters are provided through SESSION_PARAMS
// Response to Start Session is SyncSession
EFI_STATUS
StartSession (
    OPAL_DEVICE     *OpalDevice,
    VOID            *Buffer,
    UINTN           BufferLength,
    SESSION_PARAMS  *SessionParams
)
{

    EFI_STATUS          Status;
    UINT16              SPSpecificID = ToBigLittleEndianWord(OpalDevice->OpalFeature.BaseComID);
    DEBUG ((DEBUG_VERBOSE, "\nStartSession Buffer : %lx\n", Buffer));
    Status = BuildStartSessionCmdBlock( OpalDevice, Buffer, BufferLength, SessionParams);
    TCG_STORAGE_ERR_CHECK(Status);

    Status = ExecuteSendTcgCmd(OpalDevice, SECURITY_PROTOCOL_1, SPSpecificID, Buffer, BufferLength);
    DEBUG ((DEBUG_VERBOSE, "StartSession Status : %r\n", Status));
    return Status;

}

/** @internal
    Prepares and Ends a Session.

    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.

    @retval EFI_STATUS
    
    @endinternal
**/

EFI_STATUS
EndofSession (
    OPAL_DEVICE         *OpalDevice,
    VOID                *Buffer,
    UINTN               BufferLength,
    SESSION_PARAMS      *SessionParams
)
{

    EFI_STATUS          Status;
    UINT16              SPSpecificID = ToBigLittleEndianWord(OpalDevice->OpalFeature.BaseComID);

    DEBUG ((DEBUG_VERBOSE, "\nEndofSession Buffer : %lx\n", Buffer));
    Status = BuildEndOfSessionCmdBlock( OpalDevice, Buffer, BufferLength, SessionParams);
    TCG_STORAGE_ERR_CHECK (Status);

    Status = ExecuteSendTcgCmd(OpalDevice, SECURITY_PROTOCOL_1, SPSpecificID, Buffer, BufferLength);
    DEBUG ((DEBUG_VERBOSE, "EndofSession Status : %r\n", Status));
    return Status;

}

/** @internal
    SynSession  - This is in response to StartSession

    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.

    @retval EFI_STATUS
    
    @endinternal
**/

// TCG_Storage_Architechture Spec v2.01 Section 5.2.3.2

EFI_STATUS
SyncSession (
    OPAL_DEVICE         *OpalDevice,
    VOID                *Buffer,
    UINTN               BufferLength,
    SESSION_PARAMS      *SessionParams
)
{

    EFI_STATUS          Status;
    UINT8               bData = 0xFF;
    UINT16              SPSpecificID = ToBigLittleEndianWord(OpalDevice->OpalFeature.BaseComID);
    DEBUG ((DEBUG_VERBOSE, "\nSyncSession Buffer : %lx\n", Buffer));
    Status = OpalDevice->StorageSecurityProtocol->ReceiveData(
                                OpalDevice->StorageSecurityProtocol,
                                OpalDevice->MediaId,
                                OpalDevice->BusInterfaceType == AmiStorageInterfaceNvme ? \
                                        NVME_COMMAND_TIMEOUT * 10000000 : SATA_COMMAND_TIMEOUT * 10000000,
                                SECURITY_PROTOCOL_1,   // Security Protocol ID
                                SPSpecificID,
                                0x200,
                                Buffer,
                                &BufferLength
                                );

    DEBUG ((DEBUG_VERBOSE, "SyncSession Status : %r\n", Status));
    if (EFI_ERROR(Status)){
        return Status;
    }
#if defined(TCG_STORAGE_SEC_VERBOSE_PRINT) && (TCG_STORAGE_SEC_VERBOSE_PRINT == 1)
    PrintCmdBlock (Buffer);
#endif
    if (!EFI_ERROR(Status)) {
        Status = GetStatusValue (Buffer, BufferLength, &bData);
        TCG_STORAGE_ERR_CHECK(Status);
    }
    Status = DecodeSyncSessionData (Buffer, BufferLength, SessionParams);

    return Status;

}

/** @internal
    Starts a SM_UID Session.

    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.

    @retval EFI_STATUS
    
    @endinternal
**/

// SessionParams  has been updated with InvokingID, MethodID, HostSession# and SPID
// There are mandatory and optional items in SESSION_PARAMS that needs to be filled based on the session that
// is getting started.
// On successful start session SessionParams->Tsn will get updated.
EFI_STATUS
SMUidStartSession (
    OPAL_DEVICE     *OpalDevice,
    SESSION_PARAMS  *SessionParams,
    VOID            *Buffer,
    UINTN           BufferLength
)
{
    EFI_STATUS      Status;

    ZeroMem(Buffer, BufferLength);

    Status = StartSession (OpalDevice, Buffer, BufferLength,SessionParams);
    TCG_STORAGE_ERR_CHECK(Status);

    ZeroMem(Buffer, BufferLength);
    Status = GetResponse (OpalDevice, Buffer, BufferLength, SessionParams);
    TCG_STORAGE_ERR_CHECK(Status);

    // Section 5.2.3.2 Core Spec
    Status = DecodeSyncSessionData (Buffer, BufferLength, SessionParams);
    TCG_STORAGE_ERR_CHECK(Status);

    return Status;

}

/** @internal
    Ends a SM_UID Session.

    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.

    @retval EFI_STATUS
    
    @endinternal
**/

EFI_STATUS
SMUidCloseSession (
    OPAL_DEVICE     *OpalDevice,
    SESSION_PARAMS  *SessionParams,
    VOID            *Buffer,
    UINTN           BufferLength
)
{
    EFI_STATUS      Status;

    if (SessionParams->Tsn == 0) {
        ZeroMem(SessionParams, sizeof (SESSION_PARAMS));
        return EFI_SUCCESS;         // Session is already closed
    }

    ZeroMem(Buffer, BufferLength);
    Status = EndofSession (OpalDevice, Buffer, BufferLength, SessionParams);
    TCG_STORAGE_ERR_CHECK(Status);

    ZeroMem(Buffer, BufferLength);
    Status = GetResponse (OpalDevice, Buffer, BufferLength, SessionParams);
    TCG_STORAGE_ERR_CHECK(Status);

    ZeroMem(SessionParams, sizeof (SESSION_PARAMS));

    return EFI_SUCCESS;

}

/** @internal
    SetBooleanExpression session Wrapper function

    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.
    @param  TableObjUid       - UID of the table
    @param  Uid1              - User Authority UID 1
    @param  Uid2              - User Authority UID 2
    @param  BooleanOperator   - Boolean value to be set [0 or 1]

    @retval EFI_STATUS
    
    @endinternal
**/
// Table 177 in CORE spec ACE Table Description
EFI_STATUS
ObjectUidSetBooleanExpression (
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    VOID                *Buffer,
    UINTN               BufferLength,
    UINT64              TableObjUid,
    UINT64              Uid1,
    UINT64              Uid2,
    BOOLEAN             BooleanOperator
)
{

    EFI_STATUS          Status;
    UINT16              SPSpecificID = ToBigLittleEndianWord(OpalDevice->OpalFeature.BaseComID);
    DEBUG ((DEBUG_VERBOSE, "\nObjectUidSetBooleanExpression  : %lx\n", Buffer));
    Status = BuildSetBooleanExpressionCmdBlock (OpalDevice, SessionParams, Buffer, BufferLength, TableObjUid, Uid1, Uid2, BooleanOperator);
    TCG_STORAGE_ERR_CHECK(Status);

    Status = ExecuteSendTcgCmd(OpalDevice, SECURITY_PROTOCOL_1, SPSpecificID, Buffer, BufferLength);
    DEBUG ((DEBUG_VERBOSE, "\nObjectUidSetBooleanExpression  Status : %r\n", Status));
    return Status;

}

/** @internal
    Function to authenticate Password for Enterprise SED drive.

    @param  OpalDevice        - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.
    @param  AuthObj           - Authority UID of Authentication Table
    @param  Challenge         - Authentication pin
    @param  ChallengeLength   - Length of the Authentication PIN

    @retval EFI_STATUS
    
    @endinternal
**/
EFI_STATUS
SpObjectUidAuthenticate (
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    VOID                *Buffer,
    UINTN               BufferLength,
    UINT64              AuthObj,
    UINT8               *Challenge,
    UINT32              ChallengeLength
)
{
    
    EFI_STATUS          Status;
    UINT16              SPSpecificID = ToBigLittleEndianWord(OpalDevice->OpalFeature.BaseComID);
    UINT64              AuthUid = (OpalDevice->OpalFeature.FeatureCode != ENTERPRISE_SSC_Feature) ? AUTH_UID : AUTH_UID_ENTERPRISE;
    
    DEBUG ((DEBUG_VERBOSE, "\nSpObjectUidAuthenticate  : %lx\n", Buffer));
    
    Status = BuildSpObjectUidAuthenticate( OpalDevice, SessionParams, Buffer, BufferLength, THISSP_UID, AuthUid, AuthObj, Challenge, (UINT8)ChallengeLength);
    TCG_STORAGE_ERR_CHECK(Status);
    
    Status = ExecuteSendTcgCmd(OpalDevice, SECURITY_PROTOCOL_1, SPSpecificID, Buffer, BufferLength);
    
    DEBUG ((DEBUG_VERBOSE, "\nSpObjectUidAuthenticate  Status : %r\n", Status));
    
    return Status;
    
}

/** @internal
    Function to authenticate Password for Enterprise SED drive.
    First call is to open the session using AuthObj1 and the second call is to Authenticate on AuthObj2.

    @param  OpalDevice        - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.
    @param  AuthObj1          - SP UID for open session
    @param  AuthObj2          - Authority table ID to which Authentication is done
    @param  Pwd               - Authentication pin
    @param  PwdLength         - Length of the Authentication PIN

    @retval EFI_STATUS
    
    @endinternal
**/
EFI_STATUS
OpenSessionAndAuthenticate (
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    VOID                *Buffer,
    UINTN               BufferLength,
    UINT64              AuthObj1,
    UINT64              AuthObj2,
    UINT8               *Pwd,
    UINT32              PwdLength
)
{

    EFI_STATUS      Status;
//# AptioV_Override_START: EIP#642147 Address Security Advisory SA50093
    ZeroMem(Buffer, BufferLength);
    ZeroMem(SessionParams, sizeof (SESSION_PARAMS));
//# AptioV_Override_END: EIP#642147 Address Security Advisory SA50093
    // Open session to Admin SP as the SID authority using the MSID password
    SessionParams->InvokingID = SM_UID;
    SessionParams->MethodID = START_SESSION_UID;
    SessionParams->HostSessionID = 1;
    SessionParams->SPID = AuthObj1;
    SessionParams->Write = 1;
    
    Status = SMUidStartSession (OpalDevice, SessionParams, Buffer, BufferLength);
    TCG_STORAGE_ERR_CHECK(Status);
//# AptioV_Override_START: EIP#642147 Address Security Advisory SA50093	
    ZeroMem(Buffer, BufferLength);
//# AptioV_Override_END: EIP#642147 Address Security Advisory SA50093
    // Authenticate only if pwd buffer is valid
    if (Pwd) {
        Status = SpObjectUidAuthenticate (OpalDevice, SessionParams, Buffer, BufferLength, AuthObj2, Pwd, PwdLength);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
//# AptioV_Override_START: EIP#642147 Address Security Advisory SA50093
        ZeroMem(Buffer, BufferLength);
//# AptioV_Override_END: EIP#642147 Address Security Advisory SA50093
        Status = GetResponse (OpalDevice, Buffer, BufferLength, SessionParams);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
    }
    return EFI_SUCCESS;
    
CloseSession:
    SMUidCloseSession (OpalDevice, SessionParams, Buffer, BufferLength);
    return Status;

}

/** @internal
    SET command session - Wrapper function

    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.
    @param  TableObjUid       - UID of the table
    @param  Column            - Column # to set
    @param  pByte             - Value to set
    @param  ByteLngth         - Value length
    @param  ByteInteger       - byte data or integer data
    @param  EncodingType      - Encoding type of the data	

    @retval EFI_STATUS
    
    @endinternal
**/
// TCG_Storage_Architechture Spec v2.01 Sec 5.3.3.7
// It is used to change the value in the selected table and column
EFI_STATUS
ObjectUidSet (
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    VOID                *Buffer,
    UINTN               BufferLength,
    UINT64              TableObjUid,
    UINT8               Column,
    CHAR8               *ColumnName,
    UINT8               *pByte,
    UINTN               ByteLength,
    BOOLEAN             ByteInteger,
    COLUMN_ENCODING_TYPES EncodingType
)
{

    EFI_STATUS          Status;
    UINT16              SPSpecificID = ToBigLittleEndianWord(OpalDevice->OpalFeature.BaseComID);
    DEBUG ((DEBUG_VERBOSE, "\n ObjectUidSet  : %lx\n", Buffer));
    Status = BuildSetCmdBlock (OpalDevice, SessionParams, Buffer, BufferLength, TableObjUid, 
                      Column, ColumnName, pByte, ByteLength, ByteInteger, EncodingType, TRUE, TRUE);
    TCG_STORAGE_ERR_CHECK(Status);

    Status = ExecuteSendTcgCmd(OpalDevice, SECURITY_PROTOCOL_1, SPSpecificID, Buffer, BufferLength);
    DEBUG ((DEBUG_VERBOSE, "\n ObjectUidSet  Status : %r\n", Status));
    return Status;

}


/** @internal
    Sends BlockSID command - Wrapper function

    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.

    @retval EFI_STATUS
    @note
    If the Storage device ownership is not taken, to avoid malicious software
    taking control of the device in OS.EnableBlockSID should be called in
    readyToBoot event callback function.
    
    @endinternal
**/

EFI_STATUS
EnableBlockSID(
    OPAL_DEVICE         *OpalDevice,
    VOID                *Buffer,
    UINTN               BufferLength
)
{

    EFI_STATUS          Status;

    if (OpalDevice->BlockSid.FeatureCode != Block_SID_Authentification_Feature) {
        DEBUG((DEBUG_VERBOSE, "Block SID feature not supported by the drive\n"));
        return EFI_NOT_FOUND;
    }

    if (OpalDevice->BlockSid.SIDStateValue == 1) {
        DEBUG((DEBUG_VERBOSE, "C_PIN_SID is not equal to C_PIN_MISD\n"));
        return EFI_NOT_FOUND;
    }

    if (OpalDevice->BlockSid.SIDBlockedState == 1) {
        DEBUG((DEBUG_VERBOSE, "SID already blocked\n"));
        return EFI_NOT_FOUND;
    }

    ZeroMem(Buffer, BufferLength);
    ((UINT8 *)Buffer)[0] = 1;
    Status = ExecuteSendTcgCmd( OpalDevice,
                                SECURITY_PROTOCOL_2,
                                ToBigLittleEndianWord(BLOCK_SID_COMID),
                                Buffer,
                                BufferLength);
    DEBUG((DEBUG_VERBOSE, "SID Block cmd Status : %r \n", Status ));
    return Status;

}
