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
/** @file
  AmiAmlLib library instance. Collection of AML manipulation/genaration routines.
**/

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AmiAmlLib.h>
#include <Library/BaseMemoryLib.h>
#include <IndustryStandard/Acpi60.h>
#include <Library/BaseLib.h>
#include <Token.h>

#define AMI_ACPI_TABLE_VERSION_XSDT (EFI_ACPI_TABLE_VERSION_2_0 | EFI_ACPI_TABLE_VERSION_3_0 | EFI_ACPI_TABLE_VERSION_4_0 | EFI_ACPI_TABLE_VERSION_5_0 | (1 << 6))

//AML Name Object Size Preffix
#define AMI_AML_PR_BYTE     0x0A
#define AMI_AML_PR_WORD     0x0B
#define AMI_AML_PR_DWORD    0x0C
#define AMI_AML_PR_STRING   0x0D
#define AMI_AML_PR_QWORD    0x0E

#define AMI_AML_NULL_NAME   0x00

#define AMI_ASLR_START_DEP_FUNC_NO_PRI  ACPI_SMALL_RESOURCE_HEADER

#define AMI_AML_PR_EXTOP    0x5B    //'['
#define AMI_AML_PR_ROOT     0x5C    //'\'
#define AMI_AML_PR_PARENT   0x5E    //'^'
//==============================================================
//AML Definition for AML Objects NameString
//==============================================================
#pragma pack(push, 1)

//--------------------------------------------------
//LeadNameChar := 'A'-'Z' | '_' := 0x5f
//DigitChar := '0'-'9'
//NameChar := DigitChar | LeadNameChar
//RootChar := '\'=0x5c
//ParentPrefixChar := '^'=0x5e
//NullName := 0x00
//--------------------------------------------------

//--------------------------------------------------
//NameSeg := <LeadNameChar NameChar NameChar NameChar>
// Notice that NameSegs shorter than 4 characters are
// filled with trailing '_'s.
typedef union _AMI_AML_NAME_SEG{
    UINT32      Name32;
    UINT8       Name[4];
}AMI_AML_NAME_SEG;

//Generic ASL resource header
typedef union _AMI_ASLR_HDR{
    ACPI_SMALL_RESOURCE_HEADER      SHdr;
    ACPI_LARGE_RESOURCE_HEADER      LHdr;
} AMI_ASLR_HDR;

typedef union _AMI_ASLRF_PFLAGS{
    UINT8   PrFlags;
    struct {
        UINT8   Cmp : 2;
        UINT8   Prf : 2;
        UINT8   Reserved: 4;
    } Bits;
}AMI_ASLRF_PFLAGS;

typedef struct _AMI_ASLR_START_DEPENDENT_FUNC{
    ACPI_SMALL_RESOURCE_HEADER          Hdr;
    AMI_ASLRF_PFLAGS                    Priority;       //Priority
}AMI_ASLR_START_DEPENDENT_FUNC;

typedef struct _AMI_ASLR_END_TAG {
    ACPI_SMALL_RESOURCE_HEADER      Hdr;
    UINT8                           Chsum;
} AMI_ASLR_END_TAG;

#pragma pack(pop)

/**
    Generates a short IRQ descriptor and returns its address.
    User is responsible for freeing the returned memory.

    @param NumberOfIrqs Number of IRQ numbers in the variable argument list.
    @param Active IRQ flag.  0 = active-high, 1 = active-low.
    @param Trigger IRQ flag.  0 = level-triggered, 1 = edge-triggered.
    @param Shareable IRQ flag.  0 = not interrupt shareable, 1 = interrupt shareable.
    @param ... List of IRQ numbers.  Valid values: 0-15.

    @return Address of the created IRQ descriptor.
            User is responsible for freeing the returned memory.
**/
EFI_ACPI_IRQ_DESCRIPTOR* EFIAPI AmiBuildIrqDesc(
    UINT8 NumberOfIrqs, UINT8 Active, UINT8 Trigger, UINT8 Shareable, ...
){
    //TODO: input parameter validation
    UINT8 i;
    UINT16 Irq=0;
    VA_LIST Marker;
    EFI_ACPI_IRQ_DESCRIPTOR *Desc = AllocateZeroPool(sizeof(EFI_ACPI_IRQ_DESCRIPTOR));

    if (!Desc) return NULL;

    //Update descriptor Header
    Desc->Header.Bits.Length = sizeof(EFI_ACPI_IRQ_DESCRIPTOR)-sizeof(ACPI_SMALL_RESOURCE_HEADER); // 3
    Desc->Header.Bits.Name = ACPI_SMALL_IRQ_DESCRIPTOR_NAME;
    Desc->Header.Bits.Type = ACPI_SMALL_ITEM_FLAG;

    //Update descriptor Body
    Desc->Information = (Trigger << 0) | (Active << 3) | (Shareable << 4);

    VA_START(Marker, Shareable);

    for (i=0; i<NumberOfIrqs; i++)
    {
        Irq |= (1<<VA_ARG(Marker, UINT8));
    }

    VA_END(Marker);
    Desc->Mask=Irq;
    return Desc;
}

