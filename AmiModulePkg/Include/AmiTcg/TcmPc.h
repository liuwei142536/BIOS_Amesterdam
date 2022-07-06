//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2010, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**        5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093         **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//*************************************************************************
// $Header: /Alaska/SOURCE/Modules/TCG/TcmPc.h 2     3/29/11 12:27p Fredericko $
//
// $Revision: 2 $
//
// $Date: 3/29/11 12:27p $
//*************************************************************************
//<AMI_FHDR_START>
//
// Name:  TcmPc.h
//
// Description:
//       Header file for Tcm structures
//
//<AMI_FHDR_END>
//*************************************************************************
#ifndef _TCMPC_H_
#define _TCMPC_H_
#include "AmiTcg/TcgEFI12.h"
#include <AmiTcg/Tcm.h>


#pragma pack (push)
#pragma pack (1)



typedef struct tdTCM_PCR_EVENT
{
    UINT32            PCRIndex;  // PCRIndex event extended to
    UINT32            EventType; // TCG EFI event type
    TCM_DIGEST        Digest;    // Value extended into PCRIndex
    UINT32            EventSize; // Size of the event data
    UINT8             Event[1];  // The event data
} TCM_PCR_EVENT;


typedef struct _EFI_TCM_EV_S_CRTM_VERSION
{
    EFI_GUID                          VersionGuid;
}EFI_TCM_EV_S_CRTM_VERSION;


typedef struct _EFI_TCM_EV_POST_CODE
{
    EFI_PHYSICAL_ADDRESS              PostCodeAddress;
    UINT64                            PostCodeLength;
}EFI_TCM_EV_POST_CODE;


typedef struct _EFI_TCM_EV_TAG_OPTION_ROM_EXECUTE
{
    UINT16     Reserved;
    UINT16     PFA;
    TCM_DIGEST Hash;
} EFI_TCM_EV_TAG_OPTION_ROM_EXECUTE;


typedef struct _EFI_TCM_EV_CPU_MICROCODE
{
    EFI_PHYSICAL_ADDRESS MicrocodeEntrypoint;
}EFI_TCM_EV_CPU_MICROCODE;


typedef union _EFI_TCM_EV_TAG_EVENT_DATA
{
    struct _EFI_TCM_EV_TAG_OPTION_ROM_EXECUTE
        OptionRomExecute;
}EFI_TCM_EV_TAG_EVENT_DATA;


typedef struct _EFI_TCM_EV_TAG
{
    UINT32 EventID;
    UINT32 EventSize;
    union _EFI_TCM_EV_TAG_EVENT_DATA EventData;
}EFI_TCM_EV_TAG;


typedef struct _EFI_TCM_PCR_EVENT_HEADER
{
    TPM_PCRINDEX PCRIndex;
    UINT32       EventType;
    TCM_DIGEST   Digest;
    UINT32       EventDataSize;
} EFI_TCM_PCR_EVENT_HEADER;


typedef union _EFI_TCM_PCR_EVENT_DATA
{
    struct _EFI_TCM_EV_S_CRTM_VERSION SCrtmVersion;
    struct _EFI_TCM_EV_POST_CODE PostCode;
    struct _EFI_TCM_EV_TAG Tagged;
    struct _EFI_TCM_EV_CPU_MICROCODE Mcu;
} EFI_TCM_PCR_EVENT_DATA;



typedef struct _EFI_TCM_PCR_EVENT
{
    EFI_TCM_PCR_EVENT_HEADER Header;
    EFI_TCM_PCR_EVENT_DATA   Event;
} EFI_TCM_PCR_EVENT;

#pragma pack (pop)

#endif  // _TPMPC_H_