/**
    Generates a short IRQ descriptor without an optional IRQ information byte and returns its
    address.  User is responsible for freeing the returned memory.

    @param DscCnt Number of IRQ numbers in the list.
    @param ... List of IRQ numbers.  Valid values: 0-15.

    @retval  VOID* address of the created IRQ descriptor.  User is responsible for freeing the returned memory.

**/
EFI_ACPI_IRQ_NOFLAG_DESCRIPTOR* EFIAPI AmiBuildIrqDescNoFlags(UINT8 NumberOfIrqs, ... )
{
    UINT8                           i;
    VA_LIST                         Marker;
    UINT16                          Irq=0;
    UINT32                          Sz=sizeof(EFI_ACPI_IRQ_NOFLAG_DESCRIPTOR);
    EFI_ACPI_IRQ_NOFLAG_DESCRIPTOR  *Descriptor=(EFI_ACPI_IRQ_NOFLAG_DESCRIPTOR*)AllocateZeroPool(Sz);
//------------------------------

    if (!Descriptor)return NULL;


    //Update descriptor Header
    Descriptor->Header.Bits.Length=2;//sizeof(EFI_ACPI_IRQ_NOFLAG_DESCRIPTOR)-sizeof(ACPI_SMALL_RESOURCE_HEADER);
    Descriptor->Header.Bits.Name=ACPI_SMALL_IRQ_DESCRIPTOR_NAME;
    Descriptor->Header.Bits.Type=ACPI_SMALL_ITEM_FLAG;

    VA_START(Marker, NumberOfIrqs);

    for (i=0; i<NumberOfIrqs; i++)
    {
        Irq|=(1<<VA_ARG(Marker, UINT8));
    }

    VA_END(Marker);
    Descriptor->Mask=Irq;
    return Descriptor;
}

/**
    Generates a short DMA descriptor and returns its address.  User is responsible for freeing the returned memory.

    @param DscCnt Number of channel numbers in the list.
    @param Type DMA channel speed.  Valid values are: EFI_ACPI_DMA_SPEED_TYPE_COMPATIBILITY, EFI_ACPI_DMA_SPEED_TYPE_A,
            EFI_ACPI_DMA_SPEED_TYPE_B, EFI_ACPI_DMA_SPEED_TYPE_F
    @param Bm - Bus Master. Valid values are: EFI_ACPI_DMA_BUS_MASTER  if BusMaster or 0 if NotBusMaster.
    @param Size Transfer size.  Valid values are:  EFI_ACPI_DMA_TRANSFER_TYPE_8_BIT, EFI_ACPI_DMA_TRANSFER_TYPE_8_BIT_AND_16_BIT,
            EFI_ACPI_DMA_TRANSFER_TYPE_16_BIT
    @param ... List of channel numbers.  Valid values are 0-7.

    @retval  VOID* address of the created DMA descriptor.  User is responsible for freeing the returned memory.

**/

EFI_ACPI_DMA_DESCRIPTOR* EFIAPI AmiBuildDmaDesc(UINT8 NumberOfChannels, UINT8 Type, UINT8 Bm, UINT8 Size, ... )
{
    UINT8                       i, Dma=0;
    VA_LIST                     Marker;
    UINT32                      Sz=sizeof(EFI_ACPI_DMA_DESCRIPTOR);
    EFI_ACPI_DMA_DESCRIPTOR     *Descriptor=(EFI_ACPI_DMA_DESCRIPTOR*)AllocateZeroPool(Sz);
//------------------------------

    if (!Descriptor)return NULL;

    //Update descriptor Header
    Descriptor->Header.Bits.Length=sizeof(EFI_ACPI_DMA_DESCRIPTOR)-sizeof(ACPI_SMALL_RESOURCE_HEADER);
    Descriptor->Header.Bits.Name=ACPI_SMALL_DMA_DESCRIPTOR_NAME;
    Descriptor->Header.Bits.Type=ACPI_SMALL_ITEM_FLAG;

    //Update descriptor Body
    Descriptor->Information = Type | Bm | Size;

    VA_START(Marker, Size);

    for (i=0; i<NumberOfChannels; i++)
    {
        Dma|=(1<<VA_ARG(Marker, UINT8));
    }

    VA_END(Marker);
    Descriptor->ChannelMask=Dma;

    return Descriptor;
}

/**
     Generates a Start-Dependent Function descriptor with the optional priority byte and returns its address.  User is
     responsible for freeing the returned memory.

    @param DscCnt Number of items in the descriptor list.
    @param Comp Compatibility priority.  Valid values are 0-2.
    @param Perf Performance/robustness priority.  Valid values are 0-2.
    @param ... List of descriptors for this dependent function.

    @retval  VOID* address of the created Start-Dependent Function descriptor.  User is responsible for freeing the returned memory.

**/
AMI_AML_DEP_FUNC* EFIAPI AmiBuildStartDepFnDesc(UINT8 DscCnt,UINT8 Comp,UINT8 Perf, ...)
{
    UINT8                   i;
    VA_LIST                 Marker;
    UINT32                  Sz=sizeof(AMI_AML_DEP_FUNC)+sizeof(AMI_ASLR_START_DEPENDENT_FUNC);
    AMI_AML_DEP_FUNC   *Descriptor=(AMI_AML_DEP_FUNC*)AllocateZeroPool(Sz);

//--------------------------------------------
    if (!Descriptor)return NULL;

    Descriptor->DepFn=(Descriptor+1);
    ((AMI_ASLR_START_DEPENDENT_FUNC*)Descriptor->DepFn)->Hdr.Bits.Length=sizeof(AMI_ASLR_START_DEPENDENT_FUNC)-sizeof(ACPI_SMALL_RESOURCE_HEADER);
    ((AMI_ASLR_START_DEPENDENT_FUNC*)Descriptor->DepFn)->Hdr.Bits.Name=ACPI_SMALL_START_DEPENDENT_DESCRIPTOR_NAME;
    ((AMI_ASLR_START_DEPENDENT_FUNC*)Descriptor->DepFn)->Hdr.Bits.Type=ACPI_SMALL_ITEM_FLAG;
    ((AMI_ASLR_START_DEPENDENT_FUNC*)Descriptor->DepFn)->Priority.Bits.Cmp=Comp;
    ((AMI_ASLR_START_DEPENDENT_FUNC*)Descriptor->DepFn)->Priority.Bits.Prf=Perf;

    Descriptor->DepRes.Items=AllocateZeroPool(sizeof(AMI_ASLR_HDR*)*DscCnt);

    if (!Descriptor->DepRes.Items) return NULL;

    Descriptor->DepRes.InitialCount=DscCnt;
    Descriptor->DepRes.ItemCount=DscCnt;
    VA_START(Marker, Perf);

    for (i=0; i<DscCnt; i++)
        Descriptor->DepRes.Items[i]=VA_ARG(Marker, VOID*);

    return Descriptor;
}

/**
     Generates a Start-Dependent Function descriptor without the optional priority byte and returns its address.
     User is responsible for freeing the returned memory.

    @param DscCnt Number of items in the descriptor list.
    @param ... List of descriptors for this dependent function.

    @retval VOID* address of the created Start-Dependent Function descriptor.  User is responsible for freeing the returned memory.

**/
AMI_AML_DEP_FUNC* EFIAPI AmiBuildStartDepFnDescNoPri(UINT8 DscCnt, ...)
{
    UINT8                       i;
    VA_LIST                     Marker;
    UINT32                      Sz=sizeof(AMI_AML_DEP_FUNC)+sizeof(AMI_ASLR_START_DEP_FUNC_NO_PRI);
    AMI_AML_DEP_FUNC       *Descriptor=(AMI_AML_DEP_FUNC*)AllocateZeroPool(Sz);

//--------------------------------------------
    if (!Descriptor)return NULL;

    Descriptor->DepFn=(Descriptor+1);
    ((AMI_ASLR_START_DEP_FUNC_NO_PRI*)Descriptor->DepFn)->Bits.Length=sizeof(AMI_ASLR_START_DEP_FUNC_NO_PRI)-sizeof(ACPI_SMALL_RESOURCE_HEADER);
    ((AMI_ASLR_START_DEP_FUNC_NO_PRI*)Descriptor->DepFn)->Bits.Name=ACPI_SMALL_START_DEPENDENT_DESCRIPTOR_NAME;
    ((AMI_ASLR_START_DEP_FUNC_NO_PRI*)Descriptor->DepFn)->Bits.Type=ACPI_SMALL_ITEM_FLAG;

    Descriptor->DepRes.Items=AllocateZeroPool(sizeof(AMI_ASLR_HDR*)*DscCnt);

    if (!Descriptor->DepRes.Items) return NULL;

    Descriptor->DepRes.InitialCount=DscCnt;
    Descriptor->DepRes.ItemCount=DscCnt;

    VA_START(Marker, DscCnt);

    for (i=0; i<DscCnt; i++) Descriptor->DepRes.Items[i]=VA_ARG(Marker, VOID*);

    return Descriptor;
}

/**
    Generates a short I/O descriptor and returns its address. User is responsible for freeing the returned memory.

    @param Dec Decode value.  Valid values are: ASLV_Decode16, ASLV_Decode10.
    @param Min Address minimum.
    @param Max Address maximum.
    @param Aln Base alignment.
    @param Len Range length.

    @retval  VOID* address of the created I/O descriptor.  User is responsible for freeing the returned memory.

**/
EFI_ACPI_IO_PORT_DESCRIPTOR* EFIAPI AmiBuildIoDesc(UINT8 Dec,UINT16 Min,UINT16 Max,UINT8 Aln,UINT8 Len)
{
    EFI_ACPI_IO_PORT_DESCRIPTOR         *Descriptor=(EFI_ACPI_IO_PORT_DESCRIPTOR*)AllocateZeroPool(sizeof(EFI_ACPI_IO_PORT_DESCRIPTOR));

//------------------------------------------
    if (!Descriptor)return NULL;

    Descriptor->Header.Bits.Length=sizeof(EFI_ACPI_IO_PORT_DESCRIPTOR)-sizeof(ACPI_SMALL_RESOURCE_HEADER);
    Descriptor->Header.Bits.Name=ACPI_SMALL_IO_PORT_DESCRIPTOR_NAME;
    Descriptor->Header.Bits.Type=ACPI_SMALL_ITEM_FLAG;

    Descriptor->Information=Dec;
    Descriptor->BaseAddressMin=Min;
    Descriptor->BaseAddressMax=Max;
    Descriptor->Alignment=Aln;
    Descriptor->Length=Len;

    return Descriptor;
}

/**
    Generates a short fixed I/O descriptor and returns its address.  User is responible for freeing the returned memory.
    @param  Bas Address base.
    @param Len Range length.

    @retval VOID* address of the created fixed I/O descriptor.  User is responsible for freeing the returned memory.

**/
EFI_ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR* EFIAPI AmiBuildFixedIoDesc(UINT16 Bas, UINT8 Len)
{
    EFI_ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR    *Descriptor=(EFI_ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR*)AllocateZeroPool(sizeof(EFI_ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR));

//------------------------------------------
    if (!Descriptor)return NULL;

    Descriptor->Header.Bits.Length=sizeof(EFI_ACPI_FIXED_LOCATION_IO_PORT_DESCRIPTOR)-sizeof(ACPI_SMALL_RESOURCE_HEADER);
    Descriptor->Header.Bits.Name=ACPI_SMALL_FIXED_IO_PORT_DESCRIPTOR_NAME;
    Descriptor->Header.Bits.Type=ACPI_SMALL_ITEM_FLAG;

    Descriptor->BaseAddress=Bas;
    Descriptor->Length=Len;

    return Descriptor;
}

/**
    Generates a long 24-bit memory descriptor and returns its address.  User is responsible for freeing the returned memory.
    @param Rw  Write protection flag.  Valid values: EFI_ACPI_MEMORY_RESOURCE_SPECIFIC_FLAG_READ_ONLY,
            EFI_ACPI_MEMORY_RESOURCE_SPECIFIC_FLAG_READ_WRITE.
    @param Min Minimum base memory address [23:8].
    @param Max Maximum base memory address [23:8].
    @param Aln Base alignment.
    @param Len Range length.

    @retval  VOID* address of the created long 24-bit memory descriptor.  User is responsible for freeing the returned memory.

**/
EFI_ACPI_24_BIT_MEMORY_RANGE_DESCRIPTOR* EFIAPI AmiBuildMemory24Desc(UINT8 Rw, UINT8 Min, UINT8 Max, UINT8 Aln, UINT8 Len)
{
    EFI_ACPI_24_BIT_MEMORY_RANGE_DESCRIPTOR   *Descriptor=(EFI_ACPI_24_BIT_MEMORY_RANGE_DESCRIPTOR*)AllocateZeroPool(sizeof(EFI_ACPI_24_BIT_MEMORY_RANGE_DESCRIPTOR));

//------------------------------------------
    if (!Descriptor)return NULL;

    Descriptor->Header.Length=sizeof(EFI_ACPI_24_BIT_MEMORY_RANGE_DESCRIPTOR)-sizeof(ACPI_SMALL_RESOURCE_HEADER);
    Descriptor->Header.Header.Bits.Name=ACPI_LARGE_24_BIT_MEMORY_RANGE_DESCRIPTOR_NAME;
    Descriptor->Header.Header.Bits.Type=ACPI_SMALL_ITEM_FLAG;

    Descriptor->Information=Rw;
    Descriptor->BaseAddressMin=Min;
    Descriptor->BaseAddressMax=Max;
    Descriptor->Alignment=Aln;
    Descriptor->Length=Len;

    return Descriptor;

}

/**
    Validates a ACPI_LARGE_ITEM_FLAG descriptor.

    @param Descriptor Descriptor to be validated.
    @param ResType The resource type of the descriptor.  Valid values are: AmiAmlResourseAll, AmiAmlResourseIoMem, AmiAmlResourseBus
    @param Fixed If TRUE, checks the general flags for consistancy.

    @retval  TRUE if a valid EFI_ACPI_QWORD_ADDRESS_SPACE_DESCRIPTOR descriptor.  Otherwise, FALSE.

**/
STATIC BOOLEAN LocalValidateDescriptor(EFI_ACPI_QWORD_ADDRESS_SPACE_DESCRIPTOR *Descriptor,AMI_AML_RESOURSE_TYPE ResType,BOOLEAN Fixed)
{
    //Check ASL Resource Descriptor Header consistancy
    if (
        (Descriptor->Header.Header.Bits.Name!=ACPI_LARGE_QWORD_ADDRESS_SPACE_DESCRIPTOR_NAME)   ||
        (Descriptor->Header.Header.Bits.Type!=ACPI_LARGE_ITEM_FLAG)      ||
        (Descriptor->Header.Length!=sizeof(EFI_ACPI_QWORD_ADDRESS_SPACE_DESCRIPTOR)-sizeof(ACPI_LARGE_RESOURCE_HEADER) )
    )return FALSE;

    //Check ASL Resource Descriptor General Flags Consistancy
    if (Fixed)
    {
        if ((Descriptor->GenFlag & (BIT2 | BIT3)) != (BIT2 | BIT3))
            return FALSE;
    }

    if (Descriptor->ResType>2) return FALSE;

    //Check ASL Resource Descriptor Resource Type
    switch (ResType)
    {
        case AmiAmlResourseBus:

            if (Descriptor->ResType!=ACPI_ADDRESS_SPACE_TYPE_BUS) return FALSE;

            if ((Descriptor->AddrRangeMin+Descriptor->AddrLen)>0x100)return FALSE;

            if ((Descriptor->AddrRangeMax - Descriptor->AddrRangeMin +1)!= Descriptor->AddrLen ) return FALSE;

            break;
        case AmiAmlResourseIoMem:

            if (Descriptor->ResType>=ACPI_ADDRESS_SPACE_TYPE_BUS) return FALSE;

            break;
        case AmiAmlResourseAll:

            if (Descriptor->ResType>ACPI_ADDRESS_SPACE_TYPE_BUS) return FALSE;

            break;
        default : return FALSE;
    }

    return TRUE;
}

/**
    Validates the consistency of the provided ACPI QWORD resource descriptors block which contains some
    number of EFI_ACPI_QWORD_ADDRESS_SPACE_DESCRIPTORs in memory terminated by an AMI_ASLR_END_TAG.  Returns the total size of the resource
    descriptors plus the size of the AMI_ASLR_END_TAG.

    @param DscStart Pointer to the first descriptor in the block to be validated.
    @param ResType The resource type of the descriptors.  Valid values are:  AmiAmlResourseAll, AmiAmlResourseIoMem, AmiAmlResourseBus
    @param Fixed If TRUE, checks the general flags for consistency.

    @retval  UINTN size of the resource descriptor block plus the size of the AMI_ASLR_END_TAG.

**/
UINTN EFIAPI AmiGetAcpiDescriptorBlockLength(EFI_ACPI_QWORD_ADDRESS_SPACE_DESCRIPTOR* DscStart, AMI_AML_RESOURSE_TYPE ResType, BOOLEAN Fixed)
{
    EFI_ACPI_QWORD_ADDRESS_SPACE_DESCRIPTOR         *Qwd=DscStart;
    UINTN                                           Len=0;
    AMI_ASLR_END_TAG                                *EndTag=(AMI_ASLR_END_TAG*)DscStart;

//-----------------------------------
    //resource descriptor is empty...
    if (EndTag->Hdr.Byte==ACPI_END_TAG_DESCRIPTOR) return sizeof(AMI_ASLR_END_TAG);

    while (EndTag->Hdr.Byte!=ACPI_END_TAG_DESCRIPTOR)
    {
        if (!LocalValidateDescriptor(Qwd, ResType, Fixed)) return 0;
        else Len+=sizeof(EFI_ACPI_QWORD_ADDRESS_SPACE_DESCRIPTOR);

        Qwd++;
        EndTag=(AMI_ASLR_END_TAG*)Qwd;
    }

    Len+=sizeof(AMI_ASLR_END_TAG);
    return Len;
}

/**
    Checks the signature of the provided ACPI header for the DSDT signature.

    @param Dsdt Pointer to the ACPI header to be checked for the DSDT signature.

    @retval  TRUE if the header signature is that of the DSDT.  Otherwise, FALSE.

**/
STATIC BOOLEAN LocalCheckDsdt(EFI_ACPI_SDT_HEADER *Dsdt)
{
    if (Dsdt->Signature!=EFI_ACPI_1_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) return FALSE;

    return TRUE;
}


/**
    Determines the length in bytes of an AML package.

    @param Pck  AML package to be measured.
    @param BytesCnt The ByteCount value of the package.

    @retval  UINTN size of the AML package in bytes.

**/
UINTN EFIAPI AmiGetAmlPackageLength(AMI_AML_PLEN_BYTE *Pck, UINT8 *BytesCnt)
{
    UINTN   Len=0;
    UINT8   *Pointer=(UINT8*)Pck;

//------------------
    if (BytesCnt)*BytesCnt=0;

    if (!Pck->ByteCount) Len=Pck->Length;
    else
    {
        Len|=(Pck->Length&0x0F);

        if (BytesCnt)*BytesCnt=Pck->ByteCount;

        switch (Pck->ByteCount)
        {
            case 1:
                {
                    UINT8   b=*(Pointer+1);
                    Len|=(b<<4);
                } break;
            case 2:
                {
                    UINT16 w=(UINT16)(*((UINT16*)(Pointer+1)));
                    Len|=(w<<4);
                } break;
            case 3:
                {
                    UINT32 d=(UINT32)(*((UINT32*)(Pointer+1)));
                    d&=0xffffff;
                    Len|=(d<<4);
                } break;
        } //switch
    }

    return Len;
}

/**
    Converts an ASL name into a UINT32 value and returns it.

    @param AslName  Pointer to the ASL name string.

    @retval  UINT32 translation of the ASL name.
**/
STATIC UINT32 LocalConvertAslName(CHAR8 *AslName)
{
    AMI_AML_NAME_SEG    Name;
    UINTN               Length=AsciiStrnLenS(AslName, 4);
//-------------------------
    Name.Name32=0x5F5F5F5F;

    CopyMem(&Name.Name[0],AslName,Length);
    return Name.Name32;
}

/**
    Finds an ASL object by its name string and returns its location.

    @param Start Location to begin scanning for the ASL object.
    @param ObjName Name of the ASL object to be located.
    @param Len Maximum length of bytes to scan for the ASL object.

    @retval pointer to the start of the ASL name string in memory.

**/
VOID* EFIAPI AmiFindAmlObjectByName(UINT8 *Start, CHAR8 *ObjName, UINTN Len)
{
    UINTN               i;
    UINT8               *Pointer;
    AMI_AML_NAME_SEG    *Obj,Nm;
//------------------------------
    Nm.Name32=LocalConvertAslName(ObjName);

    Pointer=Start;

    for (i=0; i<Len; i++)
    {
        Obj=(AMI_AML_NAME_SEG*)(&Pointer[i]);

        if (Obj->Name32!=Nm.Name32)continue;

        return &Pointer[i];
    }

    return NULL;
}
/**
    Destroys Method passed as MethodName parameter if it is present in ASL object
    described by AslObj parameter

    @param AslObj  Pointer to AMI_AML_OBJECT_INFO structure, which discribes ASL object to modify
    @param MethodName Name of the ASL Method to be destroyed.

**/
EFI_STATUS EFIAPI AmiHideAmlMethodFromOs (AMI_AML_OBJECT_INFO *AslObj, CHAR8 *MethodName)
{
    UINT32  AslMethodName = LocalConvertAslName(MethodName), i, Position; //*(UINT32*)MethodName
    UINT8   *Start = AslObj->DataStart;

    for (i = 0; i < AslObj->Length; i++)
    {
        Position =*(UINT32*)(Start+i);

        if (Position == AslMethodName)
            // If method is found
        {
            Start[i] = 'X'; //Replace leading _ with X
            return EFI_SUCCESS;
        }
    }
    return EFI_NOT_FOUND;
}


/**

    Checks an ASL object for the user provided op code and extended op code and returns the op code
    location in a user allocated AMI_AML_OBJECT_INFO structure.

    @param NameStart Pointer to the start of the ASL object's name.
    @param OpCode Op code to be checked.
    @param ExtOpCode  Extended op code to be checked.  If set as 0, only checks for OpCode.
    @param ObjInfo  Pointer to a user allocated AMI_AML_OBJECT_INFO structure which will be updated
    with the associated information if the op codes are located.  If an
    extended op code is specified, ObjInfo->Object is filled with the location
    of the extended op code; otherwise ObjInfo->Object refers to the location of the op code.

    @retval TRUE if the op code and extended op code (if specified) are located;
    otherwise, FALSE.

**/

STATIC BOOLEAN LocalCheckOpCode(UINT8 *NameStart, UINT8 OpCode, UINT8 ExtOpCode, AMI_AML_OBJECT_INFO *ObjInfo)
{
    UINT8   *Pointer, *Pointer1;
    INTN    i;

//---------------------
    //Maximum number of bytes in PackageLength is 4
    if ( (*(NameStart-1))==AMI_AML_PR_ROOT || (*(NameStart-1))==AMI_AML_PR_PARENT) NameStart--;

    //if we Looking For ASL Name Object its a special case.
    if (OpCode==AML_NAME_OP && !ExtOpCode)
    {
        Pointer=NameStart-1;

        if (*Pointer==OpCode)
        {
            ObjInfo->Object=Pointer;
            ObjInfo->ObjName=NameStart;
            ObjInfo->Length=0;
            return TRUE;
        }

        return FALSE;
    }

    //if we Looking For ASL Name Object its a special case.
    if (OpCode==AML_EXT_REGION_OP && ExtOpCode)
    {
        Pointer=NameStart-2;

        if (*Pointer==ExtOpCode && *(Pointer+1)==OpCode)
        {
            ObjInfo->Object=Pointer;
            ObjInfo->ObjName=NameStart;
            ObjInfo->Length=0;
            return TRUE;
        }

        return FALSE;
    }

    for (i=2; i<6; i++)
    {
        Pointer=NameStart-i;

        if (ExtOpCode) Pointer1=Pointer-1;
        else Pointer1=NULL;

        if (Pointer1)if (*Pointer1!=ExtOpCode) continue;

        if (*Pointer!=OpCode) continue;

        if (Pointer1)ObjInfo->Object=Pointer1;
        else ObjInfo->Object=Pointer;

        ObjInfo->Length=AmiGetAmlPackageLength((AMI_AML_PLEN_BYTE*)(Pointer+1),NULL);
        ObjInfo->ObjName=NameStart;
        return TRUE;
    }

    return FALSE;
}

/**
    Getting Asl Obj

    @param Start Starting location in memory to begin scanning for the desired ASL object.
    @param Length Maximum number of bytes in memory to be scanned starting from Start for the ASL object.
    @param Name Pointer to the name of the ASL object to be located.
    @param ObjType Type of the ASL object to be located.  Supports: otScope, otName, otProc, otTermal, otDevice, otMethod, otPwrRes, otOpReg
    @param ObjInfo Returns completed AMI_AML_OBJECT_INFO structure, if the ASL object is found.

    @retval EFI_NOT_FOUND,  if object not found or invalid type.
        Otherwise, EFI_SUCCESS.
**/

//Since the search is done by locating the OBJECT name.
//Caller must know what ASL Object Type he is looking for.
EFI_STATUS EFIAPI AmiGetAmlObject(UINT8 *Start, UINTN Length, CHAR8 *Name, AMI_AML_OBJECT_TYPE ObjType, AMI_AML_OBJECT_INFO *ObjInfo)
{
    UINT8           *Pn, *St, Eop, Op;
    UINTN           Len, i;
    BOOLEAN         Objfound=FALSE;

    St=Start;

    Len=Length;

    while (!Objfound)
    {

        Pn=AmiFindAmlObjectByName(St,Name,Len);

        if (!Pn) return EFI_NOT_FOUND;

        //If we found the name let's check if it is the Object we are looking for
        //it could be just object reference, or field definition inside the object,
        //or double name, or multiple name definition we must filter such situations
        // and make sure we have got the actual object but not its reference instance
        switch (ObjType)
        {
            case    AmiAmlObjTypeScope     :
                i=0;
                Eop=0;
                Op=AML_SCOPE_OP;
                break;
            case    AmiAmlObjTypeName      :
                i=0;
                Eop=0;
                Op=AML_NAME_OP;
                break;
            case    AmiAmlObjTypeProc      :
                i=6;
                Eop=AMI_AML_PR_EXTOP;
                Op=AML_EXT_PROCESSOR_OP;
                break;
            case    AmiAmlObjTypeTermal    :
                i=0;
                Eop=AMI_AML_PR_EXTOP;
                Op=AML_EXT_THERMAL_ZONE_OP;
                break;
            case    AmiAmlObjTypeDevice    :
                i=0;
                Eop=AMI_AML_PR_EXTOP;
                Op=AML_EXT_DEVICE_OP;
                break;
            case    AmiAmlObjTypeMethod        :
                i=1;
                Eop=0;
                Op=AML_METHOD_OP;
                break;
            case    AmiAmlObjTypePwrRes    :
                i=3;
                Eop=AMI_AML_PR_EXTOP;
                Op=AML_EXT_POWER_RES_OP;
                break;
            case    AmiAmlObjTypeOpReg :
                i=0;
                Eop=AMI_AML_PR_EXTOP;
                Op=AML_EXT_REGION_OP;
                break;

            default: return EFI_NOT_FOUND;
        } //switch

        Objfound=LocalCheckOpCode(Pn, Op, Eop, ObjInfo);

        if (!Objfound)
        {
            Len-=(UINTN)(Pn-St)-4;
            St=Pn+4;
        }
    } //while ()

    ObjInfo->ObjType=ObjType;
    ObjInfo->DataStart=Pn+i+4;

    return EFI_SUCCESS;
}


/**
    Locates an ASL object by its name, ObjName, in Memory Buffer specified by BufferStart, and BufferLen
    and updates its data start with the user provided Value.

    @param BufferStart Pointer to the DSDT.
    @param BufferLen Length of the Buffer
    @param ObjName Name of the ASL object to be updated.
    @param Value New value for the ASL object's data entry.

    @retval EFI_INVALID_PARAMETER,  if the DSDT pointer, ObjName or Value are invalid.
        EFI_NOT_FOUND,     if object specified by ObjName not present.
        Otherwise, EFI_SUCCESS.
**/

STATIC EFI_STATUS  LocalUpdateAslNameBuffer(UINT8 *BufferStart, UINTN BufferLen, CHAR8 *ObjName, UINT64 Value)
{
    EFI_STATUS          Status;
    AMI_AML_OBJECT_INFO        Obj;

    Status=AmiGetAmlObject(BufferStart,BufferLen, ObjName, AmiAmlObjTypeName, &Obj);

    if (EFI_ERROR(Status)) return Status;

    switch (*((UINT8*)Obj.DataStart))
    {
        case AMI_AML_PR_BYTE:
            {
                UINT8   *p = (UINT8*)((UINT8*)Obj.DataStart+1);
                //----------------------------------
                *p=(UINT8)Value;
            } break;

        case AMI_AML_PR_WORD:
            {
                UINT16  *p = (UINT16*)((UINT8*)Obj.DataStart+1);
                //----------------------------------
                *p=(UINT16)Value;
            } break;

        case AMI_AML_PR_DWORD:
            {
                UINT32  *p = (UINT32*)((UINT8*)Obj.DataStart+1);
                //----------------------------------
                *p=(UINT32)Value;
            } break;

        case AMI_AML_PR_QWORD:
            {
                UINT64  *p = (UINT64*)((UINT8*)Obj.DataStart+1);
                //----------------------------------
                *p=(UINT64)Value;
            } break;

        case AML_ZERO_OP: case AML_ONE_OP:
            {
                if ((Value == 0) || (Value == 1))
                {
                    UINT8 *p = (UINT8*)((UINT8*)Obj.DataStart);
                    *p = (UINT8)Value;
                }

                else
                {
                    return EFI_INVALID_PARAMETER;
                }
            } break;
        default:
            {
                return EFI_INVALID_PARAMETER;
            }
    }

    return EFI_SUCCESS;
}

/**
     Locates an ASL object by its name, ObjName within the scope of device DeviceName and updates its data with the provided Value.

     @param Sdt Pointer to the DSDT or SSDT.
     @param DeviceName Name of the ASL Device object having in it's scope Name Object to be updated.
     @param ObjName Name of the ASL Name Object to be updated.
     @param Value New value for the ASL object's data entry.

     @retval EFI_INVALID_PARAMETER,  if the DSDT pointer, ObjName or Value are invalid.
        EFI_NOT_FOUND,  if object specified by Device Name Or ObjName not present.
        Otherwise, EFI_SUCCESS.
**/
EFI_STATUS EFIAPI AmiUpdateAmlObjectValueInDeviceScope(
    EFI_ACPI_SDT_HEADER *Sdt, CHAR8 *DeviceName, CHAR8 *ObjName, UINT64 Value
){
    EFI_STATUS          Status;
    AMI_AML_OBJECT_INFO        Obj;

    if (!LocalCheckDsdt(Sdt)) return EFI_INVALID_PARAMETER;

    Status=AmiGetAmlObject((UINT8*)(Sdt+1),Sdt->Length-sizeof(EFI_ACPI_SDT_HEADER)-1, DeviceName, AmiAmlObjTypeDevice, &Obj);

    if (EFI_ERROR(Status)) return Status;

    Status=LocalUpdateAslNameBuffer((UINT8*)Obj.DataStart, Obj.Length, ObjName, Value);
    return Status;
}

/**
     Locates an ASL object by its name, ObjName in the DSDT or SSDT and updates its data with the provided Value.

     @param Sdt Pointer to the DSDT or SSDT.
     @param ObjName Name of the ASL object to be updated.
     @param Value New value for the ASL object's data entry.

     @retval EFI_INVALID_PARAMETER,  if the DSDT pointer, ObjName or Value are invalid.
        Otherwise, EFI_SUCCESS.
**/

EFI_STATUS EFIAPI AmiUpdateAmlObjectValue(EFI_ACPI_SDT_HEADER *Sdt, CHAR8 *ObjName, UINT64 Value)
{
    if (!LocalCheckDsdt(Sdt)) return EFI_INVALID_PARAMETER;

    return LocalUpdateAslNameBuffer((UINT8*)(Sdt+1),Sdt->Length-sizeof(EFI_ACPI_SDT_HEADER)-1, ObjName, Value);
}

/**
 Retrieves a pointer to the DSDT ACPI table for the user requested Version of ACPI.


 @param  DsdtAddr On output address of the DSDT associated with the user provided Version.
 @param Version Which version of ACPI's DSDT to return.  Valid values include:



  @retval EFI_NOT_AVAILABLE_YET,  if ACPI support protocol not installed yet.
  Otherwise based on result.

**/
EFI_STATUS EFIAPI AmiGetDsdt(EFI_ACPI_SDT_HEADER **Dsdt, EFI_ACPI_TABLE_VERSION Version)
{
    EFI_PHYSICAL_ADDRESS        Dsdt1 = 0;
    EFI_PHYSICAL_ADDRESS        Dsdt2 = 0;
    EFI_ACPI_SDT_HEADER         *Table = NULL;
    EFI_ACPI_SDT_PROTOCOL       *AcpiSdt;

    EFI_STATUS                  Status = EFI_NOT_FOUND;
    UINTN                       i;
    EFI_ACPI_TABLE_VERSION      Ver;
    UINTN                       AcpiHandle;

//---------------------------

    if (Dsdt==NULL) return EFI_INVALID_PARAMETER;
    Status=gBS->LocateProtocol(&gEfiAcpiSdtProtocolGuid, NULL, (VOID **)&AcpiSdt);

    if (EFI_ERROR(Status)) {
        return EFI_NOT_AVAILABLE_YET;
    } else {
        for (i=0; ; i++) {
            Status = AcpiSdt->GetAcpiTable(i, &Table, &Ver, &AcpiHandle);

            if (EFI_ERROR(Status)) {
                break;
            }

            if (Table->Signature == EFI_ACPI_1_0_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE) {
                if (Ver == EFI_ACPI_TABLE_VERSION_1_0B) {
                    Dsdt1 = (EFI_PHYSICAL_ADDRESS)(((EFI_ACPI_6_0_FIXED_ACPI_DESCRIPTION_TABLE *)Table)->Dsdt);
                }

                if ((Ver & AMI_ACPI_TABLE_VERSION_XSDT)!= 0) {
                    Dsdt2 = (EFI_PHYSICAL_ADDRESS)(((EFI_ACPI_6_0_FIXED_ACPI_DESCRIPTION_TABLE *)Table)->XDsdt);
                }
            }
#if ACPI_MODULE_VER < 170
            gBS->FreePool((VOID *)Table);
#endif
            if (Dsdt1 && Dsdt2)
                break;
        }
    }

    if (Dsdt2 && ((Version & AMI_ACPI_TABLE_VERSION_XSDT) || (Version & EFI_ACPI_TABLE_VERSION_NONE))) {
        *Dsdt = (EFI_ACPI_SDT_HEADER*)(UINTN)Dsdt2;
        Status = EFI_SUCCESS;
    } else {
        if (Dsdt1 && ((Version & EFI_ACPI_TABLE_VERSION_1_0B) || (Version & EFI_ACPI_TABLE_VERSION_NONE))) {
            *Dsdt = (EFI_ACPI_SDT_HEADER*)(UINTN)Dsdt1;
            Status = EFI_SUCCESS;
        }
    }
    return Status;
}

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
